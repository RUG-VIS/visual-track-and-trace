#include "LGlyphLayer.h"
#include "../commands/SingularSpawnPointCallback.h"
#include <vtkActor2D.h>
#include <vtkGlyph2D.h>
#include <vtkLookupTable.h>
#include <vtkGlyphSource2D.h>
#include <vtkNamedColors.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleUser.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>

#include "../CartographicTransformation.h"
#include "luts.h"

vtkSmartPointer<SpawnPointCallback> LGlyphLayer::createSpawnPointCallback() {
  vtkNew<SingularSpawnPointCallback> newPointCallBack;
  newPointCallBack->setPoints(this->points);
  newPointCallBack->setRen(this->ren);
  newPointCallBack->setUVGrid(this->uvGrid);
  newPointCallBack->setBeached(this->particlesBeached);
  newPointCallBack->setAge(this->particlesAge);
  newPointCallBack->setIdx(this->lutIdx);
  return newPointCallBack;
}


LGlyphLayer::LGlyphLayer(std::shared_ptr<UVGrid> uvGrid, std::unique_ptr<AdvectionKernel> advectionKernel) {
  buildLuts();

  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(2);

  this->points = vtkSmartPointer<vtkPoints>::New();
  vtkNew<vtkPolyData> data;
  data->SetPoints(this->points);

  this->particlesBeached = vtkSmartPointer<vtkIntArray>::New();
  this->particlesAge = vtkSmartPointer<vtkIntArray>::New();

  this->lutIdx = vtkSmartPointer<vtkIntArray>::New();
  this->lutIdx->SetName("lutIdx");
  this->lutIdx->SetNumberOfComponents(0);

  data->GetPointData()->AddArray(this->lutIdx);
  data->GetPointData()->SetActiveScalars("lutIdx");

  advector = std::move(advectionKernel);
  this->uvGrid = uvGrid;

  vtkSmartPointer<vtkTransformFilter> transformFilter = createCartographicTransformFilter(uvGrid);
  transformFilter->SetInputData(data);

  this->glyphSource = vtkSmartPointer<vtkGlyphSource2D>::New();
  this->glyphSource->SetGlyphTypeToCircle();
  this->glyphSource->SetScale(0.02);
  this->glyphSource->Update();

  vtkNew<vtkGlyph2D> glyph2D;
  glyph2D->SetSourceConnection(this->glyphSource->GetOutputPort());
  glyph2D->SetInputConnection(transformFilter->GetOutputPort());
  glyph2D->SetScaleModeToDataScalingOff();
  glyph2D->Update();

  this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->mapper->SetInputConnection(glyph2D->GetOutputPort());
  this->mapper->SetColorModeToMapScalars();

  setColourMode(COMPLEMENTARY);
  this->mapper->UseLookupTableScalarRangeOn();
  this->mapper->Update();
  
  vtkNew<vtkActor> actor;
  actor->SetMapper(this->mapper);

  this->ren->AddActor(actor);

  this->callback = createSpawnPointCallback();
}


void LGlyphLayer::buildLuts() {
  this->tables.push_back(buildComplementary());
  this->tables.push_back(buildComplementary());
  this->tables.push_back(buildComplementary());
  this->tables.push_back(buildDesaturated());

  this->activeColourMode = COMPLEMENTARY;
  this->activeSaturationMode = SATURATED;
}

void LGlyphLayer::spoofPoints() {
  for (int i=0; i < 330; i+=5) {
    for (int j=0; j < 330; j+=5) {
      this->points->InsertNextPoint(-15.875+(12.875+15.875)/330*j, 46.125+(62.625-46.125)/330*i, 0);
      this->particlesBeached->InsertNextValue(0);
      this->particlesAge->InsertNextValue(0);
    }
  }
  this->points->Modified();
}

void LGlyphLayer::updateData(int t) {
  const int SUPERSAMPLINGRATE = 4;
  double point[3], oldX, oldY;
  bool modifiedData = false;

  // iterate over every point.
  for (vtkIdType n=0; n < this->points->GetNumberOfPoints(); n++) {
    // first check: only update points within our grid's boundary.
    this->points->GetPoint(n, point);
    if (point[0] <= this->uvGrid->lonMin() or point[0] >= this->uvGrid->lonMax() or point[1] <= this->uvGrid->latMin() or point[1] >= this->uvGrid->latMax()) {
      // sets any particle out of bounds to be beached - so it gets assigned the right colour in the lookup table.
      this->particlesAge->SetValue(n, -1);
      this->lutIdx->SetValue(n, -1);
      continue;
    }
    
    // update particle age.
    int age = this->particlesAge->GetValue(n);
    if (age >= 0)
      this->particlesAge->SetValue(n, age+1);

    // second check: only update non-beached particles.
    int beachedFor = this->particlesBeached->GetValue(n);
    if (beachedFor < this->beachedAtNumberOfTimes-1) {

      oldX = point[0]; oldY = point[1];

      // supersampling
      for (int i=0; i < SUPERSAMPLINGRATE; i++) {
        std::tie(point[1], point[0]) = advector->advect(t, point[1], point[0], this->dt/SUPERSAMPLINGRATE);
      }

      // if the particle's location remains unchanged, increase beachedFor number. Else, decrease it and update point position.
      if (oldX == point[0] and oldY == point[1]) {
        this->particlesBeached->SetValue(n, beachedFor+1);
        this->lutIdx->SetValue(n, beachedFor == this->beachedAtNumberOfTimes-2 ? calcIndex(age+1, true) : calcIndex(age+1, false));
      } else {
        this->particlesBeached->SetValue(n, std::max(beachedFor-1, 0));
        this->points->SetPoint(n, point);
        this->lutIdx->SetValue(n, calcIndex(age+1, false));
        modifiedData = true;
      }
    } else {
      this->lutIdx->SetValue(n, calcIndex(age+1, true));
    }
  }
  if (modifiedData) {
    this->particlesAge->Modified();
    this->points->Modified();
  }
}

int LGlyphLayer::calcIndex(int age, bool beached) {
  // particle out of bounds.
  if (age == -1) return -1;

  int mod = beached ? 50 : 0;
  int ageCalc = age/60;
  if (ageCalc >= 50) ageCalc = 49;

  return ageCalc + mod;
}

void LGlyphLayer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this->callback);
  interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, this->callback);
  interactor->AddObserver(vtkCommand::MouseMoveEvent, this->callback);
}

void LGlyphLayer::removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
}


void LGlyphLayer::setColourMode(ColourMode mode) {

  this->activeColourMode = mode;
  if (this->activeSaturationMode == DESATURATED) return;
  
  this->mapper->SetLookupTable(this->tables[mode]);
}

void LGlyphLayer::setSaturationMode(SaturationMode mode) {
  this->activeSaturationMode = mode;

  if (mode == DESATURATED) {
    this->mapper->SetLookupTable(this->tables[mode]);
  } else {
    this->mapper->SetLookupTable(this->tables[this->activeColourMode]);
  }
}

void LGlyphLayer::setGlyphStyle(GlyphStyle style) {
  switch (style) {
    case CIRCLE:
      this->glyphSource->SetGlyphTypeToCircle();
      break;
    case SQUARE:
      this->glyphSource->SetGlyphTypeToSquare();
      break;
    case TRIANGLE:
      this->glyphSource->SetGlyphTypeToTriangle();
      break;
    case CROSS:
      this->glyphSource->SetGlyphTypeToCross();
      break;
  }
}


void LGlyphLayer::setDt(int dt) {
  this->dt = dt;
}

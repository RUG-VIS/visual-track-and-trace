#include "LGlyphLayer.h"
#include "../commands/SpawnPointCallback.h"
#include <vtkActor2D.h>
#include <vtkGlyph2D.h>
#include <vtkLookupTable.h>
#include <vtkGlyphSource2D.h>
#include <vtkNamedColors.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleUser.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>

#include "../CartographicTransformation.h"

vtkSmartPointer<SpawnPointCallback> LGlyphLayer::createSpawnPointCallback() {
  auto newPointCallBack = vtkSmartPointer<SpawnPointCallback>::New();
  newPointCallBack->setData(this->data);
  newPointCallBack->setPoints(this->points);
  newPointCallBack->setRen(this->ren);
  newPointCallBack->setUVGrid(this->uvGrid);
  newPointCallBack->setBeached(this->particlesBeached);
  newPointCallBack->setAge(this->particlesAge);
  return newPointCallBack;
}

// Further notes; current thinking is to allow tracking a particle's age by using a scalar array in the VtkPolyData. This would be incremented for every tick/updateData function call.

/**
 * Build and returns a vtkLookupTable for the given number of colours in grayscale.
 * @param n : number of colours to add to the SetTableRange
 * @return : a vtkLookupTable with grayscale colours from [1,1,1,1] to [0.25, 0.25, 0.25, 1] in n steps.
 */
vtkSmartPointer<vtkLookupTable> buildLutBrightness(int n) {
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(n);
  lut->SetTableRange(0, n);
  lut->SetScaleToLinear();
  lut->Build();
  for (int i=0; i < n; i++) {
    lut->SetTableValue(i, 1-(0.75*i/(n-1)), 1-(0.75*i/(n-1)), 1-(0.75*i/(n-1)), 1);
  }
  lut->UseAboveRangeColorOn();
  lut->SetAboveRangeColor(0.2, 0.2, 0.2, 1);
  
  // We cheat a little here: any particle with an age of -1 is out of bounds, and thus set invisible.
  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(1,1,1,0);

  return lut;
}

/**
 * Build and returns a vtkLookupTable for the given number of colours in grayscale.
 * @param n : number of colours to add to the SetTableRange
 * @return : a vtkLookupTable with grayscale colours from [1,1,1,1] to [1,1,1,0.25] in n steps.
 */
vtkSmartPointer<vtkLookupTable> buildLutOpacity(int n) {
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(n);
  lut->SetTableRange(0, n);
  lut->SetScaleToLinear();
  lut->Build();
  for (int i=0; i < n; i++) {
    lut->SetTableValue(i, 1, 1, 1, 1-(0.75*i/(n-1)));
  }
  lut->UseAboveRangeColorOn();
  lut->SetAboveRangeColor(1,1,1,0.20);
  
  // We cheat a little here: any particle with an age of -1 is out of bounds, and thus set invisible.
  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(1,1,1,0);

  return lut;
}

LGlyphLayer::LGlyphLayer(std::shared_ptr<UVGrid> uvGrid, std::unique_ptr<AdvectionKernel> advectionKernel) {
  this->luts.push(buildLutOpacity(512));
  this->luts.push(buildLutBrightness(512));

  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(2);

  this->points = vtkSmartPointer<vtkPoints>::New();
  vtkNew<vtkPolyData> data;
  data->SetPoints(this->points);

  this->particlesBeached = vtkSmartPointer<vtkIntArray>::New();
  this->particlesBeached->SetName("particlesBeached");
  this->particlesBeached->SetNumberOfComponents(0);

  this->particlesAge = vtkSmartPointer<vtkIntArray>::New();
  this->particlesAge->SetName("particlesAge");
  this->particlesAge->SetNumberOfComponents(0);

  data->GetPointData()->AddArray(this->particlesBeached);
  data->GetPointData()->AddArray(this->particlesAge);
  data->GetPointData()->SetActiveScalars("particlesAge");

  advector = std::move(advectionKernel);
  this->uvGrid = uvGrid;

  vtkSmartPointer<vtkTransformFilter> transformFilter = createCartographicTransformFilter(uvGrid);
  transformFilter->SetInputData(data);

  vtkNew<vtkGlyphSource2D> circleSource;
  circleSource->SetGlyphTypeToCircle();
  circleSource->SetScale(0.02);
  circleSource->Update();

  vtkNew<vtkGlyph2D> glyph2D;
  glyph2D->SetSourceConnection(circleSource->GetOutputPort());
  glyph2D->SetInputConnection(transformFilter->GetOutputPort());
  glyph2D->SetScaleModeToDataScalingOff();
  glyph2D->Update();

  this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->mapper->SetInputConnection(glyph2D->GetOutputPort());
  this->mapper->SetColorModeToMapScalars();

  auto lut = this->luts.front();
  mapper->SetLookupTable(lut);
  this->luts.pop();
  this->luts.push(lut);

  this->mapper->UseLookupTableScalarRangeOn();
  this->mapper->Update();
  
  vtkNew<vtkActor> actor;
  actor->SetMapper(this->mapper);

  this->ren->AddActor(actor);
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
      this->particlesBeached->SetValue(n, this->beachedAtNumberOfTimes+1);
      this->particlesAge->SetValue(n, -1);
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
      } else {
        this->particlesBeached->SetValue(n, std::max(beachedFor-1, 0));
        this->points->SetPoint(n, point);
        modifiedData = true;
      }
    }
  }
  if (modifiedData) {
    this->particlesAge->Modified();
    this->points->Modified();
  }
}

void LGlyphLayer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  auto newPointCallBack = createSpawnPointCallback();
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, newPointCallBack);
  interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, newPointCallBack);
  interactor->AddObserver(vtkCommand::MouseMoveEvent, newPointCallBack);
}


void LGlyphLayer::cycleGlyphStyle() {
  auto lut = this->luts.front();
  this->mapper->SetLookupTable(lut);
  this->luts.pop();
  this->luts.push(lut);
}

void LGlyphLayer::setDt(int dt) {
  this->dt = dt;
}

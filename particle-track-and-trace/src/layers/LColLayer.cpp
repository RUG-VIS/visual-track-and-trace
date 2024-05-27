#include "LColLayer.h"
#include "../commands/SpawnPointCallback.h"
#include <vtkActor2D.h>
#include <vtkCellData.h>
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

vtkSmartPointer<SpawnPointCallback> LColLayer::createSpawnPointCallback() {
  vtkNew<SpawnPointCallback> newPointCallBack;
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
 * @return : a vtkLookupTable with grayscale colours from [1,1,1,1] to [1,1,1,0.25] in n steps.
 */
vtkSmartPointer<vtkLookupTable> buildLut(int n) {
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

LColLayer::LColLayer(std::shared_ptr<UVGrid> uvGrid, std::unique_ptr<AdvectionKernel> advectionKernel) {
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(2);

  this->points = vtkSmartPointer<vtkPoints>::New();
  vtkNew<vtkPolyData> data;
  data->SetPoints(this->points);

  this->particlesBeached = vtkSmartPointer<vtkIntArray>::New();
  this->particlesBeached->SetName("particlesBeached");
  this->particlesBeached->SetNumberOfComponents(1);

  this->particlesAge = vtkSmartPointer<vtkIntArray>::New();
  this->particlesAge->SetName("particlesAge");
  this->particlesAge->SetNumberOfComponents(1);

  this->uvGrid = uvGrid;
  this->numLats = uvGrid->lats.size();
  this->numLons = uvGrid->lons.size();
  this->cellParticleDensity = vtkSmartPointer<vtkIntArray>::New();
  this->cellParticleDensity->SetName("cellParticleDensity");
  this->cellParticleDensity->SetNumberOfComponents(1);
  this->cellParticleDensity->SetNumberOfTuples((numLats-1)*(numLons-1));
  
  data->GetPointData()->AddArray(this->particlesBeached);
  data->GetPointData()->AddArray(this->particlesAge);
  data->GetCellData()->AddArray(this->cellParticleDensity);
  data->GetPointData()->SetActiveScalars("particlesAge");
  data->GetCellData()->SetActiveScalars("cellParticleDensity");

  advector = std::move(advectionKernel);
  this->uvGrid = uvGrid;

  vtkSmartPointer<vtkTransformFilter> transformFilter = createCartographicTransformFilter(uvGrid);
  transformFilter->SetInputData(data);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  this->ren->AddActor(actor);
  this->callback = createSpawnPointCallback();
}

void LColLayer::spoofPoints() {
  for (int i=0; i < 330; i+=5) {
    for (int j=0; j < 330; j+=5) {
      this->points->InsertNextPoint(-15.875+(12.875+15.875)/330*j, 46.125+(62.625-46.125)/330*i, 0);
      this->particlesBeached->InsertNextValue(0);
      this->particlesAge->InsertNextValue(0);
    }
  }
  this->points->Modified();
}

void LColLayer::updateData(int t) {
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

void LColLayer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this->callback);
  interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, this->callback);
  interactor->AddObserver(vtkCommand::MouseMoveEvent, this->callback);
}

void LColLayer::removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
}


void LColLayer::setDt(int dt) {
  this->dt = dt;
}

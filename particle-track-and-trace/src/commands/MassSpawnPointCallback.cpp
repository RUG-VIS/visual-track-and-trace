#include <vtkVertex.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>

#include "MassSpawnPointCallback.h"
#include "../CartographicTransformation.h"


void MassSpawnPointCallback::Execute(vtkObject *caller, unsigned long evId, void *callData) {
    // Note the use of reinterpret_cast to cast the caller to the expected type.
    auto interactor = reinterpret_cast<vtkRenderWindowInteractor *>(caller);

    if (evId == vtkCommand::LeftButtonPressEvent) {
        dragging = true;
    }
    if (evId == vtkCommand::LeftButtonReleaseEvent) {
        dragging = false;
    }
    if (!dragging) {
        return;
    }

    int x, y;
    interactor->GetEventPosition(x, y);

    randomParticles(x,y);
    this->points->Modified();
    this->ren->GetRenderWindow()->Render();
}

void MassSpawnPointCallback::randomParticles(double x, double y) {
  double worldPos[4] = {2, 0 ,0, 0};
  double displayPos[3] = {static_cast<double>(x), static_cast<double>(y), 0.0};
  this->ren->SetDisplayPoint(displayPos);
  this->ren->DisplayToWorld();
  this->ren->GetWorldPoint(worldPos);
  this->inverseCartographicProjection->TransformPoint(worldPos, worldPos);

  double mod[2] = {0,0};
  for (int i=0; i < 10; i++) {
    mod[0] = this->lats[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(this->lats[1] - this->lats[0])));
    mod[1] = this->lons[0] + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(this->lons[1] - this->lons[0])));
    addParticle(worldPos[0]+mod[0], worldPos[1]+mod[1]);
  }
}

void MassSpawnPointCallback::addParticle(double lon, double lat) {
  points->InsertNextPoint(lon, lat, 0);
  this->particlesBeached->InsertNextValue(0);
  this->particlesAge->InsertNextValue(0);
  this->lutIdx->InsertNextValue(0);
}

MassSpawnPointCallback::MassSpawnPointCallback() {}

MassSpawnPointCallback *MassSpawnPointCallback::New() {
  return new MassSpawnPointCallback;
}



void MassSpawnPointCallback::setUVGrid(const std::shared_ptr<UVGrid> &uvGrid) {
  this->uvGrid = uvGrid;
  inverseCartographicProjection = createInverseCartographicTransformFilter(uvGrid)->GetTransform();

  this->lats[0] = this->uvGrid->latStep()*-3;
  this->lats[1] = this->uvGrid->latStep()*3;
  this->lons[0] = this->uvGrid->lonStep()*-3;
  this->lons[1] = this->uvGrid->lonStep()*3;

}

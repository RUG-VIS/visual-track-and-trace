#include "SingularSpawnPointCallback.h"

#include <vtkVertex.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>


void SingularSpawnPointCallback::Execute(vtkObject *caller, unsigned long evId, void *callData) {
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

    double worldPos[4] = {2, 0 ,0, 0};
    double displayPos[3] = {static_cast<double>(x), static_cast<double>(y), 0.0};
    this->ren->SetDisplayPoint(displayPos);
    this->ren->DisplayToWorld();
    this->ren->GetWorldPoint(worldPos);
    this->inverseCartographicProjection->TransformPoint(worldPos, worldPos);

    this->points->InsertNextPoint(worldPos[0], worldPos[1], 0);
    this->particlesBeached->InsertNextValue(0);
    this->particlesAge->InsertNextValue(0);
    this->lutIdx->InsertNextValue(0);

    this->points->Modified();
    this->ren->GetRenderWindow()->Render();
}


SingularSpawnPointCallback::SingularSpawnPointCallback() { }

SingularSpawnPointCallback *SingularSpawnPointCallback::New() {
  return new SingularSpawnPointCallback;
}

#include "CameraMoveCallback.h"

#include <vtkVertex.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>

using std::string;

void CameraMoveCallback::Execute(vtkObject *caller, unsigned long evId,
                                 void *callData) {
  // Note the use of reinterpret_cast to cast the caller to the expected type.
  auto intr = reinterpret_cast<vtkRenderWindowInteractor *>(caller);

  switch (evId) {
    case vtkCommand::KeyPressEvent:
      if (not strcmp("minus", intr->GetKeySym())) {
        zoom(false);
      } else if (not strcmp("equal", intr->GetKeySym())) {
        zoom(true);
      } else {
        pan(intr->GetKeySym());
      }
      break;
    case vtkCommand::MouseWheelForwardEvent:
      zoom(true);
      break;
    case vtkCommand::MouseWheelBackwardEvent:
      zoom(false);
      break;
    default:
      break;
  }

  // also calls the render function when the camera's position has not actually changed. This is a negligble inefficiency.
  intr->GetRenderWindow()->Render();
}


void CameraMoveCallback::clampCamera(double pos[3]) {
  double scale = this->cam->GetParallelScale();

  // only check the x,y coords of the camera; we don't care about z
  for (int i=0; i < 2; i++) {
    //boundary cond: scale+|pos| < 1.
    if (abs(pos[i])+scale > 1.01) { 
      if (pos[i] >= 0) {
        pos[i] = 1 - scale;
      } else {
        pos[i] = scale - 1;
      }
    }
  }

  this->cam->SetPosition(pos);
  this->cam->SetFocalPoint(pos[0], pos[1], 0);
}

void CameraMoveCallback::zoom(const bool in) {
  double scale = this->cam->GetParallelScale();

  if (in) {
    if (scale >= 0.2) 
      scale -= 0.1;
  } else {
    if (scale <= 0.9)
      scale += 0.1;
  }
  
  this->cam->SetParallelScale(scale);
  clampCamera(this->cam->GetPosition());
}

// we use the interactor's getKeySym instead of getKeyCode because getKeyCode is platform-dependent.
void CameraMoveCallback::pan(const string dir) {
  double pos[3];
  this->cam->GetPosition(pos);

  if (dir == "Left" or dir == "h") {
    pos[0] -= 0.1;
  } else if (dir == "Up" or dir == "k" ) {
    pos[1] += 0.1;
  } else if (dir == "Right" or dir == "l" ) {
    pos[0] += 0.1;
  } else if (dir == "Down" or dir == "j" ) {
    pos[1] -= 0.1;
  }

  clampCamera(pos);
}

CameraMoveCallback::CameraMoveCallback() : cam(nullptr) {}

CameraMoveCallback *CameraMoveCallback::New(vtkCamera *cam) {
  auto me = new CameraMoveCallback;
  me->setCam(cam);
  return me;
}

void CameraMoveCallback::setCam(const vtkSmartPointer<vtkCamera> &cam) {
    this->cam = cam;
}

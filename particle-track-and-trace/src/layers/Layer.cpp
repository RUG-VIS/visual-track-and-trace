#include "Layer.h"
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>

using std::string;

vtkSmartPointer<vtkRenderer> Layer::getLayer() {
  return this->ren;
}

void Layer::updateData(int t) {
  // By default, do nothing
}

void Layer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
    // By default, do nothing
}

void Layer::removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
    // By default, do nothing
}

void Layer::setCamera(vtkSmartPointer<vtkCamera> cam) {
  this->getLayer()->SetActiveCamera(cam.GetPointer());
}

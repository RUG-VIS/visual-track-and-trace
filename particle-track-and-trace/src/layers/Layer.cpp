#include "Layer.h"
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>

using std::string;

vtkSmartPointer<vtkRenderer> Layer::getLayer() {
  return this->ren;
}

void Layer::setCamera(vtkSmartPointer<vtkCamera> cam) {
  this->getLayer()->SetActiveCamera(cam.GetPointer());
}


void Layer::updateData(int t) {}
void Layer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {}
void Layer::removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {}
void Layer::setColorMode(ColourMode mode) {}
void Layer::setSaturationMode(SaturationMode mode) {}
void Layer::setGlyphStyle(GlyphStyle style) {}
void Layer::setSamplingMode(SamplingMode mode) {}


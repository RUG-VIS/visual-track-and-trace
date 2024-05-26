#include <vtkRenderWindow.h>

#include "Technique.h"

Technique::Technique(vtkSmartPointer<vtkCamera> cam) : cam(cam) {}


void Technique::addLayer(Layer *l) {
  l->setCamera(this->cam);
  this->layers.push_back(l);
}


void Technique::removeLayer(Layer *l) {
  auto it = std::find(this->layers.begin(), this->layers.end(), l);
  if (it != this->layers.end()) {
    this->layers.erase(it);
  }
}


void Technique::updateData(int t) {
  for (Layer *l : this->layers) {
    l->updateData(t);
  }
}


int Technique::numberOfLayers() {
  return this->layers.size();
}


void Technique::bind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr) {
  for (Layer *l : this->layers) {
    l->addObservers(intr);
    win->AddRenderer(l->getLayer());
  }
  win->SetNumberOfLayers(this->layers.size());
  // win->Render();
}


void Technique::unbind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr) {
  for (Layer *l : this->layers) {
    l->removeObservers(intr);
    win->RemoveRenderer(l->getLayer());
  }
  win->SetNumberOfLayers(0);
}

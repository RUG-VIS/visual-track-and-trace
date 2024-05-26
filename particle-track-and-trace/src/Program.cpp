#include <stdexcept>
#include <vtkRenderWindow.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkGlyphSource2D.h>
#include <vtkRegularPolygonSource.h>
#include <vtkGlyph2D.h>
#include <vtkActor2D.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkVertexGlyphFilter.h>
#include <netcdf>
#include <vtkArrowSource.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCallbackCommand.h>
#include <vtkInteractorStyleUser.h>

#include "Program.h"
#include "commands/TimerCallbackCommand.h"
#include "CartographicTransformation.h"
#include "commands/CameraMoveCallback.h"

void Program::setWinProperties() {
  this->win->SetWindowName("Simulation");
  this->win->SetSize(661, 661);
  this->win->SetDesiredUpdateRate(60);

  this->interact->SetRenderWindow(this->win.Get());
  this->interact->Initialize();

  vtkNew<vtkInteractorStyleUser> style;
  this->interact->SetInteractorStyle(style);
}

void Program::setupTimer(int dt) {
  auto callback = vtkSmartPointer<TimerCallbackCommand>::New(this);
  callback->SetClientData(this);
  callback->setDt(dt);
  this->interact->AddObserver(vtkCommand::TimerEvent, callback);
  this->interact->AddObserver(vtkCommand::KeyPressEvent, callback);
  this->interact->CreateRepeatingTimer(17); // 60 fps == 1000 / 60 == 16.7 ms per frame
}

void Program::setupCameraCallback() {
  auto callback = vtkSmartPointer<CameraMoveCallback>::New(this->cam);
  this->interact->AddObserver(vtkCommand::MouseWheelForwardEvent, callback);
  this->interact->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback);
  this->interact->AddObserver(vtkCommand::KeyPressEvent, callback);
}

Program::Program(QWidget *parent): QVTKOpenGLNativeWidget(parent) {
  this->win = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  setRenderWindow(this->win);
  this->interact = win->GetInteractor();
  this->cam = createNormalisedCamera();
  this->activeIdx = -1;

  this->win->SetNumberOfLayers(0);
  setWinProperties();
  setupTimer(60 * 60); // FIXME: manually tracking this variable is a little stupid.
  setupCameraCallback();
}


void Program::addTechnique(Technique *technique) {
  this->techniques.push_back(technique);

}

void Program::removeTechnique(Technique *technique) {
  auto it = std::find(this->techniques.begin(), this->techniques.end(), technique);
  if (it != this->techniques.end()) {
    int idx = it - this->techniques.begin();
    if (idx == this->activeIdx) {
      throw std::out_of_range("Can't remove active technique.");
    }
    this->techniques.erase(it);
    this->activeIdx = -1;
    setActiveTechnique(0);
  }
}

void Program::updateData(int t) {
  // FIXME: think on how to update techniques; do we update all? just active? unsure.
  win->Render();
  this->techniques[this->activeIdx]->updateData(t);
}

void Program::setActiveTechnique(int idx) {
  // Only change things if a different technique has been selected.
  if (idx == this->activeIdx) {
    return;
  }

  // check the given idx is valid.
  if (idx >= this->techniques.size()) {
    throw std::out_of_range("Index out of range!");
  }

  if (this->activeIdx >= 0 and this->activeIdx < this->techniques.size())
    this->techniques[this->activeIdx]->unbind(this->win, this->interact);

  this->techniques[idx]->bind(this->win, this->interact);
  
  this->activeIdx = idx;
}


Program::~Program() {
  cout << "deleting program" << endl;
}


vtkSmartPointer<vtkCamera> Program::getCamera() {
  return this->cam;
}

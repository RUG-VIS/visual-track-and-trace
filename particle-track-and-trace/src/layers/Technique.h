#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "Layer.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include "enums.h"

class Technique {
  //TODO: comments
private:
  std::vector<Layer *> layers;
  vtkSmartPointer<vtkCamera> cam;

public:
  Technique(vtkSmartPointer<vtkCamera> cam);
  void addLayer(Layer *l);
  void removeLayer(Layer *l);
  void updateData(int t); 
  int numberOfLayers();
  void bind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr); 
  void unbind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr);

  void setColourMode(ColourMode mode);
  void setSaturationMode(SaturationMode mode);
  void setGlyphStyle(GlyphStyle style);
  void setSamplingMode(SamplingMode mode);

};



#endif

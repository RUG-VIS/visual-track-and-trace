#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "layers/Layer.h"
#include <vtkPolyData.h>
class Technique {
private:
  std::vector<Layer *> layers;
  vtkSmartPointer<vtkPoints> points;
  vtkSmartPointer<vtkPolyData> data;

  void setupInteractions();

public:
  Technique();
  void addLayer(Layer *l);
  void removeLayer(Layer *l);
  void updateData(int t); 
  int numberOfLayers();

};



#endif

#ifndef TIMELAYER_H
#define TIMELAYER_H

#include "Layer.h"
#include <vtkTextActor.h>

class TimeLayer : public Layer {
private:
  std::string secondsToDate(int t);
  int years;
  int lastT;
  vtkSmartPointer<vtkTextActor> textActor;

public:
  TimeLayer();
  void updateData(int t) override;

};


#endif

#include "TimeLayer.h"
#include <vtkTextProperty.h>

using std::string;

TimeLayer::TimeLayer() {
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(4);

  this->years = 0;
  this->lastT = 0;

  this->textActor = vtkSmartPointer<vtkTextActor>::New();
  this->textActor->SetDisplayPosition(10, 10);
  vtkSmartPointer<vtkTextProperty> textProp = this->textActor->GetTextProperty();
  textProp->SetFontFamilyToArial();
  textProp->BoldOn();
  textProp->SetFontSize(16);
  textProp->ShadowOff();
  textProp->SetColor(0,0,0);

  ren->AddActor(this->textActor);
}

string TimeLayer::secondsToDate(int t) {
  if (t < lastT) this->years++;

  int hours = t /3600;
  int days = hours / 24;
  
  this->lastT = t;
  return string("year " + std::to_string(years) + ", day " + std::to_string(days));
  // return string("seconds  " + std::to_string(t));
}


void TimeLayer::updateData(int t) {
  this->textActor->SetInput(secondsToDate(t).c_str());
  this->textActor->Modified();
}

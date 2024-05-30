#ifndef LEGENDLAYER_H
#define LEGENDLAYER_H

#include "Layer.h"
#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
#include <vtkImageReader2.h>


class LegendLayer : public Layer {
private:
  std::vector<vtkSmartPointer<vtkImageReader2>> readers;
  vtkSmartPointer<vtkImageChangeInformation> imageCenterer;
  ColourMode activeColourMode;
  SaturationMode activeSaturationMode;
  std::string legendPath;
  std::string techName;

  void addLegends();
  vtkSmartPointer<vtkImageReader2> readImage(std::string path);
  

  

public:
  LegendLayer(std::string imagePath, std::string techName);
  void setColourMode(ColourMode mode) override;
  void setSaturationMode(SaturationMode mode) override;
};


vtkSmartPointer<vtkMatrix4x4> toDesiredMatrix(const double x0, const double y0, const int xMax, const int yMax);
vtkSmartPointer<vtkMatrix4x4> toNormalisedMatrix(const double x0, const double y0, const int xMax, const int yMax);
#endif

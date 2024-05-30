#include "LegendLayer.h"
#include <vtkImageDataGeometryFilter.h>
#include <vtkImageReader2Factory.h>
#include <vtkPolyDataMapper.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

using std::string;

LegendLayer::LegendLayer(string legendPath, string techName) : legendPath(legendPath), techName(techName) {
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(3);
  this->ren->InteractiveOff();

  this->activeColourMode = COMPLEMENTARY;
  this->activeSaturationMode = SATURATED;
  addLegends();

  //setup pipeline
  // translates an image such that the middle is at (0,0)
  this->imageCenterer = vtkSmartPointer<vtkImageChangeInformation>::New();
  this->imageCenterer->SetInputConnection(this->readers[0]->GetOutputPort());
  this->imageCenterer->CenterImageOn();
  this->imageCenterer->Update();

  // get some info from the data we'll need in a second
  // FIXME: not updating this value means differently sized images are not properly scaled.
  vtkSmartPointer<vtkImageData> imageData = imageCenterer->GetOutput();
  double origin[3];
  int extent[6];
  imageData->GetOrigin(origin);
  imageData->GetExtent(extent);

  // map the imageData to a vtkPolydata so we can use a vtkTransform
  vtkNew<vtkImageDataGeometryFilter> imageDataGeometryFilter;
  imageDataGeometryFilter->SetInputData(imageData);
  imageDataGeometryFilter->Update();

  // setup the vtkTransform - this is where use the data from imageData we got earlier
  vtkNew<vtkTransform> transform;
  transform->Identity();
  // FIXME: can be improved for one matrix instead.
  transform->Concatenate(toDesiredMatrix(0.5, -0.5, 1, -1));
  transform->Concatenate(toNormalisedMatrix(origin[0], origin[1], extent[1]+origin[0], extent[3]+origin[1]));
  vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
  transformFilter->SetTransform(transform);
  transformFilter->SetInputConnection(imageDataGeometryFilter->GetOutputPort());
  transformFilter->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(transformFilter->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  this->ren->AddActor(actor);
  
  setColourMode(COMPLEMENTARY);
}

void LegendLayer::addLegends() {
  this->readers.push_back(readImage(this->legendPath + "/" + this->techName + "_complementary.png"));
  this->readers.push_back(readImage(this->legendPath + "/" + this->techName + "_contrasting.png"));
  this->readers.push_back(readImage(this->legendPath + "/" + this->techName + "_monochromatic.png"));
  this->readers.push_back(readImage(this->legendPath + "/" + this->techName + "_desaturated.png"));
}

vtkSmartPointer<vtkImageReader2> LegendLayer::readImage(string path) {
  vtkNew<vtkImageReader2Factory> readerFactory;
  vtkSmartPointer<vtkImageReader2> imageReader;
  imageReader.TakeReference(readerFactory->CreateImageReader2(path.c_str()));
  imageReader->SetFileName(path.c_str());
  imageReader->Update();
  return imageReader;
}


void LegendLayer::setColourMode(ColourMode mode) {
  this->activeColourMode = mode;
  if (this->activeSaturationMode == DESATURATED) return;
  
  this->imageCenterer->SetInputConnection(this->readers[mode]->GetOutputPort());
  imageCenterer->Update();
}

void LegendLayer::setSaturationMode(SaturationMode mode) {
  this->activeSaturationMode = mode;

  if (mode == DESATURATED) {
    this->imageCenterer->SetInputConnection(this->readers[mode]->GetOutputPort());
  } else {
    this->imageCenterer->SetInputConnection(this->readers[this->activeColourMode]->GetOutputPort());
  }
  imageCenterer->Update();
}


// transforms from [0,1] x [0,1] to [x0, xMax] x [y0, yMax]
vtkSmartPointer<vtkMatrix4x4> toDesiredMatrix(const double x0, const double y0, const int xMax, const int yMax) {
    double eyeTransform[] = {
            xMax-x0, 0, 0, x0,
            0, yMax-y0, 0, y0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->DeepCopy(eyeTransform);
    return matrix;
}

// transform image from [x0, xMax] x [y0, yMax] to [0,1] x [0,1]
vtkSmartPointer<vtkMatrix4x4> toNormalisedMatrix(const double x0, const double y0, const int xMax, const int yMax) {
    double eyeTransform[] = {
            1/(xMax-x0), 0, 0, 0.5-(xMax+x0)/(2*(xMax-x0)),
            0, 1/(yMax-y0), 0, 0.5-(yMax+y0)/(2*(yMax-y0)),
            0, 0, 1, 0,
            0, 0, 0, 1
    };
    auto matrix = vtkSmartPointer<vtkMatrix4x4>::New();
    matrix->DeepCopy(eyeTransform);
    return matrix;
}

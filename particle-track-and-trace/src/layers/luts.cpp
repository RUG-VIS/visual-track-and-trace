#include <vtkLookupTable.h>
#include <vtkColorTransferFunction.h>
#include "luts.h"

// LGlyph tables
vtkSmartPointer<vtkLookupTable> buildComplementary() {
  // uses a modified navia colour map as base.
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  vtkNew<vtkColorTransferFunction> colorTransferFunction;
  colorTransferFunction->AddRGBPoint(0, 0.317647, 0.52549, 0.329412);
  colorTransferFunction->AddRGBPoint(49, 0.584, 0.584, 0.584);

  double c[3];
  int idx=0;
  for (double notBeached=1; notBeached >= 0.5; notBeached-=0.5) {
    for (int i=0; i < 50; i++) {
      colorTransferFunction->GetColor(i, c);
      lut->SetTableValue(idx++, c[0], c[1], c[2], notBeached);
    }
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(1,1,1,0);

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDesaturated() {
  // uses the grayC colour map.
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  vtkNew<vtkColorTransferFunction> colorTransferFunction;
  colorTransferFunction->AddRGBPoint(0, 0.313725, 0.313725, 0.313725);
  colorTransferFunction->AddRGBPoint(49, 0.909804, 0.909804, 0.909804);

  double c[3];
  int idx=0;
  for (double notBeached=1; notBeached >= 0.5; notBeached-=0.5) {
    for (int i=0; i < 50; i++) {
      colorTransferFunction->GetColor(i, c);
      lut->SetTableValue(idx++, c[0], c[1], c[2], notBeached);
    }
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(1,1,1,0);

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


// ECol tables
vtkSmartPointer<vtkLookupTable> buildCyclicComplementary() {
  // uses the corkO cyclic colour map
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0;
  for (double opacity=0.1; opacity <= 1.0; opacity+=0.1) {
    lut->SetTableValue(idx++, 0.247059, 0.243137, 0.227451, opacity);
    lut->SetTableValue(idx++, 0.243137, 0.266667, 0.364706, opacity);
    lut->SetTableValue(idx++, 0.329412, 0.431373, 0.580392, opacity);
    lut->SetTableValue(idx++, 0.517647, 0.619608, 0.729412, opacity);
    lut->SetTableValue(idx++, 0.678431, 0.768627, 0.784314, opacity);
    lut->SetTableValue(idx++, 0.631373, 0.768627, 0.65098, opacity);
    lut->SetTableValue(idx++, 0.45098, 0.639216, 0.435294, opacity);
    lut->SetTableValue(idx++, 0.309804, 0.45098, 0.231373, opacity);
    lut->SetTableValue(idx++, 0.262745, 0.309804, 0.172549, opacity);
    lut->SetTableValue(idx++, 0.247059, 0.243137, 0.227451, opacity);
  }

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildCyclicContrasting() {
  // uses the romaO cyclic colour map.
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100.0);
  lut->SetTableRange(0.0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0.0;
  for (double opacity=0.1; opacity <= 1.0; opacity+=0.1) {
    lut->SetTableValue(idx++, 0.45098, 0.223529, 0.341176, opacity);
    lut->SetTableValue(idx++, 0.529412, 0.25098, 0.215686, opacity);
    lut->SetTableValue(idx++, 0.639216, 0.403922, 0.172549, opacity);
    lut->SetTableValue(idx++, 0.764706, 0.639216, 0.294118, opacity);
    lut->SetTableValue(idx++, 0.839216, 0.847059, 0.576471, opacity);
    lut->SetTableValue(idx++, 0.705882, 0.870588, 0.776471, opacity);
    lut->SetTableValue(idx++, 0.454902, 0.733333, 0.803922, opacity);
    lut->SetTableValue(idx++, 0.309804, 0.533333, 0.72549, opacity);
    lut->SetTableValue(idx++, 0.360784, 0.32549, 0.545098, opacity);
    lut->SetTableValue(idx++, 0.447059, 0.223529, 0.34902, opacity);
  }

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildCyclicMonochromatic() {
  // uses a slightly modified 9-class blues 
  // https://colorbrewer2.org/#type=sequential&scheme=Blues&n=9
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100.0);
  lut->SetTableRange(0.0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0.0;
  for (double opacity=0.1; opacity <= 1.0; opacity+=0.1) {
    lut->SetTableValue(idx++, 0.258824, 0.572549, 0.776471, opacity);
    lut->SetTableValue(idx++, 0.129412, 0.443137, 0.709804, opacity);
    lut->SetTableValue(idx++, 0.031373, 0.317647, 0.611765, opacity);
    lut->SetTableValue(idx++, 0.031373, 0.188235, 0.419608, opacity);
    lut->SetTableValue(idx++, 0.968627, 0.984314, 1.0, opacity);
    lut->SetTableValue(idx++, 0.870588, 0.921569, 0.968627, opacity);
    lut->SetTableValue(idx++, 0.776471, 0.858824, 0.937255, opacity);
    lut->SetTableValue(idx++, 0.619608, 0.792157, 0.882353, opacity);
    lut->SetTableValue(idx++, 0.419608, 0.682353, 0.839216, opacity);
    lut->SetTableValue(idx++, 0.180392, 0.494118, 0.698039, opacity);
  }

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildCyclicDesaturated() {
  // uses a modified brocO colour map
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100.0);
  lut->SetTableRange(0.0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0.0;
  for (double opacity=0.1; opacity <= 1.0; opacity+=0.1) {
    lut->SetTableValue(idx++, 44, 44, 44, opacity);
    lut->SetTableValue(idx++, 56, 56, 56, opacity);
    lut->SetTableValue(idx++, 85, 85, 85, opacity);
    lut->SetTableValue(idx++, 114, 114, 114, opacity);
    lut->SetTableValue(idx++, 149, 149, 149, opacity);
    lut->SetTableValue(idx++, 146, 146, 146, opacity);
    lut->SetTableValue(idx++, 110, 110, 110, opacity);
    lut->SetTableValue(idx++, 78, 78, 78, opacity);
    lut->SetTableValue(idx++, 52, 52, 52, opacity);
    lut->SetTableValue(idx++, 45, 45, 45, opacity);
  }

  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


// LCol tables
vtkSmartPointer<vtkLookupTable> buildDensityComplementary() {
  vtkNew<vtkLookupTable> lut;

  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityContrasting() {
  vtkNew<vtkLookupTable> lut;

  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityMonochromatic() {
  vtkNew<vtkLookupTable> lut;

  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityDesaturated() {
  vtkNew<vtkLookupTable> lut;

  return lut;  
}

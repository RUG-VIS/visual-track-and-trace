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
  colorTransferFunction->AddRGBPoint(49, 0.082, 0.129, 0.082);


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
  colorTransferFunction->AddRGBPoint(0, 0.909804, 0.909804, 0.909804);
  colorTransferFunction->AddRGBPoint(49, 0.313725, 0.313725, 0.313725);

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
  for (double opacity=0.5; opacity <= 1.0; opacity+=0.05) {
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

  lut->SetBelowRangeColor(0,0,0,0);
  lut->UseBelowRangeColorOn();
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

  lut->SetBelowRangeColor(0,0,0,0);
  lut->UseBelowRangeColorOn();
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

  lut->SetBelowRangeColor(0,0,0,0);
  lut->UseBelowRangeColorOn();
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
    lut->SetTableValue(idx++, 0.172549, 0.172549, 0.172549, opacity);
    lut->SetTableValue(idx++, 0.219608, 0.219608, 0.219608, opacity);
    lut->SetTableValue(idx++, 0.333333, 0.333333, 0.333333, opacity);
    lut->SetTableValue(idx++, 0.447059, 0.447059, 0.447059, opacity);
    lut->SetTableValue(idx++, 0.584314, 0.584314, 0.584314, opacity);
    lut->SetTableValue(idx++, 0.572549, 0.572549, 0.572549, opacity);
    lut->SetTableValue(idx++, 0.431373, 0.431373, 0.431373, opacity);
    lut->SetTableValue(idx++, 0.305882, 0.305882, 0.305882, opacity);
    lut->SetTableValue(idx++, 0.203922, 0.203922, 0.203922, opacity);
    lut->SetTableValue(idx++, 0.176471, 0.176471, 0.176471, opacity);
  }

  lut->SetBelowRangeColor(0,0,0,0);
  lut->UseBelowRangeColorOn();
  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


// LCol tables
vtkSmartPointer<vtkLookupTable> buildDensityComplementary() {
  // uses a reverse matplotlib YlGn colour map.
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0;
  for (double opacity=0.5; opacity <= 1.0; opacity+=0.05) {
    lut->SetTableValue(idx++, 0.0, 0.27058823529411763, 0.1607843137254902, opacity);
    lut->SetTableValue(idx++, 0.0, 0.3911572472126105, 0.20901191849288736, opacity);
    lut->SetTableValue(idx++, 0.10388312187620147, 0.4909496347558632, 0.2513033448673587, opacity);
    lut->SetTableValue(idx++, 0.21568627450980393, 0.6196078431372549, 0.330718954248366, opacity);
    lut->SetTableValue(idx++, 0.3724721261053441, 0.7283044982698962, 0.424559784698193, opacity);
    lut->SetTableValue(idx++, 0.5651364859669358, 0.8175009611687812, 0.5119723183391004, opacity);
    lut->SetTableValue(idx++, 0.7359477124183007, 0.8915032679738563, 0.5843137254901961, opacity);
    lut->SetTableValue(idx++, 0.8795847750865051, 0.9526182237600923, 0.6601922337562476, opacity);
    lut->SetTableValue(idx++, 0.9724413687043445, 0.9896655132641292, 0.7464667435601692, opacity);
    lut->SetTableValue(idx++, 1.0, 1.0, 0.8980392156862745, opacity);
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(0,0,0,0);
  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityContrasting() {
  // uses the matplotlib viridis colour map.
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0;
  for (double opacity=0.5; opacity <= 1.0; opacity+=0.05) {
    lut->SetTableValue(idx++, 0.267004, 0.004874, 0.329415, opacity);
    lut->SetTableValue(idx++, 0.281412, 0.155834, 0.469201, opacity);
    lut->SetTableValue(idx++, 0.244972, 0.287675, 0.53726, opacity);
    lut->SetTableValue(idx++, 0.190631, 0.407061, 0.556089, opacity);
    lut->SetTableValue(idx++, 0.147607, 0.511733, 0.557049, opacity);
    lut->SetTableValue(idx++, 0.119699, 0.61849, 0.536347, opacity);
    lut->SetTableValue(idx++, 0.20803, 0.718701, 0.472873, opacity);
    lut->SetTableValue(idx++, 0.430983, 0.808473, 0.346476, opacity);
    lut->SetTableValue(idx++, 0.709898, 0.868751, 0.169257, opacity);
    lut->SetTableValue(idx++, 0.993248, 0.906157, 0.143936, opacity);
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(0,0,0,0);
  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityMonochromatic() {
  // uses a reverse matplotlib Greens colour map.
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  int idx=0;
  for (double opacity=0.5; opacity <= 1.0; opacity+=0.05) {
    lut->SetTableValue(idx++, 0.0, 0.26666666666666666, 0.10588235294117647, opacity);
    lut->SetTableValue(idx++, 0.0, 0.4079046520569012, 0.16444444444444445, opacity);
    lut->SetTableValue(idx++, 0.10388312187620147, 0.5164936562860438, 0.2467512495194156, opacity);
    lut->SetTableValue(idx++, 0.21568627450980393, 0.6287581699346405, 0.3333333333333333, opacity);
    lut->SetTableValue(idx++, 0.36392156862745095, 0.7240292195309497, 0.4181468665897732, opacity);
    lut->SetTableValue(idx++, 0.5351787773933102, 0.8060899653979239, 0.5287504805843906, opacity);
    lut->SetTableValue(idx++, 0.681045751633987, 0.8718954248366013, 0.6562091503267974, opacity);
    lut->SetTableValue(idx++, 0.8089965397923875, 0.9251672433679354, 0.7834525182622069, opacity);
    lut->SetTableValue(idx++, 0.9066205305651672, 0.9641214917339485, 0.8884429065743944, opacity);
    lut->SetTableValue(idx++, 0.9686274509803922, 0.9882352941176471, 0.9607843137254902, opacity);
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(0,0,0,0);
  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}


vtkSmartPointer<vtkLookupTable> buildDensityDesaturated() {
  // uses a modified grayC colour map.
  // https://www.fabiocrameri.ch/colourmaps/
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(100);
  lut->SetTableRange(0, 99);
  lut->SetScaleToLinear();
  lut->Build();

  vtkNew<vtkColorTransferFunction> colorTransferFunction;
  colorTransferFunction->AddRGBPoint(0, 0.313725, 0.313725, 0.313725);
  colorTransferFunction->AddRGBPoint(9, 0.909804, 0.909804, 0.909804);
  double c[3];

  int idx=0;
  for (double opacity=0.5; opacity <= 1.0; opacity+=0.05) {
    for (int i=0; i < 10; i++) {
      colorTransferFunction->GetColor(i, c);
      lut->SetTableValue(idx++, c[0], c[1], c[2], opacity);
    }
  }

  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(0,0,0,0);
  lut->SetNanColor(0.0,0,0,0);
  return lut;  
}

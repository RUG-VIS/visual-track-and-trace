#include "EColLayer.h"
#include <vtkGeneralTransform.h>
#include <vtkGeoProjection.h>
#include <vtkGeoTransform.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkAbstractTransform.h>
#include <vtkOutlineSource.h>
#include <vtkDoubleArray.h>
#include <vtkProbeFilter.h>
#include <vtkGlyphSource2D.h>
#include <vtkRegularPolygonSource.h>
#include <vtkGlyph2D.h>
#include <vtkActor2D.h>
#include <vtkPlaneSource.h>
#include <vtkNamedColors.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkTransform.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkArrowSource.h>

#include "../CartographicTransformation.h"
#include "luts.h"

using std::numbers::pi;

using namespace std;

EColLayer::EColLayer(std::shared_ptr<UVGrid> uvGrid) {
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(1);
  this->ren->InteractiveOff();

  this->uvGrid = uvGrid;
  this->numLats = uvGrid->latSize;
  this->numLons = uvGrid->lonSize;

  this->lutIdx = vtkSmartPointer<vtkIntArray>::New();
  this->lutIdx->SetName("lutIdx");
  this->lutIdx->SetNumberOfComponents(1);
  this->lutIdx->SetNumberOfTuples((numLats-1)*(numLons-1));

  calcMaxStrength();
  buildLuts();
  readCoordinates();
}

void EColLayer::buildLuts() {
  this->tables.push_back(buildCyclicComplementary());
  this->tables.push_back(buildCyclicContrasting());
  this->tables.push_back(buildCyclicMonochromatic());
  this->tables.push_back(buildCyclicDesaturated());

  this->activeColourMode = COMPLEMENTARY;
  this->activeSaturationMode = SATURATED;
}

void EColLayer::calcMaxStrength() {
  double u1 = uvGrid->uMin,
         u2 = uvGrid->uMax,
         v1 = uvGrid->vMin,
         v2 = uvGrid->vMax;

  double a = sqrt(u1*u1 + v1*v1);
  double b = sqrt(u1*u1 + v2*v2);
  double c = sqrt(u2*u2 + v1*v1);
  double d = sqrt(u2*u2 + v2*v2);

  this->maxStrength = max(a, max(b, max(c, d)));
}


// TODO: Bit of a superfunction here; can do with some refactoring.
void EColLayer::readCoordinates() {
  vtkNew<vtkPoints> points;
  points->Allocate(numLats*numLons);

  vtkNew<vtkPolyData> data;
  data->SetPoints(points);
  data->Allocate((numLats-1)*(numLons-1));

  auto transformFilter = createCartographicTransformFilter(uvGrid);
  auto transform = transformFilter->GetTransform();

  int cellId = 0, pointId = 0;
  int latIndex = 0, lonIndex = 0;
  for (auto lon : uvGrid->lons) {
    latIndex = 0;
    for (auto lat : uvGrid->lats) {
      double out[3] = {lon, lat, 0};
      transform->TransformPoint(out, out);
      points->InsertPoint(pointId++, out[0], out[1], 0);
      
      // logic for adding cells
      if (latIndex > 0 and lonIndex > 0 ) {
        int idx = latIndex+lonIndex*numLats;
        vtkNew<vtkIdList> l;
        l->SetNumberOfIds(4);
        l->SetId(0, idx-1);
        l->SetId(1, idx-numLats-1);
        l->SetId(2, idx-numLats);
        l->SetId(3, idx);
        data->InsertNextCell(VTK_QUAD, l);

        // ltake the average of the four surrounding points as the cell's velocity.
        double u=0, v=0;
        for (int j=0; j < 2; j++  )   {
          for (int k=0; k < 2; k++  )   {
            auto [u1, v1] = (*uvGrid)[0, latIndex-j, lonIndex-k];
            u += u1;
            v += v1;
          }
        }
        u /= 4;
        v /= 4;
        this->lutIdx->SetValue(cellId++, calcIndex(u,v, this->maxStrength));
      }
      latIndex++;
    }
    lonIndex++;
  }

  data->GetCellData()->AddArray(this->lutIdx);
  data->GetCellData()->SetActiveScalars("lutIdx");

  this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->mapper->SetInputData(data);
  setColourMode(COMPLEMENTARY);
  this->mapper->UseLookupTableScalarRangeOn();
  this->mapper->Update();

  vtkNew<vtkActor> actor;
  actor->SetMapper(this->mapper);
  actor->GetProperty()->SetColor(0, 1, 0);
  actor->GetProperty()->SetOpacity(0.5);

  this->ren->AddActor(actor);
}


void EColLayer::updateData(int t)   {
  int i = 0;
  for (int lon = 1; lon < uvGrid->lonSize; lon++) {
    for (int lat = 1; lat < uvGrid->latSize; lat++) {
      double u=0, v=0;
      for (int j=0; j < 2; j++  )   {
        for (int k=0; k < 2; k++  )   {
          auto [u1, v1] = (*uvGrid)[t/3600, lat-j, lon-k];
          u += u1;
          v += v1;
        }
      }
      u /= 4;
      v /= 4;
      this->lutIdx->SetValue(i++, calcIndex(u,v, this->maxStrength));
    }
  }
  this->lutIdx->Modified();
}


void EColLayer::setColourMode(ColourMode mode) {
  this->activeColourMode = mode;
  if (this->activeSaturationMode == DESATURATED) return;
  
  this->mapper->SetLookupTable(this->tables[mode]);
}

void EColLayer::setSaturationMode(SaturationMode mode) {
  this->activeSaturationMode = mode;

  if (mode == DESATURATED) {
    this->mapper->SetLookupTable(this->tables[mode]);
  } else {
    this->mapper->SetLookupTable(this->tables[this->activeColourMode]);
  }
}


int calcIndex(double u, double v, double maxStren) {
  int angleIdx = calcAngleIndex(u,v);
  int strIdx = calcStrengthIndex(u,v, maxStren);

  return angleIdx+strIdx*10;
}

int calcAngleIndex(double u, double v) {
  double angle = (atan2(v,u)+pi) * 180 / pi;
  return (int)std::floor(angle/360*10) % 10;
}

// TODO: there's room for improvement in this function.
// Currently maps all strengths to [0,10) where 10 is assigned when strength >= maxStrength/2
// But this is completely heuristic - a more accurate calculation could take into account mean/std of velocity strengths, or do something more fancy with the maxStrength value.
int calcStrengthIndex(double u, double v, double maxStren) {
  double strength = sqrt(u*u + v*v);
  int idx = strength/(maxStren/2)*10;

  if (idx > 9) idx = 9;
  return idx;
}

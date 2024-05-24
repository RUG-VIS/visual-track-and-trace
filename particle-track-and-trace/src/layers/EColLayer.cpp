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

using std::numbers::pi;

using namespace std;

EColLayer::EColLayer(std::shared_ptr<UVGrid> uvGrid) {
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(1);
  this->ren->InteractiveOff();

  this->uvGrid = uvGrid;
  this->numLats = uvGrid->lats.size();
  this->numLons = uvGrid->lons.size();

  this->strength = vtkSmartPointer<vtkDoubleArray>::New();
  this->strength->SetName("strength");
  this->strength->SetNumberOfComponents(1);
  this->strength->SetNumberOfTuples((numLats-1)*(numLons-1));

  this->direction = vtkSmartPointer<vtkDoubleArray>::New();
  this->direction->SetName("direction");
  this->direction->SetNumberOfComponents(1);
  this->direction->SetNumberOfTuples((numLats-1)*(numLons-1));

  readCoordinates();
}

/**
  * Sets a given rgba colour to a range of values [start, end] in the lut.
  * @param lut : lookuptable to operate on.
  * @ param start : starting index of range to assign
  * @ param end: ending index of range to assign
  * @param r : red value [0,1]
  * @param g : green value [0,1]
  * @param n : blue value [0,1]
  * @param a : alpha value [0,1]
  */
void setLutRange(vtkSmartPointer<vtkLookupTable> lut, int start, int end, double r, double g, double b, double a) {
  for (int i=start; i <= end; i++) {
    lut->SetTableValue(i, r, g, b, a);
  }
}

// builds a 4-way lookuptable, used to encode the directional component
vtkSmartPointer<vtkLookupTable> buildLutDirs() {
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(360);
  lut->SetTableRange(0, 359);
  lut->SetScaleToLinear();
  lut->Build();
  //currently builds a corkO cyclic colour map, divided into 8 colours (see https://www.fabiocrameri.ch/cycliccolourmaps/)
  setLutRange(lut, 000, 020, 0.247, 0.243, 0.227, 1);
  setLutRange(lut, 021, 060, 0.243, 0.267, 0.365, 1);
  setLutRange(lut, 061, 100, 0.318, 0.416, 0.557, 1);
  setLutRange(lut, 101, 140, 0.518, 0.620, 0.729, 1);
  setLutRange(lut, 141, 180, 0.667, 0.757, 0.773, 1);
  setLutRange(lut, 181, 220, 0.631, 0.769, 0.651, 1);
  setLutRange(lut, 221, 260, 0.451, 0.639, 0.435, 1);
  setLutRange(lut, 261, 300, 0.298, 0.431, 0.224, 1);
  setLutRange(lut, 301, 340, 0.263, 0.310, 0.173, 1);
  setLutRange(lut, 341, 359, 0.247, 0.243, 0.227, 1);

  lut->SetNanColor(0,0,0,0);

  return lut;
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

        double coords[12];
        points->GetPoint(idx-1, coords);
        points->GetPoint(idx-numLats-1, coords+3);
        points->GetPoint(idx-numLats, coords+6);
        points->GetPoint(idx, coords+9);
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
        this->strength->SetTuple1(cellId, std::sqrt(u*u + v*v));
        this->direction->SetTuple1(cellId++, atan(u/v)*180/pi);
      }
      latIndex++;
    }
    lonIndex++;
  }

  data->GetCellData()->AddArray(this->strength);
  data->GetCellData()->AddArray(this->direction);
  // data->GetCellData()->SetActiveScalars("strength");

  vtkNew<vtkPolyDataMapper>(mapper);
  mapper->SetInputData(data);
  mapper->SetLookupTable(buildLutDirs());
  mapper->UseLookupTableScalarRangeOn();
  mapper->Update();
  data->GetCellData()->SetActiveScalars("direction");

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0, 1, 0);
  actor->GetProperty()->SetOpacity(0.2);

  // vtkNew<vtkActor> act2;
  // act2->SetMapper(mapper);
  // act2->GetProperty()->SetRepresentationToWireframe();
  // ren->AddActor(act2);

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
      this->strength->SetTuple1(i, std::sqrt(u*u + v*v));
      this->direction->SetTuple1(i++, atan(u/v)*180/pi);
    }
  }
  this->strength->Modified();
}

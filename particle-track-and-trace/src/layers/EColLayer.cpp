#include "EColLayer.h"
#include <vtkGeneralTransform.h>
#include <vtkGeoProjection.h>
#include <vtkGeoTransform.h>
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

  readCoordinates();
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
      // cout << "inserting point (" << pointId << "): " << out[0] << " " << out[1] << endl;
      points->InsertPoint(pointId++, out[0], out[1], 0);
      
      if (latIndex > 0 and lonIndex > 0 ) {
        int idx = latIndex+lonIndex*numLats;
        // cout << idx << " at " << lonIndex << " " << latIndex << endl;
        vtkNew<vtkIdList> l;
        l->SetNumberOfIds(4);
        l->SetId(0, idx-1);
        l->SetId(1, idx-numLats-1);
        l->SetId(2, idx-numLats);
        l->SetId(3, idx);
        // cout << "inserting cell: " << idx-1 << " " << idx-numLats-1 << " " << idx- numLats<< " " << idx << endl;
        double coords[12];
        points->GetPoint(idx-1, coords);
        points->GetPoint(idx-numLats-1, coords+3);
        points->GetPoint(idx-numLats, coords+6);
        points->GetPoint(idx, coords+9);
        // cout << "Inserting cell with points at: (" << coords[0] << " " << coords[1] << "), (" << coords[3] << " " << coords[4] << "), (" << coords[6] << " " << coords[7] << "), (" << coords[9] << " " << coords[10] << ")" << endl;
        data->InsertNextCell(VTK_QUAD, l);

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
        this->strength->SetTuple1(cellId++, std::sqrt(u*u + v*v));
      }
      latIndex++;
    }
    lonIndex++;
  }

  data->GetCellData()->AddArray(this->strength);
  data->GetCellData()->SetActiveScalars("strength");

  vtkNew<vtkPolyDataMapper>(mapper);
  mapper->SetInputData(data);
  mapper->Update();

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
      this->strength->SetTuple1(i++, std::sqrt(u*u + v*v));
    }
  }
  this->strength->Modified();
}

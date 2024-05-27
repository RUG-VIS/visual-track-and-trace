#include "LColLayer.h"
#include "../commands/SpawnPointCallback.h"
#include <vtkActor2D.h>
#include <vtkCellData.h>
#include <vtkGlyph2D.h>
#include <vtkLookupTable.h>
#include <vtkGlyphSource2D.h>
#include <vtkNamedColors.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleUser.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>

#include "../CartographicTransformation.h"

vtkSmartPointer<SpawnPointCallback> LColLayer::createSpawnPointCallback() {
  vtkNew<SpawnPointCallback> newPointCallBack;
  newPointCallBack->setPoints(this->points);
  newPointCallBack->setRen(this->ren);
  newPointCallBack->setUVGrid(this->uvGrid);
  newPointCallBack->setBeached(this->particlesBeached);
  newPointCallBack->setAge(this->particlesAge);
  return newPointCallBack;
}


/**
 */
// TODO: edit this function; probably extract all lut builders into a builder pattern or something.
vtkSmartPointer<vtkLookupTable> buildLutDens() {
  int n = 5;
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfColors(n);
  lut->SetTableRange(1, n+1);
  lut->SetScaleToLinear();
  lut->Build();
  for (int i=n-1; i >= 0; i--) {
    lut->SetTableValue(i, 0, 1-(0.75*i/(n-1)), 1, 1);
  }
  lut->UseAboveRangeColorOn();
  lut->SetAboveRangeColor(1,0,0,1);
  
  lut->UseBelowRangeColorOn();
  lut->SetBelowRangeColor(1,1,1,0);

  return lut;

}

// There's two separate pipelines going on here: the one where we build a vtkPoints array for the spawnpointcallback,
// and the one where we build a vtkPolyData with Cells for the colour map.
// TODO: it would make sense to separate these pipelines out to their own functions.
LColLayer::LColLayer(std::shared_ptr<UVGrid> uvGrid, std::unique_ptr<AdvectionKernel> advectionKernel) {
  // general management; probably should be the actual constructor.
  this->ren = vtkSmartPointer<vtkRenderer>::New();
  this->ren->SetLayer(2);

  this->uvGrid = uvGrid;
  this->numLats = uvGrid->latSize;
  this->numLons = uvGrid->lonSize;
  this->advector = std::move(advectionKernel);


  // pipeline 1
  this->points = vtkSmartPointer<vtkPoints>::New();

  this->particlesBeached = vtkSmartPointer<vtkIntArray>::New();
  this->particlesBeached->SetName("particlesBeached");
  this->particlesBeached->SetNumberOfComponents(1);

  this->particlesAge = vtkSmartPointer<vtkIntArray>::New();
  this->particlesAge->SetName("particlesAge");
  this->particlesAge->SetNumberOfComponents(1);


  // pipeline 2
  this->data = vtkSmartPointer<vtkPolyData>::New();
  vtkNew<vtkPoints> cellPoints;
  cellPoints->Allocate(numLats*numLons);
  this->data->SetPoints(cellPoints);
  data->Allocate((numLats-1)*(numLons-1));

  this->cellParticleDensity = vtkSmartPointer<vtkIntArray>::New();
  this->cellParticleDensity->SetName("cellParticleDensity");
  this->cellParticleDensity->SetNumberOfComponents(1);
  this->cellParticleDensity->SetNumberOfTuples((numLats-1)*(numLons-1));
  
  this->data->GetCellData()->AddArray(this->cellParticleDensity);
  this->data->GetCellData()->SetActiveScalars("cellParticleDensity");

  vtkSmartPointer<vtkTransformFilter> transformFilter = createCartographicTransformFilter(uvGrid);
  auto transform = transformFilter->GetTransform();

  int cellId = 0, pointId = 0;
  int latIndex = 0, lonIndex = 0;
  for (auto lon : uvGrid->lons) {
    latIndex = 0;
    for (auto lat : uvGrid->lats) {
      double out[3] = {lon, lat, 0};
      transform->TransformPoint(out, out);
      cellPoints->InsertPoint(pointId++, out[0], out[1], 0);
      
      // logic for adding cells
      if (latIndex > 0 and lonIndex > 0 ) {
        int idx = latIndex+lonIndex*numLats;
        vtkNew<vtkIdList> l;
        l->SetNumberOfIds(4);
        l->SetId(0, idx-1);
        l->SetId(1, idx-numLats-1);
        l->SetId(2, idx-numLats);
        l->SetId(3, idx);

        this->data->InsertNextCell(VTK_QUAD, l);
        this->cellParticleDensity->SetTuple1(cellId++, 0);
      }
      latIndex++;
    }
    lonIndex++;
  }

  vtkNew<vtkPolyDataMapper>(mapper);
  mapper->SetInputData(data);
  mapper->SetLookupTable(buildLutDens());
  mapper->UseLookupTableScalarRangeOn();
  mapper->Update();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0, 1, 0);
  actor->GetProperty()->SetOpacity(0.5);

  // vtkNew<vtkActor> act2;
  // act2->SetMapper(mapper);
  // act2->GetProperty()->SetRepresentationToWireframe();
  // this->ren->AddActor(act2);

  this->ren->AddActor(actor);

  this->callback = createSpawnPointCallback();
}

void LColLayer::spoofPoints() {
  for (int i=0; i < 330; i+=5) {
    for (int j=0; j < 330; j+=5) {
      this->points->InsertNextPoint(-15.875+(12.875+15.875)/330*j, 46.125+(62.625-46.125)/330*i, 0);
      this->particlesBeached->InsertNextValue(0);
      this->particlesAge->InsertNextValue(0);
    }
  }
  this->points->Modified();
}

// calculates the cellIndex for a given lat,lon pair.
int LColLayer::calcIndex(double x, double y) {
  int lonIndex = std::floor((x - uvGrid->lonMin()) / uvGrid->lonStep());
  int latIndex = std::floor((y - uvGrid->latMin()) / uvGrid->latStep());

  return latIndex+lonIndex*(uvGrid->latSize-1);
}


void printArray(vtkSmartPointer<vtkIntArray> data, int numLons, int numLats, int latsize) {
  for (int i=0; i < numLons-1; i++) {
    for (int j=0; j < numLats-1; j++) {
      int value = data->GetValue(j+i*latsize);
      if (value > 1)
        cout << "(" << i << ", " << j << ") ("<< j+i*latsize << "): " << value << endl;
    }
  }
  cout << endl;
}

void LColLayer::updateData(int t) {
  const int SUPERSAMPLINGRATE = 4;
  double point[3], oldX, oldY;
  bool modifiedData = false;
  this->cellParticleDensity->Fill(0);

  // iterate over every point.
  for (vtkIdType n=0; n < this->points->GetNumberOfPoints(); n++) {
    // first check: only update points within our grid's boundary.
    this->points->GetPoint(n, point);
    if (point[0] <= this->uvGrid->lonMin() or point[0] >= this->uvGrid->lonMax() or point[1] <= this->uvGrid->latMin() or point[1] >= this->uvGrid->latMax()) {
      // sets any particle out of bounds to be beached - so it gets assigned the right colour in the lookup table.
      this->particlesBeached->SetValue(n, this->beachedAtNumberOfTimes+1);
      this->particlesAge->SetValue(n, -1);
      continue;
    }
    
    // update particle age.
    int age = this->particlesAge->GetValue(n);
    if (age >= 0)
      this->particlesAge->SetValue(n, age+1);

    // second check: only update non-beached particles.
    int beachedFor = this->particlesBeached->GetValue(n);
    if (beachedFor < this->beachedAtNumberOfTimes-1) {

      oldX = point[0]; oldY = point[1];

      // supersampling
      for (int i=0; i < SUPERSAMPLINGRATE; i++) {
        std::tie(point[1], point[0]) = advector->advect(t, point[1], point[0], this->dt/SUPERSAMPLINGRATE);
      }

      // if the particle's location remains unchanged, increase beachedFor number. Else, decrease it and update point position.
      if (oldX == point[0] and oldY == point[1]) {
        this->particlesBeached->SetValue(n, beachedFor+1);
      } else {
        this->particlesBeached->SetValue(n, std::max(beachedFor-1, 0));
        this->points->SetPoint(n, point);
        modifiedData = true;
      }
    }
    // add point to cellparticleDensity
    int index = calcIndex(point[0], point[1]);
    this->cellParticleDensity->SetValue(index, cellParticleDensity->GetValue(index)+1);
  }
  if (modifiedData) {
    this->particlesAge->Modified();
    this->points->Modified();
    this->cellParticleDensity->Modified();
  }
  // printArray(this->cellParticleDensity, numLons, numLats, uvGrid->latSize-1);
}

void LColLayer::addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->AddObserver(vtkCommand::LeftButtonPressEvent, this->callback);
  interactor->AddObserver(vtkCommand::LeftButtonReleaseEvent, this->callback);
  interactor->AddObserver(vtkCommand::MouseMoveEvent, this->callback);
}

void LColLayer::removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
  interactor->RemoveObserver(this->callback);
}


void LColLayer::setDt(int dt) {
  this->dt = dt;
}

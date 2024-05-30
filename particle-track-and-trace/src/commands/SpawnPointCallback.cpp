#include "SpawnPointCallback.h"

#include <vtkSmartPointer.h>

#include "../CartographicTransformation.h"

void SpawnPointCallback::setPoints(const vtkSmartPointer<vtkPoints> &points) {
  this->points = points;
}

void SpawnPointCallback::setRen(const vtkSmartPointer<vtkRenderer> &ren) {
  this->ren = ren;
}

void SpawnPointCallback::setUVGrid(const std::shared_ptr<UVGrid> &uvGrid) {
  this->uvGrid = uvGrid;
  inverseCartographicProjection = createInverseCartographicTransformFilter(uvGrid)->GetTransform();
}

void SpawnPointCallback::setBeached(const vtkSmartPointer<vtkIntArray> &ints) {
  this->particlesBeached = ints;
}

void SpawnPointCallback::setAge(const vtkSmartPointer<vtkIntArray> &ints) {
  this->particlesAge = ints;
}


void SpawnPointCallback::setIdx(const vtkSmartPointer<vtkIntArray> &idx) {
  this->lutIdx = idx;
}

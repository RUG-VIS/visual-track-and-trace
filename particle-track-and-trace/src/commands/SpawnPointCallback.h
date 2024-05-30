#ifndef SPAWNPOINTCALLBACK_H
#define SPAWNPOINTCALLBACK_H

#include <memory>
#include <vtkAbstractTransform.h>
#include <vtkCallbackCommand.h>
#include <vtkIntArray.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>


#include "../advection/UVGrid.h"

class SpawnPointCallback : public vtkCallbackCommand {
protected:
  vtkSmartPointer<vtkPoints> points;
  vtkSmartPointer<vtkRenderer> ren;
  vtkSmartPointer<vtkIntArray> particlesBeached;
  vtkSmartPointer<vtkIntArray> particlesAge;
  vtkSmartPointer<vtkIntArray> lutIdx;
  std::shared_ptr<UVGrid> uvGrid;
  vtkSmartPointer<vtkAbstractTransform> inverseCartographicProjection;
  bool dragging = false;

public:
  virtual void setPoints(const vtkSmartPointer<vtkPoints> &points);

  virtual void setRen(const vtkSmartPointer<vtkRenderer> &ren);

  virtual void setBeached(const vtkSmartPointer<vtkIntArray> &parts);

  virtual void setAge(const vtkSmartPointer<vtkIntArray> &parts);
  virtual void setIdx(const vtkSmartPointer<vtkIntArray> &idx);

  virtual void setUVGrid(const std::shared_ptr<UVGrid> &uvGrid);
};

#endif

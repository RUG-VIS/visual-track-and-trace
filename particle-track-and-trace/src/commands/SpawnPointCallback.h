#ifndef SPAWNPOINTCALLBACK_H
#define SPAWNPOINTCALLBACK_H

#include <memory>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkAbstractTransform.h>

#include "../advection/UVGrid.h"

class SpawnPointCallback : public vtkCallbackCommand {

public:
  static SpawnPointCallback *New();

  SpawnPointCallback();

  void setPoints(const vtkSmartPointer<vtkPoints> &points);

  void setRen(const vtkSmartPointer<vtkRenderer> &ren);

  void setBeached(const vtkSmartPointer<vtkIntArray> &parts);

  void setAge(const vtkSmartPointer<vtkIntArray> &parts);
  void setIdx(const vtkSmartPointer<vtkIntArray> &idx);

  void setUVGrid(const std::shared_ptr<UVGrid> &uvGrid);

private:
  vtkSmartPointer<vtkPoints> points;
  vtkSmartPointer<vtkRenderer> ren;
  vtkSmartPointer<vtkIntArray> particlesBeached;
  vtkSmartPointer<vtkIntArray> particlesAge;
  vtkSmartPointer<vtkIntArray> lutIdx;
  std::shared_ptr<UVGrid> uvGrid;
  vtkSmartPointer<vtkAbstractTransform> inverseCartographicProjection;

  void Execute(vtkObject *caller, unsigned long evId, void *callData) override;

  bool dragging = false;
};


#endif //SPAWNPOINTCALLBACK_H

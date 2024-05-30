#ifndef MASSSPAWNPOINTCALLBACK_H
#define MASSSPAWNPOINTCALLBACK_H
#include <vtkCallbackCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkAbstractTransform.h>

#include "SpawnPointCallback.h"

class MassSpawnPointCallback : public SpawnPointCallback {

public:
  MassSpawnPointCallback();
  static MassSpawnPointCallback *New();

  void setUVGrid(const std::shared_ptr<UVGrid> &uvGrid) override;

private:
  double lats[2];
  double lons[2];

  void randomParticles(double x, double y);
  void addParticle(double x, double y);
  void Execute(vtkObject *caller, unsigned long evId, void *callData) override;

};


#endif //SPAWNPOINTCALLBACK_H

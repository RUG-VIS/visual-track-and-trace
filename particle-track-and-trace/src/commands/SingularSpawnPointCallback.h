#ifndef SINGULARSPAWNPOINTCALLBACK_H
#define SINGULARSPAWNPOINTCALLBACK_H

#include "SpawnPointCallback.h"

class SingularSpawnPointCallback : public SpawnPointCallback {

public:
  SingularSpawnPointCallback();
  static SingularSpawnPointCallback *New();

private:
  void Execute(vtkObject *caller, unsigned long evId, void *callData) override;

};


#endif //SINGULARSPAWNPOINTCALLBACK_H

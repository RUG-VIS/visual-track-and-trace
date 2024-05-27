#ifndef LCOLLAYER_H
#define LCOLLAYER_H

#include "Layer.h"
#include "../advection/kernel/AdvectionKernel.h"
#include "../commands/SpawnPointCallback.h"
#include <vtkPolyData.h>
#include <vtkInteractorStyle.h>

/** Implements the Layer class for the case of a Lagrangian visualization.
  * Specifically, this class models the Lagrangian particles in the simulation using the 'glyph' mark and 'transparency' channel to denote age.
  */
class LColLayer : public Layer {
private:
  vtkSmartPointer<vtkPoints> points;
  vtkSmartPointer<vtkPolyData> data;
  vtkSmartPointer<vtkIntArray> particlesBeached;
  vtkSmartPointer<vtkIntArray> particlesAge;
  vtkSmartPointer<vtkIntArray> cellParticleDensity;
  vtkSmartPointer<SpawnPointCallback> callback;
  std::unique_ptr<AdvectionKernel> advector;
  std::shared_ptr<UVGrid> uvGrid;
  int dt = 3600;
  int beachedAtNumberOfTimes = 20;
  int numLats;
  int numLons;

public:
  /** Constructor.
    */
  LColLayer(std::shared_ptr<UVGrid> uvGrid, std::unique_ptr<AdvectionKernel> advectionKernel);

  /**
   * This function spoofs a few points in the dataset. Mostly used for testing.
   */
  void spoofPoints();

  /**
   * updates the glyphs to reflect the given timestamp in the dataset.
   * @param t : the time at which to fetch the data.
   */
  void updateData(int t) override;

  vtkSmartPointer<SpawnPointCallback> createSpawnPointCallback();

  void addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;
  void removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;


  /** 
   * Sets a custom DT value, needed for advect calls to the simulation logic.
   */ 
  void setDt(int dt);
};

#endif

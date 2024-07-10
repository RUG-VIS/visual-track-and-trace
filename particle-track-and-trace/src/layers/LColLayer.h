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
  vtkSmartPointer<vtkPolyDataMapper> mapper;
  vtkSmartPointer<vtkIntArray> particlesBeached;
  vtkSmartPointer<vtkIntArray> particlesAge;
  vtkSmartPointer<vtkIntArray> lutIdx;
  vtkSmartPointer<vtkIntArray> cellParticleDensity;
  vtkSmartPointer<vtkIntArray> cellParticleAge;
  vtkSmartPointer<SpawnPointCallback> callback;
  std::vector<vtkSmartPointer<vtkLookupTable>> tables;
  ColourMode activeColourMode;
  SaturationMode activeSaturationMode;
  std::unique_ptr<AdvectionKernel> advector;
  std::shared_ptr<UVGrid> uvGrid;
  int dt = 3600;
  int beachedAtNumberOfTimes = 20;
  int numLats;
  int numLons;

  void buildLuts();

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

  /** This function sets up the spawnpointcallback.
   * @return pointer to a new spawnpointcallback.
   */
  vtkSmartPointer<SpawnPointCallback> createSpawnPointCallback();

  /** Sets a custom DT value, needed for advect calls to the simulation logic.  */ 
  void setDt(int dt);


  void addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;
  void removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor) override;
  void setColourMode(ColourMode mode) override;
  void setSaturationMode(SaturationMode mode) override;

};

 /** calculates the LookupTableIndex for a given particle age and density.
  * Assumes a 10x10 lookupTable where each column is a separate age-index and each row is a separate density-index.
  * @param age : int representing particle age.
  * @param density : int representing particle density in a given cell.
  * @return the index in a 10x10 lookupTable
  */
int calcIndex(const int age, const int density);


int calcCellIndex(const double u, const double v, const std::shared_ptr<UVGrid> uvGrid);

#endif

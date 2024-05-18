#ifndef ECOLLAYER_H
#define ECOLLAYER_H

#include "Layer.h"
#include <memory>
#include <vtkGeneralTransform.h>
#include <vtkPolyData.h>
#include <vtkTransformFilter.h>

#include "../advection/UVGrid.h"

/** Implements the Layer class for the case of a Eulerian visualization.
  * Specifically, this class models the eulerian flow-fields of the simulation using the 'colourmap' mark and 'hue' channel to denote strength of velocities.
  */
class EColLayer : public Layer {
private:
  vtkSmartPointer<vtkDoubleArray> strength;
  std::shared_ptr<UVGrid> uvGrid;
  int numLats;
  int numLons;

  /** This private function sets up the initial coordinates for the glyphs in the dataset.
    * It also reads some initial data to actually display.
    */
  void readCoordinates();


  /** This private function builds up the lookup table VTK uses when determning what colour each cell ought to be.
    */
  // TODO: implement this function.
  void buildLut();

public:
  /** Constructor.
    */
  EColLayer(std::shared_ptr<UVGrid> uvGrid);

  /** updates the map to reflect the given timestamp in the dataset.
    * @param t : the time at which to fetch the data.
    */
  void updateData(int t);

};


#endif

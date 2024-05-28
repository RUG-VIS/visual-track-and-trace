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
  vtkSmartPointer<vtkIntArray> lutIdx;
  vtkSmartPointer<vtkPolyDataMapper> mapper;
  std::vector<vtkSmartPointer<vtkLookupTable>> tables;
  ColourMode activeColourMode;
  SaturationMode activeSaturationMode;
  std::shared_ptr<UVGrid> uvGrid;
  double maxStrength;
  int numLats;
  int numLons;

  /** This private function sets up the initial coordinates for the glyphs in the dataset.
    * It also reads some initial data to actually display.
    */
  void readCoordinates();

  /** This function calculates the maximum strength values for the associated uvGrid
   */
  void calcMaxStrength();

  /** This function builds the used lookuptables and adds them to the tables attribute.
    */
  void buildLuts();

public:
  /** Constructor.
    */
  EColLayer(std::shared_ptr<UVGrid> uvGrid);

  /** updates the map to reflect the given timestamp in the dataset.
    * @param t : the time at which to fetch the data.
    */
  void updateData(int t) override;


  void setColourMode(ColourMode mode) override;
  void setSaturationMode(SaturationMode mode) override;

};


/** Calculates the discrete index of a given velocity strength, taking into account both strength and direction.
  * Maps to a 10*10 lookuptable, in which the columns are discrete colours (mapped to direction), and rows are discrete opacities (mapped to strength).
  * @param u : longitudinal velocity
  * @param v : latitudinal velocity
  * @param maxStren : maximum strength value
  * @return index in a 10*10 lookuptable.
  */
int calcIndex(double u, double v, double maxStren);

/** Calculates the discrete index of the angle of a velocity, mapping it to the range [0,10).
  * @param u : longitudinal velocity
  * @param v : latitudinal velocity
  * @return index in the range [0,10)
  */
int calcAngleIndex(double u, double v);

/** Calculates the discrete index of the strength of a velocity, mapping it to the range [0,10).
  * @param u : longitudinal velocity
  * @param v : latitudinal velocity
  * @param maxStren : maximum strength value
  * @return index in the range [0,10) 
  */
int calcStrengthIndex(double u, double v, double maxStren);

#endif

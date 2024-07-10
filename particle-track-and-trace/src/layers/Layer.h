#ifndef LAYER_H
#define LAYER_H

#include "enums.h"
#include <vtkInteractorStyle.h>
#include <vtkRenderer.h>

/** This class represents one abstract layer to be rendered to VTK.
  * It exists to manage multiple different layers under the Program class.
  */
class Layer {
protected:
  vtkSmartPointer<vtkRenderer> ren;

public:
  /** gets the vtkRenderer to assign it to the vtkRenderWindow of the program class.
   * @return pointer to the vtkRenderer of this class.
   */
  virtual vtkSmartPointer<vtkRenderer> getLayer();


  /** updates the data in the layer to reflect the given timestamp.
    * @param t : the timestamp which the data should reflect.
    */
  virtual void updateData(int t);

  /** Adds observers to the renderWindowinteractor within which this layer is active.
    * @param interactor : pointer to the interactor that observers can be added to.
    */
  virtual void addObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor);


  /** Removes observers to the renderWindowinteractor within which this layer is active.
    * @param interactor : pointer to the interactor that observers can be removed from.
    */
  virtual void removeObservers(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

  /** Sets the active camera for the vtkRenderer associated with this layer.
    * Used to share one camera between multiple layers.
    */ 
  virtual void setCamera(vtkSmartPointer<vtkCamera> cam);


  /** Sets the colour mode used by this layer.
    * Actual effect is implementation dependent, but can be expected to change the used lookupTable of the layer in some way.
    * Note that not all layers are required to implement this function, if they do not support changing colour configuration. 
    */
  virtual void setColourMode(ColourMode mode);


  /** Sets the saturation mode used by this layer.
    * Actual effect is implementation dependent, but can be expected to change the used lookupTable of the layer in some way.
    * Note that not all layers are required to implement this function, if they do not support changing saturation configuration. 
    */
  virtual void setSaturationMode(SaturationMode mode);

  /** Sets the glyph style used by this layer.
    * Actual effect is implementation dependent, but can be expected to change the shape of used glyphs in some way.
    * Note that not all layers are required to implement this function, if they do not render glyphs or do not support changing their style.
    */
  virtual void setGlyphStyle(GlyphStyle style);

  /** Sets the sampling mode used by this layer.
    * Actual effect is implementation dependent, but can be expected to change the sampling function used in some manner.
    * Note that not all layers are required to implement this function, if they do not support different sampling modes.
    */
  virtual void setSamplingMode(SamplingMode mode);
};

#endif

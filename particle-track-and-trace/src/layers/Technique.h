#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "Layer.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include "enums.h"

/** This class models one complete 'package' of layers.
  * It has a number of associated layers that, when put together, form one cohesive visualisation.
  * The class manages these layers itself, including un/rebinding to a given renderWindow and managing various settings.
  */
class Technique {
private:
  /** vector of associated layers */
  std::vector<Layer *> layers;

  /** associated camera instance, added to all associated layers */
  vtkSmartPointer<vtkCamera> cam;

public:
  /** Constructor. */
  Technique(vtkSmartPointer<vtkCamera> cam);

  /** This function adds a new layer to the technique.
    * @ @param t : pointer to the layer to be added
    */
  void addLayer(Layer *l);

  /** This function removes a layer from the technique.
  * @ param t : pointer to the layer to be removed
  */
  void removeLayer(Layer *l);

  /** This function simply passes through to every layer's updateData function.
  * @ param t : the timestamp to update to.
  */
  void updateData(int t); 

  /** Gets the number of layers in this technique.
   * @return : number of layers
  */
  int numberOfLayers();

  /** Binds every layer in the technique to the given window and interactor.
  * @param win : renderWindow to bind the layers to.
  * @param intr : interactor to bind the layers to.
  */
  void bind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr); 


  /** Unbinds every layer in the technique to the given window and interactor.
  * @param win : renderWindow to unbind the layers from.
  * @param intr : interactor to unbind the layers from.
  */
  void unbind(vtkSmartPointer<vtkRenderWindow> win, vtkSmartPointer<vtkRenderWindowInteractor> intr);

  /** Sets the colour mode for every associated layer. 
  * @param mode : the mode to set the layers to.
  */
  void setColourMode(ColourMode mode);

  /** Sets the saturation mode for every associated layer.
   * @param mode : the mode to set the layers to.
  */
  void setSaturationMode(SaturationMode mode);

  /** Sets the glyph style for every associated layer.
  * @param style : the style to set the layers to.
  */
  void setGlyphStyle(GlyphStyle style);

  /** Sets the sampling mode for every associated layer.
  * @param mode : the mode to set the layers to.
  */
  void setSamplingMode(SamplingMode mode);
};



#endif

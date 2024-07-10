#ifndef PROGRAM_H
#define PROGRAM_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "layers/Technique.h"

/** This class manages the upper levels of the vtk pipeline; it has attributes for the vtkrenderWindow and a vector of Techniques to represent a variable number of vtkRenderers.
  * It can also set up a vtkTimer by connecting an instance of TimerCallbackCommand with its contained vtkRenderWindowInteractor.
  */ 
class Program : public QVTKOpenGLNativeWidget {
  Q_OBJECT

private:
  /** This attribute models a variable number of vtkRenderers, managed through the abstract Technique class.
    */ 
  std::vector<Technique *> techniques;
  ActiveTechnique activeTech;


  /** The window this program's layers render to.
    */ 
  vtkSmartPointer<vtkGenericOpenGLRenderWindow> win;

  /** The interactor through which the layers can interact with the window.
    */ 
  vtkSmartPointer<vtkRenderWindowInteractor> interact;


  /** The camera used by all layers for this program.
    */
  vtkSmartPointer<vtkCamera> cam;

  /** This function sets some default properties on the vtkRenderWindow. Extracted to its' own function to keep the constructor from becoming cluttered.
    */ 
  void setWinProperties();

  /** This function sets up the camera's associated movement callbacks..
    */ 
  void setupCameraCallback();

public:
  /** Constructor.
    */ 
  Program(QWidget *parent = nullptr);
  ~Program() override;

  /** This function adds a new technique to the program.
    * The technique is expected to manage the layers itself in the vtkRenderWindow layer system.
    * @param technique : pointer to the technique to add.
    */ 
  void addTechnique(Technique *technique);

  /** This function removes a given layer from the vtkRenderWindow and layers vector.
    * If the given layer is not actually in the program, nothing happens.
    * @param layer : the layer to removeTechnique
    */
  void removeTechnique(Technique *layer);

  /** This function updates the data for the associated techniques to the given timestamp.
    * Also updates the renderWindow.
    * @param t : the timestamp to update the data to.
    */
  void updateData(int t);

  /** This function calls the instance's renderWindow->Render() method.
    */
  void requestRender();

  /** This function switches the active technique of the instance. 
    * To do so it unbinds the current active technique, then binds the new one.
    */
  void setActiveTechnique(ActiveTechnique tech);

  /** Getter for the cam attribute.
    */
  vtkSmartPointer<vtkCamera> getCamera();

  /** Getter for the techniques attribute.
    */
  std::vector<Technique *> getTechniques();

  /** Getter for the interact attribute.
    */ 
  vtkSmartPointer<vtkRenderWindowInteractor> getInteractor();
};

#endif

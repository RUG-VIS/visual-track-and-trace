#ifndef LUTS_H
#define LUTS_H
#include <vtkLookupTable.h>

// LGlyph tables
vtkSmartPointer<vtkLookupTable> buildComplementary(); 
vtkSmartPointer<vtkLookupTable> buildDesaturated(); 

// ECol tables
vtkSmartPointer<vtkLookupTable> buildCyclicComplementary(); 
vtkSmartPointer<vtkLookupTable> buildCyclicContrasting(); 
vtkSmartPointer<vtkLookupTable> buildCyclicMonochromatic(); 
vtkSmartPointer<vtkLookupTable> buildCyclicDesaturated(); 

// LCol tables
vtkSmartPointer<vtkLookupTable> buildDensityComplementary(); 
vtkSmartPointer<vtkLookupTable> buildDensityContrasting(); 
vtkSmartPointer<vtkLookupTable> buildDensityMonochromatic(); 
vtkSmartPointer<vtkLookupTable> buildDensityDesaturated(); 
#endif

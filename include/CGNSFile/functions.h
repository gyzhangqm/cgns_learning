#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "CGNSFile/structure.h"

void getUserInput3D(int argc, char *argv[], cgns_unstructured_file *data);
void getUserInput2D(int argc, char *argv[], cgns_unstructured_file *data);

void generateCGNSFile(cgns_unstructured_file *data);
void openCGNSFile(cgns_unstructured_file *data);
void generateBase(cgns_unstructured_file *data);

void generateZone3D(cgns_unstructured_file *data);
void generateCoordinates3D(cgns_unstructured_file *data);
void generateElementsConnectivity3D(cgns_unstructured_file *data);
void generateBoundaryQuadrangleElements3D(cgns_unstructured_file *data);

void generateZone2D(cgns_unstructured_file *data);
void generateCoordinates2D(cgns_unstructured_file *data);
void generateElementsConnectivity2D(cgns_unstructured_file *data);
void generateElementsConnectivity2D_triangle(cgns_unstructured_file *data);
void generateBoundaryLines2D(cgns_unstructured_file *data);

void generateSimulationType_TimeAccurate(cgns_unstructured_file *data);

/* Private */
void verifyUserInput3D(int argc);
void getNumberOfVerticesFromUserInput3D(int argc, char *argv[], cgns_unstructured_file *data);
void getDistancesFromUserInput3D(int argc, char *argv[], cgns_unstructured_file *data);

void verifyUserInput2D(int argc);
void getNumberOfVerticesFromUserInput2D(int argc, char *argv[], cgns_unstructured_file *data);
void getDistancesFromUserInput2D(int argc, char *argv[], cgns_unstructured_file *data);

#endif

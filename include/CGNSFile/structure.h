#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cgnslib.h>

#define CHKERRQ(err) if((err)) cg_error_exit()

#define MAX_STRING_LEN 200

typedef enum Dimension {two_dimensional, three_dimensional} Dimension;

typedef struct cgns_unstructured_file
{
	Dimension dimension;
	char fileName[MAX_STRING_LEN];
	char baseName[MAX_STRING_LEN];
	char zoneName[MAX_STRING_LEN];
	char solutionName[MAX_STRING_LEN];
	char gridName[MAX_STRING_LEN];

	int nx, ny, nz;
	double lengthX, lengthY, lengthZ;
	double dx, dy, dz;

	int file, base, zone, solution, grid;
	int cellDimension, physicalDimension;
	ZoneType_t zonetype;
	cgsize_t size[9];
	int numberOfVertices, numberOfElements;

	int coorX, coorY, coorZ;
	double *x, *y, *z;

	int lastElementNumber;

	char gridConnectivitySectionName[MAX_STRING_LEN];
	int gridConnectivitySection;

	char topBoundarySectionName[MAX_STRING_LEN];
	char bottomBoundarySectionName[MAX_STRING_LEN];
	char northBoundarySectionName[MAX_STRING_LEN];
	char southBoundarySectionName[MAX_STRING_LEN];
	char eastBoundarySectionName[MAX_STRING_LEN];
	char westBoundarySectionName[MAX_STRING_LEN];
	int topBoundarySection;
	int bottomBoundarySection;
	int northBoundarySection;
	int southBoundarySection;
	int eastBoundarySection;
	int westBoundarySection;
} cgns_unstructured_file;

#endif

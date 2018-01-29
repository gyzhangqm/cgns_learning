#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

#define PHYSICAL_DIMENSION 3
#define CELL_DIMENSION 3

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	getUserInput3D(argc, argv, &data);

	strcpy(data.fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns");
	strcpy(data.baseName, "Simple structured grid");
	strcpy(data.zoneName, "Grid");
	strcpy(data.gridConnectivitySectionName, "Hexaedron conectivity");

	data.cellDimension = CELL_DIMENSION;
	data.physicalDimension = PHYSICAL_DIMENSION;

	generateCGNSFile(&data);
	generateBase(&data);
	generateZone3D(&data);
	generateCoordinates3D(&data);
	generateElementsConnectivity3D(&data);
	generateBoundaryQuadrangleElements3D(&data);

	cg_close(data.file);

	printf("\nSuccessfully wrote grid to file '%s'\n\n", data.fileName);

	exit(EXIT_SUCCESS);
}

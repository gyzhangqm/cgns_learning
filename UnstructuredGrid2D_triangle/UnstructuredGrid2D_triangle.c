#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

#define PHYSICAL_DIMENSION 2
#define CELL_DIMENSION 2

int main(int argc, char *argv[])
{
	int err;
	cgns_unstructured_file data;

	getUserInput2D(argc, argv, &data);

	strcpy(data.fileName, "/home/guesser/cgns_examples/output/UnstructuredGrid2D_triangle.cgns");
	strcpy(data.baseName, "Simple structured grid");
	strcpy(data.zoneName, "Grid");
	strcpy(data.gridConnectivitySectionName, "triangle conectivity");

	data.cellDimension = CELL_DIMENSION;
	data.physicalDimension = PHYSICAL_DIMENSION;

	generateCGNSFile(&data);
	generateBase(&data);
	generateZone2D_triangle(&data);
	generateCoordinates2D(&data);
	generateElementsConnectivity2D_triangle(&data);
	generateBoundaryLines2D(&data);

	cg_close(data.file);

	printf("\nSuccessfully wrote grid to file '%s'\n\n", data.fileName);

	exit(EXIT_SUCCESS);
}

#include "CGNSFile/structure.h"
#include "CGNSFile/functions.h"

void getUserInput2D(int argc, char *argv[], cgns_unstructured_file *data)
{
	verifyUserInput2D(argc);
	getNumberOfVerticesFromUserInput2D(argc, argv, data);
	getDistancesFromUserInput2D(argc, argv, data);
}

void verifyUserInput2D(int argc)
{
	if(argc!=3 && argc!=5)
	{
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "\tprogram NX NY\n");
		fprintf(stdout, "or\n");
		fprintf(stdout, "\tprogram NX NY LX LY\n");
		exit(EXIT_FAILURE);
	}
	return ;
}

void getNumberOfVerticesFromUserInput2D(int argc, char *argv[], cgns_unstructured_file *data)
{
	data->nx = atoi(argv[1]);
	data->ny = atoi(argv[2]);
	return ;
}

void getDistancesFromUserInput2D(int argc, char *argv[], cgns_unstructured_file *data)
{
	if(argc==3)
	{
		data->lengthX = data->nx;
		data->lengthY = data->ny;
	}
	if(argc==5){
		data->lengthX = atof(argv[3]);
		data->lengthY = atof(argv[4]);
	}
	data->dx = data->lengthX/(data->nx-1);
	data->dy = data->lengthY/(data->ny-1);
	return ;
}

void getUserInput3D(int argc, char *argv[], cgns_unstructured_file *data)
{
	verifyUserInput3D(argc);
	getNumberOfVerticesFromUserInput3D(argc, argv, data);
	getDistancesFromUserInput3D(argc, argv, data);
}

void verifyUserInput3D(int argc)
{
	if(argc!=4 && argc!=7)
	{
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "\tprogram NX NY NZ\n");
		fprintf(stdout, "or\n");
		fprintf(stdout, "\tprogram NX NY NZ LX LY LZ\n");
		exit(EXIT_FAILURE);
	}
	return ;
}

void getNumberOfVerticesFromUserInput3D(int argc, char *argv[], cgns_unstructured_file *data)
{
	data->nx = atoi(argv[1]);
	data->ny = atoi(argv[2]);
	data->nz = atoi(argv[3]);
	return ;
}
void getDistancesFromUserInput3D(int argc, char *argv[], cgns_unstructured_file *data)
{
	if(argc==4)
	{
		data->lengthX = data->nx;
		data->lengthY = data->ny;
		data->lengthZ = data->nz;
	}
	if(argc==7){
		data->lengthX = atof(argv[4]);
		data->lengthY = atof(argv[5]);
		data->lengthZ = atof(argv[6]);
	}
	data->dx = data->lengthX/(data->nx-1);
	data->dy = data->lengthY/(data->ny-1);
	data->dz = data->lengthZ/(data->nz-1);
	return ;
}

void generateCGNSFile(cgns_unstructured_file *data)
{
	int err;
	err = cg_open(data->fileName, CG_MODE_WRITE, &(data->file)); CHKERRQ(err);
	return ;
}

void openCGNSFile(cgns_unstructured_file *data)
{
	int err;
	err = cg_open(data->fileName, CG_MODE_MODIFY, &(data->file)); CHKERRQ(err);
	return ;
}

void generateBase(cgns_unstructured_file *data)
{
	int err;
	err = cg_base_write(data->file, data->baseName, data->cellDimension, data->physicalDimension, &(data->base)); CHKERRQ(err);
	return ;
}

void generateZone3D(cgns_unstructured_file *data)
{
	int err;
	cgsize_t size[3];
	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;

	/* Define zone size */
	size[0] = NX*NY*NZ;
	size[1] = (NX-1)*(NY-1)*(NZ-1);
	size[2] = 0;

	/* Open zone */
	err = cg_zone_write(data->file, data->base, data->zoneName, size, Unstructured, &(data->zone)); CHKERRQ(err);

	return;
}

void generateZone2D(cgns_unstructured_file *data)
{
	int err;
	cgsize_t size[3];
	const int NX = data->nx;
	const int NY = data->ny;

	/* Define zone size */
	size[0] = NX*NY;
	size[1] = (NX-1)*(NY-1);
	size[2] = 0;

	/* Open zone */
	err = cg_zone_write(data->file, data->base, data->zoneName, size, Unstructured, &(data->zone)); CHKERRQ(err);

	return;
}

void generateCoordinates3D(cgns_unstructured_file *data)
{
	int err;
	int i, j, k, entry;
	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;

	double * x;
	double * y;
	double * z;

	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*NZ*sizeof(double**));
	y = (double *) malloc(NX*NY*NZ*sizeof(double**));
	z = (double *) malloc(NX*NY*NZ*sizeof(double**));

	/* Generate coordinates values */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			for(k=0 ; k<NZ ; ++k)
			{
				entry = i + j*NX + k*NX*NY;
				x[entry] = i*data->dx;
				y[entry] = j*data->dy;
				z[entry] = k*data->dz;
			}
		}
	}
	/* Write coordinates */
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorXName, x, &(data->coorX)); CHKERRQ(err);
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorYName, y, &(data->coorY)); CHKERRQ(err);
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorZName, z, &(data->coorZ)); CHKERRQ(err);

	free(x);
	free(y);
	free(z);
}

void generateCoordinates2D(cgns_unstructured_file *data)
{
	int err;
	int i, j, entry;
	const int NX = data->nx;
	const int NY = data->ny;

	double * x;
	double * y;

	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*sizeof(double**));
	y = (double *) malloc(NX*NY*sizeof(double**));

	/* Generate coordinates values */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			entry = i + j*NX;
			x[entry] = i*data->dx;
			y[entry] = j*data->dy;
		}
	}
	/* Write coordinates */
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorXName, x, &(data->coorX)); CHKERRQ(err);
	err = cg_coord_write(data->file, data->base, data->zone, CGNS_ENUMV(RealDouble), coorYName, y, &(data->coorY)); CHKERRQ(err);

	free(x);
	free(y);
}

void generateElementsConnectivity3D(cgns_unstructured_file *data)
{
	int i, j, k;
	int numberOfElements, elementNumber, firstVerticeIndex;
	cgsize_t *hexaedronConnectivity;
	cgsize_t firstElementNumber = 1;
	cgsize_t lastElementNumber;

	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;

	/* Elements conectivity */
		/* 0 <= elementNumber <= (NX-1)*(NY-1)*(NZ-1) */
	numberOfElements = (NX-1)*(NY-1)*(NZ-1);
	hexaedronConnectivity = (cgsize_t *) malloc(8*numberOfElements*sizeof(cgsize_t));
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			for(k=0 ; k<(NZ-1) ; ++k)
			{
				elementNumber = i + (NX-1)*j + (NX-1)*(NY-1)*k;
				firstVerticeIndex = i + j*NX + k*NX*NY;
				hexaedronConnectivity[8*elementNumber+0] = 1 + firstVerticeIndex;
				hexaedronConnectivity[8*elementNumber+1] = 1 + firstVerticeIndex + 1;
				hexaedronConnectivity[8*elementNumber+2] = 1 + firstVerticeIndex + NX + 1;
				hexaedronConnectivity[8*elementNumber+3] = 1 + firstVerticeIndex + NX;
				hexaedronConnectivity[8*elementNumber+4] = 1 + firstVerticeIndex + NX*NY;
				hexaedronConnectivity[8*elementNumber+5] = 1 + firstVerticeIndex + NX*NY + 1;
				hexaedronConnectivity[8*elementNumber+6] = 1 + firstVerticeIndex + NX*(NY+1) + 1;
				hexaedronConnectivity[8*elementNumber+7] = 1 + firstVerticeIndex + NX*(NY+1);
			}
		}
	}
	lastElementNumber = numberOfElements;
	cg_section_write(data->file, data->base, data->zone, data->gridConnectivitySectionName, CGNS_ENUMV(HEXA_8), firstElementNumber, lastElementNumber, 0, hexaedronConnectivity, &(data->gridConnectivitySection));
	data->lastElementNumber = lastElementNumber;

	free(hexaedronConnectivity);
}

void generateBoundaryQuadrangleElements3D(cgns_unstructured_file *data)
{
	int i, j, k, err;
	int numberOfElements, elementNumber, firstVerticeIndex;
	char boundaryName[MAX_STRING_LEN];
	int firstElementNumber, lastElementNumber;
	const int NX = data->nx;
	const int NY = data->ny;
	const int NZ = data->nz;
	cgsize_t * quadrangleConnectivity;

	/* bottom */
	strcpy(data->bottomBoundarySectionName, "bottom boundary");
	numberOfElements = (NX-1)*(NY-1);
	quadrangleConnectivity = (cgsize_t *) malloc(4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	k = 0;
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			elementNumber = i + (NX-1)*j;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + NX;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX + 1;
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + 1;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->bottomBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->bottomBoundarySection));
	data->lastElementNumber = lastElementNumber;

	/* top */
	strcpy(data->topBoundarySectionName, "top boundary");
	numberOfElements = (NX-1)*(NY-1);
	quadrangleConnectivity = (cgsize_t *) realloc(quadrangleConnectivity, 4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	k = NZ - 1;
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			elementNumber = i + (NX-1)*j;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + 1;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX + 1;
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + NX;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->topBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->topBoundarySection));
	data->lastElementNumber = lastElementNumber;

	/* north */
	strcpy(data->northBoundarySectionName, "north boundary");
	numberOfElements = (NX-1)*(NZ-1);
	quadrangleConnectivity = (cgsize_t *) realloc(quadrangleConnectivity, 4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	j = NY - 1;
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(k=0 ; k<(NZ-1) ; ++k)
		{
			elementNumber = i + (NX-1)*k;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + NX*NY;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX*NY + 1;
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + 1;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->northBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->northBoundarySection));
	data->lastElementNumber = lastElementNumber;

	/* south */
	strcpy(data->southBoundarySectionName, "south boundary");
	numberOfElements = (NX-1)*(NZ-1);
	quadrangleConnectivity = (cgsize_t *) realloc(quadrangleConnectivity, 4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	j = 0;
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(k=0 ; k<(NZ-1) ; ++k)
		{
			elementNumber = i + (NX-1)*k;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + 1;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX*NY + 1;
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + NX*NY;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->southBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->southBoundarySection));
	data->lastElementNumber = lastElementNumber;

	/* east */
	strcpy(data->eastBoundarySectionName, "east boundary");
	numberOfElements = (NY-1)*(NZ-1);
	quadrangleConnectivity = (cgsize_t *) realloc(quadrangleConnectivity, 4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	i = NX - 1;
	for(j=0 ; j<(NY-1) ; ++j)
	{
		for(k=0 ; k<(NZ-1) ; ++k)
		{
			elementNumber = j + (NY-1)*k;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + NX;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX*(NY+1);
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + NX*NY;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->eastBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->eastBoundarySection));
	data->lastElementNumber = lastElementNumber;

	/* west */
	strcpy(data->westBoundarySectionName, "west boundary");
	numberOfElements = (NY-1)*(NZ-1);
	quadrangleConnectivity = (cgsize_t *) realloc(quadrangleConnectivity, 4*numberOfElements*sizeof(cgsize_t));
	firstElementNumber = data->lastElementNumber + 1;
	i = 0;
	for(j=0 ; j<(NY-1) ; ++j)
	{
		for(k=0 ; k<(NZ-1) ; ++k)
		{
			elementNumber = j + (NY-1)*k;
			firstVerticeIndex = i + j*NX + k*NX*NY;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + NX*NY;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX*(NY+1);
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + NX;
		}
	}
	lastElementNumber = firstElementNumber + numberOfElements - 1;
	cg_section_write(data->file, data->base, data->zone, data->westBoundarySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->westBoundarySection));
	data->lastElementNumber = lastElementNumber;

	free(quadrangleConnectivity);
	return;
}

void generateElementsConnectivity2D(cgns_unstructured_file *data)
{
	int i, j;
	int numberOfElements, elementNumber, firstVerticeIndex;
	cgsize_t *quadrangleConnectivity;
	cgsize_t firstElementNumber = 1;
	cgsize_t lastElementNumber;

	const int NX = data->nx;
	const int NY = data->ny;

	numberOfElements = (NX-1)*(NY-1);
	quadrangleConnectivity = (cgsize_t *) malloc(4*numberOfElements*sizeof(cgsize_t));
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			elementNumber = i + (NX-1)*j;
			firstVerticeIndex = i + j*NX;
			quadrangleConnectivity[4*elementNumber+0] = 1 + firstVerticeIndex;
			quadrangleConnectivity[4*elementNumber+1] = 1 + firstVerticeIndex + 1;
			quadrangleConnectivity[4*elementNumber+2] = 1 + firstVerticeIndex + NX + 1;
			quadrangleConnectivity[4*elementNumber+3] = 1 + firstVerticeIndex + NX;
		}
	}
	lastElementNumber = numberOfElements;
	cg_section_write(data->file, data->base, data->zone, data->gridConnectivitySectionName, CGNS_ENUMV(QUAD_4), firstElementNumber, lastElementNumber, 0, quadrangleConnectivity, &(data->gridConnectivitySection));

	free(quadrangleConnectivity);
	return ;
}

void generateSimulationType_TimeAccurate(cgns_unstructured_file *data)
{
	int err;
	err = cg_simulation_type_write(data->file, data->base, CGNS_ENUMV(TimeAccurate)); CHKERRQ(err);
	return;
}

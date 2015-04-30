#include "simpleCL.h"
#include <math.h>

#define aSizeX 3
#define aSizeY 3

#define bSizeX aSizeY
#define bSizeY 3

typedef struct{
	int cols;
	int rows;
	int dataSize;
	void* dataStart;
} matrix;

void printMatrix(matrix ma)
{
	if(ma.dataStart == NULL) printf("fuck\n");
	printf("META: %d %d %d\n", ma.cols, ma.rows, ma.cols * ma.rows);
	for(int i = 0; i < ma.cols*ma.rows; i++){
		//printf("%d", ((int*)ma.dataStart)[i]);
		printf("%d %c", ((int*)ma.dataStart)[i], i % ma.cols == ma.cols - 1 ? '\n' : ' ');
	}
}

matrix makeIntMatrix(int rows, int cols)
{
	matrix ma;
	ma.cols = cols;
	ma.rows = rows;
	ma.dataSize = sizeof(int);
	int nextCols = pow(2, ceil(log(cols)/log(2)));
	int nextRows = pow(2, ceil(log(rows)/log(2)));
	printf("%d %d %d %d\n", ma.cols, nextCols, ma.rows, nextRows);
	ma.dataStart = calloc(ma.cols*ma.rows, ma.dataSize);

	return ma;
}

void fillIntMatrix(matrix m)
{
	for (int i = 0; i < m.rows; ++i)
	{
		for (int j = 0; j < m.cols; ++j)
		{
			((int*)m.dataStart)[i*m.cols+j] = j;
		}
	}
}

int main()
{
	/* Declare matrices */
	matrix ma = makeIntMatrix(aSizeX,aSizeY);
	matrix mb = makeIntMatrix(bSizeX,bSizeY);
	matrix result = makeIntMatrix(aSizeX, bSizeY);

	//printMatrix(result);

	fillIntMatrix(mb);
	fillIntMatrix(ma);

	printMatrix(ma);
	printMatrix(mb);
/*
	for(int i = 0; i < result.rows; i++) {
        /* Inner loop for each column,
         * The first values iterated over are the first ones in the first row
         * This make the cache use more efficient.
         *
        for(int j = 0; j < result.cols; j++) {
            for(int k = 0; k < mb.rows; k++) {
            /* this is the m mentioned in the topmost description,
             * and the number of multiplications made for each index in the resulting matrix *
                ((int*)result.dataStart)[i * result.cols + j] += ((int*)ma.dataStart)[i * ma.cols + k] * ((int*)mb.dataStart)[k * mb.cols + j];
            }
        }
    }
*/
	size_t global_size[2];
	size_t local_size[2];
	size_t maDataLength = ma.cols*ma.rows;
	size_t maDataSize = ma.dataSize*maDataLength;
	size_t mbDataLength = mb.cols*mb.rows;
	size_t mbDataSize = mb.dataSize*mbDataLength;

	global_size[0] = aSizeX;
	local_size[0] = 1;
	global_size[1] = bSizeY;
	local_size[1] = 1;

/*	 OpenCL Hardware and software */
	sclHard hardware;
	sclSoft software;
	int found = 0;
	hardware = sclGetCPUHardware( 0, &found );
	software = sclGetCLSoftware("MatVecMulKernal.cl", "MatVecMul", hardware);


	sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%r %r %R %a %a %a %a",
		maDataSize, ma.dataStart, mbDataSize, mb.dataStart, aSizeX * bSizeY, result.dataStart,
		sizeof(int), &ma.rows, sizeof(int), &ma.cols, sizeof(int), &mb.rows, sizeof(int), &mb.cols);

	printMatrix(result);
	//printf("%d\n", ((int*)ma.dataStart)[0]);
	
}

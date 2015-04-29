#include "simpleCL.h"

#define aSizeX 2	
#define aSizeY 1

#define bSizeX aSizeY
#define bSizeY 1


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
	fillIntMatrix(result);

	//printMatrix(ma);
	//printMatrix(mb);

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

	/* OpenCL Hardware and software*/
	sclHard hardware;
	sclSoft software;
	int found = 0;
	hardware = sclGetGPUHardware( 0, &found );
	software = sclGetCLSoftware("MatVecMulKernal.cl", "MatVecMul", hardware);


	sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%r %r %R %a %a %a",
		maDataSize, ma.dataStart, mbDataSize, mb.dataStart, mbDataSize, result.dataStart,
		sizeof(int), &ma.rows, sizeof(int), &mb.cols, sizeof(int), &ma.cols);


	//printMatrix(result);
	printf("%d\n", ((int*)ma.dataStart)[0]);
	
}

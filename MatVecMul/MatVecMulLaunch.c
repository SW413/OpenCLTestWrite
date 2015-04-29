#include "simpleCL.h"

typedef struct{
	int cols;
	int rows;
	int dataSize;
	void* dataStart;
} matrix;

void printMatrix(matrix ma){

	for(int i = 0; i < ma.cols*ma.rows; i++){
	printf("%d %c", ((int*)ma.dataStart)[i], i % ma.cols == ma.cols - 1 ? '\n' : ' ');
}
}

int main(){
sclHard hardware;
sclSoft software;

matrix ma;
ma.cols = 4;
ma.rows = 4;
ma.dataSize = sizeof(int);
ma.dataStart = calloc(ma.cols*ma.rows, ma.dataSize);

for (int i = 0; i < ma.rows; ++i)
{
	for (int j = 0; j < ma.cols; ++j)
	{
		((int*)ma.dataStart)[i*ma.cols+j] = j;
	}
}

matrix mb;
mb.cols = 5;
mb.rows = 4;
mb.dataSize = sizeof(int);
mb.dataStart = calloc(mb.cols*mb.rows, mb.dataSize);

for (int i = 0; i < mb.rows; ++i)
{
	for (int j = 0; j < mb.cols; ++j)
	{
		((int*)mb.dataStart)[i*mb.cols+j] = j;
	}
}
puts("Matrix A:\n");
printMatrix(ma);
puts("Matrix B:\n");
printMatrix(mb);


size_t global_size[2];
size_t local_size[2];
size_t maDataLength = ma.cols*ma.rows;
size_t maDataSize = ma.dataSize*maDataLength;
size_t mbDataLength = mb.cols*mb.rows;
size_t mbDataSize = mb.dataSize*mbDataLength;

global_size[0] = ma.rows;
local_size[0] = 1;
global_size[1] = mb.cols;
local_size[1] = 1;

int found = 0;
//allHardware = sclGetAllHardware(&found);
//hardware = sclGetFastestDevice(allHardware, found);
    hardware = sclGetCPUHardware( 0, &found );


software = sclGetCLSoftware("MatVecMulKernal.cl", "MatVecMul", hardware);

matrix result;
result.cols = mb.cols;
result.rows = ma.rows;
result.dataSize = sizeof(int);
result.dataStart = calloc(result.cols*result.rows, result.dataSize);
sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%r %r %R %a %a %a",
	maDataSize, ma.dataStart, mbDataSize, mb.dataStart, mbDataSize, result.dataStart,
	sizeof(int), &ma.rows, sizeof(int), &mb.cols, sizeof(int), &ma.cols);

//puts("\n new matrix \n");
printMatrix(result);

}

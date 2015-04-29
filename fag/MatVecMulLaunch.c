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
	
sclHard* allHardware;
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
mb.datasize = sizeof(int);
mb.dataStart = calloc(ma.cols*ma.rows, mb.dataSize);

for (int i = 0; i < mb.rows; ++i)
{
	for (int j = 0; j < mb.cols; ++j)
	{
		((int*)mb.dataStart)[i*mb.cols+j] = j;
	}
}
puts("Matrix A:\n")
printMatrix(ma);
puts("Matrix B:\n");
printMatrix(mb);


size_t global_size[2];
size_t local_size[2];
size_t maDataLength = ma.cols*mb.rows;
size_t maDataSize = ma.dataSize*dataLength;
size_t mbDataLength = mb.cols*mb.rows;
size_t mbDataSize = mb.datasize*dataLength;

global_size[0] = maDataLength;
local_size[0] = 4;
global_size[1] = mbDataLength;
local_size[1] = 5;
int scalar = 2;

int found = 0;
allHardware = sclGetAllHardware(&found);
hardware = sclGetFastestDevice(allHardware, found);
software = sclGetCLSoftware("MatVecMulKernal.cl", "MatVecMul", hardware);

int result[20];
sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%r %r %w",
	maDataSize, ma.dataStart, mbDataSize, mb.dataStart, mbDataSize, result);

puts("\n new matrix \n");
printMatrix(result);

}
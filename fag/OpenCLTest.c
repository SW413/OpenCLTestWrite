#include "simpleCL.h"

#pragma OPENCL EXTENSION cl_intel_printf : enable
#pragma OPENCL EXTENSION cl_amd_printf : enable

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

printMatrix(ma);
/*
int a[] = {1,2,3,4,5,6,7,8,9};
int b[] = {9,8,7,6,5,4,3,2,1};
int result[9];

size_t global_size[2];
size_t local_size[2];
size_t dataLength = 9;
size_t dataSize = sizeof(int)*dataLength;

global_size[0] = dataLength;
local_size[0] = 1;
global_size[1] = 1;
local_size[1] = 1;*/

size_t global_size[2];
size_t local_size[2];
size_t dataLength = ma.cols*ma.rows;
size_t dataSize = ma.dataSize*dataLength;

global_size[0] = dataLength;
local_size[0] = 4;
global_size[1] = 1;
local_size[1] = 1;
int scalar = 2;

int found = 0;
//allHardware = sclGetAllHardware(&found);

//hardware = sclGetFastestDevice(allHardware, found);
hardware = sclGetGPUHardware( 0, &found );
software = sclGetCLSoftware("Noget.cl", "Noget2", hardware);

/*sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%R %R %R", 
	dataSize, a, dataSize, b, dataSize, result);
*/
sclManageArgsLaunchKernel(hardware, software, global_size, local_size, "%R %a", 
	dataSize, ma.dataStart, sizeof(int) ,&scalar);
printf("\n");

printMatrix(ma);

}
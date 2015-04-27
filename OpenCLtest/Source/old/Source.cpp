#include <stdio.h>	
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>	
#endif

#define MAX_SOURCE_SIZE (0x100000)	
#define MATRIX_SIDE_SIZE 100

int main()
{
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem Amobj = NULL;
	cl_mem Bmobj = NULL;
	cl_mem Cmobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;

	int i, j;
	float *A;
	float *B;
	float *C;

	A = (float *)malloc(MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float));
	B = (float *)malloc(MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float));
	C = (float *)malloc(MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float));

	FILE *fp;
	const char fileName[] = "./dataParallel.cl";
	size_t source_size;
	char *source_str;

	/* Load kernel source file */
	int a = fopen_s(&fp, fileName, "r");
	if (a != 0) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	/* Load random data from file */

	int *input = (int *)malloc(10000 * sizeof(int));

	fopen_s(&fp, "10000_random_numbers", "r");

	for (size_t ii = 0; ii < 10000; ii++)
	{
		fscanf_s(fp, "%d, ", &input[ii]);
	}

	printf_s("%d\n%d\n", input[0], input[9999]);

	/* Initialize input data */
	for (i = 0; i < MATRIX_SIDE_SIZE; i++) {
		for (j = 0; j < MATRIX_SIDE_SIZE; j++) {
			A[i * MATRIX_SIDE_SIZE + j] = input[i * MATRIX_SIDE_SIZE + j];
			B[i * MATRIX_SIDE_SIZE + j] = input[i * MATRIX_SIDE_SIZE + j];
		}
	}

	/* Get Platform/Device Information */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	/* Create OpenCL Context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create command queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/*Create Buffer Object */
	Amobj = clCreateBuffer(context, CL_MEM_READ_ONLY, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), NULL, &ret);
	Bmobj = clCreateBuffer(context, CL_MEM_READ_ONLY, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), NULL, &ret);
	Cmobj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), NULL, &ret);

	/* Copy input data to the memory buffer */
	ret = clEnqueueWriteBuffer(command_queue, Amobj, CL_TRUE, 0, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, Bmobj, CL_TRUE, 0, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), B, 0, NULL, NULL);

	/* Create kernel program from source file*/
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create data parallel OpenCL kernel */
	kernel = clCreateKernel(program, "dataParallel", &ret);

	/* Set OpenCL kernel arguments */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&Amobj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&Bmobj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&Cmobj);

	// size_t global_item_size = MATRIX_SIDE_SIZE;
	size_t global_item_size = 1;
	size_t local_item_size = 1;

	/* Execute OpenCL kernel as data parallel */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Transfer result to host */
	ret = clEnqueueReadBuffer(command_queue, Cmobj, CL_TRUE, 0, MATRIX_SIDE_SIZE * MATRIX_SIDE_SIZE * sizeof(float), C, 0, NULL, NULL);

	/* Display Results */
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%8.0f ", C[i * MATRIX_SIDE_SIZE + j]);
		}
		printf("\n");
	}

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(Amobj);
	ret = clReleaseMemObject(Bmobj);
	ret = clReleaseMemObject(Cmobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(source_str);

	free(A);
	free(B);
	free(C);

	return 0;
}
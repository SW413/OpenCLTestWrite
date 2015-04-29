#pragma OPENCL EXTENSION cl_intel_printf : enable

__kernel void Noget2(__global int *ma,
					 uint scalar){

	int global_x = get_global_id( 0);

	//printf("%s", "Hello");
	//puts("Fuck");

	ma[ global_x]     	*= scalar;
}
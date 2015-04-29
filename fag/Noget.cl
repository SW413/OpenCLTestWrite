__kernel void Noget(__global int *a,
					__global int *b,
					__global int *result
					){
	
	int local_x = get_local_id(0);
	int global_x = get_global_id( 0);

	//printf("hgh\n");

	result[global_x] = a[global_x] + b[global_x];
}

__kernel void Noget2(__global int *ma,
					 uint scalar){

	int global_x = get_global_id( 0);

	ma[ global_x]     	*= scalar;
}


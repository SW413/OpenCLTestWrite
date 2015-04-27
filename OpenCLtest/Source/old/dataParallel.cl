__kernel void dataParallel(__global float* A, __global float* B, __global float* C)
{
	int base = get_global_id(0);
	int edgesize = get_global_size(0);
	size_t i;

	printf("%d %d\n", edgesize, base);

	/* In this loop 'i' is the row number (up down), and base is the collom (side to side) */

	/* So to access any given element access: base + i * edgesize */

	for (i = 0; i < edgesize; i++)
	{
		C[base + i * edgesize] = A[base + i * edgesize] * B[base + i * edgesize];
	}
}
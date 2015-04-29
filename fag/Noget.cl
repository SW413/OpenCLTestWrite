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

__kernel void MatVecMul(const __global int *matrixA  
						const __global int *matrixB
						__global int *matrixResult
						){
	//Height A equal to B width
	//Width A equal to B height
	int gID = get_global_id(0);
	int dSize = matrixA->dataSize;
	int colIndexB = dSize * (matrixB->cols) ;
	matrixResult[gID] = 0;
	for(int k = 0; k < (matrixA->rows); k++){
		matrixResult[gID] += matrixA[gID + dsize * k] * matrixB[gID + colIndexB * k]
	}
}









/*

__kernel void MatVecMulUncoalesced0(const __global float* M,
                                    const __global float* V,
                                    uint width, uint height,
                                    __global float* W)
{
    // Row index
    uint y = get_global_id(0);
    if (y < height) {
    
        // Row pointer
        const __global float* row = M + y * width;

        // Compute dot product  
        float dotProduct = 0;
        for (int x = 0; x < width; ++x)
            dotProduct += row[x] * V[x];

        // Write result to global memory
        W[y] = dotProduct;
    }

}for(int k = 0; k < cols(b); k++) {
            /* this is the m mentioned in the topmost description,
             * and the number of multiplications made for each index in the resulting matrix */
               // res[i, j] += a[i, k] * b[k, j];
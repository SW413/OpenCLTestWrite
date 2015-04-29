__kernel void MatVecMul(const __global int *matrixA,  
                        const __global int *matrixB,
                        __global int *matrixResult,
                        int rowsA,
                        int colsB,
                        int colsA
                        ){
    //Height A equal to B width
    //Width A equal to B height
    int currentX = get_global_id(0);
    int currentY = get_global_id(1);
    int index = currentX * colsB + currentY;
    //printf("\n[%d, %d] %d = ", currentX, currentY, index);
    matrixResult[currentX * colsB + currentY] = 0;
    for(int k = 0; k < colsA; k++){
        matrixResult[index] += matrixA[currentX * rowsA + k] * matrixB[currentY + colsB * k];
    }
    //printf("%d\n", matrixResult[currentX * colsB + currentY]);
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
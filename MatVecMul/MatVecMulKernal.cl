__kernel void MatVecMul(const __global int *matrixA,  
                        const __global int *matrixB,
                        __global int *matrixResult,
                        int rowsA,
                        int colsB,
                        int colsA
                        )
{
    // The current point on the X axis of the matrix (from left to right)
    int currentX = get_global_id(0);
    // The current point on the Y axis of the matrix (from up to down)
    int currentY = get_global_id(1);
    // The index in the array.
    int index = currentX * colsB + currentY;
    //printf("%d\n", index);

    // Iterate over the columns of A to calculate the inner products
    for(int k = 0; k < colsA; k++){
        // Add the cross product to the result matrix in the index
        matrixResult[index] += matrixA[currentX * rowsA + k] * matrixB[currentY + colsB * k];

           // printf("%d %d %d %d %d\n", currentX, rowsA, k, currentX * rowsA + k, matrixA[currentX * rowsA + k] );

    }
}
__kernel void MatVecMul(const __global int *matrixA,  
                        const __global int *matrixB,
                        __global int *matrixResult,
                        int rowsA,
                        int colsA,
                        int rowsB,
                        int colsB
                        )
{
    // The current point on the X axis of the matrix (from left to right)
    int currentX = get_global_id(0);
    // The current point on the Y axis of the matrix (from up to down)
    int currentY = get_global_id(1);
    // The index in the array.
    int index = currentX * colsA + currentY;
    printf("%d %d %d\n", index, currentX, currentY);

    // Iterate over the columns of A to calculate the inner products
    for(int k = 0; k < rowsB; k++){
        // RESULT.ROWS == rowsA
        // RESULT.COLS == colsB
        // rowsB == colsA

        int aVal = matrixA[currentX * colsA + k];
        int bVal = matrixB[k * colsB + currentY];
        barrier(CLK_GLOBAL_MEM_FENCE);
        matrixResult[index] += aVal * bVal;
        printf("%d %d\n", aVal, bVal);
    }
    printf("%d\n", matrixResult[index]);
}
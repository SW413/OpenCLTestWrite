__kernel void MatVecMul(const __global int *matrixA, 
						const __global int *matrixB,
						__global int *matrixResult){
	
	//Height A equal to B width
	//Width A equal to B height
	int gID = get_global_id(0);
	int dSize = matrixA->dataSize;
	int colIndexB = dSize * (matrixB->cols);
	printf("%d %d %d", gID, dSize,colIndexB);
	matrixResult[gID] = 0;/*
	for(int k = 0; k < (matrixA->cols); k++){
		matrixResult[gID] += matrixA[gID + dSize * k] * matrixB[gID + colIndexB * k];
	} */
}
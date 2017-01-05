#ifndef _nnLayerUtils_h
#define _nnLayerUtils_h
#ifdef MKL
#include <mkl.h>
#include <mkl_cblas.h>
#include <mkl_blas.h>
#include <mkl_lapack.h>
#include <mkl_lapacke.h>
#endif


typedef struct nnLayer {
	uint outDim;
	uint inDim;
	float *A;
	float *b;
} nnLayer;

nnLayer * createLayer(float *A, float *b, uint outDim, uint inDim);
void freeLayer(nnLayer * layer);

#endif
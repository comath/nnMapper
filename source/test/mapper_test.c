/*
This just tests that there's no race conditions, or memory leaks. 
Running Valgrind or gdb on the python wrap leads to a bit more headaches.
*/
#include <stdio.h>
#include <stdlib.h>
#include "../cutils/nnLayerUtils.h"
#include "../cutils/mapper.h"

void printFloatArrNoNewLine(float * arr, int numElements){
	int i = 0;
	printf("[");
	for(i=0;i<numElements-1;i++){
		printf("%f,",arr[i]);
	}
	printf("%f", arr[numElements-1]);
	printf("]");
}

//Creates a layer with HPs parrallel to the cordinate axes
nnLayer *createDumbLayer(uint dim, uint numHP)
{
	nnLayer * layer = calloc(1,sizeof(nnLayer));
	layer->A = calloc((dim+1)*numHP,sizeof(float));
	layer->b = layer->A + dim*numHP;
	uint i,j;
	for(i=0;i<numHP;i++){
		for(j=0;j<dim;j++){
			if(i==j){
				layer->A[i*dim + j] = 1;
			} else {
				layer->A[i*dim + j] = 0;
			}
		}
		layer->b[i] = 0;
	}
	layer->inDim = dim;
	layer->outDim = numHP;
	return layer;
}

void randomizePoint(float *p, uint dim)
{
	uint i = 0;
	for(i=0;i<dim;i++){
		p[i] = (double)((rand() % 20000) - 10000)/10000;
	}
}

float * randomData(uint dim, uint numData)
{
	float * data = malloc(dim*numData*sizeof(float));
	uint i;
	for(i=0;i<numData;i++){
		randomizePoint(data +dim*i,dim);
	}
	return data;
}


int main(int argc, char* argv[])
{
	uint dim = 3;
	uint outDim = 2;
	uint numHP = 5;
	uint numData = 5;
	uint keySize = calcKeyLen(numHP);
	uint maxThreads = sysconf(_SC_NPROCESSORS_ONLN);

	srand(time(NULL));
	uint i = 0;
	printf("If no faliures are printed then we are fine.\n");
	nnLayer *layer0 = createDumbLayer(dim,numHP);
	_nnMap *map = allocateMap(layer0,2,0.5);
	

	uint *ipSignature = malloc(keySize*numData*sizeof(uint));
	float *data = randomData(dim,numData);
	float *errorMargins = randomData(1,numData);

	printf("Calculating the signature of Points\n");

	addDatumToMap(map, data, 1.0);
	addDataToMapBatch(map,data,errorMargins,numData,1);

	freeMap(map);
	free(data);
	free(errorMargins);


	return 0;
}
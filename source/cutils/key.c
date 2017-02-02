#include "key.h"

#define DATASIZE 32

uint calcKeyLen(uint dataLen)
{
	uint keyLen = (dataLen/DATASIZE);
	if(dataLen % DATASIZE){
		keyLen++;
	}
	return keyLen;
}

char compareKey(uint *x, uint *y, uint keyLen)
{
	uint i = 0;
	for(i = 0; i < keyLen; i++){
		if(x[i] > y[i]){
			return -1;
		} 
		if (x[i] < y[i]){
			return 1;
		}
	}
	return 0;
}

void convertToKey(int * raw, uint *key,uint dataLen)
{

	uint keyLen = calcKeyLen(dataLen);
	clearKey(key, keyLen);
	uint i = 0,j=0;
	for(i=0;i<dataLen;i++){
		j = i % DATASIZE;
		if(raw[i]){
			key[i/DATASIZE] += (1 << (DATASIZE -j -1))	;
		}	
	}
}



void batchConvertToKey(int * raw, uint *key,uint dataLen, uint numData){
	uint i =0;
	uint keyLen = calcKeyLen(dataLen);
	for(i=0;i<numData;i++){
		convertToKey(raw + i*dataLen, key +i*keyLen,dataLen);
	}
}



void addIndexToKey(uint * key, uint index)
{
	int j = index % DATASIZE;
	if(checkIndex(key,index) == 0){
		key[index/DATASIZE] += (1 << (DATASIZE-1-j));
	}
}

uint checkIndex(uint * key, uint index)
{
	return key[index/DATASIZE] & (1 << (DATASIZE-1-(index % DATASIZE)));
}

void clearKey(uint *key, uint keyLen)
{
	uint i = 0;
	for(i=0;i<keyLen;i++){
		key[i] = 0;
	}
}

void printKeyArr(uint *key, uint length){
	uint i = 0;
	printf("[");
	for(i=0;i<length-1;i++){
		printf("%u,",key[i]);
	}
	printf("%u", key[length-1]);
	printf("]\n");
}

void printIntArr(int *arr, uint length){
	uint i = 0;
	printf("[");
	for(i=0;i<length-1;i++){
		printf("%d,",arr[i]);
	}
	printf("%d", arr[length-1]);
	printf("]\n");
}

void printKey(uint* key, uint dataLen){
	int *raw = calloc(dataLen,sizeof(int));
	convertFromKey(key,raw,dataLen);
	printIntArr(raw,dataLen);
	free(raw);
}

void convertFromKey(uint *key, int * raw, uint dataLen)
{



	uint i = 0;
	for(i=0;i<dataLen;i++){
		if(checkIndex(key,i)){
			raw[i] = 1;
		} else {
			raw[i] = 0;
		}
	}

}

void batchConvertFromKey(uint *key, int * raw, uint dataLen,uint numData){
	uint i =0;
	uint keyLen = calcKeyLen(dataLen);
	printf("Converting a key. Data length: %u, numData: %u\n",dataLen,numData );
	for(i=0;i<numData;i++){
		convertFromKey(key + i*keyLen, raw +i*dataLen,dataLen);
	}
}

void chromaticKey(uint* key, float *rgb, uint dataLen)
{
	rgb[0] = 0;
	rgb[1] = 0;
	rgb[2] = 0;
	uint i = 0;
	for(i =0;i<dataLen;i++){
		if(checkIndex(key,i)){
			if(i % 3 == 0){ rgb[0]+= 1.0f/ (1 << (int)(i/3+1)); }
			if(i % 3 == 1){ rgb[1]+= 1.0f/ (1 << (int)(i/3+1)); }
			if(i % 3 == 2){ rgb[2]+= 1.0f/ (1 << (int)(i/3+1)); }
		}		
	}
}

void batchChromaticKey(uint* key, float *rgb, uint dataLen, uint numData){
	uint i =0;
	uint keyLen = calcKeyLen(dataLen);
	for(i=0;i<numData;i++){
		chromaticKey(key + i*keyLen, rgb +i*3,dataLen);
	}
}
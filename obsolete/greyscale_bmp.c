//Author : Gustave HERVE
//4DGENERATES RULZZZZ

#include <stdlib.h>
#include <stdio.h> 

int max(int a, int b){
	if (a <= b)
		return b;
	return a;
}

int max3(int a, int b, int c){

	int temp = max(a, b);
	if (temp <= max(temp, c))
		return c;
	return temp;

}

void greyscale(char path[], char out[]){

	FILE *fileStream;
	fileStream = fopen(path, "rb");
	
	//Go to end of file, get file size and go back at beginning of file
	fseek(fileStream, 0L, SEEK_END);
	long size = ftell(fileStream);
	rewind(fileStream);

	char *buffer = malloc(sizeof(char) * size);
	if (buffer == NULL)
		printf("merde");
	
	char *copy = malloc(sizeof(char) * size);

	if (fileStream == NULL){
		printf("Image doesn't exist");
		return;
	}

	fread(buffer, size, 1, fileStream);

	//we must precise unsigned int 8-bit to have correct values
	uint8_t offset = (uint8_t)buffer[10];
	//uint8_t depth = (uint8_t)buffer[28];

	for (int i = 0; i < size; i++){
		copy[i] = buffer[i];
	}

	fclose(fileStream);
	free(buffer);

	for (int i = offset; i < size; i += 3){
		if (i+3 < size){

			int grey = max3(copy[i], copy[i+1], copy[i+2]); //algo made in briac
			//old version, too many artifacts (REC 701 DCode):
			//int grey = 0.0721 * (int)copy[i] + 0.7154 * (int)copy[i+1] + 0.2125 * (int)copy[i+2];
			copy[i] = (char)grey;	//B
			copy[i+1] = (char)grey;	//G
			copy[i+2] = (char)grey;	//R
		}
	}

	FILE *write = fopen(out, "wb");
	fwrite(copy, size, 1, write);
	fclose(write);
	free(copy);
}

int main(int argc, char* argv[]){
	if (argc != 3){
		printf("Wrong argument(s)");
		exit(1);
	}
	greyscale(argv[1], argv[2]);
	return 0;
}
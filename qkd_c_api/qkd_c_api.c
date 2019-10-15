#include <stdio.h>
#include <stdlib.h>
#include "qkd_c_api.h"


int QKD_OPEN(context_t* context) {
	if ((context = (context_t*) malloc(1)) == NULL) {
		return -1;
	}
	
	*context = 42;

	return 0;
}

int QKD_GET_KEY(context_t* context, uint32_t key_size, uint8_t* key_buffer) {
	srand(*context);

	if ((key_buffer = (uint8_t*) malloc(key_size)) == NULL) {
		return -1;
	}

	for (uint32_t i = 0; i < key_size; i++) {
		key_buffer[i] = (uint8_t) rand() % 255;
	}

	return 0;
}

int QKD_CLOSE(context_t* context) {
	free(context);

	return 0;
}

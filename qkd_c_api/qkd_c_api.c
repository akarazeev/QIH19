#include <stdio.h>
#include "qkd_c_api.h"


uint32_t QKD_OPEN(uint32_t destination, qos_t QoS, context_t* key_handle) {
	if ((key_handle = (context_t*) malloc(1)) == NULL) {
		return -1;
	}

	*key_handle = 42;

	return 0;
}

uint32_t QKD_CONNECT(context_t* key_handle, uint32_t timeout) {

	return 0;
}

uint32_t QKD_GET_KEY(context_t* key_handle, uint8_t* key_buffer) {

	return 0;
}

uint32_t QKD_CLOSE(context_t* key_handle) {
	free(key_handle);

	return 0;
}

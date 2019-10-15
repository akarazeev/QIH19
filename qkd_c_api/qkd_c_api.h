#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef uint32_t context_t;

int QKD_OPEN(context_t* context);

int QKD_GET_KEY(context_t* context, uint32_t key_size, uint8_t* key_buffer);

int QKD_CLOSE(context_t* context);

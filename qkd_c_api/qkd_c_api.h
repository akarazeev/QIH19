#include <stdlib.h>

typedef char key_handle_t[64];

typedef struct {
  uint32_t requested_length;
  uint32_t max_bps;
  uint32_t priority;
  uint32_t timeout;
} qos_t;

uint32_t QKD_OPEN(uint32_t destination, qos_t QoS, key_handle_t* key_handle);

uint32_t QKD_CONNECT(key_handle_t* key_handle, uint32_t timeout);

uint32_t QKD_GET_KEY(key_handle_t* key_handle, uint8_t* key_buffer);

uint32_t QKD_CLOSE(key_handle_t* key_handle);

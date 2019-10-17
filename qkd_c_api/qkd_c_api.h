#include <stdlib.h>

typedef struct {
  uint32_t id;

  int sockfd;
  int portno;

  char* hostname;
  char host;
} context_t;


typedef struct {
  uint32_t requested_length;
  uint32_t max_bps;
  uint32_t priority;
  uint32_t timeout;
} qos_t;

uint32_t QKD_OPEN(uint32_t destination, qos_t QoS, context_t* key_handle);

uint32_t QKD_CONNECT(context_t* key_handle, uint32_t timeout);

uint32_t QKD_GET_KEY(context_t* key_handle, uint8_t* key_buffer);

uint32_t QKD_CLOSE(context_t* key_handle);

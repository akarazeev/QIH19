#include "qkd_c_api.c"

int main() {
  srand(time(NULL));
  
  /* QKD_OPEN */
  uint32_t destination = 0;

  qos_t qos;
  qos.requested_length = 0;
  qos.max_bps = 0;
  qos.priority = 0;
  qos.timeout = 0;

  context_t* key_handle = (context_t*) malloc(1);
  key_handle->host = 'c';
  key_handle->portno = 8080;
  key_handle->hostname = "localhost";

  QKD_OPEN(destination, qos, key_handle);

  /* QKD_CONNECT */
  uint32_t timeout = 0;
  QKD_CONNECT(key_handle, timeout);

  /* QKD_GET_KEY */
  uint8_t* key_buffer;
  QKD_GET_KEY(key_handle, &key_buffer);
  printf("-> key %s\n", key_buffer);

  QKD_GET_KEY(key_handle, &key_buffer);
  printf("-> key %s\n", key_buffer);

  QKD_CLOSE(key_handle);

  return 0;
}

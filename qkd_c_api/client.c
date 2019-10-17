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

  key_handle_t key_handle = "init_value";

  QKD_OPEN(destination, qos, &key_handle);

  /* QKD_CONNECT */
  uint32_t timeout = 0;
  QKD_CONNECT(&key_handle, timeout);

  /* QKD_GET_KEY */
  uint8_t* key_buffer = (uint8_t*) malloc(sizeof(uint8_t) * KEYSIZE);
  QKD_GET_KEY(&key_handle, key_buffer);
  printf("-> key: %s\n", key_buffer);
  for (size_t i = 0; i < KEYSIZE; i++) {
    if (i == KEYSIZE - 1) {
      printf("%d\n", key_buffer[i]);
    } else {
      printf("%d, ", key_buffer[i]);
    }
  }

  QKD_GET_KEY(&key_handle, key_buffer);
  printf("-> key: %s\n", key_buffer);
  for (size_t i = 0; i < KEYSIZE; i++) {
    if (i == KEYSIZE - 1) {
      printf("%d\n", key_buffer[i]);
    } else {
      printf("%d, ", key_buffer[i]);
    }
  }

  QKD_CLOSE(&key_handle);

  return 0;
}

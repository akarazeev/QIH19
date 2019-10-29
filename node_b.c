#include <time.h>
#include <stdio.h>
#include <string.h>

#include "qkd_api/qkd_api.h"

int main() {
    srand(time(NULL));

    /* QKD_OPEN */
    ip_address_t destination;

    qos_t qos;
    qos.requested_length = 3;

    key_handle_t key_handle = "specified_key";

    QKD_OPEN(destination, qos, key_handle);

    /* QKD_CONNECT */
    uint32_t timeout = 0;
    QKD_CONNECT_BLOCKING(key_handle, timeout);

    /* QKD_GET_KEY */
    uint8_t* key_buffer = (uint8_t*) malloc(sizeof(uint8_t) * qos.requested_length);
    if (QKD_GET_KEY(key_handle, key_buffer) != 0) {
        perror("Error");
    }
    for (size_t i = 0; i < qos.requested_length; i++) {
        if (i == qos.requested_length - 1) {
            printf("%d\n", key_buffer[i]);
        } else {
            printf("%d, ", key_buffer[i]);
        }
    }
    printf("=====\n");

    memcpy(key_handle, "000111", 6);

    if (QKD_GET_KEY(key_handle, key_buffer) != 0) {
        perror("Error");
    }
    for (size_t i = 0; i < qos.requested_length; i++) {
        if (i == qos.requested_length - 1) {
            printf("%d\n", key_buffer[i]);
        } else {
            printf("%d, ", key_buffer[i]);
        }
    }

    QKD_CLOSE(key_handle);

    return 0;
}

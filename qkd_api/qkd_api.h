#include <stdlib.h>

#define BUFSIZE 1024
#define KEYHANDLE_SIZE 64
#define IP_ADDR_MAX_LEN 16

enum RETURN_CODES {
    SUCCESS,
    QKD_GET_KEY_FAILED,
    NO_QKD_CONNECTION,
    QKD_OPEN_FAILED,
    TIMEOUT_ERROR
};

typedef uint8_t key_handle_t[KEYHANDLE_SIZE];

typedef struct {
    uint32_t requested_length;
    uint32_t max_bps;
    uint32_t priority;
    uint32_t timeout;
} qos_t;

typedef struct {
    uint8_t length;
    char address[IP_ADDR_MAX_LEN];
} ip_address_t;

uint32_t QKD_OPEN(ip_address_t destination, qos_t QoS, key_handle_t key_handle);

uint32_t QKD_CONNECT_NONBLOCKING(key_handle_t key_handle, uint32_t timeout);

uint32_t QKD_CONNECT_BLOCKING(key_handle_t key_handle, uint32_t timeout);

uint32_t QKD_GET_KEY(key_handle_t key_handle, uint8_t* key_buffer);

uint32_t QKD_CLOSE(key_handle_t key_handle);

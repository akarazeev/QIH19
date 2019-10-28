#include <stdlib.h>

#define KEYHANDLE_SIZE 64
#define IP_ADDR_MAX_LEN 16

/* Define an enum for the return codes as specified on p. 10 of the API doc.
 * However, leave the return type as uint32_t, because enums can only be an
 * int. */

/* Interesting - is key_handle_t * a pointer to the first element of the array
 * or is it a double pointer? */
typedef char key_handle_t[KEYHANDLE_SIZE];

typedef struct {
    uint32_t requested_length;
    uint32_t max_bps;
    uint32_t priority;
    uint32_t timeout;
} qos_t;

/* My suggestion for IP address struct */
typedef struct {
    uint8_t length;
    char address[IP_ADDR_MAX_LEN];
} ip_address_t;

/* destination should be an IP address, to support - you don't use it in your
 * code, but the API should be able to handle IPv4 and IPv6 - see my struct
 * suggstion above */
uint32_t QKD_OPEN(uint32_t destination, qos_t QoS, key_handle_t* key_handle);

/* Please have this header include both BLOCKING and NONBLOCK - you don't have
 * to implement both, but the header must have both */
uint32_t QKD_CONNECT(key_handle_t* key_handle, uint32_t timeout);

/* spec specifies key_buffer to be of type char * - it's exactly the same thing
 * from a bit-level point f view, but in general in C if you mean byte, you use
 * char. This makes the intended use clearer. */
uint32_t QKD_GET_KEY(key_handle_t* key_handle, uint8_t* key_buffer);

uint32_t QKD_CLOSE(key_handle_t* key_handle);

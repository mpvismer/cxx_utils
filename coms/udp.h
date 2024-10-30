
#include <stdbool.h>
#include <stdint.h>

typedef udp_context;


void udp_init(udp_context *context);

void udp_send(const udp_context *context, const uint8_t *const buf, const size_t len);

size_t udp_recv(const udp_context *context, uint8_t *const buf, const size_t max_len);



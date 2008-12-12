#ifndef __AR_IO_STATUS_PACKET_H__
#define __AR_IO_STATUS_PACKET_H__

#include <stdlib.h>

struct ar_io_status_packet
{
  unsigned char id, error, param_count, *params;
};

typedef struct ar_io_status_packet ar_io_status_packet_t;

ar_io_status_packet_t*
ar_io_status_packet_init(const unsigned char* data);

void
ar_io_status_packet_free(ar_io_status_packet_t *p);

#endif

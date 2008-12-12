#include "status_packet.h"

#include <assert.h>

ar_io_status_packet_t*
ar_io_status_packet_init(const unsigned char* data)
{
  if (data[0] != 0xFF || data[1] != 0xFF)
    return NULL;

  ar_io_status_packet_t* p = malloc(sizeof(ar_io_status_packet_t));
  assert(p != NULL);

  p->id = data[2];
  p->error = data[4];
  p->param_count = data[3] - 6;
  if (p->param_count)
    {
      p->params = malloc(sizeof(unsigned char) * p->param_count);
  
      unsigned char i;
      for(i = 0; i < p->param_count; ++i)
	p->params[i] = data[i+5];
    }
  else
    p->params = NULL;

  return p;
}

void
ar_io_status_packet_free(ar_io_status_packet_t *p)
{
  if (p)
    {
      free(p->params);
      free(p);
    }
}

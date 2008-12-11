#include "instruction_packet.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ar_io_instruction_packet_t*
ar_io_instruction_packet_init(unsigned char id,
			      unsigned char instruction,
			      const unsigned char* params,
			      unsigned char param_count)
{
  ar_io_instruction_packet_t *p = malloc(sizeof(ar_io_instruction_packet_t));
  assert(p != NULL);
  
  p->id = id;
  p->instruction = instruction;
  p->param_count = param_count;
  if (param_count == 0)
    {
      p->params = NULL;
    }
  else
    {
      p->params = malloc(sizeof(unsigned char) * p->param_count);
    }

  return p;
}

void
ar_io_instruction_packet_free(ar_io_instruction_packet_t* p)
{
  if (p)
    {
      free(p->params);
      free(p);
    }
}

unsigned char*
ar_io_instruction_packet_to_bytes(ar_io_instruction_packet_t* p)
{
  assert(p != NULL);

  unsigned char i;
  unsigned char len = p->param_count + 6;
  unsigned char *r = malloc(sizeof(unsigned char) * len);
  r[0] = r[1] = 0xFF;
  r[2] = p->id;
  r[3] = p->param_count + 2;
  r[4] = p->instruction;
  r[len-1] = r[2] + r[3] + r[4];
  for(i = 0; i < p->param_count; ++i)
    {
      r[len-1] += p->params[i];
      r[i+5] = p->params[i];
    }
  r[len-1] = ~r[len-1];
  return r;
}

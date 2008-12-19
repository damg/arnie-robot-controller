/**
   \addtogroup serial_interface
*/

/*@{*/

#include "connection.h"

#include <string.h>
#include <assert.h>
#include "errno.h"

int ar_io_open(struct ftdi_context* ftdic)
{
  assert(ftdic != NULL);
  int rc;

  rc = ftdi_init(ftdic);
  if (rc == -1)
    {
      ar_io_errno = AR_IO_EINIT;
      return -1;
    }

  rc = ftdi_usb_open(ftdic, 0x0403, 0x6001);
  if (rc < 0)
    {
      ar_io_errno = AR_IO_EOPEN;
      return -1;
    }

  rc = ftdi_usb_reset(ftdic);
  if (rc == -1)
    {
      ar_io_errno = AR_IO_ERESET;
      return -1;
    }

  rc = ftdi_set_baudrate(ftdic, 1000000);
  switch(rc)
    {
    case -1:
      ar_io_errno = AR_IO_EINVBAUD;
      return -1;
    case -2:
      ar_io_errno = AR_IO_ESETBAUD;
      return -1;
    default:
      break;
    }

  ftdi_usb_purge_buffers(ftdic);
  return 0;
}

int ar_io_close(struct ftdi_context* ftdic)
{
  assert(ftdic != NULL);
  int rc = ftdi_usb_close(ftdic);
  if (rc < 0)
    {
      ar_io_errno = AR_IO_ECLOSE;
      return -1;
    }
  return 0;
}

int ar_io_write_instruction_packet(struct ftdi_context *ftdic,
				   const struct ar_io_instruction_packet *p)
{
  assert(p != NULL);
  unsigned char* buf = ar_io_instruction_packet_to_bytes(p);
  unsigned char len = p->param_count + 6; // Buffer length
  int rc;

  rc = ftdi_write_data(ftdic, buf, len);
  if (rc < 0)
    {
      ar_io_errno = AR_IO_EWRITE;
      free(buf);
      return -1;
    }

  free(buf);
  return 0;
}

int ar_io_read_status_packet(struct ftdi_context *ftdic,
			     struct ar_io_status_packet *p)
{
  int rc;
  unsigned char hdr[4]; // header buf
  
  rc = ftdi_read_data(ftdic, hdr, 4);
  if (rc < 0)
    {
      ar_io_errno = AR_IO_EREAD;
      return -1;
    }
  if (rc == 0 || rc < 4)
    {
      ar_io_errno = AR_IO_EREADNODATA;
      return -1;
    }

  if (hdr[0] != 0xFF || hdr[1] != 0xFF)
    {
      ar_io_errno = AR_IO_EREADHEADER;
      return -1;
    }

  p->id = hdr[2];
  p->param_count = hdr[3] - 2;

  unsigned char tl[hdr[3]]; // message tail
  rc = ftdi_read_data(ftdic, tl, hdr[3]);
  if (rc < 0)
    {
      ar_io_errno = AR_IO_EREAD;
      return -1;
    }
  if (rc == 0 || rc < hdr[3])
    {
      ar_io_errno = AR_IO_EREADNODATA;
      return -1;
    }

  p->error = tl[0];
  
  // copy params
  p->params = realloc(p->params, sizeof(unsigned char) * p->param_count);

  unsigned char i;
  for(i = 0; i < hdr[3] - 2; ++i)
    p->params[i] = tl[i+1];

  return 0;
}

int ar_io_read_control_table(struct ftdi_context *ftdic,
			     unsigned char id,
			     struct ar_io_control_table *ct)
{

  assert(ftdic != NULL);
  assert(ct != NULL);
  assert(id < 0xFE);

  int rc;
  struct ar_io_instruction_packet ip;
  struct ar_io_status_packet sp;
  unsigned char params[] = { 0x00, 50 };
  sp.params = NULL;

  ip.id = id;
  ip.instruction = 0x02;
  ip.param_count = 2;
  ip.params = params;

  rc = ar_io_write_instruction_packet(ftdic, &ip);
  if (rc == -1)
    return -1;

  rc = ar_io_read_status_packet(ftdic, &sp);
  if (rc == -1)
    {
      free(sp.params);
      return -1;
    }
  
  if (sp.param_count != 50)
    {
      free(sp.params);
      return -1;
    }

  memcpy(ct, sp.params, 50);

  free(sp.params);
  return 0;
}

int ar_io_set_moving_speed(struct ftdi_context *ftdic,
			   unsigned char id,
			   unsigned short moving_speed,
			   struct ar_io_status_packet *sp)
{
  assert(moving_speed <= 0x03FF);
  unsigned char buf[] =
    {
      moving_speed & 0x00FF, (moving_speed & 0xFF00) >> 8
    };
  return ar_io_write_memory(ftdic, id, 0x20, buf, 2, sp);
}

int ar_io_set_goal_position(struct ftdi_context *ftdic,
			   unsigned char id,
			   unsigned short goal_position,
			   struct ar_io_status_packet *sp)
{
  assert(goal_position <= 0x03FF);
  unsigned char buf[] =
    { 
      goal_position & 0x00FF, (goal_position & 0xFF00) >> 8
    };

  return ar_io_write_memory(ftdic, id, 0x1E, buf, 2, sp);
}

int ar_io_write_memory(struct ftdi_context *ftdic,
		       unsigned char id,
		       unsigned char address,
		       unsigned char *bytes,
		       unsigned char byte_count,
		       struct ar_io_status_packet *sp)
{
  assert(ftdic != NULL);
  assert(id < 0xFE);
  assert(byte_count != 0 && bytes != NULL);
  assert(sp != NULL);
  assert((address >= 0x03 && address <= 0x09) ||
	 (address >= 0x0B && address <= 0x12) ||
	 (address >= 0x18 && address <= 0x23) ||
	 (address == 0x2C) ||
	 (address >= 0x2F && address <= 0x31));

  unsigned char params[byte_count + 1];
  params[0] = address;

  memcpy(params+1, bytes, byte_count);

  struct ar_io_instruction_packet ip;
  ip.id = id;
  ip.instruction = 0x03;
  ip.params = params;
  ip.param_count = byte_count + 1;

  int rc;
  rc = ar_io_write_instruction_packet(ftdic, &ip);
  if (rc == -1)
    return -1;

  rc = ar_io_read_status_packet(ftdic, sp);
  return rc;
}

int ar_io_set_torque_limit(struct ftdi_context *ftdic,
			   unsigned char id,
			   unsigned short torque_limit,
			   struct ar_io_status_packet *sp)
{
  assert(torque_limit < 0xFE);
  unsigned char buf[] = { torque_limit & 0x00FF,
			  (torque_limit & 0xFF00) >> 8 };
  return ar_io_write_memory(ftdic, id, 0x22, buf, 2, sp);
}

int ar_io_set_torque_enable(struct ftdi_context *ftdic,
			    unsigned char id,
			    bool torque_enable,
			    struct ar_io_status_packet *sp)
{
  unsigned char buf = torque_enable ? 1 : 0;
  return ar_io_write_memory(ftdic, id, 0x18, &buf, 1, sp);
}

/*@}*/

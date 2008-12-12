#include "connection.h"

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
  
  assert(p != NULL);
  assert(p->params == NULL);

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
  p->params = malloc(sizeof(unsigned char) * p->param_count);
  unsigned char i;
  for(i = 0; i < hdr[3] - 2; ++i)
    p->params[i] = tl[i+1];

  return 0;
}

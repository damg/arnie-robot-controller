/**
   \addtogroup serial_interface
*/

/*@{*/

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
  assert(ct != NULL);
  assert(id != 0xFF);

  struct ar_io_instruction_packet ip;
  unsigned char read_data_params[] = { 0x03, 32 };
  struct ar_io_status_packet sp;
  sp.params = NULL;

  ip.id = id;
  ip.instruction = 0x02;
  ip.params = read_data_params;
  ip.param_count = 2;

  int rc;
  rc = ar_io_write_instruction_packet(ftdic, &ip);

  if (rc == -1)
    return -1;

  rc = ar_io_read_status_packet(ftdic, &sp);
  if (rc == -1)
      return -1;

  if (sp.param_count != 32)
    {
      free(sp.params);
      ar_io_errno = AR_IO_EREADNODATA;
      return -1;
    }

  ct->id = sp.params[0];
  ct->baud_rate = sp.params[1];
  ct->return_delay_time = sp.params[2];
  ct->cw_angle_limit = sp.params[3] | (sp.params[4] << 8);
  ct->ccw_angle_limit = sp.params[5] | (sp.params[6] << 8);
  ct->highest_limit_temperature = sp.params[7];
  ct->lowest_limit_voltage = sp.params[8];
  ct->highest_limit_voltage = sp.params[9];
  ct->max_torque = sp.params[10] | (sp.params[11] << 8);
  ct->status_return_level = sp.params[12];
  ct->alarm_led = sp.params[13];
  ct->alarm_shutdown = sp.params[14];
  ct->reserved = sp.params[15];
  ct->torque_enable = sp.params[16];
  ct->led = sp.params[17];
  ct->cw_compliance_margin = sp.params[18];
  ct->ccw_compliance_margin = sp.params[19];
  ct->cw_compliance_slope = sp.params[20];
  ct->ccw_compliance_slope = sp.params[21];
  ct->goal_position = sp.params[22] | (sp.params[23] << 8);
  ct->moving_speed = sp.params[24] | (sp.params[25] << 8);
  ct->torque_limit = sp.params[26] | (sp.params[27] << 8);
  ct->registered_instruction = sp.params[28];
  ct->lock = sp.params[29];
  ct->punch = sp.params[30] | (sp.params[31] << 8);

  free(sp.params);
  return 0;
}

int ar_io_write_control_table(struct ftdi_context *ftdic,
			      struct ar_io_control_table *ct)
{
  assert(ftdic != NULL);
  assert(ct != NULL);

  unsigned char buf[] =
    {
      0x03,
      ct->id,
      ct->baud_rate,
      ct->return_delay_time,
      (ct->cw_angle_limit & 0xFF00) >> 8,
      ct->cw_angle_limit & 0x00FF,
      (ct->ccw_angle_limit & 0xFF00) >> 8,
      ct->ccw_angle_limit & 0x00FF,
      ct->highest_limit_temperature,
      ct->lowest_limit_voltage,
      ct->highest_limit_voltage,
      (ct->max_torque & 0xFF00) >> 8,
      ct->max_torque & 0x00FF,
      ct->status_return_level,
      ct->alarm_led,
      ct->alarm_shutdown,
      ct->reserved,
      ct->torque_enable,
      ct->led,
      ct->cw_compliance_margin,
      ct->ccw_compliance_margin,
      ct->cw_compliance_slope,
      ct->ccw_compliance_slope,
      (ct->goal_position & 0xFF00) >> 8,
      ct->goal_position & 0x00FF,
      (ct->moving_speed & 0xFF00) >> 8,
      ct->moving_speed & 0x00FF,
      (ct->torque_limit & 0xFF00) >> 8,
      ct->torque_limit & 0x00FF,
      ct->registered_instruction,
      ct->lock,
      (ct->punch & 0xFF00) >> 8,
      ct->punch & 0x00FF
    };

  struct ar_io_instruction_packet ip;
  ip.id = ct->id;
  ip.instruction = 0x03;
  ip.params = buf;
  ip.param_count = 33;

  return ar_io_write_instruction_packet(ftdic, &ip);
}

/*@}*/

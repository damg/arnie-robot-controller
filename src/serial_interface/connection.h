#ifndef __ARNIE_IO_CONNECTION_H__
#define __ARNIE_IO_CONNECTION_H__

#include <ftdi/ftdi.h>
#include "instruction_packet.h"
#include "status_packet.h"

int ar_io_open(struct ftdi_context* ftdic);
int ar_io_close(struct ftdi_context* ftdic);
int ar_io_write_instruction_packet(struct ftdi_context *ftdic,
				   const struct ar_io_instruction_packet *p);
int ar_io_read_status_packet(struct ftdi_context *ftdic,
			     struct ar_io_status_packet *p);

#endif

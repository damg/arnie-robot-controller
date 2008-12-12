#include <stdlib.h>
#include <stdio.h>

#include <serial_interface/connection.h>
#include <serial_interface/errno.h>

int main()
{
  struct ftdi_context ftdic;
  int rc;

  rc = ar_io_open(&ftdic);
  if (rc == -1)
    {
      fprintf(stderr, "%s\n", ar_io_errstring(ar_io_errno));
      return 1;
    }

  unsigned char id;
  struct ar_io_instruction_packet ip = { 0x00, 0x01, 0x00, NULL };
  struct ar_io_status_packet sp = { 0, 0, 0, NULL };
  for(id = 0; id < 0xFE; ++id)
    {
      printf("ID: %i PING...", id);
      ip.id = id;
      rc = ar_io_write_instruction_packet(&ftdic, &ip);
      if (rc == -1)
	{
	  printf("ERROR: %s\n", ar_io_errstring(ar_io_errno));
	  continue;
	}

      rc = ar_io_read_status_packet(&ftdic, &sp);
      if (rc == -1)
	{
	  printf("ERROR: %s\n", ar_io_errstring(ar_io_errno));
	  continue;
	}
    }

  rc = ar_io_close(&ftdic);
  if (rc == -1)
    {
      fprintf(stderr, "%s\n", ar_io_errstring(ar_io_errno));
      return 1;
    }

  return 0;
}

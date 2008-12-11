#include <ftdi/ftdi.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
  struct ftdi_context ftdic;
  ftdi_init(&ftdic);
  int rc;

  if ((rc = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0)
    {
      fprintf (stderr, "unable to open ftdi device: %d (%s)\n", rc,
	       ftdi_get_error_string (&ftdic));
      exit(1);
    }

  ftdi_usb_reset (&ftdic);
  ftdi_set_baudrate (&ftdic, 1000000);
  ftdi_usb_purge_buffers (&ftdic);

  printf (" Err : %s \n", ftdic.error_str);

  unsigned char nix[150];
  int j = 0;
  int cr;
  for (j = 0; j < 10; j++) {
    cr = ftdi_read_data (&ftdic, &nix[0], 100);
    printf (" %i \n", cr);
  }

  unsigned char id;
  for(id = 0; id < 0xFE; ++id)
    {
      unsigned char packet[6];
      packet[0] = packet[1] = 0xFF;
      packet[2] = id;
      packet[3] = 0x02;
      packet[4] = 0x01;
      packet[5] = ~(id + 0x02 + 0x01);

      rc = ftdi_write_data(&ftdic, packet, 6);
      if (rc < 0)
	{
	  printf("Error writing: %s\n", ftdic.error_str);
	  continue;
	}

      unsigned char header[4];
      rc = ftdi_read_data(&ftdic, header, 4);
      if (rc < 0)
	{
	  printf("Error reading: %s\n", ftdic.error_str);
	  continue;
	}

      if (id != header[2])
	continue;
      printf("ID: %i  ", id);
      int i;
      for(i = 0; i < 4; ++i)
	printf("%i ", header[i]);
      printf("\n");

      ftdi_usb_purge_buffers(&ftdic);
    }

  return 0;
}

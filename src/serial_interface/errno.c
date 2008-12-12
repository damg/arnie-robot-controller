#include "errno.h"

AR_IO_ERRNO_T ar_io_errno;

const char* ar_io_errstring(AR_IO_ERRNO_T e)
{
  switch(e)
    {
    case AR_IO_EINIT:
      return "Error initializing connection";
    case AR_IO_EOPEN:
      return "Error opening connection";
    case AR_IO_EREAD:
      return "Error reading from connection";
    case AR_IO_EWRITE:
      return "Error writing to connection";
    default:
      return "Unknown IO error";
    }
}

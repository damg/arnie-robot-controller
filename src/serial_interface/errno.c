#include "errno.h"

/** \addtogroup serial_interface */
/*@{*/

/// global var holding error number of the last operation
AR_IO_ERRNO_T ar_io_errno;

const char* ar_io_errstring(AR_IO_ERRNO_T e)
{
  switch(e)
    {
    case AR_IO_EINIT:
      return "Error initializing connection";
    case AR_IO_EOPEN:
      return "Error opening connection";
    case AR_IO_ERESET:
      return "Error resetting device";
    case AR_IO_EREAD:
      return "Error reading from connection";
    case AR_IO_EREADHEADER:
      return "Received illegal packet header";
    case AR_IO_EREADNODATA:
      return "Received less data than expected";
    case AR_IO_EWRITE:
      return "Error writing to connection";
    case AR_IO_EINVBAUD:
      return "Requested invalid baud rate";
    case AR_IO_ESETBAUD:
      return "Failed to set requested baud rate";
    case AR_IO_ECLOSE:
      return "Failed to close connection";
    default:
      return "Unknown IO error";
    }
}

/*@}*/

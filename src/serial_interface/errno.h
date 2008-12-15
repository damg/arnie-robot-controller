/** \addtogroup serial_interface */
/*@{*/

#ifndef __AR_IO_ERRNO_H__
#define __AR_IO_ERRNO_H__

/// Possible ar_io_errno values
typedef enum {
  /// Error initializing connection port
  AR_IO_EINIT,
  /// Error opening connection port
  AR_IO_EOPEN,
  /// Error resetting connection port
  AR_IO_ERESET,
  /// Error reading from connection
  AR_IO_EREAD, 
  /// Received data holds invalid packet header
  AR_IO_EREADHEADER, 
  /// No or not enough data received
  AR_IO_EREADNODATA, 
  /// Error writing to connection
  AR_IO_EWRITE, 
  /// Requested invalid baud rate
  AR_IO_EINVBAUD, 
  /// Failed to set requested baud rate
  AR_IO_ESETBAUD, 
  /// Failed to close connection
  AR_IO_ECLOSE, 
} AR_IO_ERRNO_T;

/** \brief Every function in ar_io namespace sets an error code from
    AR_IO_ERRNO_T on failure. If an operation failed, the user may
    obtain more information by reading this variable. */
extern AR_IO_ERRNO_T ar_io_errno;

/** \brief Converts an error code to its human-readable string
    representation. */
const char* ar_io_errstring(AR_IO_ERRNO_T e);

#endif

/*@}*/

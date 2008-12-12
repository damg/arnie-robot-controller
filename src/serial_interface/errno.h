#ifndef __AR_IO_ERRNO_H__
#define __AR_IO_ERRNO_H__

typedef enum {
  AR_IO_EINIT,
  AR_IO_EOPEN,
  AR_IO_EREAD,
  AR_IO_EWRITE,
} AR_IO_ERRNO_T;

extern AR_IO_ERRNO_T ar_io_errno;

const char* ar_io_errstring(AR_IO_ERRNO_T e);

#endif

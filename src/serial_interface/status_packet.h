/** \addtogroup serial_interface */

/*\@{*/

#ifndef __AR_IO_STATUS_PACKET_H__
#define __AR_IO_STATUS_PACKET_H__

#include <stdlib.h>


/** Represents a packet returned by a dynamixel. */
struct ar_io_status_packet
{
  unsigned char id, /// Id of the replying dynamixel
    error, /// Error bitmask
    param_count, /// Count of parameters
    *params; /// Parameters
};

typedef struct ar_io_status_packet ar_io_status_packet_t;

/** 
 * \brief Initialize a new status packet on heap from an incoming
 * byte string.
 * \note{If data is corrupted, the behaviour is undefined.
 * \param data Data byte string.
 * 
 * \return NULL if the header is corrupted. Pointer to the new
 * structure otherwise. Params must be freed from heap manually after use.
 */
ar_io_status_packet_t*
ar_io_status_packet_init(const unsigned char* data);

/** 
 * \brief Remove a status packet from heap. Frees internal param
 *   vector, too.
 * \param p Packet to be freed.
 */
void
ar_io_status_packet_free(ar_io_status_packet_t *p);

#endif

/*\@}*/

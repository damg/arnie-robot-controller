/**
    \page serial_interface serial_interface API documentation

    Library to talk to dynamixel robots. You find the latest versions
    of the source at
    http://www.github.org/damg/arnie-robot-controller
*/

/**
   \addtogroup serial_interface
*/

/*@{*/

#ifndef __ARNIE_IO_CONNECTION_H__
#define __ARNIE_IO_CONNECTION_H__

#include <ftdi/ftdi.h>
#include "instruction_packet.h"
#include "status_packet.h"
#include "control_table.h"

/**
 * \brief Open a new connection on the first available port with baud
 * rate of 1000000.
 * 
 * \param ftdic FTDI context to be used. \note{If ftdic is NULL,
 * behaviour is undefined.}  
 * \return 0 on success. -1 on failure.
 * Sets ar_io_errno to AR_IO_EINIT on ftdic initialization failure.
 * Sets ar_io_errno to AR_IO_EOPEN on connection opening failure.
 * Sets ar_io_errno to AR_IO_ERESET on connection resetting failure.
 * Sets ar_io_errno to AR_IO_EINVBAUD if invalid baud rate has been
 * supplied.  Sets ar_io_errno to AR_IO_ESETBAUD if requested baud
 * could not be set.
 */
int ar_io_open(struct ftdi_context* ftdic);
/**
 * \brief Close an existing connection.
 *
 * \param ftdic ftdi context to be closed. \note{If ftdic is NULL,
 * behaviour is undefined.
 * \return 0 on success, -1 on failure.
 * Sets ar_io_errno to AR_IO_ECLOSE on close failure.
 */
int ar_io_close(struct ftdi_context* ftdic);

/**
 * \brief writes an instruction packet to a connection.
 *
 * \param ftdic FTDI context to be used. \note{If ftdic is NULL,
 * behaviour is undefined.}
 * 
 * \param p instruction packet to be used. \note{If p is NULL,
 * behaviour is undefined.
 * 
 * \return 0 on success, -1 on failure.
 * Sets ar_io_errno to AR_IO_EWRITE on write failure.
 */
int ar_io_write_instruction_packet(struct ftdi_context *ftdic,
				   const struct ar_io_instruction_packet *p);

/**
 * \brief Reads an instruction packet into supplied packet structure.
 * 
 * \param ftdic FTDI context to be used. \note{If ftdic is NULL,
 * behaviour is undefined.
 * 
 * \param p Instruction packet structure to be written into. Internal params vector is reallocated to required size. \note{If p is NULL, behaviour is undefined} \note{Param pointer may be NULL}
 * 
 * \return 0 on success, -1 on failure.
 * 
 * Sets ar_io_errno to AR_IO_EREADNODATA if no or not enough data was
 * available on the port.
 *
 * Sets ar_io_errno to AR_IO_EREADHEADER if received data didn't match
 * packet header specification.
 *
 * Sets ar_io_errno to AR_IO_EREAD on other read failure.
 */
int ar_io_read_status_packet(struct ftdi_context *ftdic,
			     struct ar_io_status_packet *p);


/**
   \brief Read the control table of a dynamixel.
   \param ftdic FTDI context to use.
   \param id ID of the dynamixel.
   \param ct Control table structure to write into.
   \note ct may not be NULL.
   \note ftdic may not be NULL.
   \note id must not be 0xFF.
   \return 0 on success, -1 on failure.
*/
int ar_io_read_control_table(struct ftdi_context *ftdic,
			     unsigned char id,
			     struct ar_io_control_table *ct);
/**
   \brief Set the goal position of a dynamixel.
   \param ftdic FTDI context to use.
   \param id ID of the dynamixel.

   \param sp Status packet to write status information into.

   \return 0 on success, -1 on failure.
   \note ftdic may not be NULL.
   \note sp may not be NULL.
   \note Only IO errors are reported via return code. Logical
   operation errors must be checked by using sp->error.
*/
int ar_io_set_goal_position(struct ftdi_context *ftdic,
			    unsigned char id,
			    unsigned short goal_position,
			    struct ar_io_status_packet *sp);

/**
   \brief Set the moving speed of a dynamixel.
   \param ftdic FTDI context to use.
   \param id ID of the dynamixel.

   \param sp Status packet to write status information into. 

   \return 0 on success, -1 on failure.
   \note ftdic may not be NULL.
   \note sp may not be NULL.
   \note Only IO errors are reported via return code. Logical
   operation errors must be checked by using sp->error.
*/
int ar_io_set_moving_speed(struct ftdi_context *ftdic,
			    unsigned char id,
			    unsigned short moving_speed,
			    struct ar_io_status_packet *sp);

#endif

/*@}*/

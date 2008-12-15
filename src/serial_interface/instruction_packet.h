/** \addtogroup serial_interface */
/*@{*/

#ifndef __AR_IO_INSTRUCTION_PACKET_H__
#define __AR_IO_INSTRUCTION_PACKET_H__

/** Instruction packet structure */
struct ar_io_instruction_packet
{
  unsigned char id, /// id of the target dynamixel
    instruction,  /// instruction code
    param_count, /// number of parameters
    *params; /// parameters
};

typedef struct ar_io_instruction_packet ar_io_instruction_packet_t;

/** 
 * \brief initializes a new instruction packet on heap.
 * \param id ID of the target dynamixel
 * \param instruction Instruction code.
 * \param params Parameters of the instruction.
 * \param param_count Count of parameters.
 */
ar_io_instruction_packet_t*
ar_io_instruction_packet_init(unsigned char id,
			      unsigned char instruction,
			      const unsigned char* params,
			      unsigned char param_count);

/** 
 * \brief Frees allocated instruction packet.
 * \note{Deallocates memory referenced by params}
 * \param p pointer to memory to be deallocated.
 */
void
ar_io_instruction_packet_free(ar_io_instruction_packet_t* p);

/** 
 * \brief Converts p to byte representation ready to be used for
 * communication with AX-12.
 * \note{Asserts p is not NULL}
 * \param p pointer to the structure to be converted
 *
 * \return Pointer to byte string allocated on heap. \note{Allocated
 * memory has to be deallocated via free after use.}
 */
unsigned char*
ar_io_instruction_packet_to_bytes(const ar_io_instruction_packet_t* p);

#endif

/*@}*/

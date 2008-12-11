#ifndef __AR_IO_INSTRUCTION_PACKET_H__
#define __AR_IO_INSTRUCTION_PACKET_H__

struct ar_io_instruction_packet
{
  unsigned char id, instruction, param_count, *params;
};

typedef struct ar_io_instruction_packet ar_io_instruction_packet_t;

/** \brief Initialisiert ein neues Instruktionspaket. Inhalt von
 * params wird in ein internes, auf dem Heap alloziertes Array
 * kopiert.
 * 
 * \param id Die ID des Zieldynamixels 0..0xFE
 * \param instruction Zu versendende Instruktion
 * \param params Parameter der Instruktion
 * \param param_count Anzahl der Parameter
 * \return Pointer auf die neue Paket-struktur
 */
ar_io_instruction_packet_t*
ar_io_instruction_packet_init(unsigned char id,
			      unsigned char instruction,
			      const unsigned char* params,
			      unsigned char param_count);

/** \brief Entfernt das Paket vom Heap. Der interne Parameter-Vektor
 * wird vom Heap entfernt. 
 * 
 * \param Pointer aufs zu entfernende Paket
 */
void
ar_io_instruction_packet_free(ar_io_instruction_packet_t* p);

/** \brief Konvertiert ein Paket in einen Byte-Vektor, der
 * versandbereit an Dynamixel ist.
 *
 * \param p Pointer auf ein Paket-objekt. Wenn p NULL ist, ist das
 * Verhalten undefiniert.
 * \return Pointer auf ein auf dem Heap allozierten Byte-Vektor
 */
unsigned char*
ar_io_instruction_packet_to_bytes(ar_io_instruction_packet_t* p);

#endif

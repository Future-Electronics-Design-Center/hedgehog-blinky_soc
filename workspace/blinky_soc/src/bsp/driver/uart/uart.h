/*   ==================================================================

     >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
     ------------------------------------------------------------------
     Copyright (c) 2019-2024 by Lattice Semiconductor Corporation
     ALL RIGHTS RESERVED
     ------------------------------------------------------------------

       IMPORTANT: THIS FILE IS USED BY OR GENERATED BY the LATTICE PROPEL™
       DEVELOPMENT SUITE, WHICH INCLUDES PROPEL BUILDER AND PROPEL SDK.

       Lattice grants permission to use this code pursuant to the
       terms of the Lattice Propel License Agreement.

     DISCLAIMER:

    LATTICE MAKES NO WARRANTIES ON THIS FILE OR ITS CONTENTS,
    WHETHER EXPRESSED, IMPLIED, STATUTORY,
    OR IN ANY PROVISION OF THE LATTICE PROPEL LICENSE AGREEMENT OR
    COMMUNICATION WITH LICENSEE,
    AND LATTICE SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
    MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
    LATTICE DOES NOT WARRANT THAT THE FUNCTIONS CONTAINED HEREIN WILL MEET
    LICENSEE 'S REQUIREMENTS, OR THAT LICENSEE' S OPERATION OF ANY DEVICE,
    SOFTWARE OR SYSTEM USING THIS FILE OR ITS CONTENTS WILL BE
    UNINTERRUPTED OR ERROR FREE,
    OR THAT DEFECTS HEREIN WILL BE CORRECTED.
    LICENSEE ASSUMES RESPONSIBILITY FOR SELECTION OF MATERIALS TO ACHIEVE
    ITS INTENDED RESULTS, AND FOR THE PROPER INSTALLATION, USE,
    AND RESULTS OBTAINED THEREFROM.
    LICENSEE ASSUMES THE ENTIRE RISK OF THE FILE AND ITS CONTENTS PROVING
    DEFECTIVE OR FAILING TO PERFORM PROPERLY AND IN SUCH EVENT,
    LICENSEE SHALL ASSUME THE ENTIRE COST AND RISK OF ANY REPAIR, SERVICE,
    CORRECTION,
    OR ANY OTHER LIABILITIES OR DAMAGES CAUSED BY OR ASSOCIATED WITH THE
    SOFTWARE.IN NO EVENT SHALL LATTICE BE LIABLE TO ANY PARTY FOR DIRECT,
    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,
    INCLUDING LOST PROFITS,
    ARISING OUT OF THE USE OF THIS FILE OR ITS CONTENTS,
    EVEN IF LATTICE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
    LATTICE 'S SOLE LIABILITY, AND LICENSEE' S SOLE REMEDY,
    IS SET FORTH ABOVE.
    LATTICE DOES NOT WARRANT OR REPRESENT THAT THIS FILE,
    ITS CONTENTS OR USE THEREOF DOES NOT INFRINGE ON THIRD PARTIES'
    INTELLECTUAL PROPERTY RIGHTS, INCLUDING ANY PATENT. IT IS THE USER' S
    RESPONSIBILITY TO VERIFY THE USER SOFTWARE DESIGN FOR CONSISTENCY AND
    FUNCTIONALITY THROUGH THE USE OF FORMAL SOFTWARE VALIDATION METHODS.
     ------------------------------------------------------------------

     ================================================================== */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

#define _UART_NO_INTERRUPTS_

/* LINE STATUS REGISTER BITS USED IN THE CODE */
#define UART_LSR_RX_RDY_MASK    (0x01)
#define UART_LSR_PE_MASK        (0x04)
#define UART_LSR_FE_MASK        (0x08)
#define UART_LSR_TX_RDY_MASK    (0x20)

/* INTERRUPT ENABLE REGISTER BITS USED IN THE CODE */
#define UART_IER_RX_INT_MASK    (0x01)
#define UART_IER_TX_INT_MASK    (0x02)

/* INTERRUPT IDENTIFICATION REGISTER BITS USED IN THE CODE */
#define UART_IIR_RXRDY          (0x04)
#define UART_IIR_TXRDY          (0x02)

/*
 ------------------------------------------------------
 -                                                    -
 - UART Device Register-map                           -
 -                                                    -
 ------------------------------------------------------
 */
struct uart_dev {
	volatile unsigned int rxtx;
	volatile unsigned int ier;
	volatile unsigned int iir;
	volatile unsigned int lcr;
	volatile unsigned int reserved1;
	volatile unsigned int lsr;
	volatile unsigned int reserved2;
	volatile unsigned int reserved3;
	volatile unsigned int dlr_lsb;
	volatile unsigned int dlr_msb;
};

#define UART_BUFFER_SIZE    16

struct uart_instance {
	const char *name;
	unsigned int base;
	unsigned int sys_clk;
	unsigned char intrLevel;
	unsigned char intrAvail;
	unsigned int baudrate;
	unsigned int databits;
	unsigned int stopbits;
	unsigned char rxBufferSize;
	unsigned char txBufferSize;
	unsigned char blockingTx;
	unsigned char blockingRx;
	unsigned int fifoenable;
	unsigned char rxBuffer[UART_BUFFER_SIZE];
	unsigned char txBuffer[UART_BUFFER_SIZE];
	unsigned char rxWriteLoc;
	unsigned char rxReadLoc;
	unsigned char txWriteLoc;
	unsigned char txReadLoc;
	unsigned int timeoutMicroSecs;
	volatile unsigned char txDataBytes;
	volatile unsigned char rxDataBytes;
	unsigned int errors;
	unsigned char ier;
};

#define UART_ERR_WOULD_BLOCK        (1)
#define UART_ERR_DEVICE_ERROR       (2)
#define UART_ERR_INVALID_ARGUMENT   (3)

unsigned char uart_init(struct uart_instance *this_uart,
			unsigned int base_addr,
			unsigned int sys_clk,
			unsigned int baud_rate,
			unsigned char stop_bits, unsigned char data_width);
unsigned char uart_putc(struct uart_instance *this_uart,
			unsigned char ucChar);
unsigned char uart_getc(struct uart_instance *this_uart,
			unsigned char *pucChar);
unsigned char uart_set_rate(struct uart_instance *this_uart,
			    unsigned int baudrate);
unsigned char uart_config(struct uart_instance *this_uart,
			  unsigned int dwidth, unsigned char parity_en,
			  unsigned char even_odd, unsigned int stopbits);

#endif				/* UART_H_ */

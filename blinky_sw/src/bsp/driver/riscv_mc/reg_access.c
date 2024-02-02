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

#include "reg_access.h"

uint8_t reg_32b_write(uint32_t reg_addr, uint32_t value)
{
	uint32_t volatile *reg_value = (void *) reg_addr;

	*reg_value = value;
	return 0;
}

uint8_t reg_32b_read(uint32_t reg_addr,
			   uint32_t *reg_32b_value)
{
	uint32_t volatile *reg_value = (void *) reg_addr;
	*reg_32b_value = *reg_value;

	return 0;
}

uint8_t reg_32b_modify(uint32_t reg_addr, uint32_t bits_mask,
			     uint32_t value)
{
	uint32_t volatile *reg_value = (void *) reg_addr;

	//*reg_value = ((*reg_value) & ~bits_mask) | value;
	*reg_value = ((*reg_value) & (~bits_mask)) | (value & bits_mask);
	return 0;
}

uint8_t reg_16b_write(uint32_t reg_addr, unsigned short value)
{
	unsigned short volatile *reg_value = (void *) reg_addr;

	*reg_value = value;
	return 0;
}

uint8_t reg_16b_read(uint32_t reg_addr,
			   unsigned short *reg_16b_value)
{
	unsigned short volatile *reg_value = (void *) reg_addr;
	*reg_16b_value = *reg_value;
	return 0;
}

uint8_t reg_8b_write(uint32_t reg_addr, uint8_t value)
{
	uint8_t volatile *reg_value = (void *) reg_addr;

	*reg_value = value;
	return 0;
}

uint8_t reg_8b_modify(uint32_t reg_addr, uint8_t bits_mask,
			    uint8_t value)
{
	uint8_t volatile *reg_value = (void *) reg_addr;

	*reg_value = ((*reg_value) & (~bits_mask)) | (value & bits_mask);
	return 0;
}

uint8_t reg_8b_read(uint32_t reg_addr,
			  uint8_t *reg_8b_value)
{
	uint8_t volatile *reg_value = (void *) reg_addr;
	*reg_8b_value = *reg_value;
	return 0;
}

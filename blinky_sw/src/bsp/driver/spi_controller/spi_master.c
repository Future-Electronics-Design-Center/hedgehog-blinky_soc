/*
 * =================================================================
     >>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
     ------------------------------------------------------------------
     Copyright (c) 2006-2024 by Lattice Semiconductor Corporation
     ALL RIGHTS RESERVED
     ------------------------------------------------------------------

     IMPORTANT: THIS FILE IS AUTO-GENERATED BY LATTICE RADIANT Software.

     Permission:

	Lattice grants permission to use this code pursuant to the
	terms of the Lattice Corporation Open Source License Agreement.

     Disclaimer:

	Lattice provides no warranty regarding the use or functionality
	of this code. It is the user's responsibility to verify the
	user Software design for consistency and functionality through
	the use of formal Software validation methods.

     ------------------------------------------------------------------

     Lattice Semiconductor Corporation
     111 SW Fifth Avenue, Suite 700
     Portland, OR 97204
     U.S.A

     Email: techsupport@latticesemi.com
     Web: http://www.latticesemi.com/Home/Support/SubmitSupportTicket.aspx
     ==================================================================
*/

#include "spi_master.h"
#include "spi_master_regs.h"
#include "hal.h"

uint8_t spi_master_init(struct spim_instance *this_spim,
			uint32_t base_addr,
			uint8_t slave_count,
			uint32_t *tx_buf,
			uint32_t *rx_buf,
			uint32_t prescaler,
			uint32_t spi_rx_fifo_afull_flag,
			uint32_t spi_rx_fifo_depth,
			uint32_t spi_tx_fifo_depth)
{
	if(this_spim == NULL)
		return PTR_ERROR;

	if(spi_rx_fifo_depth < spi_rx_fifo_afull_flag)
		return SPI_RX_FIFO_ERR;

	this_spim->base_address = base_addr;
	this_spim->slave_count = slave_count;
	this_spim->spim_rxbuf = rx_buf;
	this_spim->spim_txbuf = tx_buf;
	this_spim->spim_rx_fifo_afull_flag = spi_rx_fifo_afull_flag;
	this_spim->spi_rx_fifo_depth = spi_rx_fifo_depth;
	this_spim->spi_tx_fifo_depth = spi_tx_fifo_depth;

	return SUCCESS;
}

uint8_t spi_master_config(struct spim_instance *this_spim,
                          spim_data_width data_width,
                          uint32_t prescaler,
                          uint8_t clk_polarity,uint8_t lsb_first, uint8_t clk_phase,uint8_t ssnp, uint8_t only_write ,uint8_t spi_en)
{
	uint8_t spi_config = 0;

	if(this_spim == NULL)
		return PTR_ERROR;


	this_spim->data_width = data_width;
	this_spim->spi_clk_polarity = clk_polarity;
	this_spim->spi_clk_phase = clk_phase;

	spi_config =
			 (spi_en << 7) | (lsb_first << 6) |  (only_write << 5) | (data_width << 3) | (ssnp<<2) |(clk_polarity << 1) | clk_phase;
	reg_32b_modify(this_spim->base_address | SPI_MASTER_SPI_CFG,SPI_CFG_DATA_WIDTH_MASK | SPI_CFG_LSB_FIRST_MASK | SPI_CFG_CPOL_MASK | SPI_CFG_CPHA_MASK | SPI_CFG_SSNP_MASK | SPI_CFG_SPI_EN_MASK | SPI_CFG_WR_ONLY_MASK, spi_config);

	// clear the tx/rx fifo
	reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST,
	                 SPI_MASTER_TX_FIFO_RST|SPI_MASTER_RX_FIFO_RST);

	// enable all the INTs by default
	reg_8b_write(this_spim->base_address | SPI_MASTER_INT_ENABLE,BITS_ALL_SET);

	reg_8b_write(this_spim->base_address | SPI_MASTER_BYTE_COUNT_RST,BITS_ALL_SET);

	// manually clear the FIFO reset flag
	reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST, BITS_ALL_CLEAR);
    reg_8b_write(this_spim->base_address | SPI_MASTER_CLK_PRESCALER_LOW, prescaler & BITS_ALL_SET);
    reg_8b_write(this_spim->base_address | SPI_MASTER_CLK_PRESCALER_HIGH, prescaler >> 8);
    reg_8b_write(this_spim->base_address | SPI_MASTER_BYTE_COUNT_RST, BITS_ALL_SET);
	return SUCCESS;
}


uint8_t spi_master_transfer(struct spim_instance *this_spim,
			    uint32_t *data_buffer_out,
			    uint32_t byte_out_size,
			    uint32_t byte_in_size)
{
	uint32_t byte_out_cnt = 0;
	uint8_t int_status = 0;
	uint8_t fifo;

	if(this_spim->spim_current_status == SPIM_STATE_BUSY){
		return STATE_ERROR;
	}

	if(this_spim == NULL)
		return PTR_ERROR;

    // clear the tx/rx fifo
    reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST,
                     SPI_MASTER_TX_FIFO_RST|SPI_MASTER_RX_FIFO_RST);

	// clear all the interrupt bits
	reg_8b_write(this_spim->base_address | SPI_MASTER_INT_STATUS,
		     BITS_ALL_SET);

    // clear the tx/rx fifo reset flag
    reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST, 0);

    reg_8b_write(this_spim->base_address | SPI_MASTER_BYTE_COUNT_RST, BITS_ALL_SET);
    // set the word count register
    reg_8b_write(this_spim->base_address | SPI_MASTER_TARGET_WORD_COUNT,byte_out_size+byte_in_size);

    // write the data into the out buffer

    this_spim->spim_current_status = SPIM_STATE_BUSY;

	for(byte_out_cnt = 0;byte_out_cnt < byte_out_size;byte_out_cnt++)
	{
		reg_32b_write(this_spim->base_address |
	                     SPI_MASTER_WR_DATA, *data_buffer_out);
	    data_buffer_out++;

	}

	for(;byte_out_cnt < byte_in_size + byte_out_size ; byte_out_cnt++)
	{
	    reg_32b_write(this_spim->base_address | SPI_MASTER_WR_DATA, 0xffffffff);// write dummy byte
	}

	return SUCCESS;
}


uint8_t spi_master_transfer_blocking(struct spim_instance *this_spim,
			    uint32_t *data_buffer_out,
			    uint32_t byte_out_size,
			    uint32_t *data_buffer_in,
			    uint32_t byte_in_size)
{
	uint32_t byte_out_cnt = 0;
	uint32_t byte_in_cnt = 0;
	uint32_t value;
	uint8_t int_status = 0;
	uint8_t fifo;

	if(this_spim->spim_current_status == SPIM_STATE_BUSY){
			return STATE_ERROR;
	}
	if(this_spim == NULL)
		return PTR_ERROR;

    // clear the tx/rx fifo
    reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST,
                     SPI_MASTER_TX_FIFO_RST|SPI_MASTER_RX_FIFO_RST);

	// clear all the interrupt bits
	reg_8b_write(this_spim->base_address | SPI_MASTER_INT_STATUS,
		     BITS_ALL_SET);

    // clear the tx/rx fifo reset flag
    reg_8b_write(this_spim->base_address | SPI_MASTER_FIFO_RST, 0);

    reg_8b_write(this_spim->base_address | SPI_MASTER_BYTE_COUNT_RST, BITS_ALL_SET);
    // set the byte count register
    reg_8b_write(this_spim->base_address | SPI_MASTER_TARGET_WORD_COUNT,byte_out_size+byte_in_size);

	while(1)
	{
	    reg_8b_read(this_spim->base_address |
	            SPI_MASTER_INT_STATUS, &int_status);
	    /* If the transmit buffer full interrupt is triggered */
	    if(int_status & (SPI_INT_TX_BUFFER_FULL))
	    {
	    	do{
	   			 reg_8b_read(this_spim->base_address |
	   			 		            SPI_MASTER_INT_STATUS, &int_status);
	   		 }
	   		 while(int_status & (SPI_INT_TX_BUFFER_FULL));
	    }
	    /* If the receive buffer almost full interrupt is triggered */
	    if(int_status & (SPI_INT_RX_BUFFER_AFULL))
	    {
	    	for(;byte_in_cnt < byte_in_size;byte_in_cnt++)
	    	{
	    		reg_32b_read(this_spim->base_address |
	    				SPI_MASTER_RD_DATA, data_buffer_in);

	    		data_buffer_in++;
	    	}
	    }
	    /* If the transfer complete interrupt is triggered */
	    if(int_status & (SPI_INT_TRAN_CMP))
	    {
	        reg_8b_read(this_spim->base_address |
	    			SPI_MASTER_FIFO_STATUS, &fifo);
	    	if((fifo & SPI_MASTER_RX_FIFO_EMPTY) == 0)
	    	{

	    		for(;byte_in_cnt < byte_in_size;byte_in_cnt++)
				{
					reg_32b_read(this_spim->base_address |
							SPI_MASTER_RD_DATA, data_buffer_in);

					data_buffer_in++;

				}
	    	}
	    	reg_8b_write(this_spim->base_address |
	    	                  SPI_MASTER_INT_STATUS, int_status & BITS_ALL_SET);
	    	break;
	   	}
	    /*transfer not done,continue to send data */
	    else if(byte_out_cnt < byte_in_size + byte_out_size)
	    {
	        if(byte_out_cnt < byte_out_size)
	        {
	            reg_32b_write(this_spim->base_address |
	                     SPI_MASTER_WR_DATA, *data_buffer_out);


	            data_buffer_out++;

	        }
	        else if(byte_out_cnt < byte_in_size + byte_out_size)
	        {
	        	reg_32b_write(this_spim->base_address |
	                                 SPI_MASTER_WR_DATA, 0xffffffff);  // write dummy byte

            }

            byte_out_cnt++;
        }

	}
	return SUCCESS;
}

uint8_t spi_device_select(struct spim_instance *this_spim,
			  uint8_t enable_slave)
{
	if(this_spim == NULL)
		return PTR_ERROR;

	reg_8b_write(this_spim->base_address | SPI_MASTER_SLAVE_SEL,
		     enable_slave);
	reg_8b_write(this_spim->base_address | SPI_MASTER_POL_SEL,
			     0);
	return SUCCESS;
}

uint8_t spi_device_enable(struct spim_instance *this_spim, uint8_t enable_slave)
{
	if(this_spim == NULL)
		return PTR_ERROR;

	reg_8b_write(this_spim->base_address | SPI_ENABLE_REG,
			     enable_slave);

	return SUCCESS;
}

/* Interrupt service routine for SPI master */
void spi_master_isr(void* ctx)
{
    /* Get the SPI master instance */
    volatile struct spim_instance *this_spim = (struct spim_instance *) ctx;
    uint8_t buffer_status;
    uint8_t fifo_status;
    uint8_t int_status = 0;

    /* Read the interrupt status register */
    reg_8b_read(this_spim->base_address |
                SPI_MASTER_INT_STATUS, &int_status);

    /* If the transmit buffer full interrupt is triggered */
    if(int_status & (SPI_INT_TX_BUFFER_FULL))
    {
        /* Wait until the transmit FIFO is no longer full */
        do{
            reg_8b_read(this_spim->base_address |
                        SPI_MASTER_FIFO_STATUS, &fifo_status);
        }
        while(fifo_status & (SPI_MASTER_TX_FIFO_FULL));
    }

    /* If the transfer complete interrupt is triggered */
    if(int_status & (SPI_INT_TRAN_CMP))
    {
        /* Set the current status to read state */
        this_spim->spim_current_status = SPIM_STATE_IDLE;

        /* Read the received data and store it in the receive buffer */
        for(uint32_t i = 0 ; i < this_spim->spi_rx_fifo_depth ; i++)
        {
            reg_32b_read(this_spim->base_address |
                         SPI_MASTER_RD_DATA,this_spim->spim_rxbuf + this_spim->rx_input_cnt);
            this_spim->rx_input_cnt++;
        }
        /* Reset the counter */
        this_spim->rx_input_cnt = 0;
    }

    /* If the receive buffer almost full interrupt is triggered */
    if(int_status & (SPI_INT_RX_BUFFER_AFULL))
    {
        /* Read the received data and store it in the receive buffer */
        for(uint32_t i = 0 ; i < this_spim->spim_rx_fifo_afull_flag ; i++)
        {
            reg_32b_read(this_spim->base_address |
                         SPI_MASTER_RD_DATA,this_spim->spim_rxbuf + this_spim->rx_input_cnt);
            this_spim->rx_input_cnt++;
        }
    }

    /* Clear all pending interrupts */
    reg_8b_write(this_spim->base_address |
                 SPI_MASTER_INT_STATUS, int_status & BITS_ALL_SET);
}

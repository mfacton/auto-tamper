/*
 * receiver.c
 *
 *  Created on: Jul 4, 2023
 *      Author: mfact
 */

#include <modules/receiver.h>
#include "lib/nrf24l01p.h"

static uint8_t data_ready;
static uint8_t data;

static nrf24l01p_HandleTypeDef h_nrf;

void Receiver_Init(SPI_HandleTypeDef *hspi) {
	nrf24l01p_create_handle(&h_nrf, hspi, 1000, NRF_CSN_GPIO_Port, NRF_CSN_Pin, NRF_CE_GPIO_Port, NRF_CE_Pin);

	data_ready = 0;
	data = 0;

	//configure nrf
	nrf24l01p_set_mode(&h_nrf, NRF_RECEIVE);//recieve mode
	nrf24l01p_set_crc_en(&h_nrf, NRF_ON);//enable crc
	nrf24l01p_set_crc_bits(&h_nrf, NRF_CRC_16);//crc to 2 bytes
	nrf24l01p_set_rt_count(&h_nrf, NRF_RT_OFF);//disable retransmits
	nrf24l01p_set_frequency(&h_nrf, 2476);//set frequency to 2476 Mhz
	nrf24l01p_set_data_rate(&h_nrf, NRF_250KBPS);//set data rate to 250 kbps
	nrf24l01p_set_output_power(&h_nrf, NRF_0DBM);//set output power to +0dBm (only really needed for tx mode)

	//configure pipe 1
	uint8_t address[5] = "00001";
	nrf24l01p_set_rx_pipe_en(&h_nrf, NRF_PIPE_1, NRF_ON);//enable rx pipe 1
	nrf24l01p_set_payload_size(&h_nrf, NRF_PIPE_1, 1);//set payload to 1 byte
	nrf24l01p_set_rx_addr(&h_nrf, NRF_PIPE_1, address, 5);//set address

	//configure interrupts to rx only
	nrf24l01p_flush_all(&h_nrf);
	nrf24l01p_set_irq_rx_en(&h_nrf, NRF_ON);
	nrf24l01p_set_irq_tx_en(&h_nrf, NRF_OFF);
	nrf24l01p_set_irq_rt_en(&h_nrf, NRF_OFF);
	nrf24l01p_reset_irq_rx(&h_nrf);
	nrf24l01p_reset_irq_tx(&h_nrf);
	nrf24l01p_reset_irq_rt(&h_nrf);

	//ready nrf
	nrf24l01p_set_power(&h_nrf, NRF_ON);
	nrf24l01p_set_ce(&h_nrf, NRF_ON);
}

void Receiver_Handler() {
	data_ready = 1;
}

uint8_t Receiver_Available() {
	return data_ready;
}

uint8_t Receiver_Read() {
	nrf24l01p_irq_handler(&h_nrf);
	data_ready = 0;
	return data;
}

//overrided
void nrf24l01p_receive_callback(uint8_t *buf, uint8_t len) {
	data = buf[0];
	UNUSED(len);
}

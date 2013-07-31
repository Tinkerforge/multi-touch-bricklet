/* multi-touch-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * multi-touch.c: Implementation of Multi Touch Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "multi-touch.h"

#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/util_definitions.h"
#include "bricklib/utility/init.h"
#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "config.h"

const uint8_t mpr121_config[][2] = {
	{ELE_CFG, 0x00},
	// Rising (when the electrode data is greater than the baseline value).
	{MHD_R, 0x01}, // Largest magnitude of variation to pass through baseline filter (1-63).
	{NHD_R, 0x01}, // Incremental Change when non-noise drift is detected (1-63).
	{NCL_R, 0x00}, // Number of samples consecutively greater than the Max Half Delta value (0-255).
	{FDL_R, 0x00}, // Operation rate of the filter A larger count limit means the filter delay is operating more slowly (0-255).
	// Falling (when the electrode data is less than the baseline value).
	{MHD_F, 0x01},
	{NHD_F, 0x01},
	{NCL_F, 0xFF},
	{FDL_F, 0x02},
	{ELE0_T, TOUCH_THRESHOLD},
	{ELE0_R, RELEASE_THRESHOLD},
	{ELE1_T, TOUCH_THRESHOLD},
	{ELE1_R, RELEASE_THRESHOLD},
	{ELE2_T, TOUCH_THRESHOLD},
	{ELE2_R, RELEASE_THRESHOLD},
	{ELE3_T, TOUCH_THRESHOLD},
	{ELE3_R, RELEASE_THRESHOLD},
	{ELE4_T, TOUCH_THRESHOLD},
	{ELE4_R, RELEASE_THRESHOLD},
	{ELE5_T, TOUCH_THRESHOLD},
	{ELE5_R, RELEASE_THRESHOLD},
	{ELE6_T, TOUCH_THRESHOLD},
	{ELE6_R, RELEASE_THRESHOLD},
	{ELE7_T, TOUCH_THRESHOLD},
	{ELE7_R, RELEASE_THRESHOLD},
	{ELE8_T, TOUCH_THRESHOLD},
	{ELE8_R, RELEASE_THRESHOLD},
	{ELE9_T, TOUCH_THRESHOLD},
	{ELE9_R, RELEASE_THRESHOLD},
	{ELE10_T, TOUCH_THRESHOLD},
	{ELE10_R, RELEASE_THRESHOLD},
	{ELE11_T, TOUCH_THRESHOLD},
	{ELE11_R, RELEASE_THRESHOLD},
//	{ELE_CFG, 0x0C},
//	{ATO_CFG0, 0x0B}, // Auto Config
//	{ATO_CFGU, 0xC9},
//	{ATO_CFGL, 0x82},
//	{ATO_CFGT, 0xB5},
	{ELE_CFG, ELE_CFG_ENABLE_ALL | ELE_CFG_ELEPROX_EN_00 | ELE_CFG_CL_00},
	{ENDOFCONFIG, 0}
};

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_GET_TOUCH_STATE: {
			get_touch_state(com, (GetTouchState*)data);
			return;
		}

		default: {
			BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
			return;
		}
	}
}

void get_touch_state(const ComType com, const GetTouchState *data) {
	GetTouchStateReturn gtsr;
	gtsr.header        = data->header;
	gtsr.header.length = sizeof(GetTouchStateReturn);
	gtsr.state         = BC->state;
	BA->send_blocking_with_timeout(&gtsr,
	                               sizeof(GetTouchStateReturn),
	                               com);
}

void constructor(void) {
	PIN_IRQ.type = PIO_INPUT;
	PIN_IRQ.attribute = PIO_PULLUP;
    BA->PIO_Configure(&PIN_IRQ, 1);

	mpr121_configure();
	BC->state = 0;
	BC->send_state_callback = false;
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!(PIN_IRQ.pio->PIO_PDSR & PIN_IRQ.mask)) {
			uint16_t new_state;
			read_registers(0, (uint8_t*)&new_state, 2);
			new_state &= 0xFFF;
			if(new_state != BC->state) {
				BC->state = new_state;
				BC->send_state_callback = true;
			}
		}
	}
	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		if(BC->send_state_callback) {
			TouchState ts;
			BA->com_make_default_header(&ts, BS->uid, sizeof(TouchState), FID_TOUCH_STATE);
			ts.state = BC->state;

			BA->send_blocking_with_timeout(&ts,
										   sizeof(TouchState),
										   *BA->com_current);
			BC->send_state_callback = false;
		}
	}
}

void mpr121_configure(void) {
	for(uint8_t i = 0; mpr121_config[i][0] != ENDOFCONFIG; i++) {
		write_register(mpr121_config[i][0], mpr121_config[i][1]);
	}
}

void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length) {
	BA->mutex_take(*BA->mutex_twi_bricklet, MUTEX_BLOCKING);
	// TODO: Analog switch on
    BA->TWID_Read(BA->twid,
                  I2C_ADDRESS_MPR121,
                  reg,
                  I2C_INTERNAL_ADDRESS_BYTES,
                  data,
                  length,
                  NULL);
	// TODO: Analog switch off
    BA->mutex_give(*BA->mutex_twi_bricklet);
}

void write_register(const uint8_t reg, uint8_t value) {
	BA->mutex_take(*BA->mutex_twi_bricklet, MUTEX_BLOCKING);
	// TODO: Analog switch on
    BA->TWID_Write(BA->twid,
                   I2C_ADDRESS_MPR121,
                   reg,
                   I2C_INTERNAL_ADDRESS_BYTES,
                   &value,
                   I2C_DATA_LENGTH,
                   NULL);

	// TODO: Analog switch off
    BA->mutex_give(*BA->mutex_twi_bricklet);
}

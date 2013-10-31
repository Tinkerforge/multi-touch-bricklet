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
    // Touch

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

	// Proximity

	// Rising
	{PROX_MHD_R, 0xFF},
	{PROX_NHDA_R, 0xFF},
	{PROX_NCL_R, 0x00},
	{PROX_FDL_R, 0x00},
    // Falling
	{PROX_MHD_F, 0x01},
	{PROX_NHDA_F, 0x01},
	{PROX_NCL_F, 0xFF},
	{PROX_NDL_F, 0xFF},

	// Thresholds

	// Touch
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
	// Proximity
	{PROX_T, PROXIMITY_TOUCH_THRESHOLD},
	{PROX_R, PROXIMITY_RELEASE_THRESHOLD},

	// Auto Config

	{ATO_CFGU, ELECTRODE_SENSITIVITY_MAX},
//	{ATO_CFGL, MPR121_TL_TO_UTL(ELECTRODE_SENSITIVITY_DEFAULT)},
//	{ATO_CFGT, ELECTRODE_SENSITIVITY_DEFAULT},
	{ATO_CFG0, 0x03},

	{ENDOFCONFIG, 0}
};

void invocation(const ComType com, const uint8_t *data) {
	switch(((MessageHeader*)data)->fid) {
		case FID_GET_TOUCH_STATE: {
			get_touch_state(com, (GetTouchState*)data);
			return;
		}

		case FID_RECALIBRATE: {
			recalibrate(com, (Recalibrate*)data);
			return;
		}

		case FID_SET_ELECTRODE_CONFIG: {
			set_electrode_config(com, (SetElectrodeConfig*)data);
			return;
		}

		case FID_GET_ELECTRODE_CONFIG: {
			get_electrode_config(com, (GetElectrodeConfig*)data);
			return;
		}

		case FID_SET_ELECTRODE_SENSITIVITY: {
			set_electrode_sensitivity(com, (SetElectrodeSensitivity*)data);
			return;
		}

		case FID_GET_ELECTRODE_SENSITIVITY: {
			get_electrode_sensitivity(com, (GetElectrodeSensitivity*)data);
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

void recalibrate(const ComType com, const Recalibrate *data) {
	mpr121_reset();
	SLEEP_US(20);
	mpr121_disable();
	mpr121_configure();
	mpr121_enable();

	BC->force_read = true;

	BA->com_return_setter(com, data);
}

void set_electrode_config(const ComType com, const SetElectrodeConfig *data) {
	BC->enabled_electrodes = data->enabled_electrodes & 0x1FFF;
	mpr121_enable();
	BA->com_return_setter(com, data);
}

void get_electrode_config(const ComType com, const GetElectrodeConfig *data) {
	GetElectrodeConfigReturn gecr;
	gecr.header             = data->header;
	gecr.header.length      = sizeof(GetElectrodeConfigReturn);
	gecr.enabled_electrodes = BC->enabled_electrodes;

	BA->send_blocking_with_timeout(&gecr,
	                               sizeof(GetElectrodeConfigReturn),
	                               com);
}

void set_electrode_sensitivity(const ComType com, const SetElectrodeSensitivity *data) {
	BC->sensitivity = BETWEEN(ELECTRODE_SENSITIVITY_MIN, data->sensitivity, ELECTRODE_SENSITIVITY_MAX);
	mpr121_update_sensitivity();
}

void get_electrode_sensitivity(const ComType com, const GetElectrodeSensitivity *data) {
	GetElectrodeSensitivityReturn gesr;
	gesr.header             = data->header;
	gesr.header.length      = sizeof(GetElectrodeSensitivityReturn);
	gesr.sensitivity        = BC->sensitivity;

	BA->send_blocking_with_timeout(&gesr,
	                               sizeof(GetElectrodeSensitivityReturn),
	                               com);
}

void constructor(void) {
    PIN_SDA.type = PIO_INPUT;
    PIN_SDA.attribute = PIO_DEFAULT;
    BA->PIO_Configure(&PIN_SDA, 1);
    PIN_SDA.pio->PIO_CODR = PIN_SDA.mask;

    PIN_SCL.type = PIO_INPUT;
    PIN_SCL.attribute = PIO_DEFAULT;
    BA->PIO_Configure(&PIN_SCL, 1);
    PIN_SCL.pio->PIO_CODR = PIN_SCL.mask;

	PIN_IRQ.type = PIO_INPUT;
	PIN_IRQ.attribute = PIO_PULLUP;
    BA->PIO_Configure(&PIN_IRQ, 1);

	BC->state = 0;
	BC->enabled_electrodes = 0x1FFF;
	BC->send_state_callback = false;
	BC->force_read = false;
	BC->sensitivity = ELECTRODE_SENSITIVITY_DEFAULT;

	mpr121_reset();
	SLEEP_US(20);
	mpr121_disable();
	mpr121_configure();
	mpr121_enable();
}

void destructor(void) {
}

void tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		if(!(PIN_IRQ.pio->PIO_PDSR & PIN_IRQ.mask) || BC->force_read) {
			BC->force_read = false;
			uint16_t new_state;
			read_registers(0, (uint8_t*)&new_state, 2);
			new_state &= BC->enabled_electrodes;
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

void mpr121_reset(void) {
	write_register(SOFT_RESET, 0x63);
}

void mpr121_disable(void) {
	write_register(ELE_CFG, 0x00);
}

void mpr121_update_sensitivity(void) {
	write_register(ATO_CFGL, MPR121_TL_TO_UTL(BC->sensitivity));
	write_register(ATO_CFGT, BC->sensitivity);
}

void mpr121_enable(void) {
	uint16_t value = BC->enabled_electrodes & 0xFFF;
	if(BC->enabled_electrodes & (1 << 12)) {
		value |= ELE_CFG_ELEPROX_EN_11;
	}

	write_register(ELE_CFG, value);
}

void mpr121_configure(void) {
	mpr121_update_sensitivity();

	for(uint8_t i = 0; mpr121_config[i][0] != ENDOFCONFIG; i++) {
		write_register(mpr121_config[i][0], mpr121_config[i][1]);
	}
}

void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS_MPR121 << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_stop();

	i2c_start();
	i2c_send_byte((I2C_ADDRESS_MPR121 << 1) | I2C_READ);
	for(uint8_t i = 0; i < length; i++) {
		data[i] = i2c_recv_byte(i != (length - 1));
	}
	i2c_stop();
}

void write_register(const uint8_t reg, uint8_t value) {
	i2c_start();
	i2c_send_byte((I2C_ADDRESS_MPR121 << 1) | I2C_WRITE);
	i2c_send_byte(reg);
	i2c_send_byte(value);
	i2c_stop();
}

bool i2c_scl_value(void) {
	return PIN_SCL.pio->PIO_PDSR & PIN_SCL.mask;
}

void i2c_scl_high(void) {
	PIN_SCL.pio->PIO_ODR = PIN_SCL.mask;
	while(!i2c_scl_value()); // allow slave to clock-stretch
}

void i2c_scl_low(void) {
	PIN_SCL.pio->PIO_OER = PIN_SCL.mask;
}

bool i2c_sda_value(void) {
	return PIN_SDA.pio->PIO_PDSR & PIN_SDA.mask;
}

void i2c_sda_high(void) {
	PIN_SDA.pio->PIO_ODR = PIN_SDA.mask;
}

void i2c_sda_low(void) {
	PIN_SDA.pio->PIO_OER = PIN_SDA.mask;
}

void i2c_sleep_halfclock(void) {
	SLEEP_NS(I2C_HALF_CLOCK_400KHZ);
}

void i2c_stop(void) {
	i2c_scl_low();
	i2c_sda_low();
	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();
	i2c_sda_high();
	i2c_sleep_halfclock();
}

void i2c_start(void) {
	i2c_scl_high();
	i2c_sleep_halfclock();
	i2c_sda_low();
	i2c_sleep_halfclock();
}

uint8_t i2c_recv_byte(bool ack) {
	uint8_t value = 0;

	for(int8_t i = 7; i >= 0; i--) {
		i2c_scl_low();
		i2c_sda_high(); // allow slave to read
		i2c_sleep_halfclock();
		i2c_scl_high();
		if(i2c_sda_value()) {
			value |= (1 << i);
		}
		i2c_sleep_halfclock();
	}

	// ACK
	i2c_scl_low();
	if(ack) {
		i2c_sda_low();
	} else {
		i2c_sda_high();
	}
	i2c_sleep_halfclock();
	i2c_scl_high();
	i2c_sleep_halfclock();

	return value;
}

bool i2c_send_byte(const uint8_t value) {
	for(int8_t i = 7; i >= 0; i--) {
		i2c_scl_low();
		if((value >> i) & 1) {
			i2c_sda_high();
		} else {
			i2c_sda_low();
		}
		i2c_sleep_halfclock();
		i2c_scl_high();
		i2c_sleep_halfclock();
	}

	// Wait for ACK
	bool ret = false;

	i2c_scl_low();
	i2c_sleep_halfclock();
	i2c_scl_high();
	if(!i2c_sda_value()) {
		ret = true;
	}

	i2c_sleep_halfclock();
	return ret;
}

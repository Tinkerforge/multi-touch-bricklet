/* multi-touch-bricklet
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * multi-touch.h: Implementation of Multi Touch Bricklet messages
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

#ifndef MULTI_TOUCH_H
#define MULTI_TOUCH_H

#include <stdint.h>
#include "bricklib/com/com_common.h"

#define I2C_EEPROM_ADDRESS_HIGH 84

#define I2C_ADDRESS_MPR121 0x5A // 0b1011010
#define I2C_INTERNAL_ADDRESS_BYTES 1
#define I2C_DATA_LENGTH 1

#define I2C_READ  1
#define I2C_WRITE 0

//#define TOUCH_THRESHOLD   0x06
//#define RELEASE_THRESHOLD 0x0A
#define TOUCH_THRESHOLD              0x1F
#define RELEASE_THRESHOLD            0x1A
#define PROXIMITY_TOUCH_THRESHOLD    0x34
#define PROXIMITY_RELEASE_THRESHOLD  0x30

#define ELE07_T     0x00
#define ELE811_T    0x01
#define ELE07_OOR   0x02
#define ELE811_OOR  0x03
#define ELE0_F_LSB  0x04
#define ELE0_F_MSB  0x05
#define ELE1_F_LSB  0x06
#define ELE1_F_MSB  0x07
#define ELE2_F_LSB  0x08
#define ELE2_F_MSB  0x09
#define ELE3_F_LSB  0x0A
#define ELE3_F_MSB  0x0B
#define ELE4_F_LSB  0x0C
#define ELE4_F_MSB  0x0D
#define ELE5_F_LSB  0x0E
#define ELE5_F_MSB  0x0F
#define ELE6_F_LSB  0x10
#define ELE6_F_MSB  0x11
#define ELE7_F_LSB  0x12
#define ELE7_F_MSB  0x13
#define ELE8_F_LSB  0x14
#define ELE8_F_MSB  0x15
#define ELE9_F_LSB  0x16
#define ELE9_F_MSB  0x17
#define ELE10_F_LSB 0x18
#define ELE10_F_MSB 0x19
#define ELE11_F_LSB 0x1A
#define ELE11_F_MSB 0x1B
#define ELEPROX_LSB 0x1C
#define ELEPROX_MSB 0x1D
#define E0BV        0x1E
#define E1BV        0x1F
#define E2BV        0x20
#define E3BV        0x21
#define E4BV        0x22
#define E5BV        0x23
#define E6BV        0x24
#define E7BV        0x25
#define E8BV        0x26
#define E9BV        0x27
#define E10BV       0x28
#define E11BV       0x29
#define EPROXBV     0x2A
#define MHD_R	    0x2B
#define NHD_R	    0x2C
#define	NCL_R 	    0x2D
#define	FDL_R	    0x2E
#define	MHD_F	    0x2F
#define	NHD_F	    0x30
#define	NCL_F	    0x31
#define	FDL_F	    0x32
#define	PROX_MHD_R  0x36
#define	PROX_NHDA_R 0x37
#define	PROX_NCL_R  0x38
#define	PROX_FDL_R  0x39
#define	PROX_MHD_F  0x3A
#define	PROX_NHDA_F 0x3B
#define	PROX_NCL_F  0x3C
#define	PROX_NDL_F  0x3D
#define	PROX_NHDAT  0x3E
#define	PROX_NCLT   0x3F
#define	PROX_FDLT   0x40
#define	ELE0_T	    0x41
#define	ELE0_R	    0x42
#define	ELE1_T	    0x43
#define	ELE1_R	    0x44
#define	ELE2_T	    0x45
#define	ELE2_R	    0x46
#define	ELE3_T    	0x47
#define	ELE3_R	    0x48
#define	ELE4_T	    0x49
#define	ELE4_R	    0x4A
#define	ELE5_T	    0x4B
#define	ELE5_R	    0x4C
#define	ELE6_T	    0x4D
#define	ELE6_R	    0x4E
#define	ELE7_T	    0x4F
#define	ELE7_R	    0x50
#define	ELE8_T	    0x51
#define	ELE8_R	    0x52
#define	ELE9_T	    0x53
#define	ELE9_R	    0x54
#define	ELE10_T	    0x55
#define	ELE10_R	    0x56
#define	ELE11_T	    0x57
#define	ELE11_R    	0x58
#define PROX_T      0x59
#define PROX_R      0x5A
#define	DEBOUNCE    0x5B
#define	FIL_CFG_CDC	0x5C
#define	FIL_CFG_CDT	0x5D
#define	ELE_CFG	    0x5E
#define GPIO_CTRL0	0x73
#define	GPIO_CTRL1	0x74
#define GPIO_DATA	0x75
#define	GPIO_DIR	0x76
#define	GPIO_EN		0x77
#define	GPIO_SET	0x78
#define	GPIO_CLEAR	0x79
#define	GPIO_TOGGLE	0x7A
#define	ATO_CFG0	0x7B
#define	ATO_CFGU	0x7D
#define	ATO_CFGL	0x7E
#define	ATO_CFGT	0x7F
#define SOFT_RESET  0x80

#define ENDOFCONFIG 0xFF

#define FIL_CFG_CDC_FFI_00       (0b00 << 6)
#define FIL_CFG_CDC_FFI_01       (0b01 << 6)
#define FIL_CFG_CDC_FFI_10       (0b10 << 6)
#define FIL_CFG_CDC_FFI_11       (0b11 << 6)
#define FIL_CFG_CDC_CDC_ENC(x)   (x) // Charge Discharge Current

#define FIL_CFG_CDT_CDT(x)       ((x) << 5) // Charge Discharge Time
#define FIL_CFG_CDT_SFI(x)       ((x) << 3) // Second Filter Iterations
#define FIL_CFG_CDT_ESI(x)       (x) // Electrode Sample Interval

#define ELE_CFG_ENABLE_ALL       0x0C
#define ELE_CFG_ELEPROX_EN_00    (0b00 << 4)
#define ELE_CFG_ELEPROX_EN_01    (0b01 << 4)
#define ELE_CFG_ELEPROX_EN_10    (0b10 << 4)
#define ELE_CFG_ELEPROX_EN_11    (0b11 << 4)
#define ELE_CFG_CL_00            (0b00 << 6)
#define ELE_CFG_CL_01            (0b01 << 6)
#define ELE_CFG_CL_10            (0b10 << 6)
#define ELE_CFG_CL_11            (0b11 << 6)

#define I2C_HALF_CLOCK_400KHZ  1250  // 2500ns per clock

#define FID_GET_TOUCH_STATE       1
#define FID_RECALIBRATE           2
#define FID_SET_ELECTRODE_CONFIG  3
#define FID_GET_ELECTRODE_CONFIG  4
#define FID_TOUCH_STATE           5

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) StandardMessage;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetTouchState;

typedef struct {
	MessageHeader header;
	uint16_t state;
} __attribute__((__packed__)) GetTouchStateReturn;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) Recalibrate;

typedef struct {
	MessageHeader header;
	uint16_t enabled_electrodes;
} __attribute__((__packed__)) SetElectrodeConfig;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetElectrodeConfig;

typedef struct {
	MessageHeader header;
	uint16_t enabled_electrodes;
} __attribute__((__packed__)) GetElectrodeConfigReturn;

typedef struct {
	MessageHeader header;
	uint16_t state;
} __attribute__((__packed__)) TouchState;

void get_touch_state(const ComType com, const GetTouchState *data);
void recalibrate(const ComType com, const Recalibrate *data);
void set_electrode_config(const ComType com, const SetElectrodeConfig *data);
void get_electrode_config(const ComType com, const GetElectrodeConfig *data);

void mpr121_reset(void);
void mpr121_disable(void);
void mpr121_enable(void);
void mpr121_configure(void);
void read_registers(const uint8_t reg, uint8_t *data, const uint8_t length);
void write_register(const uint8_t reg, uint8_t value);

void i2c_scl_high(void);
void i2c_scl_low(void);
bool i2c_sda_value(void);
void i2c_sda_high(void);
void i2c_sda_low(void);
void i2c_sleep_halfclock(void);
void i2c_stop(void);
void i2c_start(void);
uint8_t i2c_recv_byte(bool ack);
bool i2c_send_byte(uint8_t value);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif

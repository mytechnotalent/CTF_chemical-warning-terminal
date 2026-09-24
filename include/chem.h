// MIT License
//
// Copyright (c) 2026 Kevin Thomas
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Author:  Kevin Thomas
// Email:   kevin@mytechnotalent.com
// GitHub:  https://github.com/mytechnotalent/chemical-warning-terminal
// File:    chem.h
// Desc:    Declares platform pin mapping, peripheral handles, and
//          provisioning boundaries for the IRON CURTAIN chemical storage
//          warning terminal.
// Created: 2026

#ifndef CHEM_H
#define CHEM_H

#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "packet_artifact.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Onboard heartbeat LED GPIO pin number.
 */
#define CHEM_LED_PIN 25u

/**
 * @brief DHT11 chemical store temperature sensor GPIO pin number.
 */
#define CHEM_DHT_PIN 4u

/**
 * @brief I2C peripheral used by the 1602 LCD backpack.
 */
#define CHEM_I2C i2c1

/**
 * @brief I2C SDA GPIO pin number.
 */
#define CHEM_I2C_SDA 2u

/**
 * @brief I2C SCL GPIO pin number.
 */
#define CHEM_I2C_SCL 3u

/**
 * @brief I2C bus clock rate in hertz.
 */
#define CHEM_I2C_BAUD 100000u

/**
 * @brief I2C address of the 1602 LCD PCF8574 backpack.
 */
#define CHEM_LCD_ADDR PACKET_LCD_I2C_ADDRESS

/**
 * @brief UART peripheral used by the RYLR998 safety link.
 */
#define CHEM_UART uart1

/**
 * @brief UART TX GPIO pin number to the RYLR998 RX input.
 */
#define CHEM_UART_TX 8u

/**
 * @brief UART RX GPIO pin number from the RYLR998 TX output.
 */
#define CHEM_UART_RX 9u

/**
 * @brief UART baud rate negotiated with the RYLR998.
 */
#define CHEM_UART_BAUD 115200u

/**
 * @brief RYLR998 network identifier shared by all classroom radios.
 */
#define CHEM_NETWORK_ID 18u

/**
 * @brief Fixed warning frame size in bytes.
 */
#define CHEM_FRAME_SIZE PACKET_FRAME_SIZE

/**
 * @brief Time to wait for a sealed hazard command before failing safe.
 */
#define CHEM_LINK_WAIT_MS PACKET_LINK_WAIT_MS

/**
 * @brief Servo pulse width in microseconds that raises the hazard siren.
 */
#define CHEM_RAISE_PULSE_US PACKET_SIREN_RAISE_PULSE_US

/**
 * @brief Servo pulse width in microseconds that lowers the hazard siren.
 */
#define CHEM_LOWER_PULSE_US PACKET_SIREN_LOWER_PULSE_US

/**
 * @brief Red HAZARD warning lamp GPIO pin number.
 */
#define CHEM_RED_LED_PIN 16u

/**
 * @brief Yellow WATCH warning lamp GPIO pin number.
 */
#define CHEM_YELLOW_LED_PIN 17u

/**
 * @brief Green CLEAR warning lamp GPIO pin number.
 */
#define CHEM_GREEN_LED_PIN 18u

/**
 * @brief Acknowledge push-button GPIO pin number.
 */
#define CHEM_BUTTON_PIN 15u

/**
 * @brief Siren and vent actuator servo PWM GPIO pin number.
 */
#define CHEM_SERVO_PIN 14u

/**
 * @brief Infrared maintenance receiver GPIO pin number.
 */
#define CHEM_IR_PIN 5u

/**
 * @brief Lowest acceptable chemical store temperature in tenths of a degree.
 */
#define CHEM_TEMP_MIN_TENTHS 0

/**
 * @brief Highest acceptable chemical store temperature in tenths of a degree.
 */
#define CHEM_TEMP_MAX_TENTHS 400

/**
 * @brief Lowest accepted chemical storage zone identifier.
 */
#define CHEM_ZONE_MIN 0

/**
 * @brief Highest accepted chemical storage zone identifier.
 */
#define CHEM_ZONE_MAX 16

/**
 * @brief Provisioned warning terminal node identifier.
 */
#define CHEM_NODE_ID PACKET_NODE_ADDRESS

/**
 * @brief Provisioned warning control gateway LoRa address.
 */
#define CHEM_GATEWAY_ADDRESS PACKET_GATEWAY_ADDRESS

#endif // CHEM_H

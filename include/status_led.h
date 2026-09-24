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
// File:    status_led.h
// Desc:    Declares the red, yellow, and green chemical hazard status
//          annunciator light.
// Created: 2026

#ifndef STATUS_LED_H
#define STATUS_LED_H

#include "chem.h"
#include <stdbool.h>

/**
 * @brief Tri-color chemical hazard annunciator states.
 */
typedef enum chem_led_state {
    /**
     * @brief All annunciator lamps dark.
     */
    CHEM_LED_OFF = 0,
    /**
     * @brief Red HAZARD lamp lit for a chemical release warning.
     */
    CHEM_HAZARD = 1,
    /**
     * @brief Yellow WATCH lamp lit while a warning is pending.
     */
    CHEM_WATCH = 2,
    /**
     * @brief Green CLEAR lamp lit while the store is safe.
     */
    CHEM_CLEAR = 3,
} chem_led_state_t;

/**
 * @brief Initialize the tri-color chemical hazard annunciator GPIO pins.
 *
 * @param void No parameters.
 * @return bool true when initialization completed.
 */
bool status_led_init(void);

/**
 * @brief Drive exactly one annunciator lamp for a hazard state.
 *
 * @param state Desired annunciator state.
 * @return void
 */
void status_led_show(chem_led_state_t state);

#endif // STATUS_LED_H

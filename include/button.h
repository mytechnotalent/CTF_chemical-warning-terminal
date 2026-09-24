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
// File:    button.h
// Desc:    Declares the debounced hazard acknowledge push-button input.
// Created: 2026

#ifndef BUTTON_H
#define BUTTON_H

#include "chem.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Contact bounce lockout window in microseconds.
 */
#define CHEM_ACK_DEBOUNCE_US 30000u

/**
 * @brief Initialize the hazard acknowledge push-button input.
 *
 * @param void No parameters.
 * @return bool true when initialization completed.
 */
bool ack_init(void);

/**
 * @brief Report whether the acknowledge button is held down.
 *
 * @param void No parameters.
 * @return bool true while the pin reads low (pressed).
 */
bool ack_pressed(void);

/**
 * @brief Consume one debounced acknowledge press edge.
 *
 * The acknowledge raises a pending indication but does NOT bypass
 * authorization, so a press alone never changes the guarded hazard state
 * and never silences the siren without an authorized sealed command.
 *
 * @param void No parameters.
 * @return bool true when a new press edge was consumed.
 */
bool ack_consume_press(void);

/**
 * @brief Clear the debounce state.
 *
 * @param void No parameters.
 * @return void
 */
void ack_reset(void);

#endif // BUTTON_H

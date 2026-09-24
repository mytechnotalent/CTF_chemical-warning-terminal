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
// File:    siren.h
// Desc:    Declares the chemical hazard siren state machine that sequences
//          the SG90 actuator and fails safe to the raised hazard posture.
// Created: 2026

#ifndef SIREN_H
#define SIREN_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Bounded siren travel time in milliseconds.
 */
#define CHEM_SIREN_TRAVEL_MS 1000u

/**
 * @brief Hazard siren position and health states.
 */
typedef enum chem_siren_state {
    /**
     * @brief Siren is lowered and the store is silent.
     */
    CHEM_SIREN_LOWERED = 0,
    /**
     * @brief Siren is raised and sounding the hazard warning.
     */
    CHEM_SIREN_RAISED = 1,
    /**
     * @brief Siren has failed safe into the raised hazard posture.
     */
    CHEM_SIREN_FAULT = 2,
    /**
     * @brief Siren actuator is travelling between positions.
     */
    CHEM_SIREN_MOVING = 3,
} chem_siren_state_t;

/**
 * @brief Initialize the siren state machine and raise the hazard siren.
 *
 * @param void No parameters.
 * @return void
 */
void siren_init(void);

/**
 * @brief Return the current siren state.
 *
 * @param void No parameters.
 * @return chem_siren_state_t Current siren state.
 */
chem_siren_state_t siren_state(void);

/**
 * @brief Report whether the hazard siren is currently fully raised.
 *
 * @param void No parameters.
 * @return bool true when the siren is raised.
 */
bool siren_is_raised(void);

/**
 * @brief Apply an authorized raise or lower command to the siren.
 *
 * Unauthorized commands are refused. An authorized command starts a
 * bounded travel interval that siren_tick completes. This is the guarded
 * command path that prevents an unauthenticated local press from moving
 * the siren.
 *
 * @param raise True to drive the siren up, false to lower it.
 * @param authorized True when the caller has validated the command.
 * @return void
 */
void siren_apply_command(bool raise, bool authorized);

/**
 * @brief Advance the siren state machine by one tick.
 *
 * Completes a pending travel once the bounded interval has elapsed.
 *
 * @param void No parameters.
 * @return void
 */
void siren_tick(void);

/**
 * @brief Force the siren up and record the fault.
 *
 * This is the fail-safe posture taken when the safety link is lost or a
 * hazard frame cannot be authorized.
 *
 * @param void No parameters.
 * @return void
 */
void siren_fail_safe(void);

#endif // SIREN_H

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
// File:    siren.c
// Desc:    Implements the chemical hazard siren state machine that
//          sequences the SG90 actuator and fails safe to the raised
//          hazard posture.
// Created: 2026

#include "pico/time.h"
#include "siren.h"
#include "servo.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Current siren position and health state.
 */
static chem_siren_state_t g_siren_state;

/**
 * @brief Pending travel target, true when the siren is rising.
 */
static bool g_siren_target_raised;

/**
 * @brief Absolute time in microseconds when the pending travel completes.
 */
static uint64_t g_siren_move_until_us;

/**
 * @brief Complete a pending travel by driving the siren actuator.
 *
 * @param void No parameters.
 * @return void
 */
static void siren_complete(void) {
    if (g_siren_target_raised) {
        siren_raise();
        g_siren_state = CHEM_SIREN_RAISED;
        return;
    }
    siren_lower();
    g_siren_state = CHEM_SIREN_LOWERED;
}

void siren_init(void) {
    g_siren_target_raised = true;
    g_siren_state = CHEM_SIREN_RAISED;
    siren_raise();
}

chem_siren_state_t siren_state(void) {
    return g_siren_state;
}

bool siren_is_raised(void) {
    return g_siren_state == CHEM_SIREN_RAISED;
}

void siren_apply_command(bool raise, bool authorized) {
    if (!authorized) {
        return;
    }
    g_siren_target_raised = raise;
    g_siren_state = CHEM_SIREN_MOVING;
    g_siren_move_until_us =
        time_us_64() + (uint64_t)CHEM_SIREN_TRAVEL_MS * 1000u;
}

void siren_tick(void) {
    if (g_siren_state != CHEM_SIREN_MOVING) {
        return;
    }
    if (time_us_64() < g_siren_move_until_us) {
        return;
    }
    siren_complete();
}

void siren_fail_safe(void) {
    siren_raise();
    g_siren_target_raised = true;
    g_siren_state = CHEM_SIREN_FAULT;
}

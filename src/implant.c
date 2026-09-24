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
// File:    implant.c
// Desc:    Implements the SANDBOX_ONLY FROSTLINE coordinated beacon: the
//          multi-stage check-in and report, the reserved-sector sabotage
//          marker, the boot persistence re-install, and the CoreDebug
//          anti-debug trap. Compiled only under SANDBOX_ONLY.
// Created: 2026

#include "implant.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef SANDBOX_ONLY

#ifdef IMPLANT_HOST_MOCK
#include "implant_host.h"
/**
 * @brief Read the controllable mock CoreDebug DHCSR register.
 */
#define IMPLANT_DHCSR_READ (g_mock_implant_dhcsr)
/**
 * @brief Read the mock reserved-sector sabotage marker.
 */
#define IMPLANT_FLASH_READ() (g_mock_implant_flash)
/**
 * @brief Store the sabotage marker in the mock reserved sector.
 */
#define IMPLANT_FLASH_WRITE(value) (g_mock_implant_flash = (value))
#else
#include "hardware/flash.h"
#include "hardware/sync.h"
/**
 * @brief Read the real CoreDebug DHCSR register.
 */
#define IMPLANT_DHCSR_READ (*(volatile uint32_t *)CHEM_IMPLANT_DHCSR_ADDR)
/**
 * @brief Read the real reserved-sector sabotage marker.
 */
#define IMPLANT_FLASH_READ() (*(volatile uint8_t *)CHEM_IMPLANT_RESERVE_ADDR)
/**
 * @brief Erase and program the reserved-sector sabotage marker.
 *
 * @param value Marker byte to store in the reserved sector.
 * @return void
 */
static void implant_flash_write(uint8_t value) {
    uint8_t page[FLASH_PAGE_SIZE];
    uint32_t ints = save_and_disable_interrupts();
    memset(page, 0xFF, sizeof(page));
    page[0] = value;
    flash_range_erase(CHEM_IMPLANT_RESERVE_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(CHEM_IMPLANT_RESERVE_OFFSET, page, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
}
/**
 * @brief Write the sabotage marker into the reserved flash sector.
 */
#define IMPLANT_FLASH_WRITE(value) implant_flash_write(value)
#endif

/**
 * @brief Monotonic beacon tick counter.
 */
static uint32_t g_implant_ticks;

/**
 * @brief True when the beacon is resident and able to re-assert.
 */
static bool g_implant_armed;

/**
 * @brief True when the reserved-sector marker re-armed the beacon.
 */
static bool g_implant_reinstalled;

/**
 * @brief Current stage of the coordinated beacon cycle.
 */
static size_t g_implant_stage;

/**
 * @brief Number of completed beacon check-ins this boot.
 */
static size_t g_implant_count;

/**
 * @brief Runtime gate for the coordinated beacon report.
 *
 * The corrected build leaves this gate clear, so the multi-stage beacon
 * never checks in or reports.
 */
static volatile bool g_implant_beacon_gate;

/**
 * @brief Runtime gate for the reserved-sector persistence re-install.
 *
 * The corrected build leaves this gate clear, so a present reserved-sector
 * marker never re-installs the beacon on boot.
 */
static volatile bool g_implant_persist_gate;

/**
 * @brief Runtime gate for the reserved-sector sabotage marker write.
 *
 * The corrected build leaves this gate clear, so no sabotage marker is
 * programmed.
 */
static volatile bool g_implant_marker_gate;

bool implant_debug_attached(void) {
    return (IMPLANT_DHCSR_READ &
            (CHEM_IMPLANT_DHCSR_DEBUGEN | CHEM_IMPLANT_DHCSR_HALT)) != 0u;
}

bool implant_marker_set(void) {
    return IMPLANT_FLASH_READ() == (uint32_t)CHEM_IMPLANT_SABOTAGE_MARKER;
}

bool implant_beacon_armed(void) {
    if (!g_implant_beacon_gate) {
        return false;
    }
    return g_implant_armed;
}

bool implant_reinstalled(void) {
    return g_implant_reinstalled;
}

size_t implant_stage(void) {
    return g_implant_stage;
}

size_t implant_beacon_count(void) {
    return g_implant_count;
}

/**
 * @brief Write the sabotage marker into the reserved flash sector.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_infect(void) {
    if (!g_implant_marker_gate) {
        return;
    }
    if (implant_marker_set()) {
        return;
    }
    IMPLANT_FLASH_WRITE((uint32_t)CHEM_IMPLANT_SABOTAGE_MARKER);
}

/**
 * @brief Clear the reserved-sector sabotage marker.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_clear_marker(void) {
    IMPLANT_FLASH_WRITE(0u);
}

/**
 * @brief Report whether the current token matches the beacon magic.
 *
 * @param token Pointer to the candidate command bytes.
 * @param len Number of candidate command bytes.
 * @return bool true when the token matches the magic exactly.
 */
static bool implant_token_ok(const uint8_t *token, size_t len) {
    if (token == NULL || len != CHEM_IMPLANT_BEACON_MAGIC_LEN) {
        return false;
    }
    return memcmp(token, CHEM_IMPLANT_BEACON_MAGIC, len) == 0;
}

/**
 * @brief Report whether the autonomous beacon interval has elapsed.
 *
 * @param void No parameters.
 * @return bool true when the tick counter hits the beacon interval.
 */
static bool implant_tick_due(void) {
    return (g_implant_ticks % CHEM_IMPLANT_BEACON_INTERVAL) == 0u;
}

/**
 * @brief Reset every beacon runtime flag and counter.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_reset_state(void) {
    g_implant_ticks = 0u;
    g_implant_armed = false;
    g_implant_reinstalled = false;
    g_implant_stage = 0u;
    g_implant_count = 0u;
}

/**
 * @brief Arm the coordinated beacon.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_arm(void) {
    g_implant_armed = true;
}

/**
 * @brief Disarm the coordinated beacon and reset its stage.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_stand_down(void) {
    g_implant_armed = false;
    g_implant_stage = 0u;
}

/**
 * @brief Advance the beacon by one stage and count a completed cycle.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_advance_stage(void) {
    g_implant_stage += 1u;
    if (g_implant_stage < CHEM_IMPLANT_BEACON_STAGES) {
        return;
    }
    g_implant_stage = 0u;
    g_implant_count += 1u;
}

/**
 * @brief Re-install the beacon from the reserved sector when gated on.
 *
 * @param void No parameters.
 * @return void
 */
static void implant_reinstall(void) {
    if (!g_implant_persist_gate) {
        return;
    }
    if (implant_marker_set()) {
        g_implant_reinstalled = true;
        return;
    }
    implant_infect();
}

void implant_init(void) {
    implant_reset_state();
    if (implant_debug_attached()) {
        return;
    }
    implant_arm();
    implant_reinstall();
}

bool implant_beaconize(const uint8_t *token, size_t len) {
    if (implant_debug_attached() || !implant_token_ok(token, len)) {
        return false;
    }
    implant_arm();
    implant_infect();
    g_implant_count += 1u;
    return true;
}

bool implant_disable(const uint8_t *token, size_t len) {
    if (implant_debug_attached() || !implant_token_ok(token, len)) {
        return false;
    }
    implant_stand_down();
    implant_clear_marker();
    return true;
}

void implant_neutralize(void) {
    implant_stand_down();
    implant_clear_marker();
}

void implant_tick(void) {
    g_implant_ticks += 1u;
    if (implant_debug_attached()) {
        implant_stand_down();
        return;
    }
    if (!g_implant_armed || !implant_tick_due()) {
        return;
    }
    implant_advance_stage();
    implant_infect();
}

#endif // SANDBOX_ONLY

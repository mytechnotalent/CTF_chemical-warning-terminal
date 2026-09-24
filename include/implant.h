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
// File:    implant.h
// Desc:    Declares the SANDBOX_ONLY FROSTLINE coordinated beacon: the
//          multi-stage check-in and report, the reserved-sector sabotage
//          marker, the boot persistence re-install, and the CoreDebug
//          anti-debug trap. Compiled only under SANDBOX_ONLY.
// Created: 2026

#ifndef IMPLANT_H
#define IMPLANT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Magic command that arms the coordinated beacon.
 *
 * The beacon wakes only when this exact token is presented to
 * implant_beaconize. Anything else leaves the terminal disarmed.
 */
#define CHEM_IMPLANT_BEACON_MAGIC "IRON-CURTAIN-BEACON-2026"

/**
 * @brief Length in bytes of the magic beacon command token.
 */
#define CHEM_IMPLANT_BEACON_MAGIC_LEN 24u

/**
 * @brief Sabotage marker byte written into the reserved flash sector.
 */
#define CHEM_IMPLANT_SABOTAGE_MARKER 0x58u

/**
 * @brief Offset of the reserved flash sector used by the sabotage marker.
 *
 * The final 4 KiB sector of the 4 MiB flash, well beyond the firmware.
 */
#define CHEM_IMPLANT_RESERVE_OFFSET 0x3FF000u

/**
 * @brief Reserved flash sector address used by the sabotage marker.
 */
#define CHEM_IMPLANT_RESERVE_ADDR 0x103FF000u

/**
 * @brief CoreDebug DHCSR register address used by the anti-debug trap.
 */
#define CHEM_IMPLANT_DHCSR_ADDR 0xE000EDF0u

/**
 * @brief CoreDebug DHCSR bit that reports an enabled debugger.
 */
#define CHEM_IMPLANT_DHCSR_DEBUGEN 0x00000001u

/**
 * @brief CoreDebug DHCSR bit that reports a halted core.
 */
#define CHEM_IMPLANT_DHCSR_HALT 0x00000002u

/**
 * @brief Number of ticks between autonomous beacon check-ins.
 */
#define CHEM_IMPLANT_BEACON_INTERVAL 4u

/**
 * @brief Number of stages in one coordinated beacon check-in cycle.
 */
#define CHEM_IMPLANT_BEACON_STAGES 3u

/**
 * @brief Initialize the coordinated beacon and re-install on boot.
 *
 * On first run the beacon writes its sabotage marker into the reserved
 * flash sector. On every later boot the marker is present, so the beacon
 * re-installs from the reserved sector without any firmware change. The
 * beacon stays benign while a debug probe is attached.
 *
 * @param void No parameters.
 * @return void
 */
void implant_init(void);

/**
 * @brief Advance the coordinated beacon by one tick.
 *
 * @param void No parameters.
 * @return void
 */
void implant_tick(void);

/**
 * @brief Report whether the coordinated beacon is resident.
 *
 * @param void No parameters.
 * @return bool true when the beacon is armed.
 */
bool implant_beacon_armed(void);

/**
 * @brief Report whether this boot re-installed from the reserved sector.
 *
 * @param void No parameters.
 * @return bool true when the reserved-sector marker re-armed the beacon.
 */
bool implant_reinstalled(void);

/**
 * @brief Report whether a debug probe is attached via CoreDebug DHCSR.
 *
 * @param void No parameters.
 * @return bool true when C_DEBUGEN or C_HALT is set.
 */
bool implant_debug_attached(void);

/**
 * @brief Report whether the reserved-sector sabotage marker is set.
 *
 * @param void No parameters.
 * @return bool true when the sabotage marker occupies the reserved sector.
 */
bool implant_marker_set(void);

/**
 * @brief Arm the coordinated beacon only when the magic command matches.
 *
 * A mismatched token, a null pointer, or an attached debug probe leaves
 * the terminal disarmed. On success the sabotage marker is persisted so
 * the beacon re-installs on the next boot.
 *
 * @param token Pointer to the candidate magic command bytes.
 * @param len Number of candidate command bytes.
 * @return bool true when the beacon armed.
 */
bool implant_beaconize(const uint8_t *token, size_t len);

/**
 * @brief Disarm the coordinated beacon only when the magic command matches.
 *
 * A mismatched token, a null pointer, or an attached debug probe leaves
 * the beacon armed. On a successful disarm the sabotage marker is
 * cleared so the beacon is not re-installed on the next boot.
 *
 * @param token Pointer to the candidate release command bytes.
 * @param len Number of candidate command bytes.
 * @return bool true when the beacon was disarmed.
 */
bool implant_disable(const uint8_t *token, size_t len);

/**
 * @brief Disarm the beacon and clear the reserved-sector marker.
 *
 * @param void No parameters.
 * @return void
 */
void implant_neutralize(void);

/**
 * @brief Return the current stage of the coordinated beacon cycle.
 *
 * @param void No parameters.
 * @return size_t Zero-based beacon stage.
 */
size_t implant_stage(void);

/**
 * @brief Return the number of completed beacon check-ins this boot.
 *
 * @param void No parameters.
 * @return size_t Number of completed check-in cycles.
 */
size_t implant_beacon_count(void);

#endif // IMPLANT_H

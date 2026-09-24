# chemical-warning-terminal - Design Blueprint (Act X, IRON CURTAIN)

Repo: `chemical-warning-terminal`
Companion CTF repo: `CTF_chemical-warning-terminal` (artifact prefix `ACT-X`)
Codename: IRON CURTAIN
Author: Kevin Thomas (kevin@mytechnotalent.com)

## Act X of the OPERATION COLD IRON saga (the finale)

Act I the lie. Act II the door. Act III the payload. Act IV the payload that
would not die. Act V the payload that spreads. Act VI the payload that steals.
Act VII the payload that takes orders. Act VIII the payload that holds the
building hostage. Act IX the payload that becomes a weapon. Act X is the payload
that does all of it at once.

The chemical storage warning terminal is the last node before the evacuation
siren. FROSTLINE's finale implant combines every technique: a coordinated
beacon, persistence, and a sabotage marker, so the terminal lies about a
chemical hazard while the Ministry's campaign executes. WHITEOUT must run a
full incident response and restore the terminal. This is the coordinated-attack
finale.

## Safety contract

- No network, no internet, no host impact. Bare-metal RP2350, no OS.
- All effects are confined to GPIO: the hazard LEDs, the LCD, the siren servo.
- Synthetic data only. No external address.
- A `SANDBOX_ONLY` build guard disables the implant.
- The finale ends in analysis and neutralization.

## Parity contract

Same layout, crypto, tooling, pin map, README standard, disclaimer, and REAL
flash persistence (0x103FF000) as Acts I-IX.

## Pin map (identical, new roles)

| Pin | Act X role |
| --- | ---------- |
| DHT11 GP4 | chemical store temperature |
| LCD SDA GP2 / SCL GP3 | hazard warning |
| IR GP5 | local maintenance remote |
| Servo GP14 | siren/vent actuator |
| Red GP16 | HAZARD |
| Yellow GP17 | WATCH |
| Green GP18 | CLEAR |
| Button GP15 | acknowledge |
| RYLR998 GP8/9 | safety link |
| Debug Probe | incident response |
| Onboard GP25 | heartbeat |

## Fix track

- Hazard commands must be sealed and authorized.
- Acknowledge must not silently bypass authorization.
- The terminal must fail to the safe hazard state.

## Malware track (coordinated finale, benign)

Module `include/implant.h` + `src/implant.c`, only under `SANDBOX_ONLY`:

- **Coordinated beacon.** A multi-stage beacon that checks in and reports.
- **Persistence.** Re-install from the reserved sector on boot.
- **Sabotage marker.** Program a sabotage marker into the reserved flash sector
  (`CHEM_IMPLANT_RESERVE_ADDR` 0x103FF000) with the real flash API.
- **Anti-debug.** Reads DHCSR and behaves benignly under a probe.
- **Neutralization.** Full incident response: disable the beacon, break the
  persistence, clear the marker, restore the hazard state.

## Companion CTF: ACT-X, four deep tasks

| Task | Points | Objective |
| ---- | ------ | --------- |
| 1 | 10 | Setup and analysis |
| 2 | 20 | Cut the coordinated beacon |
| 3 | 20 | Break the persistence |
| 4 | 20 | Clear the sabotage marker |
| 5 | 20 | Seal the hazard command path (fix track) |
| 6 | 10 | Export, verify, hardware proof, reflection |

Every patch is in-place and same-size.

## Naming

Project `chemical-warning-terminal`; companion
`CTF_chemical-warning-terminal`; prefix `ACT-X`. The CTF's Next link points to
the post-ten backbone, https://github.com/mytechnotalent/telescreen.

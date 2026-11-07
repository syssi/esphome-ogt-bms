#pragma once
#include "common.h"

namespace esphome::ogt_bms_ble::testing {

// ── Real frames from a TAB SmartBat 100Ah (4S LiFePO4, SmartBat-B22704) ──────
// Source: https://github.com/syssi/esphome-ogt-bms/issues/86
//
// Encryption key for SmartBat-B22704:
//   hex_id = f"{int('22704'):04X}" = "58B0"
//   crypt_seq = [2,5,4,3,1,4,1,6,8,3,7,2,5,8,9,3]
//   key_sum = crypt_seq[5]+crypt_seq[8]+crypt_seq[0xB]+crypt_seq[0] = 4+8+2+2 = 16
//   key = 16 + 8 (Type B offset) = 24 = 0x18
// Cross-check: the captured request 33.4A.29.2E.2B.5E.28.2A decrypts to "+R163F02".
static const uint8_t TYPE_B_ENCRYPTION_KEY = 0x18;

// +RD,08080B  → cmd=0x08 (temperature), LE=0x0B08=2824 → 282.4K → 9.25°C
static const std::vector<uint8_t> TYPE_B_TEMPERATURE = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x20,
                                                        0x28, 0x20, 0x28, 0x5A, 0x15, 0x12};

// +RD,091034  → cmd=0x09 (total voltage), LE=0x3410=13328 → 13.328V
static const std::vector<uint8_t> TYPE_B_TOTAL_VOLTAGE = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x21,
                                                          0x29, 0x28, 0x2B, 0x2C, 0x15, 0x12};

// +RD,0ABCFE01  → cmd=0x0A (current), LE=0xFEBC=-324, scale=0x01 → -0.324A
static const std::vector<uint8_t> TYPE_B_CURRENT = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x59, 0x5A,
                                                    0x5B, 0x5E, 0x5D, 0x28, 0x29, 0x15, 0x12};

// +RD,0D64  → cmd=0x0D (state of charge), 0x64=100 → 100%
static const std::vector<uint8_t> TYPE_B_STATE_OF_CHARGE = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x5C, 0x2E, 0x2C, 0x15, 0x12};

// +RD,0F3F280A  → cmd=0x0F (remaining capacity), LE=0x283F=10303, scale=0x0A → 103.03Ah
static const std::vector<uint8_t> TYPE_B_CAPACITY_REMAINING = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x5E, 0x2B,
                                                               0x5E, 0x2A, 0x20, 0x28, 0x59, 0x15, 0x12};

// +RD,1088280A  → cmd=0x10 (full charge capacity), LE=0x2888=10376, scale=0x0A → 103.76Ah
static const std::vector<uint8_t> TYPE_B_FULL_CHARGE_CAPACITY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x28, 0x20,
                                                                 0x20, 0x2A, 0x20, 0x28, 0x59, 0x15, 0x12};

// +RD,123246  → cmd=0x12 (time to empty), LE=0x4632=17970 min → 1078200 sec
static const std::vector<uint8_t> TYPE_B_TIME_TO_EMPTY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2A,
                                                          0x2B, 0x2A, 0x2C, 0x2E, 0x15, 0x12};

// +RD,13FFFF  → cmd=0x13 (time to full), LE=0xFFFF → not available (NaN)
static const std::vector<uint8_t> TYPE_B_TIME_TO_FULL = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2B,
                                                         0x5E, 0x5E, 0x5E, 0x5E, 0x15, 0x12};

// +RD,171400  → cmd=0x17 (cycles), LE=0x0014=20 → 20 cycles
static const std::vector<uint8_t> TYPE_B_CYCLES = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2F,
                                                   0x29, 0x2C, 0x28, 0x28, 0x15, 0x12};

// +RD,1810270A  → cmd=0x18 (design capacity), LE=0x2710=10000, scale=0x0A → 100.0Ah
static const std::vector<uint8_t> TYPE_B_DESIGN_CAPACITY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x20, 0x29,
                                                            0x28, 0x2A, 0x2F, 0x28, 0x59, 0x15, 0x12};

// Cell voltages: cmd=0x3F..0x30, cell_index = 63 - cmd, voltage = LE * 0.001V
// Sum of cells 1-4 = 13.333V, matches the total voltage of 13.328V.
// +RD,3F010D  → cmd=0x3F=63 (cell 1), LE=0x0D01=3329 → 3.329V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_01 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5E,
                                                            0x28, 0x29, 0x28, 0x5C, 0x15, 0x12};
// +RD,3E070D  → cmd=0x3E=62 (cell 2), LE=0x0D07=3335 → 3.335V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_02 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5D,
                                                            0x28, 0x2F, 0x28, 0x5C, 0x15, 0x12};
// +RD,3D050D  → cmd=0x3D=61 (cell 3), LE=0x0D05=3333 → 3.333V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_03 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5C,
                                                            0x28, 0x2D, 0x28, 0x5C, 0x15, 0x12};
// +RD,3C080D  → cmd=0x3C=60 (cell 4), LE=0x0D08=3336 → 3.336V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_04 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5B,
                                                            0x28, 0x20, 0x28, 0x5C, 0x15, 0x12};
// +RD,3B0000  → cmd=0x3B=59 (cell 5), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_05 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5A,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,3A0000  → cmd=0x3A=58 (cell 6), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_06 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x59,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,300000  → cmd=0x30=48 (cell 16), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_16 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x28,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};

// ── Invalid frames ────────────────────────────────────────────────────────────

// 2 bytes: even length but decrypts to "+R" (missing "D," prefix) → rejected
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_TOO_SHORT = {0x33, 0x4A};

// 3 bytes: odd length → rejected at size check
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_ODD_LENGTH = {0x33, 0x4A, 0x5C};

}  // namespace esphome::ogt_bms_ble::testing

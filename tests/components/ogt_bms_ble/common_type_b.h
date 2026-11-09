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
// +RD,390000  → cmd=0x39=57 (cell 7), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_07 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x21,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,380000  → cmd=0x38=56 (cell 8), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_08 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x20,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,370000  → cmd=0x37=55 (cell 9), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_09 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2F,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,360000  → cmd=0x36=54 (cell 10), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_10 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2E,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,350000  → cmd=0x35=53 (cell 11), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_11 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2D,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,340000  → cmd=0x34=52 (cell 12), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_12 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2C,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,330000  → cmd=0x33=51 (cell 13), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_13 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2B,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,320000  → cmd=0x32=50 (cell 14), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_14 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2A,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,310000  → cmd=0x31=49 (cell 15), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_15 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x29,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};
// +RD,300000  → cmd=0x30=48 (cell 16), not present on a 4S pack → 0.000V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_16 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x28,
                                                            0x28, 0x28, 0x28, 0x28, 0x15, 0x12};

// ── Second real update cycle (same device, SmartBat-B22704) ─────────────────
// Source: docs/traffic-type-b.txt, first complete cycle (frames 1-26).
// Discharging at a lower SoC than the issue #86 snapshot. Cells 5-16 report
// 0x0000 and are byte-identical to TYPE_B_CELL_VOLTAGE_05..16 above.
// Sum of cells 1-4 = 13.259V, matches the total voltage of 13.256V.
// +RD,08FD0A  → cmd=0x08 (temperature), LE=0x0AFD=2813 → 281.3K → 8.15°C
static const std::vector<uint8_t> TYPE_B_CAPTURE2_TEMPERATURE = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x20,
                                                                 0x5E, 0x5C, 0x28, 0x59, 0x15, 0x12};
// +RD,09C833  → cmd=0x09 (total voltage), LE=0x33C8=13256 → 13.256V
static const std::vector<uint8_t> TYPE_B_CAPTURE2_TOTAL_VOLTAGE = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x21,
                                                                   0x5B, 0x20, 0x2B, 0x2B, 0x15, 0x12};
// +RD,0A4AFD01  → cmd=0x0A (current), LE=0xFD4A=-694, scale=0x01 → -0.694A
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CURRENT = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x59, 0x2C,
                                                             0x59, 0x5E, 0x5C, 0x28, 0x29, 0x15, 0x12};
// +RD,0D63  → cmd=0x0D (state of charge), 0x63=99 → 99%
static const std::vector<uint8_t> TYPE_B_CAPTURE2_STATE_OF_CHARGE = {0x33, 0x4A, 0x5C, 0x34, 0x28,
                                                                     0x5C, 0x2E, 0x2B, 0x15, 0x12};
// +RD,0F29280A  → cmd=0x0F (remaining capacity), LE=0x2829=10281, scale=0x0A → 102.81Ah
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CAPACITY_REMAINING = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x5E, 0x2A,
                                                                        0x21, 0x2A, 0x20, 0x28, 0x59, 0x15, 0x12};
// +RD,1088280A  → cmd=0x10 (full charge capacity), LE=0x2888=10376, scale=0x0A → 103.76Ah
static const std::vector<uint8_t> TYPE_B_CAPTURE2_FULL_CHARGE_CAPACITY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x28, 0x20,
                                                                          0x20, 0x2A, 0x20, 0x28, 0x59, 0x15, 0x12};
// +RD,12DF1F  → cmd=0x12 (time to empty), LE=0x1FDF=8159 min → 489540 sec
static const std::vector<uint8_t> TYPE_B_CAPTURE2_TIME_TO_EMPTY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2A,
                                                                   0x5C, 0x5E, 0x29, 0x5E, 0x15, 0x12};
// +RD,13FFFF  → cmd=0x13 (time to full), LE=0xFFFF → not available (NaN)
static const std::vector<uint8_t> TYPE_B_CAPTURE2_TIME_TO_FULL = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2B,
                                                                  0x5E, 0x5E, 0x5E, 0x5E, 0x15, 0x12};
// +RD,171400  → cmd=0x17 (cycles), LE=0x0014=20 → 20 cycles
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CYCLES = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2F,
                                                            0x29, 0x2C, 0x28, 0x28, 0x15, 0x12};
// +RD,1810270A  → cmd=0x18 (design capacity), LE=0x2710=10000, scale=0x0A → 100.00Ah
static const std::vector<uint8_t> TYPE_B_CAPTURE2_DESIGN_CAPACITY = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x20, 0x29,
                                                                     0x28, 0x2A, 0x2F, 0x28, 0x59, 0x15, 0x12};
// +RD,3FF10C  → cmd=0x3F=63 (cell 1), LE=0x0CF1=3313 → 3.313V
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CELL_VOLTAGE_01 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5E,
                                                                     0x5E, 0x29, 0x28, 0x5B, 0x15, 0x12};
// +RD,3EF30C  → cmd=0x3E=62 (cell 2), LE=0x0CF3=3315 → 3.315V
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CELL_VOLTAGE_02 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5D,
                                                                     0x5E, 0x2B, 0x28, 0x5B, 0x15, 0x12};
// +RD,3DF30C  → cmd=0x3D=61 (cell 3), LE=0x0CF3=3315 → 3.315V
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CELL_VOLTAGE_03 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5C,
                                                                     0x5E, 0x2B, 0x28, 0x5B, 0x15, 0x12};
// +RD,3CF40C  → cmd=0x3C=60 (cell 4), LE=0x0CF4=3316 → 3.316V
static const std::vector<uint8_t> TYPE_B_CAPTURE2_CELL_VOLTAGE_04 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5B,
                                                                     0x5E, 0x2C, 0x28, 0x5B, 0x15, 0x12};

// ── Synthetic frames (not captured from a real device) ──────────────────────
// Cover decoder paths the real 4S captures do not exercise: a valid time to
// full, a charging current and a fully populated 16S pack.
// Encrypted with TYPE_B_ENCRYPTION_KEY.
// 16S pack: all cells 3.320V except cell 7 (min, 3.290V) and cell 12 (max, 3.350V).
// +RD,13B400  → cmd=0x13 (time to full), LE=0x00B4=180 min → 10800 sec
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_TIME_TO_FULL = {0x33, 0x4A, 0x5C, 0x34, 0x29, 0x2B,
                                                                   0x5A, 0x2C, 0x28, 0x28, 0x15, 0x12};
// +RD,0A881301  → cmd=0x0A (current), LE=0x1388=5000, scale=0x01 → 5.000A
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_CURRENT_CHARGING = {0x33, 0x4A, 0x5C, 0x34, 0x28, 0x59, 0x20,
                                                                       0x20, 0x29, 0x2B, 0x28, 0x29, 0x15, 0x12};
// +RD,3FF80C  → cmd=0x3F=63 (cell 1), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_01 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5E,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,3EF80C  → cmd=0x3E=62 (cell 2), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_02 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5D,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,3DF80C  → cmd=0x3D=61 (cell 3), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_03 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5C,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,3CF80C  → cmd=0x3C=60 (cell 4), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_04 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5B,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,3BF80C  → cmd=0x3B=59 (cell 5), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_05 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x5A,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,3AF80C  → cmd=0x3A=58 (cell 6), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_06 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x59,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,39DA0C  → cmd=0x39=57 (cell 7), LE=0x0CDA=3290 → 3.290V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_07 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x21,
                                                                          0x5C, 0x59, 0x28, 0x5B, 0x15, 0x12};
// +RD,38F80C  → cmd=0x38=56 (cell 8), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_08 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x20,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,37F80C  → cmd=0x37=55 (cell 9), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_09 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2F,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,36F80C  → cmd=0x36=54 (cell 10), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_10 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2E,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,35F80C  → cmd=0x35=53 (cell 11), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_11 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2D,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,34160D  → cmd=0x34=52 (cell 12), LE=0x0D16=3350 → 3.350V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_12 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2C,
                                                                          0x29, 0x2E, 0x28, 0x5C, 0x15, 0x12};
// +RD,33F80C  → cmd=0x33=51 (cell 13), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_13 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2B,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,32F80C  → cmd=0x32=50 (cell 14), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_14 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x2A,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,31F80C  → cmd=0x31=49 (cell 15), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_15 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x29,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};
// +RD,30F80C  → cmd=0x30=48 (cell 16), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_SYNTHETIC_16S_CELL_VOLTAGE_16 = {0x33, 0x4A, 0x5C, 0x34, 0x2B, 0x28,
                                                                          0x5E, 0x20, 0x28, 0x5B, 0x15, 0x12};

// ── Invalid frames ────────────────────────────────────────────────────────────

// 2 bytes: even length but decrypts to "+R" (missing "D," prefix) → rejected
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_TOO_SHORT = {0x33, 0x4A};

// 3 bytes: odd length → rejected at size check
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_ODD_LENGTH = {0x33, 0x4A, 0x5C};

}  // namespace esphome::ogt_bms_ble::testing

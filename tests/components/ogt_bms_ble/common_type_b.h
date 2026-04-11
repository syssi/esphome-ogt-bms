#pragma once
#include "common.h"

namespace esphome::ogt_bms_ble::testing {

// Encryption key for SmartBat-B12294:
//   hex_id = f"{int('12294'):04X}" = "3006"
//   crypt_seq = [2,5,4,3,1,4,1,6,8,3,7,2,5,8,9,3]
//   key_sum = crypt_seq[3]+crypt_seq[0]+crypt_seq[0]+crypt_seq[6] = 3+2+2+1 = 8
//   key = 8 + 8 (Type B offset) = 16 = 0x10
static const uint8_t TYPE_B_ENCRYPTION_KEY = 0x10;

// ── Frames from esp32-ble-example-type-b-faker.yaml ──────────────────────────
// Device: SmartBat-B12294, encryption key 0x10, Type B protocol
// Decrypt: each byte XOR 0x10 → "+RD,<hex_payload>\r\n"

// +RD,0974B2  → cmd=0x09 (total voltage), LE=0xB274=45684 → 45.684V
static const std::vector<uint8_t> TYPE_B_TOTAL_VOLTAGE = {0x3B, 0x42, 0x54, 0x3C, 0x20, 0x29,
                                                          0x27, 0x24, 0x52, 0x22, 0x1D, 0x1A};

// +RD,0F20030A  → cmd=0x0F (remaining capacity), LE=0x0320=800, scale=0x0A → 8.0Ah
static const std::vector<uint8_t> TYPE_B_CAPACITY_REMAINING = {0x3B, 0x42, 0x54, 0x3C, 0x20, 0x56, 0x22,
                                                               0x20, 0x20, 0x23, 0x20, 0x51, 0x1D, 0x1A};

// +RD,10D2040A  → cmd=0x10 (full charge capacity), LE=0x04D2=1234, scale=0x0A → 12.34Ah
static const std::vector<uint8_t> TYPE_B_FULL_CHARGE_CAPACITY = {0x3B, 0x42, 0x54, 0x3C, 0x21, 0x20, 0x54,
                                                                 0x22, 0x20, 0x24, 0x20, 0x51, 0x1D, 0x1A};

// +RD,127800  → cmd=0x12 (time to empty), LE=0x0078=120 min → 7200 sec
static const std::vector<uint8_t> TYPE_B_TIME_TO_EMPTY = {0x3B, 0x42, 0x54, 0x3C, 0x21, 0x22,
                                                          0x27, 0x28, 0x20, 0x20, 0x1D, 0x1A};

// +RD,13B400  → cmd=0x13 (time to full), LE=0x00B4=180 min → 10800 sec
static const std::vector<uint8_t> TYPE_B_TIME_TO_FULL = {0x3B, 0x42, 0x54, 0x3C, 0x21, 0x23,
                                                         0x52, 0x24, 0x20, 0x20, 0x1D, 0x1A};

// +RD,176300  → cmd=0x17 (cycles), LE=0x0063=99 → 99 cycles
static const std::vector<uint8_t> TYPE_B_CYCLES = {0x3B, 0x42, 0x54, 0x3C, 0x21, 0x27,
                                                   0x26, 0x23, 0x20, 0x20, 0x1D, 0x1A};

// +RD,18B0040A  → cmd=0x18 (design capacity), LE=0x04B0=1200, scale=0x0A → 12.0Ah
static const std::vector<uint8_t> TYPE_B_DESIGN_CAPACITY = {0x3B, 0x42, 0x54, 0x3C, 0x21, 0x28, 0x52,
                                                            0x20, 0x20, 0x24, 0x20, 0x51, 0x1D, 0x1A};

// Cell voltages: cmd=0x3E..0x2F, cell_index = 62 - cmd, voltage = LE * 0.001V
// +RD,3E7A0D  → cmd=0x3E=62 (cell 1), LE=0x0D7A=3450 → 3.450V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_01 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x55,
                                                            0x27, 0x51, 0x20, 0x54, 0x1D, 0x1A};
// +RD,3D700D  → cmd=0x3D=61 (cell 2), LE=0x0D70=3440 → 3.440V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_02 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x54,
                                                            0x27, 0x20, 0x20, 0x54, 0x1D, 0x1A};
// +RD,3C660D  → cmd=0x3C=60 (cell 3), LE=0x0D66=3430 → 3.430V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_03 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x53,
                                                            0x26, 0x26, 0x20, 0x54, 0x1D, 0x1A};
// +RD,3B5C0D  → cmd=0x3B=59 (cell 4), LE=0x0D5C=3420 → 3.420V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_04 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x52,
                                                            0x25, 0x53, 0x20, 0x54, 0x1D, 0x1A};
// +RD,3A520D  → cmd=0x3A=58 (cell 5), LE=0x0D52=3410 → 3.410V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_05 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x51,
                                                            0x25, 0x22, 0x20, 0x54, 0x1D, 0x1A};
// +RD,39480D  → cmd=0x39=57 (cell 6), LE=0x0D48=3400 → 3.400V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_06 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x29,
                                                            0x24, 0x28, 0x20, 0x54, 0x1D, 0x1A};
// +RD,383E0D  → cmd=0x38=56 (cell 7), LE=0x0D3E=3390 → 3.390V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_07 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x28,
                                                            0x23, 0x55, 0x20, 0x54, 0x1D, 0x1A};
// +RD,37340D  → cmd=0x37=55 (cell 8), LE=0x0D34=3380 → 3.380V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_08 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x27,
                                                            0x23, 0x24, 0x20, 0x54, 0x1D, 0x1A};
// +RD,362A0D  → cmd=0x36=54 (cell 9), LE=0x0D2A=3370 → 3.370V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_09 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x26,
                                                            0x22, 0x51, 0x20, 0x54, 0x1D, 0x1A};
// +RD,35200D  → cmd=0x35=53 (cell 10), LE=0x0D20=3360 → 3.360V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_10 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x25,
                                                            0x22, 0x20, 0x20, 0x54, 0x1D, 0x1A};
// +RD,34160D  → cmd=0x34=52 (cell 11), LE=0x0D16=3350 → 3.350V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_11 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x24,
                                                            0x21, 0x26, 0x20, 0x54, 0x1D, 0x1A};
// +RD,330C0D  → cmd=0x33=51 (cell 12), LE=0x0D0C=3340 → 3.340V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_12 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x23,
                                                            0x20, 0x53, 0x20, 0x54, 0x1D, 0x1A};
// +RD,32020D  → cmd=0x32=50 (cell 13), LE=0x0D02=3330 → 3.330V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_13 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x22,
                                                            0x20, 0x22, 0x20, 0x54, 0x1D, 0x1A};
// +RD,31F80C  → cmd=0x31=49 (cell 14), LE=0x0CF8=3320 → 3.320V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_14 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x21,
                                                            0x56, 0x28, 0x20, 0x53, 0x1D, 0x1A};
// +RD,30EE0C  → cmd=0x30=48 (cell 15), LE=0x0CEE=3310 → 3.310V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_15 = {0x3B, 0x42, 0x54, 0x3C, 0x23, 0x20,
                                                            0x55, 0x55, 0x20, 0x53, 0x1D, 0x1A};
// +RD,2FE40C  → cmd=0x2F=47 (cell 16), LE=0x0CE4=3300 → 3.300V
static const std::vector<uint8_t> TYPE_B_CELL_VOLTAGE_16 = {0x3B, 0x42, 0x54, 0x3C, 0x22, 0x56,
                                                            0x55, 0x24, 0x20, 0x53, 0x1D, 0x1A};

// ── Invalid frames ────────────────────────────────────────────────────────────

// 2 bytes: even length but decrypts to "+R" (missing "D," prefix) → rejected
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_TOO_SHORT = {0x3B, 0x42};

// 3 bytes: odd length → rejected at size check
static const std::vector<uint8_t> TYPE_B_INVALID_FRAME_ODD_LENGTH = {0x3B, 0x42, 0x54};

}  // namespace esphome::ogt_bms_ble::testing

#pragma once
#include "common.h"

namespace esphome::ogt_bms_ble::testing {

// Encryption key for SmartBat-A03747:
//   hex_id = f"{int('03747'):04X}" = "0EA3"
//   crypt_seq = [2,5,4,3,1,4,1,6,8,3,7,2,5,8,9,3]
//   key_sum = crypt_seq[0]+crypt_seq[14]+crypt_seq[10]+crypt_seq[3] = 2+9+7+3 = 21
//   key = 21 + 5 (Type A offset) = 26 = 0x1A
static const uint8_t TYPE_A_ENCRYPTION_KEY = 0x1A;

// ── Real-life frames from esp32-ble-example-type-a-faker.yaml ────────────────
// Device: SmartBat-A03747, encryption key 0x1A, Type A protocol
// Decrypt: each byte XOR 0x1A → "+RD,<hex_payload>\r\n"

// +RD,0246  → cmd=0x02 (SOC), value=0x46=70 → SOC=70%
static const std::vector<uint8_t> TYPE_A_SOC_70 = {0x31, 0x48, 0x5E, 0x36, 0x2A, 0x28, 0x2E, 0x2C, 0x17, 0x10};

// +RD,04251C0A  → cmd=0x04 (remaining capacity), LE=0x1C25=7205, scale=0x0A → 72.05Ah
static const std::vector<uint8_t> TYPE_A_CAPACITY_REMAINING = {0x31, 0x48, 0x5E, 0x36, 0x2A, 0x2E, 0x28,
                                                               0x2F, 0x2B, 0x59, 0x2A, 0x5B, 0x17, 0x10};

// +RD,06DE270A  → cmd=0x06 (full charge capacity), LE=0x27DE=10206, scale=0x0A → 102.06Ah
static const std::vector<uint8_t> TYPE_A_FULL_CHARGE_CAPACITY = {0x31, 0x48, 0x5E, 0x36, 0x2A, 0x2C, 0x5E,
                                                                 0x5F, 0x28, 0x2D, 0x2A, 0x5B, 0x17, 0x10};

// +RD,080034  → cmd=0x08 (total voltage), LE=0x3400=13312 → 13.312V
static const std::vector<uint8_t> TYPE_A_TOTAL_VOLTAGE = {0x31, 0x48, 0x5E, 0x36, 0x2A, 0x22,
                                                          0x2A, 0x2A, 0x29, 0x2E, 0x17, 0x10};

// +RD,0C980B  → cmd=0x0C (temperature), LE=0x0B98=2968 → 2968*0.1-273.15=23.65°C
static const std::vector<uint8_t> TYPE_A_TEMPERATURE = {0x31, 0x48, 0x5E, 0x36, 0x2A, 0x59,
                                                        0x23, 0x22, 0x2A, 0x58, 0x17, 0x10};

// +RD,10D9FF00  → cmd=0x10 (current), LE=int16(0xFFD9)=-39 → -0.39A (discharging)
static const std::vector<uint8_t> TYPE_A_CURRENT_DISCHARGE_A = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x2A, 0x5E,
                                                                0x23, 0x5C, 0x5C, 0x2A, 0x2A, 0x17, 0x10};

// +RD,10DCFF00  → cmd=0x10 (current), LE=int16(0xFFDC)=-36 → -0.36A (discharging)
static const std::vector<uint8_t> TYPE_A_CURRENT_DISCHARGE_B = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x2A, 0x5E,
                                                                0x59, 0x5C, 0x5C, 0x2A, 0x2A, 0x17, 0x10};

// +RD,10DFFF00  → cmd=0x10 (current), LE=int16(0xFFDF)=-33 → -0.33A (discharging)
static const std::vector<uint8_t> TYPE_A_CURRENT_DISCHARGE_C = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x2A, 0x5E,
                                                                0x5C, 0x5C, 0x5C, 0x2A, 0x2A, 0x17, 0x10};

// +RD,18A42D  → cmd=0x18 (time to empty), LE=0x2DA4=11684 min → 701040 sec
static const std::vector<uint8_t> TYPE_A_TIME_TO_EMPTY = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x22,
                                                          0x5B, 0x2E, 0x28, 0x5E, 0x17, 0x10};

// +RD,1AFFFF  → cmd=0x1A (time to full), 0xFFFF → NaN (not charging)
static const std::vector<uint8_t> TYPE_A_TIME_TO_FULL_INVALID = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x5B,
                                                                 0x5C, 0x5C, 0x5C, 0x5C, 0x17, 0x10};

// +RD,28A30E  → cmd=0x28 (serial number), LE=0x0EA3=3747 → "03747"
static const std::vector<uint8_t> TYPE_A_SERIAL_NUMBER = {0x31, 0x48, 0x5E, 0x36, 0x28, 0x22,
                                                          0x5B, 0x29, 0x2A, 0x5F, 0x17, 0x10};

// +RD,2C1A00  → cmd=0x2C (cycles), LE=0x001A=26 → 26 cycles
static const std::vector<uint8_t> TYPE_A_CYCLES = {0x31, 0x48, 0x5E, 0x36, 0x28, 0x59,
                                                   0x2B, 0x5B, 0x2A, 0x2A, 0x17, 0x10};

// +RD,3C10270A  → cmd=0x3C (design capacity), LE=0x2710=10000, scale=0x0A → 100.0Ah
static const std::vector<uint8_t> TYPE_A_DESIGN_CAPACITY = {0x31, 0x48, 0x5E, 0x36, 0x29, 0x59, 0x2B,
                                                            0x2A, 0x28, 0x2D, 0x2A, 0x5B, 0x17, 0x10};

// +RD,487150  → cmd=0x48 (manufacture date), LE=0x5071=20593 → 2020.03.17
static const std::vector<uint8_t> TYPE_A_MANUFACTURE_DATE = {0x31, 0x48, 0x5E, 0x36, 0x2E, 0x22,
                                                             0x2D, 0x2B, 0x2F, 0x2A, 0x17, 0x10};

// ── Synthesized frames (key 0x1A, not in faker yaml) ─────────────────────────

// +RD,107B0000  → cmd=0x10 (current), LE=0x007B=123 → +1.23A (charging)
static const std::vector<uint8_t> TYPE_A_CURRENT_CHARGE = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x2A, 0x2D,
                                                           0x58, 0x2A, 0x2A, 0x2A, 0x2A, 0x17, 0x10};

// +RD,10000000  → cmd=0x10 (current), LE=0x0000=0 → 0A (treated as discharging)
static const std::vector<uint8_t> TYPE_A_CURRENT_ZERO = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x2A, 0x2A,
                                                         0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x17, 0x10};

// +RD,1AB400  → cmd=0x1A (time to full), LE=0x00B4=180 min → 10800 sec
static const std::vector<uint8_t> TYPE_A_TIME_TO_FULL_VALID = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x5B,
                                                               0x58, 0x2E, 0x2A, 0x2A, 0x17, 0x10};

// +RD,18FFFF  → cmd=0x18 (time to empty), 0xFFFF → NaN
static const std::vector<uint8_t> TYPE_A_TIME_TO_EMPTY_INVALID = {0x31, 0x48, 0x5E, 0x36, 0x2B, 0x22,
                                                                  0x5C, 0x5C, 0x5C, 0x5C, 0x17, 0x10};

// ── Invalid frames ────────────────────────────────────────────────────────────

// 2 bytes: even length but decrypts to "+R" (missing "D," prefix) → rejected
static const std::vector<uint8_t> TYPE_A_INVALID_FRAME_TOO_SHORT = {0x31, 0x48};

// 3 bytes: odd length → rejected at size check
static const std::vector<uint8_t> TYPE_A_INVALID_FRAME_ODD_LENGTH = {0x31, 0x48, 0x5E};

}  // namespace esphome::ogt_bms_ble::testing

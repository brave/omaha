// Copyright 2023 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================
//
// CUP-ECDSA public keys consist of a byte array, 66 bytes long, containing:
// * The key ID (one byte)
// * The public key in X9.62 uncompressed encoding (65 bytes):
//     * Uncompressed header byte (0x04)
//     * Gx coordinate (256-bit integer, big-endian)
//     * Gy coordinate (256-bit integer, big-endian)
{0x02,
0x04,
0x29, 0x3e, 0x29, 0xa6, 0x00, 0x8b, 0x36, 0x14,
0xdb, 0x54, 0x4f, 0x1d, 0x5d, 0xc1, 0x70, 0x2f,
0x31, 0xdc, 0xab, 0xcf, 0xd1, 0x7e, 0x3f, 0x97,
0x5a, 0x6b, 0xc3, 0xe8, 0x6c, 0x80, 0x91, 0xd4,
0xd4, 0xa8, 0x3a, 0x22, 0xb2, 0x63, 0xd2, 0x12,
0x2d, 0x8f, 0x19, 0x8a, 0x5f, 0x85, 0x42, 0x1f,
0xef, 0x87, 0x8a, 0x60, 0x52, 0xf1, 0xf6, 0x3c,
0xe1, 0x35, 0xee, 0x8f, 0x30, 0x79, 0xdd, 0x09};

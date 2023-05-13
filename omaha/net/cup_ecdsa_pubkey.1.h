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
{0x01,
0x04,
0xc6, 0x78, 0x80, 0x7b, 0x51, 0x70, 0x8d, 0xb1,
0xb0, 0x1d, 0x1e, 0x9d, 0xd4, 0x49, 0x1b, 0xce,
0x6e, 0x67, 0xaf, 0xb9, 0x42, 0xeb, 0x62, 0x75,
0xbe, 0x40, 0xc6, 0x7c, 0xa9, 0xcc, 0xb4, 0xd8,
0xc0, 0x56, 0x40, 0x27, 0xe5, 0x4e, 0x27, 0x2f,
0x03, 0x15, 0xe3, 0xb2, 0x39, 0x4f, 0xf4, 0x98,
0x22, 0x2e, 0x59, 0xa6, 0x1b, 0x29, 0xfc, 0xf2,
0x5f, 0x67, 0x49, 0xe0, 0x95, 0x24, 0x4a, 0xae};
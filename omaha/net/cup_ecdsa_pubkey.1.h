// Copyright 2018 Google Inc.
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
0x6a, 0x13, 0xc5, 0xf6, 0xb7, 0x28, 0xad, 0x94,
0xbf, 0x77, 0x84, 0xc6, 0x95, 0x82, 0x09, 0x9b,
0xab, 0x02, 0xed, 0x27, 0x97, 0x5d, 0x58, 0x43,
0x3c, 0x85, 0x70, 0x13, 0x5a, 0xeb, 0xb1, 0xf3,
0x62, 0xb3, 0x21, 0x43, 0x47, 0x26, 0x2e, 0x18,
0x7e, 0x27, 0xc4, 0xbb, 0x5a, 0x9f, 0x0e, 0xe0,
0x57, 0xa8, 0x2b, 0x51, 0xb0, 0x9b, 0x77, 0x12,
0x93, 0x6e, 0x2d, 0xdd, 0xc0, 0x2a, 0x97, 0x66};

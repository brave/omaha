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
0x84, 0xc9, 0x34, 0x7a, 0x5a, 0x3a, 0xfe, 0x18,
0x1c, 0x8a, 0xe2, 0x65, 0xff, 0x82, 0x85, 0x3f,
0x70, 0x1a, 0x6b, 0x5b, 0xf6, 0xd3, 0xc4, 0xe7,
0x54, 0xbd, 0x4c, 0x8a, 0x85, 0xb8, 0xdb, 0xe6,
0xe5, 0x22, 0x6b, 0xf5, 0x71, 0x50, 0xa2, 0x5b,
0xa0, 0x96, 0x65, 0x2b, 0x43, 0x0b, 0xdf, 0x67,
0x7b, 0x09, 0x6c, 0x3f, 0x31, 0x5e, 0xfe, 0xb0,
0x3c, 0x1e, 0x2e, 0x78, 0x68, 0x14, 0x6f, 0x1f};

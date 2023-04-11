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
0x69, 0x56, 0x0a, 0x02, 0xae, 0xb8, 0x48, 0x44,
0x8a, 0x4d, 0x00, 0x1c, 0xff, 0x09, 0xb7, 0xb1,
0xfe, 0x6f, 0x6e, 0x83, 0xd3, 0x12, 0x60, 0x2e,
0xb1, 0xb6, 0xc8, 0x4b, 0x2f, 0x52, 0x80, 0x2b,
0x28, 0x62, 0x28, 0x2a, 0x88, 0x55, 0x3c, 0xea,
0x2d, 0x74, 0x98, 0x51, 0x89, 0xc0, 0x03, 0x0d,
0x01, 0xf4, 0xd9, 0xdc, 0x9a, 0x9a, 0x06, 0x89,
0xf7, 0xbb, 0xd7, 0x2b, 0x34, 0x41, 0xb5, 0x13};

/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "at32f4xx.h"
#include "read_uid.h"

//#define STM32_UUID ((uint32_t *)0x1FFF7A10)

void read_unique_id(uint32_t *id) {
    *id = 0x11223344;//STM32_UUID[0] ^ STM32_UUID[1] ^ STM32_UUID[2];
}

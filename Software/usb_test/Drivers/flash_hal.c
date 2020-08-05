/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2014 ARM Limited
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

#include "flash_hal.h"        // FlashOS Structures
#include "at32f4xx.h" 

#define M8(adr)  (*((vu8  *) (adr)))
#define M16(adr) (*((vu16 *) (adr)))
#define M32(adr) (*((vu32 *) (adr)))


// Flash Keys
#define RDPRT_KEY       0x00A5
#define FLASH_KEY1      0x45670123
#define FLASH_KEY2      0xCDEF89AB

// Flash Control Register definitions
#define FLASH_PG                ((unsigned int)0x00000001)
#define FLASH_SER               ((unsigned int)0x00000002)
#define FLASH_MER               ((unsigned int)0x00000004)
#define FLASH_MER1              ((unsigned int)0x00008000)
#define FLASH_SNB_POS           ((unsigned int)0x00000003)
#define FLASH_SNB_MSK           ((unsigned int)0x000000F8)
#define FLASH_PSIZE_POS         ((unsigned int)0x00000008)
#define FLASH_PSIZE_MSK         ((unsigned int)0x00000300)
#define FLASH_STRT              ((unsigned int)0x00010000)
#define FLASH_LOCK              ((unsigned int)0x80000000)

// Flash Option Control Register definitions
#define FLASH_OPTLOCK           ((unsigned int)0x00000001)
#define FLASH_OPTSTRT           ((unsigned int)0x00000002)


#define FLASH_PSIZE_Byte        ((unsigned int)0x00000000)
#define FLASH_PSIZE_HalfWord    ((unsigned int)0x00000100)
#define FLASH_PSIZE_Word        ((unsigned int)0x00000200)
#define FLASH_PSIZE_DoubleWord  ((unsigned int)0x00000300)

// Flash Status Register definitions
#define FLASH_EOP               ((unsigned int)0x00000001)
#define FLASH_OPERR             ((unsigned int)0x00000002)
#define FLASH_WRPERR            ((unsigned int)0x00000010)
#define FLASH_PGAERR            ((unsigned int)0x00000020)
#define FLASH_PGPERR            ((unsigned int)0x00000040)
#define FLASH_PGSERR            ((unsigned int)0x00000080)
#define FLASH_BSY               ((unsigned int)0x00010000)

#define FLASH_PGERR             (FLASH_PGSERR | FLASH_PGPERR | FLASH_PGAERR | FLASH_WRPERR)




/*
 * Get Sector Number
 *    Parameter:      adr:  Sector Address
 *    Return Value:   Sector Number
 */

unsigned long GetSecNum (unsigned long adr) {
  unsigned long n;

  n = (adr >> 12) & 0x000FF;                             // only bits 12..19

  if    (n >= 0x20) {
    n = 4 + (n >> 5);                                    // 128kB Sector
  }
  else if (n >= 0x10) {
    n = 3 + (n >> 4);                                    //  64kB Sector
  }
  else                {
    n = 0 + (n >> 2);                                    //  16kB Sector
  }

  if (adr & 0x00100000)
    n |=0x00000010;                                      // sector in second half

  return (n);                                            // Sector Number
}

int flash_hal_init(uint32_t clk)
{

//  FLASH->KEYR = FLASH_KEY1;                             // Unlock Flash
//  FLASH->KEYR = FLASH_KEY2;

//	  /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
//  FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
//	
//  FLASH->SR  |= FLASH_PGERR;                            // Reset Error Flags

//  if ((FLASH->OPTCR & 0x20) == 0x00) {                  // Test if IWDG is running (IWDG in HW mode)
//    // Set IWDG time out to ~32.768 second
//    IWDG->KR  = 0x5555;                                 // Enable write access to IWDG_PR and IWDG_RLR     
//    IWDG->PR  = 0x06;                                   // Set prescaler to 256  
//    IWDG->RLR = 4095;                                   // Set reload value to 4095
//  }

  return (0);
}

int flash_hal_uninit(uint32_t fnc)
{
//  FLASH->CR |=  FLASH_LOCK;                             // Lock Flash

  return (0);
}

int flash_hal_erase_chip(void)
{

//  FLASH->CR |=  FLASH_MER;                              // Mass Erase Enabled (sectors  0..11)
//#ifdef STM32F4xx_2048
//  FLASH->CR |=  FLASH_MER1;                             // Mass Erase Enabled (sectors 12..23)
//#endif
//  FLASH->CR |=  FLASH_STRT;                             // Start Erase

//  while (FLASH->SR & FLASH_BSY) {
//    IWDG->KR = 0xAAAA;                                  // Reload IWDG
//  }

//  FLASH->CR &= ~FLASH_MER;                              // Mass Erase Disabled
//#ifdef STM32F4xx_2048
//  FLASH->CR &= ~FLASH_MER1;                             // Mass Erase Disabled
//#endif

  return (0);                                           // Done
}

int flash_hal_erase_sector(uint32_t adr)
{
//  unsigned long n;

//  n = GetSecNum(adr);                                   // Get Sector Number

//  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags

//  FLASH->CR  =  FLASH_SER;                              // Sector Erase Enabled 
//  FLASH->CR |=  ((n << FLASH_SNB_POS) & FLASH_SNB_MSK); // Sector Number
//  FLASH->CR |=  FLASH_STRT;                             // Start Erase

//  while (FLASH->SR & FLASH_BSY) {
//    IWDG->KR = 0xAAAA;                                  // Reload IWDG
//  }

//  FLASH->CR &= ~FLASH_SER;                              // Page Erase Disabled 

//  if (FLASH->SR & FLASH_PGERR) {                        // Check for Error
//    FLASH->SR |= FLASH_PGERR;                           // Reset Error Flags
//    return (1);                                         // Failed
//  }

  return (0);                                           // Done
}

int flash_hal_program_page(uint32_t adr, uint32_t sz, unsigned char *buf)
{
//  sz = (sz + 3) & ~3;                                   // Adjust size for Words
//  
//  FLASH->SR |= FLASH_PGERR;                             // Reset Error Flags
//  FLASH->CR  =  0;                                      // reset CR 

//  while (sz) {
//    FLASH->CR |= (FLASH_PG              |               // Programming Enabled
//                  FLASH_PSIZE_Word);                    // Programming Enabled (Word)

//    M32(adr) = *((u32 *)buf);                           // Program Double Word
//    while (FLASH->SR & FLASH_BSY);

//    FLASH->CR &= ~FLASH_PG;                             // Programming Disabled

//    if (FLASH->SR & FLASH_PGERR) {                      // Check for Error
//      FLASH->SR |= FLASH_PGERR;                         // Reset Error Flags
//      return (1);                                       // Failed
//    }

//    adr += 4;                                           // Go to next Word
//    buf += 4;
//    sz  -= 4;
//  }

  return (0);                                           // Done
}

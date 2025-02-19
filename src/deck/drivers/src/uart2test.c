/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2021 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * uart2test.c - Uart echo implementation to test uart.
 */
#define DEBUG_MODULE "U1T"

#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "stm32fxxx.h"
#include "system.h"
#include "config.h"
#include "debug.h"
#include "deck.h"
#include "uart2.h"

#include "deck.h"
#include "param.h"


#include <stdio.h>

typedef enum {

    sn_STOP = 0,
    sn_FOWARD = 1,
    sn_BACKWARD = 2,
    sn_SPINR = 3,
    sn_SPINL = 4,
    sn_QI = 5,
    sn_ARMF1 = 6, 
    sn_ARMF2 = 7, 
    sn_ARMF3 = 8, 
    sn_RST = 9

} SN;


static int sn;



//Hardware configuration
static bool isInit;

void uart2testTask(void* arg);

static void uart2testInit(DeckInfo *info)
{
  if(isInit)
    return;

  uart2Init(9600);

  xTaskCreate(uart2testTask, UART2_TEST_TASK_NAME, UART2_TEST_TASK_STACKSIZE, NULL, UART2_TEST_TASK_PRI, NULL);

  isInit = true;
}

static bool uart2testTest()
{
  bool status = true;

  if(!isInit)
    return false;

  return status;
}


void uart2testTask(void* arg)
{
  systemWaitStart();

  while (1)
  {

    switch (sn) {
      case sn_STOP:
        uart2SendDataDmaBlocking(1, (uint8_t *)"0");
        break;
      case sn_FOWARD:
        uart2SendDataDmaBlocking(1, (uint8_t *)"1");
        break;
      case sn_BACKWARD:
        uart2SendDataDmaBlocking(1, (uint8_t *)"2");
        break;
      case sn_SPINR:
        uart2SendDataDmaBlocking(1, (uint8_t *)"3");
        break;
      case sn_SPINL:
        uart2SendDataDmaBlocking(1, (uint8_t *)"4");
        break;
        case sn_QI:
        uart2SendDataDmaBlocking(1, (uint8_t *)"5");
        break;
      case sn_ARMF1:
        uart2SendDataDmaBlocking(1, (uint8_t *)"6");
        break;
      case sn_ARMF2:
        uart2SendDataDmaBlocking(1, (uint8_t *)"7");
        break;
      case sn_ARMF3:
        uart2SendDataDmaBlocking(1, (uint8_t *)"8");
        break;
      case sn_RST:
        uart2SendDataDmaBlocking(1, (uint8_t *)"9");
        break;
      default:
        uart2SendDataDmaBlocking(1, (uint8_t *)"0");
        break;
    
    }
  }
}

PARAM_GROUP_START(sn)
PARAM_ADD(PARAM_UINT32, state, &sn)
PARAM_GROUP_STOP(sn)

static const DeckDriver deck_uart2test = {
  .vid = 0xBC,
  .pid = 0x08,
  .name = "bcUart2Test",

  .usedPeriph = DECK_USING_UART2,
  .usedGpio = 0,
  .init = uart2testInit,
  .test = uart2testTest,
};

DECK_DRIVER(deck_uart2test);

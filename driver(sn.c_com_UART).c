#include <stdint.h>
#include <stdlib.h>
#include "stm32fxxx.h"

#include "FreeRTOS.h"
#include "timers.h"

#include "deck.h"
#include "param.h"

#include "uart2.h"

/* Define quais IOs o deck de expansão vai usar. */
#define TXr     DECK_GPIO_TX2
#define RXr     DECK_GPIO_RX2

/* Configuração de diferentes estados por meio de enumeração. */
typedef char SN[100];

static SN sn;

/* Loop e Handler do timer. */
static xTimerHandle timer;
static void snTimer(xTimerHandle timer)
{
    uart2SendData(100, sn);
}

/* Main initialization */
static void snInit(DeckInfo *info)
{
    uart2Init(9600);

    timer = xTimerCreat("snTimer", M2T(10),
                          pdTRUE, NULL, snTimer);
    xTimerStart(timer, 100);

}

PARAM_GROUP_START(sn)
PARAM_ADD(PARAM_UINT32, state, &sn)
PARAM_GROUP_STOP(sn)

static const DeckDriver sn_deck = {
  .vid = 0,
  .pid = 0,
  .name = "ufuSN",
  .usedGpio = DECK_USING_UART2,
  .init = snInit,
};

DECK_DRIVER(sn_deck);

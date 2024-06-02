#include <stdint.h>
#include <stdlib.h>
#include "stm32fxxx.h"

#include "FreeRTOS.h"
#include "timers.h"

#include "deck.h"
#include "param.h"

 /* Os diodos são conectados entre o pino MCU e GND.*/ 
#define LED_ON HIGH
#define LED_OFF LOW

/* Define quais IOs o deck de expansão vai usar. */
#define RED_LED     DECK_GPIO_IO1
#define GREEN_LED     DECK_GPIO_IO4

/* Configuração de diferentes estados por meio de enumeração. */
typedef enum {
    sn_off = 0,
    sn_red = 1,
    sn_green = 2,
    sn_all = 3
} SN;
static SN sn;

/* Loop e Handler do timer. */
static xTimerHandle timer;
static void snTimer(xTimerHandle timer)
{
  digitalWrite(RED_LED, LED_OFF);
  digitalWrite(GREEN_LED, LED_OFF);
  switch (sn) {
    case sn_red:
      digitalWrite(RED_LED, LED_ON);
      break;
    case sn_green:
      digitalWrite(GREEN_LED, LED_ON);
      break;
    case sn_all:
      digitalWrite(RED_LED, LED_ON);
      digitalWrite(GREEN_LED, LED_ON);
      break;
    default:
      /* Matém todos os LEDs desligados. */
      break;
  }
}

/* Main initialization */
static void snInit(DeckInfo *info)
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(RED_LED, LED_OFF);
  digitalWrite(GREEN_LED, LED_OFF);

  timer = xTimerCreate( "snTimer", M2T(10),
                         pdTRUE, NULL, tfTimer );
  xTimerStart(timer, 100);
}

PARAM_GROUP_START(sn)
PARAM_ADD(PARAM_UINT32, state, &sn)
PARAM_GROUP_STOP(sn)

static const DeckDriver sn_deck = {
  .vid = 0,
  .pid = 0,
  .name = "bcSN",
  .usedGpio = DECK_USING_IO_1 | DECK_USING_IO_4,
  .init = snInit,
};

DECK_DRIVER(sn_deck);

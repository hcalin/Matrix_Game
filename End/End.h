#ifndef END_H
# define END_H

/* Header definitions */
#include "Pattern.h"

/* Definition of digits on the matrix; */
byte ZERO_LEFT[] = {
  B00000000,
  B01110000,
  B01010000,
  B01010000,
  B01010000,
  B01010000,
  B01110000,
  B00000000
};
byte ONE_LEFT[] = {
  B00000000,
  B00010000,
  B00110000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00000000
};
byte TWO_LEFT[] = {
  B00000000,
  B01110000,
  B00010000,
  B01110000,
  B01000000,
  B01110000,
  B00000000,
  B00000000
};
byte THREE_LEFT[] = {
  B00000000,
  B01110000,
  B00010000,
  B01110000,
  B00010000,
  B01110000,
  B00000000,
  B00000000
};
byte FOUR_LEFT[] = {
  B00000000,
  B01000000,
  B01010000,
  B01110000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};
byte FIVE_LEFT[] = {
  B00000000,
  B01110000,
  B01000000,
  B01110000,
  B00010000,
  B01110000,
  B00000000,
  B00000000
};
byte SIX_LEFT[] = {
  B00000000,
  B01110000,
  B01000000,
  B01110000,
  B01010000,
  B01110000,
  B00000000,
  B00000000
};
byte SEVEN_LEFT[] = {
  B00000000,
  B01110000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};
byte EIGHT_LEFT[] = {
  B00000000,
  B01110000,
  B01010000,
  B01110000,
  B01010000,
  B01110000,
  B00000000,
  B00000000
};
byte NINE_LEFT[] = {
  B00000000,
  B01110000,
  B01010000,
  B01110000,
  B00010000,
  B01110000,
  B00000000,
  B00000000
};

/* making an array with those bytes */
typedef byte * left_digits;

const left_digits left_digits_arr[10] = {
  ZERO_LEFT,
  ONE_LEFT,
  TWO_LEFT,
  THREE_LEFT,
  FOUR_LEFT,
  FIVE_LEFT,
  SIX_LEFT,
  SEVEN_LEFT,
  EIGHT_LEFT,
  NINE_LEFT
};

uint32_t END_timePerDigit = 2000; //10 ms
uint32_t END_currTime = 0x00;
uint32_t END_prevTime = 0x00;

/*@desc: 
This function is used to reset game variables and prepare for the next game
@param:
no param
@return:
void
*/
void END_reset() {
  END_prevTime = 0x00;
  END_timePerDigit = 10;
  PAT_Reset();
  PAT_EndGameReset();
}

/*@desc: 
This function is used as a task for showing the end game score and make the transition to idle state
@param:
no param
@return:
void
*/
void END_StateFunction() {
  uint8_t scoreIndex = 0x00;
  END_prevTime = millis();
  while (1) {
    END_currTime = millis();
    if (END_currTime - END_prevTime > 4000) {
      scoreIndex++;
      Serial.println(PAT_Score);

      END_reset();
      state = IDLE_STATE;
      break;
    }
    DRAW_rightOnScreen(left_digits_arr[PAT_Score]);

  }
}

#endif
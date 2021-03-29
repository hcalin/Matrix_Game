/* MEMORY GAME  */

/* Includes */
#include "MatrixSettings.h"

#include "Reading.h"

#include "Draw.h"

#include "Pattern.h"

#include "End.h"


/* Delay time per cycle set to 200; otherwise the transitions between frames is too fast */
#define DELAY_TIME_PER_CYCLE 200

void setup() {
  /* Begin serial communication */
  Serial.begin(9600);

  /* initialize all pins as output high */
  initPins();
  /* adjust laser detection threshold values for the columns */
  adjustThreshCol();
  /* set the default screen for the idle state */
  memcpy(showByte, Idle_Image + 8, 8);
  /* set default system state to IDLE_STATE */
  state = IDLE_STATE;
}

/* Main loop */
void loop() {
  switch (state) {
  case IDLE_STATE:
    IdleStateFunction();
    break;
  case COUNTDOWN_STATE:
    DRAW_CountDownStateFunction();
    break;
  case SHOWPATTERN_STATE:
    PAT_ShowPatternFunction();
    break;
  case TRACKPATTERN_STATE:
    PAT_TrackPatternFunction();
    break;
  case END_STATE:
    END_StateFunction();
    break;
  default:
    break;
  }
  _delay_ms(DELAY_TIME_PER_CYCLE);
}

/* IDLE Task - called when the system is in idle state */
void IdleStateFunction() {
  uint8_t col = checkColumn();
  uint8_t row = checkRow();

  if (col != 9 && row != 9) {
    /* if we detected the laser begin the countdown */
    state = COUNTDOWN_STATE;
  } else {
    /* else display idle message */
    DRAW_Screen(showByte);
    currTime = millis();
	/* If less than 50 ms passed then don't update the matrix */
    if (currTime - prevTime > 50) {
      /* Swiping algorithm; calculating the matrix for the next iteration */
      for (uint8_t i = 0; i < 8; i++) // rows
      {
        showByte[i] <<= 1;
        showByte[i] &= ~(0x01);
        showByte[i] |= ((Idle_Image[next_letter * 8 + i] >> maskNr) & 0x01);
      }
      if (maskNr == 0) {
        maskNr = 8;
        next_letter++;
        if (next_letter > 10) next_letter = 0;
      } else {
        maskNr--;
      }
      /* Keep the screen for a little bit more; for visual effect */
      _delay_ms(3);

	  /* See in which iteration we are of the matrix */
      shift++;
      if (shift == scrollTextLen) {
        shift = 0;
      }
      prevTime = currTime;
    }
  }
}
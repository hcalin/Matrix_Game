#ifndef PATTERN_H /* GUARD __PATTERN_H_ */
# define PATTERN_H

#include "Draw.h"

/* Variables */

uint8_t currNrOfPoints = 6; // currentNrOfPoints per Pattern; Starts with 10 points
uint8_t PAT_lvlPoints = 0; // currentActivePoints in current level
uint8_t PAT_trackMat[8];
uint8_t PAT_wrongPointsMat[8];
uint8_t PAT_wrongPoints = 0;
uint8_t PAT_Matrix[8];
uint32_t PAT_currLvlScore = 0x00;
uint32_t PAT_Score = 0x00;
uint32_t PAT_currTime, PAT_prevTime;

/* Function prototypes */
void PAT_drawScreen(uint8_t * buffer2);
void PAT_Reset(void);
void PAT_EndGameReset(void);
void PAT_createPattern(void);

/*@desc: 
This function is used as a task for showing the pattern phase
@param:
no param
@return:
void
*/
void PAT_ShowPatternFunction() {
  PAT_prevTime = millis();
  memset(PAT_Matrix, 0, sizeof(PAT_Matrix));
  PAT_createPattern();

  while (1) {
    DRAW_Screen(PAT_Matrix);

    PAT_currTime = millis();
    if (PAT_currTime - PAT_prevTime > 3000) {
      state = TRACKPATTERN_STATE;

      break;
    }
  }
}

/*@desc: 
This function is used as task for track of the pattern phase
@param:
no param
@return:
void
*/
void PAT_TrackPatternFunction() {
  uint32_t currTime = 0, prevTime = 0;

  while (1) {
    uint8_t row = checkRow();
    uint8_t col = checkColumn();
    currTime = millis();

    /* if we point the laser towards the matrix */
    if (row != 9 && col != 9) {
      if (PAT_Matrix[row] & (1 << 7 - col)) {
        /* if we kept track of row,col point */
        if (PAT_trackMat[row] & (1 << 7 - col)) {
          ; /* do nothing */
        } else {
          /* keep track of it */
          PAT_trackMat[row] |= (1 << 7 - col);
          PAT_currLvlScore++;
        }
      } else {
        if (PAT_wrongPointsMat[row] & (1 << 7 - col)) {
          // pointed to a square already flickering wrong
          // nothing to do
        } else {
          PAT_wrongPoints++;
          if (PAT_wrongPoints > 4) /* end current game session */ {
            state = END_STATE;
            PAT_Score += PAT_currLvlScore;
            break;
          }
          PAT_wrongPointsMat[row] |= (1 << 7 - col);
        }
      }
    }
    PAT_drawScreen(PAT_trackMat);
    if (currTime - prevTime > 40) {
      PAT_drawScreen(PAT_wrongPointsMat);
      prevTime = currTime;
      currTime = millis();
    }
    if (PAT_currLvlScore >= PAT_lvlPoints) /* if all the Pattern points have been pointed */ {
      PAT_Score += PAT_lvlPoints;
      PAT_Reset();
      state = SHOWPATTERN_STATE;
      break;
    }
  }
}

/*@desc: 
This function is used to reset all variables related to PAT
@param:
no param
@return:
void
*/
void PAT_Reset() {
  PAT_wrongPoints = 0x00;
  PAT_currLvlScore = 0x00;
  PAT_lvlPoints = 0x00;
  memset(PAT_trackMat, 0x00, 0x08);
  memset(PAT_wrongPointsMat, 0x00, 0x08);
}

/*@desc: 
This function is used to reset the score and the start number of points to 6
@param:
no param
@return:
void
*/
void PAT_EndGameReset() {
  currNrOfPoints = 6; /* reset nr of points per pattern */
  PAT_Score = 0x00; /* reset score */
}

/*@desc: 
This function is used to draw the matrix with buffer2 
@param:
uint8 *buffer2 -> Buffer will be drawn on the matrix 
@return:
void
*/
void PAT_drawScreen(uint8_t * buffer2) {
  /* Turn on each row in series */
  for (byte i = 0; i < 8; i++) // count next row
  {
    digitalWrite(rows[i], HIGH); //initiate whole row
    for (byte a = 0; a < 8; a++) // count next row
    {

      digitalWrite(cols[a], ((~buffer2[i] >> a) & 0x01)); // initiate whole column

      delayMicroseconds(50);

      digitalWrite(cols[a], 1); // reset whole column
    }
    digitalWrite(rows[i], LOW); // reset whole row
    // otherwise last row will intersect with next row
  }
}

/*@desc: 
This function is used to create a random pattern
@param:
no param
@return:
void
*/
void PAT_createPattern() {
  uint8_t PosCol = 0x00;
  uint8_t PosRow = 0x00;
  uint8_t currPosRow = 0x00;
  uint8_t currPosCol = 0x00;
  PAT_lvlPoints = 0x00;
  currPosRow = random(7);
  currPosCol = random(7);
  PAT_Matrix[currPosRow] = (1 << (7 - currPosCol));
  PAT_lvlPoints++;
  for (uint8_t index = 0; index < currNrOfPoints - 1; index++) {
    uint8_t isOk = 1;
    /* Next position:
       rand == 0 -> UP
       rand == 1 -> DOWN
       rand == 2 -> SAME
       rand == 0 -> LEFT 
       rand == 1 -> RIGHT
       rand == 2 -> SAME
     */
    do {

      /* Select Row dir */
      PosRow = random(2);
      switch (PosRow) {
      case 0:
        currPosRow--; //UP
        break;
      case 1:
        currPosRow++;
      default:
        break;
      }

      if (currPosRow >= 8) {
        isOk = 0x00;
      } else {
        isOk = 0x01;
      }
    } while (isOk == 0x00);
    isOk = 0x00;
    /* Select column dir */
    do {
      PosCol = random(2);
      switch (PosCol) {
      case 0:
        currPosCol--; //LEFT
        break;
      case 1:
        currPosCol++;
        break;
      default:
        break;
      } /* END SWITCH */

      if (currPosCol >= 8) {
        isOk = 0x00;
      } else {
        if ((PAT_Matrix[currPosRow] & (1 << (7 - currPosCol)))) // if we overlap other point
        {
          isOk = 0x00; // not ok, pick another point
        } else {
          isOk = 0x01;
        }
      }

    } while (isOk == 0x00);

    /* Now we have the next point */

    if ((PAT_Matrix[currPosRow] & (1 << (7 - currPosCol)))) // if we overlap other point
    {
      ;
    } else {
      PAT_lvlPoints++;
    }
    PAT_Matrix[currPosRow] |= (1 << (7 - currPosCol)); //update pattern matrix 
  }

  /* Add a point for each level passed */
  currNrOfPoints++;
}

#endif /* GUARD __PATTERN_H_ */
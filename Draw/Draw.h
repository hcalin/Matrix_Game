#ifndef DRAW_H
# define DRAW_H


/* Defines */
#define timeMuxLoop 140 // time per countdown digit
#define countDownLen 24 // length of the CountDown buffer

/* Function prototypes */
void  DRAW_Screen(byte buffer2[]);
void  DRAW_loopFadeScreen();
void  DRAW_UpdateMatrix();
void  DRAW_fadeScreen();


/* variables */
uint8_t  mainLine = 0;
uint8_t  finishedFade = 0x00;
uint8_t  matrixIndex = 0x00;
uint32_t timer,timerPrev;




byte CountDown[] = { 
  B00000000,B00111100,B00000100,B00011100,B00000100,B00000100,B00111100,B00000000, //3
  B00000000,B00111100,B00100100,B00000100,B00111100,B00100000,B00111100,B00000000, //2
  B00000000,B00001000,B00011000,B00001000,B00001000,B00001000,B00001000,B00000000, // 1
  B00000000,B00111100,B00100100,B00100100,B00100100,B00100100,B00111100,B00000000, // 0
//  B11111111,B11000011,B11011011,B11011011,B11011011,B11011011,B11000011,B11111111, // ~0
//  B00000000,B00111100,B00100100,B00100100,B00100100,B00100100,B00111100,B00000000, // 0
//  B11111111,B11000011,B11011011,B11011011,B11011011,B11011011,B11000011,B11111111 // ~0
};


/*@desc: 
This function realizes the drawing of the fading and the countdown (acts like a task)
@param:
no param
@return:
void
*/
void DRAW_CountDownStateFunction()
{
      DRAW_loopFadeScreen();
      matrixIndex += 8;
      if ( matrixIndex > countDownLen) {
        matrixIndex = 0x00;
		state = SHOWPATTERN_STATE;
      }
}

/*@desc: 
This function realizes the drawing of the fading at the begging of the countdown
@param:
no param
@return:
void
*/
void DRAW_loopFadeScreen()
{
  while (1)
  {
   uint32_t currTime = millis ();
  
  if (currTime - prevTime > timeMuxLoop ) 
  {
    DRAW_UpdateMatrix();
    if (finishedFade == 0x01)
    {
      finishedFade = 0x00; 
      break; 
    }
    prevTime = currTime;
  }
  DRAW_fadeScreen();

  DRAW_Screen(CountDown + matrixIndex);
  }
}

void DRAW_UpdateMatrix()
{
  mainLine++;
  if (mainLine > 9)
  {
    mainLine = 0;
    finishedFade = 0x01;
  }
}


/*@desc: 
This function realizes the drawing of the fading at the begging of the game 
@param:
no param
@return:
void
*/
void  DRAW_fadeScreen()
 { 
   /* Turn on each row in series */
   
    for (byte i = mainLine - 2; i <= mainLine; i++)        /* count next row */
     {
        digitalWrite(rows[i], HIGH);    /* initiate whole row */
        for (byte a = 0; a < 8; a++)    /* count next row */
        {
          digitalWrite(cols[a], LOW); /* initiate whole column */
          if ( i == mainLine)
          delayMicroseconds(300);       /* uncoment deley for diferent speed of display */
          else if (i == mainLine - 1)
          delayMicroseconds(230);
          else if ( i == mainLine - 2) 
          delayMicroseconds(50);
          digitalWrite(cols[a], 1);      /* reset whole column */
        }
        digitalWrite(rows[i], LOW);     /* reset whole row */
        /* otherwise last row will intersect with next row */
    }
}

/*@desc: 
This function is used to draw a buffer 
@param:
buffer2[] - the buffer that will be drawn into the matrix 
@return:
void
*/
void  DRAW_Screen(byte buffer2[])
 { 
   /* Turn on each row in series */
    for (byte i = 0; i < 8; i++)        /* count next row */
     {
        digitalWrite(rows[i], HIGH);    /* initiate whole row */
        
		for (byte a = 0; a < 8; a++)    /* count next row */
        {

			digitalWrite(cols[a], (~buffer2[i] >> a) & 0x01); /* initiate whole column */  

			delayMicroseconds(200);       /* uncomment delay for diferent speed of display */

			digitalWrite(cols[a], 1);      /* reset whole column */
        }
        
		digitalWrite(rows[i], LOW);     /* reset whole row */
        /* otherwise last row will intersect with next row */
    }
 }

/*@desc: 
This function is used to draw a digit in the right of the screen (via a shift)
@param:
buffer[] - to be shifted and drawn
@return:
void
*/
void DRAW_rightOnScreen(byte buffer[])
{
	byte digit[8];
	memcpy(digit, buffer, 0x08);
	
	
	for (uint8_t index; index < 8 ; index ++)
	{
		digit[index] >>= 3;
	}
	
	DRAW_Screen(digit);
}

#endif
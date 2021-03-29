#ifndef READING_H /* GUARD __READING_H*/
#define READING_H

uint32_t screen[8] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
}; /* bits displayed on the LED matrix */
int32_t colThr[8] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};
/* threshold values used in CheckColumn()
 * the thresholds are adjusted in
 * adjustThreshCol() /*

 
 
/*@desc: 
This function is used to determine the column that the laser pointed
@param:
no param
@return:
the column that the laser pointed; values 0-7; 9 means invalid(the laser isn't pointing at the matrix) 
*/
int checkColumn() {
  int val[8]; /* brightness per column */
  int signal; /* signal that contains the pointed column number */

  /* Clear charges that were left; make all anodes output and low set all cathodes low */
  DDRD |= B11111100;
  DDRB |= B00000011;
  PORTD &= B00000011;
  PORTB &= B11111100;

  /* make all cathodes output high */
  DDRC = B00111111;
  PORTC = B00111111;
  DDRB |= B00011000;
  PORTB |= B00011000;

  _delay_us(100);

  /* make all cathodes input and low; */
  DDRC &= B11000000;
  DDRB &= B11100111;

  PORTC &= B11000000;
  PORTB &= B11100111;

  /* read analog values for all anodes */
  for (int col = 0; col < 8; col++) {
    val[col] = analogRead(col);
  }

  for (int i = 0; i < 8; i++) {
    Serial.print(val[i]);
    Serial.print(',');
  }
  Serial.println("");

  /* invalid returned column */
  signal = 9;

  /* if the differences are bigger than 20 the column is pointed by a laser pointer */
  for (int col = 0; col < 8; col++) {
    if (colThr[col] - val[col] > 20) {
      signal = col;
      break;
    }
  }

  /* Return to OUTPUT*/
  PORTC |= 0b00111111;
  PORTB |= B00011000;
  DDRD = 0xFF;
  DDRC = 0xFF;
  DDRB = 0xFF;

  return (signal);

}

/*@desc: 
This function is used to determine the row that the laser pointed
@param:
no param
@return:
the row that the laser pointed
*/
int checkRow() {
  uint8_t signal;

  /* Apply reverse voltage, charge up the pin and led capacitance make all cathodes output high */
  DDRC = B00111111;
  PORTC = B00111111;
  DDRB |= B00011000;
  PORTB |= B00011000;

  /* Set all anodes output low */
  DDRD |= B11111100;
  PORTD &= B00000011;
  DDRB |= B00000011;
  PORTB &= B11111100;

  /* wait for a while to charge */
  _delay_us(100);

  /* Isolate the pin connected to cathods */
  DDRD &= B00000011; // make N0-N5 INPUT
  DDRB &= B11111100; // make N6 and N7 INPUT

  /* Turn off internal pull-up resistor */
  PORTD &= B00000011; // make N0-N5 LOW
  PORTB &= B11111100; // make N6 and N7 LOW  

  signal = 9;
  
  /* measure how long it takes for cathodes to become low; init values to  100 */
  int val[8] = {
    100,
    100,
    100,
    100,
    100,
    100,
    100,
    100
  };
  for (int cnt = 0; cnt < 130; cnt++) { //130 is an adjustable threshold depending on your matrix 
    for (int r = 0; r < 8; r++) {
      if (digitalRead(2 + r) == LOW) {
        val[r] = cnt;
      }
    }
  }
  for (int r = 0; r < 8; r++) {
    if (val[r] < 125) {
      signal = r;
      break;
    }
  }

  DDRD = 0xFF;
  DDRC = 0xFF;
  DDRB = 0xFF;
  return (signal);
}

/*@desc: 
This function is used to determine the coeficient for the outdoor lighting using reverse biased method
@param:
no param
@return:
void
 */
void adjustThreshCol() {
  /* set all anodes output low */
  DDRD |= B11111100;
  DDRB |= B00000011;
  PORTD &= B00000011;
  PORTB &= B11111100;

  /* make all cathodes output high */
  DDRD |= B11111100;
  DDRB |= B00011000;
  PORTD |= B11111100;
  PORTB |= B00011000;

  _delay_us(10);

  /* make all cathodes input and low; */
  DDRC &= B11000000;
  DDRB &= B11100111;

  PORTC = B11000000;
  PORTB &= B11100111;

  _delay_ms(30); // wait for a while to discharge

  for (int x = 30; 1; x--) {
    for (int i = 0; i < 50; i++)
      /* read analog values at all anodes */
      for (int col = 0; col < 8; col++) {
        colThr[col] = analogRead(col);
      }
    Serial.println(colThr[1]);
    _delay_ms(500);

  }

  /* Set all ports to output again */
  DDRD = 0xFF;
  DDRC = 0xFF;
  DDRB = 0xFF;
}

#endif /* GUARD __READING_H*/
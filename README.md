# Matrix_Game
Video: 
https://vimeo.com/467857999

Please note the steps of the game from the video.
1. Scrolling from right to left "Memory Game" until the laser touches the matrix
2. When this happens the countdown will start
3. A random 6 piece pattern will be shown
4. Tracking the pattern
5. Add +1 piece to the pattern and show a random pattern.
6. Observe that wrong pointed LEDs will flick. If the wrong points > 4 then show score and go to idle state.

PCB layout:
![image](https://user-images.githubusercontent.com/22204258/112875142-783e2980-90cc-11eb-86ac-03882c94d4ce.png)


State machine design:
![image](https://user-images.githubusercontent.com/22204258/112875175-84c28200-90cc-11eb-86e6-267823f9fdb0.png)

The interesting fact is that I don't use any photoreceptors. The matrix is connected to the microcontroller via resistors. I use a diode inverse biased algorithm to read the amount of lights on the LEDs, so a constant reading/writing takes place.

You can see my code and further details on github. My username is hcalin. This was my final project for the bachelor degree at Telecommunication Faculty in Cluj.

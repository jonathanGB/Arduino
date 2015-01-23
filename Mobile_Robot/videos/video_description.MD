Video Description
=================


 Final Project Video
 -------------------

--> This was the day of the presentation of our physics project to the whole cégep. There was a competition for the best project of the day with the other teams in many scientific areas, which we won! 

--> So, at the beginning the robot turns on itself and emits red light (part of a RGB LED). When it spots something within 70cm, it stops, does a little light show (a "chenillard"); after 1.5 seconds, another reading is done with the distance sensor. If nothing is detected, it was just noise (or maybe a foot moving in front), so it keeps spinning and emitting red. Otherwise, it goes towards the target, blinking the red LED. When it detects the target to be somewhat close, it slows down, and starts blinking blue light. Finally, when a collision is detected (with the flexion sensor), engines are shut off and the RGB LED blinks green!





 Flexion Sensor Demo Video
 -------------------------

--> This is the flexion sensor we used on our mobile robot adapted to its environment. It is basically a variable resistor which has a 10 kOhms nominal resistance, but when flexed, its resistance diminishes.

--> This video was to prove that the resistance was decreasing, and not increasing, when flexed. To do this, we put a LED in a series circuit with the flexion sensor, and an Arduino Uno Board, which would power the circuit with 5V DC. According to Ohm's Law ( V = R * I --> I = V / R where I = Intensity in Ampères, V = Voltage in Volts and R = Resistance in Ohms), if we had a constant voltage (5V), the only way intensity could change was if the resistance of the circuit was changing. Here, the intensity is, therefore, inversely proportional: if the resistance decreases, the intensity increases, and vice versa. When flexing, the LED was getting brighter, which proved that the resistance was really decreasing when flexed.

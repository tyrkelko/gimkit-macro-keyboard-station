# gimkit-macro-keyboard-station

With this Arduino pro micro and 4 arcade push buttons you can create a custom USB arcade style macro keyboard to play a GimKit game.

This was a nice project for an internal expo, however, the kids enjoyed playing with it as well.

The arcade button used included LEDs and were in the colors of the GimKit game buttons so it was easy to understand how to use them for anyone who gave it a try.

Buttons switches are connected to a common ground in the Arduino board and to pins 2, 3, 4 and 5 for red, yellow, green and blue accordingly.

Buttons LEDs were connected via a 2N2222 transistor acting as a switch so the transistor base was connected via 10Kohm resistors to pins 6, 7, 8 and 9 for red, yellow, green and blue accordingly.

2N2222 transistors collectors were all connected to 5v RAW output of the Arduino pro micro and each transistor emitter was connected to the LED cathode via a 220ohm resistor.

LEDs cathodes were connected to ground.

To use the keyboard, connect the Arduino pro micro to a computer via USB data cable. They Arduino pro micro will be identified as a keyboard.

As of building this kit, once a GimKit game begins and questions are being displayed, click on each button produces a keyboard press on the numbers 1-4 according to the order of the answers (red, yellow, green and blue), and sets to press "Enter" after either 5 seconds or any other key being released.

The lights organ displays some animation of lights as long as there is no key press and it can be configured by an array of uint8_t representing a binary for determining which lights are on or off and amultiplier for determining the how long to wait until the next light value is executed.

![connections_protoboard_with_arduino](https://user-images.githubusercontent.com/6322663/206876075-617945d2-a57c-4e6d-b7d5-52e39f43e36b.jpeg)


https://user-images.githubusercontent.com/6322663/206876371-5cf26914-8652-40ed-a2c9-f144a8b56528.mp4


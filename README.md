# arduflower
Arduino based moonflower disco light

I modified a cheap disco light I got from amazon to improve it. It looks much better with some vapour / smoke in the air. This video was in a moderately clear room. The modfied light is brighter than the original and has more spread too, so it works really nicely in my living room!

I used a WS2812b 8x8 LED matrix which is attached to a 3D printed piece so it slots in where the old LED board used to be. I have used the original PSU that came in the light, and a boost converter to bring it up to a solid 5v as it was only putting out 4.2v. This is powering an Arudino Nano running some FastLED matrix code which I have been using to make moonflower type patterns for beam effects. I have made around 30 patterns so far. I have also used the original potentiometer to adjust the speed of the patterns so its possible to match it to music somewhat.

![PrototypeImage](images/arduflower_prototype.jpg?raw=true)

I was working on a sound reactive mode, but as this is my first Arduino project from scratch that part is stumping me for now. I have the input side all worked out using a voltage divider for a line in input and get what look like useful readings from the analog input, but am unsure how to make a BPM detector that would work alongside my code for the moonflower patterns. I will keep working on it but if anyone can suggest how to do it I would be very grateful!

Parts used:
RGBW Airship disco light - https://amzn.to/2XGAL9M
8x8 2812b LED matrix - https://amzn.to/2XFtiHX
Arduino Nano - Listing is for 3 - https://amzn.to/2XFu1Jb
5v boost converter - https://amzn.to/2X9dmPl

*Links are amazon associate links which means I get a little tip if you use them, you pay no more than usual by doing this.

When tuning the boost converter, do so BEFORE connecting to the main circuit. I found mine was set to 24v when I got it! Adjust and check with a meter as the display may not be 100% accurate.

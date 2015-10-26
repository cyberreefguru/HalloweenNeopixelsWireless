/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */

#include "HalloweenWirelessServer.h"

void test();
void selectCrossover();

/**
 *
 */
void setup()
{

    Serial.begin(115200);
    printf_begin();
    printf("\n\rNeoPixelWirelessServer\n\r");

	randomSeed(analogRead(0));
	randomSeed(analogRead(0));

    if( wireless.initialize(MODE_SERVER) )
    {
    	Serial.println(F("Server initialized."));
    }
    else
    {
    	Serial.println(F("Error initializing server."));
    	Helper::error(ERROR_WIRELESS);
    }
}

/**
 *
 */
void loop()
{
	uint8_t * commandBuffer = wireless.getCommandBuffer();

	cmdFade = (fade_t *) commandBuffer;
    cmdFade->command = CMD_FADE;
    cmdFade->direction = UP;
    cmdFade->fadeIncrement = 1;
    cmdFade->fadeTime = 10;
    cmdFade->color = ORANGE;
    wireless.sendCommand();
    delay(10000);

    cmdFade->direction = DOWN;
    wireless.sendCommand();
    delay((255*10)+100); // wait while we fade plus some small pad

	selectCrossover();

	cmdFade = (fade_t *) commandBuffer;
    cmdFade->command = CMD_FADE;
    cmdFade->direction = UP;
    cmdFade->color = PURPLE;
    cmdFade->fadeIncrement = 1;
    cmdFade->fadeTime = 10;
    wireless.sendCommand();
	delay(10000);

    cmdFade->direction = DOWN;
    wireless.sendCommand();
    delay((255*10)+100); // wait while we fade plus some small pad

    cmdFade->direction = UP;
    cmdFade->color = RED;
    wireless.sendCommand();
	delay(10000);

	selectCrossover();

	cmdFade = (fade_t *) commandBuffer;
    cmdFade->command = CMD_FADE;
    cmdFade->direction = DOWN;
    cmdFade->color = RED;
    cmdFade->fadeIncrement = 1;
    cmdFade->fadeTime = 10;
    wireless.sendCommand();
    delay((255*10)+100); // wait while we fade plus some small pad

    cmdFade->direction = UP;
    cmdFade->color = GREEN;
    wireless.sendCommand();
	delay(10000);

    cmdFade->direction = DOWN;
    wireless.sendCommand();
    delay((255*10)+100); // wait while we fade plus some small pad

    selectCrossover();

	cmdFade = (fade_t *) commandBuffer;
    cmdFade->command = CMD_FADE;
    cmdFade->direction = UP;
    cmdFade->color = BLUE;
    cmdFade->fadeIncrement = 1;
    cmdFade->fadeTime = 10;
    wireless.sendCommand();
	delay(10000);

	selectCrossover();

	cmdFade = (fade_t *) commandBuffer;
    cmdFade->command = CMD_FADE;
    cmdFade->direction = DOWN;
    cmdFade->color = BLUE;
    cmdFade->fadeIncrement = 1;
    cmdFade->fadeTime = 10;
    wireless.sendCommand();
    delay((255*10)+100); // wait while we fade plus some small pad

	selectCrossover();

}


/**
 * Randomly determines if there will be
 * lightning or strobe.
 */
void selectCrossover()
{
	if( random(0,100) < 35 )
	{
	     cmdStrobe = (strobe_t *) wireless.getCommandBuffer();
	     cmdStrobe->command = CMD_STROBE;
	     cmdStrobe->duration = 0;
	     cmdStrobe->onColor = WHITE;
	     cmdStrobe->offColor = BLACK;
	     cmdStrobe->onTime = 25;
	     cmdStrobe->offTime = 25;
	     wireless.sendCommand();
	     delay(random(1000, 3000));
	}
}


/**
 * Tests all commands
 */void test()
{
	 uint8_t * commandBuffer = wireless.getCommandBuffer();

     // Test fill
     printf("Fill-White\n\r");
     cmdFill = (fill_t *)commandBuffer;
     cmdFill->command = CMD_FILL;
     cmdFill->color = WHITE;
     wireless.sendCommand();
     delay(4000);
     printf("Fill-Red\n\r");
     cmdFill->color = RED;
     wireless.sendCommand();
     delay(4000);
     printf("Fill-Black\n\r");
     cmdFill->color = BLACK;
     wireless.sendCommand();
     delay(4000);

     // Test Fill Pattern
     printf("Fill Pattern\n\r");
     cmdFillPattern = (fill_pattern_t *) commandBuffer;
     cmdFillPattern->command = CMD_FILL_PATTERN;
     cmdFillPattern->pattern = 0x03;
	 cmdFillPattern->onColor = CYAN;
	 cmdFillPattern->offColor = CRGB::Brown;
	 wireless.sendCommand();
	 delay(4000);

    // Test Pattern
    printf("Pattern\n\r");
    cmdPattern = (pattern_t *) commandBuffer;
    cmdPattern->command = CMD_PATTERN;
    cmdPattern->repeat = 0;
    cmdPattern->pattern = 0x33;
    cmdPattern->direction = LEFT;
    cmdPattern->onColor = RED;
    cmdPattern->offColor = WHITE;
    cmdPattern->onTime = 250;
    cmdPattern->offTime = 250;
    wireless.sendCommand();
    delay(4000);

    // Test Runway
    printf("Wipe\n\r");
    cmdWipe = (wipe_t *) commandBuffer;
    cmdWipe->command = CMD_WIPE;
    cmdWipe->pattern = 2;
    cmdWipe->direction = LEFT;
    cmdWipe->onColor = RED;
    cmdWipe->offColor = BLUE;
    cmdWipe->onTime = 100;
    cmdWipe->offTime = 0;
    cmdWipe->clearAfter = true;
    cmdWipe->clearEnd = true;
    wireless.sendCommand();
    delay(6000);

    cmdWipe->onTime = 50;
    wireless.sendCommand();
    delay(4000);

    cmdWipe->onTime = 25;
    wireless.sendCommand();
    delay(4000);

    // Test bounce
    printf("Bounce - Clear After\n\r");
    cmdBounce = (bounce_t *) commandBuffer;
    cmdBounce->command = CMD_BOUNCE;
    cmdBounce->repeat = 0;
    cmdBounce->pattern = 0x01;
    cmdBounce->direction = LEFT;
    cmdBounce->onColor = RED;
    cmdBounce->offColor = BLUE;
    cmdBounce->onTime = 25;
    cmdBounce->offTime = 0;
    cmdBounce->bounceTime = 0;
    cmdBounce->clearAfter = true;
    cmdBounce->clearEnd = true;
    wireless.sendCommand();
    delay(6000);

    // Test bounce
    printf("Bounce - No Clear After\n\r");
    cmdBounce->onColor = GREEN;
    cmdBounce->offColor = WHITE;
    cmdBounce->clearAfter = false;
    cmdBounce->clearEnd = false;
    wireless.sendCommand();
    delay(6000);

    // Clear after test
    cmdFill = (fill_t *)commandBuffer;
    cmdFill->command = CMD_FILL;
    cmdFill->color = BLACK;
    wireless.sendCommand();

    // Test Middle
    printf("Middle\n\r");
    cmdMiddle = (middle_t *) commandBuffer;
    cmdMiddle->command = CMD_MIDDLE;
    cmdMiddle->repeat = 0;
    cmdMiddle->direction = LEFT;
    cmdMiddle->onColor = RED;
    cmdMiddle->offColor = BLUE;
    cmdMiddle->onTime = 100;
    cmdMiddle->offTime = 0;
    cmdMiddle->clearAfter = true;
    cmdMiddle->clearEnd = true;
    wireless.sendCommand();
    delay(2000);
    cmdMiddle->direction = RIGHT;
    wireless.sendCommand();
    delay(2000);

     // Test random flash
     printf("Random Flash - Red/Black\n\r");
     cmdRandomFlash = (random_flash_t *) commandBuffer;
     cmdRandomFlash->command = CMD_RANDOM_FLASH;
     cmdRandomFlash->onColor = RED;
     cmdRandomFlash->offColor = BLACK;
     cmdRandomFlash->onTime = 25;
     cmdRandomFlash->offTime = 0;
     wireless.sendCommand();
     delay(5000);

     // Test random flash
     printf("Random Flash - White/Red\n\r");
     cmdRandomFlash = (random_flash_t *) commandBuffer;
     cmdRandomFlash->command = CMD_RANDOM_FLASH;
     cmdRandomFlash->onColor = WHITE;
     cmdRandomFlash->offColor = RED;
     cmdRandomFlash->onTime = 25;
     cmdRandomFlash->offTime = 0;
     wireless.sendCommand();
     delay(5000);

     // Test fade
     printf("Fade - Up/White\n\r");
     cmdFade = (fade_t *) commandBuffer;
     cmdFade->command = CMD_FADE;
     cmdFade->direction = UP;
     cmdFade->fadeIncrement = 1;
     cmdFade->fadeTime = 25;
     cmdFade->color = WHITE;
     wireless.sendCommand();
     delay((255*25)+500); // wait while we fade plus some small pad

     printf("Fade - Down/Red\n\r");
     cmdFade = (fade_t *) commandBuffer;
     cmdFade->command = CMD_FADE;
     cmdFade->direction = DOWN;
     cmdFade->fadeIncrement = 1;
     cmdFade->fadeTime = 25;
     cmdFade->color = RED;
     wireless.sendCommand();
     delay((255*25)+500); // wait while we fade plus some small pad

     // Test strobe
     printf("Strobe - White/Black\n\r");
     cmdStrobe = (strobe_t *) commandBuffer;
     cmdStrobe->command = CMD_STROBE;
     cmdStrobe->duration = 0;
     cmdStrobe->onColor = WHITE;
     cmdStrobe->offColor = BLACK;
     cmdStrobe->onTime = 25;
     cmdStrobe->offTime = 25;
     wireless.sendCommand();
     delay(5000);

     printf("Strobe - Green/Blue\n\r");
     cmdStrobe = (strobe_t *) commandBuffer;
     cmdStrobe->command = CMD_STROBE;
     cmdStrobe->duration = 0;
     cmdStrobe->onColor = GREEN;
     cmdStrobe->offColor = BLUE;
     cmdStrobe->onTime = 25;
     cmdStrobe->offTime = 25;
     wireless.sendCommand();
     delay(5000);

     // Test lightning
     printf("Lightning - White/Black\n\r");
     cmdLightning = (lightning_t *) commandBuffer;
     cmdLightning->command = CMD_LIGHTNING;
     cmdLightning->onColor = YELLOW;
     cmdLightning->offColor = BLACK;
     wireless.sendCommand();
     delay(2000);


    // Test rainbow
    printf("Rainbow\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 0;
    wireless.sendCommand();
    delay(4000);

    // Test rainbow
    printf("Rainbow with glitter\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 80;
    cmdRainbow->glitterColor = WHITE;
    wireless.sendCommand();
    delay(4000);

    // Test rainbow
    printf("Rainbow with blue glitter\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 80;
    cmdRainbow->glitterColor = BLUE;
    wireless.sendCommand();
    delay(4000);

    // Test rainbow fade
    printf("Rainbow Fade\n\r");
    cmdRainbowFade = (rainbow_fade_t *) commandBuffer;
    cmdRainbowFade->command = CMD_RAINBOW_FADE;
    wireless.sendCommand();
    delay(4000);

    // Test confetti
    printf("Confetti - 10\n\r");
    cmdConfetti = (confetti_t *) commandBuffer;
    cmdConfetti->command = CMD_CONFETTI;
    cmdConfetti->color = RED;
    cmdConfetti->numOn = 10;
    wireless.sendCommand();
    delay(4000);

    // Test confetti
    printf("Confetti - 5\n\r");
    cmdConfetti = (confetti_t *) commandBuffer;
    cmdConfetti->command = CMD_CONFETTI;
    cmdConfetti->color = GREEN;
    cmdConfetti->numOn = 5;
    wireless.sendCommand();
    delay(4000);

    // Test cylon
    printf("Cylon - Red\n\r");
    cmdCylon = (cylon_t *) commandBuffer;
    cmdCylon->command = CMD_CYLON;
    cmdCylon->repeat = 0;
    cmdCylon->color = RED;
    wireless.sendCommand();
    delay(4000);

    // Test BPM
    printf("BPM\n\r");
    cmdBPM = (bpm_t *) commandBuffer;
    cmdBPM->command = CMD_BPM;
    wireless.sendCommand();
    delay(4000);

    // Test Juggle
    printf("Juggle\n\r");
    cmdJuggle = (juggle_t *) commandBuffer;
    cmdBPM->command = CMD_JUGGLE;
    wireless.sendCommand();
    delay(4000);

}

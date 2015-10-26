/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */
#include "HalloweenWirelessClient.h"

void commandMode();
void parseCommand();

NeopixelWrapper controller = NeopixelWrapper();
//WirelessWrapper wireless = WirelessWrapper();
//ClientConfig config = ClientConfig();

volatile uint32_t endTime;

/**
 *
 */
void setup()
{
	Serial.begin(115200);
	printf_begin();

	Serial.println(F("\n\n** NeoPixelWirelessClient ** \n\n"));

	if( wireless.initialize(MODE_CLIENT) == false )
	{
		Serial.println(F("\nError configuring wireless module"));
		Helper::error( ERROR_WIRELESS );
	}
	else
	{
		Serial.println(F("\nWireless Module Configured."));
	}

	if (controller.initialize(NUM_PIXELS, DEFAULT_INTENSITY) == false)
	{
		Serial.println(F("\nError configuring LED module"));
		Helper::error( ERROR_DRIVER );
	}
	else
	{
		controller.fill(CRGB::White, true);
		delay(500);
		controller.fill(CRGB::Black, true);
		Serial.println(F("\nLED Module Configured."));
	}

	Serial.println(F("\nSetup Complete.\n"));

}

/**
 *
 */
void loop()
{
	// Wait for command
	if (isCommandAvailable())
	{
		parseCommand();

	} // end commandAvailable

	if( Serial.available() )
	{
		commandMode();
	}

} // end loop

/**
 * Handles serial commands and changing configuration
 */
void commandMode()
{
	uint8_t id = 0;

	// Read first character - discard since it just gets us into command mode
	char c = toupper(Serial.read());
	Serial.println(F("COMMAND MODE:"));
	Serial.println(F("C - Change Node ID"));
	Serial.println(F("D - Dump Configuration"));
	Serial.println(F("E - exit"));
	while(!Serial.available() ){}

	c = toupper(Serial.read());
	switch( c )
	{
	case 'C':
		Serial.print(F("** Change Node Id **\nCurrent "));
		wireless.dump();

		Serial.print(F("\nEnter 1 digit node ID: "));
		while(!Serial.available() ){}
		id = Serial.read();
		Serial.println( (char)id ); // echo what the user typed
		Serial.println(F("\nProcessing...\n"));
		if( id >= '0' && id <= '9' )
		{
			if( wireless.setNodeId( id ) )
			{
				Serial.print(F("Node ID accepted: "));
				Serial.println( wireless.getNodeId(), HEX);
				wireless.dump();
			}
			else
			{
				Serial.println(F("Error saving configuration information."));
			}
		}
		else
		{
			Serial.print(F("Illegal node value entered:"));
			Serial.println(id);
		}
		break;
	case 'D':
		wireless.dump();
		break;
	case 'E':
		break;
	}
}

void parseCommand()
{
	volatile uint8_t *buf = getCommandBuffer();

	Helper::dumpBuffer( (uint8_t *)buf,  MAX_COMMAND_SIZE);
	setCommandAvailable(false);
	printf("COMMAND RECEIVED: 0x%02x - ", buf[0]);
	switch (buf[0])
	{

	case CMD_FILL:
		printf("FILL\n\r");
		cmdFill = (fill_t *) buf;
		controller.fill(cmdFill->color, true);
		break;
	case CMD_FILL_PATTERN:
		printf("FILL_PATTERN\n\r");
		cmdFillPattern = (fill_pattern_t *) buf;
		controller.fillPattern(cmdFillPattern->pattern, cmdFillPattern->onColor, cmdFillPattern->offColor);
		break;
	case CMD_PATTERN:
		printf("PATTERN\n\r");
		cmdPattern = (pattern_t *) buf;
		controller.pattern(cmdPattern->repeat, cmdPattern->pattern, cmdPattern->direction, cmdPattern->onColor, cmdPattern->offColor, cmdPattern->onTime, cmdPattern->offTime);
		break;
	case CMD_WIPE:
		printf("WIPE\n\r");
		cmdWipe = (wipe_t *) buf;
		controller.wipe(cmdWipe->pattern, cmdWipe->direction, cmdWipe->onColor, cmdWipe->offColor, cmdWipe->onTime, cmdWipe->offTime, cmdWipe->clearAfter, cmdWipe->clearEnd);
		break;
	case CMD_BOUNCE:
		printf("BOUNCE\n\r");
		cmdBounce = (bounce_t *) buf;
		controller.bounce(cmdBounce->repeat, cmdBounce->pattern, cmdBounce->direction, cmdBounce->onColor, cmdBounce->offColor, cmdBounce->onTime, cmdBounce->offTime, cmdBounce->bounceTime, cmdBounce->clearAfter, cmdBounce->clearEnd);
		break;
	case CMD_MIDDLE:
		printf("MIDDLE\n\r");
		cmdMiddle = (middle_t *) buf;
		controller.middle(cmdMiddle->repeat, cmdMiddle->direction, cmdMiddle->onColor, cmdMiddle->offColor, cmdMiddle->onTime, cmdMiddle->offTime, cmdMiddle->clearAfter, cmdMiddle->clearEnd);
		break;
	case CMD_RANDOM_FLASH:
		printf("RANDOM_FLASH\n\r");
		cmdRandomFlash = (random_flash_t *) buf;
		controller.randomFlash(0, cmdRandomFlash->onTime, cmdRandomFlash->offTime, cmdRandomFlash->onColor, cmdRandomFlash->offColor);
		break;
	case CMD_FADE:
		printf("FADE\n\r");
		cmdFade = (fade_t *) buf;
		controller.fade(cmdFade->direction, cmdFade->fadeIncrement, cmdFade->fadeTime, cmdFade->color);
		break;
	case CMD_STROBE:
		printf("STROBE\n\r");
		cmdStrobe = (strobe_t *) buf;
		controller.strobe(cmdStrobe->duration, cmdStrobe->onColor, cmdStrobe->offColor, cmdStrobe->onTime, cmdStrobe->offTime);
		break;
	case CMD_LIGHTNING:
		//	    void lightning(CRGB onColor, CRGB offColor);
		printf("LIGHTNING\n\r");
		cmdLightning = (lightning_t *) buf;
		controller.lightning(cmdLightning->onColor, cmdLightning->offColor);
		break;
	case CMD_RAINBOW:
		printf("RAINBOW\n\r");
		cmdRainbow = (rainbow_t *) buf;
		controller.rainbow(0, cmdRainbow->glitterProbability, cmdRainbow->glitterColor);
		break;
	case CMD_RAINBOW_FADE:
		printf("RAINBOW_FADE\n\r");
		cmdRainbowFade = (rainbow_fade_t *) buf;
		controller.rainbowFade(0);
		break;
	case CMD_CONFETTI:
		printf("CONFETTI\n\r");
		cmdConfetti = (confetti_t *) buf;
		controller.confetti(0, cmdConfetti->color, cmdConfetti->numOn);
		break;
	case CMD_CYLON:
		printf("CYLON\n\r");
		cmdCylon = (cylon_t *) buf;
		controller.cylon(cmdCylon->repeat, cmdCylon->color);
		break;
	case CMD_BPM:
		printf("BPM\n\r");
		cmdBPM = (bpm_t *) buf;
		controller.bpm(0);
		break;
	case CMD_JUGGLE:
		printf("JUGGLE\n\r");
		cmdJuggle = (juggle_t *) buf;
		controller.juggle(0);
		break;
	case CMD_SET_HUE_UPDATE_TIME:
		printf("SET_HUE_UPDATE_TIME\n\r");
		cmdSetHueUpdateTime = (set_hue_update_time_t *) buf;
		controller.setHueUpdateTime(cmdSetHueUpdateTime->updateTime);
		break;
	case CMD_SET_FPS:
		printf("SET_FPS\n\r");
		cmdSetFPS = (set_fps_t *) buf;
		controller.setFramesPerSecond(cmdSetFPS->fps);
		break;
	case CMD_ERROR:
	default:
		printf("ERROR\n\r");
		break;
	}

}


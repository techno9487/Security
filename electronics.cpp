#include "config.h"
#include "electronics.hpp"
#include "wiringPi.h"

void SetOk(bool on)
{
	if(on)
	{
		digitalWrite(GREEN_LED_PIN,HIGH);
	}else{
		digitalWrite(GREEN_LED_PIN,LOW);
	}
}

void SetBuzzer(bool on)
{
	if(on)
	{
		digitalWrite(BUZZER_PIN,HIGH);
	}else{
		digitalWrite(BUZZER_PIN,LOW);
	}
}

void SetAlarm(bool on)
{
	if(on)
	{
		digitalWrite(RED_LED_PIN,HIGH);
	}else{
		digitalWrite(RED_LED_PIN,LOW);
	}
}
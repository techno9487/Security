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

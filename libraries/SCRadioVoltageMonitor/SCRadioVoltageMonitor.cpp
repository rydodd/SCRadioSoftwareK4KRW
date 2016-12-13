/**
* SCRadioVoltageMonitor.cpp - Library for logic for monitoring supply voltage for rig
*
* Copyright (c) 2016 - Richard Young Dodd
*
* Hank Ellis K5HDE created a version of the original software for 
* this rig that included logic for reading and displaying the
* supply voltage for the rig.  This logic is based on his.
* 
* Richard Young Dodd licenses this file to you under the MIT license.
* See the LICENSE file in the project root for more information.
* If you did not receive the 'LICENSE' file with this software
* see <https://opensource.org/licenses/MIT>.
*
* @author Richard Y. Dodd - K4KRW
* @version 1.0.2  12/12/2016.
*/

#include "Arduino.h"

#include "EventManager.h"

#include "SCRadioConstants.h"

#include "SCRadioVoltageMonitor.h"

SCRadioVoltageMonitor::SCRadioVoltageMonitor(EventManager &eventManager,
											int8_t arduinoPinToRead,
											float   voltageCalcMultiplier,
											EventType voltageChangedEventTypeCode,
											int16_t loopsBetweenVoltageChecks): 
			_eventManager(eventManager), 
			_arduinoPinToRead(arduinoPinToRead),
			_voltageCalcMultiplier(voltageCalcMultiplier),
			_voltageChangedEventTypeCode(voltageChangedEventTypeCode),
			_loopsBetweenVoltageChecks(loopsBetweenVoltageChecks),
			// setting this to the loops max value so we immediately check the voltage
			// on startup.
			_loopCounter(loopsBetweenVoltageChecks)
{	
}

void SCRadioVoltageMonitor::loop()
{
	_loopCounter += 1;

	// timing between voltage checks does not need to be precise, so I'm just 
	// using a loop counter and checking the voltage after a set number
	// of loops.  16 bit math is a lot less expensive than the 32 bit math 
	// required when using the 'millis' value.
	if (_loopCounter < _loopsBetweenVoltageChecks)
	{
		return;
	}

	_loopCounter = 0;

	int16_t rigVoltageX10 = getRigVoltageX10AsInt();

	// We won't continue with 'telling the world' unless the voltage actually changed.
	if (rigVoltageX10 == _lastVoltageRead)
	{
		return;
	}

	_lastVoltageRead = rigVoltageX10;

	_eventManager.queueEvent(static_cast<int>(_voltageChangedEventTypeCode), rigVoltageX10);
}

int16_t SCRadioVoltageMonitor::getRigVoltageX10AsInt()
{
	/**
	* VOLTAGE_CALC_MULTIPLIER
	*
	* Number multiplied by voltage read on analog pin 7 to get voltage being
	* supplied to 49er.
	*
	* So, where does this number come from?
	*
	* The Nano can read the voltage on an analog pin.  But, it does not read it
	* directly and it can only read voltages from 0 to 5 Volts.
	*
	* 0 Volts is 0 and 5 Volts is 1023.  So, if you are dealing with 0 to 5 Volts
	* you can multiple the value you read from the pin by 0.049 to get the
	* actual voltage.
	*
	* In the rig, we are using 12 Volts.  This cannot be read directly by the
	* Arduino.  So the VFO circuit has an adjustable voltage divider circuit
	* that is used to bring the 12 Volts down to an acceptable range for
	* sampling by the Arduino.
	*
	* So, as long as the voltage divider keeps the actual voltage being read under
	* 5V we can then compare the known voltage going into the rig with
	* what is read by the Arduino and come up with a multiplier to give us
	* the actual voltage.
	*
	* This number is a bit arbitrary as Hank mentions in his comments.  So, you can
	* use this or something close to it and can then use R8 to fine tune the
	* voltage displayed by the rig to match a known actual supply voltage.
	*
	* In this app's logic, both the 5/1023 and the voltage divider reversal
	* are represented in this constant.
	*
	* So, my Arduino was returning 739 when reading the analog pin.
	* 739 * 0.171070  becomes about 126.  This is 12.6 * 10.
	* I handle the voltage as an integer value multiplied by 10 up until the
	* time I display it because I can only handle an integer in my messaging
	* scheme.  I then devide this value by 10 before I display it so we get
	* 12.6V.
	*
	* Again, the exact value displayed depends on how you adjust R8.  So, adjust
	* R8 until the display is accurate and you are done.
	*/
	return (int16_t)((float)analogRead(_arduinoPinToRead) * _voltageCalcMultiplier);
}
/**
 * SCRadioDDS.cpp - Class for controlling AD9850 DDS
 *
 * Copyright (c) 2016 - Richard Young Dodd
 *
 * Richard Young Dodd licenses this file to you under the MIT license.
 * See the LICENSE file in the project root for more information.
 * If you did not receive the 'LICENSE' file with this software
 * see <https://opensource.org/licenses/MIT>.
 *
 * @author Richard Y. Dodd - K4KRW
 * @version 1.0.3  12/22/2016.
 */

#include "Arduino.h"
#include "SCRadioDDS.h"
#include "Wire.h"

 // Constructor
 // The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioDDS::SCRadioDDS(int8_t ddsWordLoadClockPin,
						int8_t ddsFrequencyUpdatePin,
						int8_t ddsDataPin,
						int8_t ddsResetPin,
						float ddsTuningWord) : _ddsWordLoadClockPin(ddsWordLoadClockPin),
												_ddsFrequencyUpdatePin(ddsFrequencyUpdatePin),
												_ddsDataPin(ddsDataPin),
												_ddsResetPin(ddsResetPin),
												_ddsTuningWord(ddsTuningWord) 
{
	// Don't bother putting any logic here.  Arduino constructors are not.  This section will never run.
	// Put your logic in 'begin() instead and call it after instantiating your object.
}

void SCRadioDDS::begin()
{
	// Initializing pins for sending data to the DDS
	pinMode(_ddsFrequencyUpdatePin, OUTPUT);   
	pinMode(_ddsWordLoadClockPin, OUTPUT);
	pinMode(_ddsDataPin, OUTPUT);
	pinMode(_ddsResetPin, OUTPUT);

	// Initialize the DDS
	pulseHigh(_ddsResetPin);
	pulseHigh(_ddsWordLoadClockPin);
	pulseHigh(_ddsFrequencyUpdatePin);  // this pulse enables serial mode on the AD9850 - Datasheet page 12.`
  
  SCRadioDDS::_beginHasRun = true;
}

void SCRadioDDS::sendFrequencyToDDS(int32_t frequency)
{
	// I want to be sure this DDS object is initialized before it does anything.
	// So, if begin() has not run, I'm running it before sending frequency info to the DDS
	if (!_beginHasRun)
	{
		begin();
	}
	
	// The frequency is multiplied by the DDS tuning word before sending it to the DDS
	// The DDS tuning word allows us to fine tune the frequency of the DDS
	// If it just took the frequency directly, it would be harder to adjust the DDS 
	// frequency precisely.
	int32_t freq = (uint32_t) (((float)frequency * _ddsTuningWord));
//	
//	Serial.print("Frequency : ");
//	Serial.println(frequency);
	
	// The following loop does the work of sending the frequency to the DDS
	// It sends the first 32 bits (hence the 32)
	//
	// It only sends the right most bit each time info is sent.
	//
	// The 'freq >>= 1' logic shifts the frequency bits to the right
	// So after each loop, the shift occurs and now we are ready to send the next bit.
	//
	// The 'freq & '0x01' 'ands' the frequency and a 1.  
	// A 1 in a 32 bit integer looks like this              : '00000000000000000000000000000001'
	// The frequency initially might look like this (7.030) : '00001110011001011100001100010001'
	//
	// So, if the rightmost bit  of the frequency = '1', the result of the
	// first 'and' operation is 1.  
	// If the rightmost bit of the frequency is 0, the 'and' operation results in a zero. 
	//
	// All other bits of the frequency will 'and' as zero.  Since they 'anded'
	// with a zero.  So, this gives us just the rightmost bit.
	// 
	// After sending the rightmust bit to the DDS, we send a pulse
	// to the dds word load clock pin to tell it it has a bit to process.
	//
	// The next right shift of the frequency then gets us ready to send the next bit.
	//
	for (int b = 0; b < 32; b++, freq >>= 1) 
	{
		digitalWrite(_ddsDataPin, freq & 0x01);
		pulseHigh(_ddsWordLoadClockPin);
	}
	
	// Here we are sending all zeros for the control byte to the dds
	// (no control, power-down or phase configuration info to send)
	// This is done after sending the frequency information
	// 
	// so after all of this is done, 40 bits was sent to the DDS
	for (int b = 0; b < 8; b++) 
	{
		digitalWrite(_ddsDataPin, 0);
		pulseHigh(_ddsWordLoadClockPin);
	}
	
	// Now we are finished sending the new frequency information.  We pulse the
	// Frequency update pin to tell the DDS to go ahead and use the new frequency
	// we just sent.
	pulseHigh(_ddsFrequencyUpdatePin);
};
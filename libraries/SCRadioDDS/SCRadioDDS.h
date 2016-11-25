/**
* SCRadioDDS.h - Class for controlling AD9850 DDS
*
* Copyright (c) 2016 - Richard Young Dodd
*
* Richard Young Dodd licenses this file to you under the MIT license.
* See the LICENSE file in the project root for more information.
* If you did not receive the 'LICENSE' file with this software
* see <https://opensource.org/licenses/MIT>.
*
* @author Richard Y. Dodd - K4KRW
* @version 1.0  11/20/2016.
*/

#ifndef SCRadioDDS_h
#define SCRadioDDS_h

#include "Wire.h"

/**
 * pulseHigh
 * 
 * @detail
 *   Macro to pulses the specified Arduini PIN HIGH and then LOW
 * 
 * @param[in] pin Arduino pin to pulse
 */
#define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

class SCRadioDDS
{
private:
	// Private member variables

	/**
	 * We pulse this Arduino pin HIGH to tell the DDS it has a bit to process on the data line
	 */
   const int8_t _ddsWordLoadClockPin;

	/**
	 * We pulse this Arduino pin HIGH to tell the DDS we are finished sending it a new frequency
	 */
   const int8_t _ddsFrequencyUpdatePin;

	/**
	 * This Arduino pin connects to the data pin on the DDS.  We send each bit of frequency data via this line
	 */
   const int8_t _ddsDataPin;

	/**
	 * This Arduino pin connects to the reset pin on the DDS.  We pulse it high to issue a reset
	 */
   const int8_t _ddsResetPin;

	/**
	 * This holds the value that allows us to fine tune the frequency of the DDS
	 */
   const float _ddsTuningWord;

	/**
	 * If this is true, then begin() has been run.
	 * We don't want to send data to the DDS if this class has not been initialized.  
	 * This helps determine the status
	 */
   bool _beginHasRun = false;
   
  public:
	// public methods
	
	/**
	 * SCRadioDDS
	 * 
	 * @detail
	 *   Creates a SCRadioDDS class.
	 *   begin() must be called before using
	 * 
	 * @param[in] ddsWordLoadClockPin The Arduino pin talking to the frequency update pin on the DDS
	 * @param[in] ddsDataPin The Arduino pin talking to the data pin on the DDS
	 * @param[in] ddsResetPin The Arduino pin talking to the reset pin on the DDS
	 * @param[in] ddsTuningWord The value used to fine tune the frequency
	 */
    SCRadioDDS(int8_t ddsWordLoadClockPin,
   						int8_t ddsFrequencyUpdatePin,
   						int8_t ddsDataPin,
   						int8_t ddsResetPin,
   						float ddsTuningWord);
    
	/**
	 * sendFrequencyToDDS
	 * 
	 * @detail
	 *   Sends a new frequency to the DDS
	 * 
	 * @param frequency Integer representation of a frequency ex: 7.030.000 would be 7030000
	 */
	void sendFrequencyToDDS(int32_t frequency);    	
   	
	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes the SCRadioDDS object
	 */
    void begin();
    
  private:
	 // private methods
};

#endif
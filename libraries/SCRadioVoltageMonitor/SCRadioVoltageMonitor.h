/**
* SCRadioVoltageMonitor.h - Library for logic for monitoring supply voltage for rig
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
* @version 1.0.3  12/22/2016.
*/

#ifndef SCRadioVoltageMonitor_h
#define SCRadioVoltageMonitor_h

#include "SCRadioConstants.h"

class SCRadioVoltageMonitor
{
private:
	/**
	 * The event manager is used to enqueue messages that tell the
	 * remaining logic that the voltage has changed.
	 */
	EventManager &_eventManager;

	int8_t _arduinoPinToRead;

	/**
	 * a value to multiply by the read voltage from the arduino to get
	 * the actual voltage value.
	 */
	float _voltageCalcMultiplier;

	/**
	 * Holds the event type code to be sent with the message indicating
	 * the voltage has changed.
	 */
	EventType _voltageChangedEventTypeCode;

	/**
	 * The number of loops between voltage checks.  We don't want to bog
	 * down the system checking ever time we loop.
	 */
	int16_t _loopsBetweenVoltageChecks;

	/**
	 * Counts the number of times loop has been called.  Resets every
	 * _loopsBetweenVoltageChecks times.
	 */
	int16_t _loopCounter;

	/**
	* The last voltage value read.  Used for comparison to see if voltage
	* changed between loops.
	*/
	int16_t _lastVoltageRead = 0;

public:
	/**
	 * SCRadioVoltageMonitor
	 * 
	 * @detail
	 *   Periodically checks the rig's supply voltage and sends a message
	 *   if the supply voltage changed from the last value read.
	 *   
	 * @param[in] eventManager reference to eventManager used to send
	 *                          a message when the voltage changes
	 * 
	 * @param[in] voltageCalcMultiplier A value that is multiplied by
	 *                          the value read from an Arduino analog
	 *                          pin to get the actual voltage.
	 *                          
	 * @param[in] voltageChangedEventTypeCode The event type code to send
	 *                         with the event message to indicate that the
	 *                         voltage has changed.
	 *                         
	 * @param[in] loopsBetweenVoltageChecks  Checks will not be done on
	 *                         every loop so we don't needlessly overload
	 *                         the app.  When the loop counter reaches
	 *                         this value, we will read the voltage and
	 *                         reset the counter.
	 */
	SCRadioVoltageMonitor(EventManager &eventManager, 
							int8_t arduinoPinToRead,
							float voltageCalcMultiplier,
							EventType voltageChangedEventTypeCode, 
							int16_t loopsBetweenVoltageChecks);

	/**
	 * getRigVoltageX10AsInt
	 * 
	 * @detail
	 *   Returns the voltage read multiplied by 10.  The event logic deals
	 *   in integers only but we want to be able to display tenths of a
	 *   volt.
	 */
	int16_t getRigVoltageX10AsInt();

	/**
	 * loop
	 * 
	 * @detail
	 *   Each time the rig's loop executes, this should be called
	 */
	void loop();
};
#endif

/**
 * SCRadioVFO.h - Class for controlling radio's VFO
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

#ifndef SCRadioVFO_h
#define SCRadioVFO_h

// forwards for class pointers and references
class EventManager;
class SCRadioDDS;
class SCRadioEventData;

// includes
#include "SCRadioConstants.h"
#include "SCRadioKey.h"
#include "SCRadioFrequency.h"

class SCRadioVFO
{
private:

	// private member data

	/**
	 * Object used to enqueue new messages
	 */
	EventManager &_eventManager;

	/**
	 * Data needed while processing events
	 */
	SCRadioEventData &_eventData;

	/**
	 * Object actually interacting directly with DDS;
	 */
	SCRadioDDS &_dds;

	/**
	 * Used to fine tune the accuracy of the DDS
	 */
	const float _ddsTuningWord;

	/**
	 * Frequency limit for the bottom of the band
	 */
	SCRadioFrequency _lowerFrequencyLimit;

	/**
	 * Frequency limit for the top of the band
	 */
	SCRadioFrequency _upperFrequencyLimit;

	/**
	 * Startup frequency for the rig if there is no persisted frequency in EEPROM memory
	 * (Or if persisted value is nonsensical)
	 */
	SCRadioFrequency _initialFrequency;

	/**
	 * The current transmit frequency
	 */
	SCRadioFrequency _currentTXFrequency;

	/**
	 * The current receive frequency
	 */
	SCRadioFrequency _currentRXFrequency;


	// adjustable tuning increment settings

	/**
	 * Increment value when turning vfo knob slow
	 */
	const int16_t _slowTuningIncrement;

	/**
	 * Increment value when turning the vfo knob at a medium speed
	 */
	const int16_t _mediumTuningIncrement;

	/**
	 * Increment value when turning the vfo knob at a fast speed
	 */
	const int16_t _fastTuningIncrement;

	/**
	 * The current tuning increment
	 */
	int16_t _currentTuningIncrement;

	/**
	 * The calculated tuning increment the last time it was computed
	 */
	int8_t _lastTuningIncrement;
	
	/**
	 * Milliseconds boundary between slow and medium knob turn event intervals
	 */
	const uint16_t _mediumTuningThresholdms;

	/**
	 * Milliseconds boundary between fast and meduim knob turn event intervals
	 */
	const uint16_t _fastTuningThresholdms;


	/**
	 * Arduino pin number that is sending the keying signal to the transceiver
	 */
	const int8_t _keyOutPin;

	/**
	 * The current RIT status (enabled, disabled)
	 */
	RitStatus _ritStatus;

	/**
	 * The millis() reading when the main knob last turned
	 */
	uint32_t _mSecsAtLastKnobTurn;

	/**
	 * The current receive offset (allows us to hear the received signal - if zero beat we would not hear anything
	 */
	int32_t _rxOffset;

	/**
	 * The maximum offset for the RIT
	 */
	int32_t _ritMaxOffsetHz;

	/**
	 * The upper limit for the RIT based on the max offset value
	 */
	int32_t _ritUpperLimitHz;

	/**
	 * The lower limit for the RIT based on the max offset value
	 */
	int32_t _ritLowerLimitHz;

	/**
	 * Indicates whether we are receiving or transmitting
	 */
	RxTxStatus _rxTXStatus;

	/**
	 * indicates whether offset is positive or negative
	 */
	RxOffsetDirection _rxOffsetDirection;

public:  
	
	// public methods

	/**
	 * SCRadioVFO
	 * 
	 * @detail
	 *   Creates an instance of the SCRadioVFO class
	 * 
	 * @param[in] eventManager EventManager object (used to send event messages)
	 * @param[in] eventData holds data needed for event related logic
	 * @param[in] dds interacts with DDS hardware
	 * @param[in] keyOutPin Arduino pin that keys the transmitter
	 * @param[in] ddsTuningWord Used as a means of fine tuning the frequency of the DDS
	 * @param[in] lowerFrequencyLimit Bottom of the ham band 
	 * @param[in] upperFrequencyLimit Top of the ham band
	 * @param[in] ritMaxOffsetHz Maximum RIT offset
	 * @param[in] slowTuningIncrement Frequency increment in Hertz when turning knob slow
	 * @param[in] mediumTuningIncrement Frequency increment in Hertz when turning knob at a medium speed
	 * @param[in] fastTuningIncrement Frequency increment in Hertz when turning knob fast
	 * @param[in] mediumTuningThreshold Milliseconds since last knob turn boundary between slow and medium knob speed
	 * @param[in] fastTuningThreshold Milliseconds since last knob turn boundary between medium and fast knob speed
	 */
	SCRadioVFO(EventManager &eventManager,
					SCRadioEventData &eventData,
					SCRadioDDS &dds,
					int8_t keyOutPin,
					float ddsTuningWord,
					int32_t rxOffset,
    				int32_t lowerFrequencyLimit,
    				int32_t upperFrequencyLimit,
    				int32_t ritMaxOffsetHz,
    				int16_t slowTuningIncrement,
    				int16_t mediumTuningIncrement,
    				int16_t fastTuningIncrement,
    				uint16_t mediumTuningThresholdms,
    				uint16_t fastTuningThresholdms);

	/**
	 * begin
	 * 
	 * @detail
	 *   sets up object so it is ready to use - constructor type logic goes here. 
     *   It gets called in the 'setup()' section of the main program
	 */
	void begin();
	
	/**
	 * keyLineChangedListener
	 * 
	 * @detail
	 *   Listens for key state changed events
	 * 
	 * @param[in] eventCode Identifies which event type
	 * @param[in] keyStatus Indicates whether pressed or released
	 */
	void keyLineChangedListener(int eventCode, int keyStatus); 

	/**
	* ritKnobTurnListener
	*
	* @detail
	*   Listens for rit knob turned events
	*
	* @param[in] eventCode Identifies which event type
	* @param[in] turnDirection Indicates which direction the knob turned
	*/	
	void ritKnobTurnedListener(int eventCode, int turnDirection);

	/**
	* ritStatusChangedListener
	*
	* @detail
	*   Listens for rit status changed events
	*
	* @param[in] eventCode Identifies which event type
	* @param[in] whichMenuItem indicates which menu item sent the message
	*/
	void ritStatusChangedListener(int eventCode, int whichMenuItem);
   	
	/**
	* rxOffsetDirectionChangedListener
	*
	* @detail
	*   Listens for rx Offset Direction change events
	*
	* @param[in] eventCode Identifies which event type
	* @param[in] whichMenuItem indicates which menu item sent the message
	*/
	void rxOffsetDirectionChangedListener(int eventCode, int whichMenuItem);

	/**
	 * setInitialFrequency
	 * 
	 * @detail
	 *   Sets the initial frequency for the VFO
	 * 
	 * @param[in] initialFrequency 32 bit representation
	 */
   	void setInitialFrequency( int32_t initalFrequency );
    
	/**
	* vfoKnobTurnListener
	*
	* @detail
	*   Listens for vfo knob turn event
	*
	* @param[in] eventCode Identifies which event type
	* @param[in] turnDirection indicates which direction it turned
	*/
	void vfoKnobTurnedListener(int eventCode, int turnDirection);

private:
   	
	// private methods

	/**
	 * calculateFrequencyFlagsInt
	 * 
	 * @detail
	 *   This combines the state of the RIT on off option and the receive offset direction option
	 *   into one integer value so the display can use that value to determine what is being
	 *   shown to a user.
	 * 
	 * @returns calculated flags value
	 */
	int8_t calculateFrequencyFlagsInt();

	/**
	 * calculateRXFrequency
	 * 
	 * @detail
	 *   Calculates a new receive frequency taking into account RxOffset 
	 *   value and direction and also RIT status and setting
	 */
	void calculateRXFrequency();

	/**
	 * calculateTuningIncrement
	 * 
	 * @detail
	 *   Calculates a new tuning increment value
	 */
	void calculateTuningIncrement();
   	
	/**
	 * changeFrequency
	 * 
	 * @detail
	 *   Changes frequency based on the last knob turn direction and tuning increment
	 * 
	 * @param[in] turnDirection
	 */
	void changeFrequency(int8_t turnDirection);
	
	/**
	 * changeRITOffset
	 * 
	 * @detail
	 *   Changes the RIT offset based on the last knob turn direction and configured RIT tuning increment
	 */
	void changeRITOffset(int8_t turnDirection);
	
	/**
	 * changeRITStatus
	 * 
	 * @detail
	 *   Changes RIT between enabled and disabled status
	 *   
	 * @param[in] whichMenuItem Number of menu item that directed the change
	 */
	void changeRITStatus(int8_t whichMenuItem);

	/**
	 * changeRxOffsetDirection
	 * 
	 * @detail
	 *   Changes the RxOffset to the opposite side of the receive frequency
	 *   
	 * @param[in] whichMenuItem Number of menu item that directed the change
	 */
	void changeRxOffsetDirection(int8_t whichMenuItem);

	/**
	 * checkBoundsAndCorrectIfNeeded
	 * 
	 * @detail
	 *   Checks a new frequency against the band boundaries and corrects if if necessary
	 *   
	 * @param[in-out] newTXFrequency new TX frequency to check and adjust if needed
	 */
	void checkBoundsAndCorrectIfNeeded(SCRadioFrequency &newTXFrequency);
   	
	/**
	 * checkRITBoundariesAndCorrectIfNeeded
	 * 
	 * @detail
	 *   Checks provided rit offset against the maximum RIT offset and returns the value (corrected if necessary)
	 *   
	 * @param[in] newRitOffsetHz 
	 * 
	 * @returns new RIT value corrected if necessary
	 */
	int16_t checkRITBoundariesAndCorrectIfNeeded(int16_t newRitOffsetHz);

	/**
	 * initializeKeyOutPin
	 * 
	 * @detail
	 *   Sets up the output pin that keys the 49er to transmit
	 */
	void initializeKeyOutPin();

	/**
	 * switchTXRXFrequencyAndStatus
	 * 
	 * @detail
	 *   Does either:
	 *   Send the TX frequency to the DDS and key the transmitter
	 *   Unkey the transmitter and send the RX frequency to the DDS
	 *   
	 * @param[in] keyStatus status of the key 
	 */
	void sendToDDSTxRxFrequencyAndChangeTxRxStatus(int16_t keyStatus);
};

#endif
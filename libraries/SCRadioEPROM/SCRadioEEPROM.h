/*
 * SCRadioEEPROM.h - Class for storing and retrieving values saved in EEPROM memory
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

#ifndef SCRadioEEPROM_h
#define SCRadioEEPROM_h

// forwards for classes accessed via pointers and references only
class SCRadioEventData;

#include "SCRadioConstants.h"

class SCRadioEEPROM
{
	// private member data
private:
	// this union allows us to deal with a long integer as either a long or as an array of 4 bytes
	union {
	byte array[4];
	uint32_t val;
	} myUnion;
  	
	/**
	 * Clock time (millis()) when last EEPROM write occured
	 */
	uint32_t _lastWriteMillis;
	
	/**
	 * Allows access to event related data
	 */
	SCRadioEventData &_eventData;
	
	/**
	 * Used to help tell if frequency has changed since the last time it was written to EEPROM
	 */
	uint32_t _lastTXFrequencyWritten;
	
	/**
	 * Used to help tell if keyer mode has changed since the last time it was written to EEPROM
	 */
	uint32_t _lastKeyerModeWritten;

	/**
	 * Used to help tell if keyer speed has changed since the last time it was written to the EEPROM
	 */
	uint32_t _lastKeyerSpeedWritten;

	/**
	 * Used to help tell if paddles orientation has changed since the last tiem it was written
	 */
	uint32_t _lastPaddlesOrientationWritten;

	/**
	 * Frequency to be written to the EEPROM
	 */
	uint32_t _frequencyToWrite;
	
	/**
	 * New keyer mode to be written to the EEPROM
	 */
	uint32_t _keyerModeToWrite;

	/**
	 * New keyer speed to be written to the EEPROM
	 */
	uint32_t _keyerSpeedToWrite;
	
	/**
	* New paddles orientation to be written to the EEPROM
	*/
	uint32_t _paddlesOrientationToWrite;
	
	/**
	 * Minimum milliseconds between writes to EEPROM memory
	 */
	uint32_t _minimumWriteIntervalMillis;
	
	/**
	 * Flag tells us whether any value we could send to EEPROM has changed
	 */
	bool _itemsHaveChanged;
	
	/**
	 * Flag tells us the frequency has changed since written to EEPROM
	 */
	bool _txFrequencyHasChanged;
   
	/**
	 * Flag tells us the keyer mode has changed since written to EEPROM
	 */
	bool _keyerModeHasChanged;

	/**
	* Flag tells us the keyer speed has changed since written to EEPROM
	*/
	bool _keyerSpeedHasChanged;

	/**
	 * Flag tells us the paddles orientation has changed since last written
	 */
	bool _paddlesOrientationHasChanged;

  public:     
	// public methods

	/**
	 * SCRadioEEPROM
	 * 
	 * @detail
	 *   Creates a SCRadioEEPROM.  Run begin() after creating and before using.
	 *   
	 * @param[in] eventData Reference to eventData instance to allow access to event related data
	 * @param[in] minimumWriteInervalMillis Minimum interval (milliseconds) between writes to EEPROM
	 */
	SCRadioEEPROM(SCRadioEventData &eventData,
		int32_t minimumWriteIntervalMillis);

	/**
	 * frequencyChangedListener
	 * 
	 * @detail
	 *   Listens for frequency changed events
	 * 
	 * @param[in] eventCode identifies type of event
	 * @param[in] eventFrequencyIndex index in eventData of frequency that changed
	 */
	void frequencyChangedListener(int eventCode, int eventFrequencyIndex);
	
	/**
	 * keyerModeChangedListener
	 * 
	 * @detail
	 *   Listens for changes in keyer mode
	 * 
	 * @param[in] eventCode Identifieds type of event
	 * @param[in] whichMenuItem Identifies which menu item set the keyer mode
	 */
	void keyerModeChangedListener(int eventCode, int whichMenuItem);

	/**
	* keyerModeChangedListener
	*
	* @detail
	*   Listens for changes in keyer speed
	*
	* @param[in] eventCode Identifieds type of event
	* @param[in] whichMenuItem Identifies which menu item set the keyer speed
	*/	
	void keyerSpeedChangedListener(int eventCode, int whichMenuItem);

	/**
	* paddlesOrientationChangedListener
	*
	* @detail
	*   Listens for changes in paddles orientation setting
	*
	* @param[in] eventCode Identifieds type of event
	* @param[in] whichMenuItem Identifies which menu item set the orientation
	*/
	void paddlesOrientationChangedListener(int eventCode, int whichMenuItem);
	
	/**
	 * begin
	 * 
	 * @detail
	 *   Initializes the object. Must be called before using object
	 */
	void begin();
    
	/**
	 * loop
	 * 
	 * @detail
	 *   This should be called once each time the app's main execution loop method runs.
	 *   The object will then check to see if it needs to upodate the stored frequency value.
	 */
	void loop();

	/**
	 * readStoredOperatingFrequency
	 * 
	 * @detail
	 *   Returns the stored operating frequency from the eprom
	 * 
	 * @returns operating frequency
	 */
	uint32_t readStoredOperatingFrequency();

	/**
	 * readStoredKeyerMode
	 * 
	 * @detail
	 *   Returns the stored keyer mode from the eprom
	 * 
	 * @returns keyer mode
	 */
	int8_t readStoredKeyerMode();

	/**
	 * readStoredKeyerSpeed
	 * 
	 * @detail
	 *   returns the stored keyer speed from the eprom
	 * 
	 * @returns keyer speed
	 */
	int8_t readStoredKeyerSpeed();

	/**
	* readStoredPaddlesOrientation
	*
	* @detail
	*   returns the stored paddles orientation value from the eprom
	*
	* @returns paddles orientation
	*/	
	int8_t readStoredPaddlesOrientation();

  private:
	// private methods

	/**
	 * processFrequencyToPotentiallyArchive
	 * 
	 * @detail
	 *   Determines if the frequency has changed so we know we will need to save it to EEPROM
	 */
	void processFrequencyToPotentiallyArchive(EventFrequencyField eventFrequencyIndex);

	/**
	 * processKeyerModeToPotentiallyArchive
	 * 
	 * @detail
	 *   Gets changed keyer mode to potentially archive
	 *   
	 * @param[in] whichMenuItem Menu item adjusting the keyer mode
	 */
	void processKeyerModeToPotentiallyArchive(int whichMenuItem);

	/**
	* processKeyerSpeedToPotentiallyArchive
	*
	* @detail
	*   Gets changed keyer speed to potentially archive
	*
	* @param[in] whichMenuItem Menu item adjusting the keyer speed
	*/
	void processKeyerSpeedToPotentiallyArchive(int whichMenuItem);


	/**
	* processPaddlesOrientationToPotentiallyArchive
	*
	* @detail
	*   Gets changed paddles orientation to potentially archive
	*
	* @param[in] whichMenuItem Menu item adjusting the paddles orientation
	*/
	void processPaddlesOrientationToPotentiallyArchive(int whichMenuItem);
	
	/**
	* readStoredValue
	*
	* @detail
	*   Reads an unsigned 32 bit integer from EEPROM
	*
	* @param[in] which value to read
	* 
	* @returns the stored value
	*/
	// read a 32 bit unsigned integer from EEPROM
	uint32_t readStoredValue(EEPROMValueIndex whichValue);

	/**
	 * writeOperatingFrequency
	 * 
	 * @detail
	 *   Writes current operating frequency to EEPROM
	 */
	void writeOperatingFrequency();
  	
	/**
	 * writeKeyerMode
	 * 
	 * @detail
	 *   Writes current keyer mode to EEPROM
	 */
	void writeKeyerMode();

	/**
	 * writeKeyerSpeed
	 * 
	 * @detail
	 *   Writes current keyer speed to EEPROM
	 */
	void writeKeyerSpeed();

	/**
	 * writePaddlesOrientation
	 * 
	 * @detail
	 *   Writes current paddles orientation to EEPROM
	 */
	void writePaddlesOrientation();

	/**
	 * writeEEPROMValue
	 * 
	 * @detail
	 *   Writes a 32 bit unsigned integer to EEPROM
	 *   
	 * @param[in] valueToSet Value to write to EEPROM
	 * @param[in] indexOfValue index of location to write value
	 */
	void writeEEPROMValue(uint32_t valueToSet, uint8_t indexOfValue);
};

#endif
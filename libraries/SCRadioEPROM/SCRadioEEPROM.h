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
 * @version 1.0  11/20/2016.
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
	 * Frequency to be written to the EEPROM
	 */
	uint32_t _frequencyToWrite;
	
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
	 * readStoredOperatingFrequency
	 * 
	 * @detail
	 *   Retrieves the operating frequency saved in EEPROM memory
	 * 
	 * @returns 32 bit unsigned int representing frequency (7030000 would be 7.030.000)
	 */
	 uint32_t readStoredOperatingFrequency();
	
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
	 * writeOperatingFrequency
	 * 
	 * @detail
	 *   Writes current operating frequency to EEPROM
	 */
	void writeOperatingFrequency();
  	
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
  	
	/**
	 * readEEPROMValue
	 * 
	 * @detail
	 *   Reads an unsigned 32 bit integer from EEPROM
	 *   
	 * @param[in] indexOfValue index of value to read
	 */
	// read a 32 bit unsigned integer from EEPROM
	uint32_t readEEPROMValue(uint8_t indexOfValue);
};

#endif
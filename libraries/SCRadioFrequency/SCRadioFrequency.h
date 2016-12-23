/*
 * SCRadioFrequency.h - Class for an object to represent a frequency and to offer logic to manipulate it
 *
  * Why does this exist?
 * 
 * I found that it was faster to manipulate the rig frequency values if they were three separate smaller integer values
 * (megahertz, kilihertz and hertz) rather than one 32 bit integer
 * 
 * So I wrote this class to handle the frequency as three separate components
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

#ifndef SCRadioFrequency_h
#define SCRadioFrequency_h

#include "SCRadioConstants.h"

class SCRadioFrequency
{
private:
	// private member data

	/**
	 * MegaHertz component of frequency (ex: 7 for 7.030.850
	 */
	int16_t _megaHertz;

	/**
	 * KiloHertz component of frequency (ex: 30 for 7.030.850
	 */
	int16_t _kiloHertz;

	/**
	 * Hertz component of frequency (ex: 850 for 7.030.850
	 */
	int16_t _hertz;
   
  public:
	// public methods

	/**
	 * SCRadioFrequency
	 * 
	 * @detail
	 *   Creates an empty frequency object
	 */
  	SCRadioFrequency();
  	
	/**
	 * SCRadioFrequency
	 * 
	 * @detail
	 *   Create one from separate frequency components
	 * 
	 * @param[in] megaHertz
	 * @param[in] kiloHertz
	 * @param[in] hertz
	 */
    SCRadioFrequency(int8_t megaHertz,
    					int16_t kiloHertz,
    					int16_t hertz);
   
	/**
	 * SCRadioFrequency
	 * 
	 * @detail
	 *   Create one from an integer representation of a frequency (7030000 is 7.030.000)
	 * 
	 * @param[in] frequency Integer representation of frequency
	 */
    SCRadioFrequency(int32_t frequency);
    
	/**
	 * addHertz
	 * 
	 * @detail
	 *   Add the specified number of Hertz to the frequency
	 * 
	 * @param[in] hertzToAdd
	 */
    void addHertz(int16_t hertzToAdd);
    
	/**
	 * addKiloHertz
	 * 
	 * @detail
	 *   Add the specified number of kiloHertz to the frequency
	 * 
	 * @param[in] kiloHertzToAdd
	 */
    void addKiloHertz(int16_t kiloHertzToAdd);
    
	/**
	 * addMegaHertz
	 * 
	 * @detail
	 *   Add the specified number of megaHertz to the frequency
	 * 
	 * @param[in] megaHertzToAdd
	 */
    void addMegaHertz(int16_t megaHertzToAdd);
    
	/**
	 * compare
	 * 
	 * @detail
	 *   Compare this frequency to another frequency
	 * 
	 * @param[in] frequency to compare
	 * 
	 * @returns FrequencyCompareResult enum (GREATER_THAN LESS_THAN EQUAL)
	 */
	FrequencyCompareResult compare(SCRadioFrequency otherFrequency);
	
	/**
	 * equals
	 * 
	 * @detail
	 *   Tells if two frequencies hold the same value
	 * 
	 * @param[in] otherFrequency frequency to compare
	 * 
	 * @returns true if equal, false if not
	 */
	bool equals(SCRadioFrequency otherFrequency);
	
	/**
	 * replaceValue
	 * 
	 * @detail
	 *   Replaces the frequency object's value with the value of the specified other frequency
	 *   (reusing the instance due to Arduino memory limitations)
	 * 
	 * @param[in] frequency to replace existing value.
	 */
	void replaceValue(SCRadioFrequency otherFrequency);
	
	/**
	 * replaceValue
	 * 
	 * @detail
	 *   Replaces the freqency object's value with the value represented by the provided frequency components
	 * 
	 * @param[in] megaHertz MegaHertz component of new frequency
	 * @param[in] kiloHertz KiloHertz component of new frequency
	 * @param[in] hertz Hertz component of new frequency
	 */
	void replaceValue(int16_t megaHertz, 
						int16_t kiloHertz, 
						int16_t hertz);
	
	/**
	 * replaceValue
	 * 
	 * @detail
	 *   Replaces the frequency object's value with the value represented by the provided 32 bit word representation of a frequency
	 * 
	 * @param[in] 32 bit integer representation of new frequency (7030000 for 7.030.000)
	 */
	void replaceValue(int32_t integerFrequency);
	
	/**
	 * asInt32
	 * 
	 * @detail
	 *   Returns a 32 bit integer representation of a frequency
	 * 
	 * @returns frequency value as 32 bit int
	 */
	int32_t asInt32();
	
	/**
	 * megaHertz
	 * 
	 * @detail
	 *   Returns megaHertz component of frequency
	 * 
	 * @returns megaHertz component of frequency
	 */
	int16_t megaHertz();
		
	/**
	* kiloHertz
	*
	* @detail
	*   Returns kiloHertz component of frequency
	*
	* @returns kiloHertz component of frequency
	*/
	int16_t kiloHertz();
	
	/**
	* hertz
	*
	* @detail
	*   Returns Hertz component of frequency
	*
	* @returns Hertz component of frequency
	*/
	int16_t hertz();
			
  private:
};

#endif
/**
 * SCRadioFrequency.cpp - Class for an object to represent a frequency and to offer logic to manipulate it
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

#include "Arduino.h"
#include "SCRadioFrequency.h"

// public methods

// Constructor for an empty frequency value;
SCRadioFrequency::SCRadioFrequency() 
{
}

// constructor to make a frequency from frequency components
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioFrequency::SCRadioFrequency(int8_t megaHertz,
									int16_t kiloHertz,
    							int16_t hertz) : _megaHertz(megaHertz), _kiloHertz(kiloHertz), _hertz(hertz) 
{
}

// contstructor to make a frequency from a 32 bit representation of a frequency
// The logic after the ':' is initializer logic.  It will assign the input parameter values to object instance variables.
SCRadioFrequency::SCRadioFrequency(int32_t frequency) :  
	_megaHertz((int16_t)(frequency / 1000000)),
	_kiloHertz((int16_t)((frequency % 1000000) / 1000)),
	_hertz((int16_t)(frequency % 1000)) 
{
}

void SCRadioFrequency::addHertz(int16_t hertzToAdd) 
{	
	int16_t kiloHertzToAdd = hertzToAdd / 1000;
	
	hertzToAdd = hertzToAdd % 1000;

	_hertz += hertzToAdd;
	if (_hertz < 0) 
	{
		_hertz += 1000;
		kiloHertzToAdd -= 1;
	}

	if (_hertz > 999) 
	{
		_hertz -= 1000;
		kiloHertzToAdd += 1;
	}
	
	addKiloHertz(kiloHertzToAdd);
}

void SCRadioFrequency::addKiloHertz(int16_t kiloHertzToAdd) 
{	
	int16_t megaHertzToAdd = kiloHertzToAdd / 1000;
	
	kiloHertzToAdd = kiloHertzToAdd % 1000;
	
	_kiloHertz += kiloHertzToAdd;
	
	if (_kiloHertz < 0) 
	{
		_kiloHertz += 1000;
		megaHertzToAdd -= 1;
	}
	
	if (_kiloHertz > 999) 
	{
		_kiloHertz -= 1000;
		megaHertzToAdd += 1;
	}
	
	addMegaHertz(megaHertzToAdd);
}
  
void SCRadioFrequency::addMegaHertz(int16_t megaHertzToAdd) 
{
	_megaHertz += megaHertzToAdd;
}
    				
FrequencyCompareResult SCRadioFrequency::compare(SCRadioFrequency otherFrequency)
{

	if ((_megaHertz == otherFrequency.megaHertz()) && (_kiloHertz == otherFrequency.kiloHertz()) && (_hertz == otherFrequency.hertz()))
	{
		return FrequencyCompareResult::EQUAL;
	}
	else if ((_megaHertz < otherFrequency.megaHertz()) 
			|| ((_megaHertz == otherFrequency.megaHertz()) && (_kiloHertz < otherFrequency.kiloHertz()))
			|| ((_megaHertz == otherFrequency.megaHertz()) && (_kiloHertz == otherFrequency.kiloHertz()) && (_hertz < otherFrequency.hertz()))) 
	{
		return FrequencyCompareResult::LESS_THAN;
	} 

	return FrequencyCompareResult::GREATER_THAN;
}

bool SCRadioFrequency::equals(SCRadioFrequency otherFrequency) {

	if (compare(otherFrequency) == FrequencyCompareResult::EQUAL)
	{
		return true;
	}

	return false;
}

void SCRadioFrequency::replaceValue(SCRadioFrequency otherFrequency) 
{
	_megaHertz = otherFrequency.megaHertz();
	_kiloHertz = otherFrequency.kiloHertz();
	_hertz = otherFrequency.hertz();
}

void SCRadioFrequency::replaceValue(int16_t megaHertz, int16_t kiloHertz, int16_t hertz) 
{
	_megaHertz = megaHertz;
	_kiloHertz = kiloHertz;
	_hertz = hertz;
}

void SCRadioFrequency::replaceValue(int32_t integerFrequency) 
{
	_megaHertz = (integerFrequency / 1000000);
	_kiloHertz = ((integerFrequency % 1000000) / 1000);
	_hertz = (integerFrequency % 1000);
}
		
int32_t SCRadioFrequency::asInt32() 
{
	return (int32_t)((int32_t)_megaHertz * 1000000 + ((int32_t)_kiloHertz * 1000) + (int32_t)_hertz);
}

int16_t SCRadioFrequency::megaHertz() 
{
	return _megaHertz;
}
		
int16_t SCRadioFrequency::kiloHertz() 
{
	return _kiloHertz;
}
			
int16_t SCRadioFrequency::hertz() 
{
	return _hertz;
}
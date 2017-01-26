/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: XMLrulesClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: XML Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/

//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "XMLrulesClass.hpp"

//xml comments not yet supported by this parser

XMLrulesClass* ANNrulesSprite;	//common sprite xml file is ANNrules.xml
#ifdef USE_CS
XMLrulesClass* CSrulesSprite;
XMLrulesClass* CSrulesDraw;
XMLparserTag* CSfirstTagInXMLfile;
#endif
#ifdef USE_GIA
XMLrulesClass* GIArulesSprite;
XMLrulesClass* GIArulesDraw;
XMLparserTag* GIAfirstTagInXMLfile;
#endif
#ifdef USE_OR
XMLrulesClass* ORrulesObjectRecognition;
#endif
#ifdef USE_NLC
XMLparserTag* NLCfirstTagInXMLfile;
#endif

static string nullString;
static bool nullBool;
static XMLrulesClass nullRulesObject;

XMLrulesClass::XMLrulesClass(void)
{
	name = "";					//attribute 1

	stringValue = "";				//attribute 2 - this attribute may not be used by the object
	fractionalValue = 0.0;				//attribute 3 - this attribute may not be used by the object
	attribute4 = 0;				//attribute 4 - this attribute may not be used by the object
	attribute5 = 0;				//attribute 5 - this attribute may not be used by the object
	attribute6 = 0;				//attribute 6 - this attribute may not be used by the object
	attribute7 = 0;				//attribute 7 - this attribute may not be used by the object
	attribute8 = 0;				//attribute 8 - this attribute may not be used by the object

	next = NULL;

	/*
	defenceMod = 0;				//this attribute may not be used by the object

	closeCombatAttackMod = 0;	//this attribute may not be used by the object
	mountedAttackBonus = 0;		//this attribute may not be used by the object

	longDistanceAttackMod = 0;	//this attribute may not be used by the object
	longDistanceRange = 0;		//this attribute may not be used by the object

	*/
}

XMLrulesClass::~XMLrulesClass()
{
	if(next != NULL)
	{
		delete next;
	}
}


#ifdef USE_NLC
bool XMLrulesClassClass::parseNLCrulesXMLfile()
{
	bool result = true;

 	NLCfirstTagInXMLfile = new XMLparserTag();	//the firstTagInXMLfile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,
	if(!XMLparserClass.readXMLfile(NLC_RULES_XML_FILE_NAME, NLCfirstTagInXMLfile))
 	{
		result = false;
	}

	return result;
}
#endif

#ifdef USE_GIA
bool XMLrulesClassClass::parseGIArulesXMLfile()
{
	bool result = true;

 	GIAfirstTagInXMLfile = new XMLparserTag();	//the firstTagInXMLfile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,
	if(!XMLparserClass.readXMLfile(GIA_RULES_XML_FILE_NAME, GIAfirstTagInXMLfile))
 	{
		result = false;
	}

	XMLparserTag* currentTag = GIAfirstTagInXMLfile;

	GIArulesSprite = new XMLrulesClass();
	GIArulesDraw = new XMLrulesClass();

	if(!this->parseGIARulesTag(currentTag))
	{
		result = false;
	}

	return result;
}

bool XMLrulesClassClass::parseGIARulesTag(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_rules, &result);
	if(result)
	{
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_sprite, GIArulesSprite, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_draw, GIArulesDraw, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
	}

	return result;
}
#endif


#ifdef USE_CS
bool XMLrulesClassClass::parseCSrulesXMLfile()
{
	bool result = true;

 	CSfirstTagInXMLfile = new XMLparserTag();	//the firstTagInXMLfile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,
 	if(!XMLparserClass.readXMLfile(CS_RULES_XML_FILE_NAME, CSfirstTagInXMLfile))
 	{
		result = false;
	}

	XMLparserTag* currentTag = CSfirstTagInXMLfile;

	CSrulesSprite = new XMLrulesClass();
	CSrulesDraw = new XMLrulesClass();

	if(!this->parseCSRulesTag(currentTag))
	{
		result = false;
	}

	return result;
}

bool XMLrulesClassClass::parseCSRulesTag(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_rules, &result);
	if(result)
	{
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_sprite, CSrulesSprite, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
		currentTagUpdated=currentTagUpdated->nextTag;
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_draw, CSrulesDraw, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
	}

	return result;
}
#endif




#ifdef USE_OR
bool XMLrulesClassClass::parseORrulesXMLfile()
{
	bool result = true;

 	XMLparserTag* firstTagInXMLfile = new XMLparserTag();	//the firstTagInXMLfile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,
 	if(!XMLparserClass.readXMLfile(OR_RULES_XML_FILE_NAME, firstTagInXMLfile))
 	{
		result = false;
	}

	XMLparserTag* currentTag = firstTagInXMLfile;

	ORrulesObjectRecognition = new XMLrulesClass();

	if(!this->parseORrulesTag(currentTag))
	{
		result = false;
	}

	delete firstTagInXMLfile;


	return result;
}

bool XMLrulesClassClass::parseORrulesTag(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_rules, &result);
	if(result)
	{
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_objectRecognition, ORrulesObjectRecognition, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
	}

	return result;
}
#endif




bool XMLrulesClassClass::parseANNrulesXMLfile()
{
	bool result = true;

 	XMLparserTag* firstTagInXMLfile = new XMLparserTag();	//the firstTagInXMLfile object must be initialised here (in XMLrulesClass.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,
 	if(!XMLparserClass.readXMLfile(ANN_RULES_XML_FILE_NAME, firstTagInXMLfile))
 	{
		result = false;
	}

	/*
	#define TEMP_XML_FILE_NAME "temp.xml"
 	if(!writeXMLfile(TEMP_XML_FILE_NAME, firstTagInXMLfile))
 	{
		result = false;
	}
	*/

	XMLparserTag* currentTag = firstTagInXMLfile;

	ANNrulesSprite = new XMLrulesClass();

	if(!this->parseANNrulesTag(currentTag))
	{
		result = false;
	}

	delete firstTagInXMLfile;

	return result;
}




//Top Level
bool XMLrulesClassClass::parseANNrulesTag(XMLparserTag* currentTag)
{
	bool result = true;

	XMLparserTag* currentTagUpdated = currentTag;
	currentTagUpdated = XMLparserClass.parseTagDownALevel(currentTagUpdated, RULES_XML_TAG_rules, &result);
	if(result)
	{
		if(!this->addRulesClassObjectsBasedOnSectionTag(currentTagUpdated, RULES_XML_TAG_sprite, ANNrulesSprite, RULES_XML_TAG_miscellaneousItem, 3, RULES_XML_ATTRIBUTE_name, RULES_XML_ATTRIBUTE_stringValue, RULES_XML_ATTRIBUTE_fractionalValue, nullString, nullString, nullString, nullString, nullString))
		{
			result = false;
		}
	}

	return result;
}







bool XMLrulesClassClass::addRulesClassObjectsBasedOnSectionTag(XMLparserTag* currentTag, const string sectionTagName, XMLrulesClass* firstReferenceToObjectClass, const string tagName, const int numberOfAttributes, const string attributeName1, const string attributeName2, const string attributeName3, const string attributeName4, const string attributeName5, const string attributeName6, const string attributeName7, const string attributeName8)
{
	bool result = true;

	if(currentTag->name == sectionTagName)
	{
		XMLparserTag* currentLowerLevelTag = currentTag->firstLowerLevelTag;

		XMLrulesClass* currentReferenceToObjectClass = firstReferenceToObjectClass;

		while(currentLowerLevelTag->nextTag != NULL)
		{
			if(!this->addRulesClassObjectBasedOnTag(currentLowerLevelTag, currentReferenceToObjectClass, tagName, numberOfAttributes, attributeName1, attributeName2, attributeName3, attributeName4, attributeName5, attributeName6, attributeName7, attributeName8))
			{
				result = false;
			}
			else
			{
				/*
				cout << "name = " << currentReferenceToObjectClass->name << endl;
				cout << "stringValue = " << currentReferenceToObjectClass->stringValue << endl;
				cout << "fractionalValue = " << currentReferenceToObjectClass->fractionalValue << endl;
				cout << "attribute4 = " << currentReferenceToObjectClass->attribute4 << endl;
				cout << "attribute5 = " << currentReferenceToObjectClass->attribute5 << endl;
				cout << "attribute6 = " << currentReferenceToObjectClass->attribute6 << endl;
				cout << "attribute7 = " << currentReferenceToObjectClass->attribute7 << endl;
				cout << "attribute8 = " << currentReferenceToObjectClass->attribute8 << endl;
				*/

				XMLrulesClass* newObject = new XMLrulesClass();
				currentReferenceToObjectClass->next = newObject;
				currentReferenceToObjectClass = currentReferenceToObjectClass->next;
			}

			currentLowerLevelTag = currentLowerLevelTag->nextTag;
		}
	}
	else
	{
		result = false;
		cout << "parse error; sectionTagName expected = " <<  sectionTagName << ". section tag name found = " << currentTag->name << endl;
	}
	currentTag = currentTag->nextTag;

	return result;

}


bool XMLrulesClassClass::addRulesClassObjectBasedOnTag(XMLparserTag* currentTag, XMLrulesClass* currentReferenceToObjectClass, const string tagName, const int numberOfAttributes, const string attributeName1, const string attributeName2, const string attributeName3, const string attributeName4, const string attributeName5, const string attributeName6, const string attributeName7, const string attributeName8)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = currentTag->firstAttribute;

	int attributeNumber = 1;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(attributeNumber == 1)
		{
			if(currentAttribute->name != attributeName1)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName1.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName1 = " << attributeName1 << endl;
			}
			else
			{
				currentReferenceToObjectClass->name = currentAttribute->value;
			}
		}
		else if(attributeNumber == 2)
		{
			if(currentAttribute->name != attributeName2)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName2.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName2 = " << attributeName2 << endl;
			}
			else
			{
				currentReferenceToObjectClass->stringValue = currentAttribute->value;
			}
		}
		else if(attributeNumber == 3)
		{
			if(currentAttribute->name != attributeName3)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName3.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName3 = " << attributeName3 << endl;
			}
			else
			{
				currentReferenceToObjectClass->fractionalValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			}
		}
		else if(attributeNumber == 4)
		{
			if(currentAttribute->name != attributeName4)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName4.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName4 = " << attributeName4 << endl;
			}
			else
			{
				currentReferenceToObjectClass->attribute4 = int(SHAREDvars.convertStringToDouble(currentAttribute->value));
			}
		}
		else if(attributeNumber == 5)
		{
			if(currentAttribute->name != attributeName5)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName5.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName5 = " << attributeName5 << endl;
			}
			else
			{
				currentReferenceToObjectClass->attribute5 = int(SHAREDvars.convertStringToDouble(currentAttribute->value));
			}
		}
		else if(attributeNumber == 6)
		{
			if(currentAttribute->name != attributeName6)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName6.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName6 = " << attributeName6 << endl;
			}
			else
			{
				currentReferenceToObjectClass->attribute6 = int(SHAREDvars.convertStringToDouble(currentAttribute->value));
			}
		}
		else if(attributeNumber == 7)
		{
			if(currentAttribute->name != attributeName7)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName7.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName7 = " << attributeName7 << endl;
			}
			else
			{
				currentReferenceToObjectClass->attribute7 = int(SHAREDvars.convertStringToDouble(currentAttribute->value));
			}
		}
		else if(attributeNumber == 8)
		{
			if(currentAttribute->name != attributeName8)
			{
				result = false;
				cout << "parse error; currentAttribute->name != attributeName8.  currentAttribute->name = " <<  currentAttribute->name << ". attributeName8 = " << attributeName8 << endl;
			}
			else
			{
				currentReferenceToObjectClass->attribute8 = int(SHAREDvars.convertStringToDouble(currentAttribute->value));
			}
		}
		attributeNumber++;

		currentAttribute = currentAttribute->nextAttribute;
	}

	if((attributeNumber-1) != numberOfAttributes)
	{
		result = false;
		cout << "(attributeNumber-1) != numberOfAttributes, (attributeNumber-1) =  " <<  (attributeNumber-1) << ". numberOfAttributes = " << numberOfAttributes << endl;
	}

	return result;
}









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
 * File Name: XMLparserClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: XML Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/

//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "XMLparserClass.hpp"

#define XML_FILE_NAME "rules.xml"

#define CHAR_TAG_OPEN '<'
#define CHAR_TAG_OPEN_STR "<"
#define CHAR_TAG_CLOSE '>'
#define CHAR_TAG_CLOSE_STR ">"
#define CHAR_TAG_END '/'
#define CHAR_TAG_END_STR "/"
#define CHAR_TAG_XML_DEF '$'
#define CHAR_EXCLAMATION '!'
#define CHAR_QUESTIONMARK '?'
#define STRING_TAG_COMMENT_OPEN "--"
#define STRING_TAG_COMMENT_CLOSE "--"
#define STRING_TAG_XML_DEF_FULL "?xml version=\"1.0\" encoding=\"UTF-8\"?"
#define CHAR_TAG_ATTRIBUTE_VAL_OPEN '"'
#define CHAR_TAG_ATTRIBUTE_VAL_OPEN_STR "\""
#define CHAR_TAG_ATTRIBUTE_VAL_CLOSE '"'
#define CHAR_TAG_ATTRIBUTE_VAL_CLOSE_STR "\""
#define CHAR_TAG_ATTRIBUTE_VAL_EQUALS '='
#define CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR "="


static int charCount;
static int lineCount;

/*
string tagName;
string attributeName;
string attributeValue;
string tagValue;
*/

XMLparserAttribute::XMLparserAttribute(void)
{
	name = "";
	value = "";

	nextAttribute = NULL;
}

XMLparserAttribute::~XMLparserAttribute()
{

	if(nextAttribute != NULL)
	{
		delete nextAttribute;
	}

}




XMLparserTag::XMLparserTag(void)
{
	name = "";
	value = "";

	nextTag = NULL;
	firstLowerLevelTag = NULL;

	firstAttribute = new XMLparserAttribute();
	currentAttribute = firstAttribute;
}

XMLparserTag::~XMLparserTag()
{

	if(firstAttribute != NULL)
	{
		delete firstAttribute;
	}
	if(firstLowerLevelTag != NULL)
	{
		delete firstLowerLevelTag;
	}
	if(nextTag != NULL)
	{
		delete nextTag;
	}

}


XMLparserTag* XMLparserClassClass::parseTagDownALevel(XMLparserTag* currentTag, const string sectionTagName, bool* result)
{
	*result = true;

	XMLparserTag* updatedCurrentTag;

	if(currentTag->name != sectionTagName)
	{
		*result = false;
		cout << "parse error; lower level sectionTagName expected = " <<  sectionTagName << ". section tag name found currentTag->name = " << currentTag->name << endl;
		updatedCurrentTag = currentTag;
	}
	else
	{
		if(currentTag->firstLowerLevelTag != NULL)
		{
			updatedCurrentTag = currentTag->firstLowerLevelTag;
		}
		else
		{
			updatedCurrentTag = NULL;
		}
	}

	return updatedCurrentTag;

}




bool XMLparserClassClass::readXMLfile(const string xmlFileName, XMLparserTag* firstTagInXMLfile)
{
	bool result = true;

	charCount = 0;		//this variable needs to start being used
	lineCount = 1;

	ifstream parseFileObject(xmlFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//xml file does not exist in current directory.
		cout << "Error: XML File does not exist in current directory: " << xmlFileName << endl;
		result = false;
	}
	else
	{
		bool isSubTag = false;	//first tag in file is not a subtag
		int treeLayer = 0;
		if(!this->parseTagOpen(&parseFileObject, firstTagInXMLfile, "noCurrentTagName", isSubTag, treeLayer))
		{
			result = false;
		}
	}


	return result;
}

bool XMLparserClassClass::parseTagOpen(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer)
{
	bool result = true;
	char currentToken;
	bool endOfFile = false;

	bool finishedParsingObject = false;
	while(!finishedParsingObject && result)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
			endOfFile = true;
		}
		charCount++;

		if(currentToken == CHAR_TAG_OPEN)
		{
			this->parseTagName(parseFileObject, currentTag, parentTagName, isSubTag, treeLayer);
			finishedParsingObject = true;
		}
		else
		{
			#ifdef XML_PARSER_DO_NOT_ALLOW_TAG_VALUE_TO_BE_DEFINED_AFTER_SUBTAG
			if(this->isBlankChar(parseFileObject, currentToken))
			{

			}
			else
			{
				cout << "currentToken = " << currentToken << ":" << (int)currentToken << endl;
				cout << "XML_PARSER_ERROR 1: invalid tag opening" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
			#else
			currentTag->value = currentTag->value + currentToken;
			#ifndef XML_PARSER_DO_NOT_ALLOW_TABS_OR_NEWLINES_WITHIN_TAG_VALUE
			if(currentToken == CHAR_NEWLINE)
			{
				lineCount++;
				charCount=0;
			}
			#endif
			#endif
		}
	}

	return result;
}

bool XMLparserClassClass::parseTagName(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer)
{
	bool result = true;
	char currentToken;
	string tagName = "";

	bool firstChar = true;

	bool finishedParsingObject = false;
	bool endTagFound = false;

	while(!finishedParsingObject && result)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		charCount++;

		if(firstChar)
		{
			if((currentToken == CHAR_EXCLAMATION) || (currentToken == CHAR_QUESTIONMARK))
			{
				if(!this->parseTagComment(parseFileObject, currentToken))
				{
					result = false;
				}
				this->parseTagOpen(parseFileObject, currentTag, parentTagName, isSubTag, treeLayer);
				finishedParsingObject = true;
			}
			firstChar = false;
		}

		if(!finishedParsingObject)
		{
			if(this->isBlankChar(parseFileObject, currentToken))
			{
				if(!endTagFound)
				{
					currentTag->name = tagName;
					#ifdef XML_DEBUG
					cout << "parseTagName: " << tagName << ", treeLayer = " << treeLayer << ", lineCount = " << lineCount << endl;
					#endif
					if(!this->parseTagAttributeName(parseFileObject, currentTag, parentTagName, isSubTag, treeLayer))
					{
						result = false;
					}

					finishedParsingObject = true;
				}
				else
				{
					cout << "XML_PARSER_ERROR 2: end tag found containing white space" << endl;
					this->throwGenericXMLParseError();
					result = false;
				}
			}
			else if(currentToken == CHAR_TAG_END)
			{
				#ifdef XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_WITHOUT_PRECEEDING_SPACE
				if(tagName != "")
				{
					cout << "XML_PARSER_ERROR 3: incorrect end tag indicator detected" << endl;
					this->throwGenericXMLParseError();
					result = false;
				}
				else
				{
				#endif
					endTagFound = true;
				#ifdef XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_WITHOUT_PRECEEDING_SPACE
				}
				#endif
			}
			else if((currentToken == CHAR_EXCLAMATION) || (currentToken == CHAR_QUESTIONMARK))
			{
				cout << "XML_PARSER_ERROR 4: invalid comment openining" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
			else if(currentToken == CHAR_TAG_CLOSE)
			{
				if(tagName == "")
				{
					cout << "XML_PARSER_ERROR 5: empty tag name" << endl;
					this->throwGenericXMLParseError();
					result = false;
				}
				else
				{
					if(!this->processTagClose(parseFileObject, &currentTag, parentTagName, isSubTag, treeLayer, endTagFound, false, tagName))
					{
						result = false;
					}
					finishedParsingObject = true;
				}
			}
			else
			{
				tagName = tagName+currentToken;
			}
		}
	}

	return result;

}

bool XMLparserClassClass::parseTagAttributeName(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer)
{
	bool result = true;
	char currentToken;
	string attributeName="";

	bool endTagFound = false;

	bool finishedParsingObject = false;

	while(!finishedParsingObject && result)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		charCount++;

		if(this->isBlankChar(parseFileObject, currentToken))
		{
			if(attributeName != "")
			{
				cout << "XML_PARSER_ERROR 8: invalid attribute name, attributeName = " << attributeName << ". currentTag->name = " << currentTag->name << ". parentTagName = " << parentTagName <<  endl;
				this->throwGenericXMLParseError();
				result = false;
			}
		}
		else if(currentToken == CHAR_TAG_END)
		{
			if(attributeName != "")
			{
				cout << "XML_PARSER_ERROR 9: invalid attribute name" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
			else
			{
				endTagFound = true;
			}
		}
		else if(currentToken == CHAR_TAG_CLOSE)
		{
			if(attributeName != "")
			{
				cout << "XML_PARSER_ERROR 10: invalid attribute name, attributeName = " << attributeName << ". currentTag->name = " << currentTag->name << ". parentTagName = " << parentTagName <<  endl;
				this->throwGenericXMLParseError();
				result = false;
			}
			else
			{
				if(!this->processTagClose(parseFileObject, &currentTag, parentTagName, isSubTag, treeLayer, endTagFound, true, currentTag->name))
				{
					result = false;
				}
				finishedParsingObject = true;
			}
		}
		else if(currentToken == CHAR_TAG_ATTRIBUTE_VAL_EQUALS)
		{
			if(attributeName != "")
			{
				currentTag->currentAttribute->name = attributeName;
				if(!this->parseTagAttributeValue(parseFileObject, currentTag, parentTagName, isSubTag, treeLayer))
				{
					result = false;
				}

				finishedParsingObject = true;
			}
			else
			{
				cout << "XML_PARSER_ERROR 12: empty attribute name" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
		}
		else
		{
			attributeName = attributeName+currentToken;
		}
	}

	return result;
}

bool XMLparserClassClass::parseTagAttributeValue(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, const int treeLayer)
{
	bool result = true;
	char currentToken;

	string attributeValue = "";

	bool attributeValueOpenedYet = false;

	bool finishedParsingObject = false;
	while(!finishedParsingObject && result)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		charCount++;

		if(this->isBlankChar(parseFileObject, currentToken))
		{
			if(attributeValueOpenedYet)
			{
				attributeValue = attributeValue+currentToken;
			}
			else
			{
				//do nothing - white space
			}
		}
	#ifdef XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_INSIDE_ATTRIBUTE_VALUE
		else if(currentToken == CHAR_TAG_END)
		{
			cout << "XML_PARSER_ERROR 13: invalid attribute value" << endl;
			this->throwGenericXMLParseError();
			result = false;
		}
	#endif
		else if(currentToken == CHAR_TAG_CLOSE)
		{
			cout << "XML_PARSER_ERROR 14: invalid attribute value" << endl;
			this->throwGenericXMLParseError();
			result = false;
		}
		else if(currentToken == CHAR_TAG_ATTRIBUTE_VAL_OPEN)
		{
			if(!attributeValueOpenedYet)
			{
				attributeValueOpenedYet = true;
			}
			else
			{
				#ifdef XML_DEBUG
				if(attributeValue != "")
				{
					//cout << "empty attribute value detected" << endl;
				}
				#endif

				currentTag->currentAttribute->value = attributeValue;
				XMLparserAttribute* newAttribute = new XMLparserAttribute();
				currentTag->currentAttribute->nextAttribute = newAttribute;
				currentTag->currentAttribute = currentTag->currentAttribute->nextAttribute;
				#ifdef XML_DEBUG
				//cout << "DEBUG: rentering parseTagAttributeName after recording an attribute. treeLayer = " << treeLayer << endl;
				#endif

				if(!this->parseTagAttributeName(parseFileObject, currentTag, parentTagName, isSubTag, treeLayer))
				{
					result = false;
				}
				finishedParsingObject = true;

			}
		}
		else
		{
			attributeValue = attributeValue+currentToken;
		}
	}

	return result;
}

bool XMLparserClassClass::parseTagValueAssumingExistenceOfSubtagsAndClose(ifstream* parseFileObject, XMLparserTag* currentTag, int treeLayer)
{
	bool result = true;

	string tagName = currentTag->name;

	XMLparserTag* newLowerLevelTag = new XMLparserTag();
	currentTag->firstLowerLevelTag = newLowerLevelTag;
	XMLparserTag* subTag = newLowerLevelTag;

	char currentToken;
	string tagValue = "";

	bool finishedParsingObject = false;

	while(!finishedParsingObject && result)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		charCount++;

		if(currentToken == CHAR_TAG_OPEN)
		{
			currentTag->value = tagValue;
			treeLayer = treeLayer+1;
			this->parseTagName(parseFileObject, subTag, tagName, true, treeLayer);
			finishedParsingObject = true;
		}
		#ifdef XML_PARSER_DO_NOT_ALLOW_TABS_OR_NEWLINES_WITHIN_TAG_VALUE
		else if(this->isBlankCharTabOrNewLine(parseFileObject, currentToken))
		{//NB spaces are added to the value however tabs and new lines are not

		}
		#endif
		else
		{
			tagValue = tagValue + currentToken;	//allow tag value contents before subtag but not after....
			#ifndef XML_PARSER_DO_NOT_ALLOW_TABS_OR_NEWLINES_WITHIN_TAG_VALUE
			if(currentToken == CHAR_NEWLINE)
			{
				lineCount++;
				charCount=0;
			}
			#endif
		}
	}
	return result;

}

//NB this current XML parser accepts <!-- .... --> and <? .... > as a comment, but <! .... > as a comment is not allowed
bool XMLparserClassClass::parseTagComment(ifstream* parseFileObject, const char type)
{
	bool result = true;
	char currentToken;

	if(type == CHAR_EXCLAMATION)
	{
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		else
		{
			if(currentToken == CHAR_DASH)
			{//foundFirstDash
				if(!(parseFileObject->get(currentToken)))
				{
					result = false;
				}
				else
				{
					if(currentToken == CHAR_DASH)
					{//foundSecondDash
						#ifdef XML_DEBUG
						cout << "found legal comment opening" << endl;
						this->throwGenericXMLParseError();
						#endif
					}
					else
					{
						cout << "found illegal comment opening" << endl;
						this->throwGenericXMLParseError();
						result = false;
					}
				}
			}
			else
			{
				cout << "found illegal comment opening" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
		}
	}

	if(result)
	{
		bool finishedParsingObject = false;
		bool foundFirstDash = false;
		bool foundSecondDash = false;
		while(!finishedParsingObject && result)
		{
			if(!(parseFileObject->get(currentToken)))
			{
				result = false;
			}
			charCount++;

			if(currentToken == CHAR_TAG_CLOSE)
			{
				if(type == CHAR_EXCLAMATION)
				{
					if(foundSecondDash)
					{
						finishedParsingObject = true;
						#ifdef XML_DEBUG
						cout << "found legal comment close" << endl;
						this->throwGenericXMLParseError();
						#endif
					}
					else
					{
						foundFirstDash = false;
						foundSecondDash = false;
					}
				}
				else
				{
					finishedParsingObject = true;
				}
			}
			else if(currentToken == CHAR_DASH)
			{
				if(foundFirstDash)
				{
					foundSecondDash = true;
				}
				else
				{
					foundFirstDash = true;
				}
			}
			else
			{
				foundFirstDash = false;
				foundSecondDash = false;

				//display comments when parsing:
				#ifdef XML_PARSER_DISPLAY_COMMENTS_WHEN_PARSING
				cout << currentToken;
				#endif

				if(currentToken == CHAR_NEWLINE)
				{
					lineCount++;
					charCount=0;
				}
			}
		}
	}
	return result;

}

XMLparserTag* XMLparserClassClass::createNewTag(XMLparserTag* currentTag)
{
	XMLparserTag* newTag = new XMLparserTag();
	currentTag->nextTag = newTag;
	currentTag = currentTag->nextTag;
	return currentTag;
}

bool XMLparserClassClass::processTagClose(ifstream* parseFileObject, XMLparserTag** currentTag, const string parentTagName, const bool isSubTag, int treeLayer, const bool endTagFound, const bool parsingAttributeName, const string tagName)
{
	bool result = true;
	if(!isSubTag)
	{
		if(!parsingAttributeName)
		{
			(*currentTag)->name = tagName;
			#ifdef XML_DEBUG
			cout << "processTagClose1{}: parseTagName: " << tagName << ", treeLayer = " << treeLayer << ", lineCount = " << lineCount << endl;
			#endif
		}
		//else (*currentTag)->name has already been filled in in parseTagName()

		if(endTagFound)
		{

		}
		else
		{
			if(!this->parseTagValueAssumingExistenceOfSubtagsAndClose(parseFileObject, (*currentTag), treeLayer))
			{//go up a level in the tree
				result = false;
			}
		}

		//continue parsing at same level
		*currentTag = this->createNewTag(*currentTag);
		this->parseTagOpen(parseFileObject, *currentTag, parentTagName, isSubTag, treeLayer);
	}
	else
	{
		if(endTagFound)
		{
			if(tagName == parentTagName)
			{
				//finished parsing subtags within the value of this tag
			}
			else
			{
				#ifdef XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_WITHOUT_PRECEEDING_SPACE
				if(!parsingAttributeName)	//CHECKTHIS requirement
				{
					//subtag+end detected with no attributes - this is illegal
					cout << "XML_PARSER_ERROR 6: subtag+end detected with no attributes - this is illegal" << endl;
					this->throwGenericXMLParseError();
					result = false;
				}
				else
				{
				#endif
					//subtag+end detected without attributes
					if(!parsingAttributeName)
					{
						//NB (*currentTag)->name has not already been filled
						(*currentTag)->name = tagName;
						#ifdef XML_DEBUG
						cout << "processTagClose2{}: parseTagName: " << tagName << ", treeLayer = " << treeLayer << ", lineCount = " << lineCount << endl;
						#endif
					}
					//else (*currentTag)->name has already been filled in in parseTagName()

					*currentTag = this->createNewTag(*currentTag);
					this->parseTagOpen(parseFileObject, *currentTag, parentTagName, isSubTag, treeLayer);
				#ifdef XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_WITHOUT_PRECEEDING_SPACE
				}
				#endif
			}
		}
		else
		{
			#ifdef XML_PARSER_DO_NOT_ALLOW_SUBTAGS_WITH_SAME_NAME_AS_PARENT_TAG
			if(tagName == parentTagName)
			{
				cout << "XML_PARSER_ERROR 7: subtag detected with same name as parent tab - this is illegal" << endl;
				this->throwGenericXMLParseError();
				result = false;
			}
			else
			{
			#endif
				if(!parsingAttributeName)
				{
					(*currentTag)->name = tagName;
				}
				//else (*currentTag)->name has already been filled in in parseTagName()

				if(!this->parseTagValueAssumingExistenceOfSubtagsAndClose(parseFileObject, (*currentTag), treeLayer))
				{//go up a level in the tree
					result = false;
				}

				//continue parsing at same level
				*currentTag = this->createNewTag(*currentTag);
				this->parseTagOpen(parseFileObject, *currentTag, parentTagName, isSubTag, treeLayer);
			#ifdef XML_PARSER_DO_NOT_ALLOW_SUBTAGS_WITH_SAME_NAME_AS_PARENT_TAG
			}
			#endif
		}
	}
	return result;
}






//Low Level
////////////////////////////////////////////////////////////////////////////////

bool XMLparserClassClass::isBlankChar(const ifstream* parseFileObject, const char c)
{
	bool result;

/*
	#define LF 0x0A
	#define CR 0x0D

	if(c == LF)
	{
		cout << 'here' << endl;

		char currentToken;
		if(!(parseFileObject->get(currentToken)))
		{
			result = false;
		}
		if(c != CR)
		{
			cout << "error: not ASCII XML file" << endl;
		}

		lineCount++;
		charCount=0;
		result = true;
	}
*/
	if(c == CHAR_NEWLINE)
	{
		lineCount++;
		charCount=0;
		result = true;
	}
	else if(c == CHAR_TAB)
	{
		result = true;
	}
	else if(c == CHAR_SPACE)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

bool XMLparserClassClass::isBlankCharTabOrNewLine(const ifstream* parseFileObject, const char c)
{
	bool result;

	if(c == CHAR_NEWLINE)
	{
		lineCount++;
		charCount=0;
		result = true;
	}
	else if(c == CHAR_TAB)
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return result;
}

void XMLparserClassClass::throwGenericXMLParseError()
{
	cout << "Error: Parsing XML File: Line = " << lineCount << ", Character = " << charCount << endl;
}

bool XMLparserClassClass::getAttribute(XMLparserTag* tag, const string attributeName, string* attributeValueFound)
{
	bool result = false;
	XMLparserAttribute* currentAttribute = tag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == attributeName)
		{
			result = true;
			*attributeValueFound = currentAttribute->value;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}

XMLparserAttribute* XMLparserClassClass::createNewAttribute(XMLparserAttribute* currentAttribute)
{
	XMLparserAttribute* newAttribute2 = new XMLparserAttribute();
	currentAttribute->nextAttribute = newAttribute2;
	currentAttribute = currentAttribute->nextAttribute;
	return currentAttribute;
}







bool XMLparserClassClass::writeXMLfileInefficient(const string xmlFileName, const XMLparserTag* firstTagInXMLfile)
{
	bool result = true;

	charCount = 0;		//this variable needs to start being used
	lineCount = 1;

	long XMLfileByteArrayCurrentPosition = 0;
	long XMLFileByteArraySize = XML_FILE_MAX_SIZE;
	char* XMLfileByteArray = new char[XMLFileByteArraySize];

	if(!this->addTagLayerToByteArrayInefficient(firstTagInXMLfile, XMLfileByteArray, &XMLfileByteArrayCurrentPosition, 0))
	{
		result = false;
	}

	XMLfileByteArray[XMLfileByteArrayCurrentPosition] = CHAR_NEWLINE;	//need to add a new line onto the end of the file
	XMLfileByteArrayCurrentPosition++;

	SHAREDvars.writeByteArrayToFile(xmlFileName, XMLfileByteArray, XMLfileByteArrayCurrentPosition);

	delete XMLfileByteArray;

	return result;
}


void XMLparserClassClass::writeXMLHeader(ofstream* writeFileObject)
{
	string headerString = "";
	headerString = headerString + "<" + STRING_TAG_XML_DEF_FULL + ">";
	for(int i = 0; i<headerString.length(); i++)
	{
		writeFileObject->put(headerString[i]);
	}
}


bool XMLparserClassClass::writeXMLfile(const string xmlFileName, const XMLparserTag* firstTagInXMLfile)
{
	bool result = true;

	ofstream writeFileObject(xmlFileName.c_str());

	#ifdef XML_WRITE_STANDARD_XML_HEADER
	this->writeXMLHeader(&writeFileObject);
	#endif

	if(!this->addTagLayerToFileObject(firstTagInXMLfile, &writeFileObject, 0))
	{
		result = false;
	}

	//Added by RBB 30 August 2009 - required for Windows SW to re-read xml files
	writeFileObject.put(CHAR_NEWLINE); //(s.cStr())[i]

	writeFileObject.close();

	return result;
}



bool XMLparserClassClass::addTagLayerToFileObject(const XMLparserTag* firstTagInCurrentLayer, ofstream* writeFileObject, const int treeLayer)
{
	bool result = true;

	const XMLparserTag* currentTag = firstTagInCurrentLayer;

	while(currentTag->nextTag != NULL)
	{
		bool tagHasAttributes = false;
		bool tagHasValue = false;
		bool tagHasLowerLevelTags = false;
		if(currentTag->firstAttribute->nextAttribute != NULL)
		{//tag has attributes
			tagHasAttributes = true;
		}
		if(currentTag->value != "")
		{//tag has value
			tagHasValue = true;
		}
		if(currentTag->firstLowerLevelTag != NULL)
		{//tag has lower level tags
			tagHasLowerLevelTags = true;
		}

		this->incrementLineAndAddTabsToFileObject(writeFileObject, treeLayer);
		string tagOpeningString = CHAR_TAG_OPEN_STR + currentTag->name;
		SHAREDvars.writeStringToFileObject(tagOpeningString, writeFileObject);
		if(tagHasAttributes)
		{
			const XMLparserAttribute* currentAttribute = currentTag->firstAttribute;
			while(currentAttribute->nextAttribute != NULL)
			{
				string attributeString = " " + currentAttribute->name + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + CHAR_TAG_ATTRIBUTE_VAL_OPEN_STR + currentAttribute->value + CHAR_TAG_ATTRIBUTE_VAL_CLOSE_STR;
				SHAREDvars.writeStringToFileObject(attributeString, writeFileObject);

				currentAttribute = currentAttribute->nextAttribute;
			}
		}

		if(tagHasLowerLevelTags || tagHasValue)
		{
			string firstTagClosingString = CHAR_TAG_CLOSE_STR;
			SHAREDvars.writeStringToFileObject(firstTagClosingString, writeFileObject);

			if(tagHasLowerLevelTags)
			{
				if(!this->addTagLayerToFileObject(currentTag->firstLowerLevelTag, writeFileObject, (treeLayer+1)))
				{
					result = false;
				}
			}
			if(tagHasValue)
			{
				this->addTabsToFileObject(writeFileObject, treeLayer);
				string tagValueString = currentTag->value;
				SHAREDvars.writeStringToFileObject(tagValueString, writeFileObject);
			}

			this->incrementLineAndAddTabsToFileObject(writeFileObject, treeLayer);
			string endTagString = CHAR_TAG_OPEN_STR;
			endTagString = endTagString + CHAR_TAG_END_STR;
			endTagString = endTagString + ((string)(currentTag->name));
			endTagString = endTagString + CHAR_TAG_CLOSE_STR;

			//endTagString[1] = CHAR_TAG_END;	 //work around as CHAR_TAG_END_STR does not work
			SHAREDvars.writeStringToFileObject(endTagString, writeFileObject);

		}
		else
		{
			string tagEndAndClose = " ";
			tagEndAndClose = tagEndAndClose + CHAR_TAG_END_STR;
			tagEndAndClose = tagEndAndClose + CHAR_TAG_CLOSE_STR;
			//tagEndAndClose[0] = CHAR_TAG_END;		//work around as CHAR_TAG_END_STR does not work
			SHAREDvars.writeStringToFileObject(tagEndAndClose, writeFileObject);

		}

		currentTag = currentTag->nextTag;
	}

	return result;

}

void XMLparserClassClass::incrementLineAndAddTabsToFileObject(ofstream* writeFileObject, const int treeLayer)
{
	writeFileObject->put(CHAR_NEWLINE); //(s.cStr())[i]

	for(int i=0; i < treeLayer; i++)
	{
		writeFileObject->put(CHAR_TAB); //(s.cStr())[i]
	}

}

void XMLparserClassClass::addTabsToFileObject(ofstream* writeFileObject, const int treeLayer)
{
	for(int i=0; i < treeLayer; i++)
	{
		writeFileObject->put(CHAR_TAB); //(s.cStr())[i]
	}

}





bool XMLparserClassClass::addTagLayerToByteArrayInefficient(const XMLparserTag* firstTagInCurrentLayer, char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer)
{
	bool result = true;

	const XMLparserTag* currentTag = firstTagInCurrentLayer;

	while(currentTag->nextTag != NULL)
	{

		bool tagHasAttributes = false;
		bool tagHasValue = false;
		bool tagHasLowerLevelTags = false;
		if(currentTag->firstAttribute->nextAttribute != NULL)
		{//tag has attributes
			tagHasAttributes = true;
		}
		if(currentTag->value != "")
		{//tag has value
			tagHasValue = true;
		}
		if(currentTag->firstLowerLevelTag != NULL)
		{//tag has lower level tags
			tagHasLowerLevelTags = true;
		}

		this->incrementLineAndAddTabsToByteArrayInefficient(XMLfileByteArray, XMLfileByteArrayCurrentPosition, treeLayer);
		string tagOpeningString = CHAR_TAG_OPEN_STR + currentTag->name;
		this->writeStringToByteArrayInefficient(tagOpeningString, XMLfileByteArray, XMLfileByteArrayCurrentPosition);
		if(tagHasAttributes)
		{
			const XMLparserAttribute* currentAttribute = currentTag->firstAttribute;
			while(currentAttribute->nextAttribute != NULL)
			{
				string attributeString = " " + currentAttribute->name + CHAR_TAG_ATTRIBUTE_VAL_EQUALS_STR + CHAR_TAG_ATTRIBUTE_VAL_OPEN_STR + currentAttribute->value + CHAR_TAG_ATTRIBUTE_VAL_CLOSE_STR;
				this->writeStringToByteArrayInefficient(attributeString, XMLfileByteArray, XMLfileByteArrayCurrentPosition);

				currentAttribute = currentAttribute->nextAttribute;
			}
		}

		if(tagHasLowerLevelTags || tagHasValue)
		{
			string firstTagClosingString = CHAR_TAG_CLOSE_STR;
			this->writeStringToByteArrayInefficient(firstTagClosingString, XMLfileByteArray, XMLfileByteArrayCurrentPosition);

			if(tagHasLowerLevelTags)
			{
				if(!this->addTagLayerToByteArrayInefficient(currentTag->firstLowerLevelTag, XMLfileByteArray, XMLfileByteArrayCurrentPosition, (treeLayer+1)))
				{
					result = false;
				}
			}
			if(tagHasValue)
			{
				this->addTabsToByteArrayInefficient(XMLfileByteArray, XMLfileByteArrayCurrentPosition, treeLayer);
				string tagValueString = currentTag->value;
				this->writeStringToByteArrayInefficient(tagValueString, XMLfileByteArray, XMLfileByteArrayCurrentPosition);
			}

			this->incrementLineAndAddTabsToByteArrayInefficient(XMLfileByteArray, XMLfileByteArrayCurrentPosition, treeLayer);
			string endTagString = CHAR_TAG_OPEN_STR;
			endTagString = endTagString + CHAR_TAG_END_STR;
			endTagString = endTagString + ((string)(currentTag->name));
			endTagString = endTagString + CHAR_TAG_CLOSE_STR;

			//endTagString[1] = CHAR_TAG_END;	 //work around as CHAR_TAG_END_STR does not work
			this->writeStringToByteArrayInefficient(endTagString, XMLfileByteArray, XMLfileByteArrayCurrentPosition);

		}
		else
		{
			string tagEndAndClose = " ";
			tagEndAndClose = tagEndAndClose + CHAR_TAG_END_STR;
			tagEndAndClose = tagEndAndClose + CHAR_TAG_CLOSE_STR;
			//tagEndAndClose[0] = CHAR_TAG_END;		//work around as CHAR_TAG_END_STR does not work
			this->writeStringToByteArrayInefficient(tagEndAndClose, XMLfileByteArray, XMLfileByteArrayCurrentPosition);

		}

		currentTag = currentTag->nextTag;
	}

	return result;

}

void XMLparserClassClass::incrementLineAndAddTabsToByteArrayInefficient(char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer)
{
	XMLfileByteArray[*XMLfileByteArrayCurrentPosition] = CHAR_NEWLINE;
	*XMLfileByteArrayCurrentPosition = (*XMLfileByteArrayCurrentPosition + 1);

	for(int i=0; i < treeLayer; i++)
	{
		XMLfileByteArray[*XMLfileByteArrayCurrentPosition] = CHAR_TAB;
		*XMLfileByteArrayCurrentPosition = (*XMLfileByteArrayCurrentPosition + 1);
	}

}

void XMLparserClassClass::addTabsToByteArrayInefficient(char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer)
{
	for(int i=0; i < treeLayer; i++)
	{
		XMLfileByteArray[*XMLfileByteArrayCurrentPosition] = CHAR_TAB;
		*XMLfileByteArrayCurrentPosition = (*XMLfileByteArrayCurrentPosition + 1);
	}

}


void XMLparserClassClass::writeStringToByteArrayInefficient(string s, char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition)
{
	for(int i=0; i < s.size(); i++)
	{
		XMLfileByteArray[*XMLfileByteArrayCurrentPosition] = s[i]; //(s.cStr())[i]
		*XMLfileByteArrayCurrentPosition = (*XMLfileByteArrayCurrentPosition + 1);
	}
}







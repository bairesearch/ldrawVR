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
 * File Name: XMLparserClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: XML Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_XML_PARSER_CLASS
#define HEADER_XML_PARSER_CLASS

#include "SHAREDglobalDefs.hpp"
#include "SHAREDvars.hpp"	//required for writeByteArrayToFile/writeStringToFileObject


#define XML_WRITE_STANDARD_XML_HEADER

//#define XML_PARSER_DISPLAY_COMMENTS_WHEN_PARSING

//#define XML_DEBUG

#ifdef USE_GIA
	/*yet untested without GIA;
	#define XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_INSIDE_ATTRIBUTE_VALUE
	#define XML_PARSER_DO_NOT_ALLOW_TABS_OR_NEWLINES_WITHIN_TAG_VALUE
	*/
#else
	#define XML_PARSER_DO_NOT_ALLOW_SUBTAGS_WITH_SAME_NAME_AS_PARENT_TAG		//new 5 April 2012 - to support Stanford NLP Core XML files with embedded <coreference><coreference> tags, where the first of these should actually be "<coreferences>"
	#define XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_WITHOUT_PRECEEDING_SPACE	//new 5 April 2012 - to support Stanford NLP Core XML files with end tag immediately following tag name, without a preceeding space eg <basic-dependencies/>
	#define XML_PARSER_DO_NOT_ALLOW_CHAR_TAG_END_SLASH_INSIDE_ATTRIBUTE_VALUE
	#define XML_PARSER_DO_NOT_ALLOW_TABS_OR_NEWLINES_WITHIN_TAG_VALUE
	#define XML_PARSER_DO_NOT_ALLOW_TAG_VALUE_TO_BE_DEFINED_AFTER_SUBTAG		//added 3i16a for Doxygen support, eg " *" in "<type><ref refid="classGIArelation" kindref="compound">GIArelation</ref> *</type>"
#endif

#define XML_FILE_MAX_SIZE (100000000)		//100MB




class XMLparserAttribute
{
private:
	/*There are currently no private attributes of this class*/
public:

	XMLparserAttribute(void); // constructor
	~XMLparserAttribute();	// and destructor.

	string name;
	string value;

	XMLparserAttribute* nextAttribute;


};





class XMLparserTag
{
private:
	/*There are currently no private attributes of this class*/
public:

	XMLparserTag(void); // constructor
	~XMLparserTag();	// and destructor.

	string name;
	string value;

	XMLparserTag* nextTag;
	XMLparserTag* firstLowerLevelTag;

	XMLparserAttribute* firstAttribute;
	XMLparserAttribute* currentAttribute;	//temporary variable used for attribute list creation

};


class XMLparserClassClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: XMLparserTag* parseTagDownALevel(XMLparserTag* currentTag, const string sectionTagName, bool* result);

	public: bool readXMLfile(const string xmlFileName, XMLparserTag* firstTagInXMLfile);
	private: bool parseTagOpen(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer);
		private: bool parseTagName(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer);
			private: bool parseTagAttributeName(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, int treeLayer);
				private: bool parseTagAttributeValue(ifstream* parseFileObject, XMLparserTag* currentTag, const string parentTagName, const bool isSubTag, const int treeLayer);
			private: bool parseTagValueAssumingExistenceOfSubtagsAndClose(ifstream* parseFileObject, XMLparserTag* currentTag, int treeLayer);
			private: bool parseTagComment(ifstream* parseFileObject, const char type);
			private: bool processTagClose(ifstream* parseFileObject, XMLparserTag** currentTag, const string parentTagName, const bool isSubTag, int treeLayer, const bool endTagFound, const bool parsingAttributeName, const string tagName);
			private: XMLparserTag* createNewTag(XMLparserTag* currentTag);

//Low Level
	private: bool isBlankChar(const ifstream* parseFileObject, const char c);
	private: bool isBlankCharTabOrNewLine(const ifstream* parseFileObject, const char c);
	private: void throwGenericXMLParseError();
	public: bool getAttribute(XMLparserTag* tag, const string attributeName, string* attributeValueFound);
	public: XMLparserAttribute* createNewAttribute(XMLparserAttribute* currentAttribute);

	private: bool writeXMLfileInefficient(const string xmlFileName, const XMLparserTag* firstTagInXMLfile);
	private: void writeXMLHeader(ofstream* writeFileObject);	//not used?
	public: bool writeXMLfile(const string xmlFileName, const XMLparserTag* firstTagInXMLfile);
		public: bool addTagLayerToFileObject(const XMLparserTag* firstTagInCurrentLayer, ofstream* writeFileObject, const int treeLayer);
			private: void incrementLineAndAddTabsToFileObject(ofstream* writeFileObject, const int treeLayer);
			private: void addTabsToFileObject(ofstream* writeFileObject, const int treeLayer);
		private: bool addTagLayerToByteArrayInefficient(const XMLparserTag* firstTagInCurrentLayer, char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer);
			private: void incrementLineAndAddTabsToByteArrayInefficient(char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer);
			private: void addTabsToByteArrayInefficient(char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition, const int treeLayer);
			private: void writeStringToByteArrayInefficient(string s, char* XMLfileByteArray, long* XMLfileByteArrayCurrentPosition);
};

#endif

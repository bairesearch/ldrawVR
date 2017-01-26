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
 * File Name: XMLrulesClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: XML Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_XML_RULES_CLASS
#define HEADER_XML_RULES_CLASS

#include "SHAREDglobalDefs.hpp"
#include "XMLparserClass.hpp"
#include "SHAREDvars.hpp"

#define ANN_RULES_XML_FILE_NAME "ANNrules.xml"
#define CS_RULES_XML_FILE_NAME "CSrules.xml"
#define OR_RULES_XML_FILE_NAME "ATORrules.xml"
#define GIA_RULES_XML_FILE_NAME "GIArules.xml"
#define NLC_RULES_XML_FILE_NAME "NLCrules.xml"

#define RULES_XML_TAG_rules ((string)"rules")
#define RULES_XML_TAG_miscellaneousItem ((string)"miscellaneousItem")
#define RULES_XML_TAG_sprite ((string)"sprite")
#ifdef USE_CS
#define RULES_XML_TAG_draw ((string)"draw")
#endif
#ifdef USE_GIA
#define RULES_XML_TAG_draw ((string)"draw")
#endif
#ifdef USE_OR
#define RULES_XML_TAG_objectRecognition ((string)"objectRecognition")
#endif

#define RULES_XML_ATTRIBUTE_name ((string)"name")
#define RULES_XML_ATTRIBUTE_stringValue ((string)"stringValue")
#define RULES_XML_ATTRIBUTE_fractionalValue ((string)"fractionalValue")

class XMLrulesClass
{
private:
	/*There are currently no private attributes of this class*/
public:

	XMLrulesClass(void); // constructor
	~XMLrulesClass();	// and destructor.

	string name;	//attribute1;

	string stringValue;	//attribute2;
	double fractionalValue;
	int attribute4;
	int attribute5;
	int attribute6;
	int attribute7;
	int attribute8;

	XMLrulesClass* next;
};

//High Level

class XMLrulesClassClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
#ifdef USE_NLC
	public: bool parseNLCrulesXMLfile();
#endif

#ifdef USE_GIA
	public: bool parseGIArulesXMLfile();
		private: bool parseGIARulesTag(XMLparserTag* currentTag);
#endif

#ifdef USE_CS
	public: bool parseCSrulesXMLfile();
		private: bool parseCSRulesTag(XMLparserTag* currentTag);
#endif

#ifdef USE_OR
	public: bool parseORrulesXMLfile();
		private: bool parseORrulesTag(XMLparserTag* currentTag);
#endif

	public: bool parseANNrulesXMLfile();
		private: bool parseANNrulesTag(XMLparserTag* currentTag);

	private: bool addRulesClassObjectsBasedOnSectionTag(XMLparserTag* currentTag, const string sectionTagName, XMLrulesClass* firstReferenceToObjectClass, const string tagName, const int numberOfAttributes, const string attributeName1, const string attributeName2, const string attributeName3, const string attributeName4, const string attributeName5, const string attributeName6, const string attributeName7, const string attributeName8);
	private: bool addRulesClassObjectBasedOnTag(XMLparserTag* currentTag, XMLrulesClass* currentReferenceToObjectClass, const string tagName, const int numberOfAttributes, const string attributeName1, const string attributeName2, const string attributeName3, const string attributeName4, const string attributeName5, const string attributeName6, const string attributeName7, const string attributeName8);
};

extern XMLrulesClass* ANNrulesSprite;	//common sprite xml file is ANNrules.xml
#ifdef USE_CS
extern XMLrulesClass* CSrulesSprite;
extern XMLrulesClass* CSrulesDraw;
extern XMLparserTag* CSfirstTagInXMLfile;
#endif
#ifdef USE_GIA
extern XMLrulesClass* GIArulesSprite;
extern XMLrulesClass* GIArulesDraw;
extern XMLparserTag* GIAfirstTagInXMLfile;
#endif
#ifdef USE_OR
extern XMLrulesClass* ORrulesObjectRecognition;
#endif
#ifdef USE_NLC
extern XMLparserTag* NLCfirstTagInXMLfile;
#endif

#endif

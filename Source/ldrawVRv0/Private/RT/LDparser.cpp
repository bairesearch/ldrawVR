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
 * File Name: LDparser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "LDparser.hpp"
#ifdef USE_LRRC
//#include "LRRCparser.hpp"
#endif


bool LDparserClass::parseFile(string parseFileName, LDreference* initialReference, LDreference* parentReference, const bool recurseIntoPartsDir)
{
	bool result = true;

	int charCount = 0;
	int lineCount = 0;

	char c;	//current character being read in

		/*new parameters added to this->parseFile() 18-mar-07*/
	int type = REFERENCE_TYPE_UNDEFINED;
	bool readingVertex1X = false;
	bool readingVertex1Y = false;
	bool readingVertex1Z = false;
	bool readingVertex2X = false;
	bool readingVertex2Y = false;
	bool readingVertex2Z = false;
	bool readingVertex3X = false;
	bool readingVertex3Y = false;
	bool readingVertex3Z = false;
	bool readingVertex4X = false;
	bool readingVertex4Y = false;
	bool readingVertex4Z = false;
	string vertex1X = "";
	string vertex1Y = "";
	string vertex1Z = "";
	string vertex2X = "";
	string vertex2Y = "";
	string vertex2Z = "";
	string vertex3X = "";
	string vertex3Y = "";
	string vertex3Z = "";
	string vertex4X = "";
	string vertex4Y = "";
	string vertex4Z = "";
	bool finishedReadingReference = false;

	bool readingType = true;
	bool readingColour = false;
	bool readingCoordX = false;
   	bool readingCoordY = false;
	bool readingCoordZ = false;
	bool readingRotation1 = false;
	bool readingRotation2 = false;
	bool readingRotation3 = false;
	bool readingRotation4 = false;
	bool readingRotation5 = false;
	bool readingRotation6 = false;
	bool readingRotation7 = false;
	bool readingRotation8 = false;
	bool readingRotation9 = false;
	bool readingSubPartFileName = false;
	bool waitingForNewLine = false;
	string colour = "";
	string coordX = "";
	string coordY = "";
	string coordZ = "";
	string rotation1 = "";
	string rotation2 = "";
	string rotation3 = "";
	string rotation4 = "";
	string rotation5 = "";
	string rotation6 = "";
	string rotation7 = "";
	string rotation8 = "";
	string rotation9 = "";
	string subPartFileName = "";

	LDreference* currentReference = initialReference;

	ifstream parseFileObject;
	parseFileObject.open(parseFileName.c_str());
	bool managedToFindFile = false;

	if(!parseFileObject.rdbuf()->is_open())
	{
		parseFileObject.close();

		if(recurseIntoPartsDir)
		{
			string parseFileNameInPartsDir = DEFAULT_PARTS_DIRECTORY_FULL_PATH;
			parseFileName = this->removeWhiteSpaceFromString(parseFileName);
			parseFileNameInPartsDir = parseFileNameInPartsDir + parseFileName;
			parseFileObject.open(parseFileNameInPartsDir.c_str());
			if(!parseFileObject.rdbuf()->is_open( ))
			{
				parseFileObject.close();

				parseFileNameInPartsDir = DEFAULT_PARTS_PRIMITIVES_DIRECTORY_FULL_PATH;
				parseFileName = this->removeWhiteSpaceFromString(parseFileName);
				parseFileNameInPartsDir = parseFileNameInPartsDir + parseFileName;
				parseFileObject.open(parseFileNameInPartsDir.c_str());
				if(!parseFileObject.rdbuf()->is_open())
				{
					parseFileObject.close();

					//file does not exist in parts directory.
					managedToFindFile = false;
					result = false;
				}
				else
				{
					//gone into prim file.
					result = true;
					managedToFindFile = true;
				}
			}
			else
			{
				result = true;
				managedToFindFile = true;
			}

		}
		else
		{
			//file does not exist in current directory.
			managedToFindFile = false;
			result = false;
		}
	}
	else
	{
		result = true;
		managedToFindFile = true;
	}




	if(managedToFindFile)
	{
		while(parseFileObject.get(c))
		{
			charCount++;

			if((waitingForNewLine) && (c == '\n'))
			{
				lineCount++;
				waitingForNewLine = false;
				readingType = true;
			}
			else if(waitingForNewLine)
			{
				//do nothing, still waiting for new line
			}
			else if(readingType)
			{
				bool whiteSpaceLineDetectedInsteadOfReference = false;

				if(c == '0')
				{//comment detected
					waitingForNewLine = true;
					type = REFERENCE_TYPE_COMMENT;
				}
				else if(c == '1')
				{//submodel detected
					waitingForNewLine = false;
					type = REFERENCE_TYPE_SUBMODEL;
				}
				else if(c == '2')
				{//submodel detected
					waitingForNewLine = false;
					type = REFERENCE_TYPE_LINE;
				}
				else if(c == '3')
				{//tri primative detected
					waitingForNewLine = false;
					type = REFERENCE_TYPE_TRI;
				}
				else if(c == '4')
				{//quad primative detected
					waitingForNewLine = false;
					type = REFERENCE_TYPE_QUAD;
				}
				else if(c == '5')
				{//quad primative detected
					waitingForNewLine = true;
					type = REFERENCE_TYPE_OPTIONALLINE;
				}
				else
				{
					if((c == '\n') && (LD_PARSER_ALLOW_WHITE_SPACE))
					{
						lineCount++;
						//white space removal required for messy dat files from ldraw parts library
						whiteSpaceLineDetectedInsteadOfReference = true;
					}
					else if((c == '\t') && (LD_PARSER_ALLOW_WHITE_SPACE))
					{
						//white space removal required for messy dat files from ldraw parts library
						whiteSpaceLineDetectedInsteadOfReference = true;
					}
					else if((c == ' ') && (LD_PARSER_ALLOW_WHITE_SPACE))
					{
						//white space removal required for messy dat files from ldraw parts library
						whiteSpaceLineDetectedInsteadOfReference = true;
					}
					else
					{
						type = REFERENCE_TYPE_UNDEFINED;
						cout << "1" << endl;
						cout << "readingType = " << readingType << endl;
						cout << "Invalid ModelDetails. Corrupted file has been detected. LDreference Type Illegal. \n" << endl;
						cout << "character charCount = "<< charCount << endl;
						cout << "lineCount = " << lineCount << endl;
						cout << "type = " << type << endl;
						cout << "parentReference->name = " << parentReference->name << endl;
						cout << "parseFileName = " << parseFileName << endl;
						cout << "character = " << c << endl;
						cout << "(int)character = " << (int)c << endl;
						exit(0);
					}
				}

				if(!whiteSpaceLineDetectedInsteadOfReference)
				{

					parseFileObject.get(c); //gets a blank/space
					if((c != ' ') && (type != REFERENCE_TYPE_COMMENT))
					{
						cout << "2" << endl;
						cout << "\n Invalid ModelDetails. Corrupted file has been detected. Value After LDreference Type is not space \n" << endl;
						cout << "\n c = "<< c << endl;
						cout << "\n character charCount = "<< charCount << endl;
						cout << "\n lineCount = " << lineCount << endl;
						cout << "\n type = " << type << endl;
						cout << "parseFileName = " << parseFileName << endl;
						exit(0);
					}

					colour = "";
					coordX = "";
					coordY = "";
					coordZ = "";
					rotation1 = "";
					rotation2 = "";
					rotation3 = "";
					rotation4 = "";
					rotation5 = "";
					rotation6 = "";
					rotation7 = "";
					rotation8 = "";
					rotation9 = "";

					vertex1X = "";
					vertex1Y = "";
					vertex1Z = "";
					vertex2X = "";
					vertex2Y = "";
					vertex2Z = "";
					vertex3X = "";
					vertex3Y = "";
					vertex3Z = "";
					vertex4X = "";
					vertex4Y = "";
					vertex4Z = "";

					subPartFileName = "";

					readingType = false;
					readingColour = true;
				}
			}
			else
			{
				if((type == REFERENCE_TYPE_LINE) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_TRI))
				{

					if((readingColour) && (c == ' '))
					{
						readingColour = false;
						readingVertex1X = true;
					}
					else if(readingColour)
					{
						colour = colour + c;
					}

					else if((readingVertex1X) && (c == ' '))
					{
						readingVertex1X = false;
						readingVertex1Y = true;
					}
					else if(readingVertex1X)
					{
						vertex1X = vertex1X + c;
					}
					else if((readingVertex1Y) && (c == ' '))
					{
						readingVertex1Y = false;
						readingVertex1Z = true;
					}
					else if(readingVertex1Y)
					{
						vertex1Y = vertex1Y + c;
					}
					else if((readingVertex1Z) && (c == ' '))
					{
						readingVertex1Z = false;
						readingVertex2X = true;
					}
					else if(readingVertex1Z)
					{
						vertex1Z = vertex1Z + c;
					}

					else if((readingVertex2X) && (c == ' '))
					{
						readingVertex2X = false;
						readingVertex2Y = true;
					}
					else if(readingVertex2X)
					{
						vertex2X = vertex2X + c;
					}
					else if((readingVertex2Y) && (c == ' '))
					{
						readingVertex2Y = false;
						readingVertex2Z = true;
					}
					else if(readingVertex2Y)
					{
						vertex2Y = vertex2Y + c;
					}
					else if((readingVertex2Z) && ((c == ' ') || (c == '\n')))
					{
						readingVertex2Z = false;
						if(type == REFERENCE_TYPE_LINE)
						{
						#ifdef LD_USE_STRICT_PARSER
							if(c == '\n')
							{
							}
							else
							{
								cout << "3" << endl;
								cout << "Invalid ModelDetails. Corrupted file has been detected. LDreference Type Line corrupt. \n" << endl;
								cout << "character charCount = "<< charCount << endl;
								cout << "lineCount = " << lineCount << endl;
								cout << "type = " << type << endl;
								cout << "filename = " << parentReference->name << endl;
								exit(0);
							}
						#endif
							lineCount++;
							finishedReadingReference = true;
							readingType = true;
						}
						else if((type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_TRI))
						{
							if(c == ' ')
							{
							}
							else
							{
								cout << "4" << endl;
								cout << "Invalid ModelDetails. Corrupted file has been detected. LDreference Type Quad/Tri corrupt. \n" << endl;
								cout << "character charCount = "<< charCount << endl;
								cout << "lineCount = " << lineCount << endl;
								cout << "type = " << type << endl;
								cout << "filename = " << parentReference->name << endl;
								exit(0);
							}
							readingVertex3X = true;
						}
					}
					else if(readingVertex2Z)
					{
						vertex2Z = vertex2Z + c;
					}

					else if((readingVertex3X) && (c == ' '))
					{
						readingVertex3X = false;
						readingVertex3Y = true;
					}
					else if(readingVertex3X)
					{
						vertex3X = vertex3X + c;
					}
					else if((readingVertex3Y) && (c == ' '))
					{
						readingVertex3Y = false;
						readingVertex3Z = true;
					}
					else if(readingVertex3Y)
					{
						vertex3Y = vertex3Y + c;
					}
					else if((readingVertex3Z) && ((c == ' ') || (c == '\n')))
					{
						readingVertex3Z = false;
						if(type == REFERENCE_TYPE_QUAD)
						{
							if(c == ' ')
							{
							}
							else
							{
								cout << "5" << endl;
								cout << "Invalid ModelDetails. Corrupted file has been detected. LDreference Type Quad corrupt. \n" << endl;
								cout << "character charCount = "<< charCount << endl;
								cout << "lineCount = " << lineCount << endl;
								cout << "type = " << type << endl;
								cout << "filename = " << parentReference->name << endl;
								exit(0);
							}
							readingVertex4X = true;
						}
						else if(type == REFERENCE_TYPE_TRI)
						{

						#ifdef LD_USE_STRICT_PARSER
							if(c == '\n')
							{
							}
							else
							{
								cout << "6" << endl;
								cout << "Invalid ModelDetails. Corrupted file has been detected. LDreference Type Tri corrupt. \n" << endl;
								cout << "character charCount = "<< charCount << endl;
								cout << "lineCount = " << lineCount << endl;
								cout << "type = " << type << endl;
								cout << "filename = " << parentReference->name << endl;
								exit(0);
							}
						#endif
							lineCount++;
							finishedReadingReference = true;
							readingType = true;
						}
					}
					else if(readingVertex3Z)
					{
						vertex3Z = vertex3Z + c;
					}

					else if((readingVertex4X) && (c == ' '))
					{
						readingVertex4X = false;
						readingVertex4Y = true;
					}
					else if(readingVertex4X)
					{
						vertex4X = vertex4X + c;
					}
					else if((readingVertex4Y) && (c == ' '))
					{
						readingVertex4Y = false;
						readingVertex4Z = true;
					}
					else if(readingVertex4Y)
					{
						vertex4Y = vertex4Y + c;
					}
					else if((readingVertex4Z) && (c == '\n'))
					{
						lineCount++;
						readingVertex4Z = false;
						finishedReadingReference = true;
						readingType = true;
					}
					else if(readingVertex4Z)
					{
						vertex4Z = vertex4Z + c;
					}

					if(finishedReadingReference == true)
					{
						finishedReadingReference = false;

						//3. Record LDreference information into current LDreference object
						currentReference->type = type;

						currentReference->colour = (unsigned int)(SHAREDvars.convertStringToDouble(colour));
					#ifdef USE_LD_ABSOLUTE_COLOUR
						if(currentReference->colour == DAT_FILE_DEFAULT_COLOUR)
						{
							currentReference->absoluteColour = parentReference->absoluteColour;
						}
						else
						{
							currentReference->absoluteColour = currentReference->colour;
						}
					#endif

						currentReference->vertex1relativePosition.x = (SHAREDvars.convertStringToDouble(vertex1X));
						currentReference->vertex1relativePosition.y = (SHAREDvars.convertStringToDouble(vertex1Y));
						currentReference->vertex1relativePosition.z = (SHAREDvars.convertStringToDouble(vertex1Z));
						currentReference->vertex2relativePosition.x = (SHAREDvars.convertStringToDouble(vertex2X));
						currentReference->vertex2relativePosition.y = (SHAREDvars.convertStringToDouble(vertex2Y));
						currentReference->vertex2relativePosition.z = (SHAREDvars.convertStringToDouble(vertex2Z));
						currentReference->vertex3relativePosition.x = (SHAREDvars.convertStringToDouble(vertex3X));
						currentReference->vertex3relativePosition.y = (SHAREDvars.convertStringToDouble(vertex3Y));
						currentReference->vertex3relativePosition.z = (SHAREDvars.convertStringToDouble(vertex3Z));
						currentReference->vertex4relativePosition.x = (SHAREDvars.convertStringToDouble(vertex4X));
						currentReference->vertex4relativePosition.y = (SHAREDvars.convertStringToDouble(vertex4Y));
						currentReference->vertex4relativePosition.z = (SHAREDvars.convertStringToDouble(vertex4Z));

					#ifndef NO_ROTATION_OF_MODELS_ALLOWED

						currentReference->vertex1absolutePosition.x = this->calcModXPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
						currentReference->vertex1absolutePosition.y = this->calcModYPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
						currentReference->vertex1absolutePosition.z = this->calcModZPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;
						currentReference->vertex2absolutePosition.x = this->calcModXPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
						currentReference->vertex2absolutePosition.y = this->calcModYPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
						currentReference->vertex2absolutePosition.z = this->calcModZPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;
						currentReference->vertex3absolutePosition.x = this->calcModXPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
						currentReference->vertex3absolutePosition.y = this->calcModYPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
						currentReference->vertex3absolutePosition.z = this->calcModZPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;
						currentReference->vertex4absolutePosition.x = this->calcModXPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
						currentReference->vertex4absolutePosition.y = this->calcModYPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
						currentReference->vertex4absolutePosition.z = this->calcModZPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;

						#ifdef USE_UNREAL
						currentReference->vertex1intermediatePosition.x = calcModXPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.x;
						currentReference->vertex1intermediatePosition.y = calcModYPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.y;
						currentReference->vertex1intermediatePosition.z = calcModZPosBasedUponRotate(&(currentReference->vertex1relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.z;
						currentReference->vertex2intermediatePosition.x = calcModXPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.x;
						currentReference->vertex2intermediatePosition.y = calcModYPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.y;
						currentReference->vertex2intermediatePosition.z = calcModZPosBasedUponRotate(&(currentReference->vertex2relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.z;
						currentReference->vertex3intermediatePosition.x = calcModXPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.x;
						currentReference->vertex3intermediatePosition.y = calcModYPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.y;
						currentReference->vertex3intermediatePosition.z = calcModZPosBasedUponRotate(&(currentReference->vertex3relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.z;
						currentReference->vertex4intermediatePosition.x = calcModXPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.x;
						currentReference->vertex4intermediatePosition.y = calcModYPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.y;
						currentReference->vertex4intermediatePosition.z = calcModZPosBasedUponRotate(&(currentReference->vertex4relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.z;
						#endif
					#else
						currentReference->vertex1absolutePosition.x = currentReference->vertex1relativePosition.x + parentReference->absolutePosition.x;
						currentReference->vertex1absolutePosition.y = currentReference->vertex1relativePosition.y + parentReference->absolutePosition.y;
						currentReference->vertex1absolutePosition.z = currentReference->vertex1relativePosition.z + parentReference->absolutePosition.z;
						currentReference->vertex2absolutePosition.x = currentReference->vertex2relativePosition.x + parentReference->absolutePosition.x;
						currentReference->vertex2absolutePosition.y = currentReference->vertex2relativePosition.y + parentReference->absolutePosition.y;
						currentReference->vertex2absolutePosition.z = currentReference->vertex2relativePosition.z + parentReference->absolutePosition.z;
						currentReference->vertex3absolutePosition.x = currentReference->vertex3relativePosition.x + parentReference->absolutePosition.x;
						currentReference->vertex3absolutePosition.y = currentReference->vertex3relativePosition.y + parentReference->absolutePosition.y;
						currentReference->vertex3absolutePosition.z = currentReference->vertex3relativePosition.z + parentReference->absolutePosition.z;
						currentReference->vertex4absolutePosition.x = currentReference->vertex4relativePosition.x + parentReference->absolutePosition.x;
						currentReference->vertex4absolutePosition.y = currentReference->vertex4relativePosition.y + parentReference->absolutePosition.y;
						currentReference->vertex4absolutePosition.z = currentReference->vertex4relativePosition.z + parentReference->absolutePosition.z;
					#endif
						#ifdef LD_DEBUG
						/*
						cout << "" << endl;
						cout << "currentReference->type = " << currentReference->type << endl;
						cout << "currentReference->colour = " << currentReference->colour << endl;
						cout << "currentReference->name = " << currentReference->name << endl;
						cout << "currentReference->vertex1relativePosition.x = " << currentReference->vertex1relativePosition.x << endl;
						cout << "currentReference->vertex1relativePosition.y = " << currentReference->vertex1relativePosition.y << endl;
						cout << "currentReference->vertex1relativePosition.z = " << currentReference->vertex1relativePosition.z << endl;
						cout << "currentReference->vertex2relativePosition.x = " << currentReference->vertex2relativePosition.x << endl;
						cout << "currentReference->vertex2relativePosition.y = " << currentReference->vertex2relativePosition.y << endl;
						cout << "currentReference->vertex2relativePosition.z = " << currentReference->vertex2relativePosition.z << endl;
						cout << "currentReference->vertex3relativePosition.x = " << currentReference->vertex3relativePosition.x << endl;
						cout << "currentReference->vertex3relativePosition.y = " << currentReference->vertex3relativePosition.y << endl;
						cout << "currentReference->vertex3relativePosition.z = " << currentReference->vertex3relativePosition.z << endl;
						cout << "currentReference->vertex4relativePosition.x = " << currentReference->vertex4relativePosition.x << endl;
						cout << "currentReference->vertex4relativePosition.y = " << currentReference->vertex4relativePosition.y << endl;
						cout << "currentReference->vertex4relativePosition.z = " << currentReference->vertex4relativePosition.z << endl;
						cout << "currentReference->vertex1absolutePosition.x = " << currentReference->vertex1absolutePosition.x << endl;
						cout << "currentReference->vertex1absolutePosition.y = " << currentReference->vertex1absolutePosition.y << endl;
						cout << "currentReference->vertex1absolutePosition.z = " << currentReference->vertex1absolutePosition.z << endl;
						cout << "currentReference->vertex2absolutePosition.x = " << currentReference->vertex2absolutePosition.x << endl;
						cout << "currentReference->vertex2absolutePosition.y = " << currentReference->vertex2absolutePosition.y << endl;
						cout << "currentReference->vertex2absolutePosition.z = " << currentReference->vertex2absolutePosition.z << endl;
						cout << "currentReference->vertex3absolutePosition.x = " << currentReference->vertex3absolutePosition.x << endl;
						cout << "currentReference->vertex3absolutePosition.y = " << currentReference->vertex3absolutePosition.y << endl;
						cout << "currentReference->vertex3absolutePosition.z = " << currentReference->vertex3absolutePosition.z << endl;
						cout << "currentReference->vertex4absolutePosition.x = " << currentReference->vertex4absolutePosition.x << endl;
						cout << "currentReference->vertex4absolutePosition.y = " << currentReference->vertex4absolutePosition.y << endl;
						cout << "currentReference->vertex4absolutePosition.z = " << currentReference->vertex4absolutePosition.z << endl;
						cout << "" << endl;
						*/
						#endif

						//4. finalise LDreference object
						//code to create a new reference object
						LDreference* nextReference = new LDreference();
						currentReference -> next = nextReference;
						currentReference = currentReference->next;
					}
				}
				else if(type == REFERENCE_TYPE_SUBMODEL)
				{
					if((readingColour) && (c == ' '))
					{
						readingColour = false;
						readingCoordX = true;
					}
					else if(readingColour)
					{
						colour = colour + c;
					}
					else if((readingCoordX) && (c == ' '))
					{
						readingCoordX = false;
						readingCoordY = true;
					}
					else if(readingCoordX)
					{
						coordX = coordX + c;
					}
					else if((readingCoordY) && (c == ' '))
					{
						readingCoordY = false;
						readingCoordZ = true;
					}
					else if(readingCoordY)
					{
						coordY = coordY + c;
					}
					else if((readingCoordZ) && (c == ' '))
					{
						readingCoordZ = false;
						readingRotation1 = true;
					}
					else if(readingCoordZ)
					{
						coordZ = coordZ + c;
					}
					else if((readingRotation1) && (c == ' '))
					{
						readingRotation1 = false;
						readingRotation2 = true;
					}
					else if(readingRotation1)
					{
						rotation1 = rotation1 + c;
					}
					else if((readingRotation2) && (c == ' '))
					{
						readingRotation2 = false;
						readingRotation3 = true;
					}
					else if(readingRotation2)
					{
						rotation2 = rotation2 + c;
					}
					else if((readingRotation3) && (c == ' '))
					{
						readingRotation3 = false;
						readingRotation4 = true;
					}
					else if(readingRotation3)
					{
						rotation3 = rotation3 + c;
					}
					else if((readingRotation4) && (c == ' '))
					{
						readingRotation4 = false;
						readingRotation5 = true;
					}
					else if(readingRotation4)
					{
						rotation4 = rotation4 + c;
					}
					else if((readingRotation5) && (c == ' '))
					{
						readingRotation5 = false;
						readingRotation6 = true;
					}
					else if(readingRotation5)
					{
						rotation5 = rotation5 + c;
					}
					else if((readingRotation6) && (c == ' '))
					{
						readingRotation6 = false;
						readingRotation7 = true;
					}
					else if(readingRotation6)
					{
						rotation6 = rotation6 + c;
					}
					else if((readingRotation7) && (c == ' '))
					{
						readingRotation7 = false;
						readingRotation8 = true;
					}
					else if(readingRotation7)
					{
						rotation7 = rotation7 + c;
					}
					else if((readingRotation8) && (c == ' '))
					{
						readingRotation8 = false;
						readingRotation9 = true;
					}
					else if(readingRotation8)
					{
						rotation8 = rotation8 + c;
					}
					else if((readingRotation9) && (c == ' '))
					{
						readingRotation9 = false;
						readingSubPartFileName = true;
					}
					else if(readingRotation9)
					{
						rotation9 = rotation9 + c;
					}
					else if((readingSubPartFileName) && (c == '\n'))
					{
						lineCount++;
						readingSubPartFileName = false;
						finishedReadingReference = true;
						readingType = true;
					}
					else if(readingSubPartFileName)
					{
						subPartFileName = subPartFileName + c;
					}

					if(finishedReadingReference == true)
					{
						finishedReadingReference = false;


						//3. Record LDreference information into current LDreference object
						currentReference->type = type;
						currentReference->colour = (unsigned int)(SHAREDvars.convertStringToDouble(colour));
						#ifdef USE_UNREAL
						currentReference->relativeColourString = colour;
						#endif
					#ifdef USE_LD_ABSOLUTE_COLOUR
						if(currentReference->colour == DAT_FILE_DEFAULT_COLOUR)
						{
							currentReference->absoluteColour = parentReference->absoluteColour;
						}
						else
						{
							currentReference->absoluteColour = currentReference->colour;
						}
					#endif

						currentReference->relativePosition.x = (SHAREDvars.convertStringToDouble(coordX));
						currentReference->relativePosition.y = (SHAREDvars.convertStringToDouble(coordY));
						currentReference->relativePosition.z = (SHAREDvars.convertStringToDouble(coordZ));

							//26-jan-07 change; take into account rotation of parent reference in calculation of child reference absolute coordinates;
					#ifndef NO_ROTATION_OF_MODELS_ALLOWED

						currentReference->deformationMatrix.a.x = (SHAREDvars.convertStringToDouble(rotation1));
						currentReference->deformationMatrix.b.x = (SHAREDvars.convertStringToDouble(rotation2));
						currentReference->deformationMatrix.c.x = (SHAREDvars.convertStringToDouble(rotation3));
						currentReference->deformationMatrix.a.y = (SHAREDvars.convertStringToDouble(rotation4));
						currentReference->deformationMatrix.b.y = (SHAREDvars.convertStringToDouble(rotation5));
						currentReference->deformationMatrix.c.y = (SHAREDvars.convertStringToDouble(rotation6));
						currentReference->deformationMatrix.a.z = (SHAREDvars.convertStringToDouble(rotation7));
						currentReference->deformationMatrix.b.z = (SHAREDvars.convertStringToDouble(rotation8));
						currentReference->deformationMatrix.c.z = (SHAREDvars.convertStringToDouble(rotation9));

						SHAREDvector.multiplyMatricies(&(currentReference->absoluteDeformationMatrix), &(parentReference->absoluteDeformationMatrix), &(currentReference->deformationMatrix));

						currentReference->absolutePosition.x = this->calcModXPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.x;
						currentReference->absolutePosition.y = this->calcModYPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.y;
						currentReference->absolutePosition.z = this->calcModZPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->absoluteDeformationMatrix)) + parentReference->absolutePosition.z;

						#ifdef USE_UNREAL
						if(!(parentReference->isTopLevel))
						{
							SHAREDvector.multiplyMatricies(&(currentReference->intermediateDeformationMatrix), &(parentReference->intermediateDeformationMatrix), &(currentReference->deformationMatrix));

							currentReference->intermediatePosition.x = calcModXPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.x;
							currentReference->intermediatePosition.y = calcModYPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.y;
							currentReference->intermediatePosition.z = calcModZPosBasedUponRotate(&(currentReference->relativePosition), &(parentReference->intermediateDeformationMatrix)) + parentReference->intermediatePosition.z;
						}
						#endif
					#else
						currentReference->absolutePosition.x = currentReference->relativePosition.x + parentReference->absolutePosition.x;
						currentReference->absolutePosition.y = currentReference->relativePosition.y + parentReference->absolutePosition.y;
						currentReference->absolutePosition.z = currentReference->relativePosition.z + parentReference->absolutePosition.z;
					#endif


						currentReference->name = subPartFileName;


					#ifndef USE_OLC
						//1. Perform Recursion into sub files in designated search directory (default = current directory) if possible

							//code to create a new reference object
						LDreference* subModelReference = new LDreference(true);
						currentReference->firstReferenceWithinSubModel = subModelReference;

						#ifdef USE_LRRC
						currentReference->subModelDetails = subModelReference->subModelDetails;
						subModelReference->subModelDetails = NULL;	//added aug 08
						#endif
						subModelReference->isSubModelReference = false;	//added aug 08

						if(this->parseFile(subPartFileName, currentReference->firstReferenceWithinSubModel, currentReference, recurseIntoPartsDir))
						{
							#ifdef LD_DEBUG
							//cout << "successfully parsed; currentReference->name = " <<  currentReference->name << endl;
							#endif
							currentReference->isSubModelReference = true;

							#ifdef USE_LRRC
							//NEW 26-3-06 copy environment relevant child object values into parent object
							copyEnvironmentRelevantChildUnitDetailsIntoParentObject(parentReference->subModelDetails, currentReference->subModelDetails);
							#endif
						}
						else
						{
							#ifdef USE_LRRC
							//2. As a custom submodel was not detected, the program updates the unitDetails object based upon LDreference name (subPartFileName)
							updateUnitDetails(subPartFileName, parentReference->subModelDetails);
							currentReference->subModelDetails = NULL;	//added aug 08
							#endif
							/*clear memory of currentReference->subModelReference and currentReference->subModelDetails as submodel does not exist*/
							delete subModelReference;
							currentReference->firstReferenceWithinSubModel = NULL;	//added aug 08
							currentReference->isSubModelReference = false;	//added aug 08
						}
					#endif

						/*
						cout << "" << endl;
						cout << "currentReference->type = " << currentReference->type << endl;
						cout << "currentReference->colour = " << currentReference->colour << endl;
						cout << "currentReference->relativePosition.x = " << currentReference->relativePosition.x << endl;
						cout << "currentReference->relativePosition.y = " << currentReference->relativePosition.y << endl;
						cout << "currentReference->relativePosition.z = " << currentReference->relativePosition.z << endl;
						cout << "currentReference->deformationMatrix.a.x = " << currentReference->deformationMatrix.a.x << endl;
						cout << "currentReference->deformationMatrix.b.x = " << currentReference->deformationMatrix.b.x << endl;
						cout << "currentReference->deformationMatrix.c.x = " << currentReference->deformationMatrix.c.x << endl;
						cout << "currentReference->deformationMatrix.a.y = " << currentReference->deformationMatrix.a.y << endl;
						cout << "currentReference->deformationMatrix.b.y = " << currentReference->deformationMatrix.b.y << endl;
						cout << "currentReference->deformationMatrix.c.y = " << currentReference->deformationMatrix.c.y << endl;
						cout << "currentReference->deformationMatrix.a.z = " << currentReference->deformationMatrix.a.z << endl;
						cout << "currentReference->deformationMatrix.b.z = " << currentReference->deformationMatrix.b.z << endl;
						cout << "currentReference->deformationMatrix.c.z = " << currentReference->deformationMatrix.c.z << endl;
						cout << "currentReference->name = " << currentReference->name << endl;
						*/

						//4. finalise LDreference object

						//code to create a new reference object
						LDreference* nextReference = new LDreference();
						currentReference -> next = nextReference;
						currentReference = currentReference->next;
					}
				}
			}
		}

		parseFileObject.close();
	}


	return result;
}




double LDparserClass::calcModXPosBasedUponRotate(const vec* childRelativePosition, const mat* parentReferenceDeformationMatrix)
{
	double xPosBasedUponRotatedParent;

	xPosBasedUponRotatedParent = ((parentReferenceDeformationMatrix->a.x* childRelativePosition->x) + (parentReferenceDeformationMatrix->b.x* childRelativePosition->y) + (parentReferenceDeformationMatrix->c.x* childRelativePosition->z));

	return xPosBasedUponRotatedParent;
}

double LDparserClass::calcModYPosBasedUponRotate(const vec* childRelativePosition, const mat* parentReferenceDeformationMatrix)
{
	double yPosBasedUponRotatedParent;

	yPosBasedUponRotatedParent = ((parentReferenceDeformationMatrix->a.y* childRelativePosition->x) + (parentReferenceDeformationMatrix->b.y* childRelativePosition->y) + (parentReferenceDeformationMatrix->c.y* childRelativePosition->z));

	return yPosBasedUponRotatedParent;
}


double LDparserClass::calcModZPosBasedUponRotate(const vec* childRelativePosition, const mat* parentReferenceDeformationMatrix)
{
	double zPosBasedUponRotatedParent;

	zPosBasedUponRotatedParent = ((parentReferenceDeformationMatrix->a.z* childRelativePosition->x) + (parentReferenceDeformationMatrix->b.z* childRelativePosition->y) + (parentReferenceDeformationMatrix->c.z* childRelativePosition->z));

	return zPosBasedUponRotatedParent;
}

string LDparserClass::removeWhiteSpaceFromString(const string s)
{
	string sNew = "";
	for(int i=0; i<s.length(); i++)
	{
		if(s[i] != CHAR_SPACE)
		{
			sNew = sNew + s[i];
		}
	}

	return sNew;
}


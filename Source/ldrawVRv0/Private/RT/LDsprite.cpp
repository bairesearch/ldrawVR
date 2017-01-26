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
 * File Name: LDsprite.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 * Description: Contains common sprite commands
 *
 *******************************************************************************/


//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "LDsprite.hpp"
#ifdef USE_LRRC
//#include "LRRCrules.hpp"
#endif

int SPRITE_DEFAULT_COLOUR;

static double SPRITE_TEXT_DICE_SCALE_FACTOR;
static double SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT;
static double SPRITE_DICE_WIDTH_IN_LDRAW_UNITS;
static double SPRITE_TEXT_DICE_Y_OFFSET;
	static double SPRITE_LINE_SPACING_RATIO;	//derivable
	static double SPRITE_DICE_SPACING_SIZE_IN_LDRAW_UNITS;			//derivable

static double SPRITE_TEXT_SCALE_FACTOR;
static double SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS;
static double SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS;
static double SPRITE_CHAR_SPACING_RATIO;
static double SPRITE_TEXT_PADDING_IN_LDRAW_UNITS;
	static double SPRITE_LINE_SPACING_SIZE_IN_LDRAW_UNITS;			//derivable
	static double SPRITE_CHAR_SPACING_SIZE_IN_LDRAW_UNITS;			//derivable

static double SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS;
static double SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS;

static string SPRITE_NAME_START;
static string SPRITE_EXTENSION;
static string SPRITE_CHARACTER_START;
static string SPRITE_CHARACTER_EXTENSION;

double spriteTextKernelArray[ANSI_NUMBER_CHARACTERS];


void LDspriteClass::fillInLDspriteExternVariables()
{
	#ifdef USE_CS
		XMLrulesClass* currentReferenceRulesClass = CSrulesSprite;
	#elif defined USE_GIA
		XMLrulesClass* currentReferenceRulesClass = GIArulesSprite;
	#elif defined USE_LRRC
		XMLrulesClass* currentReferenceRulesClass = LRRCrulesSprite;
	#else
		XMLrulesClass* currentReferenceRulesClass = ANNrulesSprite;
	#endif

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == SPRITE_DEFAULT_COLOUR_NAME)
		{
			SPRITE_DEFAULT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}



		else if(currentReferenceRulesClass->name == SPRITE_TEXT_DICE_SCALE_FACTOR_NAME)
		{
			SPRITE_TEXT_DICE_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT_NAME)
		{
			SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_DICE_WIDTH_IN_LDRAW_UNITS_NAME)
		{
			SPRITE_DICE_WIDTH_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXT_DICE_Y_OFFSET_NAME)
		{
			SPRITE_TEXT_DICE_Y_OFFSET = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == SPRITE_TEXT_SCALE_FACTOR_NAME)
		{
			SPRITE_TEXT_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS_NAME)
		{
			SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS_NAME)
		{
			SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_CHAR_SPACING_RATIO_NAME)
		{
			SPRITE_CHAR_SPACING_RATIO = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXT_PADDING_IN_LDRAW_UNITS_NAME)
		{
			SPRITE_TEXT_PADDING_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}



		else if(currentReferenceRulesClass->name == SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS_NAME)
		{
			SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS_NAME)
		{
			SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS = currentReferenceRulesClass->fractionalValue;
		}


		else if(currentReferenceRulesClass->name == SPRITE_NAME_START_NAME)
		{
			SPRITE_NAME_START = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_EXTENSION_NAME)
		{
			SPRITE_EXTENSION = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_CHARACTER_START_NAME)
		{
			SPRITE_CHARACTER_START = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_CHARACTER_EXTENSION_NAME)
		{
			SPRITE_CHARACTER_EXTENSION = currentReferenceRulesClass->stringValue;
		}

		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	SPRITE_LINE_SPACING_RATIO = (SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT);
	SPRITE_LINE_SPACING_SIZE_IN_LDRAW_UNITS = ((SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS*SPRITE_LINE_SPACING_RATIO)*SPRITE_TEXT_SCALE_FACTOR);

	SPRITE_CHAR_SPACING_SIZE_IN_LDRAW_UNITS = ((SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS* SPRITE_CHAR_SPACING_RATIO)*SPRITE_TEXT_SCALE_FACTOR);
	SPRITE_DICE_SPACING_SIZE_IN_LDRAW_UNITS = ((SPRITE_DICE_WIDTH_IN_LDRAW_UNITS* SPRITE_CHAR_SPACING_RATIO)*SPRITE_TEXT_DICE_SCALE_FACTOR);


	spriteTextKernelArray[48] = 7.0;
	spriteTextKernelArray[49] = 3.0;
	spriteTextKernelArray[50] = 5.0;
	spriteTextKernelArray[51] = 6.0;
	spriteTextKernelArray[52] = 6.0;
	spriteTextKernelArray[53] = 5.0;
	spriteTextKernelArray[54] = 5.5;
	spriteTextKernelArray[55] = 5.5;
	spriteTextKernelArray[56] = 5.0;
	spriteTextKernelArray[57] = 6.0;
	spriteTextKernelArray[45] = 3.5;
	spriteTextKernelArray[43] = 5.0;
	spriteTextKernelArray[61] = 5.0;
	spriteTextKernelArray[65] = 6.0;
	spriteTextKernelArray[66] = 6.0;
	spriteTextKernelArray[67] = 6.0;
	spriteTextKernelArray[68] = 6.0;
	spriteTextKernelArray[69] = 5.0;
	spriteTextKernelArray[70] = 5.0;
	spriteTextKernelArray[71] = 6.0;
	spriteTextKernelArray[72] = 5.0;
	spriteTextKernelArray[73] = 1.0;
	spriteTextKernelArray[74] = 5.0;
	spriteTextKernelArray[75] = 6.0;
	spriteTextKernelArray[76] = 5.0;
	spriteTextKernelArray[77] = 6.0;
	spriteTextKernelArray[78] = 6.0;
	spriteTextKernelArray[79] = 7.0;
	spriteTextKernelArray[80] = 5.0;
	spriteTextKernelArray[81] = 7.0;
	spriteTextKernelArray[82] = 6.0;
	spriteTextKernelArray[83] = 4.5;
	spriteTextKernelArray[84] = 5.0;
	spriteTextKernelArray[85] = 5.0;
	spriteTextKernelArray[86] = 5.5;
	spriteTextKernelArray[87] = 8.0;
	spriteTextKernelArray[88] = 6.0;
	spriteTextKernelArray[89] = 7.0;
	spriteTextKernelArray[90] = 5.0;
	spriteTextKernelArray[46] = 2.0;


}




/*Secondary sprite routines*/

string LDspriteClass::LDcreateSpriteReferenceName(const int spriteIndex, string sceneFileName)
{
	string spriteReferenceFileName = "";

	//creates sprite reference name spriteReferenceName = [SPRITE_NAME_START][spriteIndex][sceneFileName]

	spriteReferenceFileName = sceneFileName;
	int positionOfFullStop = sceneFileName.find(CHAR_FULLSTOP);
	if(positionOfFullStop != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		spriteReferenceFileName = sceneFileName.substr(0, positionOfFullStop);
		#ifdef LD_DEBUG
		//cout << "sceneFileName = " << sceneFileName << endl;
		//cout << "spriteReferenceFileName without fullstop = " << spriteReferenceFileName << endl;
		#endif
	}

	spriteReferenceFileName = spriteReferenceFileName + SPRITE_NAME_START;
	spriteReferenceFileName = spriteReferenceFileName + SHAREDvars.convertIntToString(spriteIndex);
	spriteReferenceFileName = spriteReferenceFileName + SPRITE_EXTENSION;

	//create the "spritex.ldr" file that will be referenced by the scene file
	return spriteReferenceFileName;
}

LDreference* LDspriteClass::LDaddBasicTextualSpriteStringToReferenceList(string spriteTextString, LDreference* currentReference, vec* position, int* numSpritesAdded, const bool addIndividualSprites, int colour, double scale)
{
	bool result = true;

	LDreference* spriteSubmodelCurrentReference = currentReference;

	vec spriteSubmodelCurrentReferencePosition;
	spriteSubmodelCurrentReferencePosition.x = 0.0 - ((double)SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS*scale / 2.0);		//NB can remove* scale
	spriteSubmodelCurrentReferencePosition.y = 0.0 + (double)SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS*scale;			//NB can remove* scale
	spriteSubmodelCurrentReferencePosition.z = 0.0;


	//common local variables for loop
	int spriteLineNumber = 0;

	for(int spriteTextIndex = 0; spriteTextIndex < spriteTextString.length(); spriteTextIndex++)
	{
		/*convert lowercase text to uppercase text:*/
		if((spriteTextString[spriteTextIndex] >= 'a') && (spriteTextString[spriteTextIndex] <= 'z'))
		{
			spriteTextString[spriteTextIndex] = spriteTextString[spriteTextIndex] - ANSI_DECIMAL_DIFF_BETWEEN_CASE_TEXT;
		}

		if(spriteTextString[spriteTextIndex] == '\n')
		{
			spriteLineNumber++;
			spriteSubmodelCurrentReferencePosition.y  = spriteSubmodelCurrentReferencePosition.y + (double)SPRITE_LINE_SPACING_SIZE_IN_LDRAW_UNITS*scale;		//NB can remove* scale
			spriteSubmodelCurrentReferencePosition.x = 0.0 - ((double)SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS*scale / 2.0);						//NB can remove* scale
		}
		else if(((spriteTextString[spriteTextIndex] >= '0') && (spriteTextString[spriteTextIndex] <= '9')) || (spriteTextString[spriteTextIndex] == CHAR_PLUS) || (spriteTextString[spriteTextIndex] == CHAR_MINUS) || (spriteTextString[spriteTextIndex] == CHAR_EQUALS) || (spriteTextString[spriteTextIndex] == CHAR_FULLSTOP))
		{

			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*scale*SPRITE_TEXT_SCALE_FACTOR;

			if(!addIndividualSprites)
			{
				vec tempAbsPos;
				SHAREDvector.addVectors(&tempAbsPos, &spriteSubmodelCurrentReferencePosition, position);
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &tempAbsPos, spriteTextString[spriteTextIndex], colour, scale*SPRITE_TEXT_SCALE_FACTOR);
			}
			else
			{
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &spriteSubmodelCurrentReferencePosition, spriteTextString[spriteTextIndex], colour, scale*SPRITE_TEXT_SCALE_FACTOR);
			}

			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*scale*SPRITE_TEXT_SCALE_FACTOR;

			LDreference* newReference = new LDreference();
			spriteSubmodelCurrentReference -> next = newReference;
			spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;
			*numSpritesAdded = *numSpritesAdded + 1;
		}
		else if((spriteTextString[spriteTextIndex] >= 'A') && (spriteTextString[spriteTextIndex] <= 'Z'))
		{
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*scale*SPRITE_TEXT_SCALE_FACTOR;

			if(!addIndividualSprites)
			{
				vec tempAbsPos;
				SHAREDvector.addVectors(&tempAbsPos, &spriteSubmodelCurrentReferencePosition, position);
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &tempAbsPos, spriteTextString[spriteTextIndex], colour, scale*SPRITE_TEXT_SCALE_FACTOR);
			}
			else
			{
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &spriteSubmodelCurrentReferencePosition, spriteTextString[spriteTextIndex], colour, scale*SPRITE_TEXT_SCALE_FACTOR);
			}
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*scale*SPRITE_TEXT_SCALE_FACTOR;

			LDreference* newReference = new LDreference();
			spriteSubmodelCurrentReference -> next = newReference;
			spriteSubmodelCurrentReference = spriteSubmodelCurrentReference -> next;
			*numSpritesAdded = *numSpritesAdded + 1;
		}
		else if(spriteTextString[spriteTextIndex] == ' ')
		{
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + ((double)SPRITE_CHAR_SPACING_SIZE_IN_LDRAW_UNITS/2.0)*scale*SPRITE_TEXT_SCALE_FACTOR;
		}
		else
		{

		}
	}

	return spriteSubmodelCurrentReference;
}



LDreference* LDspriteClass::LDaddTextualSpriteInfoStringToReferenceList(LDreference* unitReference, string spriteTextString, int spriteColourArray[], LDreference* spriteSubmodelInitialReference, const int spriteNumberOfLines, int* numSpritesAdded, const bool addIndividualSprites)
{
	bool result = true;

	LDreference* spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	vec spriteSubmodelCurrentReferencePosition;
	spriteSubmodelCurrentReferencePosition.x = 0.0 - (SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS / 2);
	spriteSubmodelCurrentReferencePosition.y = 0.0 + SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS;
	spriteSubmodelCurrentReferencePosition.z = 0.0;


	//common local variables for loop
	double spriteCurrentCharacterSpacing;
	int spriteLineNumber = 0;

	for(int spriteTextIndex = 0; spriteTextIndex < spriteTextString.length(); spriteTextIndex++)
	{
		/*convert lowercase text to uppercase text:*/
		if((spriteTextString[spriteTextIndex] >= 'a') && (spriteTextString[spriteTextIndex] <= 'z'))
		{
			spriteTextString[spriteTextIndex] = spriteTextString[spriteTextIndex] - ANSI_DECIMAL_DIFF_BETWEEN_CASE_TEXT;
		}

		if(spriteTextString[spriteTextIndex] == '\n')
		{
			spriteLineNumber++;
			if(spriteLineNumber >= spriteNumberOfLines)
			{
				cout << "error; (spriteLineNumber >= spriteNumberOfLines)" << endl;
				result = false;
			}
			spriteSubmodelCurrentReferencePosition.y  = spriteSubmodelCurrentReferencePosition.y + SPRITE_LINE_SPACING_SIZE_IN_LDRAW_UNITS;
			spriteSubmodelCurrentReferencePosition.x = 0.0 - (SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS / 2);
		}
		else if(((spriteTextString[spriteTextIndex] >= '0') && (spriteTextString[spriteTextIndex] <= '9')) || (spriteTextString[spriteTextIndex] == CHAR_PLUS) || (spriteTextString[spriteTextIndex] == CHAR_MINUS) || (spriteTextString[spriteTextIndex] == CHAR_EQUALS) || (spriteTextString[spriteTextIndex] == CHAR_FULLSTOP))
		{
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*SPRITE_TEXT_SCALE_FACTOR;

			if(!addIndividualSprites)
			{
				vec tempAbsPos;
				SHAREDvector.addVectors(&tempAbsPos, &spriteSubmodelCurrentReferencePosition, &(unitReference->absolutePosition));
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &tempAbsPos, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}
			else
			{
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &spriteSubmodelCurrentReferencePosition, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}

			//spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + spriteCurrentCharacterSpacing;
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*SPRITE_TEXT_SCALE_FACTOR;

			LDreference* newReference = new LDreference();
			spriteSubmodelCurrentReference->next = newReference;
			spriteSubmodelCurrentReference = spriteSubmodelCurrentReference->next;
			*numSpritesAdded = *numSpritesAdded + 1;
		}
		else if((spriteTextString[spriteTextIndex] >= 'A') && (spriteTextString[spriteTextIndex] <= 'Z'))
		{
			//spriteCurrentCharacterSpacing = SPRITE_CHAR_SPACING_SIZE_IN_LDRAW_UNITS;
			//spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + spriteCurrentCharacterSpacing/2;
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*SPRITE_TEXT_SCALE_FACTOR;

			if(!addIndividualSprites)
			{
				vec tempAbsPos;
				SHAREDvector.addVectors(&tempAbsPos, &spriteSubmodelCurrentReferencePosition, &(unitReference->absolutePosition));
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &tempAbsPos, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}
			else
			{
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &spriteSubmodelCurrentReferencePosition, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}
			//spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + spriteCurrentCharacterSpacing/2;
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (spriteTextKernelArray[spriteTextString[spriteTextIndex]]+SPRITE_TEXT_PADDING_IN_LDRAW_UNITS)*SPRITE_TEXT_SCALE_FACTOR;

			LDreference* newReference = new LDreference();
			spriteSubmodelCurrentReference->next = newReference;
			spriteSubmodelCurrentReference = spriteSubmodelCurrentReference->next;
			*numSpritesAdded = *numSpritesAdded + 1;

		}
		else if(((unsigned char)spriteTextString[spriteTextIndex] > (unsigned char)SPRITE_CHARACTER_DICE_OFFSET) && ((unsigned char)spriteTextString[spriteTextIndex] <= (unsigned char)(SPRITE_CHARACTER_DICE_OFFSET+MAX_ATTACK_DEFENCE_LEVEL)))
		{
			spriteCurrentCharacterSpacing = SPRITE_DICE_SPACING_SIZE_IN_LDRAW_UNITS;
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + spriteCurrentCharacterSpacing/2;

			if(!addIndividualSprites)
			{
				vec tempAbsPos;
				SHAREDvector.addVectors(&tempAbsPos, &spriteSubmodelCurrentReferencePosition, &(unitReference->absolutePosition));
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &tempAbsPos, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}
			else
			{
				this->LDspriteSubmodelFillTextualReference(spriteSubmodelCurrentReference, &spriteSubmodelCurrentReferencePosition, spriteTextString[spriteTextIndex], spriteColourArray[spriteLineNumber], SPRITE_TEXT_SCALE_FACTOR);
			}
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + spriteCurrentCharacterSpacing;

			LDreference* newReference = new LDreference();
			spriteSubmodelCurrentReference->next = newReference;
			spriteSubmodelCurrentReference = spriteSubmodelCurrentReference->next;
			*numSpritesAdded = *numSpritesAdded + 1;
		}
		else if(spriteTextString[spriteTextIndex] == ' ')
		{
			spriteSubmodelCurrentReferencePosition.x = spriteSubmodelCurrentReferencePosition.x + (SPRITE_CHAR_SPACING_SIZE_IN_LDRAW_UNITS/2);
		}
		else
		{
			printf("\nunknown character - system must exit (character = %c %d)", spriteTextString[spriteTextIndex], spriteTextString[spriteTextIndex]);
			/*
			char temp = (char)SPRITE_CHARACTER_DICE_OFFSET;
			char temp2 = (char)(SPRITE_CHARACTER_DICE_OFFSET+MAX_ATTACK_DEFENCE_LEVEL);
			printf("\nunknown character - system must exit (character = %c %d)", spriteTextString[spriteTextIndex], spriteTextString[spriteTextIndex]);
			printf("\nNB SPRITE_CHARACTER_DICE_OFFSET = %c %d", temp, temp);
			printf("\nNB (SPRITE_CHARACTER_DICE_OFFSET+MAX_ATTACK_DEFENCE_LEVEL) = %c %d", temp2, temp2);
			*/
			exit(0);
		}
	}

	//spriteSubmodelFinalReference = spriteSubmodelCurrentReference;

	return spriteSubmodelCurrentReference;
}





bool LDspriteClass::LDaddSpriteToSpriteReferenceList(const vec* spriteSceneCoords, const vec* eyeCoords, LDreference* spriteListInitialReference, const string spriteReferenceFileName, const int spriteDefaultColour, double spriteScaleFactor)
{
	//add sprite to spriteByteArray (replace sprite of sprite index, spriteIndex, if it already exists)

	bool result = true;

	LDreference spriteReference;

	spriteReference.colour = spriteDefaultColour;
	spriteReference.type = REFERENCE_TYPE_SUBMODEL;
	mat spriteRotationMatrix;

	SHAREDvector.createIdentityMatrix(&spriteRotationMatrix);
	SHAREDvector.scaleMatrix(&spriteRotationMatrix, spriteScaleFactor);

	SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteReference.deformationMatrix), &(spriteRotationMatrix));

	spriteReference.relativePosition.x = spriteSceneCoords->x;
	spriteReference.relativePosition.y = spriteSceneCoords->y;
	spriteReference.relativePosition.z = spriteSceneCoords->z;
	spriteReference.name = spriteReferenceFileName;

	if(LDreferenceManipulation.search1DrefListNameAndColourFindRef(spriteListInitialReference, &spriteReference))
	{
		if(!LDreferenceManipulation.search1DrefListNameAndColourReplaceRef(spriteListInitialReference, &spriteReference, &spriteReference))
		{
			cout << "error: cannot replace LDreference in Sprite LDreference List" << endl;
			cout << "\t spriteReference.colour  = \n" << spriteReference.colour  << endl;
			cout << "\t spriteReference.name = " << spriteReference.name << endl;
			result = false;
		}
	}
	else
	{
		if(!LDreferenceManipulation.search1DrefListAddReference(spriteListInitialReference, &spriteReference))
		{
			cout << "error: cannot add LDreference to Sprite LDreference List" << endl;
			cout << "\t spriteReference.colour  = \n" << spriteReference.colour  << endl;
			cout << "\t spriteReference.name = " << spriteReference.name << endl;
			result = false;
		}
	}

	return result;
}





/*this is not complete yet - just use an Identity matrix - ie do not rotate the sprite according to the user POV*/
void LDspriteClass::LDgenerateSpriteRotationMatrix(const vec* spriteSceneCoords, const vec* eyeCoords, mat* spriteRotationMatrix)
{
	//TEMP - this is not complete
	SHAREDvector.createIdentityMatrix(spriteRotationMatrix);		//CHECK THIS
}


void LDspriteClass::LDspriteSubmodelFillTextualReference(LDreference* spriteSubmodelCurrentReference, vec* spriteParagraphCurrentPosition, const char characterToWrite, int spriteColour, double scale)
{
	string referenceNameStart = SPRITE_CHARACTER_START;
	string referenceNameEnd = SPRITE_CHARACTER_EXTENSION;
	spriteSubmodelCurrentReference->colour = spriteColour;
	spriteSubmodelCurrentReference->type = REFERENCE_TYPE_SUBMODEL;

	spriteSubmodelCurrentReference->relativePosition.x = spriteParagraphCurrentPosition->x;
	spriteSubmodelCurrentReference->relativePosition.y = spriteParagraphCurrentPosition->y;
	spriteSubmodelCurrentReference->relativePosition.z = spriteParagraphCurrentPosition->z;

	mat currentDeformationMatrix;

	string tempString = "";

	if(((unsigned char)characterToWrite > (unsigned char)SPRITE_CHARACTER_DICE_OFFSET) && ((unsigned char)characterToWrite <= (unsigned char)(SPRITE_CHARACTER_DICE_OFFSET+MAX_ATTACK_DEFENCE_LEVEL)))
	{
		/*no change to currentDeformationMatrix*/
		SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);

		SHAREDvector.scaleMatrix(&currentDeformationMatrix, SPRITE_TEXT_DICE_SCALE_FACTOR);
		spriteSubmodelCurrentReference->relativePosition.y = spriteSubmodelCurrentReference->relativePosition.y + SPRITE_TEXT_DICE_Y_OFFSET;
		spriteSubmodelCurrentReference->colour = spriteSubmodelCurrentReference->colour + DAT_FILE_COLOUR_TRANPARENT_MOD;
		tempString = "d";
		int diceValue = ((unsigned char)characterToWrite - (unsigned char)SPRITE_CHARACTER_DICE_OFFSET)*2;
		if(diceValue > 9)
		{
			char tempChar = (1 + SPRITE_CHARACTER_NUM_OFFSET);
			tempString = tempString + tempChar;
			tempChar = (diceValue - 10) + SPRITE_CHARACTER_NUM_OFFSET;
			tempString = tempString + tempChar;
		}
		else
		{
			char tempChar = (diceValue + SPRITE_CHARACTER_NUM_OFFSET);
			tempString = tempString + tempChar;
		}

	}
	else
	{
		SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);
		SHAREDvector.scaleMatrix(&currentDeformationMatrix, scale);
		tempString = characterToWrite;
	}

	SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteSubmodelCurrentReference->deformationMatrix), &(currentDeformationMatrix));

	spriteSubmodelCurrentReference->name = referenceNameStart + tempString + referenceNameEnd;
}

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
 * File Name: LDsprite.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 * Description: Contains common sprite commands
 *
 *******************************************************************************/


#ifndef HEADER_LD_SPRITE
#define HEADER_LD_SPRITE

#include "LDreferenceClass.hpp"
#include "SHAREDglobalDefs.hpp"
#include "LDreferenceManipulation.hpp"
#include "SHAREDvector.hpp"
#include "SHAREDvars.hpp"	//required for writeByteArrayToFile
#include "XMLrulesClass.hpp"
#include "math.h"

#define CPLUSPLUSERRORCORRECTION2 (500)


#define CHAR_PLUS ('+')
#define CHAR_MINUS ('-')
#define CHAR_EQUALS ('=')
#define CHAR_FULLSTOP ('.')

#define DEFAULT_LDRAW_SUBMODEL_COLOUR (16)


#define ANN_OR_LRRC_SPRITE_HEADER_NAME "\n0 Start ann.exe Sprites\n"
#define ANN_OR_LRRC_SPRITE_TRAILER_NAME "0 End ann.exe Sprites\n"
#define ANN_OR_LRRC_SPRITE_HEADER_NUM_LINES (2)
#define ANN_OR_LRRC_SPRITE_TRAILER_NUM_LINES (1)
#define ANN_OR_LRRC_SPRITE_HEADER_LENGTH 25
#define ANN_OR_LRRC_SPRITE_TRAILER_LENGTH 22





#define SPRITE_CHARACTER_DICE_OFFSET (192)
#define SPRITE_CHARACTER_NUM_OFFSET (48)
#define MAX_ATTACK_DEFENCE_LEVEL (6)
#define MAX_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED (5)			/*CHECK THIS: this will change once more spritetextdx.dat files are created - ie spritetextd12.dat [change to 6] and spritetextd20.dat [change to 10] are created*/
#define MIN_DICE_ATTACK_DEFENCE_VALUE_SUPPORTED (2)			/*CHECK THIS: this will change once more spritetextd2.dat file is created [change to 1]*/



#define ANSI_DECIMAL_DIFF_BETWEEN_CASE_TEXT (32)
#define ANSI_NUMBER_CHARACTERS (256)

#define SPRITE_INDEX_MAGNITUDE (5)

#define SPRITE_TEXT_DICE_SCALE_FACTOR_NAME "SPRITE_TEXT_DICE_SCALE_FACTOR" //assume dice have already been appropriately scaled
#define SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT_NAME "SPRITE_LINE_SPACING_RATIO_WITHOUT_DICE_PRESENT"
#define SPRITE_DICE_WIDTH_IN_LDRAW_UNITS_NAME "SPRITE_DICE_WIDTH_IN_LDRAW_UNITS"
#define SPRITE_TEXT_DICE_Y_OFFSET_NAME "SPRITE_TEXT_DICE_Y_OFFSET"

#define SPRITE_TEXT_SCALE_FACTOR_NAME "SPRITE_TEXT_SCALE_FACTOR"	//this can easily be changed
#define SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS_NAME "SPRITE_WIDTH_OF_CHARS_IN_LDRAW_UNITS"
#define SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS_NAME "SPRITE_HEIGHT_OF_CHARS_IN_LDRAW_UNITS"
#define SPRITE_CHAR_SPACING_RATIO_NAME "SPRITE_CHAR_SPACING_RATIO"
#define SPRITE_TEXT_PADDING_IN_LDRAW_UNITS_NAME "SPRITE_TEXT_PADDING_IN_LDRAW_UNITS"

#define SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS_NAME "SPRITES_FLOATING_WIDTH_IN_LDRAW_UNITS"
#define SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS_NAME "SPRITES_FLOATING_HEIGHT_IN_LDRAW_UNITS"


#define SPRITES_DISPLAY_DICE
//#define SPRITE_ALWAYS_ADD_TEXT


#define SPRITE_NAME_START_NAME "SPRITE_NAME_START"
#define SPRITE_EXTENSION_NAME  "SPRITE_EXTENSION"
#define SPRITE_CHARACTER_START_NAME "SPRITE_CHARACTER_START"
#define SPRITE_CHARACTER_EXTENSION_NAME "SPRITE_CHARACTER_EXTENSION"		//sprite submodel extension
/*
#define SPRITE_NAME_START "sprite"
#define SPRITE_EXTENSION  ".ldr"
#define SPRITE_CHARACTER_START "spritetext"
#define SPRITE_CHARACTER_EXTENSION ".dat"		//sprite submodel extension
*/


#define SPRITE_DEFAULT_COLOUR_NAME "SPRITE_DEFAULT_COLOUR"
//#define SPRITE_DEFAULT_COLOUR (DAT_FILE_DEFAULT_COLOUR)

extern int SPRITE_DEFAULT_COLOUR;

class LDspriteClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: SHAREDvectorClass SHAREDvector;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	public: void fillInLDspriteExternVariables();

	public: string LDcreateSpriteReferenceName(const int spriteIndex, string sceneFileName);
	public: LDreference* LDaddTextualSpriteInfoStringToReferenceList(LDreference* unitReference, string spriteTextString, int spriteColourArray[], LDreference* spriteSubmodelInitialReference, const int spriteNumberOfLines, int* numSpritesAdded, const bool addIndividualSprites);
	public: LDreference* LDaddBasicTextualSpriteStringToReferenceList(string spriteTextString, LDreference* currentReference, vec* position, int* numSpritesAdded, const bool addIndividualSprites, int colour, double scale);


	private: bool LDaddSpriteToSpriteReferenceList(const vec* spriteSceneCoords, const vec* eyeCoords, LDreference* spriteListInitialReference, const string spriteReferenceFileName, const int spriteDefaultColour, double spriteScaleFactor);

/*low level sprite routines*/
	private: void LDgenerateSpriteRotationMatrix(const vec* spriteSceneCoords, const vec* eyeCoords, mat* spriteRotationMatrix);
	private: void LDspriteSubmodelFillTextualReference(LDreference* spriteSubmodelCurrentReference, vec* spriteParagraphCurrentPosition, const char characterToWrite, int spriteColour, double scale);
};

#endif

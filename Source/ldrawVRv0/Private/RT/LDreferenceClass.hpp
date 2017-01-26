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
 * File Name: LDreferenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Ldraw Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


/*LDreferenceClass.cpp: Defines a class for references to submodels/parts/units within a scene file*/

#ifndef HEADER_LD_REFERENCE_CLASS
#define HEADER_LD_REFERENCE_CLASS


#include "SHAREDglobalDefs.hpp"
#include "SHAREDvars.hpp"
#ifdef USE_LRRC
//#include "LRRCmodelClass.hpp"
#endif

#ifdef USE_RT
	#define USE_LD_ABSOLUTE_COLOUR
#endif
#ifdef USE_ANN	//for opengl support
	#define USE_LD_ABSOLUTE_COLOUR
#endif

#define TAL_FILE_COLOUR_BLACK_RGB "000000"
#define TAL_FILE_COLOUR_BLUE_RGB "0000F0"
#define TAL_FILE_COLOUR_GREEN_RGB "00C000"
#define TAL_FILE_COLOUR_CYAN_RGB "00F0F0"
#define TAL_FILE_COLOUR_RED_RGB "F00000"
#define TAL_FILE_COLOUR_MAGENTA_RGB "FF00FF"
#define TAL_FILE_COLOUR_BROWN_RGB "C06000"
#define TAL_FILE_COLOUR_LIGHTGREY_RGB "C0C0C0"
#define TAL_FILE_COLOUR_DARKGREY_RGB "606060"
#define TAL_FILE_COLOUR_YELLOW_RGB "FFFF00"
#define TAL_FILE_COLOUR_WHITE_RGB "FFFFFF"
#define TAL_FILE_COLOUR_ORANGE_RGB "FF0600"
#define TAL_FILE_COLOUR_PURPLE_RGB "A000FF"
#define TAL_FILE_COLOUR_DARKBLUE_RGB "000070"
#define TAL_FILE_COLOUR_DARKGREEN_RGB "007000"
#define TAL_FILE_COLOUR_DARKRED_RGB "700000"
#define TAL_FILE_COLOUR_AQUA_RGB "007070"
#define TAL_FILE_COLOUR_DEFAULT_RGB "CCCCCC"

#define DAT_FILE_COLOUR_BLACK_RGB_R (0)
#define DAT_FILE_COLOUR_BLUE_RGB_R (0)
#define DAT_FILE_COLOUR_GREEN_RGB_R (0)
#define DAT_FILE_COLOUR_CYAN_RGB_R (0)
#define DAT_FILE_COLOUR_RED_RGB_R (240)
#define DAT_FILE_COLOUR_MAGENTA_RGB_R (255)
#define DAT_FILE_COLOUR_BROWN_RGB_R (192)
#define DAT_FILE_COLOUR_LIGHTGREY_RGB_R (192)
#define DAT_FILE_COLOUR_DARKGREY_RGB_R (96)
#define DAT_FILE_COLOUR_YELLOW_RGB_R (255)
#define DAT_FILE_COLOUR_WHITE_RGB_R (255)
#define DAT_FILE_COLOUR_ORANGE_RGB_R (255)
#define DAT_FILE_COLOUR_PURPLE_RGB_R (160)
#define DAT_FILE_COLOUR_DARKBLUE_RGB_R (0)
#define DAT_FILE_COLOUR_DARKGREEN_RGB_R (0)
#define DAT_FILE_COLOUR_DARKRED_RGB_R (112)
#define DAT_FILE_COLOUR_AQUA_RGB_R (0)
#define TAL_FILE_COLOUR_DEFAULT_RGB_R (204)

#define DAT_FILE_COLOUR_BLACK_RGB_G (0)
#define DAT_FILE_COLOUR_BLUE_RGB_G (0)
#define DAT_FILE_COLOUR_GREEN_RGB_G (192)
#define DAT_FILE_COLOUR_CYAN_RGB_G (240)
#define DAT_FILE_COLOUR_RED_RGB_G (0)
#define DAT_FILE_COLOUR_MAGENTA_RGB_G (0)
#define DAT_FILE_COLOUR_BROWN_RGB_G (96)
#define DAT_FILE_COLOUR_LIGHTGREY_RGB_G (192)
#define DAT_FILE_COLOUR_DARKGREY_RGB_G (96)
#define DAT_FILE_COLOUR_YELLOW_RGB_G (255)
#define DAT_FILE_COLOUR_WHITE_RGB_G (255)
#define DAT_FILE_COLOUR_ORANGE_RGB_G (96)
#define DAT_FILE_COLOUR_PURPLE_RGB_G (0)
#define DAT_FILE_COLOUR_DARKBLUE_RGB_G (0)
#define DAT_FILE_COLOUR_DARKGREEN_RGB_G (112)
#define DAT_FILE_COLOUR_DARKRED_RGB_G (0)
#define DAT_FILE_COLOUR_AQUA_RGB_G (112)
#define TAL_FILE_COLOUR_DEFAULT_RGB_G (204)

#define DAT_FILE_COLOUR_BLACK_RGB_B (0)
#define DAT_FILE_COLOUR_BLUE_RGB_B (240)
#define DAT_FILE_COLOUR_GREEN_RGB_B (0)
#define DAT_FILE_COLOUR_CYAN_RGB_B (240)
#define DAT_FILE_COLOUR_RED_RGB_B (0)
#define DAT_FILE_COLOUR_MAGENTA_RGB_B (255)
#define DAT_FILE_COLOUR_BROWN_RGB_B (0)
#define DAT_FILE_COLOUR_LIGHTGREY_RGB_B (192)
#define DAT_FILE_COLOUR_DARKGREY_RGB_B (96)
#define DAT_FILE_COLOUR_YELLOW_RGB_B (0)
#define DAT_FILE_COLOUR_WHITE_RGB_B (255)
#define DAT_FILE_COLOUR_ORANGE_RGB_B (0)
#define DAT_FILE_COLOUR_PURPLE_RGB_B (255)
#define DAT_FILE_COLOUR_DARKBLUE_RGB_B (112)
#define DAT_FILE_COLOUR_DARKGREEN_RGB_B (0)
#define DAT_FILE_COLOUR_DARKRED_RGB_B (0)
#define DAT_FILE_COLOUR_AQUA_RGB_B (112)
#define TAL_FILE_COLOUR_DEFAULT_RGB_B (204)

#define DAT_FILE_COLOUR_UNDEFINED (0)
#define DAT_FILE_COLOUR_BLACK (0)
#define DAT_FILE_COLOUR_BLUE (1)
#define DAT_FILE_COLOUR_GREEN (2)
#define DAT_FILE_COLOUR_CYAN (11)
#define DAT_FILE_COLOUR_RED (4)
#define DAT_FILE_COLOUR_MAGENTA (5)
#define DAT_FILE_COLOUR_BROWN (6)
#define DAT_FILE_COLOUR_LIGHTGREY (7)
#define DAT_FILE_COLOUR_DARKGREY (8)
#define DAT_FILE_COLOUR_YELLOW (14)
#define DAT_FILE_COLOUR_WHITE (15)
#define DAT_FILE_COLOUR_ORANGE (25)
#define DAT_FILE_COLOUR_PURPLE (22)
#define DAT_FILE_COLOUR_DARKBLUE (272)
#define DAT_FILE_COLOUR_DARKGREEN (288)
#define DAT_FILE_COLOUR_DARKRED (320)
#define DAT_FILE_COLOUR_AQUA (3)
#define DAT_FILE_COLOUR_TRANPARENT_MOD (32)
#define DAT_FILE_DEFAULT_COLOUR (16)
#define DAT_FILE_DEFAULT_COLOUR_EDGELINE (24)

#define DAT_FILE_FIRST_RGB_COLOUR (0xFF)	//255 - NB RGB colours are expressed as FFredGreenBlue; eg Red = FFFF0000 [NB no transparency is supported - as 8 bit padding + 24 bit colour = 32bit]

#define DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX (128)
#define DAT_FILE_REF_NUM_OF_MATRIX_VALUES (9)
#define DAT_FILE_REF_NUM_OF_COLOUR_VALUES (1)
#define DAT_FILE_REF_NUM_OF_TYPE_VALUES (1)
#define DAT_FILE_REF_NUM_OF_POSITION_VALUES (3)
#define DAT_FILE_REF_NUM_OF_SUBMODEL_VALUES (1)
#define DAT_FILE_REF_NUM_DATA_VALUES (DAT_FILE_REF_NUM_OF_TYPE_VALUES + DAT_FILE_REF_NUM_OF_COLOUR_VALUES + DAT_FILE_REF_NUM_OF_POSITION_VALUES + DAT_FILE_REF_NUM_OF_MATRIX_VALUES + DAT_FILE_REF_NUM_OF_SUBMODEL_VALUES)
#define DAT_FILE_DATA_VALUE_MAX_LENGTH (32)	//OLD;15 			//assumes matrix / position data - fraction values have a max size of 10 characters
#define DAT_FILE_DATA_VALUE_RECOMMENDED_LENGTH (6) 				//[negative sign] + [1 value before DP] + [DP] + [3 values after DP]

#define DAT_FILE_REF_MAX_SIZE (DAT_FILE_REF_NUM_OF_TYPE_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH + DAT_FILE_REF_NUM_OF_COLOUR_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH + DAT_FILE_DATA_VALUE_MAX_LENGTH* DAT_FILE_REF_NUM_OF_POSITION_VALUES + DAT_FILE_DATA_VALUE_MAX_LENGTH* DAT_FILE_REF_NUM_OF_MATRIX_VALUES + DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX + DAT_FILE_REF_NUM_DATA_VALUES)
					/*formatted;
					#define DAT_FILE_REF_MAX_SIZE
						(DAT_FILE_REF_NUM_OF_TYPE_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH +
						DAT_FILE_REF_NUM_OF_COLOUR_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH +
						DAT_FILE_DATA_VALUE_MAX_LENGTH* DAT_FILE_REF_NUM_OF_POSITION_VALUES +
						DAT_FILE_DATA_VALUE_MAX_LENGTH* DAT_FILE_REF_NUM_OF_MATRIX_VALUES +
						DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX +
						DAT_FILE_REF_NUM_DATA_VALUES)
					*/
									/*NB there is 2 (integer) + 3 (fractional) + 9 (fractional) + 1 + 15 (spaces) = a total of 15 values are present*/
#define DAT_FILE_REF_MATRIX_MAX_LENGTH  (DAT_FILE_DATA_VALUE_MAX_LENGTH*DAT_FILE_REF_NUM_OF_MATRIX_VALUES + DAT_FILE_REF_NUM_OF_MATRIX_VALUES)
#define DAT_FILE_REF_POS_COORD_MAX_LENGTH (DAT_FILE_DATA_VALUE_MAX_LENGTH*DAT_FILE_REF_NUM_OF_POSITION_VALUES + DAT_FILE_REF_NUM_OF_POSITION_VALUES)
#ifdef USE_ANN
	#define DAT_FILE_MAX_NUM_OF_REFERENCES (1000000U)	//1 million references
#else
	#define DAT_FILE_MAX_NUM_OF_REFERENCES (100000U)	//100 thousand references
#endif

	/*new parameters added to parseFile() 18-mar-07*/
#define REFERENCE_TYPE_COMMENT (0)
#define REFERENCE_TYPE_SUBMODEL (1)
#define REFERENCE_TYPE_LINE (2)
#define REFERENCE_TYPE_TRI (3)
#define REFERENCE_TYPE_QUAD (4)
#define REFERENCE_TYPE_OPTIONALLINE (5)
#define REFERENCE_TYPE_UNDEFINED (9)
#define PRIM_REF_MAX_NUM_OF_POSITION_VALUES (12)
#define PRIMATIVE_REF_MAX_SIZE (DAT_FILE_REF_NUM_OF_TYPE_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH + DAT_FILE_REF_NUM_OF_COLOUR_VALUES* DAT_FILE_DATA_VALUE_MAX_LENGTH + DAT_FILE_DATA_VALUE_MAX_LENGTH* PRIM_REF_MAX_NUM_OF_POSITION_VALUES)



	/*a matrix is defined by 3 vectors side by side*/

class LDreference{
private:
	/*There are currently no private attributes of this class*/
public:

	LDreference(void); // constructor declaration
	LDreference(string referenceName, int referenceColour, bool createNewSubmodel); // constructor declaration
	LDreference(string referenceName, int referenceColour, bool createNewSubmodel, bool topLevel);
	LDreference(bool createNewSubmodel);
	~LDreference();	//  and destructor.

	void initialiseLDreference(string referenceName, int referenceColour, bool createNewSubmodel);

	vec relativePosition;
	vec absolutePosition;
	mat deformationMatrix;
	mat absoluteDeformationMatrix;
	unsigned int colour;
	string name;
	LDreference* next;

		/*new parameters added to parseFile() 18-mar-07*/
	vec vertex1relativePosition;
	vec vertex2relativePosition;
	vec vertex3relativePosition;
	vec vertex4relativePosition;
	vec vertex1absolutePosition;
	vec vertex2absolutePosition;
	vec vertex3absolutePosition;
	vec vertex4absolutePosition;
	#ifdef USE_UNREAL
	string relativeColourString;
	bool isTopLevel;
	vec intermediatePosition;
	mat intermediateDeformationMatrix;
	vec vertex1intermediatePosition;
	vec vertex2intermediatePosition;
	vec vertex3intermediatePosition;
	vec vertex4intermediatePosition;
	//#ifdef LDRAW_VR_IO_OPTIMISATION
	bool partAdditional;	//NOT CURRENTLY USED (could be used for optimisation purposes)
	bool partModified;	//NOT CURRENTLY USED (could be used for optimisation purposes)
	bool partDeleted;		//NOT CURRENTLY USED (could be used for optimisation purposes)
	//#endif
	#endif

	vec vertex1absolutePositionBackup;
	vec vertex2absolutePositionBackup;
	vec vertex3absolutePositionBackup;
	vec vertex4absolutePositionBackup;
	bool referenceEnabledMethod2DOD;

	int type;

#ifdef USE_LD_ABSOLUTE_COLOUR
	unsigned int absoluteColour;
#endif

	/*Additional values used with recursive parser*/
	bool isSubModelReference;
	#ifdef USE_LRRC
	ModelDetails* subModelDetails;
	#endif
	LDreference* firstReferenceWithinSubModel;
};

class LDreferenceClassClass
{
	private: void copyReferencePosition(LDreference* r, LDreference* referenceToCopy);

	private: int obtainReferencePlayerID(const LDreference* referenceInThisPhaseStartSceneFile);

	public: void convertLdrawColourToDatFileRGB(const int dataFileColour, colour* col);
};


#endif


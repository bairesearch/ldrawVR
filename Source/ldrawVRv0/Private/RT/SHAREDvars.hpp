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
 * File Name: SHAREDvars.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_SHARED_VARS
#define HEADER_SHARED_VARS

#include "SHAREDglobalDefs.hpp"

#define MAP_VALUE_OUT_OF_RANGE (8888888.0)


#define TRAIN (1)
#define TEST (0)
#define TRAIN_AND_TEST (2)

#define RT_RAYTRACE_NO_HIT_DEPTH_T (10000000.0)
#define RT_RAYTRACE_NO_HIT_POINT_X (0.0)
#define RT_RAYTRACE_NO_HIT_POINT_Y (0.0)
#define RT_RAYTRACE_NO_HIT_POINT_Z (0.0)






#define TRUE (1)
#define FALSE (0)

#define REALLY_FAR_AWAY (99999999999.9)
#define REALLY_LARGE_DOUBLE (1000000.0)
#define REALLY_LARGE_INT (1000000)
#define REALLY_SMALL_DOUBLE (-1000000.0)

#define OR_METHOD2DOD_DIMENSIONS (2)
#define OR_METHOD3DOD_DIMENSIONS (3)

/* common definitions*/
typedef struct { unsigned char r, g, b; } colour;
typedef struct { double x, y, z; } vec;
typedef struct { vec a; vec b; vec c; } mat;

#ifdef COMPILE_OLC
	#define DOUBLE_MIN_PRECISION 0.005
#else
	#ifdef USE_UNREAL
		#define DOUBLE_MIN_PRECISION 0.001	//temporarily relaxed matrix comparison value for LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
	#else
		#define DOUBLE_MIN_PRECISION 0.00001
	#endif
#endif

class SHAREDvarsClass
{
	public: long getTimeAsLong();

	public: bool compareDoubles(const double a, const double b);
	public: bool compareDoublesArbitraryError(const double a, const double b, const double error);

	public: int maxInt(int a, const int b);	//should be turned into a macro instead
	public: int minInt(int a, const int b);	//should be turned into a macro instead

	public: double minDouble(double a, const double b);	//should be turned into a macro instead
	public: double maxDouble(double a, const double b);	//should be turned into a macro instead

	public: double absDouble(const double val);

	public: bool argumentExists(const int argc, const char** argv, const string keystr);
	public: float getFloatArgument(const int argc, const char** argv, const string keystr);
	public: string getStringArgument(const int argc, const char** argv, const string keystr);
	public: void getStringArrayArgument(const int argc, const char** argv, const string keystr, vector<string>* inputFileNamesVector);
	public: void changeDirectory(const string newDirectory);

	public: string getCurrentDirectory();
	public: void setCurrentDirectory(string folder);
		public: void setCurrentDirectory(string* folder);
	public: void createDirectory(string* folder);
	public: bool directoryExists(string* folder);

	public: void copyColours(colour* colToModify, colour* colToCopy);

	public: bool isWhiteSpace(const char c);
	public: string convertStringToLowerCase(const string* arbitraryCaseString);

	public: string convertFloatToString(const float number, const string format);
	public: string convertDoubleToString(const double number, const string format);
	public: string convertIntToString(const int number);
	public: string convertUnsignedIntToString(const int number);
	public: string convertBoolToString(const bool number);
	public: string convertLongToString(const long number);

	public: int convertStringToInt(const string number);
	public: bool convertStringToBool(string number);
	public: long convertStringToLong(const string number);
	public: float convertStringToFloat(const string number);
	public: double convertStringToDouble(const string number);


	public: bool textInTextArray(const string text, const string* textArray, const int arraySize);
	public: bool textInTextArray(const string text, const string* textArray, const int arraySize, int* arrayIndexOfResultFound);
	public: bool charInCharArray(const char c, const char* charArray, const int arraySize);
	public: bool intInIntArray(const int iTest, const int* intArray, const int arraySize);
	public: string replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString);
	public: string replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString, bool* foundAtLeastOneInstance);

	public: void writeByteArrayToFile(const string fileName, char* fileByteArray, int fileByteArraySize);
	public: void writeStringToFileObject(string s, ofstream* writeFileObject);
	public: void writeStringToFile(const string fileName, string* s);
	public: void appendStringToFile(const string fileName, string* s);
	public: void prependStringToFile(const string fileName, const string* s);

	public: string getFileContents(const string inputFileName);
		public: string getFileContents(const string inputFileName, int* numberLines);
	public: bool fileExists(const string inputFileName);

	public: void sprintfSafeDouble(char* stringCharStar, const char* type, double number);
	public: void sprintfSafeInt(char* stringCharStar, const char* type, int number);
	public: void sprintfSafeLong(char* stringCharStar, const char* type, long number);
};


#endif

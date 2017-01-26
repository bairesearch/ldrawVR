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
 * File Name: SHAREDglobalDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 * Description: Generic Construct Functions: shared global definitions (configure to compile different BAI projects)
 *
 *******************************************************************************/


#ifndef HEADER_SHARED_GLOBAL_DEFS
#define HEADER_SHARED_GLOBAL_DEFS


class SHAREDglobalDefsClass
{
};

	/**********
	SOURCE PROJECT VARIABLES;
	***********/

	//current/active projects;
#define COMPILE_UNREAL_PROJECT
//#define COMPILE_CF
//#define COMPILE_NLC
//#define COMPILE_GIA		//makefile.GIA -> GIA.exe
//#define COMPILE_GIA_WITH_CE	//makefile.GIAwithCE -> GIAwithCE.exe
//#define COMPILE_OR		//makefile.ATOR -> ATOR.exe
//#define COMPILE_CS		//makefile.CS -> CS.exe
//#define COMPILE_RT		//makefile.RT -> RT.exe
//#define COMPILE_ANN_WITH_RT	//makefile.ANNwithRT -> ANNwithRT.exe
//#define COMPILE_ANN		//makefile.ANN -> ANN.exe
//#define COMPILE_LRRC		//compileLRRC.bat -> LRRC.exe
//#define COMPILE_OLC		//compileOLC.bat -> OLC.exe

	//inactive tests;
//#define COMPILE_TH_OR_IMAGE_CATEGORISTION_NN	//ORTHimageCategorisationNN.exe
//#define COMPILE_LRRC_WITH_ANN		[compilation broken; requires updating]
//#define COMPILE_TH_ANN_WITH_LRRC	[compilation broken; requires updating]

	//old tests
//#define COMPILE_TH_OR_METHOD_2DOD
//#define COMPILE_TH_OR_METHOD_3DOD
//#define COMPILE_TH_RT
//#define COMPILE_TH_OR_METHOD_2DOD_WITH_ANN
//#define COMPILE_TH_OR_METHOD_3DOD_WITH_ANN



	/**********
	OS DEPENDENT VARIABLES;
	***********/

//#define LINUX
#ifndef LINUX
	//else assume windows (affects default parts directory only - this is ISO C++ so it should compile on any platform)
	//#include <windows.h>	//incompatible with LDjpeg.h (must only include windows.h when need to)
#endif

#define SQL_MYSQL_VERSION_5_7_PLUS

	/**********
	COMPILATION TYPE DEPENDENT VARIABLES;
	***********/

#ifdef COMPILE_UNREAL_PROJECT
	#define USE_UNREAL
	#define USE_RT
	#ifdef LINUX
		#define DEFAULT_PARTS_DIRECTORY_FULL_PATH "/usr/share/local/LDRAW/PARTS/"
		#define DEFAULT_PARTS_PRIMITIVES_DIRECTORY_FULL_PATH "/usr/share/local/LDRAW/P/"
	#else
		#define DEFAULT_PARTS_DIRECTORY_FULL_PATH "D:/Program Files (x86)/LDraw/PARTS/parts/"
		#define DEFAULT_PARTS_PRIMITIVES_DIRECTORY_FULL_PATH "D:/Program Files (x86)/LDraw/PARTS/p/"
	#endif
#endif

#ifdef COMPILE_CF
	#define USE_CF
#endif
#ifdef COMPILE_NLC
	#define USE_NLC
	#define USE_GIA
#endif
#ifdef COMPILE_GIA_WITH_CE
	#define USE_GIA
	#define USE_CE
#endif
#ifdef COMPILE_GIA
	#define USE_GIA
#endif
#ifdef USE_GIA
	#define USE_RT
#endif



#ifdef COMPILE_CS
	#define USE_CS
#endif
#ifdef USE_CS
	#define USE_RT
#endif



#ifdef COMPILE_OR
	#define USE_OR
#endif
#ifdef COMPILE_TH_OR_IMAGE_CATEGORISTION_NN
	#define USE_OR
	#define USE_ANN
#endif
#ifdef COMPILE_TH_OR_METHOD_2DOD
	#define USE_OR
#endif
#ifdef COMPILE_TH_OR_METHOD_3DOD
	#define USE_OR
	#define OR_METHOD3DOD_TEST
#endif
#ifdef COMPILE_TH_OR_METHOD_2DOD_WITH_ANN
	#define USE_ANN
	#define USE_OR
#endif
#ifdef COMPILE_TH_OR_METHOD_3DOD_WITH_ANN
	#define USE_ANN
	#define USE_OR
#endif
#ifdef USE_OR
	#define USE_RT
#endif



//OLD;
#ifdef OR_USE_OR_NEURAL_NETWORK_COMPARITOR
	#define OR_TEST_ORI_AND_POS_NOISE_DURING_TRANING_AND_TESTING
	#define OR_MANY_NUMBER_OF_OUTPUT_FILES
#endif



#ifdef COMPILE_ANN
	#define USE_ANN
#endif
#ifdef COMPILE_ANN_WITH_RT
	#define USE_ANN
	#define USE_RT
#endif



#ifdef COMPILE_RT
	#define USE_RT
#endif
#ifdef COMPILE_TH_RT
	#define USE_RT
#endif



#ifdef COMPILE_LRRC
	#define USE_LRRC
#endif
#ifdef COMPILE_LRRC_WITH_ANN
	#define USE_LRRC
	#define USE_ANN
	#define USE_RT
#endif
#ifdef COMPILE_TH_ANN_WITH_LRRC
	#define USE_LRRC
	#define USE_ANN
	#define TH_USE_ANN_TEST_HARNESS
	#define USE_RT
#endif


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
#ifdef USE_GIA
#include <unordered_map>	//requires C++11
#include <utility> // make_pair
#endif
#include <algorithm>
#include <stack>
#include <map>
#include <limits>
#include <sstream>
#include <iomanip>
using namespace std;



	//ANN, LRRC, + OR;
//Test variables (need optimisation)
#define MAX_ANN_BACK_PROPAGATION_ERROR (1000.0)



	//ASCII variables
#define CHAR_TAB '\t'
#define CHAR_COMMA ','
#define CHAR_NEWLINE '\n'
#define CHAR_OPEN_BRACKET '('
#define CHAR_CLOSE_BRACKET ')'
#define CHAR_OPEN_SQUARE_BRACKET '['
#define CHAR_CLOSE_SQUARE_BRACKET ']'
#define CHAR_DASH '-'
#define CHAR_VERTICAL_BAR '|'
#define CHAR_UNDERSCORE '_'
#define CHAR_SPACE ' '
#define CHAR_END_OF_STRING '\0'
#define CHAR_DEL (127)
#define CHAR_IRRELEVANT (CHAR_DEL)
#define CHAR_FORWARDSLASH '/'
#define CHAR_FULLSTOP '.'
#define CHAR_SEMICOLON ';'
#define CHAR_COLON ':'
#define CHAR_QUESTIONMARK '?'
#define CHAR_STAR '*'
#define CHAR_INVERTED_COMMAS '"'
#define CHAR_APOSTROPHE '\''
#define CHAR_BACKSLASH '\\'
#define CHAR_EQUALS '='
#define CHAR_INVERTEDCOMMAS '"'
#define CHAR_DECIMALPLACE (CHAR_FULLSTOP)

#define STRING_TAB "\t"
#define STRING_NEW_LINE "\n"
#define STRING_SPACE " "
#define STRING_FULLSTOP "."
#define STRING_COMMA ","
#define STRING_INVERTEDCOMMAS "\""
#define STRING_APOSTROPHE "'"
#define STRING_INVERTED_COMMAS "\""

#define TM_STRUCT_YEAR_OFFSET 1900

#define INT_DEFAULT_VALUE (-1)
#define CPP_STRING_FIND_RESULT_FAIL_VALUE string::npos	//-1
#define BOOL_IRRELEVANT (false)
#define INT_IRRELEVANT (0)
#define BOOL_UNKNOWN_LEGACY (false)

#define EXE_FOLDER_PATH_MAX_LENGTH (1000)
extern string exeFolder;
extern string workingFolder;
extern string tempFolder;

#define constEffective

#endif


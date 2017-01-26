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
 * File Name: SHAREDvars.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/

//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "SHAREDvars.hpp"

#ifdef LINUX
#include <sys/time.h>
#else
#include <time.h>
#include <windows.h>
#endif

//for file i/o;
#include <sys/stat.h>
#include <sys/types.h>
#ifdef LINUX
	#include <unistd.h>	//added for Ubuntu 13.1 file i/o
#else
	#include <windows.h>
#endif

using namespace std;


long SHAREDvarsClass::getTimeAsLong()
{
	#ifdef LINUX
	struct timeval tv;
	struct timezone tz;
	struct tm* tm;
	gettimeofday(&tv, &tz);
	tm=localtime(&tv.tv_sec);
	long timeAsLong = long(tm->tm_hour)* 60* 60* 1000000 + long(tm->tm_min)* 60* 1000000 + long(tm->tm_sec)* 1000000 + long(tv.tv_usec);	//time in microseconds
	return timeAsLong;

	#else

	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);

	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime, &fileTime);

	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime; // could use memcpy here!
	uli.HighPart = fileTime.dwHighDateTime;

	ULONGLONG systemTimeIn_ms(uli.QuadPart/10000);

	return systemTimeIn_ms;

	#endif


}

void SHAREDvarsClass::copyColours(colour* colToModify, colour* colToCopy)
{
	colToModify->r = colToCopy->r;
	colToModify->g = colToCopy->g;
	colToModify->b = colToCopy->b;
}


bool SHAREDvarsClass::compareDoublesArbitraryError(const double a, const double b, const double error)
{
	bool result;
	if((a < (b+error)) && (a > (b-error)))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


bool SHAREDvarsClass::compareDoubles(const double a, const double b)
{
	bool result;
	if((a < (b+DOUBLE_MIN_PRECISION)) && (a > (b-DOUBLE_MIN_PRECISION)))
	{
		result = true;
	}
	else
	{
		result = false;
	}

	return result;
}


//generic functions

int SHAREDvarsClass::maxInt(int a, const int b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int SHAREDvarsClass::minInt(int a, const int b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

double SHAREDvarsClass::minDouble(double a, const double b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
double SHAREDvarsClass::maxDouble(double a, const double b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

double SHAREDvarsClass::absDouble(const double val)
{
	if(val < 0.0)
	{
		return (-val);
	}
	else
	{
		return val;
	}
}

bool SHAREDvarsClass::argumentExists(const int argc, const char** argv, const string keystr)
{
	for(int i=1; i<argc; i++)
	{
		if(string(argv[i]) == keystr)
		{
			return true;
		}
	}
	return false;
}

float SHAREDvarsClass::getFloatArgument(const int argc, const char** argv, const string keystr)
{
	float result=0.0;
	bool foundArgument = false;

	for(int i=1; i<argc; i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				result = atof(argv[i+1]);
				foundArgument = true;
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getFloatArgument{}; " << keystr << endl;
		result = -999999999.0F;
		return result;
	}
	else
	{
		return result;
	}
}

string SHAREDvarsClass::getStringArgument(const int argc, const char** argv, const string keystr)
{
	string stringArgument = "";
	bool foundArgument = false;
	for(int i=1;i<argc;i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				stringArgument = string(argv[i+1]);
				foundArgument = true;
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getStringArgument{}; " << keystr << endl;
	}
	return stringArgument;
}

void SHAREDvarsClass::getStringArrayArgument(const int argc, const char** argv, const string keystr, vector<string>* inputFileNamesVector)
{
	bool foundArgument = false;
	for(int i=1;i<argc;i++)
	{
		if(!foundArgument)
		{
			if(string(argv[i]) == keystr)
			{
				int j=1;
				while(((argv[i+j])[0] != CHAR_DASH) && (i+j < argc))
				{
					string stringArgument = string(argv[i+j]);
					inputFileNamesVector->push_back(stringArgument);
					foundArgument = true;
					j++;
				}
			}
		}
	}
	if(!foundArgument)
	{
		cout << "Error: getStringArrayArgument{}; " << keystr << endl;
	}
}

void SHAREDvarsClass::changeDirectory(const string newDirectory)
{
	char* newDirectoryCharStar = const_cast<char*>(newDirectory.c_str());
	#ifdef LINUX
	chdir(newDirectoryCharStar);
	#else
	::SetCurrentDirectoryA(newDirectoryCharStar);
	#endif
}

string SHAREDvarsClass::getCurrentDirectory()
{
	char currentFolderCharStar[EXE_FOLDER_PATH_MAX_LENGTH];
	#ifdef LINUX
	getcwd(currentFolderCharStar, EXE_FOLDER_PATH_MAX_LENGTH);
	#else
	::GetCurrentDirectoryA(EXE_FOLDER_PATH_MAX_LENGTH, currentFolderCharStar);
	#endif
	string currentFolder = string(currentFolderCharStar);
	return currentFolder;
}

void SHAREDvarsClass::setCurrentDirectory(string folder)
{
	this->setCurrentDirectory(&folder);
}

void SHAREDvarsClass::setCurrentDirectory(string* folder)
{
	const char* folderCharStar = folder->c_str();
	#ifdef LINUX
	chdir(folderCharStar);
	#else
	::SetCurrentDirectoryA(folderCharStar);
	#endif
}

void SHAREDvarsClass::createDirectory(string* folder)
{
	const char* folderCharStar = folder->c_str();
	#ifdef LINUX
	mkdir(folderCharStar, 0755);	//NB GIAdatabase.cpp and ORdatabaseFileIO uses 0755, ORdatabaseDecisionTree.cpp use 0770 [CHECKTHIS]
	#else
	::CreateDirectoryA(folderCharStar, NULL);
	#endif
}

bool SHAREDvarsClass::directoryExists(string* folder)
{
	const char* folderCharStar = folder->c_str();
	bool folderExists = false;

	/*
	#ifdef LINUX
	struct stat st;
	if(stat(folderCharStar, &st) == 0)
	{
		folderExists = true;
	}
	#else
	DWORD ftyp = GetFileAttributes(folderCharStar);
	if(ftyp != INVALID_FILE_ATTRIBUTES)
	{
		if(ftyp & FILE_ATTRIBUTE_DIRECTORY)
		{
			folderExists = true;
		}
	}
	#endif
	*/
	struct stat info;
	if(stat(folderCharStar, &info) != 0)
	{
		//printf( "cannot access %s\n", pathname );
	}
	else if(info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows
	{
		folderExists = true;
		//printf( "%s is a directory\n", pathname );
	}
	else
	{
		//printf( "%s is no directory\n", pathname )
	}
	return folderExists;
}

bool SHAREDvarsClass::isWhiteSpace(const char c)
{
	bool result = false;
	if((c == CHAR_SPACE) || (c == CHAR_TAB))
	{
		result = true;
	}
	return result;
}

string SHAREDvarsClass::convertStringToLowerCase(const string* arbitraryCaseString)
{
	string lowerCaseString = *arbitraryCaseString;
	for(int i=0; i<arbitraryCaseString->length(); i++)
	{
		lowerCaseString[i] = tolower((*arbitraryCaseString)[i]);
	}
	return lowerCaseString;
}

string SHAREDvarsClass::convertFloatToString(const float number, const string format)
{
	char stringCharStar[100];
	sprintfSafeDouble(stringCharStar, format.c_str(), number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertDoubleToString(const double number, const string format)
{
	char stringCharStar[100];
	sprintfSafeDouble(stringCharStar, format.c_str(), number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertIntToString(const int number)
{
	char stringCharStar[100];
	sprintfSafeInt(stringCharStar, "%d", number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertUnsignedIntToString(const int number)
{
	char stringCharStar[100];
	sprintfSafeInt(stringCharStar, "%u", number);
	return string(stringCharStar);
}
string SHAREDvarsClass::convertBoolToString(const bool number)
{
	if(number)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}
string SHAREDvarsClass::convertLongToString(const long number)
{
	//return to_string(number);	//C++11

	char tempString[100];
	sprintfSafeLong(tempString, "%ld", number);
	return string(tempString);
}

int SHAREDvarsClass::convertStringToInt(const string number)
{
	return atoi(number.c_str());
}
bool SHAREDvarsClass::convertStringToBool(string number)
{
	bool boolean = false;

	for(int i=0; i<number.length(); i++)
	{
		number[i] = tolower(number[i]);
	}

	if(number == "true")
	{
		boolean = true;
	}
	else if(number == "false")
	{
		boolean = false;
	}
	else if(number == "1")
	{
		boolean = true;	//boolean = (bool)convertStringToInt(number);
	}
	else if(number == "0")
	{
		boolean = false;
	}
	else
	{
		cout << "convertStringToBool{} error: number != \"true\" or  \"false\" or  \"1\" or  \"0\"" << endl;
		cout << "number = " << number << endl;
	}

	return boolean;
}
long SHAREDvarsClass::convertStringToLong(const string number)
{
	return atol(number.c_str());
}
float SHAREDvarsClass::convertStringToFloat(const string number)
{
	return (float)this->convertStringToDouble(number);
}
double SHAREDvarsClass::convertStringToDouble(const string number)
{
	return atof(number.c_str());
}






bool SHAREDvarsClass::textInTextArray(const string text, const string* textArray, const int arraySize)
{
	int arrayIndexOfResultFound = INT_DEFAULT_VALUE;
	return this->textInTextArray(text, textArray, arraySize, &arrayIndexOfResultFound);
}

bool SHAREDvarsClass::textInTextArray(const string text, const string* textArray, const int arraySize, int* arrayIndexOfResultFound)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(text == textArray[i])
		{
			*arrayIndexOfResultFound = i;
			result = true;
		}
	}
	return result;
}

bool SHAREDvarsClass::charInCharArray(const char c, const char* charArray, const int arraySize)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(c == charArray[i])
		{
			result = true;
		}
	}
	return result;
}

bool SHAREDvarsClass::intInIntArray(const int iTest, const int* intArray, const int arraySize)
{
	bool result = false;
	for(int i=0; i<arraySize; i++)
	{
		if(iTest == intArray[i])
		{
			result = true;
		}
	}
	return result;
}


string SHAREDvarsClass::replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString)
{
	bool foundAtLeastOneInstance = false;
	string text = this->replaceAllOccurancesOfString(textOrig, stringToFind, replacementString, &foundAtLeastOneInstance);
	return text;
}

string SHAREDvarsClass::replaceAllOccurancesOfString(const string* textOrig, string stringToFind, string replacementString, bool* foundAtLeastOneInstance)
{
	*foundAtLeastOneInstance = false;
	string text = *textOrig;
	int pos = 0;
	while((pos = text.find(stringToFind, pos)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		text.replace(pos, stringToFind.length(), replacementString);
		pos = pos + replacementString.length();
		*foundAtLeastOneInstance = true;
	}
	/*
	cout << "\n*textOrig = " <<* textOrig << endl;
	cout << "stringToFind = " << stringToFind << endl;
	cout << "replacementString = " << replacementString << endl;
	cout << "text = " << text << endl;
	*/
	return text;
}

void SHAREDvarsClass::writeByteArrayToFile(const string fileName, char* fileByteArray, int fileByteArraySize)
{
	ofstream parseFileObject(fileName.c_str());

	// Exactly 8 bytes written
	parseFileObject.write(fileByteArray, (sizeof(char)*fileByteArraySize));

	parseFileObject.close();

}

void SHAREDvarsClass::writeStringToFileObject(string s, ofstream* writeFileObject)
{
	//writeFileObject.write(*s);
	for(int i=0; i < s.size(); i++)
	{
		writeFileObject->put(s[i]);
	}
}

void SHAREDvarsClass::writeStringToFile(const string fileName, string* s)
{
	ofstream writeFileObject(fileName.c_str());

	//writeFileObject.write(*s);
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject.put((*s)[i]);
	}

	writeFileObject.close();
}

void SHAREDvarsClass::appendStringToFile(const string fileName, string* s)
{
	ofstream writeFileObject(fileName.c_str(), ofstream::app);

	//writeFileObject.write(*s);
	for(int i=0; i < s->size(); i++)
	{
		writeFileObject.put((*s)[i]);
	}

	writeFileObject.close();
}

//inefficient
void SHAREDvarsClass::prependStringToFile(const string fileName, const string* s)
{
	string fileString = this->getFileContents(fileName);
	fileString = fileString + *s;
	this->writeStringToFile(fileName, &fileString);
}

string SHAREDvarsClass::getFileContents(const string inputFileName)
{
	int numberLines = 0;
	return this->getFileContents(inputFileName, &numberLines);
}

string SHAREDvarsClass::getFileContents(const string inputFileName, int* numberLines)
{
	string fileContents = "";

	bool result = true;
	ifstream parseFileObject(inputFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		// file does not exist in current directory.
		cout << "Error: input file does not exist in current directory: " << inputFileName << endl;
		//exit(0);
		result = false;
	}
	else
	{
		string currentLine = "";
		int currentLineNumber = 0;
		while(getline(parseFileObject, currentLine))
		{
			fileContents = fileContents + currentLine + CHAR_NEWLINE;
			currentLineNumber++;
		}
		*numberLines = currentLineNumber;
	}
	#ifdef CS_DEBUG_GENERATE_OBJECT_ORIENTED_CODE
	//cout << "fileContents = " << fileContents << endl;
	#endif

	return fileContents;
}

bool SHAREDvarsClass::fileExists(const string inputFileName)
{
	bool result = true;
	ifstream parseFileObject(inputFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		result = false;
	}
	else
	{
		parseFileObject.close();
	}

	return result;
}

/*
	bool result = false;
 	FILE* pFile = NULL;
	const char* fileNameCharStar = fileName.c_str();
  	pFile = fopen(fileNameCharStar,"r");
	if(pFile != NULL)
	{
		result = true;
	}
	return result;
*/


void SHAREDvarsClass::sprintfSafeDouble(char* stringCharStar, const char* type, double number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#endif
}

void SHAREDvarsClass::sprintfSafeInt(char* stringCharStar, const char* type, int number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#endif
}

void SHAREDvarsClass::sprintfSafeLong(char* stringCharStar, const char* type, long number)
{
	#ifdef LINUX
	sprintf(stringCharStar, type, number);	//snprintf
	#else
	snprintf(stringCharStar, sizeof(stringCharStar), type, number);
	#endif
}

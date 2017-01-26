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
 * File Name: LDreferenceManipulation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3j3a 26-January-2017
 *
 *******************************************************************************/


//#ifdef COMPILE_UNREAL_PROJECT //comment with COMPILE_UNREAL_PROJECT
#include "ldrawVRv0.h"
//#endif //comment with COMPILE_UNREAL_PROJECT
#include "LDreferenceManipulation.hpp"


/*currently shared routines*/








LDreference* LDreferenceManipulationClass::search1DrefListFindLastReference(LDreference* initialReferenceInList)
{
	LDreference* lastReference;
	LDreference* currentReference = initialReferenceInList;

	while(currentReference->next != NULL)
	{
		currentReference = currentReference->next;
	}
	lastReference = currentReference;

	return lastReference;

}



bool LDreferenceManipulationClass::search1DrefListFindRef(const LDreference* referenceToFind, const LDreference* initialReferenceInList)
{
	bool foundReference = false;

	const LDreference* currentReference = initialReferenceInList;

	while(currentReference->next != NULL)
	{
		if(this->compareReferences(currentReference, referenceToFind, referenceToFind->type))
		{
			foundReference = true;
		}

		currentReference = currentReference->next;
	}

	return foundReference;
}

bool LDreferenceManipulationClass::search1DrefListNameAndColourFindRef(LDreference* referenceToFind, const LDreference* initialReferenceInList)
{
	bool foundReference = false;

	const LDreference* currentReference = initialReferenceInList;

	while(currentReference->next != NULL)
	{
		if(this->compareReferenceNameAndColour(currentReference, (referenceToFind->name).c_str(), referenceToFind->colour))
		{
			foundReference = true;
		}

		currentReference = currentReference->next;
	}

	return foundReference;
}

bool LDreferenceManipulationClass::search1DrefListReplaceRef(const LDreference* referenceToFind, LDreference* referenceToReplaceWith, LDreference* initialReferenceInList)
{
	bool foundReference = false;

	LDreference* currentReference = initialReferenceInList;

	while(currentReference->next != NULL)
	{
		if(this->compareReferences(currentReference, referenceToFind, referenceToFind->type))
		{
			foundReference = true;
			this->copyReferences(currentReference, referenceToReplaceWith, referenceToReplaceWith->type);
		}

		currentReference = currentReference->next;
	}

	return foundReference;
}

bool LDreferenceManipulationClass::search1DrefListNameAndColourReplaceRef(LDreference* referenceToFind, LDreference* referenceToReplaceWith, LDreference* initialReferenceInList)
{
	bool foundReference = false;

	LDreference* currentReference = initialReferenceInList;

	while(currentReference->next != NULL)
	{
		if(this->compareReferenceNameAndColour(currentReference, (referenceToFind->name).c_str(), referenceToFind->colour))
		{
			foundReference = true;
			this->copyReferences(currentReference, referenceToReplaceWith, referenceToReplaceWith->type);
		}

		currentReference = currentReference->next;
	}

	return foundReference;
}

bool LDreferenceManipulationClass::search1DrefListAddReference(LDreference* initialReferenceInList, LDreference* referenceToAdd)
{
	bool foundReference = true;

	LDreference* currentReference = initialReferenceInList;

	if(currentReference->next == NULL)
	{
		LDreference* newReference = new LDreference();
		this->copyReferences(currentReference, referenceToAdd, referenceToAdd->type);
		currentReference->next = newReference;
	}
	else
	{
		while(currentReference->next != NULL)
		{
			currentReference = currentReference->next;

			if(currentReference->next == NULL)
			{
				LDreference* newReference = new LDreference();
				this->copyReferences(currentReference, referenceToAdd, referenceToAdd->type);
				currentReference->next = newReference;
				currentReference = currentReference->next;
			}
		}
	}

	return foundReference;
}



bool LDreferenceManipulationClass::compareReferenceNameAndColour(const LDreference* reference, const string referenceName, const int referenceColour)
{
	bool result = true;

	if(reference->name != referenceName)
	{
		result = false;
	}
	if(reference->colour != referenceColour)
	{
		result = false;
	}

	return result;
}

/*
bool compareSubmodelReferencesNameAndColour(LDreference* reference1, LDreference* reference2)
{
	bool result = true;

	if(reference1->type != reference2->type)
	{
		result = false;
	}
	if(reference1->colour != reference2->colour)
	{
		result = false;
	}

	return result;
}
*/


bool LDreferenceManipulationClass::compareReferences(const LDreference* reference1, const LDreference* reference2, const int type)
{
	bool result = true;

	if(reference1->type != reference2->type)
	{
		result = false;
	}
	if(reference1->colour != reference2->colour)
	{
		result = false;
	}

	if(type == REFERENCE_TYPE_SUBMODEL)
	{
		if(!SHAREDvector.compareVectors(&(reference1->relativePosition),  &(reference2->relativePosition)))
		{
			result = false;
		}
		if(!SHAREDvector.compareMatricies(&(reference1->deformationMatrix),  &(reference2->deformationMatrix)))
		{
			result = false;
		}
		if(reference1->name != reference2->name)
		{
			result = false;
		}
	}
	else
	{
		if((type == REFERENCE_TYPE_LINE) || (type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			if(!SHAREDvector.compareVectors(&(reference1->vertex1relativePosition),  &(reference2->vertex1relativePosition)))
			{
				result = false;
			}
			if(!SHAREDvector.compareVectors(&(reference1->vertex2relativePosition),  &(reference2->vertex2relativePosition)))
			{
				result = false;
			}
		}
		if((type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			if(!SHAREDvector.compareVectors(&(reference1->vertex3relativePosition),  &(reference2->vertex3relativePosition)))
			{
				result = false;
			}
		}
		if((type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			if(!SHAREDvector.compareVectors(&(reference1->vertex4relativePosition),  &(reference2->vertex4relativePosition)))
			{
				result = false;
			}
		}
	}

	return result;
}

void LDreferenceManipulationClass::copyReferences(LDreference* referenceNew, LDreference* referenceToCopy, const int type)
{
	referenceNew->type = referenceToCopy->type;
	referenceNew->colour = referenceToCopy->colour;


	if(type == REFERENCE_TYPE_SUBMODEL)
	{
		SHAREDvector.copyVectors(&(referenceNew->relativePosition),  &(referenceToCopy->relativePosition));
		SHAREDvector.copyVectors(&(referenceNew->absolutePosition),  &(referenceToCopy->absolutePosition));
		SHAREDvector.copyMatricies(&(referenceNew->deformationMatrix),  &(referenceToCopy->deformationMatrix));
		referenceNew->name = referenceToCopy->name;
	}
	else
	{
		if((type == REFERENCE_TYPE_LINE) || (type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex1relativePosition),  &(referenceToCopy->vertex1relativePosition));
			SHAREDvector.copyVectors(&(referenceNew->vertex2relativePosition),  &(referenceToCopy->vertex2relativePosition));
		}
		if((type == REFERENCE_TYPE_TRI) || (type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex3relativePosition),  &(referenceToCopy->vertex3relativePosition));
		}
		if((type == REFERENCE_TYPE_QUAD) || (type == REFERENCE_TYPE_OPTIONALLINE))
		{
			SHAREDvector.copyVectors(&(referenceNew->vertex4relativePosition),  &(referenceToCopy->vertex4relativePosition));
		}
	}
}





string LDreferenceManipulationClass::convertPositionCoordinatesToString(const vec* spriteSceneCoords)
{
	string positionCoordinatesString = "";

	char dataValueString[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->x);
	positionCoordinatesString = positionCoordinatesString + dataValueString + CHAR_SPACE;

	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->y);
	positionCoordinatesString = positionCoordinatesString + dataValueString + CHAR_SPACE;

	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->z);
	positionCoordinatesString = positionCoordinatesString + dataValueString + CHAR_SPACE;

	return positionCoordinatesString;
}

string LDreferenceManipulationClass::convertPositionCoordinatesToStringWithCommaDelimiterPreceeding(const vec* spriteSceneCoords)
{
	string positionCoordinatesString = "";

	char dataValueString[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	positionCoordinatesString = positionCoordinatesString + CHAR_COMMA;
	positionCoordinatesString = positionCoordinatesString + CHAR_SPACE;
	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->x);
	positionCoordinatesString = positionCoordinatesString + dataValueString;

	positionCoordinatesString = positionCoordinatesString + CHAR_COMMA;
	positionCoordinatesString = positionCoordinatesString + CHAR_SPACE;
	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->y);
	positionCoordinatesString = positionCoordinatesString + dataValueString;

	positionCoordinatesString = positionCoordinatesString + CHAR_COMMA;
	positionCoordinatesString = positionCoordinatesString + CHAR_SPACE;
	SHAREDvars.sprintfSafeDouble(dataValueString, FILE_FLOAT_PRECISION, spriteSceneCoords->z);
	positionCoordinatesString = positionCoordinatesString + dataValueString;

	return positionCoordinatesString;
}
















bool LDreferenceManipulationClass::write2DreferenceListCollapsedTo1DtoFile(const string fileName, LDreference* firstReference)
{
	ofstream writeFileObject(fileName.c_str());

	this->write2DreferencesLayerToFileObject(&writeFileObject, firstReference);

	writeFileObject.close();

	return true;
}


bool LDreferenceManipulationClass::write2DreferencesLayerToFileObject(ofstream* writeFileObject, LDreference* firstReferenceInLayer)
{
	bool result = true;

	LDreference* currentReference = firstReferenceInLayer;
	while(currentReference->next != NULL)
	{
		if(currentReference->isSubModelReference)
		{

			result = this->write2DreferencesLayerToFileObject(writeFileObject, currentReference->firstReferenceWithinSubModel);
		}
		else
		{
			LDreference collapsedReference;
			this->copyReferences(&collapsedReference, currentReference, currentReference->type);

			collapsedReference.relativePosition.x = currentReference->absolutePosition.x; //- currentReference->relativePosition.x;	//collapse 2D reference list to top level 1D reference list
			collapsedReference.relativePosition.y = currentReference->absolutePosition.y; // - currentReference->relativePosition.y;	//collapse 2D reference list to top level 1D reference list
			collapsedReference.relativePosition.z = currentReference->absolutePosition.z; // - currentReference->relativePosition.z;	//collapse 2D reference list to top level 1D reference list

			SHAREDvector.copyMatricies(&(collapsedReference.deformationMatrix), &(currentReference->absoluteDeformationMatrix));	//collapse 2D reference list to top level 1D reference list

			SHAREDvector.copyVectors(&(collapsedReference.vertex1relativePosition), &(currentReference->vertex1absolutePosition));
			SHAREDvector.copyVectors(&(collapsedReference.vertex2relativePosition), &(currentReference->vertex2absolutePosition));
			SHAREDvector.copyVectors(&(collapsedReference.vertex3relativePosition), &(currentReference->vertex3absolutePosition));
			SHAREDvector.copyVectors(&(collapsedReference.vertex4relativePosition), &(currentReference->vertex4absolutePosition));

		#ifdef USE_LD_ABSOLUTE_COLOUR
			collapsedReference.colour = currentReference->absoluteColour;
		#endif

			if(currentReference->referenceEnabledMethod2DOD)
			{
				this->addReferenceToFileObject(writeFileObject, &collapsedReference);
			}

		}

		currentReference = currentReference->next;
	}

	return result;
}


bool LDreferenceManipulationClass::writeReferencesToFile(const string fileName, LDreference* firstReference)
{
	ofstream writeFileObject(fileName.c_str());

	LDreference* currentReference = firstReference;
	while(currentReference->next != NULL)
	{
		this->addReferenceToFileObject(&writeFileObject, currentReference);
		currentReference = currentReference->next;
	}
	// Exactly 8 bytes written

	writeFileObject.close();

	return true;
}


//preconditions; submodelReferenceString must be long enough to store all reference information that is to be written to it
bool LDreferenceManipulationClass::convertReferenceToString(LDreference* currentReference, string* referenceString)
{
	bool result = true;

	*referenceString = "";

	//write reference type
	*referenceString = *referenceString + SHAREDvars.convertIntToString(currentReference->type);
	*referenceString = *referenceString + CHAR_SPACE;

	//write reference colour
	*referenceString = *referenceString + SHAREDvars.convertIntToString(currentReference->colour);
	*referenceString = *referenceString + CHAR_SPACE;

	if(currentReference->type == REFERENCE_TYPE_SUBMODEL)
	{
		//write position coords
		vec submodelPositionCoordinatesVec;
		submodelPositionCoordinatesVec.x = currentReference->relativePosition.x;
		submodelPositionCoordinatesVec.y = currentReference->relativePosition.y;
		submodelPositionCoordinatesVec.z = currentReference->relativePosition.z;
		string positionCoordinatesString = this->convertPositionCoordinatesToString(&submodelPositionCoordinatesVec);
		*referenceString = *referenceString + positionCoordinatesString;

		//write rotation matrix values
		mat submodelRotationMatrix;
		SHAREDvector.copyMatrixTwoIntoMatrixOne(&submodelRotationMatrix, &(currentReference->deformationMatrix));

		string rotationMatrixString = this->convertRotationMatrixToString(&submodelRotationMatrix);
		*referenceString = *referenceString + rotationMatrixString;

		//write submodel name
		*referenceString = *referenceString + currentReference->name;
	}
	else if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_TRI) || (currentReference->type == REFERENCE_TYPE_QUAD) || (currentReference->type == REFERENCE_TYPE_LINE))
	{
		//write primitive vertex coords

		string positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex1relativePosition));
		*referenceString = *referenceString + positionCoordinatesString;

		positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex2relativePosition));
		*referenceString = *referenceString + positionCoordinatesString;

		if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_TRI) || (currentReference->type == REFERENCE_TYPE_QUAD))
		{
			positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex3relativePosition));
			*referenceString = *referenceString + positionCoordinatesString;

			if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_QUAD))
			{
				positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex4relativePosition));
				*referenceString = *referenceString + positionCoordinatesString;
			}
		}
	}
	else if(currentReference->type == REFERENCE_TYPE_COMMENT)
	{
		//write comment name
		*referenceString = *referenceString + currentReference->name;
	}
	else
	{
		cout << "This reference type cannot be converted to string: name=" << currentReference->name << ", type=" << currentReference->type << endl;
	}

	//write new line
	*referenceString = *referenceString + CHAR_NEWLINE;

	return result;
}



//preconditions; submodelReferenceString must be long enough to store all reference information that is to be written to it
bool LDreferenceManipulationClass::addReferenceToFileObject(ofstream* writeFileObject, LDreference* currentReference)
{
	bool result = true;

	if(currentReference->type != REFERENCE_TYPE_COMMENT)
	{
		//write reference type
		string dataValueString = SHAREDvars.convertIntToString(currentReference->type);
		//writeFileObject->write(dataValueString);
		for(int i = 0; i<dataValueString.length(); i++)
		{
			writeFileObject->put(dataValueString[i]);
		}
		writeFileObject->put(CHAR_SPACE);

		//write reference colour
		dataValueString = SHAREDvars.convertUnsignedIntToString(currentReference->colour);
		//writeFileObject->write(dataValueString);
		for(int i = 0; i<dataValueString.length(); i++)
		{
			writeFileObject->put(dataValueString[i]);
		}
		writeFileObject->put(CHAR_SPACE);
	}


	if(currentReference->type == REFERENCE_TYPE_SUBMODEL)
	{
		//write position coords
		vec submodelPositionCoordinatesVec;
		submodelPositionCoordinatesVec.x = currentReference->relativePosition.x;
		submodelPositionCoordinatesVec.y = currentReference->relativePosition.y;
		submodelPositionCoordinatesVec.z = currentReference->relativePosition.z;
		string positionCoordinatesString = this->convertPositionCoordinatesToString(&submodelPositionCoordinatesVec);
		for(int i = 0; i<positionCoordinatesString.length(); i++)
		{
			writeFileObject->put(positionCoordinatesString[i]);
		}

		//write rotation matrix values
		mat submodelRotationMatrix;
		SHAREDvector.copyMatrixTwoIntoMatrixOne(&submodelRotationMatrix, &(currentReference->deformationMatrix));

		string rotationMatrixString = this->convertRotationMatrixToString(&submodelRotationMatrix);
		for(int i = 0; i<rotationMatrixString.length(); i++)
		{
			writeFileObject->put(rotationMatrixString[i]);
		}

		//write submodel name
		for(int i = 0; i<currentReference->name.length(); i++)
		{
			writeFileObject->put((currentReference->name)[i]);
		}
	}
	else if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_TRI) || (currentReference->type == REFERENCE_TYPE_QUAD) || (currentReference->type == REFERENCE_TYPE_LINE))
	{
		//write primitive vertex coords

		string positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex1relativePosition));
		for(int i = 0; i<positionCoordinatesString.length(); i++)
		{
			writeFileObject->put(positionCoordinatesString[i]);
		}

		positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex2relativePosition));
		for(int i = 0; i<positionCoordinatesString.length(); i++)
		{
			writeFileObject->put(positionCoordinatesString[i]);
		}

		if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_TRI) || (currentReference->type == REFERENCE_TYPE_QUAD))
		{
			positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex3relativePosition));
			for(int i = 0; i<positionCoordinatesString.length(); i++)
			{
				writeFileObject->put(positionCoordinatesString[i]);
			}

			if((currentReference->type == REFERENCE_TYPE_OPTIONALLINE) || (currentReference->type == REFERENCE_TYPE_QUAD))
			{
				positionCoordinatesString = this->convertPositionCoordinatesToString(&(currentReference->vertex4relativePosition));
				for(int i = 0; i<positionCoordinatesString.length(); i++)
				{
					writeFileObject->put(positionCoordinatesString[i]);
				}
			}
		}
	}
	else if(currentReference->type == REFERENCE_TYPE_COMMENT)
	{
		//write comment name
		for(int i = 0; i<currentReference->name.length(); i++)
		{
			writeFileObject->put((currentReference->name)[i]);
		}
	}
	else
	{
		cout << "This reference type cannot be converted to string: name=" << currentReference->name << ", type=" << currentReference->type << endl;
	}

	//write new line
	writeFileObject->put(CHAR_NEWLINE);

	return result;
}



bool LDreferenceManipulationClass::openFileAndCopyDataIntoCurrentFileObject(const string fileToOpenName, ofstream* writeFileObject)
{
	bool result = true;
	char c;	//current character being read in
	int currentLine = 1;
	int index = 0;

	ifstream parseFileObject(fileToOpenName.c_str());

	//2. fill in the data array
	if(!parseFileObject.rdbuf()->is_open())
	{
		//file does not exist in current directory.
		cout << "file, " << fileToOpenName << " cannot be opened" << endl;
		result = false;
	}
	else
	{
		while ((parseFileObject).get(c))
		{
			writeFileObject->put(c);
			if(c == CHAR_NEWLINE)
			{
				currentLine++;
			}
			index++;
		}
		parseFileObject.close();
	}

	return result;
}
















bool LDreferenceManipulationClass::readFileIntoString(const string fileName, string* fileContentsString, int* fileNumberOfLines, int* fileByteArraySize)
{
	char* fileNamecharstar = const_cast<char*>(fileName.c_str());

	bool result = true;
	char c;	//current character being read in
	int currentLine = 1;
	int index = 0;

	ifstream parseFileObject(fileNamecharstar);

	//2. fill in the data array
	if(!parseFileObject.rdbuf()->is_open())
	{
		//file does not exist in current directory.
		//cout << "file, " << fileName << " cannot be opened" << endl;
		result = false;
	}
	else
	{
		while ((parseFileObject).get(c))
		{
			*fileContentsString = *fileContentsString + c;
			if(c == '\n')
			{
				currentLine++;
			}
			index++;
		}
		parseFileObject.close();
		*fileNumberOfLines = currentLine;
		*fileByteArraySize = index;
	}

	return result;
}



void LDreferenceManipulationClass::copyFiles(const string newFileName, const string fileToCopyName)
{
	ofstream writeFileObject(newFileName.c_str());

	this->openFileAndCopyDataIntoCurrentFileObject(fileToCopyName.c_str(), &writeFileObject);

	writeFileObject.close();
}





bool LDreferenceManipulationClass::addSpriteReferenceListToSceneFile(const string sceneFileName, const string sceneFileNameWithSprites, LDreference* firstSpriteInReferenceList, const int spriteListByteArrayLines)
{
	bool result = true;

	ofstream writeFileObject(sceneFileNameWithSprites.c_str());

	//add original data
	this->openFileAndCopyDataIntoCurrentFileObject(sceneFileName, &writeFileObject);

	//add sprite header
	LDreference spriteHeaderReference;
	spriteHeaderReference.type = REFERENCE_TYPE_COMMENT;
	spriteHeaderReference.name = SPRITE_HEADER_NAME;
	this->addReferenceToFileObject(&writeFileObject, &spriteHeaderReference);

	//add sprite data
	LDreference* currentReference = firstSpriteInReferenceList;
	while(currentReference->next != NULL)
	{
		this->addReferenceToFileObject(&writeFileObject, currentReference);
		currentReference = currentReference->next;
	}

	//add sprite trailer
	LDreference spriteTrailerReference;
	spriteTrailerReference.type = REFERENCE_TYPE_COMMENT;
	spriteTrailerReference.name = SPRITE_TRAILER_NAME;
	this->addReferenceToFileObject(&writeFileObject, &spriteTrailerReference);

	writeFileObject.close();

	return true;
}































/*sprite routines*/


string LDreferenceManipulationClass::convertRotationMatrixToString(const mat* rotationMatrix)
{
	string rotationMatrixString = "";

	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->a.x, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->b.x, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->c.x, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->a.y, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->b.y, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->c.y, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->a.z, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->b.z, "%0.4f") + CHAR_SPACE;
	rotationMatrixString = rotationMatrixString + SHAREDvars.convertDoubleToString(rotationMatrix->c.z, "%0.4f") + CHAR_SPACE;

	return rotationMatrixString;
}













































bool LDreferenceManipulationClass::joinReferenceLists(LDreference* initialReferenceInMainList, LDreference* initialReferenceInAdditionalList)
{
	LDreference* currentReferenceInMainList = initialReferenceInMainList;
	while(currentReferenceInMainList->next != NULL)
	{
		currentReferenceInMainList = currentReferenceInMainList->next;
	}
	currentReferenceInMainList = initialReferenceInAdditionalList;

	return true;
}


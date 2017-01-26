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
* File Name: ldrawVRimportModel.cpp
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/


#include "ldrawVRv0.h"
#include "ldrawVRimportModel.h"

/*
#include <array>
#include <limits>
*/
#define _USE_MATH_DEFINES	//required for visual C++	//http://stackoverflow.com/questions/1727881/how-to-use-the-pi-constant-in-c
#include <math.h>

ldrawVRimportModel::ldrawVRimportModel()
{
	dynamicPartsLocation = "";
}

ldrawVRimportModel::~ldrawVRimportModel()
{
}

void ldrawVRimportModel::clearLDmodel(LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList)
{
	if(getSceneInitialReference() != NULL)
	{
		delete getSceneInitialReference();
	}
	setSceneInitialReference(new LDreference());

	for(int i=0; i<scenePartActorList.size(); i++)
	{
		 AldrawVRpartActor* actor = scenePartActorList[i];
		 actor->Destroy();
	}
	scenePartActorList.clear();

	initialiseSelectedActor();
}

void ldrawVRimportModel::saveLDmodel(string LDModelFileName, string LDModelFolder, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector)
{
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();
	SHAREDvars.setCurrentDirectory(LDModelFolder);

	reconcileReferenceListForSave(sceneInitialReference, scenePartActorList);

	if(!LDreferenceManipulationClass().writeReferencesToFile(LDModelFileName, sceneInitialReference))
	{
		ldrawVRops.printMessage("ldrawVRimportModel::saveLDmodel{} writeReferencesToFile error: The file: " + LDModelFileName + " could not be written");
	}

	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);
}

void ldrawVRimportModel::reconcileReferenceListForSave(LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList)
{
	#ifdef LDRAW_VR_IO_OPTIMISATION
	//removes references marked for deletion from reference list
	LDreference* currentReference = sceneInitialReference;
	LDreference* previousReference = sceneInitialReference;
	while(currentReference->next != NULL)
	{
		if(currentReference->partDeleted)
		{
			previousReference->next = currentReference->next;
			printMessage("reconcileReferenceListForSave{} removing deleted reference from list: " + currentReference->name);
		}
		else
		{
			previousReference = currentReference;
		}

		//ldrawVRops.printMessage("reconcileReferenceListForSave{} current reference name " + currentReference->name);
		//dont parse down level (assume that submodels have not been changed by user, only references to submodels have been changed)

		currentReference = currentReference->next;
	}
	#endif

	#ifndef LDRAW_VR_MODIFY_LD_TRANSFORMATION_MATRIX_IN_SYNC
	for(int i=0; i<scenePartActorList.size(); i++)
	{
		AldrawVRpartActor* actor = scenePartActorList[i];
		LDreference* partReference = actor->partReference;

		//ldrawVRops.printMessage("!LDRAW_VR_MODIFY_LD_TRANSFORMATION_MATRIX_IN_SYNC reconcileReferenceListForSave{} partReference reference name " + partReference->name);

		FVector translatorUnreal = actor->GetActorLocation();
		FRotator rotatorUnreal = actor->GetActorRotation();

		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_CORRECTION_METHOD3
		string partName = "";
		if(!ldrawVRops.submodelIsPart(partReference->name, &partName))
		{
			rotatorUnreal.Yaw = rotatorUnreal.Yaw - 180.0;
		}
		#endif

		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION
		FVector worldPosition;
		worldPosition.X = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_X;
		worldPosition.Y = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Y;
		worldPosition.Z = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Z;
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION_METHOD2
		translatorUnreal.X = translatorUnreal.X - worldPosition.X;
		translatorUnreal.Y = translatorUnreal.Y - worldPosition.Y;
		translatorUnreal.Z = translatorUnreal.Z - worldPosition.Z;
		#else
		vec translationVectorUnrealTopLevel = convertFVectorToVector(worldPosition);
		vec translationVectorLDtopLevel;
		convertTranslationVectorUnrealToLD(&translationVectorLDtopLevel, translationVectorUnrealTopLevel);
		#endif
		#endif
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ROTATION
		FRotator worldRotation;
		worldRotation.Roll = -LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_X;
		worldRotation.Pitch = -LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Y;
		worldRotation.Yaw = -LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Z;
		mat rotationMatrixLDtopLevel;
		ldrawVRops.convertRotationMatrixUnrealToLD(&rotationMatrixLDtopLevel, &worldRotation);
		#endif
		#endif

		/*
		ldrawVRops.printMessage("reconcileReferenceListForSave{} translatorUnreal.X = " + SHAREDvars.convertDoubleToString(translatorUnreal.X));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} translatorUnreal.Y = " + SHAREDvars.convertDoubleToString(translatorUnreal.Y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} translatorUnreal.Z = " + SHAREDvars.convertDoubleToString(translatorUnreal.Z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} rotatorUnreal.Roll = " + SHAREDvars.convertDoubleToString(rotatorUnreal.Roll));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} rotatorUnreal.Pitch = " + SHAREDvars.convertDoubleToString(rotatorUnreal.Pitch));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} rotatorUnreal.Yaw = " + SHAREDvars.convertDoubleToString(rotatorUnreal.Yaw));
		*/

		ldrawVRops.convertTransformationMatrixCoordinateSystemUnrealToLD(&(partReference->absolutePosition), &(partReference->absoluteDeformationMatrix), &translatorUnreal, &rotatorUnreal);

		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ROTATION
		mat rotationMatrixLDtopLevelTransposed = rotationMatrixLDtopLevel;
		SHAREDvector.transposeMatrix(&rotationMatrixLDtopLevelTransposed);
		SHAREDvector.multiplyMatricies(&(partReference->absoluteDeformationMatrix), &(rotationMatrixLDtopLevelTransposed), &(partReference->absoluteDeformationMatrix));
		partReference->absolutePosition.x = LDparser.calcModXPosBasedUponRotate(&(partReference->absolutePosition), &(rotationMatrixLDtopLevelTransposed));
		partReference->absolutePosition.y = LDparser.calcModYPosBasedUponRotate(&(partReference->absolutePosition), &(rotationMatrixLDtopLevelTransposed));
		partReference->absolutePosition.z = LDparser.calcModZPosBasedUponRotate(&(partReference->absolutePosition), &(rotationMatrixLDtopLevelTransposed));
		#endif
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION
		#ifndef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION_METHOD2
		partReference->absolutePosition.x = partReference->absolutePosition.x - translationVectorUnrealTopLevel.x;
		partReference->absolutePosition.y = partReference->absolutePosition.y - translationVectorUnrealTopLevel.y;
		partReference->absolutePosition.z = partReference->absolutePosition.z - translationVectorUnrealTopLevel.z;
		#endif
		#endif
		#endif

		//conversion of absolute to relative values is required for writeReferencesToFile{}?:
		partReference->relativePosition = partReference->absolutePosition;
		partReference->deformationMatrix = partReference->absoluteDeformationMatrix;
		partReference->colour = partReference->absoluteColour;

		/*
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->reference type = " + SHAREDvars.convertIntToString(partReference->type));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->reference name = " + partReference->name);
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.x = " + ldrawVRops(partReference->relativePosition.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.y = " + ldrawVRops(partReference->relativePosition.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.z = " + ldrawVRops(partReference->relativePosition.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.x = " + ldrawVRops(partReference->deformationMatrix.a.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.x = " + ldrawVRops(partReference->deformationMatrix.b.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.x = " + ldrawVRops(partReference->deformationMatrix.c.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.y = " + ldrawVRops(partReference->deformationMatrix.a.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.y = " + ldrawVRops(partReference->deformationMatrix.b.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.y = " + ldrawVRops(partReference->deformationMatrix.c.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.z = " + ldrawVRops(partReference->deformationMatrix.a.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.z = " + ldrawVRops(partReference->deformationMatrix.b.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.z = " + ldrawVRops(partReference->deformationMatrix.c.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->colour = " + ldrawVRops.convertIntToString(partReference->colour));
		*/
	}
	#endif

	LDreference* currentReference = sceneInitialReference;
	while(currentReference->next != NULL)
	{
		LDreference* partReference = currentReference;

		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->reference type = " + SHAREDvars.convertIntToString(partReference->type));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->reference name = " + partReference->name);
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.x = " + ldrawVRops.convertDoubleToString(partReference->relativePosition.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.y = " + ldrawVRops.convertDoubleToString(partReference->relativePosition.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->relativePosition.z = " + ldrawVRops.convertDoubleToString(partReference->relativePosition.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.x = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.a.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.x = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.b.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.x = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.c.x));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.y = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.a.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.y = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.b.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.y = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.c.y));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.a.z = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.a.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.b.z = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.b.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->deformationMatrix.c.z = " + ldrawVRops.convertDoubleToString(partReference->deformationMatrix.c.z));
		ldrawVRops.printMessage("reconcileReferenceListForSave{} partReference->colour = " + SHAREDvars.convertIntToString(partReference->colour));

		currentReference = currentReference->next;
	}

}


void ldrawVRimportModel::loadLDmodel(UWorld* theWorld, string LDModelFileName, string LDModelFolder, LDreference* topLevelReferenceInSceneFile, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector)
{
	bool result = true;

	char* LDModelFileNamecharstar = const_cast<char*>(LDModelFileName.c_str());
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();
	SHAREDvars.setCurrentDirectory(LDModelFolder);

	bool recurseIntoPartsDir = false;
	if(!LDparserClass().parseFile(LDModelFileNamecharstar, sceneInitialReference, topLevelReferenceInSceneFile, recurseIntoPartsDir))
	{
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDmodel{} parseFile error: The file: " + LDModelFileName + " does not exist in the directory " + LDModelFolder);
		result = false;
	}
	else
	{
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDmodel{} parseFile success: " + LDModelFolder + LDModelFileName);
		int partIndex = 0;
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
		loadLDmodelFirstLayer(theWorld, sceneInitialReference, &partIndex, scenePartActorList, meshScaleVector);
		#else
		loadLDmodelRecurse(theWorld, scenePartActorList, sceneInitialReference, &partIndex, meshScaleVector);
		#endif
	}

	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);
}

void ldrawVRimportModel::insertLDmodel(UWorld* theWorld, string LDModelFileName, string LDModelFolder, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector, bool insertAsSingleActor, int defaultColour)
{
	#ifndef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	insertAsSingleActor = false;
	#endif

	FVector worldPosition;	//TODO: modify this based on current user selection/view
	worldPosition.X = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_X;
	worldPosition.Y = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Y;
	worldPosition.Z = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Z;
	FRotator worldRotation;	//TODO: modify this based on current user selection/view
	worldRotation.Roll = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_X;
	worldRotation.Pitch = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_Y;
	worldRotation.Yaw = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_Z;

	LDreference* topLevelReferenceInModelFile = NULL;
	if(insertAsSingleActor)
	{
		topLevelReferenceInModelFile = sceneInitialReference;
		while(topLevelReferenceInModelFile->next != NULL)
		{
			topLevelReferenceInModelFile = topLevelReferenceInModelFile->next;
		}
	}
	else
	{
		topLevelReferenceInModelFile = new LDreference(LDModelFileName, DAT_FILE_DEFAULT_COLOUR, false, true);
	}

	ldrawVRops.convertTransformationMatrixCoordinateSystemUnrealToLD(&(topLevelReferenceInModelFile->absolutePosition), &(topLevelReferenceInModelFile->absoluteDeformationMatrix), &worldPosition, &worldRotation);
	topLevelReferenceInModelFile->name = LDModelFileName;
	topLevelReferenceInModelFile->absoluteColour = defaultColour;	//DAT_FILE_DEFAULT_COLOUR is not required so that inherited materials will be assigned correctly (the algorithm/getReplacementMaterial supports the detection of inherited materials)
	LDreference* modelInitialReference = NULL;
	if(insertAsSingleActor)
	{
		topLevelReferenceInModelFile->type = REFERENCE_TYPE_SUBMODEL;
		topLevelReferenceInModelFile->isSubModelReference = true;
		topLevelReferenceInModelFile->firstReferenceWithinSubModel = new LDreference();
		topLevelReferenceInModelFile->next = new LDreference();
		#ifdef LDRAW_VR_IO_OPTIMISATION
		topLevelReferenceInModelFile->partAdditional = true;
		#endif
		modelInitialReference = topLevelReferenceInModelFile->firstReferenceWithinSubModel;
	}
	else
	{
		//topLevelReferenceInModelFile will be disgarded
		modelInitialReference = new LDreference();
	}


	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	if(insertAsSingleActor)
	{
		string partName = "";
		bool referenceIsPart = ldrawVRops.submodelIsPart(topLevelReferenceInModelFile->name, &partName);
		/*
		//redundant:
		FVector worldPosition;
		FRotator worldRotation;
		generateUnrealTransformations(currentReference, transformationType, worldPosition, worldRotation, meshScaleVector);
		*/
		bool transformMeshesRatherThanActors = false;
		AldrawVRpartActor* actor = spawnActor(theWorld, partName, topLevelReferenceInModelFile, scenePartActorList, worldPosition, worldRotation, meshScaleVector, transformMeshesRatherThanActors);

		insertLDmodelAsSingleActor(actor, LDModelFileName, LDModelFolder, topLevelReferenceInModelFile, modelInitialReference, meshScaleVector);
	}
	else
	{
		loadLDmodel(theWorld, LDModelFileName, LDModelFolder, topLevelReferenceInModelFile, modelInitialReference, scenePartActorList, meshScaleVector);
	}
	#else
	loadLDmodel(theWorld, LDModelFileName, LDModelFolder, topLevelReferenceInModelFile, modelInitialReference, scenePartActorList, meshScaleVector);
	#endif
}

#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
void ldrawVRimportModel::insertLDmodelAsSingleActor(AldrawVRpartActor* actor, string LDModelFileName, string LDModelFolder, LDreference* topLevelReferenceInModelFile, LDreference* modelInitialReference, FVector& meshScaleVector)
{
	bool result = true;

	char* LDModelFileNamecharstar = const_cast<char*>(LDModelFileName.c_str());
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();
	SHAREDvars.setCurrentDirectory(LDModelFolder);

	bool recurseIntoPartsDir = false;
	if(!LDparser.parseFile(LDModelFileNamecharstar, modelInitialReference, topLevelReferenceInModelFile, recurseIntoPartsDir))
	{
		ldrawVRops.printMessage("ldrawVRimportModel::insertLDmodelAsSingleActor{} parseFile error: The file: " + LDModelFileName + " does not exist in the directory " + LDModelFolder);
		result = false;
	}
	else
	{
		ldrawVRops.printMessage("ldrawVRimportModel::insertLDmodelAsSingleActor{} parseFile success: " + LDModelFolder + LDModelFileName);
		int partIndex = 0;
		loadLDmodelRecurse(actor, modelInitialReference, &partIndex, meshScaleVector);
	}

	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);
}
#endif



#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
void ldrawVRimportModel::loadLDmodelFirstLayer(UWorld* theWorld, LDreference* firstReferenceInLayer, int* partIndex, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector)
{
	LDreference* currentReference = firstReferenceInLayer;
	while(currentReference->next != NULL)
	{
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_UNIQUE_MESH_COMPONENT_NAMES_PER_ACTOR
		*partIndex = 0;
		#endif

		bool transformMeshesRatherThanActors = false;
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_FIRST_LAYER_USE_RELATIVE_RATHER_THAN_ABSOLUTE_TRANSFORMATIONS
		int transformationType = LD_TRANSFORMATIONS_RELATIVE;
		#else
		int transformationType = LD_TRANSFORMATIONS_ABSOLUTE;
		#endif

		string partName = "";
		bool referenceIsPart = ldrawVRops.submodelIsPart(currentReference->name, &partName);

		FVector worldPosition;
		FRotator worldRotation;
		generateUnrealTransformations(currentReference, transformationType, worldPosition, worldRotation, meshScaleVector);
		AldrawVRpartActor* actor = spawnActor(theWorld, partName, currentReference, scenePartActorList, worldPosition, worldRotation, meshScaleVector, transformMeshesRatherThanActors);

		bool allowColourChange = true;
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDmodelFirstLayer - currentReference->name = " + currentReference->name);
		loadLDmodelLayer(actor, currentReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, true);

		(*partIndex) = (*partIndex) + 1;

		currentReference = currentReference->next;
	}
}
#endif

#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
void ldrawVRimportModel::loadLDmodelLayer(AldrawVRpartActor* actor, LDreference* currentReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool firstLayer)
#else
void ldrawVRimportModel::loadLDmodelLayer(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, LDreference* currentReference, int* partIndex, FVector& meshScaleVector)
#endif
{
	if(currentReference->isSubModelReference)
	{
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
		loadLDmodelRecurse(actor, currentReference->firstReferenceWithinSubModel, partIndex, meshScaleVector);
		#else
		loadLDmodelRecurse(theWorld, scenePartActorList, currentReference->firstReferenceWithinSubModel, partIndex, meshScaleVector);
		#endif
	}
	else
	{
		if(currentReference->type == REFERENCE_TYPE_SUBMODEL)
		{
			string partName = "";
			if(ldrawVRops.submodelIsPart(currentReference->name, &partName))
			{
				bool singleMeshPerActor = false;
				if(firstLayer)
				{
					singleMeshPerActor = true;	//this is required for dynamic meshes?
				}

				#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS
				if(findInStaticPartsList(currentReference->name))
				{
				#endif
					#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
					//ldrawVRops.printMessage("ldrawVRimportModel::spawnStaticMeshByPartName - currentReference->name = " + currentReference->name);
					spawnStaticMeshByPartName(actor, partName, currentReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, singleMeshPerActor);
					#else
					spawnActorAndStaticMeshByPartName(theWorld, scenePartActorList, partName, currentReference, partIndex, meshScaleVector);
					#endif
				#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS
				}
				else
				{
					if(findInDynamicPartsList(currentReference->name))
					{
						string LDCustomPartFolderName = string(dynamicPartsLocation);
						#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
						spawnDynamicMeshByPartName(actor, currentReference->name, LDCustomPartFolderName, currentReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, singleMeshPerActor);
						#else
						spawnActorAndDynamicMeshByPartName(theWorld, scenePartActorList, currentReference->name, LDCustomPartFolderName, currentReference, partIndex, meshScaleVector);
						#endif
					}
					else
					{
						ldrawVRops.printMessage("ldrawVRimportModel::loadLDmodelRecurse{} part not available: " + currentReference->name);
					}
				}
				#endif
			}
		}
	}
}

#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
void ldrawVRimportModel::loadLDmodelRecurse(AldrawVRpartActor* actor, LDreference* firstReferenceInLayer, int* partIndex, FVector& meshScaleVector)
#else
void ldrawVRimportModel::loadLDmodelRecurse(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, LDreference* firstReferenceInLayer, int* partIndex, FVector& meshScaleVector)
#endif
{
	LDreference* currentReference = firstReferenceInLayer;
	while(currentReference->next != NULL)
	{
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
		bool allowColourChange = false;
		bool transformMeshesRatherThanActors = true;
		int transformationType = LD_TRANSFORMATIONS_INTERMEDIATE;
		loadLDmodelLayer(actor, currentReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, false);
		#else
		int transformationType = LD_TRANSFORMATIONS_ABSOLUTE;
		loadLDmodelLayer(theWorld, scenePartActorList, currentReference, partIndex, meshScaleVector);
		#endif

		(*partIndex) = (*partIndex) + 1;

		currentReference = currentReference->next;
	}
}

#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
void ldrawVRimportModel::spawnStaticMeshByPartName(AldrawVRpartActor* actor, string partName, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool singleMeshPerActor)
{
	//ldrawVRops.printMessage("spawnStaticMeshByPartName: mesh: " + partName);

	FVector translatorUnreal;
	FRotator rotatorUnreal;
	generateUnrealTransformations(partReference, transformationType, translatorUnreal, rotatorUnreal, meshScaleVector);

	addStaticMeshToActor(actor, partName, partReference, partIndex, transformMeshesRatherThanActors, translatorUnreal, rotatorUnreal, meshScaleVector, allowColourChange, singleMeshPerActor);
}
#endif



AldrawVRpartActor* ldrawVRimportModel::spawnActorAndStaticMeshByPartName(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, string partName, LDreference* partReference, int* partIndex, FVector& meshScaleVector)
{
	bool allowColourChange = true;

	//ldrawVRops.printMessage("spawnActorAndStaticMeshByPartName: actor: " + partName);

	FVector translatorUnreal;
	FRotator rotatorUnreal;
	generateUnrealTransformations(partReference, LD_TRANSFORMATIONS_ABSOLUTE, translatorUnreal, rotatorUnreal, meshScaleVector);

	//https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UWorld/SpawnActor/4/index.html
	FVector worldPosition;
	FRotator worldRotation;
	#ifdef LDRAW_VR_TRANSFORM_MESHES_RATHER_THAN_ACTORS
	bool transformMeshesRatherThanActors = true;
	worldPosition.X = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_X;
	worldPosition.Y = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Y;
	worldPosition.Z = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Z;
	worldRotation.Roll = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_X;
	worldRotation.Pitch = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Y;
	worldRotation.Yaw = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Z;
	#else
	bool transformMeshesRatherThanActors = false;
	worldPosition = translatorUnreal;
	worldRotation = rotatorUnreal;
	#endif

	AldrawVRpartActor* actor = spawnActor(theWorld, partName, partReference, scenePartActorList, worldPosition, worldRotation, meshScaleVector, transformMeshesRatherThanActors);

	bool singleMeshPerActor = true;
	addStaticMeshToActor(actor, partName, partReference, partIndex, transformMeshesRatherThanActors, translatorUnreal, rotatorUnreal, meshScaleVector, allowColourChange, singleMeshPerActor);

	return actor;
}


void ldrawVRimportModel::generateUnrealTransformations(LDreference* partReference, int transformationType, FVector& translatorUnreal, FRotator& rotatorUnreal, FVector& meshScaleVector)
{
	vec* translationVectorLD = NULL;
	mat* rotationMatrixLD = NULL;
	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_FIRST_LAYER_USE_RELATIVE_RATHER_THAN_ABSOLUTE_TRANSFORMATIONS
	if(transformationType == LD_TRANSFORMATIONS_RELATIVE)	//not used
	{
		int order = LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT;

		translationVectorLD = &(partReference->relativePosition);
		rotationMatrixLD = &(partReference->deformationMatrix);

		ldrawVRops.convertTransformationMatrixCoordinateSystemLDtoUnreal(translationVectorLD, rotationMatrixLD, &translatorUnreal, &rotatorUnreal, order);

		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_CORRECTION_METHOD3
		string partName = "";
		if(!ldrawVRops.submodelIsPart(partReference->name, &partName))
		{
			rotatorUnreal.Yaw = rotatorUnreal.Yaw + 180.0;
		}
		#endif
	} else
	#endif
	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	if(transformationType == LD_TRANSFORMATIONS_INTERMEDIATE)
	{
		int order = LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_INTERMEDIATE;

		translationVectorLD = &(partReference->intermediatePosition);
		rotationMatrixLD = &(partReference->intermediateDeformationMatrix);
		SHAREDvector.multiplyVectorByScalarRT(translationVectorLD, 1.0/meshScaleVector.X, translationVectorLD);	//assume meshScaleVector is isotropic	//NO: or 1.0/meshScaleVector.Y, 1.0/meshScaleVector.Z etc - fix this
			//take into account the fact the mesh has been scaled at the top level (therefore the lower level translation coordinates do not need to be scaled): this inverse operation (1.0/meshScaleVector.X) will counteract their scale applied by convertTranslationVectorLDtoUnreal:LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR
		ldrawVRops.convertTransformationMatrixCoordinateSystemLDtoUnreal(translationVectorLD, rotationMatrixLD, &translatorUnreal, &rotatorUnreal, order);
	} else
	#endif
	if(transformationType == LD_TRANSFORMATIONS_ABSOLUTE)
	{
		int order = LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT;

		translationVectorLD = &(partReference->absolutePosition);
		rotationMatrixLD = &(partReference->absoluteDeformationMatrix);

		ldrawVRops.convertTransformationMatrixCoordinateSystemLDtoUnreal(translationVectorLD, rotationMatrixLD, &translatorUnreal, &rotatorUnreal, order);

		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_CORRECTION_METHOD3
		string partName = "";
		if(!ldrawVRops.submodelIsPart(partReference->name, &partName))
		{
			rotatorUnreal.Yaw = rotatorUnreal.Yaw + 180.0;
		}
		#endif
	}

	#ifdef LDRAW_VR_DEBUG_LOAD_CUBE_FOR_TRANSFORMATION_DEBUG
	vec translationVectorLD2;
	mat rotationMatrixLD2;
	ldrawVRops.convertTransformationMatrixCoordinateSystemUnrealToLD(&translationVectorLD2, &rotationMatrixLD2, &translatorUnreal, &rotatorUnreal);
	#endif
}

AldrawVRpartActor* ldrawVRimportModel::spawnActor(UWorld* theWorld, string partName, LDreference* partReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& worldPosition, FRotator& worldRotation, FVector& meshScaleVector, bool transformMeshesRatherThanActors)
{
	//ldrawVRops.printMessage("spawnActor: mesh: " + partName);

	FActorSpawnParameters spawnInfo;
	setActorSpawnParameters(&spawnInfo);
	AldrawVRpartActor* actor = theWorld->SpawnActor<AldrawVRpartActor>(worldPosition, worldRotation, spawnInfo);
	if(!transformMeshesRatherThanActors)
	{
		actor->SetActorScale3D(meshScaleVector);
	}
	actor->partReference = partReference;

	if(!actor)
	{
		ldrawVRops.printMessage("spawnActorByPartName{} error: Actor creation failed");
		return actor;
	}
	scenePartActorList.push_back(actor);

	return actor;
}

void ldrawVRimportModel::addStaticMeshToActor(AldrawVRpartActor* actor, string partName, LDreference* partReference, int* partIndex, bool transformMeshesRatherThanActors, FVector& translatorUnreal, FRotator& rotatorUnreal, FVector& meshScaleVector, bool allowColourChange, bool singleMeshPerActor)
{
	//ldrawVRops.printMessage("addStaticMeshToActor: mesh: " + partName);

	int absoluteColour = 0;
	if(partReference->absoluteColour < 255)
	{
		absoluteColour = partReference->absoluteColour;
	}
	else
	{
		ldrawVRops.printMessage("loadLDmodel{} error: only LD colours accepted for model parts (<255)");
		/*
		unsigned int colourExtracted = partReference->absoluteColour;
		colourExtracted = colourExtracted - (DAT_FILE_FIRST_RGB_COLOUR << 24);
		r = ((unsigned int)(colourExtracted << 8) >> 24);
		g = ((unsigned int)(colourExtracted << 16) >> 24);
		b = ((unsigned int)(colourExtracted << 24) >> 24);
		*/
	}

	TArray<UStaticMeshComponent*> meshComponents;
	actor->GetComponents<UStaticMeshComponent>(meshComponents);
	//ldrawVRops.printMessage("1 addStaticMeshToActor: meshComponents.Num() = " + convertIntToString(meshComponents.Num()));

	//https://answers.unrealengine.com/questions/22750/spawning-a-mesh-wo-blueprint.html
	string meshName = generateMeshName(partName, absoluteColour);
	FString meshPathNameF = generateStaticMeshPathName(meshName, absoluteColour);
	//ldrawVRops.printMessage("pathName = " + convertFStringToString(meshPathNameF));
	UStaticMesh* meshReference = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *meshPathNameF));

	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_UNIQUE_MESH_COMPONENT_NAMES_PER_ACTOR
	string meshComponentName = meshName + SHAREDvars.convertIntToString(*partIndex);
	#else
	string meshComponentName = meshName;
	#endif
	FName meshComponentNameF = ldrawVRops.convertStringToFName(meshComponentName);
	UStaticMeshComponent* meshComponent = NULL;
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
	if(singleMeshPerActor)
	{
		meshComponent = Cast<UStaticMeshComponent>(actor->K2_GetRootComponent());
	}
	else
	{
	#endif
		meshComponent = NewObject<UStaticMeshComponent>(actor, meshComponentNameF);	//UStaticMeshComponent::StaticClass()
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
	}
	#endif
	if(meshComponent)
	{
		#ifdef LDRAW_VR_SIMULATE_PHYSICS
		meshComponent->ComponentTags.Add(FName(LDRAW_VR_SIMULATE_PHYSICS_LDRAW_VR_PART_ACTOR_COMPONENT_TAGNAME));
		#endif

		//ldrawVRops.printMessage("meshComponent created");
		meshComponent->RegisterComponent();
		if(transformMeshesRatherThanActors)
		{
			//https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Math/FTransform/index.html
			#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
			FVector meshScaleVectorNull;
			meshScaleVectorNull.X = 1.0;
			meshScaleVectorNull.Y = 1.0;
			meshScaleVectorNull.Z = 1.0;
			FTransform transformUnreal(rotatorUnreal, translatorUnreal, meshScaleVectorNull);
			#else
			FTransform transformUnreal(rotatorUnreal, translatorUnreal, meshScaleVector);
			#endif
			meshComponent->SetRelativeTransform(transformUnreal);
		}
		/*
		else
		{
			//meshComponent->SetRelativeScale3D(meshScaleVector);	//this alternative method is not necessary
		}
		*/

		#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
		if(!singleMeshPerActor)
		{
		#endif
			meshComponent->AttachTo(actor->GetRootComponent());	//, NAME_None, EAttachLocation::KeepWorldPosition;	//warning C4996 : 'USceneComponent::AttachTo' : This function is deprecated, please use AttachToComponent instead.Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
			//THIS DOESNT WORK: //actor->AttachToComponent(meshComponent, FAttachmentTransformRules::KeepWorldTransform);	//https://forums.unrealengine.com/showthread.php?112644-4-12-Transition-Guide
		#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
		}
		#endif

		setStaticMeshComponentParameters(actor, meshComponent);

		meshComponent->SetStaticMesh(meshReference);

		//Material
		UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(partReference, false, allowColourChange);
		string materialNameInherited = string(LDRAWVR_NUMBER_MATERIAL_NAME_INDEX_PREPEND) + LDRAWVR_MATERIAL_NAME_INDEX_DEFAULT;

		int numberOfElements = meshReference->GetNumSections(0);		//LOD index 0
		#ifdef LDRAW_VR_UNREAL_ASSUME_UNIQUE_MATERIAL_PER_MESH_SECTION
		int elemberIndexOfInheritedMaterial = INT_DEFAULT_VALUE;
		#endif
		for(int x = 0; x < numberOfElements; x++)
		{
			UMaterialInterface* material = meshComponent->GetMaterial(x);
			string currentMaterialName = ldrawVRops.convertFStringToString(material->GetName());
			//ldrawVRops.printMessage("currentMaterialName = " + currentMaterialName);
			if(currentMaterialName.find(materialNameInherited) == 0)
			{
				#ifdef LDRAW_VR_UNREAL_ASSUME_UNIQUE_MATERIAL_PER_MESH_SECTION
				elemberIndexOfInheritedMaterial = x;
				#else
				meshComponent->SetMaterial(x, replacementMaterial);
				#endif
			}
		}
		#ifdef LDRAW_VR_UNREAL_ASSUME_UNIQUE_MATERIAL_PER_MESH_SECTION
		if(elemberIndexOfInheritedMaterial != INT_DEFAULT_VALUE)
		{
			meshComponent->SetMaterial(elemberIndexOfInheritedMaterial, replacementMaterial);
		}
		#endif

		//debug;
		TArray<UStaticMeshComponent*> meshComponents2;
		actor->GetComponents<UStaticMeshComponent>(meshComponents2);
		//ldrawVRops.printMessage("2 addStaticMeshToActor: meshComponents.Num() = " + convertIntToString(meshComponents2.Num()));
	}
}


AldrawVRpartActor* ldrawVRimportModel::spawnActorAndDynamicMeshByPartName(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, string LDCustomPartFileName, string LDCustomPartFolder, LDreference* partReference, int* partIndex, FVector& meshScaleVector)
{
	bool allowColourChange = true;

	int transformationType = LD_TRANSFORMATIONS_ABSOLUTE;

	//1. generate actor
	FVector translatorUnreal;
	FRotator rotatorUnreal;
	generateUnrealTransformations(partReference, transformationType, translatorUnreal, rotatorUnreal, meshScaleVector);

	//https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UWorld/SpawnActor/4/index.html
	FVector worldPosition;
	FRotator worldRotation;
	#ifdef LDRAW_VR_TRANSFORM_MESHES_RATHER_THAN_ACTORS
	bool transformMeshesRatherThanActors = true;
	worldPosition.X = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_X;
	worldPosition.Y = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Y;
	worldPosition.Z = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Z;
	worldRotation.Roll = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_X;
	worldRotation.Pitch = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Y;
	worldRotation.Yaw = LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Z;
	#else
	bool transformMeshesRatherThanActors = false;
	worldPosition = translatorUnreal;
	worldRotation = rotatorUnreal;
	//TODO:
	//FVector meshScaleVector;
	//meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
	//meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
	//meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
	#endif

	string partName = "";
	ldrawVRops.submodelIsPart(partReference->name, &partName);
	AldrawVRpartActor* actor = spawnActor(theWorld, partName, partReference, scenePartActorList, worldPosition, worldRotation, meshScaleVector, transformMeshesRatherThanActors);

	bool singleMeshPerActor = true;
	spawnDynamicMeshByPartName(actor, LDCustomPartFileName, LDCustomPartFolder, partReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, singleMeshPerActor);

	return actor;
}

void ldrawVRimportModel::spawnDynamicMeshByPartName(AldrawVRpartActor* actor, string LDCustomPartFileName, string LDCustomPartFolder, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool singleMeshPerActor)
{
	bool result = true;

	//2. load custom part file
	char* LDCustomPartFileNamecharstar = const_cast<char*>(LDCustomPartFileName.c_str());
	string currentDirectoryTemp = SHAREDvars.getCurrentDirectory();
	SHAREDvars.setCurrentDirectory(LDCustomPartFolder);

	LDreference* customPartInitialReference = new LDreference();
	LDreference* topLevelReferenceInCustomPartFile = new LDreference(LDCustomPartFileName, 1, false, true);
	bool recurseIntoPartsDir = true;
	if(!LDparser.parseFile(LDCustomPartFileNamecharstar, customPartInitialReference, topLevelReferenceInCustomPartFile, recurseIntoPartsDir))
	{
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDcustomPart{} parseFile error: The file: " + LDCustomPartFileName + " does not exist in the directory " + LDCustomPartFolder);
		result = false;
	}
	else
	{
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDcustomPart{} parseFile success: " + LDCustomPartFolder + " " + LDCustomPartFileName);
	}

	SHAREDvars.setCurrentDirectory(currentDirectoryTemp);

	//3. generate dynamic mesh
	bool createCollision = true;
	addDynamicMeshToActor(actor, customPartInitialReference, partReference, partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, createCollision, singleMeshPerActor);

}

void ldrawVRimportModel::addDynamicMeshToActor(AldrawVRpartActor* actor, LDreference* customPartInitialReference, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool createCollision, bool singleMeshPerActor)
{
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT_DISABLE_FOR_PROCEDURAL_GENERATED_MESHES_DUE_TO_UNREAL_LIMITATION
	if(singleMeshPerActor)
	{
		singleMeshPerActor = false;
		/*
		this is required for dynamic meshes because if(meshComponent) will fail otherwise.
			This is failing because this fails/generates NULL: "Cast<UProceduralMeshComponent>(actor->K2_GetRootComponent())".
				This is probably failing to cast because the default root component is a staticMeshComponent rather than a proceduralMeshComponent for a newly created staticMeshActor.
					could possibly try deleting the default/root mesh component upon actor creation, and adding a new proceduralMeshComponent as the root component. however this may not resolve the underlying issue (LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT) because the motion controller pickup/drop code appears to only support a single (static)MeshComponent per actor. Until this underlying issue has been resolved (perhaps in the Unreal VR template code) then there is no point attempting to get singleMeshPerActor=true working here for procedural meshes
		*/
	}
	#endif

	string partName = "";
	if(!ldrawVRops.submodelIsPart(partReference->name, &partName))
	{
		ldrawVRops.printMessage("ldrawVRimportModel::loadLDcustomPart{} error: !submodelIsPart");
	}
	//ldrawVRops.printMessage("addDynamicMeshToActor: mesh: " + partName);

	string meshName = generateMeshName(partName, partReference->absoluteColour);
	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_UNIQUE_MESH_COMPONENT_NAMES_PER_ACTOR
	string meshComponentName = meshName + SHAREDvars.convertIntToString(*partIndex);
	#else
	string meshComponentName = meshName;
	#endif
	FName meshComponentNameF = ldrawVRops.convertStringToFName(meshComponentName);
	UProceduralMeshComponent* meshComponent = NULL;
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
	if(singleMeshPerActor)
	{
		meshComponent = Cast<UProceduralMeshComponent>(actor->K2_GetRootComponent());		//this doesn't appear to work for dynamic meshes: singleMeshPerActor must always be set false when calling addDynamicMeshToActor
	}
	else
	{
	#endif
		meshComponent = NewObject<UProceduralMeshComponent>(actor, meshComponentNameF);
		//CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
	}
	#endif
	if(meshComponent)
	{
		#ifdef LDRAW_VR_SIMULATE_PHYSICS
		meshComponent->ComponentTags.Add(FName(LDRAW_VR_SIMULATE_PHYSICS_LDRAW_VR_PART_ACTOR_COMPONENT_TAGNAME));
		#endif

		//ldrawVRops.printMessage("meshComponent created");
		meshComponent->RegisterComponent();
		if(transformMeshesRatherThanActors)
		{
			//regenerate transforms (see 1. generate actor)
			FVector translatorUnreal;
			FRotator rotatorUnreal;
			generateUnrealTransformations(partReference, transformationType, translatorUnreal, rotatorUnreal, meshScaleVector);

			#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
			FVector meshScaleVectorNull;
			meshScaleVectorNull.X = 1.0;
			meshScaleVectorNull.Y = 1.0;
			meshScaleVectorNull.Z = 1.0;
			FTransform transformUnreal(rotatorUnreal, translatorUnreal, meshScaleVectorNull);
			#else
			FTransform transformUnreal(rotatorUnreal, translatorUnreal, meshScaleVector);
			#endif
			meshComponent->SetRelativeTransform(transformUnreal);
		}
		/*
		else
		{
			//meshComponent->SetRelativeScale3D(meshScaleVector);	//this alternative method is not necessary
		}
		*/

		#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
		if(!singleMeshPerActor)
		{
		#endif
			meshComponent->AttachTo(actor->GetRootComponent());	//, NAME_None, EAttachLocation::KeepWorldPosition;	//warning C4996 : 'USceneComponent::AttachTo' : This function is deprecated, please use AttachToComponent instead.Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
			//THIS DOESNT WORK: //actor->AttachToComponent(meshComponent, FAttachmentTransformRules::KeepWorldTransform);	//https://forums.unrealengine.com/showthread.php?112644-4-12-Transition-Guide
		#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
		}
		#endif

		int meshSectionIndex = 1;
		LDreference* firstPolyInMeshChain = new LDreference();
		LDreference* currentPolyInMeshChain = firstPolyInMeshChain;
		#ifdef LDRAW_VR_DYNAMIC_MESH_TRACK_RELATIVE_COLOURS
		int previousColour = customPartInitialReference->colour;
		#else
		int previousColour = customPartInitialReference->absoluteColour;
		#endif
		generateDynamicMeshRecurse(&firstPolyInMeshChain, &currentPolyInMeshChain, customPartInitialReference, meshComponent, &meshSectionIndex, &previousColour, allowColourChange, createCollision);

		//generate last mesh section
		if(firstPolyInMeshChain->next != NULL)
		{
			generateDynamicMeshSection(&firstPolyInMeshChain, &currentPolyInMeshChain, meshComponent, &meshSectionIndex, allowColourChange, createCollision);
		}
		delete firstPolyInMeshChain;

		setProceduralMeshComponentParameters(actor, meshComponent, createCollision);


		//DEBUG;
		/*
		TArray<UStaticMeshComponent*> meshComponents;
		actor->GetComponents<UStaticMeshComponent>(meshComponents);
		ldrawVRops.printMessage("addDynamicMeshToActor: meshComponents.Num() = " + SHAREDvars.convertIntToString(meshComponents.Num()));
		TArray<UProceduralMeshComponent*> meshComponents2;
		actor->GetComponents<UProceduralMeshComponent>(meshComponents2);
		ldrawVRops.printMessage("addDynamicMeshToActor: procedural meshComponents2.Num() = " + SHAREDvars.convertIntToString(meshComponents2.Num()));
		*/
	}
}

bool ldrawVRimportModel::generateDynamicMeshRecurse(LDreference** firstPolyInMeshChain, LDreference** currentPolyInMeshChain, LDreference* firstReferenceInLayer, UProceduralMeshComponent* meshComponent, int* meshSectionIndex, int* previousColour, bool allowColourChange, bool createCollision)
{
	bool result = true;

	LDreference* currentReference = firstReferenceInLayer;
	while(currentReference->next != NULL)
	{
		#ifdef LDRAW_VR_DYNAMIC_MESH_TRACK_RELATIVE_COLOURS
		if(currentReference->colour != *previousColour)
		#else
		if(currentReference->absoluteColour != *previousColour)
		#endif
		{
			//TODO: verify do not need to check allSubpartsHaveSameAbsoluteColour
			generateDynamicMeshSection(firstPolyInMeshChain, currentPolyInMeshChain, meshComponent, meshSectionIndex, allowColourChange, createCollision);
			#ifdef LDRAW_VR_DYNAMIC_MESH_TRACK_RELATIVE_COLOURS
			*previousColour = currentReference->colour;
			#else
			*previousColour = currentReference->absoluteColour;
			#endif
		}

		if(currentReference->type == REFERENCE_TYPE_SUBMODEL)
		{
			if(currentReference->firstReferenceWithinSubModel != NULL)
			{
				if(!generateDynamicMeshRecurse(firstPolyInMeshChain, currentPolyInMeshChain, currentReference->firstReferenceWithinSubModel, meshComponent, meshSectionIndex, previousColour, allowColourChange, createCollision))
				{
					result = false;
				}
			}
			else
			{
				ldrawVRops.printMessage("ldrawVRimportModel::generateDynamicMeshRecurse{} error: (currentReference->type == REFERENCE_TYPE_SUBMODEL) && (currentReference->firstReferenceWithinSubModel == NULL)");
			}
		}
		else if(currentReference->type == REFERENCE_TYPE_LINE)
		{
			//cout << "generateDynamicMeshRecurse{} notice: (currentReference->type == REFERENCE_TYPE_LINE)" << endl;
			//not currently supported: could be used to generate mesh normals
		}
		else if(currentReference->type == REFERENCE_TYPE_OPTIONALLINE)
		{
			//cout << "generateDynamicMeshRecurse{} notice: (currentReference->type == REFERENCE_TYPE_OPTIONALLINE)" << endl;
			//not currently supported: could be used to generate mesh normals
		}
		else if(currentReference->type == REFERENCE_TYPE_TRI)
		{
			//cout << "generateDynamicMeshRecurse{} notice: (currentReference->type == REFERENCE_TYPE_TRI)" << endl;
			(**currentPolyInMeshChain) = *currentReference;
			(*currentPolyInMeshChain)->next = new LDreference();
			(*currentPolyInMeshChain) = (*currentPolyInMeshChain)->next;
		}
		else if(currentReference->type == REFERENCE_TYPE_QUAD)
		{
			//cout << "generateDynamicMeshRecurse{} notice: (currentReference->type == REFERENCE_TYPE_QUAD)" << endl;
			(**currentPolyInMeshChain) = *currentReference;
			(*currentPolyInMeshChain)->next = new LDreference();
			(*currentPolyInMeshChain) = (*currentPolyInMeshChain)->next;
		}

		/*
		ldrawVRops.printMessage("generateDynamicMeshRecurse{} current reference type " + SHAREDvars.convertIntToString(currentReference->type));
		ldrawVRops.printMessage("generateDynamicMeshRecurse{} current reference name " + currentReference->name);
		ldrawVRops.printMessage("generateDynamicMeshRecurse{} current reference meshSectionIndex " + convertIntToString(*meshSectionIndex));
		ldrawVRops.printMessage("generateDynamicMeshRecurse{} current reference colour " + SHAREDvars.convertIntToString(currentReference->absoluteColour));
		*/

		currentReference = currentReference->next;
	}
	return result;
}

void ldrawVRimportModel::generateDynamicMeshSection(LDreference** firstPolyInMeshChain, LDreference** currentPolyInMeshChain, UProceduralMeshComponent* meshComponent, int* meshSectionIndex, bool allowColourChange, bool createCollision)
{
	//https://wiki.unrealengine.com/Procedural_Mesh_Component_in_C%2B%2B:Getting_Started

	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	//TArray<FVector2D> UV0;
	//TArray<FColor> vertexColors;
	//TArray<FProcMeshTangent> tangents;

	int absoluteColour = (*firstPolyInMeshChain)->absoluteColour;
	colour dataFileColourVec;
	LDreferenceClass.convertLdrawColourToDatFileRGB(absoluteColour, &dataFileColourVec);
	FColor col(dataFileColourVec.r, dataFileColourVec.g, dataFileColourVec.b);

	//ldrawVRops.printMessage("generateDynamicMeshSection{} CreateMeshSection, meshSectionIndex = " + SHAREDvars.convertIntToString(*meshSectionIndex));

	LDreference* currentReferenceMeshChain = *firstPolyInMeshChain;
	while(currentReferenceMeshChain->next != NULL)
	{
		FVector normal;
		if((currentReferenceMeshChain->type == REFERENCE_TYPE_TRI) || (currentReferenceMeshChain->type == REFERENCE_TYPE_QUAD))
		{
			normal = calculateNormal2(&(currentReferenceMeshChain->vertex1absolutePosition), &(currentReferenceMeshChain->vertex2absolutePosition), &(currentReferenceMeshChain->vertex3absolutePosition));

			if(!findVertexID(vertices, currentReferenceMeshChain->vertex1absolutePosition))
			{
				FVector vertex = ldrawVRops.convertVectorToFVector(&(currentReferenceMeshChain->vertex1absolutePosition));
				vertices.Add(vertex);
				normals.Add(normal);
				//vertexColors.Add(col);
				//ldrawVRops.printMessage("1generateMeshSection{} !findVertexID, adding vertex num = " + SHAREDvars.convertIntToString(vertices.Num()));
			}
			if(!findVertexID(vertices, currentReferenceMeshChain->vertex2absolutePosition))
			{
				FVector vertex = ldrawVRops.convertVectorToFVector(&(currentReferenceMeshChain->vertex2absolutePosition));
				vertices.Add(vertex);
				normals.Add(normal);
				//vertexColors.Add(col);
				//ldrawVRops.printMessage("2generateMeshSection{} !findVertexID, adding vertex num = " + SHAREDvars.convertIntToString(vertices.Num()));

			}
			if(!findVertexID(vertices, currentReferenceMeshChain->vertex3absolutePosition))
			{
				FVector vertex = ldrawVRops.convertVectorToFVector(&(currentReferenceMeshChain->vertex3absolutePosition));
				vertices.Add(vertex);
				normals.Add(normal);
				//vertexColors.Add(col);
				//ldrawVRops.printMessage("3generateMeshSection{} !findVertexID, adding vertex num = " + SHAREDvars.convertIntToString(vertices.Num()));
			}
		}
		if(currentReferenceMeshChain->type == REFERENCE_TYPE_QUAD)
		{
			if(!findVertexID(vertices, currentReferenceMeshChain->vertex4absolutePosition))
			{
				FVector vertex = ldrawVRops.convertVectorToFVector(&(currentReferenceMeshChain->vertex4absolutePosition));
				vertices.Add(vertex);
				normals.Add(normal);
				//vertexColors.Add(col);
				//ldrawVRops.printMessage("4generateMeshSection{} !findVertexID, adding vertex num = " + SHAREDvars.convertIntToString(vertices.Num()));
			}
		}
		currentReferenceMeshChain = currentReferenceMeshChain->next;
	}

	currentReferenceMeshChain = *firstPolyInMeshChain;
	while(currentReferenceMeshChain->next != NULL)
	{
		if(currentReferenceMeshChain->type == REFERENCE_TYPE_TRI)
		{
			convertTritoTriangles(triangles, getVertexID(vertices, currentReferenceMeshChain->vertex1absolutePosition), getVertexID(vertices, currentReferenceMeshChain->vertex2absolutePosition), getVertexID(vertices, currentReferenceMeshChain->vertex3absolutePosition));
			//ldrawVRops.printMessage("generateDynamicMeshSection{}, adding triangle");
		}
		else if(currentReferenceMeshChain->type == REFERENCE_TYPE_QUAD)
		{
			convertQuadtoTriangles(triangles, getVertexID(vertices, currentReferenceMeshChain->vertex1absolutePosition), getVertexID(vertices, currentReferenceMeshChain->vertex2absolutePosition), getVertexID(vertices, currentReferenceMeshChain->vertex3absolutePosition), getVertexID(vertices, currentReferenceMeshChain->vertex4absolutePosition));
			//ldrawVRops.printMessage("generateDynamicMeshSection{}, adding quad");
		}
		currentReferenceMeshChain = currentReferenceMeshChain->next;
	}

	//meshComponent->CreateMeshSection(meshSectionIndex, vertices, Triangles, normals, UV0, vertexColors, tangents, createCollision);
	//meshComponent->CreateMeshSection(*meshSectionIndex, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), createCollision);	//vertexColors
	meshComponent->CreateMeshSection(*meshSectionIndex, vertices, triangles, normals, TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), createCollision);	//vertexColors

	//Material

	bool allowColourChangeSubpart = false;
	if(allowColourChange)
	{
		if((*firstPolyInMeshChain)->colour == DAT_FILE_DEFAULT_COLOUR)
		{
			allowColourChangeSubpart = true;
		}
	}
	UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(*firstPolyInMeshChain, false, allowColourChangeSubpart);

	meshComponent->SetMaterial(*meshSectionIndex, replacementMaterial);

	delete (*firstPolyInMeshChain);
	*firstPolyInMeshChain = new LDreference();	//create new mesh chain
	*currentPolyInMeshChain = *firstPolyInMeshChain;

	(*meshSectionIndex) = (*meshSectionIndex) + 1;
}

#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS

void ldrawVRimportModel::loadStaticPartsList(string LDSetFileName, string LDSetFolderName, string LDSetExtensionName)
{
	string LDsetFilePathFull = LDSetFolderName + LDSetFileName;
	int numberOfLinesInList = 0;
	staticPartsList.clear();
	vector<string> internalStaticMeshPartsListRaw;
	if(!getFilesFromFileList(LDsetFilePathFull, &internalStaticMeshPartsListRaw, &numberOfLinesInList))
	{
		ldrawVRops.printMessage("AldrawVRGameControlActor::loadStaticPartsList{}: error: !getFilesFromFileList: LDsetFilePathFull = " + LDsetFilePathFull);
	}
	for(int i=0; i<internalStaticMeshPartsListRaw.size(); i++)
	{
		string currentLine = internalStaticMeshPartsListRaw[i];
		int indexOfDatFileExt = currentLine.find(LDSetExtensionName);
		if(indexOfDatFileExt != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			string partName = currentLine.substr(0, indexOfDatFileExt+LDSetExtensionName.length());
			staticPartsList.push_back(partName);
			//ldrawVRops.printMessage("ldrawVRimportModel::loadStaticPartsList{}: partName = " + partName);
		}
	}
}

void ldrawVRimportModel::loadDynamicPartsList(string LDCustomPartFolderName, string LDCustomPartExtensionName)
{
	dynamicPartsLocation = LDCustomPartFolderName;	//getCustomPartFolderName()
	FString LDCustomPartFolderNameF = ldrawVRops.convertStringToFString(LDCustomPartFolderName);	//getCustomPartFolderName()
	FString LDCustomPartExtensionNameF = ldrawVRops.convertStringToFString(LDCustomPartExtensionName);	//getCustomPartFileNameExtension()
	TArray<FString> dynamicPartsListF = getAllFilesInDirectory2(LDCustomPartFolderNameF, false, "", LDCustomPartExtensionNameF);
	for(int i=0; i<dynamicPartsListF.Num(); i++)
	{
		string partName = ldrawVRops.convertFStringToString(dynamicPartsListF[i]);
		dynamicPartsList.push_back(partName);
		//ldrawVRops.printMessage("ldrawVRimportModel::loadDynamicPartsList{}: partName = " + partName);
	}
}

bool ldrawVRimportModel::findInStaticPartsList(string referenceName)
{
	bool result = false;
	for(int i=0; i<staticPartsList.size(); i++)
	{
		#ifdef LDRAW_VR_UNREAL_USES_CASE_INSENSITIVE_ASSET_NAMES
		if((staticPartsList[i] == referenceName) || (SHAREDvars.convertStringToLowerCase(&staticPartsList[i]) == SHAREDvars.convertStringToLowerCase(&referenceName)))	//staticPartsList is case insensitive because Unreal engine uses case insensitive mesh names
		#else
		if(staticPartsList[i] == referenceName)
		#endif
		{
			result = true;
		}
	}
	return result;
}
bool ldrawVRimportModel::findInDynamicPartsList(string referenceName)
{
	bool result = false;
	for(int i=0; i<dynamicPartsList.size(); i++)
	{
		if(dynamicPartsList[i] == referenceName)
		{
			result = true;
		}
	}
	return result;
}

#endif

/*
void splitQuadIntoTris(LDReference* quad, LDreference** currentPolyInMeshChain)
{
	//https://docs.unrealengine.com/latest/INT/BlueprintAPI/Components/ProceduralMesh/ConvertQuadtoTriangles/index.html
	TArray<int32> triangles;

	convertQuadToTriangles(triangles,

}
*/


AldrawVRpartActor* ldrawVRimportModel::spawnLDgridActorAndDynamicMesh(UWorld* theWorld, FVector& worldPosition, FRotator& worldRotation, FVector& meshScaleVector, vec* LDgridSnapPos)
{
	int transformationType = LD_TRANSFORMATIONS_ABSOLUTE;
	bool transformMeshesRatherThanActors = false;
	LDreference* topLevelReference = new LDreference("grid.dat", LDRAW_VR_LD_GRID_COLOUR, false, true);	//string("grid" LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION)	//provide it a part name to prevent addDynamicMeshToActor from throwing an error
	vector<AldrawVRpartActor*> tempActorList;
	string tempPartName = "";
	AldrawVRpartActor* actor = spawnActor(theWorld, tempPartName, topLevelReference, tempActorList, worldPosition, worldRotation, meshScaleVector, transformMeshesRatherThanActors);

	LDreference* gridInitialReference = new LDreference(tempPartName, LDRAW_VR_LD_GRID_COLOUR, false, false);
	vec numberGridColumns;
	vec LDgridSize;
	for(int i=AXIS_FIRST; i<=AXIS_NUMBER_OF_TYPES; i++)
	{
		double LDgridSizeAxis = ldrawVRops.convertTranslationValueUnrealToLD(LDRAW_VR_LD_GRID_SIZE)/meshScaleVector.X;		//= convertTranslationValueUnrealToLD(LDRAW_VR_LD_GRID_SIZE)/meshScaleVector.X;	//assume meshScaleVector is isotropic
		double numberGridColumnsAxis = LDgridSizeAxis/getVecValue(LDgridSnapPos, i);
		setVecValue(&LDgridSize, i, LDgridSizeAxis);
		setVecValue(&numberGridColumns, i, numberGridColumnsAxis);
	}

	LDreference* currentGridReference = gridInitialReference;
	spawnLDgridDynamicMeshAxis(&currentGridReference, AXIS_X, AXIS_Y, AXIS_Z, &numberGridColumns, LDgridSnapPos, &LDgridSize);
	spawnLDgridDynamicMeshAxis(&currentGridReference, AXIS_Y, AXIS_X, AXIS_Z, &numberGridColumns, LDgridSnapPos, &LDgridSize);
	spawnLDgridDynamicMeshAxis(&currentGridReference, AXIS_Z, AXIS_X, AXIS_Y, &numberGridColumns, LDgridSnapPos, &LDgridSize);	//this does nothing due to bug

	bool allowColourChange = false;
	int partIndex = 0;
	bool createCollision = false;
	bool singleMeshPerActor = true;
	addDynamicMeshToActor(actor, gridInitialReference, topLevelReference, &partIndex, meshScaleVector, transformMeshesRatherThanActors, transformationType, allowColourChange, createCollision, singleMeshPerActor);
	delete gridInitialReference;

	return actor;
}

void ldrawVRimportModel::spawnLDgridDynamicMeshAxis(LDreference** currentGridReference, int axis1Primary, int axis2, int axis3, vec* numberGridColumns, vec* LDgridSnapPos, vec* LDgridSize)
{
	double axis2Position = -getVecValue(LDgridSize, axis2)/2.0;
	for(int i=0; i<getVecValue(numberGridColumns, axis2); i++)
	{
		double axis3Position = -getVecValue(LDgridSize, axis3)/2.0;
		for(int j=0; j<getVecValue(numberGridColumns, axis3); j++)	//this does nothing due to bug
		{
			//ldrawVRops.printMessage("ldrawVRimportModel::spawnLDgridDynamicMeshAxis{}: i = " + convertIntToString(i) + ", j = " + convertIntToString(j));

			(*currentGridReference)->name = "irrelevant";
			(*currentGridReference)->type = REFERENCE_TYPE_SUBMODEL;
			(*currentGridReference)->isSubModelReference = true;
			(*currentGridReference)->firstReferenceWithinSubModel = new LDreference();
			createGridLine((*currentGridReference)->firstReferenceWithinSubModel, axis1Primary, axis2, axis3, axis2Position, axis3Position, LDgridSize);

			(*currentGridReference)->next = new LDreference();
			(*currentGridReference) = (*currentGridReference)->next;

			axis3Position = axis3Position + getVecValue(LDgridSnapPos, axis3);
		}
		axis2Position = axis2Position + getVecValue(LDgridSnapPos, axis2);
	}
}

void ldrawVRimportModel::createGridLine(LDreference* firstReferenceInGridLine, int axis1Primary, int axis2, int axis3, double axis2pos, double axis3pos, vec* LDgridSize)
{
	LDreference* currentReferenceInGridLine = firstReferenceInGridLine;
	createGridLineFace(&currentReferenceInGridLine, axis2, axis3, axis1Primary, createVector(axis2pos, axis3pos, -getVecValue(LDgridSize, axis1Primary)/2.0), LDRAW_VR_LD_GRID_WIDTH, LDRAW_VR_LD_GRID_WIDTH);
	createGridLineFace(&currentReferenceInGridLine, axis2, axis3, axis1Primary, createVector(axis2pos, axis3pos, getVecValue(LDgridSize, axis1Primary)/2.0), LDRAW_VR_LD_GRID_WIDTH, LDRAW_VR_LD_GRID_WIDTH);
	createGridLineFace(&currentReferenceInGridLine, axis1Primary, axis2, axis3, createVector(0.0, axis2pos, axis3pos+(LDRAW_VR_LD_GRID_WIDTH/2.0)), getVecValue(LDgridSize, axis1Primary), LDRAW_VR_LD_GRID_WIDTH);
	createGridLineFace(&currentReferenceInGridLine, axis1Primary, axis2, axis3, createVector(0.0, axis2pos, axis3pos-(LDRAW_VR_LD_GRID_WIDTH/2.0)), getVecValue(LDgridSize, axis1Primary), LDRAW_VR_LD_GRID_WIDTH);
	createGridLineFace(&currentReferenceInGridLine, axis1Primary, axis3, axis2, createVector(0.0, axis3pos, axis2pos+(LDRAW_VR_LD_GRID_WIDTH/2.0)), getVecValue(LDgridSize, axis1Primary), LDRAW_VR_LD_GRID_WIDTH);
	createGridLineFace(&currentReferenceInGridLine, axis1Primary, axis3, axis2, createVector(0.0, axis3pos, axis2pos-(LDRAW_VR_LD_GRID_WIDTH/2.0)), getVecValue(LDgridSize, axis1Primary), LDRAW_VR_LD_GRID_WIDTH);
}

void ldrawVRimportModel::createGridLineFace(LDreference** currentReferenceInGridLine, int axis1, int axis2, int axis3Secondary, vec gridLineFacePosition, double axis1GridLineFaceSize, double axis2GridLineFaceSize)
{
	(*currentReferenceInGridLine)->type = REFERENCE_TYPE_QUAD;
	(*currentReferenceInGridLine)->name = "irrelevant";
	(*currentReferenceInGridLine)->colour = LDRAW_VR_LD_GRID_COLOUR;
	(*currentReferenceInGridLine)->absoluteColour = LDRAW_VR_LD_GRID_COLOUR;

	setVecValue(&((*currentReferenceInGridLine)->vertex1absolutePosition), axis1, getVecValue(&gridLineFacePosition, 1) - axis1GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex1absolutePosition), axis2, getVecValue(&gridLineFacePosition, 2) - axis2GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex1absolutePosition), axis3Secondary, getVecValue(&gridLineFacePosition, 3));

	setVecValue(&((*currentReferenceInGridLine)->vertex2absolutePosition), axis1, getVecValue(&gridLineFacePosition, 1) + axis1GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex2absolutePosition), axis2, getVecValue(&gridLineFacePosition, 2) - axis2GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex2absolutePosition), axis3Secondary, getVecValue(&gridLineFacePosition, 3));

	setVecValue(&((*currentReferenceInGridLine)->vertex3absolutePosition), axis1, getVecValue(&gridLineFacePosition, 1) + axis1GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex3absolutePosition), axis2, getVecValue(&gridLineFacePosition, 2) + axis2GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex3absolutePosition), axis3Secondary, getVecValue(&gridLineFacePosition, 3));

	setVecValue(&((*currentReferenceInGridLine)->vertex4absolutePosition), axis1, getVecValue(&gridLineFacePosition, 1) - axis1GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex4absolutePosition), axis2, getVecValue(&gridLineFacePosition, 2) + axis2GridLineFaceSize/2.0);
	setVecValue(&((*currentReferenceInGridLine)->vertex4absolutePosition), axis3Secondary, getVecValue(&gridLineFacePosition, 3));

	(*currentReferenceInGridLine)->next = new LDreference();
	(*currentReferenceInGridLine) = (*currentReferenceInGridLine)->next;
}

bool ldrawVRimportModel::getFilesFromFileList(string inputListFileName, vector<string>* inputListFileNameContents, int* numberOfLinesInList)
{
	bool result = true;
	*numberOfLinesInList = 0;
	ifstream parseFileObject(inputListFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//txt file does not exist in current directory.
		cout << "Error: input list file does not exist in current directory: " << inputListFileName << endl;
		result = false;
	}
	else
	{
		char currentToken;
		int fileNameIndex = 0;
		int charCount = 0;
		string currentFileName = "";
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEWLINE)
			{
				inputListFileNameContents->push_back(currentFileName);
				currentFileName = "";
				fileNameIndex++;
			}
			else
			{
				currentFileName = currentFileName + currentToken;
			}
			charCount++;
		}
		*numberOfLinesInList = fileNameIndex;
	}
	return result;
}




TArray<FString> ldrawVRimportModel::getAllFilesInDirectory2(FString folderNameF, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension)
{
	FString directory = folderNameF;

	/**
	Gets all the files in a given directory.
	@param directory The full path of the directory we want to iterate over.
	@param fullpath Whether the returned list should be the full file paths or just the filenames.
	@param onlyFilesStartingWith Will only return filenames starting with this string. Also applies onlyFilesEndingWith if specified.
	@param onlyFilesEndingWith Will only return filenames ending with this string (it looks at the extension as well!). Also applies onlyFilesStartingWith if specified.
	@return A list of files (including the extension).
	*/

	   // Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*directory, Visitor);
	TArray<FString> files;

	for(TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if filename starts with required characters
		if(!onlyFilesStartingWith.IsEmpty())
		{
			const FString left = fileName.Left(onlyFilesStartingWith.Len());

			if (!(fileName.Left(onlyFilesStartingWith.Len()).Equals(onlyFilesStartingWith)))
			{
			   shouldAddFile = false;
			}
		}

		// Check if file extension is required characters
		if(!onlyFilesWithExtension.IsEmpty())
		{
			string fileNameSS = ldrawVRops.convertFStringToString(fileName);
			string onlyFilesWithExtensionSS = ldrawVRops.convertFStringToString(onlyFilesWithExtension);
			if(fileNameSS.length() > onlyFilesWithExtensionSS.length())
			{
				if(fileNameSS.substr(fileNameSS.length() - onlyFilesWithExtensionSS.length(), onlyFilesWithExtensionSS.length()) != onlyFilesWithExtensionSS)
				{
					shouldAddFile = false;
				}
			}
			else
			{
				shouldAddFile = false;
			}
			/*doesn't work;
			if(!(FPaths::GetExtension(fileName, false).Equals(onlyFilesWithExtension, ESearchCase::IgnoreCase)))
			{
				shouldAddFile = false;
			}
			*/
		}

		// Add full path to results
		if(shouldAddFile)
		{
			files.Add(fullPath ? filePath : fileName);
		}
	}

	return files;
}

void ldrawVRimportModel::setActorSpawnParameters(FActorSpawnParameters* spawnInfo)
{
	//https://answers.unrealengine.com/questions/13953/programmaticly-add-an-instance-of-an-mesh.html
	//spawnInfo->bNoCollisionFail = true;	//DEPRECIATED
	spawnInfo->Owner = NULL;
	spawnInfo->Instigator = NULL;
	spawnInfo->bDeferConstruction = false;
}


vec ldrawVRimportModel::createVector(double x, double y, double z)
{
	vec newVector;
	newVector.x = x;
	newVector.y = y;
	newVector.z = z;
	return newVector;
}


FVector ldrawVRimportModel::calculateNormal2(vec* vertex1absolutePosition, vec* vertex2absolutePosition, vec* vertex3absolutePosition)
{
	vec normal;
	SHAREDvector.calculateNormalOfTri(vertex1absolutePosition, vertex2absolutePosition, vertex3absolutePosition, &normal);
	FVector normalF = ldrawVRops.convertVectorToFVector(&normal);
	return normalF;
}

void ldrawVRimportModel::convertTritoTriangles(TArray<int32>& triangles, int32 vert0, int32 vert1, int32 vert2)
{
	triangles.Add(vert0);
	triangles.Add(vert1);
	triangles.Add(vert2);
	#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_TWO_SIDED
	triangles.Add(vert2);
	triangles.Add(vert1);
	triangles.Add(vert0);
	#endif
}

void ldrawVRimportModel::convertQuadtoTriangles(TArray<int32>& triangles, int32 vert0, int32 vert1, int32 vert2, int32 vert3)
{
	//preconditions: assumes the the quad has been defined with vertices in order (clockwise or counterclockwise)
	//really need to use the C++ equivalent function of;
	//https://docs.unrealengine.com/latest/INT/BlueprintAPI/Components/ProceduralMesh/ConvertQuadtoTriangles/index.html

	triangles.Add(vert0);
	triangles.Add(vert1);
	triangles.Add(vert2);
	triangles.Add(vert2);
	triangles.Add(vert3);
	triangles.Add(vert0);
	#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_TWO_SIDED
	triangles.Add(vert0);
	triangles.Add(vert3);
	triangles.Add(vert2);
	triangles.Add(vert2);
	triangles.Add(vert1);
	triangles.Add(vert0);
	#endif
}

bool ldrawVRimportModel::findVertexID(TArray<FVector>& vertices, vec v)
{
	bool result = false;
	FVector vectF = ldrawVRops.convertVectorToFVector(&v);
	for(int i=0; i<vertices.Num(); i++)
	{
		if(vectF == vertices[i])	//CHECKTHIS comparison method is valid
		{
			result = true;
		}
	}
	return result;
}

int32 ldrawVRimportModel::getVertexID(TArray<FVector>& vertices, vec v)
{
	bool result = false;
	int32 index = 0;
	FVector vectF = ldrawVRops.convertVectorToFVector(&v);
	for(int32 i=0; i<vertices.Num(); i++)
	{
		if(vectF == vertices[i])	//CHECKTHIS comparison method is valid
		{
			index = i;
			result = true;
		}
	}
	if(!result)
	{
		ldrawVRops.printMessage("getVertexID{} error, cannot find vertex in vertices");
	}
	return index;
}

/*
bool ldrawVRimportModel::allSubpartsHaveSameAbsoluteColour(LDreference* firstReferenceInLayer, int colourToTest)
{
	bool result = true;
	LDreference* currentReference = firstReferenceInLayer;
	while(currentReference->next != NULL)
	{
		if((currentReference->type == REFERENCE_TYPE_SUBMODEL)
		|| (currentReference->type == REFERENCE_TYPE_LINE)
		|| (currentReference->type == REFERENCE_TYPE_TRI)
		|| (currentReference->type == REFERENCE_TYPE_QUAD))
		{
			if(currentReference->absoluteColour != colourToTest)
			{
				result = false;
			}
		}
		if(currentReference->isSubModelReference)
		{
			if(!allSubpartsHaveSameAbsoluteColour(currentReference->firstReferenceWithinSubModel, colourToTest))
			{
				result = false;
			}
		}
		currentReference = currentReference->next;
	}
	return result;

}
*/

void ldrawVRimportModel::setStaticMeshComponentParameters(AldrawVRpartActor* actor, UStaticMeshComponent* meshComponent)
{
	//configure parmeters
	meshComponent->bOwnerNoSee = false;
	meshComponent->bCastDynamicShadow = LDRAW_VR_DYNAMIC_SHADOWS;
	meshComponent->CastShadow = LDRAW_VR_SHADOWS;
	meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);

	//Visibility
	meshComponent->SetHiddenInGame(false);

	//Mobility
	//meshComponent->SetMobility(EComponentMobility::Movable);		//this doesn't work, must use actor->GetRootComponent()->SetMobility(EComponentMobility::Movable); 			//NB Static Mesh's wont change their mesh unless they are set to Mobility::Moveable
	actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	//Collision
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	#ifdef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
	meshComponent->SetSimulatePhysics(true);
	#else
	#ifdef LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT
	actor->setPhysicsOfRootComponent(false);
	#else
	actor->setPhysicsOfStaticMeshComponent(meshComponent, false);
	#endif
	#endif
	//consider moving above simulate physics setting
	meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComponent->SetNotifyRigidBodyCollision(true);
	meshComponent->bGenerateOverlapEvents = true;
	meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);
	#endif

}

void ldrawVRimportModel::setProceduralMeshComponentParameters(AldrawVRpartActor* actor, UProceduralMeshComponent* meshComponent, bool createCollision)
{
	//configure parmeters
	meshComponent->bOwnerNoSee = false;
	meshComponent->bCastDynamicShadow = LDRAW_VR_DYNAMIC_SHADOWS;
	meshComponent->CastShadow = LDRAW_VR_SHADOWS;
	meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);

	//Visibility
	meshComponent->SetHiddenInGame(false);

	//Mobility
	//meshComponent->SetMobility(EComponentMobility::Movable);	//this doesn't work, must use actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);

	//Collision
	if(createCollision)
	{
		#ifdef LDRAW_VR_SIMULATE_PHYSICS
		#ifdef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
		meshComponent->SetSimulatePhysics(true);
		#else
		#ifdef LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT
		actor->setPhysicsOfRootComponent(false);
		#else
		actor->setPhysicsOfProceduralMeshComponent(meshComponent, false);
		#endif
		#endif
		//consider moving above simulate physics setting
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;
		meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);
		#endif
	}
}


string ldrawVRimportModel::generateMeshName(string partName, int absoluteColour)
{
	/*
	#ifdef LDRAW_VR_UNIQUE_STATIC_MESH_FOR_EACH_COLOUR
	string colourString = convertIntToString(absoluteColour);
	string meshName = partName + UNREAL_MESH_NAME_COLOUR_SUFFIX + colourString;
	#else
	string meshName = partName;
	#endif
	*/
	string meshName = partName;
	return meshName;
}

FString ldrawVRimportModel::generateStaticMeshPathName(string assetName, int absoluteColour)
{
	string assetDescription = string("StaticMesh");
	//ldrawVRops.printMessage("generateStaticMeshPathName{} assetDescription = " + assetDescription);
	#ifdef LDRAW_VR_UNIQUE_STATIC_MESH_FOR_EACH_COLOUR
	string colourString = convertIntToString(absoluteColour);
	string assetLocation = UNREAL_MESH_ASSET_LOCATION + UNREAL_MESH_NAME_COLOUR_SUFFIX + colourString + "/";
	return ldrawVRops.generateUnrealAssetPathName(assetDescription, assetLocation, assetName);
	#else
	string assetLocation = UNREAL_MESH_ASSET_LOCATION;
	return ldrawVRops.generateUnrealAssetPathName(assetDescription, assetLocation, assetName);
	#endif
}


double ldrawVRimportModel::getVecValue(vec* v, int index)
{
	double value = 0.0;
	if(index == AXIS_X)
	{
		value = v->x;
	}
	else if(index == AXIS_Y)
	{
		value = v->y;
	}
	else if(index == AXIS_Z)
	{
		value = v->z;
	}
	return value;
}

void ldrawVRimportModel::setVecValue(vec* v, int index, double value)
{
	if(index == AXIS_X)
	{
		v->x = value;
	}
	else if(index == AXIS_Y)
	{
		v->y = value;
	}
	else if(index == AXIS_Z)
	{
		v->z = value;
	}
}
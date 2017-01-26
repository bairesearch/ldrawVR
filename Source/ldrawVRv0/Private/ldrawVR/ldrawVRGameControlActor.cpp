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
* File Name: ldrawVRGameControlActor.cpp
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#include "ldrawVRv0.h"
#include "ldrawVRGameControlActor.h"
#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
//#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#endif

// Sets default values
AldrawVRGameControlActor::AldrawVRGameControlActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	initialiseSceneInitialReference();
	setScenePartActorList(new vector<AldrawVRpartActor*>);

	partSelectionWindowOpened = false;
	partSelectionWindowCurrentPageIndex = 0;
	partSelectionWindowCurrentCategory = LDRAW_VR_CATEGORY_NAME_DEFAULT;
	partSelectionWindowCurrentColourIndex = DAT_FILE_DEFAULT_COLOUR;
	keypadDisplayCurrentEnteredText = "";
	//initialiseUserFolders();
	selectionColourIndex = LDRAW_VR_MODEL_DEFAULT_PART_COLOUR;
	LDgridDisplay = false;
}


// Called when the game starts or when spawned
void AldrawVRGameControlActor::BeginPlay()
{
	initialiseSelectedActor();
	ldrawVRops.setLDgridResolution(LDRAW_VR_LD_GRID_SNAP_DEFAULT);
	#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS
	ldrawVRimportMod.loadStaticPartsList(LDRAW_VR_SET_FILE_NAME_STATIC_MESHES, getUserDirSubfolder(LDRAW_VR_SET_FILE_LOCATION_DEFAULT), LDRAW_VR_SET_FILE_PARTS_FILE_EXTENSION);
	ldrawVRimportMod.loadDynamicPartsList(getUserDirSubfolder(LDRAW_VR_CUSTOM_PART_FILE_LOCATION_DEFAULT), LDRAW_VR_CUSTOM_PART_FILE_NAME_APPEND);
	#endif
	#ifdef LDRAW_VR_DEBUG_LOAD_EXAMPLE_MODEL_ON_STARTUP
	loadModelBP(LDRAW_VR_EXAMPLE_MODEL_NAME, getUserDirSubfolderF(LDRAW_VR_EXAMPLE_MODEL_LOCATION));
	#endif
	#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
	#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT_MODEL1
	saveModelBP("tmra1.ldr", getUserDirSubfolderF(LDRAW_VR_EXAMPLE_MODEL_LOCATION));
	#else
	saveModelBP("tmra2.ldr", getUserDirSubfolderF(LDRAW_VR_EXAMPLE_MODEL_LOCATION));
	#endif
	#endif
	#ifdef LDRAW_VR_DEBUG_MOVE_MODEL_ON_STARTUP
	AldrawVRpartActor* actor = getScenePartActorList()->back();
	FVector worldPosition = actor->GetActorLocation();
	worldPosition.X = worldPosition.X + 3000.0;
	FRotator worldRotation = actor->GetActorRotation();
	moveObjectBP(actor, worldPosition, worldRotation);
	#endif
	Super::BeginPlay();
}


// Called every frame
void AldrawVRGameControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#ifndef LDRAW_VR_LD_GRID_DEBUG
	if(LDgridDisplay)
	{
		repositionLDgridActor();
	}
	#endif
}

void AldrawVRGameControlActor::repositionLDgridActor()
{
	LDgridActor->SetActorLocation(getUserFocusPointSnapped());
}

FVector AldrawVRGameControlActor::getUserFocusPointSnapped()
{
	#ifdef LDRAW_VR_LD_GRID_DEBUG
	FVector userFocusPointSnapped;
	userFocusPointSnapped.X = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_X;
	userFocusPointSnapped.Y = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Y;
	userFocusPointSnapped.Z = LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Z;
	#else
	FVector userFocusPoint = getUserFocusPoint();
	//ldrawVRops.printMessage("getUserFocusPointSnapped{}: userFocusPoint x = " + convertDoubleToString(userFocusPoint.X) + ", y = " + convertDoubleToString(userFocusPoint.X) + ", z = " + convertDoubleToString(userFocusPoint.Z));

	FVector userFocusPointSnapped;
	FRotator worldRotationUnrealTemp;
	worldRotationUnrealTemp.Roll = 0.0;
	worldRotationUnrealTemp.Pitch = 0.0;
	worldRotationUnrealTemp.Yaw = 0.0;
	FRotator worldRotationUnrealSnappedTemp;
	ldrawVRops.calculateSnapPosition(&userFocusPoint, &worldRotationUnrealTemp, &userFocusPointSnapped, &worldRotationUnrealSnappedTemp);
	//ldrawVRops.printMessage("getUserFocusPointSnapped{}: userFocusPointSnapped x = " + convertDoubleToString(userFocusPointSnapped.X) + ", y = " + convertDoubleToString(userFocusPointSnapped.X) + ", z = " + convertDoubleToString(userFocusPointSnapped.Z));
	#endif
	return userFocusPointSnapped;
}

FVector AldrawVRGameControlActor::getUserFocusPoint()
{
	FVector userFocusPoint;
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if(playerController && playerController->PlayerCameraManager)
	{
		FVector CameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
		FVector MyLocation = playerController->GetPawn()->GetActorLocation();
		FVector Direction = MyLocation - CameraLocation;
		Direction.Normalize();

		userFocusPoint = CameraLocation + Direction*LDRAW_VR_LD_GRID_DISTANCE_FROM_CAMERA;
	}
	else
	{
		ldrawVRops.printMessage("getUserFocusPoint{}: error - failed to get playerController->PlayerCameraManager");
	}

	#ifdef LDRAW_VR_LD_GRID_CORRECT_FOR_LOOKAT_Z_OFFSET
	userFocusPoint.Z = userFocusPoint.Z + 200.0;	//to correct for position of button (meaning camera looks downwards)
	#endif

	return userFocusPoint;
}







void AldrawVRGameControlActor::newModelBP()
{
	//clears existing model
	ldrawVRops.printMessage("AldrawVRGameControlActor::newModelBP{} warning: overwriting LD reference list; current loaded part will be discarded (ensure to save first to prevent this from happening)");
	ldrawVRimportMod.clearLDmodel(getSceneInitialReference(), *getScenePartActorList());
}

void AldrawVRGameControlActor::loadModelBP(FString LDModelFileNameF, FString LDModelFolderNameF)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray<FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_NAME, LDModelFolderNameF, LDModelFileNameF, LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		string LDModelFileName = "";
		string LDModelFolderName = "";
		getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[0], LDModelFolderName, LDModelFileName);
	#else
		if(ldrawVRops.convertFStringToString(LDModelFolderNameF) == "")
		{
			LDModelFolderNameF = getUserDirSubfolderF(LDRAW_VR_MODEL_FILE_LOCATION_DEFAULT);
		}
		string LDModelFileName = ldrawVRops.convertFStringToString(LDModelFileNameF);
		string LDModelFolderName = ldrawVRops.convertFStringToString(LDModelFolderNameF);
	#endif
		ldrawVRops.printMessage("AldrawVRGameControlActor::loadModel{}: " + LDModelFolderName + LDModelFileName);

		if(getSceneInitialReference() != NULL)
		{
			ldrawVRops.printMessage("AldrawVRGameControlActor::loadModel{} warning: overwriting LD reference list; current loaded part will be discarded (ensure to save first to prevent this from happening)");
		}

		newModelBP();	//clears existing model

		UWorld* theWorld = GetWorld();

		LDreference* topLevelReferenceInSceneFile = new LDreference(LDModelFileName, DAT_FILE_DEFAULT_COLOUR, false, true);	//OLD colour: 1
		topLevelReferenceInSceneFile->absoluteColour = DAT_FILE_DEFAULT_COLOUR;	//partSelectionWindowCurrentColourIndex;

		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION
		FVector worldPosition;
		worldPosition.X = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_X;
		worldPosition.Y = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Y;
		worldPosition.Z = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Z;
		vec translationVectorUnreal = ldrawVRops.convertFVectorToVector(&worldPosition);
		ldrawVRops.convertTranslationVectorUnrealToLD(&(topLevelReferenceInSceneFile->absolutePosition), &translationVectorUnreal);
		#endif
		#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ROTATION
		FRotator worldRotation;
		worldRotation.Roll = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_X;
		worldRotation.Pitch = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Y;
		worldRotation.Yaw = LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Z;
		ldrawVRops.convertRotationMatrixUnrealToLD(&(topLevelReferenceInSceneFile->absoluteDeformationMatrix), &worldRotation);
		#endif
		#endif

		FVector meshScaleVector;
		meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
		meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
		meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;

		ldrawVRimportMod.loadLDmodel(theWorld, LDModelFileName, LDModelFolderName, topLevelReferenceInSceneFile, getSceneInitialReference(), *getScenePartActorList(), meshScaleVector);
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	}
	#endif
}

void AldrawVRGameControlActor::saveModelBP(FString LDModelFileNameF, FString LDModelFolderNameF)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray<FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_NAME, LDModelFolderNameF, LDModelFileNameF, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		string LDModelFileName = "";
		string LDModelFolderName = "";
		getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[0], LDModelFolderName, LDModelFileName);
	#else
		if(ldrawVRops.convertFStringToString(LDModelFolderNameF) == "")
		{
			LDModelFolderNameF = getUserDirSubfolderF(LDRAW_VR_MODEL_FILE_LOCATION_DEFAULT);
		}
		string LDModelFileName = ldrawVRops.convertFStringToString(LDModelFileNameF);
		string LDModelFolderName = ldrawVRops.convertFStringToString(LDModelFolderNameF);
	#endif

		FVector meshScaleVector;	//not currently used
		meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
		meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
		meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;


		ldrawVRimportMod.saveLDmodel(LDModelFileName, LDModelFolderName, getSceneInitialReference(), *getScenePartActorList(), meshScaleVector);
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	}
	#endif
}

void AldrawVRGameControlActor::selectSetBP(FString LDSetFileNameF, FString LDSetFolderNameF)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray<FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_NAME, LDSetFolderNameF, LDSetFileNameF, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		string LDSetFileName = "";
		string LDSetFolderName = "";
		getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[0], LDSetFolderName, LDSetFileName);
	#else
		if(ldrawVRops.convertFStringToString(LDSetFolderNameF) == "")
		{
			LDSetFolderNameF = getUserDirSubfolderF(LDRAW_VR_SET_FILE_LOCATION_DEFAULT);
		}
		string LDSetFileName = ldrawVRops.convertFStringToString(LDSetFileNameF);
		string LDSetFolderName = ldrawVRops.convertFStringToString(LDSetFolderNameF);
	#endif
		string LDsetFilePathFull = LDSetFolderName + LDSetFileName;
		int numberOfLinesInList = 0;
		setPartsListFileNameContents.clear();
		if(!ldrawVRimportMod.getFilesFromFileList(LDsetFilePathFull, &setPartsListFileNameContents, &numberOfLinesInList))
		{
			ldrawVRops.printMessage("AldrawVRGameControlActor::selectSetBP{}: error: !getFilesFromFileList: LDsetFilePathFull = " + LDsetFilePathFull);
		}
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	}
	#endif
}

void AldrawVRGameControlActor::selectCategoryBP(FString LDCategoryFileNameF, FString LDCategoryFolderNameF)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray<FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_NAME, LDCategoryFolderNameF, LDCategoryFileNameF, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		string LDCategoryFileName = "";
		string LDCategoryFolderName = "";
		getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[0], LDCategoryFolderName, LDCategoryFileName);
	#else
		string LDCategoryFileName = ldrawVRops.convertFStringToString(LDCategoryFileNameF);
	#endif

		partSelectionWindowCurrentCategory = LDCategoryFileName.substr(0, LDCategoryFileName.length()-string(LDRAW_VR_CATEGORY_FILE_NAME_APPEND).length());
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	}
	#endif
}

void AldrawVRGameControlActor::selectColourBP(FString LDColourFileNameF, FString LDColourFolderNameF)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray<FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_NAME, LDColourFolderNameF, LDColourFileNameF, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		string LDColourFileName = "";
		string LDColourFolderName = "";
		getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[0], LDColourFolderName, LDColourFileName);
	#else
		string LDColourFileName = ldrawVRops.convertFStringToString(LDColourFileNameF);
	#endif

		partSelectionWindowCurrentColourIndex = SHAREDvars.convertStringToDouble(LDColourFileName.substr(0, LDColourFileName.length()-string(LDRAW_VR_COLOUR_FILE_NAME_APPEND).length()));
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	}
	#endif
}






void AldrawVRGameControlActor::insertModelBP(FString LDModelFileNameF, FString LDModelFolderNameF, bool insertAsSingleActor)
{
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray <FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_NAME, LDModelFolderNameF, LDModelFileNameF, LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		for(int i=0; i<fileNamesList.Num(); i++)
		{
			string LDModelFileName = "";
			string LDModelFolderName = "";
			getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[i], LDModelFolderName, LDModelFileName);
	#else
			if(ldrawVRops.convertFStringToString(LDModelFolderNameF) == "")
			{
				LDModelFolderNameF = getUserDirSubfolderF(LDRAW_VR_MODEL_FILE_LOCATION_DEFAULT);
			}
			string LDModelFileName = ldrawVRops.convertFStringToString(LDModelFileNameF);
			string LDModelFolderName = ldrawVRops.convertFStringToString(LDModelFolderNameF);
	#endif
			UWorld* theWorld = GetWorld();

			FVector meshScaleVector;
			meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
			meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
			meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;

			int defaultColour = partSelectionWindowCurrentColourIndex;	//if DAT_FILE_DEFAULT_COLOUR is used, then must recolour model/part afterwards

			ldrawVRimportMod.insertLDmodel(theWorld, LDModelFileName, LDModelFolderName, getSceneInitialReference(), *getScenePartActorList(), meshScaleVector, insertAsSingleActor, defaultColour);
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
		}
	}
	#endif
}


void AldrawVRGameControlActor::insertCustomPartBP(FString LDCustomPartFileNameF, FString LDCustomPartFolderNameF)
{
	//based on insertModelBP
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	TArray <FString> fileNamesList;
	if(getFileName(LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_NAME, LDCustomPartFolderNameF, LDCustomPartFileNameF, LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FILE_TYPES, LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FLAGS, fileNamesList))
	{
		for(int i=0; i<fileNamesList.Num(); i++)
		{
			string LDCustomPartFileName = "";
			string LDCustomPartFolder = "";
			getFileAndFolderNamesFromFileListArrayEntry(fileNamesList[i], LDCustomPartFolder, LDCustomPartFileName);
	#else
			if(ldrawVRops.convertFStringToString(LDCustomPartFolderNameF) == "")
			{
				LDCustomPartFolderNameF = getUserDirSubfolderF(LDRAW_VR_CUSTOM_PART_FILE_LOCATION_DEFAULT);
			}
			string LDCustomPartFileName = ldrawVRops.convertFStringToString(LDCustomPartFileNameF);
			string LDCustomPartFolderName = ldrawVRops.convertFStringToString(LDCustomPartFolderNameF);
	#endif
			UWorld* theWorld = GetWorld();
			FVector worldPosition;	//TODO: modify this based on current user selection/view
			worldPosition.X = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_X;
			worldPosition.Y = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_Y;
			worldPosition.Z = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_Z;
			FRotator worldRotation;	//TODO: modify this based on current user selection/view
			worldRotation.Roll = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_X;
			worldRotation.Pitch = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_Y;
			worldRotation.Yaw = LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_Z;
			FVector meshScaleVector;
			meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;
			meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;
			meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;

			LDreference* newReferenceInSceneFile = getSceneInitialReference();
			while(newReferenceInSceneFile->next != NULL)
			{
				newReferenceInSceneFile = newReferenceInSceneFile->next;
			}
			ldrawVRops.convertTransformationMatrixCoordinateSystemUnrealToLD(&(newReferenceInSceneFile->absolutePosition), &(newReferenceInSceneFile->absoluteDeformationMatrix), &worldPosition, &worldRotation);
			newReferenceInSceneFile->name = LDCustomPartFileName;
			newReferenceInSceneFile->absoluteColour = partSelectionWindowCurrentColourIndex;	//if DAT_FILE_DEFAULT_COLOUR is used, then must recolour model/part afterwards
			newReferenceInSceneFile->type = REFERENCE_TYPE_SUBMODEL;
			newReferenceInSceneFile->isSubModelReference = false;
			newReferenceInSceneFile->firstReferenceWithinSubModel = new LDreference();
			newReferenceInSceneFile->next = new LDreference();
			#ifdef LDRAW_VR_IO_OPTIMISATION
			newReferenceInSceneFile->partAdditional = true;
			#endif

			int partIndex = 0;	//irrelevant (not required for spawnActorAndDynamicMeshByPartName/spawnActorAndStaticMeshByPartName, as there is only 1 mesh component per actor)
			ldrawVRimportMod.spawnActorAndDynamicMeshByPartName(theWorld, *getScenePartActorList(), LDCustomPartFileName, LDCustomPartFolderName, newReferenceInSceneFile, &partIndex, meshScaleVector);
	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
		}
	}
	#endif
}





void AldrawVRGameControlActor::createPartSelectionWindowBP(FString LDpartCategoryNameF, int pageIndex)
{
	ldrawVRops.printMessage("1 AldrawVRGameControlActor::createPartSelectionWindowBP{}");

	//FUTURE: switch from YZ (vertical) to YX (horizontal)
	if(!partSelectionWindowActorList.empty())
	{
		clearPartSelectionWindowBP();
	}

	if(pageIndex == -1)
	{
		pageIndex = partSelectionWindowCurrentPageIndex;
	}
	else
	{
		partSelectionWindowCurrentPageIndex = pageIndex;
	}
	if(LDpartCategoryNameF == "")
	{
		LDpartCategoryNameF = ldrawVRops.convertStringToFString(partSelectionWindowCurrentCategory);
	}

	string LDpartCategoryName = ldrawVRops.convertFStringToString(LDpartCategoryNameF);
	string LDpartCategoryNameLowerCase = LDpartCategoryName;
	LDpartCategoryNameLowerCase[0] = tolower(LDpartCategoryName[0]);

	ldrawVRops.printMessage("AldrawVRGameControlActor::createPartSelectionWindowBP{}: LDpartCategoryName = " + LDpartCategoryName);
	//ldrawVRops.printMessage("AldrawVRGameControlActor::createPartSelectionWindowBP{}: pageIndex = " + convertDoubleToString(pageIndex));
	//ldrawVRops.printMessage("AldrawVRGameControlActor::createPartSelectionWindowBP{}: partSelectionWindowCurrentPageIndex = " + convertDoubleToString(partSelectionWindowCurrentPageIndex));


	int categoryPartIndex = 0;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
	int previousMaxRowHeight = 0.0;
	int currentMaxRowHeight = 0.0;
	int currentColWidth = 0.0;
	FVector previousPartSelectionWindowPartPosition;
	previousPartSelectionWindowPartPosition.X = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_X;
	previousPartSelectionWindowPartPosition.Y = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Y;
	previousPartSelectionWindowPartPosition.Z = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Z;
	#endif

	for(vector<string>::iterator iter = setPartsListFileNameContents.begin(); iter != setPartsListFileNameContents.end(); iter++)
	{
		string currentLine = *iter;

		if(currentLine.find(LDpartCategoryName) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			if((categoryPartIndex >= pageIndex*getPartsDisplayed(LDpartCategoryName)) && (categoryPartIndex < (pageIndex+1)*getPartsDisplayed(LDpartCategoryName)))
			{
				//ldrawVRops.printMessage("AldrawVRGameControlActor::createPartSelectionWindowBP{}: currentLine = " + currentLine);

				int indexOfDatFileExt = currentLine.find(LDRAW_VR_SET_FILE_PARTS_FILE_EXTENSION);
				if(indexOfDatFileExt != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					ldrawVRops.printMessage("4 AldrawVRGameControlActor::createPartSelectionWindowBP{}");

					string referenceName = currentLine.substr(0, indexOfDatFileExt+string(LDRAW_VR_SET_FILE_PARTS_FILE_EXTENSION).length());
					int pagePartIndex = categoryPartIndex - pageIndex*getPartsDisplayed(LDpartCategoryName);
					int pagePartRowIndex = pagePartIndex/getPartsDisplayedRows(LDpartCategoryName);
					int pagePartColIndex = pagePartIndex%getPartsDisplayedRows(LDpartCategoryName);

					//ldrawVRops.printMessage("AldrawVRGameControlActor::createPartSelectionWindowBP{}: referenceName = " + referenceName);

					UWorld* theWorld = GetWorld();

					FVector worldPosition;
					worldPosition.Z = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Z;
					if(pagePartColIndex == 0)
					{
						#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
						previousMaxRowHeight = currentMaxRowHeight;
						#endif
						worldPosition.Y = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Y;
					}
					else
					{
						#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
						worldPosition.Y = worldPosition.Y + currentColWidth + LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COL_PADDING;
						#else
						worldPosition.Y = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Y + pagePartColIndex*getPartsDisplayedColWidth(LDpartCategoryName);
						#endif
					}
					if(pagePartRowIndex == 0)
					{
						worldPosition.X = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_X;
					}
					else
					{
						#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
						worldPosition.X = worldPosition.X + previousMaxRowHeight + LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROW_PADDING;
						#else
						worldPosition.X = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_X + pagePartRowIndex*getPartsDisplayedRowHeight(LDpartCategoryName);
						#endif
					}
					FRotator worldRotation;
					worldRotation.Roll = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_X;
					worldRotation.Pitch = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_Y;
					worldRotation.Yaw = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_Z;
					FVector meshScaleVector;
					meshScaleVector.X = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
					meshScaleVector.Y = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;
					meshScaleVector.Z = LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;

					LDreference* newReferenceInSceneFile = new LDreference();
					ldrawVRops.convertTransformationMatrixCoordinateSystemUnrealToLD(&(newReferenceInSceneFile->absolutePosition), &(newReferenceInSceneFile->absoluteDeformationMatrix), &worldPosition, &worldRotation);
					newReferenceInSceneFile->name = referenceName;
					newReferenceInSceneFile->absoluteColour = partSelectionWindowCurrentColourIndex; //if DAT_FILE_DEFAULT_COLOUR is used, then must recolour part afterwards
					newReferenceInSceneFile->type = REFERENCE_TYPE_SUBMODEL;
					newReferenceInSceneFile->isSubModelReference = false;
					string partName = "";
					ldrawVRops.submodelIsPart(referenceName, &partName);


					int partIndex = 0;	//irrelevant (not required for spawnActorAndDynamicMeshByPartName/spawnActorAndStaticMeshByPartName, as there is only 1 mesh component per actor)
					AldrawVRpartActor* actor = ldrawVRimportMod.spawnActorAndStaticMeshByPartName(theWorld, partSelectionWindowActorList, partName, newReferenceInSceneFile, &partIndex, meshScaleVector);
					actor->spawnedPartFromPartSelectionWindow = true;
					#ifdef LDRAW_VR_SIMULATE_PHYSICS_NEVER_APPLY_PHYSICS_TO_BASEPLATES
					if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
					{
						actor->neverEnablePhysics = true;
					}
					#endif

					#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
					bool bOnlyCollidingComponents = false;	//CHECKTHIS (for performance)
					FVector Origin;
					FVector BoxExtent;
					actor->GetActorBounds(bOnlyCollidingComponents, Origin, BoxExtent);
					currentColWidth = (BoxExtent.Y - Origin.Y)/2.0;
					int currentRowHeight = (BoxExtent.X - Origin.X)/2.0;
					if(currentRowHeight > currentMaxRowHeight)
					{
						currentMaxRowHeight = currentRowHeight;
					}
					previousPartSelectionWindowPartPosition.X = worldPosition.X;
					previousPartSelectionWindowPartPosition.Y = worldPosition.Y;
					previousPartSelectionWindowPartPosition.Z = worldPosition.Z;
					#endif

					ldrawVRops.printMessage("5 AldrawVRGameControlActor::createPartSelectionWindowBP{}");

				}
			}
			categoryPartIndex++;
		}
	}
		ldrawVRops.printMessage("3 AldrawVRGameControlActor::createPartSelectionWindowBP{}");

}

int AldrawVRGameControlActor::getPartsDisplayedRows(string LDpartCategoryName)
{
	int partsDisplayedRows = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROWS;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
	{
		partsDisplayedRows = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROWS/LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
	}
	#endif
	return partsDisplayedRows;
}
int AldrawVRGameControlActor::getPartsDisplayedCols(string LDpartCategoryName)
{
	int partsDisplayedCols = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COLS;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
	{
		partsDisplayedCols = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COLS/LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
	}
	#endif
	return partsDisplayedCols;
}
int AldrawVRGameControlActor::getPartsDisplayed(string LDpartCategoryName)
{
	int partsDisplayed = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
	{
		int partsDisplayedRows = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROWS/LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
		int partsDisplayedCols = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COLS/LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
		partsDisplayed = partsDisplayedRows*partsDisplayedCols;
	}
	#endif
	return partsDisplayed;
}
int AldrawVRGameControlActor::getPartsDisplayedRowHeight(string LDpartCategoryName)
{
	int partsDisplayedRowHeight = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROW_HEIGHT;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
	{
		partsDisplayedRowHeight = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROW_HEIGHT*LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
	}
	#endif
	return partsDisplayedRowHeight;
}
int AldrawVRGameControlActor::getPartsDisplayedColWidth(string LDpartCategoryName)
{
	int partsDisplayedColWidth = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COL_WIDTH;
	#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	if(LDpartCategoryName == LDRAW_VR_CATEGORY_BASEPLATE_NAME)
	{
		partsDisplayedColWidth = LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COL_WIDTH*LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO;
	}
	#endif
	return partsDisplayedColWidth;
}


void AldrawVRGameControlActor::clearPartSelectionWindowBP()
{
	for(std::vector<AldrawVRpartActor*>::iterator iter = partSelectionWindowActorList.begin(); iter != partSelectionWindowActorList.end(); iter++)
	{
		AldrawVRpartActor* actor = *iter;
		if(!(actor->spawnedPartFromPartSelectionWindowHasBeenPickedUpByUser))
		{
			actor->Destroy();
		}
		if(!(actor->spawnedPartFromPartSelectionWindow))
		{
			ldrawVRops.printMessage("AldrawVRGameControlActor::clearPartSelectionWindowBP{} error: partSelectionWindowActorList actor !spawnedPartFromPartSelectionWindow");
		}
	}
	partSelectionWindowActorList.clear();
}







void AldrawVRGameControlActor::setCategoryBP(FString categoryName)
{
	partSelectionWindowCurrentCategory = ldrawVRops.convertFStringToString(categoryName);
}

void AldrawVRGameControlActor::setSetBP(FString LDpartSetName)
{
	string LDpartSetFileName = string(LDRAW_VR_SET_FILE_NAME_PREPEND) + ldrawVRops.convertFStringToString(LDpartSetName) + LDRAW_VR_SET_FILE_NAME_APPEND;

	ldrawVRops.printMessage("AldrawVRGameControlActor::setSetBP{}: LDpartSetFileName = " + LDpartSetFileName);

	selectSetBP(ldrawVRops.convertStringToFString(LDpartSetFileName));
}

void AldrawVRGameControlActor::setColourBP(int colourIndex)
{
	partSelectionWindowCurrentColourIndex = colourIndex;
}







void AldrawVRGameControlActor::setPartSelectionWindowCurrentPageIndex(int newPageIndex)
{
	if(newPageIndex >= 0)
	{
		partSelectionWindowCurrentPageIndex = newPageIndex;
	}
}

int AldrawVRGameControlActor::getPartSelectionWindowCurrentPageIndex()
{
	return partSelectionWindowCurrentPageIndex;
}



TArray<FString> AldrawVRGameControlActor::getAllFilesInDirectory(FString folderNameF, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension)
{
	return ldrawVRimportMod.getAllFilesInDirectory2(folderNameF, fullPath, onlyFilesStartingWith, onlyFilesWithExtension);
}


FTransform AldrawVRGameControlActor::getMainMenuWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getCategorySelectionWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSetSelectionWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getPartSelectionWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getColourSelectionWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}


FTransform AldrawVRGameControlActor::getLoadModelWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSaveModelWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getInsertCustomPartWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getInsertCustomModelWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectCustomColourWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectCustomCategoryWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectCustomSetWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getKeypadWithDisplayWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}



FTransform AldrawVRGameControlActor::getSelectTestHarnessWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectTestHarnessMoveWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectTestHarnessColourSelectionWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}

FTransform AldrawVRGameControlActor::getSelectTestHarnessSelectCustomColourWBActorTransform()
{
	FVector WBActorTranslation;
	WBActorTranslation.X = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_X;
	WBActorTranslation.Y = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Y;
	WBActorTranslation.Z = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Z;
	FRotator WBActorRotator;
	WBActorRotator.Roll = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_X;
	WBActorRotator.Pitch = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Y;
	WBActorRotator.Yaw = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Z;
	FVector WBActorScale;
	WBActorScale.X = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Y = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	WBActorScale.Z = LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE;
	FTransform WBActorTransform(WBActorRotator, WBActorTranslation, WBActorScale);
	return WBActorTransform;
}




void AldrawVRGameControlActor::setKeypadDisplayCurrentEnteredText(FString txt)
{
	keypadDisplayCurrentEnteredText = ldrawVRops.convertFStringToString(txt);
}

FString AldrawVRGameControlActor::getKeypadDisplayCurrentEnteredText()
{
	FString result = ldrawVRops.convertStringToFString(keypadDisplayCurrentEnteredText);
	keypadDisplayCurrentEnteredText = "";
	return result;
}

FString AldrawVRGameControlActor::getModelFolderName()
{
	FString result = getUserDirSubfolderF(LDRAW_VR_MODEL_FILE_LOCATION_DEFAULT);
	return result;
}
FString AldrawVRGameControlActor::getCategoryFolderName()
{
	FString result = getUserDirSubfolderF(LDRAW_VR_CATEGORY_FILE_LOCATION_DEFAULT);
	return result;
}
FString AldrawVRGameControlActor::getSetFolderName()
{
	FString result = getUserDirSubfolderF(LDRAW_VR_SET_FILE_LOCATION_DEFAULT);
	return result;
}
FString AldrawVRGameControlActor::getCustomPartFolderName()
{
	FString result = getUserDirSubfolderF(LDRAW_VR_CUSTOM_PART_FILE_LOCATION_DEFAULT);
	return result;
}
FString AldrawVRGameControlActor::getColourFolderName()
{
	FString result = getUserDirSubfolderF(LDRAW_VR_COLOUR_FILE_LOCATION_DEFAULT);
	return result;
}

FString AldrawVRGameControlActor::getModelFileNameExtension()
{
	FString result = ldrawVRops.convertStringToFString(string(LDRAW_VR_MODEL_FILE_NAME_APPEND));
	return result;
}
FString AldrawVRGameControlActor::getCategoryFileNameExtension()
{
	FString result = ldrawVRops.convertStringToFString(string(LDRAW_VR_CATEGORY_FILE_NAME_APPEND));
	return result;
}
FString AldrawVRGameControlActor::getSetFileNameExtension()
{
	FString result = ldrawVRops.convertStringToFString(string(LDRAW_VR_SET_FILE_NAME_APPEND));
	return result;
}
FString AldrawVRGameControlActor::getCustomPartFileNameExtension()
{
	FString result = ldrawVRops.convertStringToFString(string(LDRAW_VR_CUSTOM_PART_FILE_NAME_APPEND));
	return result;
}
FString AldrawVRGameControlActor::getColourFileNameExtension()
{
	FString result = ldrawVRops.convertStringToFString(string(LDRAW_VR_COLOUR_FILE_NAME_APPEND));
	return result;
}

void AldrawVRGameControlActor::setPartSelectionWindowOpened(const bool value)
{
	partSelectionWindowOpened = value;
}
bool AldrawVRGameControlActor::getPartSelectionWindowOpened()
{
	return partSelectionWindowOpened;
}



void AldrawVRGameControlActor::selectSelectedActorColourBP(FString LDColourFileNameF, FString LDColourFolderNameF)
{
	string LDColourFileName = ldrawVRops.convertFStringToString(LDColourFileNameF);
	setSelectedActorColourBP(SHAREDvars.convertStringToDouble(LDColourFileName.substr(0, LDColourFileName.length()-string(LDRAW_VR_COLOUR_FILE_NAME_APPEND).length())));
}
void AldrawVRGameControlActor::setSelectedActorColourBP(int colourIndex)
{
	selectionColourIndex = colourIndex;
}
int AldrawVRGameControlActor::getSelectedActorColourBP()
{
	return selectionColourIndex;
}
void AldrawVRGameControlActor::incrementSelectedActorColourBP()
{
	//ldrawVRops.printMessage("incrementSelectedActorColourBP: current selectionColourIndex = " + SHAREDvars.convertIntToString(selectionColourIndex));
	int selectionColourIndexIncrementIndex = 0;
	for(int i=0; i<LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS; i++)
	{
		if(selectionColourIndex == ldrawVRcolourMaterialInstanceLDcolourIndexArray[i])
		{
			selectionColourIndexIncrementIndex = i;
		}
	}
	if(selectionColourIndexIncrementIndex < LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS-1)
	{
		selectionColourIndex = ldrawVRcolourMaterialInstanceLDcolourIndexArray[selectionColourIndexIncrementIndex+1];
	}
	else
	{
		selectionColourIndex = ldrawVRcolourMaterialInstanceLDcolourIndexArray[0];	//consider setting LDRAW_VR_MODEL_DEFAULT_PART_COLOUR/DAT_FILE_COLOUR_BLACK/1 instead of DAT_FILE_DEFAULT_COLOUR
	}
	//ldrawVRops.printMessage("incrementSelectedActorColourBP: new selectionColourIndex = " + SHAREDvars.convertIntToString(selectionColourIndex));

}


void AldrawVRGameControlActor::setSelectedActorBP(AldrawVRpartActor* actor)
{
	setSelectedActor(actor);
}

AldrawVRpartActor* AldrawVRGameControlActor::getSelectedActorBP()
{
	return getSelectedActor();
}

void AldrawVRGameControlActor::THselectNextActorBP()
{
	if(getSelectedActor() != NULL)
	{
		int actorIndex = getActorIndexInSceneList(getSelectedActor());
		if(actorIndex < getScenePartActorList()->size()-1)
		{
			setSelectedActorBP((*getScenePartActorList())[actorIndex+1]);
		}
		else
		{
			setSelectedActorBP((*getScenePartActorList())[0]);
		}
	}
	else
	{
		if(getScenePartActorList()->size() > 0)
		{
			setSelectedActorBP((*getScenePartActorList())[0]);
		}
		else
		{
			ldrawVRops.printMessage("AldrawVRGameControlActor::THselectNextActor{} error: getScenePartActorList()->size() == 0");
		}
	}
}
int AldrawVRGameControlActor::getActorIndexInSceneList(AldrawVRpartActor* actor)
{
	int actorIndex = INT_DEFAULT_VALUE;
	for(int i=0; i<getScenePartActorList()->size(); i++)
	{
		if((*getScenePartActorList())[i] == actor)
		{
			actorIndex = i;
		}
	}
	if(actorIndex == INT_DEFAULT_VALUE)
	{
		ldrawVRops.printMessage("AldrawVRGameControlActor::getActorIndexInSceneList{} error: cannot find actor in scenePartActorList");
	}
	return actorIndex;
}

void AldrawVRGameControlActor::moveSelectedActorIncrementalBP(FVector incrementalPosition)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();

		FVector currentPosition = getSelectedActor()->GetActorLocation();

		FVector worldPositionUnreal = getSelectedActor()->GetActorLocation();
		vec translationVectorUnreal = ldrawVRops.convertFVectorToVector(&worldPositionUnreal);
		vec absolutePositionLD;
		ldrawVRops.convertTranslationVectorUnrealToLD(&absolutePositionLD, &translationVectorUnreal);
		if(incrementalPosition.X > 0)
		{
			absolutePositionLD.x = absolutePositionLD.x + ldrawVRops.getLDgridSnapPos().x;
		}
		else if(incrementalPosition.X < 0)
		{
			absolutePositionLD.x = absolutePositionLD.x - ldrawVRops.getLDgridSnapPos().x;
		}
		else if(incrementalPosition.Y > 0)
		{
			absolutePositionLD.y = absolutePositionLD.y + ldrawVRops.getLDgridSnapPos().y;
		}
		else if(incrementalPosition.Y < 0)
		{
			absolutePositionLD.y = absolutePositionLD.y - ldrawVRops.getLDgridSnapPos().y;
		}
		else if(incrementalPosition.Z > 0)
		{
			absolutePositionLD.z = absolutePositionLD.z + ldrawVRops.getLDgridSnapPos().z;
		}
		else if(incrementalPosition.Z < 0)
		{
			absolutePositionLD.z = absolutePositionLD.z - ldrawVRops.getLDgridSnapPos().z;
		}
		ldrawVRops.convertTranslationVectorLDtoUnreal(&absolutePositionLD, &translationVectorUnreal, LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT);
		worldPositionUnreal = ldrawVRops.convertVectorToFVector(&translationVectorUnreal);

		getSelectedActor()->SetActorLocation(worldPositionUnreal);
	}
}

void AldrawVRGameControlActor::rotateSelectedActorIncrementalBP(FRotator incrementalRotation)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();

		/*
		//thorough method (use LD coordinates)
		FRotator worldRotationUnreal = getSelectedActor()->GetActorRotation();
		mat* rotationMatrixLD
		convertRotationMatrixUnrealToLD(&rotationMatrixLD, &worldRotationUnreal);
		vec eulerLD = rotationMatrixToEulerAngles(rotationMatrixLD);
		eulerLD.x = eulerLD.x + incrementalRotation.Roll;
		eulerLD.y = eulerLD.y + incrementalRotation.Pitch;
		eulerLD.z = eulerLD.z + incrementalRotation.Yaw
		mat rotationMatrixUnreal;
		vec eulerUnreal;

		convertEulerToRotationMatrix(eulerLD.x, eulerLD.y, eulerLD.z, MATH_EULER_XYZ, rotationMatrixLD);
		convertRotationMatrixLDtoUnreal(&rotationMatrixLD, &rotationMatrixUnreal, &eulerUnreal, LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT);
			//alternate;
			//convertEulerToRotationMatrix(eulerLD.x, eulerLD.z, -(eulerLD.y+M_PI), MATH_EULER_YZX, &rotationMatrixUnreal);
			//eulerUnreal = rotationMatrixToEulerAngles(&rotationMatrixUnreal);

		vec eulerUnrealDegrees = convertRadiansToDegrees(&eulerUnreal);	//required for Unreal FRotator definition
		worldRotationUnreal->Roll = eulerUnrealDegrees.x;
		worldRotationUnreal->Pitch = eulerUnrealDegrees.y;
		worldRotationUnreal->Yaw = eulerUnrealDegrees.z;

		getSelectedActor()->SetActorRotation(worldRotationUnreal);
		*/

		getSelectedActor()->AddActorLocalRotation(incrementalRotation);
	}
}
/*
void AldrawVRGameControlActor::moveSelectedActorIncrementalBP(FVector incrementalPosition)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();
		FVector currentPosition = getSelectedActor()->GetActorLocation();
		currentPosition = currentPosition + incrementalPosition;
		getSelectedActor()->SetActorLocation(currentPosition);
	}
}
void AldrawVRGameControlActor::rotateSelectedActorIncrementalBP(FRotator incrementalRotation)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();
		//FRotator currentRotation = getSelectedActor()->GetActorRotation();
		//currentRotation = currentRotation + incrementalRotation;
		getSelectedActor()->AddActorLocalRotation(incrementalRotation);
	}
}
*/
void AldrawVRGameControlActor::moveSelectedActorBP(FVector newPosition)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();
		getSelectedActor()->SetActorLocation(newPosition);
		setSnapPosition(getSelectedActor());
	}
}
void AldrawVRGameControlActor::rotateSelectedActorBP(FRotator newRotation)
{
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->movedByUser();
		getSelectedActor()->SetActorRotation(newRotation);
		setSnapPosition(getSelectedActor());
	}
}


void AldrawVRGameControlActor::deleteSelectedActorBP()
{
	if(getSelectedActor() != NULL)
	{
		AldrawVRpartActor* oldSelectedActor = getSelectedActor();
		THselectNextActorBP();
		oldSelectedActor->deletedByUser();
	}
}

void AldrawVRGameControlActor::colourSelectedActorBP(int newColour)
{
	if(newColour == -1)
	{
		newColour = selectionColourIndex;
	}
	if(getSelectedActor() != NULL)
	{
		colourActorStaticMeshComponents(getSelectedActor(), newColour);
		colourActorDynamicMeshComponents(getSelectedActor(), newColour);
		getSelectedActor()->partReference->absoluteColour = newColour;
	}
}

//limitation: only recolours parts?
void AldrawVRGameControlActor::colourActorStaticMeshComponents(AldrawVRpartActor* actor, int newColour)
{
	TArray<UStaticMeshComponent*> meshComponents;
	actor->GetComponents<UStaticMeshComponent>(meshComponents);
	for(int i=0; i<meshComponents.Num(); i++)
	{
		UStaticMeshComponent* meshComponent = meshComponents[i];
		UStaticMesh* meshReference = meshComponent->GetStaticMesh();
		if(meshReference != NULL)
		{
			int numberOfElements = meshReference->GetNumSections(0);	//LOD index 0
			for(int x = 0; x < numberOfElements; x++)
			{
				UMaterialInterface* material = meshComponent->GetMaterial(x);
				string currentMaterialName = ldrawVRops.convertFStringToString(material->GetName());
				string currentMaterialLocation = ldrawVRops.convertFStringToString(material->GetPathName());

				bool highlighted = false;
				#ifdef LDRAW_VR_REASSIGN_MATERIALS_HIGHLIGHTED_WHEN_SELECTED
				if(currentMaterialLocation.find(UNREAL_MATERIAL_HIGHLIGHTED_ASSET_LOCATION_APPEND) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					highlighted = true;
				}
				#endif
				if(currentMaterialLocation.find(UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					//ldrawVRops.printMessage("AldrawVRGameControlActor::colourActorStaticMeshComponents{}: currentMaterialName = " + currentMaterialName);
					bool inheritedMaterial = true;
					UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(newColour, inheritedMaterial, highlighted);
					meshComponent->SetMaterial(x, replacementMaterial);
				}
			}
		}
	}
}

//limitation: only recolours parts?
void AldrawVRGameControlActor::colourActorDynamicMeshComponents(AldrawVRpartActor* actor, int newColour)
{
	TArray<UProceduralMeshComponent*> meshComponents;
	actor->GetComponents<UProceduralMeshComponent>(meshComponents);
	for(int i=0; i<meshComponents.Num(); i++)
	{
		UProceduralMeshComponent* meshComponent = meshComponents[i];
		int numberOfElements = meshComponent->GetNumSections();
		for(int x = 0; x < numberOfElements; x++)
		{
			UMaterialInterface* material = meshComponent->GetMaterial(x);
			if(material != NULL)
			{
				string currentMaterialName = ldrawVRops.convertFStringToString(material->GetName());
				string currentMaterialLocation = ldrawVRops.convertFStringToString(material->GetPathName());

				bool highlighted = false;
				#ifdef LDRAW_VR_REASSIGN_MATERIALS_HIGHLIGHTED_WHEN_SELECTED
				if(currentMaterialLocation.find(UNREAL_MATERIAL_HIGHLIGHTED_ASSET_LOCATION_APPEND) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					highlighted = true;
				}
				#endif
				if(currentMaterialLocation.find(UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					//ldrawVRops.printMessage("AldrawVRGameControlActor::colourActorDynamicMeshComponents{}: currentMaterialName = " + currentMaterialName);
					bool inheritedMaterial = true;
					UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(newColour, inheritedMaterial, highlighted);
					meshComponent->SetMaterial(x, replacementMaterial);
				}
			}
		}
	}
}


void AldrawVRGameControlActor::toggleLDgridResolutionBP()
{
	int getLDgridResolutionNew = (ldrawVRops.getLDgridResolution() + 1)%LDRAW_VR_LD_GRID_SNAP_NUM_TYPES;
	ldrawVRops.setLDgridResolution(getLDgridResolutionNew);
}

int AldrawVRGameControlActor::getLDgridResolutionBP()
{
	return ldrawVRops.getLDgridResolution();
}

void AldrawVRGameControlActor::setLDgridResolutionBP(int LDgridResolutionNew)
{
	ldrawVRops.setLDgridResolution(LDgridResolutionNew);
}

/*
void AldrawVRGameControlActor::setSnapPositionBP(AldrawVRpartActor* actor)
{
	actor->setSnapPosition(actor);
}
*/

FVector AldrawVRGameControlActor::getLDgridResolutionPositionBP()
{
	vec LDgridSnapPos = ldrawVRops.getLDgridSnapPos();
	FVector LDgridSnapPosF = ldrawVRops.convertVectorToFVector(&LDgridSnapPos);
	return LDgridSnapPosF;
}
float AldrawVRGameControlActor::getLDgridResolutionRotationBP()
{
	return float(ldrawVRops.getLDgridSnapRotDegrees());
}


void AldrawVRGameControlActor::toggleLDgridDisplayBP()
{
	if(LDgridDisplay)
	{
		LDgridDisplay = false;
		LDgridActor->Destroy();
	}
	else
	{
		LDgridDisplay = true;
		UWorld* theWorld = GetWorld();
		FVector meshScaleVector;
		meshScaleVector.X = 1.0*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;	//1.0;	//LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;	//CHECKTHIS: LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR
		meshScaleVector.Y = 1.0*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;	//1.0;	//LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;	//CHECKTHIS: LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR
		meshScaleVector.Z = 1.0*LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR;	//1.0;	//LDRAW_VR_TO_UNREAL_SCALE_DEFAULT;	//CHECKTHIS: LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR
		FVector worldPosition = getUserFocusPointSnapped();

		FRotator worldRotation;
		FRotator worldRotationLD;
		worldRotationLD.Yaw = 0.0;
		worldRotationLD.Pitch = 0.0;
		worldRotationLD.Roll = 0.0;
		ldrawVRops.convertRotatorLDtoUnreal(worldRotation, worldRotationLD);
		vec LDgridSnapPos = ldrawVRops.getLDgridSnapPos();
		LDgridActor = ldrawVRimportMod.spawnLDgridActorAndDynamicMesh(theWorld, worldPosition, worldRotation, meshScaleVector, &LDgridSnapPos);
	}
}

void AldrawVRGameControlActor::duplicateSelectedActorBP()
{
	if(getSelectedActor() != NULL)
	{
		FVector worldPositionSnapped;
		FRotator worldRotationSnapped;
		getDuplicatedActorLocation(getSelectedActor(), &worldPositionSnapped, &worldRotationSnapped);

		FVector worldPosition;
		worldPosition.X = 0.0;
		worldPosition.Y = 0.0;
		worldPosition.Z = 0.0;
		FRotator worldRotation;
		worldRotation.Roll = 0.0;
		worldRotation.Pitch = 0.0;
		worldRotation.Yaw = 0.0;
		FActorSpawnParameters spawnInfo;
		ldrawVRimportMod.setActorSpawnParameters(&spawnInfo);
		spawnInfo.Template = getSelectedActor();
		AldrawVRpartActor* newActor = GetWorld()->SpawnActor<AldrawVRpartActor>(worldPosition, worldRotation, spawnInfo);

		LDreference* newActorReference = new LDreference();
		newActorReference->name = getSelectedActor()->partReference->name;
		newActorReference->absoluteColour = getSelectedActor()->partReference->absoluteColour;
		newActorReference->type = getSelectedActor()->partReference->type;	//REFERENCE_TYPE_SUBMODEL;
		newActorReference->isSubModelReference = getSelectedActor()->partReference->isSubModelReference;	//redundant
		newActorReference->firstReferenceWithinSubModel = getSelectedActor()->partReference->firstReferenceWithinSubModel;	//redundant
		#ifdef LDRAW_VR_IO_OPTIMISATION
		newActorReference->partAdditional = true;
		#endif
		newActor->partReference = newActorReference;

		newActor->addToPartActorAndReferenceList();

		setSelectedActorBP(newActor);	//select the duplicated actor

		newActor->SetActorLocation(worldPositionSnapped);
		newActor->SetActorRotation(worldRotationSnapped);
	}
}

void AldrawVRGameControlActor::getDuplicatedActorLocation(AldrawVRpartActor* actor, FVector* worldPositionUnrealSnapped, FRotator* worldRotationUnrealSnapped)
{
	FVector duplicatedActorLocation;
	FRotator duplicatedActorRotation = actor->GetActorRotation();

	/*
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if(playerController && playerController->PlayerCameraManager)
	{
		//FVector CameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
		FVector MyLocation = playerController->GetPawn()->GetActorLocation();
		FVector Direction = MyLocation - actor->GetActorLocation();
		Direction.Normalize();

		duplicatedActorLocation = MyLocation - Direction*LDRAW_VR_LD_DUPLICATE_DISTANCE_FROM_EXISTING_ACTOR;
		ldrawVRops.printMessage("AldrawVRGameControlActor::getDuplicatedActorLocation{}: MyLocation x = " + convertDoubleToString(MyLocation.X) + ", y = " + convertDoubleToString(MyLocation.X) + ", z = " + convertDoubleToString(MyLocation.Z));

	}
	else
	{
		ldrawVRops.printMessage("AldrawVRGameControlActor::getDuplicatedActorLocation{}: error - failed to get playerController->PlayerCameraManager");
	}
	*/
	duplicatedActorLocation = actor->GetActorLocation();
	duplicatedActorLocation.X = duplicatedActorLocation.X - LDRAW_VR_LD_DUPLICATE_DISTANCE_FROM_EXISTING_ACTOR;

	ldrawVRops.calculateSnapPosition(&duplicatedActorLocation, &duplicatedActorRotation, worldPositionUnrealSnapped, worldRotationUnrealSnapped);
}


void AldrawVRGameControlActor::toggleLockSelectedActorToGridBP()
{
	//NB toggleLockSelectedActorToGridBP will lock to its current position (not necessarily grid)
	if(getSelectedActor()->isLockedToGrid)
	{
		getSelectedActor()->isLockedToGrid = false;
		#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS_DISABLE_WHEN_LOCKED_TO_GRID
		getSelectedActor()->highlightActor(true);	//change material to fancy emmisive (eg red glow)
		#endif
	}
	else
	{
		getSelectedActor()->isLockedToGrid = true;
		#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS_DISABLE_WHEN_LOCKED_TO_GRID
		getSelectedActor()->highlightActor(false);	//change material to original
		#endif
	}
}

bool AldrawVRGameControlActor::getSelectedActorLockToGridBP()
{
	return getSelectedActor()->isLockedToGrid;
}

bool AldrawVRGameControlActor::isActorLockedToGridBP(AActor* actor)
{
	bool locked = false;
	if(actor->IsA(AldrawVRpartActor::StaticClass()))
	{
		AldrawVRpartActor* partActor = dynamic_cast<AldrawVRpartActor*>(actor);
		locked = partActor->isLockedToGrid;
	}
	return locked;
}


void AldrawVRGameControlActor::initialiseUserFolders()
{
	//temporarily generate category file list
	vector<string> inputListFileNameContents;
	int numberOfLinesInList = 0;
	string inputListFileName = "E:/Files/design/ldrawVR/assetImport/categories/Category.txt";
	if(ldrawVRimportMod.getFilesFromFileList(inputListFileName, &inputListFileNameContents, &numberOfLinesInList))
	{
		for(std::vector<std::string>::iterator iter = inputListFileNameContents.begin(); iter != inputListFileNameContents.end(); iter++)
		{
			string currentLine = *iter;
			string fileName = string(getUserDirSubfolder(LDRAW_VR_CATEGORY_FILE_LOCATION_DEFAULT)) + currentLine + LDRAW_VR_CATEGORY_FILE_NAME_APPEND;
			SHAREDvars.writeStringToFile(fileName, &currentLine);
			ldrawVRops.printMessage("writeStringToFile = " + currentLine);
		}
	}
	//temporarily generate colour file list
	inputListFileNameContents.clear();
	numberOfLinesInList = 0;
	inputListFileName = "E:/Files/design/ldrawVR/assetImport/colours/Colours.txt";
	if(ldrawVRimportMod.getFilesFromFileList(inputListFileName, &inputListFileNameContents, &numberOfLinesInList))
	{
		for(std::vector<std::string>::iterator iter = inputListFileNameContents.begin(); iter != inputListFileNameContents.end(); iter++)
		{
			string currentLine = *iter;
			string fileName = string(getUserDirSubfolder(LDRAW_VR_COLOUR_FILE_LOCATION_DEFAULT)) + currentLine + LDRAW_VR_COLOUR_FILE_NAME_APPEND;
			SHAREDvars.writeStringToFile(fileName, &currentLine);
			ldrawVRops.printMessage("writeStringToFile = " + currentLine);
		}
	}
}

string AldrawVRGameControlActor::getUserDirSubfolder(string userDirSubfolderName)
{
	string userDir = ldrawVRops.convertFStringToString(FPlatformProcess::UserDir());	//C:/Users/rich/Documents/
	//string userDir = convertFStringToString(FPlatformProcess::UserSettingsDir()); C:/Users/rich/AppData/Local/

	string userDirSubfolder = userDir + LDRAW_VR_CONTENT_FOLDER_NAME + userDirSubfolderName;	//C:/Users/rich/Documents/ldrawVRcontent
	ldrawVRops.printMessage("userDirSubfolder = " + userDirSubfolder);
	//GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	return userDirSubfolder;
}

FString AldrawVRGameControlActor::getUserDirSubfolderF(string userDirSubfolderName)
{
	FString userDirSubfolderF = ldrawVRops.convertStringToFString(getUserDirSubfolder(userDirSubfolderName));
	return userDirSubfolderF;
}




#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
bool AldrawVRGameControlActor::getFileName(FString fileDialogName, FString fileDialogDefaultPathF, FString LDModelFileNameF, FString fileDialogDefaultFileTypes, int fileDialogFlags, TArray<FString>& fileNamesList)
{
	bool selectedFile = false;
	if(LDModelFileNameF == "")
	{
		//DEBUG ONLY
		//fileNamesToLoad.push_back(LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FILE_NAME);
		if(FDesktopPlatformModule::Get()->OpenFileDialog(NULL, fileDialogName, fileDialogDefaultPathF, LDModelFileNameF, fileDialogDefaultFileTypes, fileDialogFlags, fileNamesList))
		{
			selectedFile = true;
			#ifdef LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION
			for(int i=0; i<fileNamesList.Num(); i++)
			{
				FString badFilePathF = fileNamesList[i];
				string badFilePath = convertFStringToString(badFilePathF);
				if(badFilePath.find(LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION_BASE_PATH_BAD) == 0)
				{
					ldrawVRops.printMessage("LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION: getFileName{}: badFilePath detected, automatically correcting...");

					string correctedFilePath = string(LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION_BASE_PATH_CORRECTED) + badFilePath.substr(string(LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION_BASE_PATH_BAD).length());
					fileNamesList[i] = ldrawVRops.convertStringToFString(correctedFilePath);
				}

			}
			#endif
		}
	}
	else
	{
		string fileDialogDefaultPath = ldrawVRops.convertFStringToString(fileDialogDefaultPathF);
		string LDModelFileName = ldrawVRops.convertFStringToString(LDModelFileNameF);
		string fullPath = fileDialogDefaultPath + LDModelFileName;
		fileNamesList.Add(convertStringToFString(fullPath));
		selectedFile = true;
	}
	return selectedFile;
}
#endif

bool AldrawVRGameControlActor::detectWhiteSpace(char c)
{
	bool result = false;
	if(c == CHAR_SPACE || c == CHAR_TAB)
	{
		result = true;
	}
	return result;
}

bool AldrawVRGameControlActor::getFileAndFolderNamesFromFileListArrayEntry(FString fileNameListEntryF, string& LDModelFolderName, string& LDModelFileName)
{
	bool result = false;
	string fileNameListEntry = ldrawVRops.convertFStringToString(fileNameListEntryF);
	int indexOfFileName = fileNameListEntry.rfind(LDRAW_VR_PATH_NAME_DELIMITER);
	if(indexOfFileName != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		indexOfFileName = indexOfFileName + 1;
		LDModelFolderName = fileNameListEntry.substr(0, indexOfFileName);
		LDModelFileName = fileNameListEntry.substr(indexOfFileName);
		ldrawVRops.printMessage("getFileAndFolderNamesFromFileListArrayEntry: LDModelFolderName = " + LDModelFolderName);
		ldrawVRops.printMessage("getFileAndFolderNamesFromFileListArrayEntry: LDModelFileName = " + LDModelFileName);
		result = true;
	}
	return result;
}


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
* File Name: ldrawVRGameControlActor.h
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#pragma once

#include "GameFramework/Actor.h"
#include "ldrawVRimportModel.h"
#include "ldrawVRGameControlActor.generated.h"

//#define LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
//#define LDRAW_VR_DEBUG_LOAD_EXAMPLE_MODEL_ON_STARTUP
//#define LDRAW_VR_DEBUG_MOVE_MODEL_ON_STARTUP

#define LDRAW_VR_LD_DUPLICATE_DISTANCE_FROM_EXISTING_ACTOR (200/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO)

#define LDRAW_VR_WB_DEFAULT_WORLD_POS_Z 420.0
#define LDRAW_VR_WB_DEFAULT_WORLD_SCALE 0.5

#define LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION
#ifdef LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION
	#define LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION_BASE_PATH_BAD "../../../../../story/"
	#define LDRAW_VR_FIX_BUG_IN_OPEN_FILE_DIALOG_FUNCTION_BASE_PATH_CORRECTED "E:/Files/design/story/"
#endif

#define LDRAW_VR_CONTENT_FOLDER_NAME "ldrawVR/"

#define LDRAW_VR_CUSTOM_PART_FILE_LOCATION_DEFAULT "customParts/"
#define LDRAW_VR_CUSTOM_PART_FILE_NAME_APPEND ".dat"
#define LDRAW_VR_MODEL_FILE_LOCATION_DEFAULT "models/"
#define LDRAW_VR_MODEL_FILE_NAME_APPEND ".ldr"
#define LDRAW_VR_CATEGORY_FILE_LOCATION_DEFAULT "categories/"
#define LDRAW_VR_CATEGORY_FILE_NAME_APPEND ".cat"
#define LDRAW_VR_COLOUR_FILE_LOCATION_DEFAULT "colours/"
#define LDRAW_VR_COLOUR_FILE_NAME_APPEND ".col"

#define LDRAW_VR_PATH_NAME_DELIMITER "/"



#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_X -0.0
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_MAIN_MENU_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)

#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_X -10.0
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_PART_SELECTION_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)

#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_X -20.0
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_CATEGORY_SELECTION_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_X -20.0
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_SET_SELECTION_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_X -20.0
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Y 0.0	//400.0
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)

#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_LOAD_MODEL_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_SAVE_MODEL_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_INSERT_CUSTOM_PART_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_INSERT_CUSTOM_MODEL_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Y 0.0	//400
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_SELECT_CUSTOM_CATEGORY_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_SELECT_CUSTOM_SET_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)

#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_X -40.0
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_Y 0.0
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_KEYPAD_WITH_DISPLAY_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)

#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_X -20.0
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_Y 400.0
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_TEST_HARNESS_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_Y 400.0
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_TEST_HARNESS_MOVE_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_X -30.0
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Y 400.0
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_TEST_HARNESS_COLOUR_SELECTION_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_X -40.0
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Y 400
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_POS_Z (LDRAW_VR_WB_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_X 0.0
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Y 0.0
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_ROT_Z 180.0
#define LDRAW_VR_TEST_HARNESS_SELECT_CUSTOM_COLOUR_WB_ACTOR_WORLD_SCALE (LDRAW_VR_WB_DEFAULT_WORLD_SCALE)


#define LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_NAME "Open Model"
#define LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FILE_PATH "models/"
#define LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FILE_NAME ""
#define LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FILE_TYPES "*.ldr"
#define LDRAW_VR_MAIN_MENU_OPEN_FILE_DIALOG_DEFAULT_FLAGS EFileDialogFlags::None
#define LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_NAME "Save Model"
#define LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_PATH "models/"
#define LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_NAME "MyModel.ldr"
#define LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FILE_TYPES "*.ldr"
#define LDRAW_VR_MAIN_MENU_SAVE_FILE_DIALOG_DEFAULT_FLAGS EFileDialogFlags::None
#define LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_NAME "Insert Model"
#define LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FILE_PATH "models/"
#define LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FILE_NAME ""
#define LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FILE_TYPES "*.ldr"
#define LDRAW_VR_MAIN_MENU_INSERT_FILE_DIALOG_DEFAULT_FLAGS EFileDialogFlags::Multiple

#define LDRAW_VR_SET_FILE_LOCATION_DEFAULT "sets/"
#define LDRAW_VR_SET_FILE_NAME_DEFAULT "partsListCastle.lst"
#define LDRAW_VR_SET_FILE_NAME_ALL "partsListAll.lst"
#define LDRAW_VR_SET_FILE_NAME_STATIC_MESHES "partsListStaticMeshes.txt"
#define LDRAW_VR_SET_FILE_NAME_PREPEND "partsList"
#define LDRAW_VR_SET_FILE_NAME_APPEND ".lst"
#define LDRAW_VR_SET_FILE_PARTS_FILE_EXTENSION (LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION)	//.dat

#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COLS (12)
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROWS (6)
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED (LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COLS*LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROWS)
#define LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
#ifdef LDRAW_VR_PART_SELECTION_WINDOW_LOW_POOL_FOR_BASEPLATES
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_LOW_RATIO (3)
#endif
#define LDRAW_VR_CATEGORY_BASEPLATE_NAME "Baseplate"
#ifdef LDRAW_VR_BUILD
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_X (-220.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Y (0.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
#else
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_X (-600.0)
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Y (600.0)
#endif
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_POS_Z (LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_X (0.0)
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_Y (0.0)
#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_DEFAULT_WORLD_ROT_Z (0.0)
//#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
#ifdef LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DYNAMIC
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COL_PADDING (100/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO)
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROW_PADDING (100/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO)
#else
	#define LDRAW_VR_PART_SELECTION_WINDOW_PREVENT_OVERLAP_USING_BOUNDING_BOXES
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_COL_WIDTH (200/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO)
	#define LDRAW_VR_PART_SELECTION_WINDOW_PARTS_DISPLAYED_ROW_HEIGHT (200/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO)
#endif

#define LDRAW_VR_CATEGORY_NAME_BASEPLATE "Baseplate"
#define LDRAW_VR_CATEGORY_NAME_BRICK "Brick"
#define LDRAW_VR_CATEGORY_NAME_ELECTRIC "Electric"
#define LDRAW_VR_CATEGORY_NAME_MINFIG "Minifig"
#define LDRAW_VR_CATEGORY_NAME_PLATE "Plate"
#define LDRAW_VR_CATEGORY_NAME_SLOPE "Slope"
#define LDRAW_VR_CATEGORY_NAME_TECHNIC "Technic"
#define LDRAW_VR_CATEGORY_NAME_TILE "Tile"
#define LDRAW_VR_CATEGORY_NAME_TRAIN "Train"
#define LDRAW_VR_CATEGORY_NAME_DEFAULT LDRAW_VR_CATEGORY_NAME_BRICK

UCLASS()
class AldrawVRGameControlActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AldrawVRGameControlActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void repositionLDgridActor();
		FVector getUserFocusPointSnapped();
			FVector getUserFocusPoint();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void newModelBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void loadModelBP(FString LDModelFileNameF, FString LDModelFolderNameF = "");
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void saveModelBP(FString LDModelFileNameF, FString LDModelFolderNameF = "");
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void selectSetBP(FString LDSetFileNameF, FString LDSetFolderNameF = "");
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void selectCategoryBP(FString LDCategoryFileNameF, FString LDCategoryFolderNameF = "");
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void selectColourBP(FString LDColourFileNameF, FString LDColourFolderNameF = "");
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void insertModelBP(FString LDModelFileNameF, FString LDModelFolderNameF = "", bool insertAsSingleActor = true);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void insertCustomPartBP(FString LDCustomPartFileNameF, FString LDCustomPartFolderNameF = "");

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void createPartSelectionWindowBP(FString LDpartCategoryName = "", int pageIndex = -1);
		int getPartsDisplayedRows(string LDpartCategoryName);
		int getPartsDisplayedCols(string LDpartCategoryName);
		int getPartsDisplayed(string LDpartCategoryName);
		int getPartsDisplayedRowHeight(string LDpartCategoryName);
		int getPartsDisplayedColWidth(string LDpartCategoryName);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void clearPartSelectionWindowBP();

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setCategoryBP(FString categoryName);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setSetBP(FString LDpartSetName);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setColourBP(int colourIndex);

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setPartSelectionWindowCurrentPageIndex(int newPageIndex);	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	int getPartSelectionWindowCurrentPageIndex();	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	TArray<FString> getAllFilesInDirectory(const FString folderNameF, const bool fullPath = false, const FString onlyFilesStartingWith = TEXT(""), const FString onlyFilesEndingWith = TEXT(""));	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getMainMenuWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getCategorySelectionWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSetSelectionWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getPartSelectionWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getColourSelectionWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getLoadModelWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSaveModelWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getInsertCustomPartWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getInsertCustomModelWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectCustomColourWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectCustomCategoryWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectCustomSetWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getKeypadWithDisplayWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectTestHarnessWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectTestHarnessMoveWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectTestHarnessColourSelectionWBActorTransform();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FTransform getSelectTestHarnessSelectCustomColourWBActorTransform();	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setKeypadDisplayCurrentEnteredText(FString txt);	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getKeypadDisplayCurrentEnteredText();	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getModelFolderName();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getCategoryFolderName();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getSetFolderName();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getCustomPartFolderName();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getColourFolderName();	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getModelFileNameExtension();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getCategoryFileNameExtension();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getSetFileNameExtension();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getCustomPartFileNameExtension();	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	FString getColourFileNameExtension();	//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setPartSelectionWindowOpened(const bool value);	//must rename to BP
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	bool getPartSelectionWindowOpened();		//must rename to BP

	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void selectSelectedActorColourBP(FString LDColourFileNameF, FString LDColourFolderNameF);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setSelectedActorColourBP(int colourIndex);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	int getSelectedActorColourBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void incrementSelectedActorColourBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void setSelectedActorBP(AldrawVRpartActor* actor);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	AldrawVRpartActor* getSelectedActorBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void THselectNextActorBP();
		int getActorIndexInSceneList(AldrawVRpartActor* actor);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void moveSelectedActorIncrementalBP(FVector incrementalPosition);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void rotateSelectedActorIncrementalBP(FRotator incrementalRotation);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void moveSelectedActorBP(FVector newPosition);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void rotateSelectedActorBP(FRotator newRotation);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void deleteSelectedActorBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void colourSelectedActorBP(int newColour = -1);
		void colourActorStaticMeshComponents(AldrawVRpartActor* actor, int newColour);
		void colourActorDynamicMeshComponents(AldrawVRpartActor* actor, int newColour);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void toggleLDgridResolutionBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	int getLDgridResolutionBP();
		UFUNCTION(BlueprintCallable, Category = "ldrawVR")
		void setLDgridResolutionBP(int LDgridResolution);
		/*
		UFUNCTION(BlueprintCallable, Category = "ldrawVR")
		void setSnapPositionBP(AldrawVRpartActor* actor);
		*/
		UFUNCTION(BlueprintCallable, Category = "ldrawVR")
		FVector getLDgridResolutionPositionBP();
		UFUNCTION(BlueprintCallable, Category = "ldrawVR")
		float getLDgridResolutionRotationBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void toggleLDgridDisplayBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void duplicateSelectedActorBP();
		void getDuplicatedActorLocation(AldrawVRpartActor* actor, FVector* worldPositionUnrealSnapped, FRotator* worldRotationUnrealSnapped);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void toggleLockSelectedActorToGridBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	bool getSelectedActorLockToGridBP();
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	bool isActorLockedToGridBP(AActor* actor);

private:
	ldrawVRoperations ldrawVRops;
	SHAREDvarsClass SHAREDvars;

	ldrawVRimportModel ldrawVRimportMod;
	vector<AldrawVRpartActor*> partSelectionWindowActorList;
	bool partSelectionWindowOpened;
	int partSelectionWindowCurrentPageIndex;
	string partSelectionWindowCurrentCategory;
	int partSelectionWindowCurrentColourIndex;
	string keypadDisplayCurrentEnteredText;
	vector<std::string> setPartsListFileNameContents;
	int selectionColourIndex;
	bool LDgridDisplay;
	AldrawVRpartActor* LDgridActor;

	void initialiseUserFolders();
	string getUserDirSubfolder(string userDirSubfolderName);
	FString getUserDirSubfolderF(string userDirSubfolderName);

	#ifdef LDRAW_VR_DEBUG_GET_FILES_USING_WINDOWS_API
	bool getFileName(FString fileDialogName, FString fileDialogDefaultPath, FString LDModelFileNameF, FString fileDialogDefaultFileTypes, int fileDialogFlags, TArray<FString>& fileNamesList);
	#endif
	bool detectWhiteSpace(char c);
	bool getFileAndFolderNamesFromFileListArrayEntry(FString fileNameListEntryF, string& LDModelFolder, string& LDModelFileName);

};

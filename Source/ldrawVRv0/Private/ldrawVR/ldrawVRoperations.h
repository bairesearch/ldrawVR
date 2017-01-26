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
* File Name: ldrawVRoperations.h
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#pragma once

#include "../RT/SHAREDvector.hpp"
#include "../RT/SHAREDvars.hpp"
#include "../RT/LDreferenceClass.hpp"

#define LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR	//required for part/submodel highlighting/moving/etc

#define LDRAW_VR_TEMPLATE_MOTION_CONTROLLER
#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER
	#define LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_BP_MotionController_ACTOR_TAGNAME "bpmotioncontrollerActor"	//must set this tag in Unreal editor
	#define LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_BP_MotionController_SCENE_COMPONENT_TAGNAME "bpmotioncontrollerSceneComponent"	//must set this tag in Unreal editor
#endif

#define LDRAW_VR_BUILD	//else walk
#ifdef LDRAW_VR_BUILD
	#define LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM (1.0)	//default 1.0
#endif

//produces an isotropic meshScaleVector
#define LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_BUILD_RATIO (25.0)
#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER
	#define LDRAW_VR_TO_UNREAL_SCALE_WALK 0.25	//minifig simulation scale
	#define LDRAW_VR_TO_UNREAL_SCALE_BUILD (LDRAW_VR_TO_UNREAL_SCALE_WALK/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_BUILD_RATIO)	//0.01	//approximately real scale (such that parts can be manipuated by touch controller)
	#define LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z 270.0
#else
	#define LDRAW_VR_TO_UNREAL_SCALE_WALK 0.5	//minifig simulation scale
	#define LDRAW_VR_TO_UNREAL_SCALE_BUILD (LDRAW_VR_TO_UNREAL_SCALE_WALK/LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_BUILD_RATIO) //0.02	//approximately real scale (such that parts can be manipuated by touch controller)
	#define LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z 350.0
#endif
#ifdef LDRAW_VR_BUILD
	#define LDRAW_VR_TO_UNREAL_SCALE_DEFAULT (LDRAW_VR_TO_UNREAL_SCALE_BUILD*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
	#define LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO (LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_BUILD_RATIO/LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
#else
	#define LDRAW_VR_TO_UNREAL_SCALE_DEFAULT (LDRAW_VR_TO_UNREAL_SCALE_WALK)
	#define LDRAW_VR_TO_UNREAL_SCALE_WALK_TO_CHOSEN_RATIO (1.0)
#endif

#define LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_SCALING_FACTOR (4.0*LDRAW_VR_TO_UNREAL_SCALE_DEFAULT)	//parameters depend on desired coordinate system transformation (this is just an example)	//this appears to be a function of both the ldraw->blender->fbx units vs the unreal unit (10cm? vs cm; ie x10) and the ldraw to mm ratio (ie 1LDU = 0.4mm; ie x0.4); ie 10x0.4 = 4
#define LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_ROLL (0)
#define LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_PITCH (0)
#define LD_TO_UNREAL_COORDINATES_SYSTEM_TRANSFORMATION_YAW (0)


//values derived from MLCad
//#define LDRAW_VR_LD_GRID_SNAP_FINE_ENABLED
#define LDRAW_VR_LD_GRID_SNAP_COARSE_POS_X 10	//20
#define LDRAW_VR_LD_GRID_SNAP_COARSE_POS_Y 24
#define LDRAW_VR_LD_GRID_SNAP_COARSE_POS_Z 10	//20
#define LDRAW_VR_LD_GRID_SNAP_COARSE_ROT_DEGREES 90
#define LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_X 10	//5
#define LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Y 8	//4
#define LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Z 10	//5
#define LDRAW_VR_LD_GRID_SNAP_MEDIUM_ROT_DEGREES 90	//45
#ifdef LDRAW_VR_LD_GRID_SNAP_FINE_ENABLED
	#define LDRAW_VR_LD_GRID_SNAP_FINE_POS_X 1
	#define LDRAW_VR_LD_GRID_SNAP_FINE_POS_Y 1
	#define LDRAW_VR_LD_GRID_SNAP_FINE_POS_Z 1
	#define LDRAW_VR_LD_GRID_SNAP_FINE_ROT_DEGREES 15
#endif
/*
#define LDRAW_VR_LD_GRID_SNAP_DEFAULT_POS_X (LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_X)
#define LDRAW_VR_LD_GRID_SNAP_DEFAULT_POS_Y (LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Y)
#define LDRAW_VR_LD_GRID_SNAP_DEFAULT_POS_Z (LDRAW_VR_LD_GRID_SNAP_MEDIUM_POS_Z)
#define LDRAW_VR_LD_GRID_SNAP_DEFAULT_ROT_DEGREES (LDRAW_VR_LD_GRID_SNAP_MEDIUM_ROT_DEGREES)
*/
#define LDRAW_VR_LD_GRID_SNAP_COARSE (0)
#define LDRAW_VR_LD_GRID_SNAP_MEDIUM (1)
#ifdef LDRAW_VR_LD_GRID_SNAP_FINE_ENABLED
	#define LDRAW_VR_LD_GRID_SNAP_FINE (2)
	#define LDRAW_VR_LD_GRID_SNAP_NUM_TYPES (3)
#else
	#define LDRAW_VR_LD_GRID_SNAP_NUM_TYPES (2)
#endif
#define LDRAW_VR_LD_GRID_SNAP_DEFAULT (LDRAW_VR_LD_GRID_SNAP_COARSE)

#define LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_DEFAULT (0)
#define LDRAW_VR_UNREAL_TRANSFORMATION_ORDER_INTERMEDIATE (1)


#define MATH_EULER_XYZ (1)
#define MATH_EULER_XZY (2)
#define MATH_EULER_YXZ (3)
#define MATH_EULER_YZX (4)
#define MATH_EULER_ZXY (5)
#define MATH_EULER_ZYX (6)
#define MATH_EULER_NUM_VARIATIONS (6)




//#define LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
	//#define LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT_MODEL1
#endif

//moved from AldrawVRpartActor;

//#define LDRAW_VR_DEBUG_LOAD_CUBE_FOR_TRANSFORMATION_DEBUG
#ifdef LDRAW_VR_DEBUG_LOAD_CUBE_FOR_TRANSFORMATION_DEBUG
	#define UNREAL_MESH_ASSET_LOCATION "/Game/parts/trial/"
	#define UNREAL_MATERIAL_ASSET_LOCATION "/Game/parts/trial/"
	#define LDRAW_VR_EXAMPLE_MODEL_LOCATION "modelsTemp/examples/"
	#define LDRAW_VR_EXAMPLE_MODEL_NAME "cubeRotate.ldr"	//"cubeRotate2.ldr"	//""
#else
	#define UNREAL_ASSET_LOCATION "/Game/parts/"
	#define UNREAL_MESH_ASSET_FOLDER_NAME "meshes/"
	#define UNREAL_MESH_ASSET_LOCATION UNREAL_ASSET_LOCATION UNREAL_MESH_ASSET_FOLDER_NAME
	#define UNREAL_MATERIAL_ASSET_FOLDER_NAME "materials/"
	#define UNREAL_MATERIAL_ASSET_LOCATION UNREAL_ASSET_LOCATION UNREAL_MATERIAL_ASSET_FOLDER_NAME
	#define UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME "materialsInherited/"
	#define UNREAL_MATERIAL_INHERITED_ASSET_LOCATION UNREAL_ASSET_LOCATION UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME

	#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
		#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT_MODEL1
			#define LDRAW_VR_EXAMPLE_MODEL_NAME "testModelRotatedSmall1.ldr"
		#else
			#define LDRAW_VR_EXAMPLE_MODEL_NAME "testModelRotatedSmall2.ldr"
		#endif
		#define LDRAW_VR_EXAMPLE_MODEL_LOCATION "modelsTemp/examples/"
	#else
		#define LDRAW_VR_EXAMPLE_MODEL_NAME "scene.ldr"	//"scene2.ldr"
		#define LDRAW_VR_EXAMPLE_MODEL_LOCATION "modelsTemp/custommodelsx-x-10/"
	#endif
#endif

#define LDRAW_VR_UNREAL_USES_CASE_INSENSITIVE_ASSET_NAMES	//assuming this allows part file names in user custom models to be case insensitive
#define LDRAW_VR_UNREAL_ASSUME_UNIQUE_MATERIAL_PER_MESH_SECTION
//#define LDRAW_VR_OLD_INHERITED_MATERIAL_NAMES
#define LDRAW_VR_DYNAMIC_MESH_TRACK_RELATIVE_COLOURS
#ifdef LDRAW_VR_DYNAMIC_MESH_TRACK_RELATIVE_COLOURS
	#ifndef LDRAW_VR_OLD_INHERITED_MATERIAL_NAMES
		#define LDRAW_VR_ASSIGN_UNIQUE_MATERIAL_NAMES_FOR_INHERITED_MESHES
		#ifdef LDRAW_VR_ASSIGN_UNIQUE_MATERIAL_NAMES_FOR_INHERITED_MESHES
			#define LDRAW_VR_REASSIGN_MATERIALS_HIGHLIGHTED_WHEN_SELECTED
			#ifdef LDRAW_VR_REASSIGN_MATERIALS_HIGHLIGHTED_WHEN_SELECTED
				#define UNREAL_MATERIAL_HIGHLIGHTED_ASSET_LOCATION_APPEND "highlighted/"
			#endif
		#endif
	#endif
#endif

#define LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS (14)
static int ldrawVRcolourMaterialInstanceLDcolourIndexArray[LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS] = {DAT_FILE_DEFAULT_COLOUR, DAT_FILE_COLOUR_BLACK, DAT_FILE_COLOUR_BLUE, DAT_FILE_COLOUR_GREEN, DAT_FILE_COLOUR_CYAN, DAT_FILE_COLOUR_RED, DAT_FILE_COLOUR_MAGENTA, DAT_FILE_COLOUR_BROWN, DAT_FILE_COLOUR_LIGHTGREY, DAT_FILE_COLOUR_DARKGREY, DAT_FILE_COLOUR_YELLOW, DAT_FILE_COLOUR_WHITE, DAT_FILE_COLOUR_ORANGE, DAT_FILE_COLOUR_PURPLE};
//static string ldrawVRcolourMaterialInstanceNameArray[LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS] = {"LDdefault", "LDblack", "LDblue", "LDgreen", "LDcyan", "LDred", "LDmagenta", "LDbrown", "LDlightgrey", "LDdarkgrey", "LDyellow", "LDwhite", "LDorange", "LDpurple"};
#define LDRAWVR_NUMBER_MATERIAL_NAME_INDEX_PREPEND "Mat_"
#define LDRAWVR_NUMBER_MATERIAL_NAME_DELIMITER "_"
#ifdef LDRAW_VR_OLD_INHERITED_MATERIAL_NAMES
static string ldrawVRcolourMaterialInstanceNameArray[LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS] = {"Mat_0x2CCCCCC", "Mat_0", "Mat_1", "Mat_2", "Mat_11", "Mat_4", "Mat_5", "Mat_6", "Mat_7", "Mat_8", "Mat_14", "Mat_15", "Mat_25", "Mat_22"};
#else
static string ldrawVRcolourMaterialInstanceNameArray[LDRAWVR_NUMBER_MATERIAL_INSTANCE_COLOURS] = {"Mat_16", "Mat_0", "Mat_1", "Mat_2", "Mat_11", "Mat_4", "Mat_5", "Mat_6", "Mat_7", "Mat_8", "Mat_14", "Mat_15", "Mat_25", "Mat_22"};
#endif
#define LDRAWVR_MATERIAL_NAME_INDEX_DEFAULT "0x2CCCCCC"
#define LDRAWVR_MATERIAL_NAME_DEFAULT LDRAWVR_NUMBER_MATERIAL_NAME_INDEX_PREPEND LDRAWVR_MATERIAL_NAME_INDEX_DEFAULT


//#define LD_DEFAULT_LDRAW_PARTS_FOLDER_PATH "D:/Program Files (x86)/LDraw/parts/"
#define LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION ".dat"
#define LD_DEFAULT_LDRAW_PARTS_FILE_EXTENSION_UPPER ".DAT"
#define LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION ".ldr"
#define LD_DEFAULT_LDRAW_MODEL_FILE_EXTENSION_UPPER ".LDR"



class ldrawVRoperations
{
public:
	ldrawVRoperations();
	~ldrawVRoperations();

	vec getLDgridSnapPos();
	void setLDgridSnapPos(vec LDgridSnapPosNew);
	double getLDgridSnapRotDegrees();
	void setLDgridSnapRotDegrees(double LDgridSnapRotDegreesNew);
	int getLDgridResolution();
	void setLDgridResolution(int LDgridResolutionNew);
	void calculateSnapPosition(FVector* worldPositionUnreal, FRotator* worldRotationUnreal, FVector* worldPositionUnrealSnapped, FRotator* worldRotationUnrealSnapped);


	void convertTransformationMatrixCoordinateSystemUnrealToLD(vec* translationVectorLD, mat* rotationMatrixLD, FVector* translatorUnreal, FRotator* rotatorUnreal);
		void convertTranslationVectorUnrealToLD(vec* translationVectorLD, vec* translationVectorUnreal);
			double convertTranslationValueUnrealToLD(double translationValueUnreal);
		void convertRotationMatrixUnrealToLD(mat* rotationMatrixLD, FRotator* rotatorUnreal);
		#ifdef LDRAW_VR_DEBUG_AUTOMATICALLY_CALCULATE_UNREAL_LD_TRANSFORM_BASED_ON_EXPECTED_RESULT
		void convertRotationMatrixUnrealToLDcalibrate(mat* rotationMatrixLD, FRotator* rotatorUnreal);
		#endif
	void convertTransformationMatrixCoordinateSystemLDtoUnreal(vec* translationVectorLD, mat* rotationMatrixLD, FVector* translatorUnreal, FRotator* rotatorUnreal, int order);
		void convertTransformationMatrixCoordinateSystemLDtoUnreal(vec* translationVectorLD, mat* rotationMatrixLD, vec* translationVectorUnreal, mat* rotationMatrixUnreal, vec* eulerUnreal, int order);
			void convertTranslationVectorLDtoUnreal(vec* translationVectorLD, vec* translationVectorUnreal, int order);
			void convertRotationMatrixLDtoUnreal(mat* rotationMatrixLD, mat* rotationMatrixUnreal, vec* eulerUnreal, int order);
	void convertRotatorLDtoUnreal(FRotator& worldRotationUnreal, FRotator& worldRotationLD);

	FRotator convertEulerToRotator(vec* eulerUnrealRadians);
	vec convertRotatorToEuler(FRotator* rotatorUnreal);
	FVector convertVectorToFVector(vec* v);
	vec convertFVectorToVector(FVector* fvec);

	vec rotationMatrixToEulerAngles(mat* R);
	vec convertRadiansToDegrees(vec* v);
		double convertRadiansToDegrees(double value);
	vec convertDegreesToRadians(vec* v);
		double convertDegreesToRadians(double value);

	void convertEulerToRotationMatrix(double x, double y, double z, int mode, mat *matrix);
	void multiplyMatricies2(mat* mat1, mat* mat2, mat* matNew);

	void printMessage(string msg);

	FString convertStringToFString(string str);
	string convertFStringToString(FString fstring);
	string convertDoubleToString(double number);
	FName convertStringToFName(string meshComponentName);


	//moved from AldrawVRpartActor;

	UMaterialInterface* getReplacementMaterial(LDreference* reference, bool highlighted, bool allowColourChange);
		UMaterialInterface* getReplacementMaterial(int absoluteColour, bool inheritedMaterial, bool highlighted);
			UMaterialInterface* getReplacementMaterial(string materialName, bool inheritedMaterial, bool highlighted);
				int getColourFromMaterialName(string materialName);

	FString generateMaterialPathName(string assetName);
		FString generateMaterialPathName(string assetName, string assetLocation);
			FString generateUnrealAssetPathName(string assetDescription, string assetLocation, string assetName);

	bool submodelIsPart(string name, string* partName);

private:

	SHAREDvectorClass SHAREDvector;
	SHAREDvarsClass SHAREDvars;
};

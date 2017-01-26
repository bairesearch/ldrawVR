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
* File Name: ldrawVRimportModel.h
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#pragma once

#include "../RT/SHAREDglobalDefs.hpp"
#include "../RT/RToperations.hpp"
#include "../RT/LDparser.hpp"
#include "../RT/LDreferenceManipulation.hpp"
#include "ldrawVRpartActor.h"
#include "ProceduralMeshComponent.h"

#define LDRAW_VR_MODEL_DEFAULT_PART_COLOUR (DAT_FILE_COLOUR_BLACK)

#define LDRAW_VR_PROCEDURAL_GENERATED_MESH_SCALE_CORRECTION_FACTOR (4.0)	//why is this required?
#define LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT_DISABLE_FOR_PROCEDURAL_GENERATED_MESHES_DUE_TO_UNREAL_LIMITATION

#define LDRAW_VR_LD_GRID
#ifdef LDRAW_VR_LD_GRID
	#define LDRAW_VR_LD_GRID_CORRECT_FOR_LOOKAT_Z_OFFSET
	//#define LDRAW_VR_LD_GRID_DEBUG
	#define LDRAW_VR_LD_GRID_COLOUR (1000)	//special grid colour: parts/materials/Mat_1000 with colour set to black, with translucent, and opacity set to 0.1
	#define LDRAW_VR_LD_GRID_SIZE	(100.0) //in Unreal coordinates
	#define LDRAW_VR_LD_GRID_WIDTH (1.0) //in LD coordinates
	#define LDRAW_VR_LD_GRID_DISTANCE_FROM_CAMERA (200.0)	//in Unreal coordinates
#endif


#define LDRAW_VR_SHADOWS (false)	//when true this has no effect
#define LDRAW_VR_DYNAMIC_SHADOWS (false)	//when true this has an effect but is very slow for large scenes

#define LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION	//optional (depends on sandbox parameters; eg height)
#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION
	#define LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION
	#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION
		#ifdef LDRAW_VR_BUILD
			#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_X (-150.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
			#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Y (0.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
		#else
			#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_X (-1000.0)
			#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Y (1000.0)
		#endif
		#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_POS_Z (LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z)
		#define LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_TRANSLATION_METHOD2
	#endif
	#define LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ROTATION
	#ifdef LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ROTATION
		#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_X 0.0
		#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Y 0.0
		#define LDRAW_VR_MODEL_NEW_DEFAULT_WORLD_ROT_Z 0.0
	#endif
	//assume true: LDRAW_VR_LOAD_MODEL_APPLY_TOP_LEVEL_TRANSFORMATION_ENSURE_LOAD_AND_SAVE_CONSISTENCY
#endif

#ifdef LDRAW_VR_BUILD
	#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_X (-150.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
	#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Y (0.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
#else
	#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_X (-1000.0)
	#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Y (1000.0)
#endif
#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_POS_Z (LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z)
#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_X 0.0
#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_Y 0.0
#define LDRAW_VR_MODEL_INSERT_DEFAULT_WORLD_ROT_Z 0.0

#ifdef LDRAW_VR_BUILD
	#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_X (-150.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)	//make -200
	#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_Y (0.0*LDRAW_VR_TO_UNREAL_SCALE_BUILD_ZOOM)
#else
	#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_X -1000.0
	#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_Y 1000.0
#endif
#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_POS_Z (LDRAW_VR_MODEL_DEFAULT_WORLD_POS_Z)	//LDRAW_VR_WB_DEFAULT_WORLD_POS_Z
#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_X 0.0
#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_Y 0.0
#define LDRAW_VR_CUSTOM_PART_INSERT_DEFAULT_WORLD_ROT_Z 0.0



#define LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS
#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	#define LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_CORRECTION_METHOD3	//only apply top level correction if the reference is a genuine/ldr submodel (not if it is a part/dat)	//need to get transformation code to work without this hack
	//#define LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_FIRST_LAYER_USE_RELATIVE_RATHER_THAN_ABSOLUTE_TRANSFORMATIONS	//don't use this as it ignores the top level transformations
	#define LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_UNIQUE_MESH_COMPONENT_NAMES_PER_ACTOR
#else
	//#define LDRAW_VR_TRANSFORM_MESHES_RATHER_THAN_ACTORS	//not compatible with VR/editable scene
	#ifdef LDRAW_VR_TRANSFORM_MESHES_RATHER_THAN_ACTORS
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_X 0.0
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Y 0.0
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_POS_Z 0.0
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_X 0.0
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Y 0.0
		#define LDRAW_VR_MODEL_NEW_TEMP_DEFAULT_WORLD_ROT_Z 0.0
	#endif
#endif


#define LDRAW_VR_GENERATE_PROCEDURAL_MESH_TWO_SIDED	//in case part has not been defined according to the BFC (back face culling) standard





//#define LDRAW_VR_UNIQUE_STATIC_MESH_FOR_EACH_COLOUR
#ifdef LDRAW_VR_UNIQUE_STATIC_MESH_FOR_EACH_COLOUR
	#define UNREAL_MESH_NAME_COLOUR_SUFFIX "col"
#endif


#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR_FIRST_LAYER_USE_RELATIVE_RATHER_THAN_ABSOLUTE_TRANSFORMATIONS
#define LD_TRANSFORMATIONS_RELATIVE (0)
#endif
#define LD_TRANSFORMATIONS_INTERMEDIATE (1)	//same as absolute but without taking into account first layer transformations
#define LD_TRANSFORMATIONS_ABSOLUTE (2)

/**
 *
 */
class ldrawVRimportModel
{
public:
	ldrawVRimportModel();
	~ldrawVRimportModel();

	void clearLDmodel(LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList);
	void saveLDmodel(string LDModelFileName, string LDModelFolder, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector);
		void reconcileReferenceListForSave(LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList);
	void loadLDmodel(UWorld* theWorld, string LDModelFileName, string LDModelFolder, LDreference* topLevelReferenceInSceneFile, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector);
	void insertLDmodel(UWorld* theWorld, string LDModelFileName, string LDModelFolder, LDreference* sceneInitialReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector, bool insertAsSingleActor, int defaultColour);
	#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
	void insertLDmodelAsSingleActor(AldrawVRpartActor* actor, string LDModelFileName, string LDModelFolder, LDreference* topLevelReferenceInModelFile, LDreference* modelInitialReference, FVector& meshScaleVector);
	#endif
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
		void loadLDmodelFirstLayer(UWorld* theWorld, LDreference* firstReferenceInLayer, int* partIndex, vector<AldrawVRpartActor*>& scenePartActorList, FVector& meshScaleVector);
			void loadLDmodelLayer(AldrawVRpartActor* actor, LDreference* currentReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool firstLayer);
				void loadLDmodelRecurse(AldrawVRpartActor* actor, LDreference* firstReferenceInLayer, int* partIndex, FVector& meshScaleVector);
		#else
			void loadLDmodelLayer(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, LDreference* currentReference, int* partIndex, FVector& meshScaleVector);
				void loadLDmodelRecurse(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, LDreference* firstReferenceInLayer, int* partIndex, FVector& meshScaleVector);
		#endif
	AldrawVRpartActor* spawnActorAndStaticMeshByPartName(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, string partName, LDreference* partReference, int* partIndex, FVector& meshScaleVector);
		#ifdef LDRAW_VR_GROUP_SUBMODEL_PARTS_INTO_SINGLE_ACTOR
		void spawnStaticMeshByPartName(AldrawVRpartActor* actor, string partName, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool singleMeshPerActor);
		#endif
			void generateUnrealTransformations(LDreference* partReference, int transformationType, FVector& translatorUnreal, FRotator& rotatorUnreal, FVector& meshScaleVector);
			AldrawVRpartActor* spawnActor(UWorld* theWorld, string partName, LDreference* partReference, vector<AldrawVRpartActor*>& scenePartActorList, FVector& worldPosition, FRotator& worldRotation, FVector& meshScaleVector, bool transformMeshesRatherThanActors);
			void addStaticMeshToActor(AldrawVRpartActor* actor, string partName, LDreference* partReference, int* partIndex, bool transformMeshesRatherThanActors, FVector& translatorUnreal, FRotator& rotatorUnreal, FVector& meshScaleVector, bool allowColourChange, bool singleMeshPerActor);
	AldrawVRpartActor* spawnActorAndDynamicMeshByPartName(UWorld* theWorld, vector<AldrawVRpartActor*>& scenePartActorList, string LDCustomPartFileName, string LDCustomPartFolder, LDreference* partReference, int* partIndex, FVector& meshScaleVector);
		void spawnDynamicMeshByPartName(AldrawVRpartActor* actor, string LDCustomPartFileName, string LDCustomPartFolder, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool singleMeshPerActor);
			void addDynamicMeshToActor(AldrawVRpartActor* actor, LDreference* customPartInitialReference, LDreference* partReference, int* partIndex, FVector& meshScaleVector, bool transformMeshesRatherThanActors, int transformationType, bool allowColourChange, bool createCollision, bool singleMeshPerActor);
				bool generateDynamicMeshRecurse(LDreference** firstPolyInMeshChain, LDreference** currentPolyInMeshChain, LDreference* firstReferenceInLayer, UProceduralMeshComponent* meshComponent, int* meshSectionIndex, int* previousColour, bool allowColourChange, bool createCollision);
					void generateDynamicMeshSection(LDreference** firstPolyInMeshChain, LDreference** currentPolyInMeshChain, UProceduralMeshComponent* meshComponent, int* meshSectionIndex, bool allowColourChange, bool createCollision);
	AldrawVRpartActor* spawnLDgridActorAndDynamicMesh(UWorld* theWorld, FVector& worldPosition, FRotator& worldRotation, FVector& meshScaleVector, vec* LDgridSnapPos);
		void spawnLDgridDynamicMeshAxis(LDreference** currentGridReference, int axis1, int axis2, int axisIterate, vec* numberGridColumns, vec* LDgridSnapPos, vec* LDgridSize);
			void createGridLine(LDreference* firstReferenceInGridLine, int axis1Primary, int axis2, int axis3, double axis2pos, double axis3pos, vec* LDgridSize);
				void createGridLineFace(LDreference** currentReferenceInGridLine, int axis1, int axis2, int axis3Secondary, vec gridLineFacePosition, double axis1GridLineFaceSize, double axis2GridLineFaceSize);

	#ifdef LDRAW_VR_GENERATE_PROCEDURAL_MESH_FOR_SUBMODEL_PARTS
	vector<string> staticPartsList;	//ie internalPartsList
	vector<string> dynamicPartsList;	//ie customPartsList
	void loadStaticPartsList(string LDSetFileName, string LDSetFolderName, string LDSetExtensionName);
	void loadDynamicPartsList(string LDCustomPartFolderName, string LDCustomPartExtensionName);
	bool findInStaticPartsList(string referenceName);
	bool findInDynamicPartsList(string referenceName);
	string dynamicPartsLocation;
	#endif

	bool getFilesFromFileList(string inputListFileName, vector<string>* inputListFileNameContents, int* numberOfLinesInList);
	TArray<FString> getAllFilesInDirectory2(FString folderNameF, const bool fullPath, const FString onlyFilesStartingWith, const FString onlyFilesWithExtension);
	void setActorSpawnParameters(FActorSpawnParameters* spawnInfo);

private:

	ldrawVRoperations ldrawVRops;
	LDreferenceClassClass LDreferenceClass;
	LDparserClass LDparser;
	LDreferenceManipulationClass LDreferenceManipulation;
	SHAREDvectorClass SHAREDvector;
	SHAREDvarsClass SHAREDvars;

	vec createVector(double x, double y, double z);
	//static void splitQuadIntoTris(LDreference* quad, LDreference** currentPolyInMeshChain);
	//static bool allSubpartsHaveSameAbsoluteColour(LDreference* firstReferenceInLayer, int colourToTest);
		FVector calculateNormal2(vec* vertex1absolutePosition, vec* vertex2absolutePosition, vec* vertex3absolutePosition);
		void convertTritoTriangles(TArray<int32>& triangles, int32 vert0, int32 vert1, int32 vert2);
		void convertQuadtoTriangles(TArray<int32>& triangles, int32 vert0, int32 vert1, int32 vert2, int32 vert3);
		bool findVertexID(TArray<FVector>& vertices, vec v);
		int32 getVertexID(TArray<FVector>& vertices, vec v);


	void setStaticMeshComponentParameters(AldrawVRpartActor* actor, UStaticMeshComponent* meshComponent);
	void setProceduralMeshComponentParameters(AldrawVRpartActor* actor, UProceduralMeshComponent* meshComponent, bool createCollision);

	string generateMeshName(string partName, int absoluteColour);
	FString generateStaticMeshPathName(string assetName, int absoluteColour);

	#define AXIS_FIRST (1)
	#define AXIS_NUMBER_OF_TYPES (3)
	double getVecValue(vec* v, int index);
	void setVecValue(vec* v, int index, double value);
};


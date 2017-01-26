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
* File Name: ldrawVRpartActor.h
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#pragma once

#include "Engine/StaticMeshActor.h"
#include "../VRtemplate/PickupActorInterface2.h"
#include "ProceduralMeshComponent.h"
#include "ldrawVRoperations.h"
#include "ldrawVRpartActor.generated.h"

//#define LDRAW_VR_IO_OPTIMISATION

//#define LDRAW_VR_OBLITERATE_SUBMODELS_UPON_DROP	//enables physics for each part of submodel once dropped

#define LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS
#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS
	#define LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS_DISABLE_WHEN_LOCKED_TO_GRID	//this is used so that the user can see/change the colour of the selected part (alternatively LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS should be disabled completely, but doing so prevents the user from knowing which actor is selected: ie which actor is currently being locked/unlocked from the grid)
#endif

//these preprocessor defs should be moved to an ldrawVRglobalDefs.h file
#define LDRAW_VR_SIMULATE_PHYSICS	//enable physics for sandbox parts picking
#ifdef LDRAW_VR_SIMULATE_PHYSICS
	#define LDRAW_VR_SIMULATE_PHYSICS_LDRAW_VR_PART_ACTOR_COMPONENT_TAGNAME "ldrawpart"
	#define LDRAW_VR_SIMULATE_PHYSICS_HIGHLIGHT_OVERLAPPING	//doesn't work for some reason (possibly because of a large/inappropriate number of getPickupImplementationActiveActorBeginOverlap/getPickupImplementationActiveActorEndOverlap events)
	//#define LDRAW_VR_SIMULATE_PHYSICS_ADVANCED	//assume that physics is disabled by for new and connected parts at this stage of development
	#define LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT	//designed to emulate blueprint pickupCube pickupactorinterface pickup/drop event definitions
	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT
		//#define LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT	//not required or tested (current implementation already sets physics of root component if that is the component upon which a static mesh is attached)
	#endif
	#define LDRAW_VR_SIMULATE_PHYSICS_NEVER_APPLY_PHYSICS_TO_BASEPLATES	//this is required so that baseplates are always locked to grid
#endif


//#define LDRAW_VR_MODIFY_LD_TRANSFORMATION_MATRIX_IN_SYNC	//doesn't support rewriting of existing models (because their parts have not moved)



UCLASS()
class AldrawVRpartActor : public AStaticMeshActor, public IPickupActorInterface2
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AldrawVRpartActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER	//this preprocessor definition test is not allowed by unreal build tool preprocessor
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ldrawVR")
	void Pickup(USceneComponent* AttachTo);
	virtual void Pickup_Implementation(USceneComponent* AttachTo) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ldrawVR")
	void Drop();
	virtual void Drop_Implementation() override;
	//#ifdef LDRAW_VR_SIMULATE_PHYSICS	//this preprocessor definition test is not allowed by unreal build tool preprocessor
	void setPhysicsOfRootComponent(bool simulatePhysics);
	void setPhysicsOfMeshComponents(bool simulatePhysics);
	void setPhysicsOfStaticMeshComponent(UStaticMeshComponent* meshComponent, bool simulatePhysics);
	void setPhysicsOfProceduralMeshComponent(UProceduralMeshComponent* meshComponent, bool simulatePhysics);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void beginOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "ldrawVR")
	void endOverlap(AActor* MyOverlappedActor, AActor* OtherActor);
	bool areOverlappingActorsRemaining();
	bool isCurrentlyOverlapping;
	bool neverEnablePhysics;	//this is used for baseplates
	//#endif
	bool isCurrentlyHeld;
	bool isLockedToGrid;
	//#endif

	void highlightActor(bool highlight);
		void highlightActorStaticMeshComponents(AldrawVRpartActor* actor, bool highlight);
		void highlightActorDynamicMeshComponents(AldrawVRpartActor* actor, bool highlight);

	void pickedUpByUser();
		void addToPartActorAndReferenceList();
			void addToPartActorList(AldrawVRpartActor* actor);
			LDreference* addToPartReferenceList(LDreference* partReferenceNew);
	void deletedByUser();
		void deleteFromPartActorAndReferenceList();
			void deleteFromPartActorList(AldrawVRpartActor* actor);
			void deleteFromPartReferenceList(LDreference* partReferenceNew);
	void movedByUser();

	LDreference* partReference;

	bool spawnedPartFromPartSelectionWindow;
	bool spawnedPartFromPartSelectionWindowHasBeenPickedUpByUser;

private:

	ldrawVRoperations ldrawVRops;
	SHAREDvarsClass SHAREDvars;
};


void setSnapPosition(AldrawVRpartActor* actor);

void initialiseSelectedActor();
void setSelectedActor(AldrawVRpartActor* actor);
AldrawVRpartActor* getSelectedActor();
void initialiseSceneInitialReference();
void setSceneInitialReference(LDreference* sceneInitialReferenceNew);
LDreference* getSceneInitialReference();
void setScenePartActorList(vector<AldrawVRpartActor*>* scenePartActorListNew);
vector<AldrawVRpartActor*>* getScenePartActorList();
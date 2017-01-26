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
* File Name: ldrawVRpartActor.cpp
* Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
* Project: LDrawVR
* Project Version: 1a211a 26-January-2017
*
*******************************************************************************/

#include "ldrawVRv0.h"
#include "ldrawVRpartActor.h"
#include "ProceduralMeshComponent.h"


static AldrawVRpartActor* selectedActor;
LDreference* sceneInitialReference;
vector<AldrawVRpartActor*>* scenePartActorList;

// Sets default values
AldrawVRpartActor::AldrawVRpartActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	OnActorBeginOverlap.AddDynamic(this, &AldrawVRpartActor::beginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AldrawVRpartActor::endOverlap);
	isCurrentlyOverlapping = false;
	neverEnablePhysics = false;
	#endif
	isCurrentlyHeld = false;
	isLockedToGrid = false;
	#endif

	partReference = NULL;

	spawnedPartFromPartSelectionWindow = false;
	spawnedPartFromPartSelectionWindowHasBeenPickedUpByUser = false;

}

// Called when the game starts or when spawned
void AldrawVRpartActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AldrawVRpartActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


//#ifdef LDRAW_VR_TEMPLATE_MOTION_CONTROLLER	//this preprocessor definition test is not allowed by unreal build tool preprocessor
void AldrawVRpartActor::Pickup_Implementation(USceneComponent* AttachTo)
{
	#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS
	if(getSelectedActor() != NULL)
	{
		getSelectedActor()->highlightActor(false);	//change material to original
	}
	#endif

	//ldrawVRops.printMessage("Pickup_Implementation");
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	#ifdef LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT
	setPhysicsOfRootComponent(false);
	#else
	setPhysicsOfMeshComponents(false);
	#endif
	#endif
	isCurrentlyHeld = true;

	/*
	//https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/GameFramework/AActor/K2_AttachToComponent/index.html
	if(!K2_AttachToComponent(AttachTo, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false))
	{
		ldrawVRops.printMessage("Pickup_Implementation error: cannot attach to component");
	}
	*/
	//alternate implementation (same)
	USceneComponent* sceneComponent = K2_GetRootComponent();
	if(!sceneComponent->K2_AttachToComponent(AttachTo, NAME_None, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false))
	{
		ldrawVRops.printMessage("Pickup_Implementation error: cannot attach to component");
	}


	#ifndef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	if(areOverlappingActorsRemaining())	//if(isCurrentlyOverlapping)
	{
		isCurrentlyOverlapping = true;
		#ifdef LDRAW_VR_SIMULATE_PHYSICS_HIGHLIGHT_OVERLAPPING
		highlightActor(true);	//change material to fancy emmisive (eg red glow)
		#endif
	}
	else
	{
		isCurrentlyOverlapping = false;
	}
	#endif
	#endif

	pickedUpByUser();
}
void AldrawVRpartActor::Drop_Implementation()
{
	//ldrawVRops.printMessage("Drop_Implementation");
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	#ifdef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
	#ifdef LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT
	setPhysicsOfRootComponent(true);
	#else
	setPhysicsOfMeshComponents(true);
	#endif
	#else
	if(!isCurrentlyOverlapping)
	{
		#ifndef LDRAW_VR_OBLITERATE_SUBMODELS_UPON_DROP
		string partName = "";
		if(!ldrawVRops.submodelIsPart(partReference->name, &partName))
		{//submodel detected
			//setPhysicsOfRootComponent(true);	//this doesn't work - actor cannot be picked up once dropped (due to the limitation fixed by LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_SIMULATE_PHYSICS_APPLY_BUG_WORKAROUND_ATTACH_MESH_TO_ROOT_COMPONENT for singleMeshPerActor actors)
		}
		else
		{
		#endif
			#ifdef LDRAW_VR_SIMULATE_PHYSICS_ROOT_COMPONENT
			setPhysicsOfRootComponent(true);
			#else
			setPhysicsOfMeshComponents(true);
			#endif
		#ifndef LDRAW_VR_OBLITERATE_SUBMODELS_UPON_DROP
		}
		#endif
	}
	#endif
	#endif
	isCurrentlyHeld = false;
	K2_DetachFromActor(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
	/*
	{
		ldrawVRops.printMessage("Drop_Implementation error: cannot deattach from component");
	}
	*/

	#ifndef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	if(isCurrentlyOverlapping)
	{
		#ifdef LDRAW_VR_SIMULATE_PHYSICS_HIGHLIGHT_OVERLAPPING
		highlightActor(false);	//change material to default
		#endif
	#endif
	#endif
		setSnapPosition(this);	//lock actor to grid
	#ifndef LDRAW_VR_SIMULATE_PHYSICS_ADVANCED
	#ifdef LDRAW_VR_SIMULATE_PHYSICS
	}
	else if(neverEnablePhysics)
	{
		setSnapPosition(this);	//lock actor to grid
	}
	#endif
	#endif
	setSelectedActor(this);
}

//#ifdef LDRAW_VR_SIMULATE_PHYSICS	//this preprocessor definition test is not allowed by unreal build tool preprocessor

void AldrawVRpartActor::setPhysicsOfRootComponent(bool simulatePhysics)	//not used
{
	if(simulatePhysics && !neverEnablePhysics)
	{
		GetRootPrimitiveComponent()->SetSimulatePhysics(true);
		GetRootPrimitiveComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		GetRootPrimitiveComponent()->SetSimulatePhysics(false);
		GetRootPrimitiveComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		//GetRootPrimitiveComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AldrawVRpartActor::setPhysicsOfMeshComponents(bool simulatePhysics)
{
	TArray<UStaticMeshComponent*> meshComponents;
	GetComponents<UStaticMeshComponent>(meshComponents);
	//ldrawVRops.printMessage("setPhysicsOfMeshComponents: meshComponents.Num() = " + convertIntToString(meshComponents.Num()));
	//UStaticMeshComponent* meshComponentOld = NULL;
	for(int i=0; i<meshComponents.Num(); i++)
	{
		UStaticMeshComponent* meshComponent = meshComponents[i];
		if(meshComponent->ComponentHasTag(ldrawVRops.convertStringToFName(LDRAW_VR_SIMULATE_PHYSICS_LDRAW_VR_PART_ACTOR_COMPONENT_TAGNAME)))
		{
			setPhysicsOfStaticMeshComponent(meshComponent, simulatePhysics);
		}
	}

	TArray<UProceduralMeshComponent*> meshComponents2;
	GetComponents<UProceduralMeshComponent>(meshComponents2);
	for(int i=0; i<meshComponents2.Num(); i++)
	{
		UProceduralMeshComponent* meshComponent = meshComponents2[i];
		if(meshComponent->ComponentHasTag(ldrawVRops.convertStringToFName(LDRAW_VR_SIMULATE_PHYSICS_LDRAW_VR_PART_ACTOR_COMPONENT_TAGNAME)))
		{
			setPhysicsOfProceduralMeshComponent(meshComponent, simulatePhysics);
		}
	}
}

void AldrawVRpartActor::setPhysicsOfStaticMeshComponent(UStaticMeshComponent* meshComponent, bool simulatePhysics)
{
	//ldrawVRops.printMessage("setPhysicsOfStaticMeshComponent: SetSimulatePhysics, simulatePhysics = " + convertIntToString(int(simulatePhysics)));

	if(simulatePhysics && !neverEnablePhysics)
	{
		meshComponent->SetSimulatePhysics(true);
		meshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

		/*
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;

		//doesnt do anything
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;
		meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);

		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Block);
		//meshComponent->WakeRigidBody();

		//meshComponent->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);
		*/
	}
	else
	{
		meshComponent->SetSimulatePhysics(false);
		meshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

		/*
		meshComponent->SetNotifyRigidBodyCollision(false);
		meshComponent->bGenerateOverlapEvents = true;
			//meshComponent->bGenerateOverlapEvents = true;

		//doesnt do anything
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;
		meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);

		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	//NoCollision	//doesnt do anything
		meshComponent->bGenerateOverlapEvents = true;	//doesnt do anything
		meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);

		//meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);
		//meshComponent->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);
		*/
	}
}

void AldrawVRpartActor::setPhysicsOfProceduralMeshComponent(UProceduralMeshComponent* meshComponent, bool simulatePhysics)
{
	if(simulatePhysics && !neverEnablePhysics)
	{
		meshComponent->SetSimulatePhysics(true);
		meshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		/*
		//doesnt do anything
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;
		meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);

		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Block);
		//meshComponent->WakeRigidBody();

		//meshComponent->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);
		*/
	}
	else
	{
		meshComponent->SetSimulatePhysics(false);
		meshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		/*
		//meshComponent->bGenerateOverlapEvents = true;
		//doesnt do anything
		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		meshComponent->SetNotifyRigidBodyCollision(true);
		meshComponent->bGenerateOverlapEvents = true;
		meshComponent->BodyInstance.SetObjectType(ECC_PhysicsBody);

		meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	//NoCollision	//doesnt do anything
		meshComponent->bGenerateOverlapEvents = true;	//doesnt do anything
		meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);

		//meshComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);
		//meshComponent->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Block);
		//meshComponent->BodyInstance.SetResponseToChannel(ECC_WorldDynamic, ECR_Block);
		*/
	}
}


void AldrawVRpartActor::beginOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	AldrawVRpartActor* thisActor = dynamic_cast<AldrawVRpartActor*>(MyOverlappedActor);
	if(thisActor->isCurrentlyHeld)
	{
		//ldrawVRops.printMessage("AldrawVRpartActor::beginOverlap: isCurrentlyHeld");

		thisActor->isCurrentlyOverlapping = true;
		#ifdef LDRAW_VR_SIMULATE_PHYSICS_HIGHLIGHT_OVERLAPPING
		thisActor->highlightActor(true);	//change material to fancy emmisive (eg red glow)
		#endif
	}
}
void AldrawVRpartActor::endOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	AldrawVRpartActor* thisActor = dynamic_cast<AldrawVRpartActor*>(MyOverlappedActor);
	if(thisActor->isCurrentlyHeld)
	{
		//ldrawVRops.printMessage("AldrawVRpartActor::endOverlap: isCurrentlyHeld");
		//if(thisActor->isCurrentlyOverlapping)
		//{
			bool overlappingActorsRemaining = thisActor->areOverlappingActorsRemaining();
			if(overlappingActorsRemaining)
			{
				//ldrawVRops.printMessage("AldrawVRpartActor::endOverlap: isCurrentlyHeld && overlappingActorsRemaining [eg a 3+ actor overlap occurred], overlappingActors.Num() = : " + convertIntToString(overlappingActors.Num()));
			}
			else
			{
				if(thisActor->isCurrentlyOverlapping)
				{
					//ldrawVRops.printMessage("AldrawVRpartActor::endOverlap: isCurrentlyHeld && !overlappingActorsRemaining");

					thisActor->isCurrentlyOverlapping = false;
					#ifdef LDRAW_VR_SIMULATE_PHYSICS_HIGHLIGHT_OVERLAPPING
					thisActor->highlightActor(false);	//change material to original (based on )
					#endif
				}
				else
				{
					//ldrawVRops.printMessage("AldrawVRpartActor::endOverlap: error: isCurrentlyHeld && !overlappingActorsRemaining && !(thisActor->isCurrentlyOverlapping)");
				}
			}
		//}
	}
}

bool AldrawVRpartActor::areOverlappingActorsRemaining()
{
	bool overlappingActorsRemaining = false;
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	//safer version (does not assume that OtherActor has already been removed from this->overlappingActors)
	for(int i = 0; i < overlappingActors.Num(); i++)
	{
		string tagName = LDRAW_VR_TEMPLATE_MOTION_CONTROLLER_BP_MotionController_ACTOR_TAGNAME;
		if(!overlappingActors[i]->ActorHasTag(ldrawVRops.convertStringToFName(tagName)))
		{
			//if(overlappingActors[i] != OtherActor)	//redundant?
			//{
			overlappingActorsRemaining = true;
			//}
		}
	}
	return overlappingActorsRemaining;
}

//#endif
//#endif



void AldrawVRpartActor::highlightActor(bool highlight)
{
	highlightActorStaticMeshComponents(this, highlight);
	highlightActorDynamicMeshComponents(this, highlight);
}

void AldrawVRpartActor::highlightActorStaticMeshComponents(AldrawVRpartActor* actor, bool highlight)
{
	TArray<UStaticMeshComponent*> meshComponents;
	actor->GetComponents<UStaticMeshComponent>(meshComponents);
	for(int i=0; i<meshComponents.Num(); i++)
	{
		UStaticMeshComponent* meshComponent = meshComponents[i];
		UStaticMesh* meshReference = meshComponent->GetStaticMesh();
		if(meshReference != NULL)
		{
			int numberOfElements = meshReference->GetNumSections(0);
			for(int x = 0; x < numberOfElements; x++)
			{
				UMaterialInterface* material = meshComponent->GetMaterial(x);
				string currentMaterialName = ldrawVRops.convertFStringToString(material->GetName());
				string currentMaterialLocation = ldrawVRops.convertFStringToString(material->GetPathName());
				//int absoluteColour = ldrawVRimportMod.getColourFromMaterialName(currentMaterialName);
				//ldrawVRops.printMessage("ldrawVRoperations::highlightActorStaticMeshComponents{}: currentMaterialName = " + currentMaterialName);
				bool inheritedMaterial = false;
				if(currentMaterialLocation.find(UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					inheritedMaterial = true;
				}
				UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(currentMaterialName, inheritedMaterial, highlight);
				meshComponent->SetMaterial(x, replacementMaterial);
			}
		}
	}
}

void AldrawVRpartActor::highlightActorDynamicMeshComponents(AldrawVRpartActor* actor, bool highlight)
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
				//int absoluteColour = ldrawVRimportMod.getColourFromMaterialName(currentMaterialName);
				//ldrawVRops.printMessage("ldrawVRoperations::highlightActorDynamicMeshComponents{}: currentMaterialName = " + currentMaterialName);
				bool inheritedMaterial = false;
				if(currentMaterialLocation.find(UNREAL_MATERIAL_INHERITED_ASSET_FOLDER_NAME) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					inheritedMaterial = true;
				}
				UMaterialInterface* replacementMaterial = ldrawVRops.getReplacementMaterial(currentMaterialName, inheritedMaterial, highlight);
				meshComponent->SetMaterial(x, replacementMaterial);
			}
		}
	}
}






void AldrawVRpartActor::pickedUpByUser()
{
	if(spawnedPartFromPartSelectionWindow)
	{
		if(!spawnedPartFromPartSelectionWindowHasBeenPickedUpByUser)
		{
			spawnedPartFromPartSelectionWindowHasBeenPickedUpByUser = true;
			addToPartActorAndReferenceList();
			#ifdef LDRAW_VR_IO_OPTIMISATION
			partReference->partAdditional = true;
			#endif
			movedByUser();
		}
	}
}

void AldrawVRpartActor::addToPartActorAndReferenceList()
{
	addToPartActorList(this);
	LDreference* newReferenceInSceneFile = addToPartReferenceList(this->partReference);
	this->partReference = newReferenceInSceneFile;
}

void AldrawVRpartActor::addToPartActorList(AldrawVRpartActor* actor)
{
	scenePartActorList->push_back(actor);
}

LDreference* AldrawVRpartActor::addToPartReferenceList(LDreference* partReferenceNew)
{
	LDreference* newReferenceInSceneFile = sceneInitialReference;
	while(newReferenceInSceneFile->next != NULL)
	{
		newReferenceInSceneFile = newReferenceInSceneFile->next;
	}
	newReferenceInSceneFile->type = partReferenceNew->type;
	newReferenceInSceneFile->name = partReferenceNew->name;
	newReferenceInSceneFile->absoluteColour = partReferenceNew->absoluteColour;
	newReferenceInSceneFile->absolutePosition = partReferenceNew->absolutePosition;
	newReferenceInSceneFile->absoluteDeformationMatrix = partReferenceNew->absoluteDeformationMatrix;
	newReferenceInSceneFile->next = new LDreference();
	return newReferenceInSceneFile;
}



void AldrawVRpartActor::deletedByUser()
{
	deleteFromPartActorAndReferenceList();
}

void AldrawVRpartActor::deleteFromPartActorAndReferenceList()
{
	#ifdef LDRAW_VR_IO_OPTIMISATION
	this->partDeleted = true;
	#else
	deleteFromPartReferenceList(this->partReference);
	#endif
	deleteFromPartActorList(this);
}


//this function is called when a->partReference->partDeleted is detected by the ldrawVRGameControlActor kernel
void AldrawVRpartActor::deleteFromPartActorList(AldrawVRpartActor* actor)
{
	bool foundActor = false;
	for(std::vector<AldrawVRpartActor*>::iterator iter = scenePartActorList->begin(); iter != scenePartActorList->end(); )
	{
		AldrawVRpartActor* a = *iter;
		if(a == actor)
		{
			iter = scenePartActorList->erase(iter);
			foundActor = true;
		}
		else
		{
			iter++;
		}
	}
	if(!foundActor)
	{
		ldrawVRops.printMessage("AldrawVRGameControlActor::deletePartActorBP{} error: !foundActor");
	}

	actor->Destroy();	//CHECKTHIS
}

void AldrawVRpartActor::deleteFromPartReferenceList(LDreference* partReferenceDelete)
{
	LDreference* currentReference = sceneInitialReference;
	LDreference* previousReference = sceneInitialReference;
	while(currentReference->next != NULL)
	{
		if(currentReference == partReferenceDelete)
		{
			previousReference->next = currentReference->next;
			ldrawVRops.printMessage("reconcileReferenceListForSave{} removing deleted reference from list: " + currentReference->name);
		}
		else
		{
			previousReference = currentReference;
		}

		//ldrawVRops.printMessage("reconcileReferenceListForSave{} current reference name " + currentReference->name);
		//dont parse down level (assume that submodels have not been changed by user, only references to submodels have been changed)

		currentReference = currentReference->next;
	}
}


void AldrawVRpartActor::movedByUser()
{
	#ifdef LDRAW_VR_IO_OPTIMISATION
	partReference->partModified = true;
	#endif
	#ifdef LDRAW_VR_MODIFY_LD_TRANSFORMATION_MATRIX_IN_SYNC
	convertTransformationMatrixCoordinateSystemUnrealToLD(&(partReference->absolutePosition), &(partReference->absoluteDeformationMatrix), GetActorLocation(), GetActorRotation());
	#endif
}









void setSnapPosition(AldrawVRpartActor* actor)
{
	FVector worldPositionUnreal = actor->GetActorLocation();
	FRotator worldRotationUnreal = actor->GetActorRotation();

	FVector worldPositionUnrealSnapped;
	FRotator worldRotationUnrealSnapped;
	ldrawVRoperations().calculateSnapPosition(&worldPositionUnreal, &worldRotationUnreal, &worldPositionUnrealSnapped, &worldRotationUnrealSnapped);

	actor->SetActorLocation(worldPositionUnrealSnapped);
	actor->SetActorRotation(worldRotationUnrealSnapped);
}




void initialiseSelectedActor()
{
	selectedActor = NULL;
}
void setSelectedActor(AldrawVRpartActor* actor)
{
	#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS
	if(selectedActor != NULL)
	{
		selectedActor->highlightActor(false);	//change material to original
	}
	#endif
	selectedActor = actor;
	#ifdef LDRAW_VR_HIGHLIGHT_SELECTED_ACTORS
	selectedActor->highlightActor(true);	//change material to fancy emmisive (eg red glow)
	#endif
}
AldrawVRpartActor* getSelectedActor()
{
	return selectedActor;
}

void initialiseSceneInitialReference()
{
	sceneInitialReference = NULL;
}
void setSceneInitialReference(LDreference* sceneInitialReferenceNew)
{
	sceneInitialReference = sceneInitialReferenceNew;
}
LDreference* getSceneInitialReference()
{
	return sceneInitialReference;
}

void setScenePartActorList(vector<AldrawVRpartActor*>* scenePartActorListNew)
{
	scenePartActorList = scenePartActorListNew;
}
vector<AldrawVRpartActor*>* getScenePartActorList()
{
	return scenePartActorList;
}






// Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)

#pragma once

#include "PickupActorInterface2.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UPickupActorInterface2 : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 *
 */
class IPickupActorInterface2
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//classes using this interface may implement the function (if classes using this interface must implement the function, use: BlueprintNativeEvent instead of BlueprintImplementableEvent)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ldrawVR")
		void Pickup(USceneComponent* AttachTo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ldrawVR")
		void Drop();

};

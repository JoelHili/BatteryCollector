// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
	

public: 
	
	// Sets default values for this actor's properties
	ABatteryPickup();

	//Override collected function
	void WasCollected_Implementation() override;
	float GetPower();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = true))
	float BatteryPower;
};

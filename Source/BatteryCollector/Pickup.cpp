// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //Disabled

	//Create the static mech component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMech"));
	//RootComponent = PickupMesh;

	bIsActive = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickup::IsActive() {
	return this->bIsActive;
};

void APickup::SetActive(bool NewPickupState) {
	bIsActive = NewPickupState;
};

void APickup::WasCollected_Implementation() {
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You Have Collected %s"), *PickupDebugString);
}
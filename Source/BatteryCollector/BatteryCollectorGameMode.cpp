// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet\GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;

	DecayRate = 0.01f;
}

void ABatteryCollectorGameMode::BeginPlay() {
	Super::BeginPlay();

	//Find Spawn Volume Actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);

		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	//Set score to beat
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MyCharacter)
	{
		PowerToWin = (MyCharacter->GetInitialPower())*1.25f;
	}

	//Create the Widget and add it to the viewport
	if (HUDWidgetClass != NULL) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (MyCharacter) {
		if (MyCharacter->GetCharacterPower() > PowerToWin) {
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (MyCharacter->GetCharacterPower() > 0) // Check if Character Power is > 0
		{
			MyCharacter->UpdatePower(-DeltaTime * DecayRate *(MyCharacter->GetInitialPower())); // This decreaces the Character Power based on the initial power * the decay rate per second
		}
		else
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const {
	return PowerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const {
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState) {
	CurrentState = NewState;

	HandleNewState(CurrentState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState NewState) {
	switch (NewState)
	{
	case EBatteryPlayState::EPlaying:
	{
		//spawn volumes active
		for (auto SpawnVol : SpawnVolumeActors)
		{
			SpawnVol->SetSpawningActive(true);
		}

	}
		break;
	case EBatteryPlayState::EWon:
	{
		//spawn volumes inactive
		for (auto SpawnVol : SpawnVolumeActors)
		{
			SpawnVol->SetSpawningActive(false);
		}
	}
		break;
	case EBatteryPlayState::EGameOver:
		//spawn volumes inactive
	{
		for (auto SpawnVol : SpawnVolumeActors)
		{
			SpawnVol->SetSpawningActive(false);
		}

		//block player input
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}

		//ragdoll character

		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

		if (MyCharacter)
		{
			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
	}
		break;
	default:
		break;
	case EBatteryPlayState::EUnknown:
		break;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "BatteryCollector.h"

//Set Default value

ABatteryPickup::ABatteryPickup() {
	GetMesh()->SetSimulatePhysics(true);
	BatteryPower = 150.0f;
}

void ABatteryPickup::WasCollected_Implementation() {
	//Call base pickup Collect-Behaviour
	Super::WasCollected_Implementation(),

	//Destroy the battery
	Destroy();
}

float ABatteryPickup::GetPower() {
	return BatteryPower;
}
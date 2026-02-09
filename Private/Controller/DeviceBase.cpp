// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/DeviceBase.h"

//================== Protected ==================//
// Sets default values
ADeviceBase::ADeviceBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADeviceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeviceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//================== Public ==================//



void ADeviceBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADeviceBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ADeviceBase::BeginFocus()
{
}

void ADeviceBase::EndFocus()
{
}

void ADeviceBase::BeginInteract()
{
}

void ADeviceBase::EndInteract()
{
}

void ADeviceBase::Interact()
{
}


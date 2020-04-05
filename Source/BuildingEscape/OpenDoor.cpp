// Copyright Lorenzo Iannaccaro 2020

#include "Engine/World.h"
#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw;

	if(!pressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("The actor %s has the OpenDoor component on it, but has no pressure plate set."), *GetOwner()->GetName());
	}

	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(pressurePlate && pressurePlate->IsOverlappingActor(actorThatOpens)){
		openDoor(DeltaTime);
		doorLastOpened = GetWorld()->GetTimeSeconds();
	}else{

		float actualTime = GetWorld()->GetTimeSeconds();
		if (actualTime - doorLastOpened >= doorClosingDelay)
		{
			closeDoor(DeltaTime);
		}

	}

}

void UOpenDoor::openDoor(float delta)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorTransform().ToString());

	// Relative rotation
	currentYaw = FMath::FInterpTo(currentYaw, targetYaw, delta, doorOpeningSpeed);
	FRotator doorRotator = GetOwner()->GetActorRotation();
	doorRotator.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotator);
}

void UOpenDoor::closeDoor(float delta)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorTransform().ToString());

	// Relative rotation
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, delta, doorClosingSpeed);
	FRotator doorRotator = GetOwner()->GetActorRotation();
	doorRotator.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotator);
}


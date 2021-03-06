// Copyright Lorenzo Iannaccaro 2020

#include "OpenDoor.h"

#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT	// macro to mark 'out parameters': it does nothing

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorTransform().ToString());

	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	checkAudioComponent();

	checkPressurePlate();

	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	targetYaw += initialYaw;

	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	controlPressurePlate(DeltaTime);

}

void UOpenDoor::openDoor(float delta)
{
	if(doorClosed)
	{
		playAudio();
		doorOpen = true;
		doorClosed = false;
	}

	// Relative rotation
	currentYaw = FMath::FInterpTo(currentYaw, targetYaw, delta, doorOpeningSpeed);
	FRotator doorRotator = GetOwner()->GetActorRotation();
	doorRotator.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotator);
}

void UOpenDoor::closeDoor(float delta)
{
	if(doorOpen)
	{
		playAudio();
		doorClosed = true;
		doorOpen = false;
	}

	// Relative rotation
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, delta, doorClosingSpeed);
	FRotator doorRotator = GetOwner()->GetActorRotation();
	doorRotator.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotator);
}

float UOpenDoor::getTotalMassOverlapping() const
{
	float totalMass = 0.f;

	// get actors that overlap with the triggervolume
	TArray<AActor*> overlappingActors;
	pressurePlate->GetOverlappingActors(OUT overlappingActors);

	// sum their masses
	for(AActor* actor: overlappingActors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}

void UOpenDoor::playAudio()
{
	audioComponent->Play();
}

void UOpenDoor::controlPressurePlate(float delta)
{
	if(pressurePlate->IsOverlappingActor(actorThatOpens) ||  getTotalMassOverlapping() > massToOpenDoor)
	{
		openDoor(delta);
		doorLastOpenedTime = GetWorld()->GetTimeSeconds();
	}else
	{
		float actualTime = GetWorld()->GetTimeSeconds();
		if (actualTime - doorLastOpenedTime >= doorClosingDelay)
		{
			closeDoor(delta);
		}
	}
}

void UOpenDoor::checkAudioComponent() const
{
	if(!audioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No audioComponent found for %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::checkPressurePlate() const
{
	if(!pressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The actor %s has the OpenDoor component on it, but has no pressure plate set."), *GetOwner()->GetName());
	}
}

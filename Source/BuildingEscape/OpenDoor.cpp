// Copyright Lorenzo Iannaccaro 2020


#include "OpenDoor.h"
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
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorTransform().ToString());

	/* float currentYaw = GetOwner()->GetActorRotation().Yaw;
	FRotator openDoor(0.f, 0.f, 0.f);	// initialization
	// openDoor.Yaw = FMath::Lerp(currentYaw, targetYaw 0.02f);
	openDoor.Yaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, 1.f);
	GetOwner()->SetActorRotation(openDoor); */

	// Relative rotation
	currentYaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, 1.f);
	FRotator doorRotator = GetOwner()->GetActorRotation();
	doorRotator.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotator);

}


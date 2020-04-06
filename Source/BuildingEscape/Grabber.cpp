// Copyright Lorenzo Iannaccaro 2020

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT	// macro to mark 'out parameters': it does nothing

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Grabber initialized."));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// get player viewpoint
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	/* UE_LOG(LogTemp, Warning, TEXT("Player location: %s, Player rotation: %s."), 
									*playerViewPointLocation.ToString(), *playerViewPointRotation.ToString()); */

	// draw debug line
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	DrawDebugLine(GetWorld(), playerViewPointLocation, lineTraceEnd, FColor(0,255,0), false, 0, 0, 5.f);

	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
	OUT hit, 
	playerViewPointLocation, 
	lineTraceEnd, 
	FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
	traceParams);

	if(hit.GetActor()){
		UE_LOG(LogTemp, Warning, TEXT("Hit object whose name is: %s"), *hit.GetActor()->GetName());
	}
}


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

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	findPhysicsHandle();
	setupInputController();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle has an object attached to it
	if(physicsHandle->GrabbedComponent)
	{
		// get linetraceend
		FVector lineTraceEnd = getPlayerReach();

		// move the object
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

FHitResult UGrabber::getFirstPhysicsInRange()
{
	FVector lineTraceEnd = getPlayerReach();

	// linetrace to physics bodies in direct sight 
	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit, 
		playerViewPointLocation, 
		lineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		traceParams
	);

	// log if linetrace hit something
	AActor* actorHit = hit.GetActor();
	if(actorHit){
		UE_LOG(LogTemp, Warning, TEXT("Hit object whose name is: %s"), *actorHit->GetName());
	}

	return hit;
}

void UGrabber::grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button pressed."));
	FHitResult hitResult = getFirstPhysicsInRange();

	// get linetraceend
	FVector lineTraceEnd = getPlayerReach();
	
	// if hit something with physics, grab it
	if(hitResult.GetActor())
	{
		UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
		physicsHandle->GrabComponentAtLocation(componentToGrab, NAME_None, lineTraceEnd);
	}

}

void UGrabber::release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab button released."));

	// eventually release a grabbed object
	if(physicsHandle->GetGrabbedComponent())
	{
		physicsHandle->ReleaseComponent();
	}

}

void UGrabber::findPhysicsHandle()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle found for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::setupInputController()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::release);
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("No input component found for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::updatePlayerViewpoint()
{
	// get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
}

FVector UGrabber::getPlayerReach()
{
	// get linetraceend after updating the actual player viewport
	updatePlayerViewpoint();
	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}
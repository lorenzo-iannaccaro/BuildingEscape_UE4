// Copyright Lorenzo Iannaccaro 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float reach = 250.f;
	UPROPERTY()
	UPhysicsHandleComponent* physicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* inputComponent = nullptr;
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	void grab();
	void release();
	void findPhysicsHandle();
	void setupInputController();
	void updatePlayerViewpoint();
	FVector getPlayerReach();
	FHitResult getFirstPhysicsInRange();
		
};

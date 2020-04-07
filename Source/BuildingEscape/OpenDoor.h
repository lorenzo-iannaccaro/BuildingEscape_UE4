// Copyright Lorenzo Iannaccaro 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void openDoor(float delta);
	void closeDoor(float delta);
	float getTotalMassOverlapping() const;

private:
	float initialYaw;
	float currentYaw;
	float doorLastOpenedTime = 0.f;;
	float doorClosingDelay = .3f;

	UPROPERTY(EditAnywhere)
	float massToOpenDoor = 2.5f;

	UPROPERTY(EditAnywhere)
	float doorOpeningSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float doorClosingSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float targetYaw = -90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* actorThatOpens = nullptr;
		
};

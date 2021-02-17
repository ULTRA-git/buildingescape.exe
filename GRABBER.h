// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GRABBER.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGRABBER : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGRABBER();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:

	float Reach = 100.f;

	UPhysicsHandleComponent * PhysicsHandle = nullptr; 

	UInputComponent * InputComponent = nullptr;
	
	void GRAB();

	void RELEASE();

	void FindPhysicsHandle();

	void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach() const;
	
};

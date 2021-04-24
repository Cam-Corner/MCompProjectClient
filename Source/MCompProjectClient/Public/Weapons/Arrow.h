// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class MCOMPPROJECTCLIENT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		float _Damage = 50;

	UPROPERTY(EditAnywhere)
		float _ArrowSpeed = 800.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** The arrow mesh */
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* _ArrowMesh;
};

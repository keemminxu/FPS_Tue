// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

// 지가 향하는 방향으로 날아가고 싶다.

UCLASS()
class FPS_TUE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category="Collision")
	class USphereComponent* collision;

	UPROPERTY(VisibleAnywhere, Category="BodyMesh")
	class UStaticMeshComponent* bodyMesh;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* movement;

	UPROPERTY(EditDefaultsOnly, Category="Speed")
	float speed = 5000;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFire.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_TUE_API UPlayerFire : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

public:
	UPROPERTY()
	class AFPSPlayer* me;

	UPROPERTY(EditDefaultsOnly, Category="BulletFactory")
	TSubclassOf<class ABullet> bulletFactory;

	void Fire();

	UPROPERTY()
	class UArrowComponent* firePosition;
	
	UPROPERTY(EditDefaultsOnly, Category = "BulletEffect")
	class UParticleSystem* bulletEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FireDistance")
	float fireDistance = 10000;
		
	UPROPERTY(EditDefaultsOnly, Category = "BulletPower")
	float bulletPower = 10000;
};

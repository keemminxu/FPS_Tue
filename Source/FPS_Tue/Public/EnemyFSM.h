// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_TUE_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UPROPERTY(EditAnywhere, Category="FSM", BlueprintReadWrite)
	EEnemyState m_state = EEnemyState::Idle;

	UPROPERTY(EditAnywhere, Category="FSM")
	float idleDelayTime = 2;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float attackDelayTime = 2;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float damageDelayTime = 2;

	UPROPERTY()
	float currentTime = 0;

	UPROPERTY(EditAnywhere, Category="FSM")
	class AFPSPlayer* target;

	UPROPERTY(VisibleAnywhere, Category="Enemy")
	class AEnemy* enemy;

	UPROPERTY(EditAnywhere, Category="FSM")
	float moveSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float attackRange = 200;

	UPROPERTY()
	FVector knockbackPos;

	//hp
	UPROPERTY(EditAnywhere, Category = "FSM", BlueprintReadWrite)
	int hp = 3;

	UPROPERTY(EditAnywhere, Category = "KnockBack")
	float knockback = 300;

	UPROPERTY(EditAnywhere, Category = "KnockBack")
	float knockbackSpeed = 20;

	UPROPERTY(EditAnywhere, Category = "KnockBack")
	float knockbackRange = 20;

	// 자동 길찾기 하기위한 Enemy 의 AIController 를 기억
	UPROPERTY()
	class AAIController* ai;

public:
	// 피격을 받았을 때 처리할 함수

	void OnDamageProcess(FVector shootDirection);

	/*UPROPERTY()
	float dtdt;*/

	FVector dir;
		
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();
};

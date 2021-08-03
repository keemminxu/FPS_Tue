// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "FPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>
#include "Enemy.h"
#include <DrawDebugHelpers.h>
#include "FPS_Tue.h"
#include <TimerManager.h>
#include <AIController.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	enemy=Cast<AEnemy>(GetOwner());

	target = Cast<AFPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass()));

	ai = Cast<AAIController>(enemy->GetController());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*dtdt = DeltaTime;*/
	// ����(����)�� ����

	switch (m_state)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UEnemyFSM::IdleState()
{
	// 1. ���¸� Move�� ��ȯ�Ѵ�.
		// 2. ���ð��� �Ǿ����ϱ�.
		// 3. �ð��� ��������ϱ�.

		// 1. �ð��� �귯��
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 2. ���ð��� �����ϱ�
		// -> ���� ����ð��� ���ð��� �ʰ��Ͽ��ٸ�
	if (currentTime > idleDelayTime)
	{
		// 3. ���¸� Move�� ��ȯ�ϰ� �ʹ�.
		m_state = EEnemyState::Move;

		// 4. �ٸ� ������ ����� �� ������ ����ð��� �ʱ�ȭ ��������.
		currentTime = 0;
	}
}

// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UEnemyFSM::MoveState()
{
	// Ÿ�� �������� �̵��ϰ� �ʹ�.
	
	
	//// Ÿ�� ã��
	//enemy = Cast<AEnemy>(GetOwner());

	//target = UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass());

	//dir = FVector(1, 1, 0) * moveSpeed;

	//if (target)
	//{
	//	dir = target->GetActorLocation() - enemy->GetActorLocation();
	//	dir.Normalize();
	//	dir *= moveSpeed;
	//}


	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSPlayer::StaticClass(), actors);*/
	
	/*for (auto t : actors)
	{
		target = Cast<AFPSPlayer>(t);
		break;
	}*/

	/*for (TActorIterator<AFPSPlayer> it(GetWorld()); it; ++it)
	{
		target = *it;
	}*/

	// 1. Ÿ���� �־�� �Ѵ�.
   // 2. ������ �ʿ� direction = target - me
	// 3. �̵��ϰ� �ʹ�.
	FVector direction = target->GetActorLocation() - enemy->GetActorLocation();
	float distance = direction.Size();
	direction.Normalize();

	if (ai)
	{
		ai->MoveToActor(target);
	}

	// enemy->AddMovementInput(direction, 1);

	/*FRotator targetRot = direction.ToOrientationRotator();
	FRotator myRot = enemy->GetActorRotation();

	myRot = FMath::Lerp(myRot, targetRot, 5 * GetWorld()->DeltaTimeSeconds);

	enemy->SetActorRotation(myRot);*/

	DrawDebugSphere(GetWorld(), enemy->GetActorLocation(), attackRange, 10, FColor::Red);

	// ���¸� �������� ��ȯ�ϰ� �ʹ�.
	// Ÿ�ٰ��� �Ÿ��� ���ݹ��� �ȿ� �������ϱ�.
	// Ÿ�ٰ��� �Ÿ���??

	if (distance < attackRange)
	{
		m_state = EEnemyState::Attack;
		currentTime = attackDelayTime;

		ai->StopMovement();
	}



	/*FVector P0 = GetOwner()->GetActorLocation();
	FVector P = P0 + direction * 500 * GetWorld()->DeltaTimeSeconds;
	GetOwner()->SetActorLocation(P, true);*/

	/*enemy = Cast<AEnemy>(GetOwner());

	target = UGameplayStatics::GetActorOfClass(GetWorld(), AFPSPlayer::StaticClass());

	dir = FVector(1, 1, 0) * moveSpeed;

	if (target)
	{
		dir = target->GetActorLocation() - enemy->GetActorLocation();
		dir.Normalize();
		dir *= moveSpeed;
	}

	FVector P = enemy->GetActorLocation() + dir * dtdt;
	enemy->SetActorLocation(P, true);*/

}


// �����ð��� �ѹ���
// �����ϰ�ʹ�.

void UEnemyFSM::AttackState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 2. ���ݽð��� �����ϱ�,
	if (currentTime > attackDelayTime)
	{
	// 3, �ֿܼ� ���� ����ϰ�ʹ�
	PRINTLOG(TEXT("ATTACK"));
	currentTime = 0;
	}


	//// �Ÿ��� �ʿ��ϴ�
	//FVector direction = target->GetActorLocation() - enemy->GetActorLocation();
	//
	//// �Ÿ��� �𷺼��� ������
	//float distance = direction.Size();
	//direction.Normalize();

	// ���� ��
	float distance = FVector::Dist(target->GetActorLocation(), enemy->GetActorLocation());

	// �Ÿ��� ���ݹ������� ũ�ٸ�
	if (distance > attackRange)
	{
	// ���¸� ����� ��ȯ���ְ�ʹ�
		m_state = EEnemyState::Move;
	}
}


// lerp�� �̿��ؼ� �ڷ� �и��� �˹� ȿ�� ������
void UEnemyFSM::DamageState()
{
	// LERP (From, to, �ӵ�)
	FVector myPos = enemy->GetActorLocation();

	myPos = FMath::Lerp(myPos, knockbackPos, knockbackSpeed * GetWorld()->DeltaTimeSeconds);

	float distance = FVector::Dist(myPos, knockbackPos);

	if (distance < knockbackRange)
	{

	myPos = knockbackPos;
	m_state = EEnemyState::Idle;
    	//Idle �϶� ����ð��� 0���� �ǵ��� ó���ϰڴ�.
   	 currentTime = 0;
		
	}

	enemy->SetActorLocation(myPos);



}

void UEnemyFSM::DieState()
{

}

void UEnemyFSM::OnDamageProcess(FVector shootDirection)
{

	ai->StopMovement();

	//���¸� Damage�� ��ȯ�ϰ� �ʹ�.
	
	// �ǰ� �޾��� �� hp�� ���ҽ�Ű�� 0 ���ϸ� die�� �ٲٰ� ���ֹ�����.
	// 1. HP�� ���������ϱ�
	hp --;

	// 2. ���� hp�� 0 ���ϸ�
	if (hp <= 0)
	{
		m_state = EEnemyState::Die;
		
		return;
	}

	// �˹� (KnockBack) ó��
	// �и� ����
	knockbackPos = enemy->GetActorLocation() + shootDirection * knockback;
	
	/*enemy->SetActorLocation(enemy->GetActorLocation() + shootDirection * knockback);
	*/
	m_state = EEnemyState::Damage;
	currentTime = 0;

	// Ÿ�̸� ���
	FTimerHandle damagTimer;
	GetWorld()->GetTimerManager().SetTimer(damagTimer, this, &UEnemyFSM::DamageState, damageDelayTime, false);
	}




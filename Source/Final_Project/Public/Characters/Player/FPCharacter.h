// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

/*
1. �ִϸ��̼��� ����� �Ʒ��� ���� �ڵ�� �ִϸ��̼��� �����ϴ� ����� ������, ������ �Ը� Ŀ���� �̷� ����� �������� �Ѱ迡 �΋H����. ���� �𸮾����� ü�������� �ִϸ��̼� �ý����� �����ϱ� ���� �������Ʈ�� ����Ѵ�.
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Animation/WarriorRun.WarriorRun"));
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
2. �ִϸ��̼� �������Ʈ�� �ҷ��� ���� ��ο� "_C"�� �ٿ�����Ѵ�. (Ŭ�������� �˷��ֱ� ����)
3. ACharater �𵨰� APawn ���� �������� ĳ���� �����Ʈ ������Ʈ�� ����Ѵٴ� ���̴�.
	ĳ���� �����Ʈ ������Ʈ�� ����
	1) �������� �߷� �ݿ� ������ ����
	2) �پ��� ������ ���� ����(�ȱ� �� ����, ���ư���, ���� ���), ���� �����ӿ� ���� �� ���� ���� ����
	3) ��Ƽ �÷��� �� ��Ʈ��ũ ȯ�濡�� ĳ���͵��� �������� �ڵ����� ����ȭ
4. �÷��� �� '~'Ű�� ������ �ܼ� ���â�� ���´�. �ű⼭ ������� DisplayAll <classname> �̷������� �˻��ϸ� �÷��� ȭ�鿡 �ش� ������ ���´�. �� �̶�, Ŭ���� ���λ�� �����ϰ� �Է��Ѵ�.
5. ���콺 ������ ��, Z��(Yaw)�� ACharactor���� useControllerRotationYaw�� �⺻ ������� �����Ǿ� ������, ������ Pitch, Roll�� �⺻ ��� �������� �����Ǿ� �ִ�.
6. ACharacter Ŭ���������� �̸� Jump��� ��� �Լ��� ���ε��Ҽ� �ְ� ���� �Ǿ��־� Ư���� ���� ���� �ٷ� ���ε��� �Ͽ� Jump�Լ��� ����� �����ϴ�. GetCharacterMovement()->JumpZVelocity�� ���� �⺻������ 420���� �����Ǿ� �ִ�.
7. UFPAnimInstance Ŭ�������� ���� ����ϴ� Ŭ������ ��������� �����Ͽ� ���� �������� �����ϴ� ���� �ٶ����ϴ�.
*/

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class FINAL_PROJECT_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

public:
	// Sets default values for this character's properties
	AFPCharacter();
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanSetWeapon();
	void Setweapon(class AFPWeapon* NewWeapon);

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class AFPWeapon* CurrentWeapon;

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UFPCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	// Attack �Լ��� ���� private�� �����Ͽ�����, NPC�� ������ �����ϱ� ���� public�� ��ġ�� �ٲپ� �־���.
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

private:
	// Axis ���� Ȱ���ؼ� �̵�Ű �����
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	// Action ����
	void Skill_Rest();
	void Attack_RMB();
	
	void ActFinish();
	void ViewChange();

	// �ִϸ��̼� ��Ÿ�ֿ� ��������Ʈ�� ����غ���
	UFUNCTION()
	void OnAttackMontageStarted(UAnimMontage* Montage);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void MovementAnimMotionState();

	void OnAssetLoadCompleted();

	bool IsActing();

	void FinishResting();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsRestEntered;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsResting;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsRMBComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	// AttackRange�� AttackRadius�� ���ݹ����� üũ�ϱ� ���� ����Ѵ�.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAnimMotionMoveing;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAnimFinish;
	// ���� ����ϴ� Ŭ������ ���漱�� �����ش�.
	UPROPERTY()
	class UFPAnimInstance* FPAnim;
	
	int32 AssetIndex = 0;

	// FSoftObjectPath Ŭ������ �ּ��� ��� ������ �����ϱ� ���� ����Ѵ�.
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class AFPAIController* FPAIController;

	UPROPERTY()
	class AFPPlayerController* FPPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = { };
};

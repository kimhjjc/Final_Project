// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/FPTutorialNPC.h"
#include "Characters/Player/FPAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/FPHUDWidget.h"
#include "UI/FPNPCWidget.h"
#include "UI/FPQuestWidget.h"
#include "UI/FPConversationWidget.h"
#include "Characters/Player/FPCharacter.h"
#include "Characters/Player/FPCharacterStatComponent.h"
#include "Characters/Player/FPPlayerController.h"
#include "Characters/NPC/FPTutorialNPC_NextQuestPosition.h"
#include "FPPlayerState.h"

// Sets default values
AFPTutorialNPC::AFPTutorialNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	QuestCheckWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QUESTCHECKWIDGET"));
	NPCNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCNAMEWIDGET"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	Trigger->SetupAttachment(GetMesh());
	NPCNameWidget->SetupAttachment(GetMesh());
	QuestCheckWidget->SetupAttachment(GetMesh());


	Trigger->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));

	QuestCheckWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	QuestCheckWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NPCNameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	NPCNameWidget->SetWidgetSpace(EWidgetSpace::Screen);


	Trigger->SetCollisionProfileName(TEXT("ItemBox"));

	NPCName = "John";
	QuestNumber = 0;
	MonsterKill = 0;
	TargetKill = 0;
	IsQuestAccept = false;
	IsInteractive = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_QUESTCHECK(TEXT("/Game/UI/UI_QuestIcon.UI_QuestIcon_C"));
	if (UI_QUESTCHECK.Succeeded())
	{
		QuestCheckWidget->SetWidgetClass(UI_QUESTCHECK.Class);
		QuestCheckWidget->SetDrawSize(FVector2D(150.0f, 150.0f));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_NPCNAME(TEXT("/Game/UI/UI_NPCName.UI_NPCName_C"));
	if (UI_NPCNAME.Succeeded())
	{
		NPCNameWidget->SetWidgetClass(UI_NPCNAME.Class);
		NPCNameWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// 애니메이션 블루프린트 사용하기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> NPC_ANIM(TEXT("/Game/Animations/NPCAnimBlueprint.NPCAnimBlueprint_C"));
	if (NPC_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(NPC_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HOWTOREST_C(TEXT("/Game/UI/TutorialUI/UI_HowToRest.UI_HowToRest_C"));
	if (UI_HOWTOREST_C.Succeeded())
	{
		HowToRestWindowWidgetClass = UI_HOWTOREST_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HOWTOATTACK_C(TEXT("/Game/UI/TutorialUI/UI_HowToAttack.UI_HowToAttack_C"));
	if (UI_HOWTOATTACK_C.Succeeded())
	{
		HowToAttackWindowWidgetClass = UI_HOWTOATTACK_C.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_COMBOATTACK_C(TEXT("/Game/UI/TutorialUI/UI_ComboAttack.UI_ComboAttack_C"));
	if (UI_COMBOATTACK_C.Succeeded())
	{
		ComboAttackWindowWidgetClass = UI_COMBOATTACK_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_CLEAR_C(TEXT("/Game/UI/TutorialUI/UI_GameClear.UI_GameClear_C"));
	if (UI_CLEAR_C.Succeeded())
	{
		ClearWindowWidgetClass = UI_CLEAR_C.Class;
	}
}

// Called when the game starts or when spawned
void AFPTutorialNPC::BeginPlay()
{
	Super::BeginPlay();

	auto NPCWidget = Cast<UFPNPCWidget>(NPCNameWidget->GetUserWidgetObject());
	FPCHECK(nullptr != NPCWidget);
	NPCWidget->BindNPCName(NPCName);

	QuestInfo = "";
	ContentInfo = "";

	auto FPCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FPCHECK(nullptr != FPCharacter);

	auto FPPlayerControlloler = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != FPPlayerControlloler);

	FPPlayerControlloler->GetFPPlayerState()->SetQuestNumber(0);
	if (FPPlayerControlloler->GetFPPlayerState()->GetQuestNumber() != 0)
	{
		QuestNumber = FPPlayerControlloler->GetFPPlayerState()->GetQuestNumber();

		TArray<AActor*> NextPlace;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPTutorialNPC_NextQuestPosition::StaticClass(), NextPlace);

		for (auto Place : NextPlace)
		{
			auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
			FPCHECK(nullptr != TempPlace);
			if (TempPlace->GetQuestNumber() == QuestNumber)
			{
				SetActorRelativeLocation(TempPlace->GetActorLocation());
				SetActorRelativeRotation(TempPlace->GetActorRotation());
			}

		}
	}


}

void AFPTutorialNPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// BoxComponent에는  OnComponentBeginOverlap이라는 멀티캐스팅 델리게이트가 있다. 이를 이용하여 박스컴포넌트에 겹침 콜리전 반응 진입 시 델리게이트 발생.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFPTutorialNPC::OnCharacterOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AFPTutorialNPC::OnOverlapEnd);
}

// Called every frame
void AFPTutorialNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPTutorialNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPTutorialNPC::QuestUpdate(class AFPCharacter* FPCharacter, bool bIsGameLoad)
{
	auto PlayerController = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != PlayerController);

	if (nullptr != FPCharacter)
	{
		FPLOG(Warning, TEXT("NPC Interactive!"));

		IsInteractive = true;

		TArray<FString> ContentInfos;

		static TArray<AActor*> NextPlace;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPTutorialNPC_NextQuestPosition::StaticClass(), NextPlace);

		if (IsQuestAccept == true && MonsterKill == TargetKill)	//QuestNumber 번째 클리어
		{
			if (QuestNumber == 0)
			{
				PlayerController->NPCKill(15);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "go to the gate.";

				ContentInfos.Add("Good.");
				ContentInfos.Add("Now you know how to restore your HP.");
				ContentInfos.Add("Well done. Shall we move on next?");
				ContentInfos.Add("I'll wait in front of the gate.");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					for (auto Place : NextPlace)
					{
						auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
						FPCHECK(nullptr != TempPlace);
						if (TempPlace->GetQuestNumber() == QuestNumber)
						{
							SetActorRelativeLocation(TempPlace->GetActorLocation());
							SetActorRelativeRotation(TempPlace->GetActorRotation());
						}

					}

					});

			}
			else if (QuestNumber == 1)
			{
				PlayerController->NPCKill(30);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "Try to talk to john again.";

				ContentInfos.Add("Good.");
				ContentInfos.Add("Looks like you're good at attacking.");
				ContentInfos.Add("Then why don't you talk to me again?");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

			}
			else if (QuestNumber == 2)
			{
				PlayerController->NPCKill(50);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "Let's follow John.";

				ContentInfos.Add("Thank you for doing me a favor.");
				ContentInfos.Add("I just found a place that looked suspicious.");
				ContentInfos.Add("Then follow me where I am.");
				ContentInfos.Add("I'll be there first.");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					for (auto Place : NextPlace)
					{
						auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
						FPCHECK(nullptr != TempPlace);
						if (TempPlace->GetQuestNumber() == QuestNumber)
						{
							SetActorRelativeLocation(TempPlace->GetActorLocation());
							SetActorRelativeRotation(TempPlace->GetActorRotation());
						}

					}

					});

			}
			else if (QuestNumber == 3)
			{
				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(ComboAttackWindowWidgetClass);

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
					});

				PlayerController->NPCKill(50);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "Try to talk to john again.";

				ContentInfos.Add("Ha ha, How do you like it?");
				ContentInfos.Add("Then let's go to the destination now.");
				ContentInfos.Add("Talk to me again when you're ready?");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
			}
			else if (QuestNumber == 4)
			{
				PlayerController->NPCKill(200);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);

				// Quest 5번 내용. 바로 시작.
				MonsterKill = -1;
				TargetKill = 1;

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Kill the big spider boss. \n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					IsQuestAccept = true;
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Oh my god, You are very strong!");
				ContentInfos.Add("I can't believe you've got them all in one fell swoop.");
				ContentInfos.Add("But a bigger enemy appeared before us.");
				ContentInfos.Add("It's the big spider boss.");
				ContentInfos.Add("Maybe the villains had it secretly.");
				ContentInfos.Add("We have to kill this to go further.");
				ContentInfos.Add("I believe you can win easily. Cheer up!");

				PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
			}
			else if (QuestNumber == 5)
			{
				PlayerController->NPCKill(300);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "Go to forward and Talk to John.";

				ContentInfos.Add("I knew it! I believed it!");
				ContentInfos.Add("A little further forward from here, Maybe there's a head of the villians.");
				ContentInfos.Add("Cheer up a little bit more.");
				ContentInfos.Add("Will only kill the boss, now it's all peaceful.");
				ContentInfos.Add("Let's go for the final fight!");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					for (auto Place : NextPlace)
					{
						auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
						FPCHECK(nullptr != TempPlace);
						if (TempPlace->GetQuestNumber() == QuestNumber)
						{
							SetActorRelativeLocation(TempPlace->GetActorLocation());
							SetActorRelativeRotation(TempPlace->GetActorRotation());
						}
					}
					});
			}
			else if (QuestNumber == 6)
			{
				PlayerController->NPCKill(500);
				PlayerController->OnQuestUpdate.Clear();
				QuestNumber++;
				PlayerController->GetFPPlayerState()->SetQuestNumber(QuestNumber);
				IsQuestAccept = false;
				QuestInfo = "Clear!!";

				ContentInfos.Add("We won!!");
				ContentInfos.Add("Soon the peaceful world will return.");
				ContentInfos.Add("You did a great job.");
				ContentInfos.Add("It's all thanks to you.");
				ContentInfos.Add("Shall we get back to town?");

				PlayerController->GetQuestWidget()->BindNPCQuest("", QuestInfo);
				PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(ClearWindowWidgetClass);
					FPPlayerCharacter->SetGameClear(true);

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
					});
			}
			FPCharacter->NPCConversation(NPCName, ContentInfos);
		}
		else if (IsQuestAccept == false)
		{
			if (QuestNumber == 0)
			{
				FPCharacter->CharacterStat->SetDamage(100);

				MonsterKill = FPCharacter->CharacterStat->GetHP();
				TargetKill = FPCharacter->CharacterStat->GetMaxHP();

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(HowToRestWindowWidgetClass);

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					MonsterKill = FMath::Clamp<int32>(FPPlayerCharacter->CharacterStat->GetHP() + 1, 0, TargetKill);
					QuestInfo = "Press the R key and restore your HP.\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Hey, you look strong, bro.");
				ContentInfos.Add("The weather is good too.The Goblin has been hunting?");
				ContentInfos.Add("But you're not ready for battle yet.");
				ContentInfos.Add("Then I'll show you the basic skills.");
				ContentInfos.Add("First, restore your HP.");
				ContentInfos.Add("If you press the R key, you can recover HP.");
				ContentInfos.Add("Give it a try.");

				IsQuestAccept = true;
			}
			else if (QuestNumber == 1)
			{
				MonsterKill = 0;
				TargetKill = 2;

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					FPPlayerCharacter->SetandOnTutorialUI(HowToAttackWindowWidgetClass);

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					auto FPPlayerCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
					int32 CurrentCombo = FPPlayerCharacter->GetCurrentCombo();

					MonsterKill = FMath::Clamp<int32>(CurrentCombo, 0, TargetKill);
					QuestInfo = "Try combo attack with press LMB button\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Oh came faster than expected?");
				ContentInfos.Add("But you're still not ready for battle.");
				ContentInfos.Add("You need to know how to attack, right?");
				ContentInfos.Add("Attack is LMB.");
				ContentInfos.Add("And you can combo attack LMB double click.");
				ContentInfos.Add("Give it a try.");

				IsQuestAccept = true;

			}
			else if (QuestNumber == 2)
			{
				MonsterKill = -1;
				TargetKill = 3;
				// 어떤 델리게이트를 받아올까?
				// 킬을 했을 때.

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Kill the monsters you see when you near the gate.\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Ok, Then let's kill Goblin out there.");
				ContentInfos.Add("I'm worried that there are a lot of goblins around recently.");
				ContentInfos.Add("I wish to reduce the number of you.");
				ContentInfos.Add("Maybe someone's plotting behind our.");
				ContentInfos.Add("Now, try to kill three goblins out there. Good luck.");

				IsQuestAccept = true;

			}
			else if (QuestNumber == 3)
			{
				MonsterKill = -1;
				TargetKill = 1;

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Open the box next to you.\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					IsQuestAccept = true;
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Ok, let's go there now.");
				ContentInfos.Add("But I'll give you an item before we leave.");
				ContentInfos.Add("The box next to you, can you open it?");
				ContentInfos.Add("You will never be disappointed.");

				IsQuestAccept = true;
			}
			else if (QuestNumber == 4)
			{
				MonsterKill = -1;
				TargetKill = 10;

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					for (auto Place : NextPlace)
					{
						auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
						FPCHECK(nullptr != TempPlace);
						if (TempPlace->GetQuestNumber() == QuestNumber)
						{
							SetActorRelativeLocation(TempPlace->GetActorLocation());
							SetActorRelativeRotation(TempPlace->GetActorRotation());
						}
					}
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Kill the bandits out there and get to the target point. \n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					IsQuestAccept = true;
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Ok, let's go there now.");
				ContentInfos.Add("But there are villains on the road.");
				ContentInfos.Add("If we ignore those villains, something big will happen later.");
				ContentInfos.Add("So you're gonna have to kill them on their way to the target point.");
				ContentInfos.Add("Then I'll be at the target point first, so I'll meet you there.");

				IsQuestAccept = true;
			}
			else if (QuestNumber == 6)
			{
				MonsterKill = -1;
				TargetKill = 1;

				FPCharacter->OnConversationEnd.AddLambda([this]() -> void {
					for (auto Place : NextPlace)
					{
						auto TempPlace = Cast<AFPTutorialNPC_NextQuestPosition>(Place);
						FPCHECK(nullptr != TempPlace);
						if (TempPlace->GetQuestNumber() == QuestNumber)
						{
							SetActorRelativeLocation(TempPlace->GetActorLocation());
							SetActorRelativeRotation(TempPlace->GetActorRotation());
						}
					}
					});

				PlayerController->OnQuestUpdate.AddLambda([this]() -> void {
					FPLOG(Warning, TEXT("OnQuestUpdate"));
					MonsterKill = FMath::Clamp<int32>(MonsterKill + 1, 0, TargetKill);
					QuestInfo = "Kill the Last boss.\n";
					QuestInfo += "(";
					QuestInfo += FString::FromInt(MonsterKill);
					QuestInfo += " / ";
					QuestInfo += FString::FromInt(TargetKill);
					QuestInfo += ")";

					auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
					PlayerController->GetQuestWidget()->BindNPCQuest(NPCName, QuestInfo);
					PlayerController->GetHUDWidget()->UpdateQuest(QuestInfo);
					IsQuestAccept = true;
					});
				PlayerController->OnQuestUpdate.Broadcast();

				ContentInfos.Add("Oh! We finally met the final boss!");
				ContentInfos.Add("The boss looks too strong. Do you think we can win?");
				ContentInfos.Add("But I'm sure you can win as you have.");
				ContentInfos.Add("Kill that boss and it's all over.");
				ContentInfos.Add("Cheer up and win the last battle!");

				IsQuestAccept = true;
			}
			else
			{
				ContentInfos.Add("Recently more monsters appearance...");
			}

			FPCharacter->NPCConversation(NPCName, ContentInfos);
		}
		else if (IsQuestAccept == true && MonsterKill != TargetKill)
		{
			ContentInfo = "You still haven't finished?";
			PlayerController->GetConversationWidget()->BindNPCContent(NPCName, ContentInfo);
		}


		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Visible);

	}

}

void AFPTutorialNPC::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	auto FPCharacter = Cast<AFPCharacter>(OtherActor);
	FPCHECK(nullptr != FPCharacter);
	auto PlayerController = FPCharacter->GetFPPlayerController();
	FPCHECK(nullptr != PlayerController);

	QuestUpdate(FPCharacter);

}

void AFPTutorialNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsInteractive = false;

	CloseContentWidget();
}


void AFPTutorialNPC::CloseContentWidget()
{
	GetWorld()->GetTimerManager().SetTimer(CloseTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		if (IsInteractive) return;

		auto PlayerController = Cast<AFPPlayerController>(GetWorld()->GetFirstPlayerController());
		PlayerController->GetConversationWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}), 5, false);
}
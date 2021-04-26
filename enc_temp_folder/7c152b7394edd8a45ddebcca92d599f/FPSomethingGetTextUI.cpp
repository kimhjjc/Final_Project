// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPSomethingGetTextUI.h"
#include "Components/WidgetComponent.h"
#include "UI/FPSomthingGetTextWidget.h"

// Sets default values
AFPSomethingGetTextUI::AFPSomethingGetTextUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("SOMTHINGTEXTWIDGET"));

	RootComponent = TextWidget;

	static ConstructorHelpers::FClassFinder<UUserWidget> EXPTEXT(TEXT("/Game/UI/UI_WantText.UI_WantText_C"));
	if (EXPTEXT.Succeeded())
	{
		TextWidget->SetWidgetClass(EXPTEXT.Class);
		TextWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	TextZLocation = 185.0f;
	TransformStart = false;
	MyText = " ";

	TextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, TextZLocation));
	TextWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void AFPSomethingGetTextUI::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		TransformStart = true;
		}), 1.5f, false);
}

// Called every frame
void AFPSomethingGetTextUI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TransformStart)
	{
		TextZLocation += 0.05f;
		if (TextZLocation >= 190.0f)
			Destroy();
		TextWidget->SetRelativeLocation(FVector(0, 0, TextZLocation));
	}

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	SetActorRotation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation());

	PlayerLocation.Z = TextZLocation;
	SetActorLocation(PlayerLocation);
}

void AFPSomethingGetTextUI::BindText(FString Text)
{
	MyText = Text;


	//auto SpawnedSomethingTextWidget = Cast<UFPSomthingGetTextWidget>(TextWidget->GetUserWidgetObject());
	//FPCHECK(nullptr != SpawnedSomethingTextWidget);
	////SpawnedSomethingTextWidget->BindWantText(MyText);
}
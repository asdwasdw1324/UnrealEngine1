// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();
    TimeToNextCameraChange = 2.0f;

    APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (OurPlayerController)
    {
        if (CameraOne != nullptr)
            {
                OurPlayerController->SetViewTarget(CameraOne);
                CurrentCameraIndex = 0;
            }
    }
}

// Called every frame
void ACameraDirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const float TimeBetweenCameraChanges = 2.0f;
    const float SmoothBlendTime = 0.75f;
    TimeToNextCameraChange = TimeToNextCameraChange - DeltaTime;

    //每一帧画面打印TimeToNextCameraChange的值
    //const float DebugMessageDuration = 2.0f;
    //const FColor DebugMessageColor = FColor::Yellow;
    //GEngine->AddOnScreenDebugMessage(-1, DebugMessageDuration, DebugMessageColor, FString::Printf(TEXT("TimeToNextCameraChange: %.2f"), TimeToNextCameraChange));

    if (TimeToNextCameraChange <= 0.0f) {
        TimeToNextCameraChange += TimeBetweenCameraChanges; // add current TimeToNextCameraChange value
        APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (OurPlayerController)
        {
            if ((CurrentCameraIndex == 0) && (CameraTwo != nullptr))
            {
                OurPlayerController->SetViewTargetWithBlend(CameraTwo, SmoothBlendTime);
                CurrentCameraIndex = (CurrentCameraIndex + 1) % 2;
            }
            else if ((CurrentCameraIndex == 1) && (CameraOne != nullptr)) {
                OurPlayerController->SetViewTargetWithBlend(CameraOne, SmoothBlendTime);
                CurrentCameraIndex = (CurrentCameraIndex + 1) % 2;
            }
        }
    }
}

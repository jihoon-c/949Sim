// Fill out your copyright notice in the Description page of Project Settings.

#include "Aircraft_HUD.h"
#include "ThirdParty/JSBSim/include/FGFDMExec.h"
#include "ThirdParty/JSBSim/include/models/FGAuxiliary.h"


#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// Native lifecycle

void UAircraft_HUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 기본 앵커/정렬을 중앙으로 고정
    if (HorizonLineImage)
    {
        if (UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HorizonLineImage))
        {
            CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
            CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            CanvasSlot->SetSize(HorizonLineSize);
        }
    }
}

void UAircraft_HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 여기서는 자동으로 업데이트하지 않습니다.
    // 외부에서 CIGI 업데이트가 들어올 때 UpdateHorizonLineFromCIGI() 를 호출하세요.
}


void UAircraft_HUD::UpdateHorizonLineFromEuler(float PitchDegrees, float RollDegrees)
{
    if (!HorizonLineImage || !GetWorld())
    {
        return;
    }

    // 이 함수에서는 더 이상 뷰포트 사이즈를 직접 계산할 필요가 없습니다.
    // 앵커가 모든 것을 처리해줍니다.

    // 피치 -> 픽셀 오프셋
    float PitchOffsetPx = CalculatePitchOffset(PitchDegrees);

    // 캔버스에 적용
    ApplyHorizonTransform(PitchOffsetPx, RollDegrees);
}

void UAircraft_HUD::UpdateHorizonLineFromPawn(APawn* Pawn)
{
    if (!Pawn) return;

    // Pawn의 World Rotation 가져오기
    FRotator WorldRotation = Pawn->GetActorRotation();

    // 언리얼 Rotator는 이미 Degree 단위
    float PitchDeg = WorldRotation.Pitch;
    float RollDeg = WorldRotation.Roll;
    float YawDeg = WorldRotation.Yaw;

    UE_LOG(LogTemp, Log, TEXT("Roll: %.2f Pitch: %.2f Yaw: %.2f"), RollDeg, PitchDeg, YawDeg);

    // HUD 업데이트 (기존 함수 활용)
    UpdateHorizonLineFromEuler(PitchDeg, RollDeg);
}
void UAircraft_HUD::ApplyHorizonTransform(float PitchOffsetPx, float RollDegrees)
{
    if (!HorizonLineImage) return;

    if (UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HorizonLineImage))
    {
        // 수평선 너비 설정 (이전과 동일)
        float DesiredWidth = FMath::Max(HorizonLineSize.X, CanvasSlot->GetSize().X);
        CanvasSlot->SetSize(FVector2D(DesiredWidth, HorizonLineSize.Y));

        // ==================== 코드 수정 시작 ====================

        // 최종 위치는 중앙(0,0)에서의 상대적인 오프셋만 반영합니다.
        FVector2D FinalOffset = FVector2D(0.0f, PitchOffsetPx + CustomVerticalOffset);
        CanvasSlot->SetPosition(FinalOffset);

        // ===================== 코드 수정 끝 ======================

        // 회전 적용 (이전과 동일)
        float Angle = CalculateRollRotation(RollDegrees);
        HorizonLineImage->SetRenderTransformAngle(Angle);

        HorizonLineImage->SetVisibility(ESlateVisibility::Visible);

        UE_LOG(LogTemp, Verbose, TEXT("HorizonLine Apply: Offset=%s, Roll=%.2fdeg, AngleApplied=%.2f"),
            *FinalOffset.ToString(), RollDegrees, Angle);
    }
}

float UAircraft_HUD::CalculatePitchOffset(float PitchDegrees) const
{
    float Offset = PitchDegrees * PixelsPerDegree;
    // 피치가 양수일 때(코가 위로 올라가면) 위로 올라가게 할지 아래로 내릴지는 convention에 따라 조정.
    // 여기서는 양수 피치 -> 위로 이동 (음수 -> 아래로 이동) 이 되며 필요시 부호를 반전하세요.
    Offset = FMath::Clamp(Offset, -MaxPitchOffset, MaxPitchOffset);
    return Offset;
}

float UAircraft_HUD::CalculateRollRotation(float RollDegrees) const
{
    // 화면에서 보이는 회전은 보정 필요할 수 있음(부호 반전 등).
    // 이전 코드와 동일하게 -Roll 을 사용하고 싶으면 return -RollDegrees;
    return -RollDegrees;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "JSBSimFlightDynamicsModel/Public/JSBSimMovementComponent.h"
//#include "ThirdParty/JSBSim/include/FGFDMExec.h"
//#include "ThirdParty/JSBSim/include/models/FGAuxiliary.h"
#include "Aircraft_HUD.generated.h"


class FGFDMExec;
/**
 * 
 */
UCLASS()
class SIM949_API UAircraft_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

    //음수가 화면 위로 올라가는거임
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Settings")
    float CustomVerticalOffset = -450.0f;

    // 위젯 초기화
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    //데이터로부터 수평선 업데이트 
   // UFUNCTION(BlueprintCallable)
    void UpdateHorizonLineFromPawn(APawn* Pawn);

    // 직접 자세(도 단위)로 업데이트 가능
    void UpdateHorizonLineFromEuler(float PitchDegrees, float RollDegrees);

protected:
    // 실제 업데이트 로직 (내부 사용)
    void ApplyHorizonTransform(/* const FVector2D& CenterPos,*/ float PitchOffsetPx, float RollDegrees);

    // 유틸
    float CalculatePitchOffset(float PitchDegrees) const;
    float CalculateRollRotation(float RollDegrees) const;

    // ---------- 위젯 바인딩 (UMG에서 바인딩) ----------
    // 수평선을 표현하는 이미지 (UMG에서 바인딩하세요)
    UPROPERTY(meta = (BindWidget))
    UImage* HorizonLineImage;

    // 수평선 기본 크기 (width는 런타임에 덮어쓸 수 있음)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    FVector2D HorizonLineSize = FVector2D(1024.0f, 6.0f);

    // 피치 각도 -> 픽셀 변환 비율 (1도당 픽셀)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    float PixelsPerDegree = 4.0f;

    // 피치 오프셋 클램프 (픽셀)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    float MaxPitchOffset = 100.0f;
};

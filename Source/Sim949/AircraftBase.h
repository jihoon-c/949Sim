#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AircraftBase.generated.h"

class UJSBSimMovementComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SIM949_API AAircraftBase : public APawn
{
    GENERATED_BODY()

public:
    AAircraftBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

   UFUNCTION(BlueprintCallable)
   void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // ============================================
    // Components
    // ============================================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* AircraftMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UJSBSimMovementComponent* JSBSimMovement;


    // ============================================
    // Input System (Enhanced Input)
    // ============================================

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* AircraftMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* PitchRollAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* YawAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ThrottleUpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ThrottleDownAction;

    // ============================================
    // Flight Control Variables
    // ============================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float PitchInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float RollInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float YawInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float ThrottleInput;

    // 조종 감도 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float PitchSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float RollSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float YawSensitivity = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float ThrottleChangeRate = 0.3f; // 초당 스로틀 변화율

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float MaxThrottle = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Control")
    float MinThrottle = 0.0f;

    // ============================================
    // Aircraft Properties
    // ============================================

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Properties")
    FString AircraftName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Properties")
    FString JSBSimConfigFile; // JSBSim XML 설정 파일 경로



    // ============================================
    // Input Handler Functions
    // ============================================

    void HandlePitchRoll(const FInputActionValue& Value);
    void HandleYaw(const FInputActionValue& Value);
    void HandleThrottleUp(const FInputActionValue& Value);
    void HandleThrottleDown(const FInputActionValue& Value);

    // ============================================
    // Flight Control Functions
    // ============================================

    UFUNCTION(BlueprintCallable, Category = "Flight Control")
    virtual void ApplyControlsToJSBSim();

    UFUNCTION(BlueprintCallable, Category = "Flight Control")
    void SetThrottle(float NewThrottle);

    UFUNCTION(BlueprintCallable, Category = "Flight Control")
    void AdjustThrottle(float DeltaThrottle);



    // ============================================
    // Utility Functions
    // ============================================

    UFUNCTION(BlueprintPure, Category = "Flight Control")
    float GetCurrentSpeed() const;

    UFUNCTION(BlueprintPure, Category = "Flight Control")
    float GetCurrentAltitude() const;

    UFUNCTION(BlueprintPure, Category = "Flight Control")
    float GetCurrentThrottle() const { return ThrottleInput; }

};
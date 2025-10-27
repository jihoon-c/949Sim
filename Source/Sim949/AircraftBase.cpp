#include "AircraftBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JSBSimFlightDynamicsModel/Public/JSBSimMovementComponent.h"


AAircraftBase::AAircraftBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // 컴포넌트 생성
    AircraftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AircraftMesh"));
    RootComponent = AircraftMesh;

    JSBSimMovement = CreateDefaultSubobject<UJSBSimMovementComponent>(TEXT("JSBSimMovement"));

    // 초기값 설정
    PitchInput = 0.0f;
    RollInput = 0.0f;
    YawInput = 0.0f;
    ThrottleInput = 0.0f;


}

void AAircraftBase::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input 설정
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            if (AircraftMappingContext)
            {
                Subsystem->AddMappingContext(AircraftMappingContext, 0);
            }
        }
    }
}

void AAircraftBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ApplyControlsToJSBSim();
}

void AAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Pitch/Roll 입력 바인딩
        if (PitchRollAction)
        {
            EnhancedInputComponent->BindAction(PitchRollAction, ETriggerEvent::Triggered, this, &AAircraftBase::HandlePitchRoll);
        }

        // Yaw 입력 바인딩
        if (YawAction)
        {
            EnhancedInputComponent->BindAction(YawAction, ETriggerEvent::Triggered, this, &AAircraftBase::HandleYaw);
        }

        // Throttle Up 입력 바인딩
        if (ThrottleUpAction)
        {
            EnhancedInputComponent->BindAction(ThrottleUpAction, ETriggerEvent::Triggered, this, &AAircraftBase::HandleThrottleUp);
        }

        // Throttle Down 입력 바인딩
        if (ThrottleDownAction)
        {
            EnhancedInputComponent->BindAction(ThrottleDownAction, ETriggerEvent::Triggered, this, &AAircraftBase::HandleThrottleDown);
        }
    }
}

// ============================================
// Input Handler Functions
// ============================================

void AAircraftBase::HandlePitchRoll(const FInputActionValue& Value)
{
    FVector2D InputVector = Value.Get<FVector2D>();

    // Y축: Pitch (위/아래)
    PitchInput = FMath::Clamp(InputVector.Y * PitchSensitivity, -1.0f, 1.0f);

    // X축: Roll (좌/우 기울기)
    RollInput = FMath::Clamp(InputVector.X * RollSensitivity, -1.0f, 1.0f);
}

void AAircraftBase::HandleYaw(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    YawInput = FMath::Clamp(InputValue * YawSensitivity, -1.0f, 1.0f);
}

void AAircraftBase::HandleThrottleUp(const FInputActionValue& Value)
{
    AdjustThrottle(ThrottleChangeRate * GetWorld()->GetDeltaSeconds());
}

void AAircraftBase::HandleThrottleDown(const FInputActionValue& Value)
{
    AdjustThrottle(-ThrottleChangeRate * GetWorld()->GetDeltaSeconds());
}

// ============================================
//  조종값 전달
// ============================================

void AAircraftBase::ApplyControlsToJSBSim()
{
    if (!JSBSimMovement) return;



    for (FEngineCommand& Command : JSBSimMovement->EngineCommands)
    {
        Command.Throttle = ThrottleInput;
    }

    JSBSimMovement->Commands.Aileron = (double)PitchInput;
    JSBSimMovement->Commands.Elevator = (double)RollInput;
}


void AAircraftBase::SetThrottle(float NewThrottle)
{
    ThrottleInput = FMath::Clamp(NewThrottle, MinThrottle, MaxThrottle);
}

void AAircraftBase::AdjustThrottle(float DeltaThrottle)
{
    ThrottleInput = FMath::Clamp(ThrottleInput + DeltaThrottle, MinThrottle, MaxThrottle);
}

// ============================================
// Utility Functions
// ============================================

float AAircraftBase::GetCurrentSpeed() const
{
    if (JSBSimMovement)
    {
        // JSBSimMovement에서 현재 속도를 가져오는 로직

        return 0.0f; // 임시 반환값
    }

    // JSBSim이 없으면 물리 속도 사용
    return GetVelocity().Size();
}

float AAircraftBase::GetCurrentAltitude() const
{
    return GetActorLocation().Z;
}
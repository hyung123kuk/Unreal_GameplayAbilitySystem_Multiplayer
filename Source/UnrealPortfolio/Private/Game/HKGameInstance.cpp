// Copyright Druid Mechanics


#include "Game/HKGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UHKGameInstance::Init()
{
    Super::Init();
	GEngine->TravelFailureEvent.AddUObject(this, &UHKGameInstance::OnNetworkLevelChangeFailure);
}

void UHKGameInstance::OnNetworkLevelChangeFailure(UWorld*, ETravelFailure::Type, const FString& ErrorMessage)
{
    LevelChangeErrorMessage = ErrorMessage;
    OccurredLevelChangeError = true;
}

/// <summary>
/// ��Ʈ��ũ ���� ���н� ���� �ٲ�鼭 �ٸ� ���� �ִ� ������ ������ϴ�.
/// ���� GameInstance���� ������ �ֽ��ϴ�.
/// ���� �޼����� �ѹ��� ������ �� �ֽ��ϴ�.
/// </summary>
/// <param name="ErrorMessage">���� �޼��� Ref</param>
/// <returns>���� �޼����� �ִ°�?</returns>
bool UHKGameInstance::GetNetworkLevelChangeErrorMessage(FString& ErrorMessage)
{
    if (OccurredLevelChangeError)
    {
        ErrorMessage = LevelChangeErrorMessage;
        OccurredLevelChangeError = false;
        return true;
    }

    return false;
}




// Copyright Druid Mechanics


#include "Game/HKGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Item/Inventory.h"

void UHKGameInstance::Init()
{
    Super::Init();
	GEngine->NetworkFailureEvent.AddUObject(this, &UHKGameInstance::OnNetworkLevelChangeFailure);

}

void UHKGameInstance::OnNetworkLevelChangeFailure(UWorld*, UNetDriver*, ENetworkFailure::Type FailType, const FString& ErrorMessage)
{
    LevelChangeErrorMessage = ErrorMessage;
    OccurredLevelChangeError = true;
}

/// <summary>
/// 네트워크 접속 실패시 씬이 바뀌면서 다른 곳에 있는 정보가 사라집니다.
/// 따라서 GameInstance에서 가지고 있습니다.
/// 에러 메세지는 한번만 가져올 수 있습니다.
/// </summary>
/// <param name="ErrorMessage">에러 메세지 Ref</param>
/// <returns>에러 메세지가 있는가?</returns>
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

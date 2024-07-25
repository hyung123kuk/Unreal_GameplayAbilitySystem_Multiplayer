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

void UHKGameInstance::StoreInGamePlayerInfoBeforeGameStart(TArray<FInGamePlayerInfo> InGamePlayerInfoParams)
{
    InGamePlayersInfo = InGamePlayerInfoParams;
}

FInGamePlayerInfo UHKGameInstance::GetPlayerInfoWithID(FString ID)
{
    for (auto& PlayerInfo : InGamePlayersInfo)
    {
        if (PlayerInfo.UserID == ID)
        {
            return PlayerInfo;
        }
    }

    return FInGamePlayerInfo();
}

FInGamePlayerInfo UHKGameInstance::GetAdminPlayerInfo()
{
    for (auto& PlayerInfo : InGamePlayersInfo)
    {
        if (PlayerInfo.RoomAdmin)
        {
            return PlayerInfo;
        }
    }
    return FInGamePlayerInfo();
}

FInGamePlayerInfo UHKGameInstance::GetMyGameInfo()
{

    return FInGamePlayerInfo();
}

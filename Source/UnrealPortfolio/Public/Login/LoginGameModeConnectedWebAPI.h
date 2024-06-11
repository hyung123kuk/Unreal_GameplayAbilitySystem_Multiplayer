// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "LoginGameModeConnectedWebAPI.generated.h"

//클라이언트에서 사용하는 GameMode입니다. 
//이 GameMode를 사용하려면 클라이언트는 StandAlone 상태여야 합니다.
//서버는 WebServer가 있어야 합니다.

class AHKUILoginController;

USTRUCT(BlueprintType)
struct FServerInfoData
{
	GENERATED_BODY()

	FServerInfoData() {}
	FServerInfoData(FString& serverName, FString& Ip, int crowdedLevel) :
		ServerName(serverName),
		IP(Ip),
		CrowdedLevel(crowdedLevel) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CrowdedLevel;

};

UCLASS()
class UNREALPORTFOLIO_API ALoginGameModeConnectedWebAPI : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);


	void CheckAccountID(const FString& Id);
	void ResponseOfCheckAccountID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void CreateAccount(const FString& Id, const FString Password);
	void ResponseOfCreateAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void LoginAccount(const FString& Id, const FString Password);
	void ResponseOfLoginAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

private:
	void Post(FHttpRequestRef& Request, TSharedRef<FJsonObject>& RequestObj, FString URL);

private:
	TObjectPtr<AHKUILoginController> LoginPlayerController;
};

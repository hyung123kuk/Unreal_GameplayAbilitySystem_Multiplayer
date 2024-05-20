// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Interaction/UIPageInterface.h"
#include "LobbyRoomInfoWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeRoomValueSignature, ULobbyRoomInfoWidgetController*, RoomInfoController);

USTRUCT(BlueprintType)
struct FLobbyRoomInfoWidgetControllerParams
{
	GENERATED_BODY()
	
	FLobbyRoomInfoWidgetControllerParams() {}
	FLobbyRoomInfoWidgetControllerParams(FString& roomName, int maxPlayerNum, int nowPlayerNum,bool publicRoom) :
		RoomName(roomName),
		MaxPlayerNum(maxPlayerNum),
		NowPlayerNum(nowPlayerNum),
		PublicRoom(publicRoom) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxPlayerNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NowPlayerNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PublicRoom;

};


UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API ULobbyRoomInfoWidgetController : public UHKWidgetControllerBase, public IUIPageInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FLobbyRoomInfoWidgetControllerParams& Params);

	virtual FString GetElementName() override;

	UPROPERTY(BlueprintAssignable, Category = "ChangeValue")
	FChangeRoomValueSignature ChangeRoomValueSignature;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString RoomName;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	int MaxPlayerNum;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	int NowPlayerNum;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	bool PublicRoom;
};

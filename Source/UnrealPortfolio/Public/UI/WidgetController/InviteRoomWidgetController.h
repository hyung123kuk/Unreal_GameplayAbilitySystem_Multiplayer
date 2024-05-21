// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "InviteRoomWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FInviteRoomWidgetControllerParams
{
	GENERATED_BODY()

	FInviteRoomWidgetControllerParams() {}
	FInviteRoomWidgetControllerParams(const FString& sendUserName, const FString& roomName, const FString& roomPassword) :
		SendUserName(sendUserName),
		RoomName(roomName),
		RoomPassword(roomPassword) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SendUserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomPassword;
};

UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UInviteRoomWidgetController : public UHKWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FInviteRoomWidgetControllerParams& Params);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString SendUserName;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString RoomName;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString RoomPassword;
};

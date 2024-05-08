// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Interaction/UIPageInterface.h"
#include "HKRoomListWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FRoomListWidgetControllerParams
{
	GENERATED_BODY()
	
	FRoomListWidgetControllerParams() {}
	FRoomListWidgetControllerParams(FString& roomName, int maxPlayerNum, int nowPlayerNum) :
		RoomName(roomName),
		MaxPlayerNum(maxPlayerNum),
		NowPlayerNum(nowPlayerNum) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxPlayerNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NowPlayerNum;

};


UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UHKRoomListWidgetController : public UHKWidgetControllerBase, public IUIPageInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FRoomListWidgetControllerParams& Params);

	virtual FString GetElementName() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString RoomName;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	int MaxPlayerNum;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	int NowPlayerNum;
};

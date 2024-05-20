// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Interaction/UIPageInterface.h"
#include "RoomUserInfoWidgetControlle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeRoomUserInfoValueSignature, URoomUserInfoWidgetControlle*, UserInfoController);

USTRUCT(BlueprintType)
struct FRoomUserInfoWidgetControllerParams
{
	GENERATED_BODY()

	FRoomUserInfoWidgetControllerParams() {}
	FRoomUserInfoWidgetControllerParams(FString& playerName, bool roomAdmin, bool isReady, int selectCharacter) :
		PlayerName(playerName),
		RoomAdmin(roomAdmin),
		IsReady(isReady),
		SelectCharacter(selectCharacter)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RoomAdmin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SelectCharacter;
};


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API URoomUserInfoWidgetControlle : public UHKWidgetControllerBase, public IUIPageInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FRoomUserInfoWidgetControllerParams& Params);
	
	virtual FString GetElementName() override;

	UPROPERTY(BlueprintAssignable, Category = "ChangeValue")
	FChangeRoomUserInfoValueSignature ChangeRoomValueSignature;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	bool RoomAdmin;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	bool IsReady;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	int SelectCharacter;

};

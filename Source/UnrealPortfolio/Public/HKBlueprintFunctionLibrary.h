// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.generated.h"

class UInventory;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|Network")
	static void JoinServer(const UObject* WorldContextObject, const FString& InIPAddress, const FString& Id, const FString& Password = TEXT(""));
	
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|Login")
	static bool CheckIDValidity(const FString& ID, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|Login")
	static bool CheckPasswordValidity(const FString& Password, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|Login")
	static bool MatchPasswordAndPasswordConfirm(const FString& Password, const FString& PasswordConfirm, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|CheckStringLen")
	static FString CheckStringLimitLen(const FString& InputString , int LimitLen);

	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|Inventory")
	static UInventory* GetInventory(const UObject* WorldContextObject);

public:
	//** SystemVariable */
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMaxIDLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMinIDLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMinPasswordLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMaxRoomNameLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMaxRoomPasswordLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMaxUserIntroductionLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static int GetMaxChattingLen();
	UFUNCTION(BlueprintCallable, Category = "UHKBlueprintFunctionLibrary|SystemVariable")
	static bool IsWebLoginServer();
	//** SystemVariable End */

};

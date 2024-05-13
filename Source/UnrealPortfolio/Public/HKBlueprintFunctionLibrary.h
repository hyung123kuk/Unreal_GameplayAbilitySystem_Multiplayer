// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.generated.h"

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

};

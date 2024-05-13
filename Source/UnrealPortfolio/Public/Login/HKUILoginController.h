// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Player/HKUIPlayerControllerBase.h"
#include "HKUILoginController.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKUILoginController : public AHKUIPlayerControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CheckIDForSignUp_Server(const FString& ID);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SignUp_Server(const FString& ID, const FString& Password,const FString& PasswordConfirm);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AttempLogin_Server(const FString& ID, const FString& Password);
	
protected:
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage = false, bool Success = false) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||CheckID")
	FMessageSuccessOrNotDelegate CheckIDSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Login")
	FMessageSuccessOrNotDelegate LoginSuccessOrNotDelegate;
};

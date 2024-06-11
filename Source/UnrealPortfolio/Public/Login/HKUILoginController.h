// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Player/HKUIPlayerControllerBase.h"
#include "HKUILoginController.generated.h"

class AHKUILoginController;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKUILoginController : public AHKUIPlayerControllerBase
{
	GENERATED_BODY()
	
	friend class ALoginGameModeConnectedWebAPI;
	
public:
	UFUNCTION(BlueprintCallable)
	void CheckIDForSignUp(const FString& ID);

	UFUNCTION(BlueprintCallable)
	void SignUp(const FString& ID, const FString& Password, const FString& PasswordConfirm);

	UFUNCTION(BlueprintCallable)
	void AttempLogin(const FString& ID, const FString& Password);

	//** To Dedicated Server */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void CheckIDForSignUp_Server(const FString& ID);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SignUp_Server(const FString& ID, const FString& Password,const FString& PasswordConfirm);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AttempLogin_Server(const FString& ID, const FString& Password);
	//** To Dedicated Server End */

protected:
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage = false, bool Success = false) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||CheckID")
	FMessageSuccessOrNotDelegate CheckIDSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||SignUp")
	FMessageSuccessOrNotDelegate SignUpSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Login")
	FMessageSuccessOrNotDelegate LoginSuccessOrNotDelegate;
};

// Copyright Druid Mechanics


#include "HKBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

void UHKBlueprintFunctionLibrary::JoinServer(const UObject* WorldContextObject, const FString& InIPAddress, const FString& Id, const FString& Password)
{
    if (!Password.IsEmpty())
    {
        UGameplayStatics::OpenLevel(WorldContextObject->GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=%s?Id=%s?Password=%s"), *InIPAddress, *Id, *Password));
    }
    else
    {
        UGameplayStatics::OpenLevel(WorldContextObject->GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=%s?Id=%s"), *InIPAddress, *Id));
    }
}

//아이디는 6~12자리 문자
//숫자,영문, 특수문자 @ . - _ 만 사용가능
bool UHKBlueprintFunctionLibrary::CheckIDValidity(const FString& ID, FString& ErrorMessage)
{
    if (ID.Len() < 6 || ID.Len() > 12)
    {
        UE_LOG(ServerLog, Error, TEXT("입력된 아이디(%s)의 문자열 길이가 6~12자리 사이가 아닙니다."), *ID);
        ErrorMessage = FString::Printf(TEXT("문자열 길이는 6~12자리만 가능합니다."));
        return false;
    }

    static const FString AvailableCharacters = TEXT("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@.-_");
    for (int i = 0; i < ID.Len(); i++)
    {
        FString IDOneChar = FString(1, &ID[i]);
        if (!AvailableCharacters.Contains(IDOneChar))
        {
            UE_LOG(ServerLog, Error, TEXT("입력된 아이디(%s)에 사용할 수 없는 문자(%s)가 포함되어 있습니다. "), *ID, *IDOneChar);
            ErrorMessage = FString::Printf(TEXT("아이디에 \"%s\"는 사용할 수 없습니다. "), *IDOneChar);
            return false;
        }
    }
    
    ErrorMessage = TEXT("");
    return true;
}

//비밀번호는 8자리 이상
//영문, 숫자만 사용가능
//영문, 숫자가 함께 있어야 함
bool UHKBlueprintFunctionLibrary::CheckPasswordValidity(const FString& Password, FString& ErrorMessage)
{
    if (Password.Len() < 8)
    {
        UE_LOG(ServerLog, Error, TEXT("입력된 비밀번호의 문자열 길이가 8자리 이상이 아닙니다."));
        ErrorMessage = FString::Printf(TEXT("비밀번호는 8자리 이상만 가능합니다."));
        return false;
    }

    static const FString eng = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    static const FString num = TEXT("0123456789");
    
    bool ContainEng = false;
    bool ContainNum = false;
    for (int i = 0; i < Password.Len(); i++)
    {
        FString PasswordOneChar = FString(1, &Password[i]);
        if (eng.Contains(PasswordOneChar))
        {
            ContainEng = true;
        }
        else if (num.Contains(PasswordOneChar))
        {
            ContainNum = true;
        }
        else //사용가능한 문자가 아님
        {
            UE_LOG(ServerLog, Error, TEXT("입력된 비밀번호에 사용할 수 없는 문자(%s)가 포함되어 있습니다."), *PasswordOneChar);
            ErrorMessage = FString::Printf(TEXT("비밀번호에 \"%s\"는 사용할 수 없습니다."), *PasswordOneChar);
            return false;
        }    
    }

    if (!ContainNum || !ContainEng)
    {
        UE_LOG(ServerLog, Error, TEXT("입력된 비밀번호가 문자와 숫자의 조합으로 이루어져 있지 않습니다."));
        ErrorMessage = FString::Printf(TEXT("비밀번호는 문자와 숫자의 조합으로 이루어져야 합니다."));
        return false;
    }

    ErrorMessage = TEXT("");
    return true;
}

bool UHKBlueprintFunctionLibrary::MatchPasswordAndPasswordConfirm(const FString& Password, const FString& PasswordConfirm, FString& ErrorMessage)
{
    if (!Password.Equals(PasswordConfirm))
    {
        UE_LOG(ServerLog, Error, TEXT("비밀번호와 비밀번호 확인이 일치하지 않습니다."));
        ErrorMessage = FString::Printf(TEXT("비밀번호와 일치하지 않습니다."));
        return false;
    }

    ErrorMessage = TEXT("");
    return true;
}

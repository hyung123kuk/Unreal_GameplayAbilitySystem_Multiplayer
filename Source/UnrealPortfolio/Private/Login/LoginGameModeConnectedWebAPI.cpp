// Copyright Druid Mechanics

#include "Login/LoginGameModeConnectedWebAPI.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Login/HKUILoginController.h"

APlayerController* ALoginGameModeConnectedWebAPI::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	LoginPlayerController = Cast<AHKUILoginController>(PlayerController);
	return PlayerController;
}

void ALoginGameModeConnectedWebAPI::CheckAccountID(const FString& Id)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginGameModeConnectedWebAPI::ResponseOfCheckAccountID);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	FString URL = TEXT("/checkId");
	RequestObj->SetStringField("CheckAccountName", *Id);
	Post(Request, RequestObj, URL);
}

void ALoginGameModeConnectedWebAPI::ResponseOfCheckAccountID(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	bool CheckOK = false;
	FString Message;
	if (bConnectedSuccessfully)
	{
		TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(jsonReader, jsonObj);

		jsonObj->TryGetBoolField(TEXT("CheckOK"), CheckOK);
		jsonObj->TryGetStringField(TEXT("Message"), Message);
	}
	else
	{
		Message = TEXT("서버와 연결이 되지 않았습니다.");
	}


	LoginPlayerController->SendServerMessage_Client(Message, EServerToClientMessageType::CheckID, true, CheckOK);
}

void ALoginGameModeConnectedWebAPI::CreateAccount(const FString& Id, const FString Password)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginGameModeConnectedWebAPI::ResponseOfCreateAccount);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	FString URL = TEXT("/create");
	RequestObj->SetStringField("AccountName", *Id);
	RequestObj->SetStringField("AccountPassword", *Password);
	Post(Request, RequestObj, URL);
}

void ALoginGameModeConnectedWebAPI::ResponseOfCreateAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	bool CreateOK = false;
	FString Message;
	if (bConnectedSuccessfully)
	{
		TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(jsonReader, jsonObj);


		jsonObj->TryGetBoolField(TEXT("CreateOK"), CreateOK);
		jsonObj->TryGetStringField(TEXT("Message"), Message);
	}
	else
	{
		Message = TEXT("서버와 연결이 되지 않았습니다.");
	}

	LoginPlayerController->SendServerMessage_Client(Message, EServerToClientMessageType::SignUp, true, CreateOK);
}

void ALoginGameModeConnectedWebAPI::LoginAccount(const FString& Id, const FString Password)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginGameModeConnectedWebAPI::ResponseOfLoginAccount);

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	FString URL = TEXT("/login");
	RequestObj->SetStringField("AccountName", *Id);
	RequestObj->SetStringField("AccountPassword", *Password);
	Post(Request, RequestObj, URL);
}

void ALoginGameModeConnectedWebAPI::ResponseOfLoginAccount(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	bool LoginOK = false;
	FString Message;
	ServerList.Empty();
	if (bConnectedSuccessfully)
	{
		TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(jsonReader, jsonObj);

		jsonObj->TryGetBoolField(TEXT("LoginOK"), LoginOK);
		jsonObj->TryGetStringField(TEXT("Message"), Message);

		const TArray<TSharedPtr<FJsonValue>>* ServerListJson;	
		if (!jsonObj->TryGetArrayField(TEXT("ServerList"), ServerListJson))
		{
			return;
		}

		for (auto Server : *ServerListJson)
		{
			FString Name;
			FString Ip;
			int CrowdedLevel;
			Server->AsObject()->TryGetStringField(TEXT("Name"), Name);
			Server->AsObject()->TryGetStringField(TEXT("Ip"), Ip);
			Ip = Ip.Replace(TEXT("0.0.0.0"), TEXT("127.0.0.1"));
			Server->AsObject()->TryGetNumberField(TEXT("CrowdedLevel"), CrowdedLevel);

			FServerInfoData ServerInfo;
			ServerInfo.ServerName = Name;
			ServerInfo.IP = Ip;
			ServerInfo.CrowdedLevel = CrowdedLevel;

			ServerList.Add(ServerInfo);
		}
	}
	else
	{
		Message = TEXT("서버와 연결이 되지 않았습니다.");
	}

	LoginPlayerController->SendServerMessage_Client(Message, EServerToClientMessageType::Login, false, LoginOK);
}

void ALoginGameModeConnectedWebAPI::Post(FHttpRequestRef& Request, TSharedRef<FJsonObject>& RequestObj, FString URL)
{
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->SetURL(WebAPIServerURL + URL);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}


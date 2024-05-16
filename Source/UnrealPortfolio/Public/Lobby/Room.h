// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class AHKLobbyPlayerState;
class ULobbyRoomInfoWidgetController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomDestroyDelegate, const FString&, RoomName);

UCLASS()
class UNREALPORTFOLIO_API ARoom : public AActor
{
	GENERATED_BODY()

public:
	FRoomDestroyDelegate RoomDestroyDelegate;
	
protected:
	ARoom();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void InitRoomInfo(const FString& RoomName, const FString& RoomPassword, int RoomMaxPlayer);

	/** Change room information */
	void ChangeRoomAdmin(AHKLobbyPlayerState* NewRoomAdminPlayer);
	bool EnterPlayer(AHKLobbyPlayerState* NewPlayer, const FString& AttempPassword, FString& Message);
	bool ExitPlayer(AHKLobbyPlayerState* ExitPlayer, FString& Message);
	void SendChangedRoomInformationToClients();

	UFUNCTION(NetMulticast, Reliable)
	void NotifyRemoveRoomToClient();

	UFUNCTION()
	void OnRep_JoinPlayers();
	UFUNCTION()
	void OnRep_Name();
	UFUNCTION()
	void OnRep_PublicRoom();
	UFUNCTION()
	void OnRep_MaxPlayer();
	/** End Change room information */
	
public:
	FString GetRoomName() const { return Name; }
	const AHKLobbyPlayerState* GetAdminPlayer() const { return RoomAdminPlayer; }
	TArray<TObjectPtr<AHKLobbyPlayerState>> GetJoinPlayers() const { return JoinPlayers; }
	
	int GetReadyPlayersCount() const;
	int GetAllPlayersCount() const { return JoinPlayers.Num(); }
	bool ReadyAllPlayers() const { return GetAllPlayersCount() == GetReadyPlayersCount(); }

protected:
	UPROPERTY(Replicated)
	TObjectPtr<AHKLobbyPlayerState> RoomAdminPlayer;

	UPROPERTY(ReplicatedUsing = OnRep_JoinPlayers)
	TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers;

	UPROPERTY(ReplicatedUsing = OnRep_Name, BlueprintReadOnly)
	FString Name;
	UPROPERTY(ReplicatedUsing = OnRep_PublicRoom)
	bool bPublicRoom;
	UPROPERTY(ReplicatedUsing = OnRep_MaxPlayer)
	int MaxPlayer;

	FString Password;
private:
	UPROPERTY()
	TObjectPtr<ULobbyRoomInfoWidgetController> RoomInfoWidgetController;

};

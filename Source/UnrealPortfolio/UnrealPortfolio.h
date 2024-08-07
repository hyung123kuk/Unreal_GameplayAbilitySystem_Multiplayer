// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define DatabaseInfoFilePath TEXT("D:\\Contents\\UnrealPortfolio2\\DataBaseAccountInformation.json")
#define DefaultItemsImagePath TEXT("/Game/Assets/ItemImages/")
#define WebAPIServerURL TEXT("https://localhost:5001/api/Login")

DECLARE_LOG_CATEGORY_EXTERN(ServerLog, Log, All);

//
// System Variable
//

static const bool bWebLoginServer = true;

//* Account */
static const int MaxIDLen = 12;
static const int MinIDLen = 6;
static const int MinPasswordLen = 8;
//* Account End */

//* Lobby */
static const int MaxLobbyUserCount = 20;
//* Lobby End*/

//* Room */
static const int MaxRoomNameLen = 12;
static const int MaxRoomPasswordLen = 8;
static const int MaxUserIntroductionLen = 30;
//* Room End */

//* Chat */
static const int MaxChattingLen = 30;
//* Chat End */


//* InGame */
#define CUSTOM_DEPTH_RED 250
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
//* InGame End */
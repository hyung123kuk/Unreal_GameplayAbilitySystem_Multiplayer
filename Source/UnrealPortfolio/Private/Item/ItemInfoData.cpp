// Copyright Druid Mechanics


#include "Item/ItemInfoData.h"

FItemInfomation UItemInfoData::FindItemInfoForItemID(const int Id) const
{
	for (const FItemInfomation& Info : AttributeInformation)
	{
		if (Info.Id == Id)
		{
			return Info;
		}
	}

	FItemInfomation Fail;
	Fail.Id = -1;
	return Fail;
}

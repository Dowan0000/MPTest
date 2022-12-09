#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Pistol UMETA(DisplayName = "Pistol"),
	EIT_Rifle UMETA(DisplayName = "Rifle"),
	EIT_Sniper UMETA(DisplayName = "Sniper"),
	EIT_Bazooka UMETA(DisplayName = "Bazooka"),
};
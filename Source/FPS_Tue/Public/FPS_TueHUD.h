// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPS_TueHUD.generated.h"

UCLASS()
class AFPS_TueHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFPS_TueHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};


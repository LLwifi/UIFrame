// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Panel/UI_PanelBase.h"
#include "UI_RootPanel.generated.h"

/**
 * 
 */
UCLASS()
class UIFRAME_API UUI_RootPanel : public UUI_PanelBase
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

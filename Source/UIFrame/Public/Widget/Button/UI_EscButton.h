// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Button/UI_Button.h"
#include "../../Common/UI_PanelInteract.h"
#include "UI_EscButton.generated.h"

/**
 * 继承自UI_Button，拥有使用预制Button信息的能力
 * 按钮内置了点击按钮调用Esc事件，可以更加方便快捷的搭建UI
 */
UCLASS()
class UIFRAME_API UUI_EscButton : public UUI_Button , public IUI_PanelInteract
{
	GENERATED_BODY()
	
public:
	UUI_EscButton();

	virtual bool IsControlledByEsc_Implementation() { return true; }
	virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual bool IsShow_Implementation() override;
	virtual EUIShowHideType GetCurUIShowHideType_Implementation() override;
	virtual TArray<EUIShowHideType> GetFollowParentShowHideType_Implementation() override;

public:
	UFUNCTION()
		void ClickEscButton();
	
	UFUNCTION()
		UUI_SubSystem* GetUISubSystem();

public:
	UPROPERTY()
		UUI_SubSystem* UISubSystem;
	UPROPERTY()
		EUIShowHideType CurUIShowHideType;
	//Show和Hide要在哪些情况下被调用
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract")
		TArray<EUIShowHideType> FollowParentShowHideType = {EUIShowHideType::Normal};
};

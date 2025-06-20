// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include <Common/UI_PanelInteract.h>
#include "UI_ComponentBase.generated.h"

/**
 * 组件基类
 * 可通过挂载 开关的 功能可以通过组件实现 
 */
UCLASS()
class UIFRAME_API UUI_ComponentBase : public UWidget, public IUI_PanelInteract
{
	GENERATED_BODY()
	
public:
	UUI_ComponentBase();

	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
	virtual UUserWidget* GetParentUI_Implementation() override;
	virtual void SetParentUI_Implementation(UUserWidget* Parent) override;
	virtual bool IsShow_Implementation() override;
	virtual EUIShowHideType GetCurUIShowHideType_Implementation() override;
	virtual TArray<EUIShowHideType> GetFollowParentShowHideType_Implementation() override;
	virtual bool IsFollowParentInteract_Implementation() override;
	
public:
	//父面板
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UIComponent")
		UUserWidget* ParentUI;

	UPROPERTY()
		EUIShowHideType CurUIShowHideType;

	//是否跟随父节点一同调用IUI_PanelInteract相关接口
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "UIComponent")
	bool bIsFollowParentInteract = true;

	//Show和Hide要在哪些情况下被调用
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI_PanelInteract")
		TArray<EUIShowHideType> FollowParentShowHideType = {EUIShowHideType::Normal};
};

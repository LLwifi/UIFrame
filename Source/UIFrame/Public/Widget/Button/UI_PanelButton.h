// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/Button/UI_Button.h"
#include "UI_PanelButton.generated.h"

class UUserWidget;

/**
 * 面板按钮
 * 该按钮存在是为了更加方便快捷的打开、关闭某些UI面板诞生的
 * 
 */
UCLASS()
class UIFRAME_API UUI_PanelButton : public UUI_Button
{
	GENERATED_BODY()
	
public:
	UUI_PanelButton();

public:
	UFUNCTION()
		void ClickPanelButton();

	UFUNCTION()
		UUI_SubSystem* GetUISubSystem();

public:
	UPROPERTY()
		UUI_SubSystem* UISubSystem;

	//<哪个UI，要显示还是隐藏>
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<TSubclassOf<UUserWidget>, bool> ClickChangeUI_Class;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<UUserWidget*, bool> ClickChangeUI_Widget;
};

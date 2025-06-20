// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include <Engine/DataTable.h>
#include "UI_UnifiedSettings.generated.h"

/**
 * 统一设置控件
 * 统一更换面板上某个类型控件的样式
 */
UCLASS()
class UIFRAME_API UUI_UnifiedSettings : public UWidget
{
	GENERATED_BODY()
public:
	UUI_UnifiedSettings();

	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

public:
	UFUNCTION(BlueprintPure)
		UPanelWidget* GetRootWidget();

	//获取全部UI控件
	UFUNCTION()
		void GetAllWidgetFormClass(UPanelWidget* PanelWidget, TSubclassOf<UWidget> Class);

public:
	UPROPERTY(VisibleAnywhere)
		UUI_UnifiedSettings* UI_UnifiedSettings;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<UWidget*> AllWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UWidget> WidgetClass;

	//要使用的预制行
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FDataTableRowHandle UseRow;

};

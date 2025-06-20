// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include <Engine/DataTable.h>
#include "../../Common/UI_StructAndEnum.h"
#include <Common/UI_PrefabInteract.h>
#include "UI_Button.generated.h"

/*
* Button预制结构体
* 需要被Button预制的信息保持到该结构体中
*/
USTRUCT(BlueprintType)
struct FPrefabButtonInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	//Button的风格
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		FButtonStyle WidgetStyle;

	//点击时的鼠标操作类型
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EButtonClickMethod::Type> ClickMethod;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EButtonPressMethod::Type> PressMethod;
};

/**
 * 自定义UIButton
 * 可以通过表格预制Button的格式
 */
UCLASS()
class UIFRAME_API UUI_Button : public UButton, public IUI_PrefabInteract
{
	GENERATED_BODY()
public:
	UUI_Button();
public:
#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

		virtual const FText GetPaletteCategory() override;

		//编辑器下的预制信息操作
		virtual void OperationEvent_Cover();
		virtual void OperationEvent_New(FName RowName);
#endif

public:

	UFUNCTION(BlueprintCallable)
		void SetButtonInfo(FPrefabButtonInfo ButtonInfo);
	UFUNCTION(BlueprintPure)
		FPrefabButtonInfo GetButtonInfo();

public:
	//要使用的预制信息
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FUI_Prefab Prefab;
};

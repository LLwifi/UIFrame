// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "../../Common/UI_StructAndEnum.h"
#include <Common/UI_PrefabInteract.h>
#include "UI_Text.generated.h"

/*
* Button预制结构体
* 需要被Button预制的信息保持到该结构体中
*/
USTRUCT(BlueprintType)
struct FPrefabTextInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	//文字颜色
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FSlateColor ColorAndOpacity;
	//字体相关
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FSlateFontInfo Font;
	//文字删除线
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FSlateBrush StrikeBrush;
	//阴影投影方向
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector2D ShadowOffset;
	//阴影颜色
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FLinearColor ShadowColorAndOpacity;
	//文本四周的留白空间
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMargin Margin;
	//文本每行间隔（按照一行的比例进行计算）
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float LineHeightPercentage = 1.0f;
};

/**
 * 自定义UIText
 * 可以通过表格预制Text的格式
 */
UCLASS()
class UIFRAME_API UUI_Text : public UTextBlock, public IUI_PrefabInteract
{
	GENERATED_BODY()
public:
	UUI_Text();
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual const FText GetPaletteCategory() override;

	//编辑器下的预制信息操作
	virtual void OperationEvent_Cover();
	virtual void OperationEvent_New(FName Name);
#endif
public:
	UFUNCTION(BlueprintCallable)
		void SetTextInfo(FPrefabTextInfo TextInfo);
	UFUNCTION(BlueprintPure)
		FPrefabTextInfo GetTextInfo();
	
	//设置字体风格
	UFUNCTION(BlueprintCallable)
	void SetTextBlockStyle(FTextBlockStyle TextBlockStyle);

	//设置换行
	UFUNCTION(BlueprintCallable)
	void SetWrapTextAt(float TextWrapTextAt);

public:
	//要使用的预制信息
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FUI_Prefab Prefab;
};





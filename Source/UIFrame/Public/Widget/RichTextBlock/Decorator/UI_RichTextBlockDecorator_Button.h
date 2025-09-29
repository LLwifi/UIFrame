// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/RichTextBlock/Decorator/UI_RichTextBlockDecoratorBase.h"
#include "UI_RichTextBlockDecorator_Button.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUI_RTBButtonDecorator_Delegate);

/*富文本按钮装饰器的配置行数据
*/
USTRUCT(Blueprintable, BlueprintType)
struct FUI_RTBButtonDecoratorRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	//按钮的风格配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FButtonStyle ButtonStyle;
	//按钮的字体风格配置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTextBlockStyle TextStyle;
	//按钮字体的Padding
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMargin TextPadding = FMargin(-10.0f,0.0f);

	//是否显示字体下划线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsShowTextUnderline = false;
	//字体下划线图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush TextUnderlineStyle;
};

/**
 * 自定义的富文本按钮
 * 该富文本也可以当【图片】使用
 */
UCLASS()
class UIFRAME_API UUI_RichTextBlockDecorator_Button : public UUI_RichTextBlockDecoratorBase
{
	GENERATED_BODY()

public:
	UUI_RichTextBlockDecorator_Button(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

	FUI_RTBButtonDecoratorRow* FindRow(FName TagOrId, bool bWarnIfMissing);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnClicked();
	virtual void OnClicked_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPressed();
	virtual void OnPressed_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReleased();
	virtual void OnReleased_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHovered();
	virtual void OnHovered_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnUnhovered();
	virtual void OnUnhovered_Implementation();

public:
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=/Script/UIFRAME.UI_RTBButtonDecoratorRow"))
	TObjectPtr<class UDataTable> DT;
};

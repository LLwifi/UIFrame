// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "UI_RichTextBlockDecoratorBase.generated.h"

/**富文本装饰器基类
 * 该类会保存一个富文本的引用方便后续使用
 */
UCLASS()
class UIFRAME_API UUI_RichTextBlockDecoratorBase : public URichTextBlockDecorator
{
	GENERATED_BODY()
	
public:
	//对应的富文本
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UUI_RichTextBlock* RichTextBlock;
};

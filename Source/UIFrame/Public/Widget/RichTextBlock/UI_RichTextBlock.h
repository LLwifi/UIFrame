// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "UI_RichTextBlock.generated.h"

//富文本的内容类型
UENUM(BlueprintType)
enum class EUI_RichContentType :uint8
{
	Text = 0 UMETA(DisplayName = "文本"),
	Icon UMETA(DisplayName = "图标"),
	Button UMETA(DisplayName = "按钮")
};

/*富文本链接的表格信息
*/
USTRUCT(BlueprintType)
struct FUI_RichTextLinkRowName
{
	GENERATED_BODY()

public:
	//内容类型
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUI_RichContentType RichContentType = EUI_RichContentType::Text;
	//富文本行名称
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RowName;
};

/*富文本的内容
*/
USTRUCT(BlueprintType)
struct FUI_RichTextContent
{
	GENERATED_BODY()

public:
	//内容类型
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EUI_RichContentType RichContentType = EUI_RichContentType::Text;
	//富文本行名称
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RowName;

	//文本信息
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MultiLine = "true"))
	FText Text;

	//换行数
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 LineBreakNum = 0;
};

/**富文本控件
 * 使用该组件可以更方便的设置富文本的内容（不必关心富文本格式）
 */
UCLASS()
class UIFRAME_API UUI_RichTextBlock : public URichTextBlock
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	virtual void UpdateStyleData() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetRichTextContent(FUI_RichTextContent Content);

	//添加富文本内容
	UFUNCTION(BlueprintCallable)
	void AppendRichTextContent(FUI_RichTextContent Content);
	//添加文本内容
	UFUNCTION(BlueprintCallable)
	void AppendTextContent(FName RowName, FText TextContent, int32 LineBreakNum = 0);
	//添加图标内容
	UFUNCTION(BlueprintCallable)
	void AppendIconContent(FName RowName, int32 LineBreakNum = 0);
	//添加按钮内容
	UFUNCTION(BlueprintCallable)
	void AppendButtonContent(FName RowName, FText TextContent, int32 LineBreakNum = 0);
};

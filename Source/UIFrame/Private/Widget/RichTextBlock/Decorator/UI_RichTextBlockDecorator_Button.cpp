// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/RichTextBlock/Decorator/UI_RichTextBlockDecorator_Button.h"

#define LOCTEXT_NAMESPACE "UI_RichTextBlockDecorator_Button"

/*富文本替换后真正创建/使用的Slate类
*/
class SRichInlineButton : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineButton)
		{}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const FUI_RTBButtonDecoratorRow* RTBStyle, UUI_RichTextBlockDecorator_Button* Decorator, FText TextContent)
	{
		RTBButtonStyle = *RTBStyle;
		ButtonDecorator = Decorator;
		ChildSlot
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SBox)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(SButton)
				.ContentPadding(RTBButtonStyle.TextPadding)
				.ButtonStyle(&RTBButtonStyle.ButtonStyle)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock).Text(TextContent).TextStyle(&RTBButtonStyle.TextStyle)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(UnderlineImage, SImage)
						.Image(&RTBButtonStyle.TextUnderlineStyle).Visibility(EVisibility::Collapsed)
					]
				]
				.OnClicked_Lambda([this]()
				{
					if (ButtonDecorator)
					{
						ButtonDecorator->OnClicked();
					}
					return FReply::Handled();
				})
				.OnPressed_Lambda([this]()
				{
					if (ButtonDecorator)
					{
						ButtonDecorator->OnPressed();
					}
					if (RTBButtonStyle.bIsShowTextUnderline)
					{
						UnderlineImage->SetVisibility(EVisibility::Collapsed);
					}
				})
				.OnReleased_Lambda([this]()
				{
					if (ButtonDecorator)
					{
						ButtonDecorator->OnReleased();
					}
					if (RTBButtonStyle.bIsShowTextUnderline)
					{
						UnderlineImage->SetVisibility(EVisibility::Visible);
					}
				})
				.OnHovered_Lambda([this]()
				{
					if (ButtonDecorator)
					{
						ButtonDecorator->OnHovered();
					}
					if (RTBButtonStyle.bIsShowTextUnderline)
					{
						UnderlineImage->SetVisibility(EVisibility::Visible);
					}
				})
				.OnUnhovered_Lambda([this]()
				{
					if (ButtonDecorator)
					{
						ButtonDecorator->OnUnhovered();
					}
					if (RTBButtonStyle.bIsShowTextUnderline)
					{
						UnderlineImage->SetVisibility(EVisibility::Collapsed);
					}
				})
			]
		];
	}

public:
	FUI_RTBButtonDecoratorRow RTBButtonStyle;
	UUI_RichTextBlockDecorator_Button* ButtonDecorator;
	TSharedPtr<SImage> UnderlineImage;
};

/*捕获和创建Slate的中间类
* Supports : 用于捕获信息
* CreateDecoratorWidget ： 创建并返回slate
*/
class FUI_RichInlineButton : public FRichTextDecorator
{
public:
	FUI_RichInlineButton(URichTextBlock* InOwner, UUI_RichTextBlockDecorator_Button* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("button") && RunParseResult.MetaData.Contains(TEXT("id")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
			const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

			const bool bWarnIfMissing = false;
			//是否在表格中找到了对应的数据
			return Decorator->FindRow(*TagId, bWarnIfMissing) != nullptr;
		}

		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		const bool bWarnIfMissing = true;
		const FUI_RTBButtonDecoratorRow* RTBStyle = Decorator->FindRow(*RunInfo.MetaData[TEXT("id")], bWarnIfMissing);

		if (ensure(RTBStyle))
		{
			FText TextContent;
			if (const FString* textString = RunInfo.MetaData.Find(TEXT("text")))
			{
				TextContent = FText::FromString(*textString);
			}

			return SNew(SRichInlineButton, RTBStyle, Decorator, TextContent);
		}
		return TSharedPtr<SWidget>();
	}

private:
	UUI_RichTextBlockDecorator_Button* Decorator;
};

//---------------------------------------------------------------------------------------------------

UUI_RichTextBlockDecorator_Button::UUI_RichTextBlockDecorator_Button(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

TSharedPtr<ITextDecorator> UUI_RichTextBlockDecorator_Button::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FUI_RichInlineButton(InOwner, this));
}

FUI_RTBButtonDecoratorRow* UUI_RichTextBlockDecorator_Button::FindRow(FName TagOrId, bool bWarnIfMissing)
{
	if (DT)
	{
		FString ContextString;
		return DT->FindRow<FUI_RTBButtonDecoratorRow>(TagOrId, ContextString, bWarnIfMissing);
	}

	return nullptr;
}

void UUI_RichTextBlockDecorator_Button::OnClicked_Implementation()
{
}

void UUI_RichTextBlockDecorator_Button::OnPressed_Implementation()
{
}

void UUI_RichTextBlockDecorator_Button::OnReleased_Implementation()
{
}

void UUI_RichTextBlockDecorator_Button::OnHovered_Implementation()
{
}

void UUI_RichTextBlockDecorator_Button::OnUnhovered_Implementation()
{
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/RichTextBlock/UI_RichTextBlock.h"
#include <Widget/RichTextBlock/Decorator/UI_RichTextBlockDecoratorBase.h>

#if WITH_EDITOR

const FText UUI_RichTextBlock::GetPaletteCategory()
{
	return FText::FromString("UI_Widget");
}

#endif

void UUI_RichTextBlock::UpdateStyleData()
{
	Super::UpdateStyleData();//Super中会动态创建Decorator（装饰器）
	for (TObjectPtr<URichTextBlockDecorator>& Decorator : InstanceDecorators)
	{
		UUI_RichTextBlockDecoratorBase* RichTextBlockDecoratorBase = Cast<UUI_RichTextBlockDecoratorBase>(Decorator);
		if (RichTextBlockDecoratorBase)
		{
			RichTextBlockDecoratorBase->RichTextBlock = this;
		}
	}
}

void UUI_RichTextBlock::SetRichTextContent(FUI_RichTextContent Content)
{
	//设置时将之前的内容先设置为空然后再重新设置内容
	SetText(FText());
	AppendRichTextContent(Content);
}

void UUI_RichTextBlock::AppendRichTextContent(FUI_RichTextContent Content)
{
	switch (Content.RichContentType)
	{
	case EUI_RichContentType::Text:
	{
		AppendTextContent(Content.RowName, Content.Text, Content.LineBreakNum);
		break;
	}
	case EUI_RichContentType::Icon:
	{
		AppendIconContent(Content.RowName, Content.LineBreakNum);
		break;
	}
	case EUI_RichContentType::Button:
	{
		AppendButtonContent(Content.RowName, Content.Text, Content.LineBreakNum);
		break;
	}
	default:
		break;
	}
}

void UUI_RichTextBlock::AppendTextContent(FName RowName, FText TextContent, int32 LineBreakNum /*= 0*/)
{
	FString Content = GetText().ToString();
	Content.Append("<" + RowName.ToString() + ">" + TextContent.ToString() + "</>");
	for (int32 i = 0; i < LineBreakNum; i++)
	{
		Content.Append("\n");
	}
	SetText(FText::FromString(Content));
}

void UUI_RichTextBlock::AppendIconContent(FName RowName, int32 LineBreakNum /*= 0*/)
{
	FString Content = GetText().ToString();
	Content.Append("<img id=\"" + RowName.ToString() + "\"/>");
	for (int32 i = 0; i < LineBreakNum; i++)
	{
		Content.Append("\n");
	}
	SetText(FText::FromString(Content));
}

void UUI_RichTextBlock::AppendButtonContent(FName RowName, FText TextContent, int32 LineBreakNum /*= 0*/)
{
	FString Content = GetText().ToString();
	FString TextString = "\" text=\"" + TextContent.ToString() + "\"/>";
	Content.Append("<button id=\"" + RowName.ToString() + (TextContent.IsEmpty() ? "\"/>" : TextString));
	for (int32 i = 0; i < LineBreakNum; i++)
	{
		Content.Append("\n");
	}
	SetText(FText::FromString(Content));
}


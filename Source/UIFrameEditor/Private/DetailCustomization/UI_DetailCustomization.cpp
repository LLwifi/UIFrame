#include "DetailCustomization/UI_DetailCustomization.h"
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>


TSharedRef<IDetailCustomization> IUI_DetailCustomization::MakeInstance()
{
	return MakeShareable(new IUI_DetailCustomization);
}

void IUI_DetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//IDetailCategoryBuilder& DetailCategoryBuilder = DetailBuilder.EditCategory("Operation");
	////搜索词条
	//DetailCategoryBuilder.AddCustomRow(FText::FromString("Operation"))
	//	.NameContent()
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString("bbbbbb"))
	//	]
	//	.ValueContent()
	//	[
	//		SNew(SButton)
	//		.Text(FText::FromString("rrrrrrrrrrr"))
	//	];
}
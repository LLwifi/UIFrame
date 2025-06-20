#include "../../Public/RedPointEditor/RedPointTreeNodeSlate.h"
#include "Widgets/Input/SButton.h"
#include "../../Public/RedPointEditor/RedPointTreeNode.h"
#include <Blueprint/UserWidget.h>
#include <RedPointTree/RedPointTree.h>


void SRedPointTreeNodeSlate::Construct(const FArguments& InArgs, URedPointTreeBaseNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
}

TSharedRef<SWidget> SRedPointTreeNodeSlate::CreateNodeContentArea()
{
	FText UITag, UIClassName, NodeClassName;
	URedPointTreeNode* RedPointNode = (URedPointTreeNode*)GraphNode;
	if (RedPointNode)
	{
		RedPointNode->NodeSlate = this;

		if (RedPointNode->RedPointNodeInfo.RedPointTreeNodeType != ERedPointTreeNodeType::Root)
		{
			RedPointNode->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Mid;
		}

		if (RedPointNode->RedPointNodeInfo.UIClass)
		{
			UITag = FText::FromString(RedPointNode->RedPointNodeInfo.UITag.ToString());
			UIClassName = FText::FromString(RedPointNode->RedPointNodeInfo.UIClass->GetName());
		}
		else
		{
			UIClassName = FText::FromString(TEXT("未填入UI类"));
		}

		if (RedPointNode->RedPointNodeInfo.NodeClass)
		{
			NodeClassName = FText::FromString(RedPointNode->RedPointNodeInfo.NodeClass->GetName());
		}
		else
		{
			NodeClassName = FText::FromString(TEXT("未填入叶子节点事件类"));
		}
	}

	URedPointTreeLeafNode* LeafNode = Cast<URedPointTreeLeafNode>(RedPointNode);
	if (LeafNode)
	{
		LeafNode->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Leaf;
		LeafNode->TitleColor = FLinearColor::Green;
	}

	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		//.BorderImage(FEditorStyle::GetBrush("NoBorder"))//UE4
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(0, 3))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			[
				// LEFT
				SAssignNew(LeftNodeBox, SVerticalBox)
			]
			//********自定义的内容
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.0f)
			.Padding(10.0f, 0.0f, 10.0f, 0.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					.Padding(0.0f, 0.0f, 5.0f, 0.0f)
					[
						SNew(SBorder)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(5.0f))
						[
							SAssignNew(TextBlock, STextBlock)
						]
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(SBorder)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(5.0f))
						[
							SNew(STextBlock)
							.Text(NodeClassName)
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					.Padding(0.0f, 0.0f, 5.0f, 0.0f)
					[
						SNew(SBorder)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(5.0f))
						[
							SNew(STextBlock)
							.Text(UITag)
						]
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(SBorder)
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(FMargin(5.0f))
						[
							SNew(STextBlock)
							.Text(UIClassName)
						]
					]
				]
			]
			//**********************
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				// RIGHT
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		];
}

void SRedPointTreeNodeSlate::SetRedPointNodeShowInfo(FText Text)
{
	if (TextBlock)
	{
		TextBlock->SetText(Text);
	}
}

//void SRedPointTreeNodeSlate::UpdateGraphNode()
//{
//	RightNodeBox.Reset();
//	LeftNodeBox.Reset();
//
//	URedPointTreeNode* RedPointTreeNode = (URedPointTreeNode*)GraphNode;
//
//	//控件界面：
//	GetOrAddSlot(ENodeZone::Center)
//		.HAlign(HAlign_Center)
//		.VAlign(VAlign_Center)
//		[
//			SNew(SVerticalBox)
//
//			+ SVerticalBox::Slot()
//			.AutoHeight()
//			[
//				SNew(SBorder)
//				.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
//				.Padding(0)
//				.ColorAndOpacity(RedPointTreeNode->TextColor)
//				.BorderBackgroundColor(RedPointTreeNode->NodeColor)
//				[
//					SNew(SVerticalBox)
//					+ SVerticalBox::Slot()
//					.AutoHeight()
//					.HAlign(HAlign_Fill)
//					.VAlign(VAlign_Top)
//					[
//						// NODE CONTENT AREA
//						SNew(SBorder)
//						.BorderImage(FEditorStyle::GetBrush("NoBorder"))
//						.HAlign(HAlign_Fill)
//						.VAlign(VAlign_Fill)
//						.Padding(FMargin(0, 3))
//						[
//							SNew(SHorizontalBox)
//							+ SHorizontalBox::Slot()
//							.AutoWidth()
//							.VAlign(VAlign_Center)
//							[
//								// LEFT
//								SNew(SBox)
//								.WidthOverride(40)
//								[
//									SAssignNew(LeftNodeBox, SVerticalBox)
//								]
//							]
//							+ SHorizontalBox::Slot()
//							.VAlign(VAlign_Center)
//							.HAlign(HAlign_Center)
//							.FillWidth(1.0f)
//							[
//								SNew(STextBlock).Text(((URedPointTreeBaseNode*)GraphNode)->NodeName)
//							]
//							+ SHorizontalBox::Slot()
//							.AutoWidth()
//							.VAlign(VAlign_Center)
//							[
//								// RIGHT
//								SNew(SBox)
//								.WidthOverride(40)
//								[
//									SAssignNew(RightNodeBox, SVerticalBox)
//								]
//							]
//						]
//					]
//				]
//			]
//		];
//
//
//	//向RightNodeBox和LeftNodeBox加入节点控件
//	CreatePinWidgets();
//}
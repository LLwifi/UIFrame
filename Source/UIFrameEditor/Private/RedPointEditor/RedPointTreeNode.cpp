// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/RedPointEditor/RedPointTreeNode.h"
#include "Blueprint/UserWidget.h"

const FName URedPointTreeBaseNode::OurGraphNodePinCategory(TEXT("OurGraphNodePinCategory"));
const FName URedPointTreeBaseNode::OurGraphNodePinSubCategory(TEXT("OurGraphNodePinSubCategory"));

#define LOCTEXT_NAMESPACE "RedPointTreeNode"

URedPointTreeNode::URedPointTreeNode()
{
	RedPointNodeInfo.NodeClass = URedPointNode::StaticClass();
}

void URedPointTreeNode::AllocateDefaultPins()
{
	switch (RedPointNodeInfo.RedPointTreeNodeType)
	{
	case ERedPointTreeNodeType::None:
		break;
	case ERedPointTreeNodeType::Root:
		OutputPin = CreatePin(EGPD_Output, OurGraphNodePinCategory, OurGraphNodePinSubCategory, nullptr, TEXT(""));
		break;
	case ERedPointTreeNodeType::Mid:
		InputPin = CreatePin(EGPD_Input, OurGraphNodePinCategory, OurGraphNodePinSubCategory, nullptr, TEXT(""));
		OutputPin = CreatePin(EGPD_Output, OurGraphNodePinCategory, OurGraphNodePinSubCategory, nullptr, TEXT(""));
		break;
	case ERedPointTreeNodeType::Leaf:
		InputPin = CreatePin(EGPD_Input, OurGraphNodePinCategory, OurGraphNodePinSubCategory, nullptr, TEXT(""));
		break;
	default:
		break;
	}
}

FText URedPointTreeNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(RedPointNodeInfo.NodeTag.ToString());
}

FText URedPointTreeNode::GetTooltipText() const
{
	switch (RedPointNodeInfo.RedPointTreeNodeType)
	{
	case ERedPointTreeNodeType::None:
		break;
	case ERedPointTreeNodeType::Root:
		return NSLOCTEXT("URedPointTreeNode", "Our Graph Node Tooltip", "根节点");
	case ERedPointTreeNodeType::Mid:
		return NSLOCTEXT("URedPointTreeNode", "Our Graph Node Tooltip", "中间节点");
	case ERedPointTreeNodeType::Leaf:
		return NSLOCTEXT("URedPointTreeNode", "Our Graph Node Tooltip", "叶子节点");
	default:
		break;
	}
	return NSLOCTEXT("URedPointTreeNode", "Our Graph Node Tooltip", "节点类型不对");
}

FLinearColor URedPointTreeNode::GetNodeTitleColor() const
{
	if (NodeSlate)
	{
		if (RedPointTree)
		{
			URedPointNode* Node = RedPointTree->GetNodeFromName(RedPointNodeInfo.NodeTag);
			if (Node)
			{
				FString Tip = FString::FromInt(Node->GetDisplayCount());

				NodeSlate->SetRedPointNodeShowInfo(FText::FromString(Tip));
			}
		}
		else
		{
			NodeSlate->SetRedPointNodeShowInfo(FText::FromString(""));
		}

	}

	return TitleColor;
}

bool URedPointTreeNode::CanUserDeleteNode() const
{
	return RedPointNodeInfo.RedPointTreeNodeType != ERedPointTreeNodeType::Root;
}

void URedPointTreeNode::AddChildNode(URedPointTreeNode* Node)
{
	if (Node)
	{
		ChildNode.AddUnique(Node);
		RedPointNodeInfo.ChildNodeTag.AddUnique(Node->RedPointNodeInfo.NodeTag);
	}
}

void URedPointTreeNode::AddParentNode(URedPointTreeNode* Node)
{
	if (Node)
	{
		ParentNode.AddUnique(Node);
		RedPointNodeInfo.ParentNodeTag.AddUnique(Node->RedPointNodeInfo.NodeTag);
	}
}

void URedPointTreeNode::RemoveChildNode(URedPointTreeNode* Node)
{
	if (Node)
	{
		ChildNode.Remove(Node);
		RedPointNodeInfo.ChildNodeTag.Remove(Node->RedPointNodeInfo.NodeTag);
	}
}

void URedPointTreeNode::RemoveParentNode(URedPointTreeNode* Node)
{
	if (Node)
	{
		ParentNode.Remove(Node);
		RedPointNodeInfo.ParentNodeTag.Remove(Node->RedPointNodeInfo.NodeTag);
	}
}

void URedPointTreeNode::ClearParentNode()
{
	ParentNode.Empty();
	RedPointNodeInfo.ParentNodeTag.Empty();
}

void URedPointTreeNode::ClearChildNode()
{
	ChildNode.Empty();
	RedPointNodeInfo.ChildNodeTag.Empty();
}

void URedPointTreeNode::UpdateParentAndChildNodeTag()
{
	UEdGraphPin* IPin = nullptr;
	UEdGraphPin* OPin = nullptr;
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			OPin = Pin;
		}
		else if (Pin->Direction == EGPD_Input)
		{
			IPin = Pin;
		}
	}

	if (IPin)
	{
		ClearParentNode();
		for (UEdGraphPin* Pin : IPin->LinkedTo)
		{
			AddParentNode(Cast<URedPointTreeNode>(Pin->GetOwningNode()));
		}
	}
	if (OPin)
	{
		ClearChildNode();
		for (UEdGraphPin* Pin : OPin->LinkedTo)
		{
			AddChildNode(Cast<URedPointTreeNode>(Pin->GetOwningNode()));
		}
	}
}

bool URedPointTreeNode::IsLink()
{
	return IsLinkRoot(this) || RedPointNodeInfo.RedPointTreeNodeType == ERedPointTreeNodeType::Root;
}

bool URedPointTreeNode::IsLinkRoot(URedPointTreeNode* Node)
{
	for (URedPointTreeNode* RedPointTreeNode : Node->ParentNode)
	{
		if (RedPointTreeNode->RedPointNodeInfo.RedPointTreeNodeType == ERedPointTreeNodeType::Root || IsLinkRoot(RedPointTreeNode))
		{
			return true;
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE

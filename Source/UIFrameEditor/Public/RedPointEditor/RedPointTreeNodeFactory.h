// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "RedPointTreeNode.h"
#include "RedPointTreeNodeSlate.h"


//����ͼ��ڵ㹤��
class FRedPointTreeNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (URedPointTreeNode* RedPointTreeNode = Cast<URedPointTreeNode>(Node))
		{
			return SNew(SRedPointTreeNodeSlate, RedPointTreeNode);
		}

		return nullptr;
	}
};

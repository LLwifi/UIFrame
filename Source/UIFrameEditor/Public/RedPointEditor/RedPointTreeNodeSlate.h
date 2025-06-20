// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SGraphNode.h"

class URedPointTreeBaseNode;

class SRedPointTreeNodeSlate : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SRedPointTreeNodeSlate) {}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, URedPointTreeBaseNode* InNode);

	// SGraphNode implementation
	//virtual void UpdateGraphNode() override;

	//节点下半部分的外观
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	// End SGraphNode implementation

	void SetRedPointNodeShowInfo(FText Text);

public:
	TSharedPtr<STextBlock> TextBlock;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <EdGraph/EdGraphNode.h>
#include <RedPointTree/RedPointTree.h>
#include "RedPointTreeNodeSlate.h"
#include "RedPointTreeNode.generated.h"

class URedPointNode;



UCLASS(Abstract)
class URedPointTreeBaseNode : public UEdGraphNode
{
	GENERATED_BODY()
protected:

	static const FName OurGraphNodePinCategory;
	static const FName OurGraphNodePinSubCategory;

public:

	/** Begin UEdGraphNode Interface */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return NSLOCTEXT("URedPointTreeNode", "ActionName", "节点基类"); }
	virtual FLinearColor GetNodeTitleColor() const override {  return TitleColor; }
	/** End UEdGraphNode Interface */

	virtual FText GetActionName() const { return NSLOCTEXT("URedPointTreeNode", "ActionName", "节点基类"); }

public:

	UPROPERTY(EditAnywhere)
		FLinearColor TitleColor = FLinearColor::White;

	SRedPointTreeNodeSlate* NodeSlate;
	URedPointTree* RedPointTree;
};

//图表节点
UCLASS()
class URedPointTreeNode : public URedPointTreeBaseNode
{
	GENERATED_BODY()

public:
	URedPointTreeNode();

	/** Begin UEdGraphNode Interface */
	// 分配默认的引脚
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// Tooltip内容
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	/** End UEdGraphNode Interface */

	virtual bool CanUserDeleteNode() const override;

	void AddChildNode(URedPointTreeNode* Node);
	void AddParentNode(URedPointTreeNode* Node);
	void RemoveChildNode(URedPointTreeNode* Node);
	void RemoveParentNode(URedPointTreeNode* Node);
	void ClearParentNode();
	void ClearChildNode();

	void UpdateParentAndChildNodeTag();

	bool IsLink();

	bool IsLinkRoot(URedPointTreeNode* Node);

public:
	virtual FText GetActionName() const override { return NSLOCTEXT("URedPointTreeNode", "ActionName_Node", "Node"); }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = false))
		TArray<URedPointTreeNode*> ParentNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = false))
		TArray<URedPointTreeNode*> ChildNode;

	//输入引脚
	UEdGraphPin* InputPin;
	//输出引脚
	UEdGraphPin* OutputPin;

	//红点信息--使用该信息创建真正游戏运行的UObject
	UPROPERTY(EditAnywhere)
		FRedPointNodeInfo RedPointNodeInfo;
};

//叶子节点
UCLASS()
class URedPointTreeLeafNode : public URedPointTreeNode
{
	GENERATED_BODY()

public:
	virtual FText GetActionName() const override { return NSLOCTEXT("URedPointTreeNode", "ActionName_LeafNode", "LeafNode"); }
};
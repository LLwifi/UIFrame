#pragma once

#pragma once

#include "CoreTypes.h"
#include "EdGraph/EdGraphSchema.h"
#include "RedPointTreeGraphSchema.generated.h"

UCLASS()
class URedPointTreeGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
protected:
	static const FName OurNodeContextMenuSection;
	static const FName OurPinContextMenuSection;

private:
	// 右键空白处或者拖拽引脚调用函数
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	// 右键一个节点或者引脚出现的函数
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	// 判断是否可以创建引脚链接
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	// 尝试创建引脚链接
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;

	// 断开从/到单个节点的所有链接
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	// 断开从/到单个接点的所有链接
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	// 断开两个节点之间的链接
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;

public:
};

//测试打印
USTRUCT()
struct FPrintHelloGraphSchemaAction : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY()

public:

	FPrintHelloGraphSchemaAction() :FEdGraphSchemaAction(
		NSLOCTEXT("RedPointTreeGraphSchema", "Special Actions Category", "特殊操作菜单"),/*分组*/
		NSLOCTEXT("RedPointTreeGraphSchema", "Print Hello Action", "打印Hello World"),/*按钮内容*/
		NSLOCTEXT("RedPointTreeGraphSchema", "Print Hello Tooltip", "打印Hello World的提示"),/*鼠标移入的提示*/
		0
	) {}

	// 核心函数
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

USTRUCT()
struct FOurGraphSchemaAction_NewNode : public FEdGraphSchemaAction_NewNode
{
	GENERATED_USTRUCT_BODY()
public:

	UClass* NodeClass;

	FOurGraphSchemaAction_NewNode() :FEdGraphSchemaAction_NewNode(
		NSLOCTEXT("EditorExtension", "Create Node Category", "创建节点菜单"),
		NSLOCTEXT("EditorExtension", "Create Out Node Action", "Node"),
		NSLOCTEXT("EditorExtension", "Create Out Node Tooltip", "Node"),
		0
	), NodeClass(nullptr) {}

	void SetNodeClass(UClass* InNodeClass);
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

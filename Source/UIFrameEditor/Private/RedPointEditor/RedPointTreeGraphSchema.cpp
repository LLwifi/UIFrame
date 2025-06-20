#include "../../Public/RedPointEditor/RedPointTreeGraphSchema.h"
#include <Kismet2/BlueprintEditorUtils.h>
#include <ToolMenu.h>
#include <ToolMenuSection.h>
#include <Framework/Commands/GenericCommands.h>
#include "../../Public/RedPointEditor/RedPointTreeNode.h"
#include <GraphEditorActions.h>
#include <Blueprint/UserWidget.h>

#define LOCTEXT_NAMESPACE "RedPointTreeGraphSchema"

const FName URedPointTreeGraphSchema::OurNodeContextMenuSection(TEXT("OurNodeContextMenuSection"));
const FName URedPointTreeGraphSchema::OurPinContextMenuSection(TEXT("OurPinContextMenuSection"));

void URedPointTreeGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const TSharedPtr<FPrintHelloGraphSchemaAction> Action = MakeShareable(new FPrintHelloGraphSchemaAction);
	ContextMenuBuilder.AddAction(Action);

	const UClass* BaseNodeClass = URedPointTreeBaseNode::StaticClass();
	TArray<UClass*> NodeClasses;
	GetDerivedClasses(BaseNodeClass, NodeClasses);//获取该class全部子类

	// 因为已经有NodeClass变量了，所以这里命名为NodeClassIterator
	for (const auto& NodeClassIterator : NodeClasses)
	{
		const TSharedPtr<FOurGraphSchemaAction_NewNode> CreateNodeAction = MakeShareable(new FOurGraphSchemaAction_NewNode);
		const URedPointTreeBaseNode* NodeCDO = Cast<URedPointTreeBaseNode>(NodeClassIterator->ClassDefaultObject);
		CreateNodeAction->UpdateSearchData(NodeCDO->GetActionName(), FText(),
			NSLOCTEXT("EditionExtension", "Create Node Category", "创建新节点"), FText());
		CreateNodeAction->SetNodeClass(NodeClassIterator);
		ContextMenuBuilder.AddAction(CreateNodeAction);
	}

	Super::GetGraphContextActions(ContextMenuBuilder);
}

void URedPointTreeGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	//注意区分Node和Pin，复制一个Pin没有什么意义
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection(OurNodeContextMenuSection, NSLOCTEXT("RedPointTreeGraphSchema", "Our Node Context Menu Section Name", "Node"));
		//Section.AddMenuEntry(FGenericCommands::Get().Copy);
		//Section.AddMenuEntry(FGenericCommands::Get().Cut);
		//Section.AddMenuEntry(FGenericCommands::Get().Paste);
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		URedPointTreeNode* RedPointTreeNode = (URedPointTreeNode*)Context->Node;
		if (RedPointTreeNode->RedPointNodeInfo.RedPointTreeNodeType == ERedPointTreeNodeType::Leaf)
		{
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Paste);
		}
	}
	if (Context->Pin)
	{
		FToolMenuSection& Section = Menu->AddSection(OurPinContextMenuSection, NSLOCTEXT("RedPointTreeGraphSchema", "Our Node Pin Menu Section Name", "Pin"));//我们的引脚操作
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
	}
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse URedPointTreeGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A != B && A->GetOwningNode() != B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("OK!"));
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("无法与自身相连"));
}

bool URedPointTreeGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	bool ReturnBool = Super::TryCreateConnection(A, B);

	if (ReturnBool)
	{
		URedPointTreeNode* ParentNode;
		URedPointTreeNode* ChildNode;
		if (A->Direction == EGPD_Output)//判断这个Pin是输入还是输出
		{
			ParentNode = Cast<URedPointTreeNode>(A->GetOwningNode());
			ChildNode = Cast<URedPointTreeNode>(B->GetOwningNode());
		}
		else
		{
			ChildNode = Cast<URedPointTreeNode>(A->GetOwningNode());
			ParentNode = Cast<URedPointTreeNode>(B->GetOwningNode());
		}
		ChildNode->UpdateParentAndChildNodeTag();
		ParentNode->UpdateParentAndChildNodeTag();
		//ParentNode->AddChildNode(ChildNode);
		//ChildNode->AddParentNode(ParentNode);
	}

	return ReturnBool;
}

void URedPointTreeGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	UE_LOG(LogTemp, Warning, TEXT("Invalid mesh passed, Nothing will be added to TrackedMeshes"));
}

void URedPointTreeGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	URedPointTreeNode* ParentNode;
	URedPointTreeNode* ChildNode;
	if (TargetPin.Direction == EGPD_Output)//判断这个Pin是输入还是输出
	{
		ParentNode = Cast<URedPointTreeNode>(TargetPin.GetOwningNode());
		ChildNode = Cast<URedPointTreeNode>(TargetPin.LinkedTo[0]->GetOwningNode());
	}
	else
	{
		ChildNode = Cast<URedPointTreeNode>(TargetPin.GetOwningNode());
		ParentNode = Cast<URedPointTreeNode>(TargetPin.LinkedTo[0]->GetOwningNode());
	}
	//ParentNode->RemoveChildNode(ChildNode);
	//ChildNode->RemoveParentNode(ParentNode);
	Super::BreakPinLinks(TargetPin,bSendsNodeNotifcation);
	//相互移除
	ChildNode->UpdateParentAndChildNodeTag();
	ParentNode->UpdateParentAndChildNodeTag();
}

void URedPointTreeGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	UE_LOG(LogTemp, Warning, TEXT("Invalid mesh passed, Nothing will be added to TrackedMeshes"));
}

#undef LOCTEXT_NAMESPACE

UEdGraphNode* FPrintHelloGraphSchemaAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (FromPin)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("EditorExtension", "Print Action With Pin", "Hello World，并且带了个节点"));
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("EditorExtension", "Print Action Without Pin", "Hello World"));
	}
	return nullptr;
}

void FOurGraphSchemaAction_NewNode::SetNodeClass(UClass* InNodeClass)
{
	NodeClass = InNodeClass;
}

UEdGraphNode* FOurGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	URedPointTreeNode* GraphNodeTemplate = Cast<URedPointTreeNode>(NodeClass->ClassDefaultObject);
	if (GraphNodeTemplate->IsA(URedPointTreeLeafNode::StaticClass()))
	{
		GraphNodeTemplate->TitleColor = FLinearColor::Green;
		GraphNodeTemplate->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Leaf;
	}
	else
	{
		GraphNodeTemplate->RedPointNodeInfo.RedPointTreeNodeType = ERedPointTreeNodeType::Mid;
	}
	
	UEdGraphNode* NewGraphNode = CreateNode(ParentGraph, FromPin, Location, GraphNodeTemplate);

	const UEdGraphSchema* GraphSchema = ParentGraph->GetSchema();
	//如果不判空的话，无前导节点的创建就会导致引擎崩溃
	if (FromPin)
	{
		for (const auto& Pin : NewGraphNode->Pins)
		{
			if (GraphSchema->TryCreateConnection(FromPin, Pin))
			{
				break;
			}
		}
	}
	return nullptr;
}

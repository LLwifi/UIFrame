// Fill out your copyright notice in the Description page of Project Settings.


#include <RedPointTree/RedPointTree.h>
#include <UI_SubSystem.h>
#include <Subsystems/SubsystemBlueprintLibrary.h>
#include <Kismet/GameplayStatics.h>


URedPointNode::URedPointNode()
{

}

UWorld* URedPointNode::GetWorld() const
{
	//不使用GetOuter()获取，可以使该函数通过重写GetWorld的判定
	UObject *outer = GetOuter();
	if (outer && outer->IsA<AActor>() && !outer->HasAnyFlags(RF_ClassDefaultObject))
	{
		return outer->GetWorld();
	}

	return nullptr;
}

bool URedPointNode::IsLinkUI(TSubclassOf<UUserWidget> UIClass, FName UITag)
{
	return RedPointNodeInfo.IsBindUI(UIClass,UITag);
}

APawn* URedPointNode::GetPlayerPawn_RedPointNode()
{
	return UGameplayStatics::GetPlayerPawn(GWorld,0);
}

int32 URedPointNode::GetDisplayCount()
{
	return RedPointChainNodes.Num();
}

void URedPointNode::ShowOrHide(bool IsShow /*= true*/)
{
	if (IsShow)
	{
		RedPointNodeShow(FRedPointMessage(this, this, 1));
	}
	else
	{
		RedPointNodeHide(FRedPointMessage(this, this, 1));
	}
}

UUI_SubSystem* URedPointNode::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(GWorld, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}

FRedPointCustomParameter URedPointNode::GetCustomParameter()
{
	return RedPointNodeInfo.CustomParameter;
}

bool URedPointNode::GetCustomParameter_Float(float& Value, int32 Index /*= 0*/)
{
	if (GetCustomParameter().CustomFloat.IsValidIndex(Index))
	{
		Value = GetCustomParameter().CustomFloat[Index];
		return true;
	}
	return false;
}

bool URedPointNode::GetCustomParameter_String(FString& Value, int32 Index /*= 0*/)
{
	if (GetCustomParameter().CustomString.IsValidIndex(Index))
	{
		Value = GetCustomParameter().CustomString[Index];
		return true;
	}
	return false;
}

bool URedPointNode::GetCustomParameter_Text(FText& Value, int32 Index /*= 0*/)
{
	if (GetCustomParameter().CustomText.IsValidIndex(Index))
	{
		Value = GetCustomParameter().CustomText[Index];
		return true;
	}
	return false;
}

FGameplayTagContainer URedPointNode::GetCustomParameter_TagContainer()
{
	return GetCustomParameter().CustomTag;
}

void URedPointNode::Init_Implementation()
{

}

void URedPointNode::RedPointNodeShow(FRedPointMessage MessageInfo)
{
	bool bIsCallShow = GetDisplayCount() == 0 ? true : false;
	RedPointChainNodes.AddUnique(MessageInfo.LeafNode);
	FRedPointMessage Info = FRedPointMessage(MessageInfo.ChildNode, MessageInfo.LeafNode, GetDisplayCount());
	if (bIsCallShow && GetDisplayCount() > 0)
	{
		bIsShow = true;
		RedPointNodeEvent.Broadcast(Info, ERedPointNodeEventType::Show);
	}
	RedPointNodeEvent.Broadcast(Info, ERedPointNodeEventType::Change);
	//父节点传递
	for (URedPointNode* Node : ParentNodes)
	{
		Node->RedPointNodeShow(Info);
	}
	UE_LOG(LogTemp, Warning, TEXT("URedPointNode::RedPointNodeShow Node:%s NodeClass:%s"),*RedPointNodeInfo.NodeTag.ToString(),*GetClass()->GetName());
}

void URedPointNode::RedPointNodeHide(FRedPointMessage MessageInfo)
{
	bool bIsCallShow = GetDisplayCount() > 0 ? true : false;
	RedPointChainNodes.Remove(MessageInfo.LeafNode);
	FRedPointMessage Info = FRedPointMessage(MessageInfo.ChildNode, MessageInfo.LeafNode, GetDisplayCount());
	if (bIsCallShow && GetDisplayCount() == 0)
	{
		bIsShow = false;
		RedPointNodeEvent.Broadcast(Info, ERedPointNodeEventType::Hide);
	}
	RedPointNodeEvent.Broadcast(Info, ERedPointNodeEventType::Change);
	//父节点传递
	for (URedPointNode* Node : ParentNodes)
	{
		Node->RedPointNodeHide(Info);
	}
	UE_LOG(LogTemp, Warning, TEXT("URedPointNode::RedPointNodeHide Node:%s NodeClass:%s"),*RedPointNodeInfo.NodeTag.ToString(),*GetClass()->GetName());
}

void URedPointNode::RedPointNodeChange(FRedPointMessage MessageInfo)
{

}

void IUI_RedPointInteract::LinkRedPointNodeChange_Implementation(FRedPointMessage MessageInfo, ERedPointNodeEventType EventType)
{
	
}

void URedPointTree::RedPointPanelShowCheck(TSubclassOf<UUserWidget> UIClass, FName UITag)
{
	TArray<URedPointNode*> BindUINodes;
	AllBindUINode.GenerateValueArray(BindUINodes);
	for (URedPointNode* Node : BindUINodes)
	{
		if (Node->IsLinkUI(UIClass,UITag))//是否是该节点链接的UI
		{
			UWidget* UI = GetUISubSystem() ? GetUISubSystem()->GetUI(UIClass, UITag) : nullptr;
			FRedPointMessage Info = FRedPointMessage(Node, Node, 1);
			if (UI && UI->Implements<UUI_RedPointInteract>())
			{
				FScriptDelegate Delegate;
				Delegate.BindUFunction(UI, "LinkRedPointNodeChange");
				Node->RedPointNodeEvent.Add(Delegate);

				if (Node->bIsShow)
				{
					IUI_RedPointInteract::Execute_LinkRedPointNodeChange(UI, Info, ERedPointNodeEventType::Show);
				}
				else
				{
					IUI_RedPointInteract::Execute_LinkRedPointNodeChange(UI, Info, ERedPointNodeEventType::Hide);
				}
				IUI_RedPointInteract::Execute_LinkRedPointNodeChange(UI, Info, ERedPointNodeEventType::Change);
			}
		}
	}
}

void URedPointTree::Init(UObject* Outer)
{
	TArray<FRedPointNodeInfo> AllNodeInfo;//节点信息
	TArray<URedPointNode*> AllNodeObject,AllLeafNodeObject;
	RedPointNodeInfo.GenerateValueArray(AllNodeInfo);
	
	AllNode.Empty();
	AllLeafNode.Empty();
	AllBindUINode.Empty();

	//创建 Node UObject
	for (FRedPointNodeInfo NodeInfo : AllNodeInfo)
	{
		if (NodeInfo.NodeClass)
		{
			URedPointNode* Node = NewObject<URedPointNode>(Outer, NodeInfo.NodeClass);
			Node->RedPointNodeInfo = NodeInfo; 
			AllNode.Add(NodeInfo.NodeTag, Node);
			AllNodeObject.Add(Node);
			if (Node->RedPointNodeInfo.RedPointTreeNodeType == ERedPointTreeNodeType::Leaf)
			{
				AllLeafNodeObject.Add(Node);
				AllLeafNode.Add(NodeInfo.NodeTag, Node);
			}
			if (Node->RedPointNodeInfo.UIClass)
			{
				AllBindUINode.Add(NodeInfo.NodeTag, Node);
			}
		}
	}

	//链接子父级关系
	for (URedPointNode* Node : AllNodeObject)
	{
		for (FName ParentNodeTag : Node->RedPointNodeInfo.ParentNodeTag)
		{
			if (AllNode.Contains(ParentNodeTag))
			{
				Node->ParentNodes.Add(*AllNode.Find(ParentNodeTag));
			}
		}
		for (FName ChildNodeTag : Node->RedPointNodeInfo.ChildNodeTag)
		{
			if (AllNode.Contains(ChildNodeTag))
			{
				Node->ChildNodes.Add(*AllNode.Find(ChildNodeTag));
			}
		}
	}
	

	//初始化
	for (URedPointNode* Node : AllNodeObject)
	{
		Node->Init();
	}
}

URedPointNode* URedPointTree::GetRootNode()
{
	return GetNodeFromName("Root");
}

URedPointNode* URedPointTree::GetNodeFromName(FName NodeName)
{
	if (AllNode.Contains(NodeName))
	{
		return *AllNode.Find(NodeName);
	}
	return nullptr;
}

UUI_SubSystem* URedPointTree::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(GWorld, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}

bool FRedPointNodeInfo::IsBindUI(TSubclassOf<UUserWidget> Class, FName Tag)
{
	if (UIClass == Class && UITag == Tag)
	{
		return true;
	}
	return false;
}

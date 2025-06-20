// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/RedPoint/UI_RedPointShowBase.h"
#include "../../../Public/UI_SubSystem.h"
#include <Subsystems/SubsystemBlueprintLibrary.h>

void UUI_RedPointShowBase::NativeConstruct()
{
	Super::NativeConstruct();
	BindRedPointEventBack(NodeTag);
}

void UUI_RedPointShowBase::LinkRedPointNodeChange_Implementation(FRedPointMessage MessageInfo, ERedPointNodeEventType EventType)
{

}

UUI_SubSystem* UUI_RedPointShowBase::GetUISubSystem()
{
	if (!IsValid(UISubSystem))
	{
		UISubSystem = Cast<UUI_SubSystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(GWorld, UUI_SubSystem::StaticClass()));
	}
	return UISubSystem;
}

void UUI_RedPointShowBase::RemoveBindRedPointNodeEvent(FName NodeName)
{
	if (GetUISubSystem() && GetUISubSystem()->CurRedPointTree)
	{
		URedPointNode* RPNode = GetUISubSystem()->CurRedPointTree->GetNodeFromName(NodeName);
		if (RPNode)
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, "LinkRedPointNodeChange");
			RPNode->RedPointNodeEvent.Remove(Delegate);
		}
	}
}

void UUI_RedPointShowBase::BindRedPointEventBack(FName NodeName)
{
	if (!NodeName.IsNone())
	{
		FRedPointMessage Info = FRedPointMessage();
		if (GetUISubSystem() && GetUISubSystem()->CurRedPointTree)
		{
			Node = GetUISubSystem()->CurRedPointTree->GetNodeFromName(NodeName);
			if (Node)
			{
				FScriptDelegate Delegate;
				Delegate.BindUFunction(this, "LinkRedPointNodeChange");
				if (!Node->RedPointNodeEvent.Contains(Delegate))
				{
					Node->RedPointNodeEvent.Add(Delegate);
				}
				NodeTag = NodeName;
				Info = FRedPointMessage(Node, Node, Node->GetDisplayCount());
				if (Node->bIsShow)
				{
					IUI_RedPointInteract::Execute_LinkRedPointNodeChange(this, Info, ERedPointNodeEventType::Show);
				}
				else
				{
					IUI_RedPointInteract::Execute_LinkRedPointNodeChange(this, Info, ERedPointNodeEventType::Hide);
				}
			}
			else
			{
				IUI_RedPointInteract::Execute_LinkRedPointNodeChange(this, Info, ERedPointNodeEventType::Hide);
			}
		}
		else
		{
			IUI_RedPointInteract::Execute_LinkRedPointNodeChange(this, Info, ERedPointNodeEventType::Hide);
		}
		IUI_RedPointInteract::Execute_LinkRedPointNodeChange(this, Info, ERedPointNodeEventType::Change);
	}
}

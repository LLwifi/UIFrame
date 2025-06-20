// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_ComponentBase.h"
#include "UI_ShowAndHide.generated.h"

/**
 * 组件-UUI_ShowAndHide
 * 提供让UI在Show和Hide时开启/关闭其他面板
 */
UCLASS()
class UIFRAME_API UUI_ShowAndHide : public UUI_ComponentBase
{
	GENERATED_BODY()
		
	public:
		UUI_ShowAndHide();
		virtual TSharedRef<SWidget> RebuildWidget() override;
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	
	public:
		//显示隐藏
		virtual void Show_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
		virtual void Hide_Implementation(UWidget* TriggerUI, EUIShowHideType Type) override;
		virtual bool Esc_Implementation(UWidget* TriggerUI) override;
	
		UFUNCTION()
			void CallUIInterfaceOfClass(TMap<TSubclassOf<UUserWidget>, EUIInterface> Class);
	
		UFUNCTION()
			void CallUIInterfaceOfUserWidget(TMap<UUserWidget*, EUIInterface> UserWidget);
	
		UFUNCTION()
			UUI_SubSystem* GetUISubSystem();
	
		UFUNCTION()
			USoundBase* GetSound(EUIInterface Type);
	
		UFUNCTION()
			void SetParentUIVisibility(EUIInterface Type);
	public:
		UPROPERTY()
			UUI_SubSystem* UISubSystem;
	
		//该UI在UI接口调用时会设置的可见模式
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Show")
		TMap<EUIInterface, ESlateVisibility> ParentUIVisibility =
			{{EUIInterface::Show,ESlateVisibility::SelfHitTestInvisible},
			{EUIInterface::Hide,ESlateVisibility::Collapsed}};
	
		//该UI在Show/Hide时会影响的其他UI<哪个UI，调用哪个接口> 
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Show")
			TMap<TSubclassOf<UUserWidget>, EUIInterface> ShowUIClass;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Show")
			TMap<UUserWidget*, EUIInterface> ShowUIWidget;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hide")
			TMap<TSubclassOf<UUserWidget>, EUIInterface> HideUIClass;
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hide")
			TMap<UUserWidget*, EUIInterface> HideUIWidget;
	
		//该UI在Show/Hide/Esc时会播放的音效，如果配置的音效为空，还会尝试读取UUI_CommonGameConfig的默认配置
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound")
			TMap<EUIInterface,USoundBase*> PlaySoundConfig;
};

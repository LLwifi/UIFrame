// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include <AssetTypeActions_Base.h>
#include "RedPointFactory.generated.h"

class URedPointTree;
class URedPointTreeGraphEntryNode;

//�����Դ
struct FAssetTypeAction_RedPointTree : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions Implementation
	// �����ʲ�������
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "FAssetTypeAction_RedPointTree", "RedPointTree"); }
	// �ʲ���Content Browser��������ɫ
	virtual FColor GetTypeColor() const override { return FColor(236, 41, 30); }
	// ��Action��Ӧ����
	virtual UClass* GetSupportedClass() const override;
	//�����������ĸ�����
	virtual uint32 GetCategories() override;

	// �༭��ͼ��
	//virtual TSharedPtr<SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;

	//���ʲ����Ҽ������Ĳ˵������һ������
	virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;
	// ���ڶ����֮����ʾ�ı༭��
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

};

/**
 * ������Ĺ�����
�����˸���Դ�����ķ���
 */
UCLASS()
class UIFRAMEEDITOR_API URedPointTreeFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	URedPointTreeFactory(); 
	//�ӱ༭����������Ҽ��˵��д���
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	//�ⲿ�϶��ļ����д���
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};



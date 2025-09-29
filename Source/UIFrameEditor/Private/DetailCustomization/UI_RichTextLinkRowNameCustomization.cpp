// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCustomization/UI_RichTextLinkRowNameCustomization.h"
#include <IDetailChildrenBuilder.h>
#include <Widgets/Input/SEditableTextBox.h>
#include <Misc/MessageDialog.h>
#include <Misc/PackageName.h>
#include <FileHelpers.h>
#include <DetailWidgetRow.h>
#include "SSearchableComboBox.h"
#include <PropertyCustomizationHelpers.h>
#include <Engine/CompositeDataTable.h>
//#include <Editor/PropertyEditor/Private/UserInterface/PropertyEditor/SPropertyEditorCombo.h>

#define LOCTEXT_NAMESPACE "UI_RichTextLinkRowNameCustomization"

void IUI_RichTextLinkRowNameCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ];
}

void IUI_RichTextLinkRowNameCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    //����FUI_RichTextLinkRowName��-��FUI_RichTextContent-��UUI_RichTextBlock
    TSharedPtr<IPropertyHandle> RichTextBlockHandle = PropertyHandle->GetParentHandle()->GetParentHandle();
    void* ValuePtr;
    RichTextBlockHandle->GetValueData(ValuePtr);
    if (ValuePtr != nullptr)//�ڶ���֪ͨ��ͬʱѡ������ʱ��Ϊ��
    {
        UUI_RichTextBlock* RichTextBlock = (UUI_RichTextBlock*)ValuePtr;
        if (RichTextBlock)
        {
            RichTextBlockDT = RichTextBlock->GetTextStyleSet();
        }
    }

    //RowNameHandle = PropertyHandle->GetChildHandle("RowName");
    //ResourceNameOrIndexHandle = PropertyHandle->GetChildHandle("ResourceNameOrIndex");
    //SoundTypeHandle = PropertyHandle->GetChildHandle("RichContentType");
    //DataTableHandle = PropertyHandle->GetChildHandle("DataTable");

    ////��Ӧ�����ݱ�
    //TSharedPtr<IPropertyHandle> SoundBaseHandle, ReferenceObjectHandle;
    //SoundBaseHandle = PropertyHandle->GetChildHandle("SoundBase");
    //ReferenceObjectHandle = PropertyHandle->GetChildHandle("ReferenceObject");


    //PropertyHandle->GetValueData(ValuePtr);
    //if (ValuePtr != nullptr)//�ڶ���֪ͨ��ͬʱѡ������ʱ��Ϊ��
    //{
    //    RichTextLinkRowName = (FUI_RichTextLinkRowName*)ValuePtr;

    //    Refresh(RowNames, ResourceNameOrIndexs);

    //    UEnum* EnumPtr = StaticEnum<ESoundAssetType>();
    //    if (EnumPtr)
    //    {
    //        for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
    //        {
    //            FString DisplayName = EnumPtr->GetNameStringByValue(i);
    //            ESoundAssetType SoundAssetType = (ESoundAssetType)EnumPtr->GetValueByIndex(i);
    //            Name_Type.Add(DisplayName, SoundAssetType);
    //            SoundTypes.Add(MakeShareable(new FString(DisplayName)));
    //        }
    //    }


    //    //slate
    //    ChildBuilder.AddCustomRow(FText())
    //        [
    //            SNew(SVerticalBox)
    //                + SVerticalBox::Slot()
    //                [
    //                    SAssignNew(SearchableComboBox_SoundType, SSearchableComboBox)
    //                        .OptionsSource(&SoundTypes)//����ѡ��
    //                        .OnGenerateWidget(this, &ISoundAssetTagCustomization::OnGenerateWidget_SoundType)//ÿ������ѡ�����ʽͨ����������
    //                        .OnSelectionChanged(this, &ISoundAssetTagCustomization::OnSelectionChanged_SoundType)//�ı�ѡ��Ļص�
    //                        [
    //                            SAssignNew(ComboBox_SoundType_Text, STextBlock)
    //                                .Text(FText::FromString(EnumPtr->GetNameStringByValue((int32)SoundAssetTag->SoundAssetType)))
    //                        ]
    //                ]
    //                + SVerticalBox::Slot()
    //                [
    //                    SAssignNew(SearchableComboBox_RowName, SSearchableComboBox)
    //                        .OptionsSource(&RowNames)//����ѡ��
    //                        .OnGenerateWidget(this, &ISoundAssetTagCustomization::OnGenerateWidget_RowName)//ÿ������ѡ�����ʽͨ����������
    //                        .OnSelectionChanged(this, &ISoundAssetTagCustomization::OnSelectionChanged_RowName)//�ı�ѡ��Ļص�
    //                        [
    //                            SAssignNew(ComboBox_Name_Text, STextBlock)
    //                                .Text(FText::FromString(SoundAssetTag->RowName.ToString()))
    //                        ]
    //                ]
    //                + SVerticalBox::Slot()
    //                [
    //                    SAssignNew(SearchableComboBox_RNOI, SSearchableComboBox)
    //                        .OptionsSource(&ResourceNameOrIndexs)//����ѡ��
    //                        .OnGenerateWidget(this, &ISoundAssetTagCustomization::OnGenerateWidget_RNOI)//ÿ������ѡ�����ʽͨ����������
    //                        .OnSelectionChanged(this, &ISoundAssetTagCustomization::OnSelectionChanged_RNOI)//�ı�ѡ��Ļص�
    //                        [
    //                            SAssignNew(ComboBox_RNOI_Text, STextBlock)
    //                                .Text(FText::FromString(SoundAssetTag->ResourceNameOrIndex))
    //                        ]
    //                ]
    //                + SVerticalBox::Slot().AutoHeight()
    //                [
    //                    SNew(SHorizontalBox)
    //                        +SHorizontalBox::Slot().AutoWidth()
    //                        [
    //                            DataTableHandle->CreatePropertyValueWidget()
    //                        ]
    //                        + SHorizontalBox::Slot().AutoWidth()
    //                        [
    //                            SoundBaseHandle->CreatePropertyValueWidget()
    //                        ]
    //                        + SHorizontalBox::Slot().AutoWidth()
    //                        [
    //                            ReferenceObjectHandle->CreatePropertyValueWidget()
    //                        ]
    //                ]
    //        ];
    //}
}

//void IUI_RichTextLinkRowNameCustomization::Refresh(TArray<TSharedPtr<FString>>& AllRowName, TArray<TSharedPtr<FString>>& AllResourceNameOrIndex)
//{
//    AllRowName.Empty();
//    AllResourceNameOrIndex.Empty();
//
//    //ParentTables
//    UDataTable* DataTable = nullptr;
//    TArray<TSoftObjectPtr<UDataTable>> AllDT;
//    TArray<FName> DTRowName;
//    TArray<FString> SoundResourceName;
//    switch (SoundAssetTag->SoundAssetType)
//    {
//    case ESoundAssetType::Sound:
//    {
//        DataTable = UResourcesConfig::GetInstance()->TypeMaping[EResourceType::Sound].LoadSynchronous();
//        if (DataTable)
//        {
//            FResourceProperty_Sound* ResourceProperty_Sound = DataTable->FindRow<FResourceProperty_Sound>(SoundAssetTag->RowName, TEXT(""));
//            if (ResourceProperty_Sound)
//            {
//                ResourceProperty_Sound->Sound.GenerateKeyArray(SoundResourceName);
//                if(ResourceProperty_Sound->Sound.Contains(SoundAssetTag->ResourceNameOrIndex))
//                {
//                    SoundAssetTag->SoundBase = ResourceProperty_Sound->Sound[SoundAssetTag->ResourceNameOrIndex].Sound;
//                    SoundAssetTag->ReferenceObject = ResourceProperty_Sound->Sound[SoundAssetTag->ResourceNameOrIndex].ReferenceObject;
//                }
//                //���û�ҵ����õ�������Դ������ʹ���ϲ�ͨ�����õ��׸���Դ
//                if (!SoundAssetTag->ReferenceObject && ResourceProperty_Sound->ReferencesObjects.IsValidIndex(0))
//                {
//                    SoundAssetTag->ReferenceObject = ResourceProperty_Sound->ReferencesObjects[0];
//                }
//            }
//            DTRowName = DataTable->GetRowNames();
//        }
//
//        //����Ѱ��׼ȷ�����ݱ�
//        AllDT = UResourcesConfig::GetInstance()->AllSoundDataTable;
//        for (TSoftObjectPtr<UDataTable> DT : AllDT)
//        {
//            if (DT.LoadSynchronous())
//            {
//                FResourceProperty_Sound* ResourceProperty_Sound = DT.LoadSynchronous()->FindRow<FResourceProperty_Sound>(SoundAssetTag->RowName, TEXT(""));
//                if (ResourceProperty_Sound)
//                {
//                    SoundAssetTag->DataTable = DT;
//                    break;
//                }
//            }
//        }
//        break;
//    }
//    case ESoundAssetType::BGM:
//    {
//        DataTable = UResourcesConfig::GetInstance()->TypeMaping[EResourceType::BGM].LoadSynchronous();
//        if (DataTable)
//        {
//            FResourceProperty_BGM* ResourceProperty_BGM = DataTable->FindRow<FResourceProperty_BGM>(SoundAssetTag->RowName, TEXT(""));
//            if (ResourceProperty_BGM)
//            {
//                ResourceProperty_BGM->BGM.GenerateKeyArray(SoundResourceName);
//
//                if(ResourceProperty_BGM->BGM.Contains(SoundAssetTag->ResourceNameOrIndex))
//                {
//                    SoundAssetTag->SoundBase = ResourceProperty_BGM->BGM[SoundAssetTag->ResourceNameOrIndex].SoundBase;
//                    SoundAssetTag->ReferenceObject = ResourceProperty_BGM->BGM[SoundAssetTag->ResourceNameOrIndex].ReferenceObject;
//                }
//
//                //���û�ҵ����õ�������Դ������ʹ���ϲ�ͨ�����õ��׸���Դ
//                if (!SoundAssetTag->ReferenceObject && ResourceProperty_BGM->ReferencesObjects.IsValidIndex(0))
//                {
//                    SoundAssetTag->ReferenceObject = ResourceProperty_BGM->ReferencesObjects[0];
//                }
//            }
//            DTRowName = DataTable->GetRowNames();
//        }
//
//        //����Ѱ��׼ȷ�����ݱ�
//        AllDT = UResourcesConfig::GetInstance()->AllSoundDataTable;
//        for (TSoftObjectPtr<UDataTable> DT : AllDT)
//        {
//            if (DT.LoadSynchronous())
//            {
//                FResourceProperty_BGM* ResourceProperty_BGM = DT.LoadSynchronous()->FindRow<FResourceProperty_BGM>(SoundAssetTag->RowName, TEXT(""));
//                if (ResourceProperty_BGM)
//                {
//                    SoundAssetTag->DataTable = DT;
//                    break;
//                }
//            }
//        }
//        break;
//    }
//    default:
//        break;
//    }
//
//    for (FName& name : DTRowName)
//    {
//        AllRowName.Add(MakeShareable(new FString(name.ToString())));
//    }
//    for (FString& string : SoundResourceName)
//    {
//        AllResourceNameOrIndex.Add(MakeShareable(new FString(string)));
//    }
//    
//    //���û�ҵ���ȷ�����ݱ�ʹ��������ݱ�
//    if (!SoundAssetTag->DataTable)
//    {
//        SoundAssetTag->DataTable = DataTable;
//    }
//
//}
//
//void IUI_RichTextLinkRowNameCustomization::OnSelectionChanged_SoundType(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo)
//{
//    //SoundTypeHandle->SetValue(&Name_Type[*InItem.Get()]);
//    ComboBox_SoundType_Text->SetText(FText::FromString(*InItem));
//    SoundAssetTag->SoundAssetType = Name_Type[*InItem.Get()];
//
//    Refresh(RowNames, ResourceNameOrIndexs);
//    SearchableComboBox_RowName->RefreshOptions();
//    SearchableComboBox_RNOI->RefreshOptions();
//}
//
//TSharedRef<SWidget> IUI_RichTextLinkRowNameCustomization::OnGenerateWidget_SoundType(TSharedPtr<FString> InItem)
//{
//    return SNew(STextBlock)
//        .Text(FText::FromString(*InItem));
//}
//
//void IUI_RichTextLinkRowNameCustomization::OnSelectionChanged_RowName(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo)
//{
//    RowNameHandle->SetValue(FName(*InItem.Get()));
//    ComboBox_Name_Text->SetText(FText::FromString(*InItem));
//    SoundAssetTag->RowName = FName(*InItem.Get());
//
//    Refresh(RowNames, ResourceNameOrIndexs);
//    SearchableComboBox_RNOI->RefreshOptions();
//}
//
//TSharedRef<SWidget> IUI_RichTextLinkRowNameCustomization::OnGenerateWidget_RowName(TSharedPtr<FString> InItem)
//{
//    return SNew(STextBlock)
//        .Text(FText::FromString(*InItem));
//}
//
//void IUI_RichTextLinkRowNameCustomization::OnSelectionChanged_RNOI(TSharedPtr<FString> InItem, ESelectInfo::Type InSelectionInfo)
//{
//    ResourceNameOrIndexHandle->SetValue(FName(*InItem.Get()));
//    ComboBox_RNOI_Text->SetText(FText::FromString(*InItem));
//    SoundAssetTag->ResourceNameOrIndex = *InItem.Get();
//
//    Refresh(RowNames, ResourceNameOrIndexs);
//}
//
//TSharedRef<SWidget> IUI_RichTextLinkRowNameCustomization::OnGenerateWidget_RNOI(TSharedPtr<FString> InItem)
//{
//    return SNew(STextBlock)
//        .Text(FText::FromString(*InItem));
//}

#undef LOCTEXT_NAMESPACE
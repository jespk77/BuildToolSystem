#include "ObjectPropertiesWidget.h"
#include "BuildToolSystem/Components/ObjectSelectionComponent.h"
#include "BuildToolSystem/Data/ObjectSelection.h"
#include "BuildToolSystem/Data/SelectableObject.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"
#include "ObjectWidgetInterface.h"
#include "BuildToolSystem/BuildToolSettings.h"

void UObjectPropertiesWidget::NativeConstruct() {
	Super::NativeConstruct();
	SelectionComponent = GetOwningPlayer()->GetComponentByClass<UObjectSelectionComponent>();
	if (IsValid(SelectionComponent)) SelectionComponent->OnSelectionChanged.AddDynamic(this, &UObjectPropertiesWidget::OnSelectionChanged);
	else UE_LOG(LogTemp, Warning, TEXT("ObjectPropertiesWidget: No ToolComponent found on owned player"));
}

void UObjectPropertiesWidget::NativeDestruct() {
	Super::NativeDestruct();
	if (IsValid(SelectionComponent)) SelectionComponent->OnSelectionChanged.RemoveDynamic(this, &UObjectPropertiesWidget::OnSelectionChanged);
	SelectionComponent = nullptr;
}

void UObjectPropertiesWidget::OnSelectionChanged_Implementation(const UObjectSelection* _) {
	UObjectSelection* selection = SelectionComponent->GetSelection();
	const bool isSelectionValid = IsValid(selection);

	TSubclassOf<UUserWidget> widgetClass = nullptr;
	if (isSelectionValid) {
		const TSet<UObject*>& objects = selection->GetSelectedObjects();
		UClass* baseClass = UClassUtilities::GetCommonClassFromSet(objects);

		if (baseClass && baseClass->ImplementsInterface(USelectableObject::StaticClass())) {
			widgetClass = ISelectableObject::Execute_GetEditorWidgetClass(GetDefault<UObject>(baseClass));
		}
	}

	if (!widgetClass && isSelectionValid && !selection->IsEmpty()) {
		const UBuildToolSettings* settings = GetDefault<UBuildToolSettings>();
		widgetClass = settings ? settings->GeneralObjectEditorWidget.LoadSynchronous() : nullptr;
	}

	UUserWidget* newWidget = widgetClass ? CreateWidget(this, widgetClass) : nullptr;
	if (ActiveWidget) {
		if (ActiveWidget->Implements<UObjectWidgetInterface>()) IObjectWidgetInterface::Execute_OnWidgetDeactivated(ActiveWidget);
		ActiveWidget->Destruct();
	}

	ActiveWidget = newWidget;
	if (ActiveWidget && ActiveWidget->Implements<UObjectWidgetInterface>()) IObjectWidgetInterface::Execute_OnWidgetActivated(ActiveWidget, selection);
	SetSelectionWidget(ActiveWidget);
}

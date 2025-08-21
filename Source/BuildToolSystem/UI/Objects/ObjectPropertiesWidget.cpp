#include "ObjectPropertiesWidget.h"
#include "BuildToolSystem/Components/ObjectSelectionComponent.h"
#include "BuildToolSystem/Data/ObjectSelection.h"
#include "BuildToolSystem/Data/SelectableObject.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"
#include "ObjectWidgetInterface.h"

void UObjectPropertiesWidget::NativeConstruct() {
	Super::NativeConstruct();
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
	const TSet<UObject*>& objects = selection->GetSelectedObjects();
	UClass* baseClass = UClassUtilities::GetCommonClassFromSet(objects);

	UUserWidget* newWidget = nullptr;
	if (baseClass && baseClass->ImplementsInterface(USelectableObject::StaticClass())) {
		TSubclassOf<UUserWidget> widgetClass = ISelectableObject::Execute_GetEditorWidgetClass(*objects.FindArbitraryElement());
		if (widgetClass) newWidget = CreateWidget(this, widgetClass);
	}

	if (ActiveWidget) {
		if (ActiveWidget->Implements<UObjectWidgetInterface>()) IObjectWidgetInterface::Execute_OnWidgetDeactivated(ActiveWidget);
		ActiveWidget->Destruct();
	}

	ActiveWidget = newWidget;
	if (ActiveWidget) {
		if (ActiveWidget->Implements<UObjectWidgetInterface>()) IObjectWidgetInterface::Execute_OnWidgetActivated(ActiveWidget, selection);
		SetSelectionWidget(ActiveWidget);
	}
}

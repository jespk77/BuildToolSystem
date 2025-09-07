#include "ObjectSelectionWidget.h"
#include "Blueprint/WidgetTree.h"
#include "BuildToolSystem/Components/ObjectSelectionComponent.h"
#include "BuildToolSystem/Data/ObjectSelection.h"
#include "BuildToolSystem/Data/SelectableObject.h"
#include "CoreGameUI/Widgets/ObjectEditorWidget.h"
#include "CoreGameUI/Widgets/PropertyWidgets.h"
#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"

void USelectionEditorWidget::InitializeSelection(UObjectSelection* newSelection) {
	ensureAlways(newSelection);
	Selection = newSelection;
	Selection->GetSelectedObjectsArray(SelectedObjects);

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	for (UWidget* widget : widgets) {
		if (widget->Implements<UPropertyObjectEditor>()) {
			IPropertyObjectEditor::Execute_SetObjects(widget, SelectedObjects);
		}
	}
	SelectionUpdated();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UObjectSelectionWidget::OnSelectionChanged(const UObjectSelection* _) {
	UUserWidget* selectionWidget = nullptr;

	if (UObjectSelection* selection = SelectionComponent->GetSelection()) {
		UClass* objectClass = UClassUtilities::GetCommonClassFromSet(selection->GetSelectedObjects());
		if (!selection->IsEmpty() && objectClass->ImplementsInterface(USelectableObject::StaticClass())) {
			const UObject* obj = *selection->GetSelectedObjects().FindArbitraryElement();
			if (TSubclassOf<UUserWidget> widgetClass = ISelectableObject::Execute_GetEditorWidgetClass(obj, selection)) {
				USelectionEditorWidget* widget = CreateWidget<USelectionEditorWidget>(this, widgetClass);
				widget->InitializeSelection(selection);
				selectionWidget = widget;
			}
		}
	}

	if (IsValid(EditorWidget)) EditorWidget->MarkAsGarbage();

	EditorWidget = selectionWidget;
	if (IsValid(EditorWidget)) {
		Container->SetContent(EditorWidget->TakeWidget());
		SetVisibility(ESlateVisibility::Visible);
	}
	else {
		Container->ClearContent();
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

TSharedRef<SWidget> UObjectSelectionWidget::RebuildWidget() {
	return SAssignNew(Container, SBorder);
}

void UObjectSelectionWidget::ReleaseSlateResources(bool releaseChildren) {
	Container.Reset();
	EditorWidget = nullptr;
	Super::ReleaseSlateResources(releaseChildren);
}

void UObjectSelectionWidget::NativePreConstruct() {
	Super::NativePreConstruct();

	if (!IsDesignTime()) {
		SelectionComponent = GetOwningPlayer()->GetComponentByClass<UObjectSelectionComponent>();
		if (IsValid(SelectionComponent)) SelectionComponent->OnSelectionChanged.AddDynamic(this, &UObjectSelectionWidget::OnSelectionChanged);
		else UE_LOG(LogTemp, Log, TEXT("UObjectSelectionWidget: No ObjectSelectionComponent found on PlayerController"));
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UObjectSelectionWidget::NativeDestruct() {
	Super::NativeDestruct();

	if (IsValid(SelectionComponent)) SelectionComponent->OnSelectionChanged.RemoveDynamic(this, &UObjectSelectionWidget::OnSelectionChanged);
	SelectionComponent = nullptr;
}

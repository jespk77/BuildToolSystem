#include "ToolPropertiesWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Layout/SUniformWrapPanel.h"
#if WITH_EDITORONLY_DATA
#include "WidgetUtilities/Widgets/DynamicEditorWidget.h"
#endif

#include "../Game/BuildToolComponent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

void UToolPropertiesWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	ToolComponent = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (ToolComponent) ToolComponent->OnToolChanged.AddDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
	else UE_LOG(LogTemp, Warning, TEXT("No ToolComponent found on owned player"));
}

void UToolPropertiesWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	if (ToolComponent) ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
	ToolComponent = nullptr;
}

void UToolPropertiesWidgetBase::OnActiveToolChanged_Implementation(int32 index) {
	UBuildTool* tool = ToolComponent->GetActiveTool();
	UUserWidget* newWidget;

	if (IsValid(tool) && tool->ToolWidget) newWidget = CreateWidget(this, tool->ToolWidget, "ToolWidget");
	else newWidget = nullptr;

	if (newWidget != ActiveWidget) {
		if (ActiveWidget) ActiveWidget->Destruct();
		ActiveWidget = newWidget;
		SetPropertyWidget(ActiveWidget);
	}
}

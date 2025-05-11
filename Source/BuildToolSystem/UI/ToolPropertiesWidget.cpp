#include "ToolPropertiesWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Layout/SUniformWrapPanel.h"

#include "BlueprintUtilities/BlueprintFunctionLibrary/ClassUtilities.h"
#include "../Game/BuildToolComponent.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

void UToolPropertiesWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	ToolComponent = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (ToolComponent) ToolComponent->OnToolChanged.AddDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
}

void UToolPropertiesWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	if (ToolComponent) ToolComponent->OnToolChanged.RemoveDynamic(this, &UToolPropertiesWidgetBase::OnActiveToolChanged);
	ToolComponent = nullptr;
}

void UToolPropertiesWidgetBase::OnActiveToolChanged(int32 index) {
	SetObject(ToolComponent->GetActiveTool());
}

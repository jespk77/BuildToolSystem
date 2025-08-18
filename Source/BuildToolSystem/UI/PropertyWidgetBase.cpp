#include "PropertyWidgetBase.h"
#include "BuildToolSystem/Components/BuildToolComponent.h"

void UPropertyWidgetBase::NativeConstruct() {
	Super::NativeConstruct();
	ToolComponent = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
}

void UPropertyWidgetBase::NativeDestruct() {
	Super::NativeDestruct();
	ToolComponent = nullptr;
}

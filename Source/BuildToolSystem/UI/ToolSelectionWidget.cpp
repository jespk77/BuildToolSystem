#include "ToolSelectionWidget.h"
#include "../Game/BuildToolComponent.h"

void UToolSelectionWidget::NativeConstruct() {
	Super::NativeConstruct();
	Owner = GetOwningPlayer()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) Owner = GetOwningPlayerPawn()->GetComponentByClass<UBuildToolComponent>();
	if (!Owner) UE_LOG(LogToolSystem, Warning, TEXT("Missing BuildToolComponent on player controller or player pawn! Input will not work for tools"));
}

void UToolSelectionWidget::NativeDestruct() {
	Super::NativeDestruct();
	Owner = nullptr;
}

#pragma once

#include "CommonUserWidget.h"
#include "ObjectPropertiesWidget.generated.h"

class UObjectSelection;
class UObjectSelectionComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UObjectPropertiesWidget : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UObjectSelectionComponent> SelectionComponent;
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UUserWidget> ActiveWidget;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(Category = "Events", BlueprintNativeEvent)
	void OnSelectionChanged(const UObjectSelection* selection);
	virtual void OnSelectionChanged_Implementation(const UObjectSelection* selection);

	UFUNCTION(Category = "Events", BlueprintImplementableEvent)
	void SetSelectionWidget(UUserWidget* widget);
};

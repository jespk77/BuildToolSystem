#pragma once

#include "CommonUserWidget.h"
#include "PropertyWidgetBase.generated.h"

class UBuildToolComponent;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UPropertyWidgetBase : public UCommonUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UBuildToolComponent> ToolComponent;

	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UUserWidget> ActiveWidget;

	UFUNCTION(Category = "Property Widget", BlueprintImplementableEvent)
	void SetPropertyWidget(UUserWidget* widget);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};

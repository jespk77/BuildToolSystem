#pragma once

#include "CommonUserWidget.h"
#include "ObjectWidgetInterface.h"
#include "ObjectEditorWidgetBase.generated.h"

class UObjectSelection;

UCLASS(Abstract)
class BUILDTOOLSYSTEM_API UObjectEditorWidgetBase : public UCommonUserWidget, public IObjectWidgetInterface {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<UObjectSelection> Selection;

public:
	virtual void OnWidgetActivated_Implementation(UObjectSelection* selection) { Selection = selection; }
	virtual void OnWidgetDeactivated_Implementation() { Selection = nullptr; }
};

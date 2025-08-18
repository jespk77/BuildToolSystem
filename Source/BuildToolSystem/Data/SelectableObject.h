#pragma once

#include "SelectableObject.generated.h"

UINTERFACE(BlueprintType)
class BUILDTOOLSYSTEM_API USelectableObject : public UInterface {
	GENERATED_BODY()
};

class BUILDTOOLSYSTEM_API ISelectableObject {
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	bool CanSelect() const;
	virtual bool CanSelect_Implementation() const;

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void ObjectSelectedTick() const;

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void OnObjectSelected();
	virtual void OnObjectSelected_Implementation();
	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void OnObjectUnselected();
	virtual void OnObjectUnselected_Implementation();

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	TSubclassOf<UUserWidget> GetEditorWidgetClass() const;
};

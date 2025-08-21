#pragma once

#include "SelectableObject.generated.h"

UINTERFACE(BlueprintType)
class BUILDTOOLSYSTEM_API USelectableObject : public UInterface {
	GENERATED_BODY()
};

class BUILDTOOLSYSTEM_API ISelectableObject {
	GENERATED_BODY()

	friend class UObjectSelection;

protected:
	static void SelectObject(UObject* obj);
	static void UnselectObject(UObject* obj);

public:
	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	bool CanSelect() const;

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void ObjectSelectedTick() const;

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void OnObjectSelected();
	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	void OnObjectUnselected();

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	TSubclassOf<UUserWidget> GetEditorWidgetClass() const;
};

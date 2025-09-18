#pragma once

#include "SelectableObject.generated.h"

UINTERFACE(BlueprintType)
class BUILDTOOLSYSTEM_API USelectableObject : public UInterface {
	GENERATED_BODY()
};

class BUILDTOOLSYSTEM_API ISelectableObject {
	GENERATED_BODY()

	friend class UObjectSelection;

private:
	static UMeshComponent* FindBestMeshComponent(AActor* actor);

protected:
	static void SelectObject(UObject* obj);
	static void UnselectObject(UObject* obj);

public:
	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	bool CanSelect() const;
	virtual bool CanSelect_Implementation() const { return true; }

	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	void ObjectSelectedTick() const;

	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	void OnObjectSelected();
	UFUNCTION(Category = "Selection", BlueprintNativeEvent)
	void OnObjectUnselected();

	UFUNCTION(Category = "Interface", BlueprintNativeEvent)
	TSubclassOf<UUserWidget> GetEditorWidgetClass(const UObjectSelection* selection) const;
};

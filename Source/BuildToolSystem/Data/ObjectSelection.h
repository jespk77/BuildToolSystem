#pragma once

#include "BuildToolSystem/TraceHitResult.h"
#include "ObjectSelection.generated.h"

UCLASS(BlueprintType, Transient)
class BUILDTOOLSYSTEM_API UObjectSelection : public UObject {
	GENERATED_BODY()

	friend class UObjectSelectionComponent;

private:
	static bool CheckValidObjects(TSet<UObject*>& objects, const bool checkInterface = true, const bool checkCompatible = false);

protected:
	UPROPERTY(Category = "Objects", VisibleInstanceOnly, BlueprintReadOnly)
	TSet<UObject*> SelectedObjects;

	void InvokeObjectsSelected();
	void InvokeObjectsUnselected();

public:
	static UObjectSelection* FromHitResult(const FHitResults& hits);
	static UObjectSelection* FromSelection(const TSet<UObject*>& objects);

	UFUNCTION(Category = "Objects", BlueprintCallable)
	const TSet<UObject*>& GetSelectedObjects() const { return SelectedObjects; }
	TSet<UObject*>::TConstIterator GetObjectIterator() const { return SelectedObjects.CreateConstIterator(); }
	UFUNCTION(Category = "Objects", BlueprintCallable)
	int32 GetObjectCount() const { return SelectedObjects.Num(); }
	UFUNCTION(Category = "Objects", BlueprintCallable)
	bool IsEmpty() const { return SelectedObjects.IsEmpty(); }

	UFUNCTION(Category = "Objects", BlueprintCallable)
	UObjectSelection* ClearSelectedObjects(const int32 size = 0) const;

	UFUNCTION(Category = "Objects", BlueprintCallable)
	UObjectSelection* SelectedObjectUnion(const TSet<UObject*>& objects) const;
	UObjectSelection* SelectedObjectUnion(const TArray<UObject*>& objects) const { return SelectedObjectUnion(TSet<UObject*>(objects)); }

	UFUNCTION(Category = "Objects", BlueprintCallable)
	UObjectSelection* SelectedObjectDifference(const TSet<UObject*>& objects) const;
	UObjectSelection* SelectedObjectDifference(const TArray<UObject*>& objects) const { return SelectedObjectDifference(TSet<UObject*>(objects)); }

	UFUNCTION(Category = "Objects", BlueprintCallable)
	UObjectSelection* SetSelectedObjects(TSet<UObject*>& objects) const;
	UObjectSelection* SetSelectedObjects(TArray<UObject*>& objects) const { TSet<UObject*> newSet(objects); return SetSelectedObjects(newSet); }
};

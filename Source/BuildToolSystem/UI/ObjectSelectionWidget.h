#pragma once

#include "Blueprint/UserWidget.h"
#include "ObjectSelectionWidget.generated.h"

class UObjectSelection;
class UObjectSelectionComponent;

UCLASS()
class BUILDTOOLSYSTEM_API USelectionEditorWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "Selection", BlueprintReadOnly)
	TObjectPtr<UObjectSelection> Selection;

	UPROPERTY(Category = "Selection", BlueprintReadOnly)
	TArray<UObject*> SelectedObjects;

public:
	UFUNCTION(Category = "Selection", BlueprintCallable)
	virtual void InitializeSelection(UObjectSelection* newSelection);

	UFUNCTION(Category= "Selection", BlueprintImplementableEvent)
	void SelectionUpdated();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS(NotBlueprintable)
class BUILDTOOLSYSTEM_API UObjectSelectionWidget : public UUserWidget {
	GENERATED_BODY()

private:
	UFUNCTION() void OnSelectionChanged(const UObjectSelection* selection);

protected:
	TSharedPtr<SBorder> Container;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UObjectSelectionComponent> SelectionComponent;

	UPROPERTY(Category = "Tools", BlueprintReadOnly)
	TObjectPtr<UUserWidget> EditorWidget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool releaseChildren) override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
};

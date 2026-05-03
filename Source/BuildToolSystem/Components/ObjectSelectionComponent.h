#pragma once

#include "../TraceHitResult.h"
#include "ObjectSelectionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, const class UObjectSelection*, Selection);

UCLASS(Within = PlayerController, meta = (BlueprintSpawnableComponent))
class BUILDTOOLSYSTEM_API UObjectSelectionComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<APlayerController> OwnedController;
	UPROPERTY(Category = "References", VisibleInstanceOnly, Instanced, BlueprintReadOnly, Transient)
	TObjectPtr<class UObjectSelection> Selection;

	UPROPERTY(Category = "Input", BlueprintReadOnly)
	bool IsDragging = false;
	UPROPERTY(Category = "Input", BlueprintReadOnly)
	FBox SelectionBoxStart;
	UPROPERTY(Category = "Input", BlueprintReadOnly)
	FBox SelectionBoxEnd;
	UPROPERTY(Category = "Input", BlueprintReadOnly)
	FVector SelectionDirection;

	bool GetActorsInSelectionBox(FHitResults& hits) const;
	void SetLocationAtDistance(FVector& locationStart, FVector& locationEnd, const float distance) const;

	virtual void ProcessSelectionChanged();

public:
	UObjectSelectionComponent(const FObjectInitializer& initializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void TickComponent(float delta, ELevelTick tick, FActorComponentTickFunction* function) override;

	UPROPERTY(Category = "Events", BlueprintAssignable)
	FOnSelectionChanged OnSelectionChanged;

	UFUNCTION(Category = "Selection", BlueprintCallable)
	class UObjectSelection* GetSelection() { return Selection; }
	const class UObjectSelection* GetSelection() const { return Selection; }
	UFUNCTION(Category = "Selection", BlueprintCallable)
	void SetSelection(class UObjectSelection* newSelection);

	virtual bool OnMouseDown(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseUp(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseMove(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) { return false; }
	virtual bool OnMouseDoubleDown(const FGeometry& geometry, const FPointerEvent& event) { return false; }
};

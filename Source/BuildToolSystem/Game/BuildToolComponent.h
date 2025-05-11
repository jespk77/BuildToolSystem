#pragma once

#include "CoreMinimal.h"
#include "BuildTool.h"
#include "BuildToolComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveToolChanged, int32, ToolIndex);

UCLASS(meta = (BlueprintSpawnableComponent))
class BUILDTOOLSYSTEM_API UBuildToolComponent : public UActorComponent {
	GENERATED_BODY()

private:
	void CreateTools();

protected:

	UPROPERTY(Category = "References", VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UBuildTool>> Tools;

	UPROPERTY(Category = "Tools", VisibleInstanceOnly, BlueprintReadOnly)
	int32 ActiveToolIndex = INDEX_NONE;

public:
	UBuildToolComponent(const FObjectInitializer& initializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void TickComponent(float delta, ELevelTick tick, FActorComponentTickFunction* function) override;

	UPROPERTY(Category = "Tools", BlueprintAssignable)
	FOnActiveToolChanged OnToolChanged;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	bool HasActiveTool() const { return Tools.IsValidIndex(ActiveToolIndex); }

	const UBuildTool* GetActiveTool() const { return HasActiveTool() ? Tools[ActiveToolIndex] : nullptr; }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	UBuildTool* GetActiveTool() { return Tools.IsValidIndex(ActiveToolIndex) ? Tools[ActiveToolIndex] : nullptr; }

	UFUNCTION(Category = "Tools", BlueprintCallable)
	void SetActiveTool(int32 toolIndex);
	UFUNCTION(Category = "Tools", BlueprintCallable)
	void SetActiveToolByName(const FString name);

	virtual bool OnMouseDown(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseUp(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseMove(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseWheel(const FGeometry& geometry, const FPointerEvent& event);
	virtual bool OnMouseDoubleDown(const FGeometry& geometry, const FPointerEvent& event);
};
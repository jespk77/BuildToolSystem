#pragma once

#include "../TraceHitResult.h"
#include "../RaycastParameters.h"
#include "BuildTool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToolEvent);

UCLASS(Abstract, Blueprintable)
class BUILDTOOLSYSTEM_API UBuildTool : public UObject {
	GENERATED_BODY()

protected:
	UPROPERTY(Category = "References", BlueprintReadOnly)
	TObjectPtr<APlayerController> OwnedController;

	UPROPERTY(Category = "Events", BlueprintReadWrite)
	bool GeneratePressedEvents = true;
	UPROPERTY(Category = "Events", BlueprintReadWrite)
	bool GenerateReleasedEvents = false;
	UPROPERTY(Category = "Events", BlueprintReadWrite)
	bool GenerateDoublePressedEvents = false;

	bool Raycast(FHitResult& hit) const;
	bool Raycast(FHitResults& hits) const;

	bool RangeRaycast(const float radius, FHitResult& hit) const;
	bool RangeRaycast(const float radius, FHitResult& lineHit, FHitResult& resultHit) const;
	bool RangeRaycast(const float radius, FHitResult& lineHit, FHitResults& hits) const;
	bool RangeRaycast(const float radius, FHitResult& lineHit, FTraceHitResults& hits) const;
	bool RangeRaycast(const float radius, struct FTraceHitRangeResult& hit) const;

	template<class ControllerType>
	inline ControllerType* GetController() const { return Cast<ControllerType>(OwnedController); }

public:
	UBuildTool(const FObjectInitializer& initializer = FObjectInitializer::Get());

	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	FName ToolName = NAME_None;

	// The minimum time between ticks, when time < 0 the tool does not receive ticks (which is better for performance)
	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	float TickTime = -1;

	// Normally gets set during construction by the BuildToolComponent, but can be overridden directly
	UPROPERTY(Category = "Tools", BlueprintReadWrite)
	TSubclassOf<class UToolPropertiesEditorWidget> ToolWidget;

	/* Whether the tool's raycast parameters should be used for selecting objects while the tool is active, if false the default selection parameters are used */
	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	bool UseRaycastParametersForSelection = false;
	/* Customization options for doing raycasts within this tool */
	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	FRaycastParameters RaycastParameters;

	UPROPERTY(Category = "Tools", BlueprintAssignable)
	FToolEvent OnSelectionUpdated;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnStartTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnEndTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void Tick(float delta) { }

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void InitializeTool(APlayerController* controller);
	/* Returns true if the tool supports editing objects in the current selection, if false the tool will be deactivated */
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual bool OnSelectionChanged(const class UObjectSelection* selection) { return false; }

	virtual bool OnKeyChar(const FGeometry& geometry, const FCharacterEvent& event) { return false; }
	virtual bool OnKeyDown(const FGeometry& geometry, const FKeyEvent& event) { return false; }
	virtual bool OnKeyUp(const FGeometry& geometry, const FKeyEvent& event) { return false; }
	virtual bool OnAnalogValueChanged(const FGeometry& geometry, const FAnalogInputEvent& event) { return false; }
	virtual bool OnMouseDown(const FGeometry& geometry, const FPointerEvent& event) { return false; }
	virtual bool OnMouseUp(const FGeometry& geometry, const FPointerEvent& event) { return false; }
	virtual bool OnMouseMove(const FGeometry& geometry, const FPointerEvent& event) { return false; }
	virtual bool OnMouseWheel(const FGeometry& geometry, const FPointerEvent& event) { return false; }
	virtual void OnMouseEnter(const FGeometry& geometry, const FPointerEvent& event) { }
	virtual void OnMouseLeave(const FPointerEvent& event) { }
	virtual bool OnMouseDoubleDown(const FGeometry& geometry, const FPointerEvent& event) { return false; }
};

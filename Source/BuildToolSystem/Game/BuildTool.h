#pragma once

#include "CoreMinimal.h"
#include "BuildTool.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToolSystem, Log, Log);

UCLASS(Abstract, Blueprintable)
class BUILDTOOLSYSTEM_API UBuildTool : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	FName ToolName = NAME_None;

	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	float TickTime = -1;

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnStartTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnEndTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void Tick(float delta) { }

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
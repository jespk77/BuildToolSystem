#pragma once

#include "CoreMinimal.h"
#include "TraceHitResult.h"
#include "../UI/ToolPropertiesWidget.h"
#include "BuildTool.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToolSystem, Log, Log);

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

	bool Raycast(FHitResult& hit, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const;

	bool Raycast(FHitResults& hits, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const;

	bool RangeRaycast(const float radius, FHitResult& hit, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const {
		FHitResult lineHit;
		return RangeRaycast(radius, lineHit, hit, channel, queryParams, responseParams);
	}

	bool RangeRaycast(const float radius, FHitResult& lineHit, FHitResult& resultHit, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const;

	bool RangeRaycast(const float radius, FHitResult& lineHit, FHitResults& hits, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const;

	bool RangeRaycast(const float radius, FHitResult& lineHit, FTraceHitResults& hits, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const;

	bool RangeRaycast(const float radius, FTraceHitRangeResult& hit, const ECollisionChannel& channel = ECC_Visibility,
		const FCollisionQueryParams& queryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& responseParams = FCollisionResponseParams::DefaultResponseParam) const {
		return RangeRaycast(radius, hit.Line, hit.Hits, channel, queryParams, responseParams);
	}

	template<class ControllerType>
	inline ControllerType* GetController() const { return Cast<ControllerType>(OwnedController); }

public:
	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	FName ToolName = NAME_None;

	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	float TickTime = -1;

	UPROPERTY(Category = "Tools", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ToolWidget = UToolPropertiesWidgetBase::StaticClass();

	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnStartTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void OnEndTool() { }
	UFUNCTION(Category = "Tools", BlueprintCallable)
	virtual void Tick(float delta) { }

	virtual void InitializeTool(APlayerController* controller);
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
#include "BuildTool.h"
#include "../TraceHitResult.h"
#include "../BuildToolSettings.h"
#include "../BuildToolSystemLog.h"

#if RAYCAST_DEBUG
#include "Components/LineBatchComponent.h"
#endif

bool UBuildTool::Raycast(FHitResult& hit) const {
	if (!OwnedController) {
		TOOLSYSTEM_LOG(Error, "No owned controller, tool raycast cannot execute");
		return false;
	}

	FVector location, direction;
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) return false;

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World)->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceSingleByChannel(hit, location, end, RaycastParameters.Channel, RaycastParameters.Query, RaycastParameters.Response);
}

bool UBuildTool::Raycast(FHitResults& hits) const {
	if (!OwnedController) {
		TOOLSYSTEM_LOG(Error, "No owned controller, tool raycast cannot execute");
		return false;
	}

	FVector location, direction;
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) return false;

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->GetLineBatcher(UWorld::ELineBatcherType::World)->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceMultiByChannel(hits, location, end, RaycastParameters.Channel, RaycastParameters.Query, RaycastParameters.Response);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& hit) const {
	FHitResult lineHit;
	return RangeRaycast(radius, lineHit, hit);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FHitResult& resultHit) const {
	if (!Raycast(lineHit)) return false;
	return GetWorld()->SweepSingleByChannel(resultHit, lineHit.Location, lineHit.Location, FQuat::Identity,
		RaycastParameters.Channel, FCollisionShape::MakeSphere(radius), RaycastParameters.Query, RaycastParameters.Response);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FHitResults& hits) const {
	if (!Raycast(lineHit)) return false;
	return GetWorld()->SweepMultiByChannel(hits, lineHit.Location, lineHit.Location, FQuat::Identity,
		RaycastParameters.Channel, FCollisionShape::MakeSphere(radius), RaycastParameters.Query, RaycastParameters.Response);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FTraceHitResults& hits) const {
	// as the SweepMultiByChannel only accepts TArray<FHitResult> the results need to be copied over to the new type array
	FHitResults originalHits;
	const bool result = RangeRaycast(radius, lineHit, originalHits);
	hits.Reset(originalHits.Num());
	hits.Append(originalHits);
	return result;
}

bool UBuildTool::RangeRaycast(const float radius, FTraceHitRangeResult& hit) const {
	return RangeRaycast(radius, hit.Line, hit.Hits);
}

UBuildTool::UBuildTool(const FObjectInitializer& initializer) : Super(initializer) {
	RaycastParameters.Channel = GetDefault<UBuildToolSettings>()->SelectionTraceChannel;
}

void UBuildTool::InitializeTool(APlayerController* controller) {
	ensure(controller);
	OwnedController = controller;
}

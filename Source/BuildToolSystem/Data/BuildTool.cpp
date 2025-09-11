#include "BuildTool.h"
#include "BuildToolSystem/TraceHitResult.h"
#include "BuildToolSystem/BuildToolSystem.h"

#define RAYCAST_DEBUG 0

#if RAYCAST_DEBUG
#include "Components/LineBatchComponent.h"
#endif

bool UBuildTool::Raycast(FHitResult& hit, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!OwnedController) {
		UE_LOG(LogToolSystem, Error, TEXT("No owned controller, tool raycast cannot execute"));
		return false;
	}

	FVector location, direction;
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) return false;

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->LineBatcher->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceSingleByChannel(hit, location, end, channel, queryParams, responseParams);
}

bool UBuildTool::Raycast(FHitResults& hits, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!OwnedController) {
		UE_LOG(LogToolSystem, Error, TEXT("No owned controller, tool raycast cannot execute"));
		return false;
	}

	FVector location, direction;
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) return false;

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->LineBatcher->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceMultiByChannel(hits, location, end, channel, queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& hit, const ECollisionChannel& channel, const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	FHitResult lineHit;
	return RangeRaycast(radius, lineHit, hit, channel, queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FHitResult& hit, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!Raycast(lineHit, ECC_Visibility, queryParams)) return false;
	return GetWorld()->SweepSingleByChannel(hit, lineHit.Location, lineHit.Location, FQuat::Identity, channel, FCollisionShape::MakeSphere(radius), queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FHitResults& hits, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!Raycast(lineHit, ECC_Visibility, queryParams)) return false;
	return GetWorld()->SweepMultiByChannel(hits, lineHit.Location, lineHit.Location, FQuat::Identity, channel, FCollisionShape::MakeSphere(radius), queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FTraceHitResults& hits, const ECollisionChannel& channel, const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	// as the SweepMultiByChannel only accepts TArray<FHitResult> the results need to be copied over to the new type array
	FHitResults originalHits;
	const bool result = RangeRaycast(radius, lineHit, originalHits, channel, queryParams, responseParams);
	hits.Reset(originalHits.Num());
	hits.Append(originalHits);
	return result;
}

bool UBuildTool::RangeRaycast(const float radius, FTraceHitRangeResult& hit, const ECollisionChannel& channel, const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	return RangeRaycast(radius, hit.Line, hit.Hits, channel, queryParams, responseParams);
}

#undef RAYCAST_DEBUG

void UBuildTool::InitializeTool(APlayerController* controller) {
	ensure(controller);
	OwnedController = controller;
}

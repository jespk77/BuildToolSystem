#include "BuildTool.h"

DEFINE_LOG_CATEGORY(LogToolSystem);

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
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) {
		UE_LOG(LogToolSystem, Error, TEXT("Failed to deproject mouse position, tool raycast cannot execute"));
		return false;
	}

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->LineBatcher->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceSingleByChannel(hit, location, end, channel, queryParams, responseParams);
}

bool UBuildTool::Raycast(TArray<FHitResult>& hits, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!OwnedController) {
		UE_LOG(LogToolSystem, Error, TEXT("No owned controller, tool raycast cannot execute"));
		return false;
	}

	FVector location, direction;
	if (!OwnedController->DeprojectMousePositionToWorld(location, direction)) {
		UE_LOG(LogToolSystem, Error, TEXT("Failed to deproject mouse position, tool raycast cannot execute"));
		return false;
	}

	const FVector end = location + (direction * OwnedController->HitResultTraceDistance);
#if RAYCAST_DEBUG
	GetWorld()->LineBatcher->DrawLine(location, end, FColor::Red, 0, 5, 10.f);
#endif
	return GetWorld()->LineTraceMultiByChannel(hits, location, end, channel, queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, FHitResult& hit, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!Raycast(lineHit, ECC_Visibility, queryParams)) return false;
	return GetWorld()->SweepSingleByChannel(hit, lineHit.Location, lineHit.Location, FQuat::Identity, channel, FCollisionShape::MakeSphere(radius), queryParams, responseParams);
}

bool UBuildTool::RangeRaycast(const float radius, FHitResult& lineHit, TArray<FHitResult>& hits, const ECollisionChannel& channel,
	const FCollisionQueryParams& queryParams, const FCollisionResponseParams& responseParams) const {
	if (!Raycast(lineHit, ECC_Visibility, queryParams)) return false;
	return GetWorld()->SweepMultiByChannel(hits, lineHit.Location, lineHit.Location, FQuat::Identity, channel, FCollisionShape::MakeSphere(radius), queryParams, responseParams);
}

#undef RAYCAST_DEBUG

void UBuildTool::InitializeTool(APlayerController* controller) {
	ensure(controller);
	OwnedController = controller;
}

#include "ObjectSelectionComponent.h"
#include "BuildToolSystem/Data/ObjectSelection.h"
#include "BuildToolSystem/Data/SelectableObject.h"
#include "BuildToolSystem/BuildToolSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#define SELECTION_DEBUG 0

constexpr float StepDistance = 1000.f, Distance = 30000.f;

bool UObjectSelectionComponent::GetActorsInSelectionBox(FHitResults& hits) const {
	FVector startCenter, startExtents;
	SelectionBoxStart.GetCenterAndExtents(startCenter, startExtents);
	FVector endCenter, endExtents;
	SelectionBoxEnd.GetCenterAndExtents(endCenter, endExtents);

	FVector start = startCenter;
	const FVector direction = (endCenter - startCenter).GetSafeNormal();
	const FQuat rotation = OwnedController->GetControlRotation().Quaternion();
	const ECollisionChannel channel = GetDefault<UBuildToolSettings>()->SelectionTraceChannel;

	uint32 batch = 1;
	for (float distance = 0.f; distance < Distance; distance += StepDistance) {
		FHitResults results;
		const FVector end = start + (direction * StepDistance);
		const FVector extent = FMath::Lerp(startExtents, endExtents, distance / Distance);
		GetWorld()->SweepMultiByChannel(results, start, end, rotation, channel, FCollisionShape::MakeBox(extent));
#if WITH_EDITOR && SELECTION_DEBUG
		DrawDebugBox(GetWorld(), start, extent, FColor::Cyan, false, 5.f, 1, 1.f);
		DrawDebugLine(GetWorld(), start, end, FColor::Cyan, false, 5.f, 1, 1.f);
#endif
		hits.Append(results);
		start = end;
	}

	return !hits.IsEmpty();
}

void UObjectSelectionComponent::SetLocationAtDistance(FVector& locationStart, FVector& locationEnd, const float distance) const {
	FVector location, direction;
	OwnedController->DeprojectMousePositionToWorld(location, direction);
	locationStart = location;
	locationEnd = location + (direction * distance);
}

UObjectSelectionComponent::UObjectSelectionComponent(const FObjectInitializer& initializer) : Super(initializer) {
	PrimaryComponentTick.TickInterval = 0.f;
	PrimaryComponentTick.bCanEverTick = PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UObjectSelectionComponent::BeginPlay() {
	Super::BeginPlay();
	OwnedController = GetOwner<APlayerController>();
	ensureAlways(IsValid(OwnedController));
}

void UObjectSelectionComponent::TickComponent(float delta, ELevelTick tick, FActorComponentTickFunction* function) {
	Super::TickComponent(delta, tick, function);

	if (IsValid(Selection)) {
		for (const UObject* obj : Selection->GetSelectedObjects()) {
			if (IsValid(obj) && obj->Implements<USelectableObject>()) ISelectableObject::Execute_ObjectSelectedTick(obj);
		}
	}
}

void UObjectSelectionComponent::SetSelection(UObjectSelection* newSelection) {
	if (Selection == newSelection) return;

	if (IsValid(Selection)) Selection->InvokeObjectsUnselected();
	Selection = newSelection;
	if (IsValid(Selection)) Selection->InvokeObjectsSelected();

	OnSelectionChanged.Broadcast(Selection);
}

bool UObjectSelectionComponent::OnMouseDown(const FGeometry& geometry, const FPointerEvent& event) {
	if (event.GetEffectingButton() != EKeys::LeftMouseButton) return false;

	IsDragging = true;
	SetLocationAtDistance(SelectionBoxStart.Min, SelectionBoxEnd.Min, Distance);
	SelectionBoxStart.Max = SelectionBoxStart.Min;
	SelectionBoxEnd.Max = SelectionBoxEnd.Max;
	return true;
}

bool UObjectSelectionComponent::OnMouseUp(const FGeometry& geometry, const FPointerEvent& event) {
	if (!IsDragging) return false;

	IsDragging = false;
	FHitResults hits;
	GetActorsInSelectionBox(hits);
	SetSelection(UObjectSelection::FromHitResult(hits));
	return true;
}

bool UObjectSelectionComponent::OnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	if (!IsDragging) return false;

	SetLocationAtDistance(SelectionBoxStart.Max, SelectionBoxEnd.Max, Distance);
	return true;
}

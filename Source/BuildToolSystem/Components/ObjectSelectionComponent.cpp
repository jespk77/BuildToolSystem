#include "ObjectSelectionComponent.h"
#include "BuildToolSystem/Data/ObjectSelection.h"
#include "BuildToolSystem/Data/SelectableObject.h"
#include "BuildToolSystem/BuildToolSettings.h"

#define SELECTION_DEBUG 0

constexpr float StepDistance = 300.f;

bool UObjectSelectionComponent::GetActorsInSelectionBox(FHitResults& hits) const {
	FVector startCenter, startExtents;
	SelectionBoxStart.GetCenterAndExtents(startCenter, startExtents);
	FVector endCenter, endExtents;
	SelectionBoxEnd.GetCenterAndExtents(endCenter, endExtents);

	FVector start = startCenter;
	const FVector direction = (endCenter - startCenter).GetSafeNormal();
	const ECollisionChannel channel = GetDefault<UBuildToolSettings>()->SelectionTraceChannel;
	const float totalDistance = (endCenter - startCenter).Length();
	startExtents.Z = endExtents.Z = StepDistance / 2;

	for (float distance = 0.f; distance < totalDistance; distance += StepDistance) {
		FHitResults results;
		const FVector end = start + (direction * StepDistance);
		const FVector extent = FMath::Lerp(startExtents, endExtents, distance / totalDistance);
		GetWorld()->SweepMultiByChannel(results, start, end, FQuat(), channel, FCollisionShape::MakeBox(extent));
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

	FHitResult hit;
	if (OwnedController->GetHitResultUnderCursor(ECC_Visibility, false, hit)) locationEnd = hit.Location;
	else locationEnd = location + (direction * distance);
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
	if (*Selection == newSelection) return;

	if (IsValid(Selection)) Selection->InvokeObjectsUnselected();
	Selection = newSelection;
	if (IsValid(Selection)) Selection->InvokeObjectsSelected();

	OnSelectionChanged.Broadcast(Selection);
}

bool UObjectSelectionComponent::OnMouseDown(const FGeometry& geometry, const FPointerEvent& event) {
	if (event.GetEffectingButton() != EKeys::LeftMouseButton) return false;

	IsDragging = true;
	SetLocationAtDistance(SelectionBoxStart.Min, SelectionBoxEnd.Min, 10000.f);
	SelectionBoxStart.Max = SelectionBoxStart.Min;
	SelectionBoxEnd.Max = SelectionBoxEnd.Max;
	return true;
}

bool UObjectSelectionComponent::OnMouseUp(const FGeometry& geometry, const FPointerEvent& event) {
	if (!IsDragging) return false;

	IsDragging = false;
	FHitResults hits;
	GetActorsInSelectionBox(hits);

	bool valid = IsValid(Selection);
	if (valid && event.IsShiftDown()) {
		TSet<UObject*> objects;
		if (UObjectSelection::ConvertHitResult(hits, objects)) SetSelection(Selection->SelectedObjectUnion(objects));
		else SetSelection(nullptr);
	}
	else if (valid && event.IsAltDown()) {
		TSet<UObject*> objects;
		if (UObjectSelection::ConvertHitResult(hits, objects)) SetSelection(Selection->SelectedObjectDifference(objects));
		else SetSelection(nullptr);
	}
	else SetSelection(UObjectSelection::FromHitResult(hits));
	return true;
}

bool UObjectSelectionComponent::OnMouseMove(const FGeometry& geometry, const FPointerEvent& event) {
	if (!IsDragging) return false;

	SetLocationAtDistance(SelectionBoxStart.Max, SelectionBoxEnd.Max, 10000.f);
	return true;
}

#undef SELECTION_DEBUG

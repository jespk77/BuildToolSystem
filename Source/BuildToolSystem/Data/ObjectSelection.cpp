#include "ObjectSelection.h"
#include "SelectableObject.h"
#include "../BuildToolSystemLog.h"

bool UObjectSelection::CheckValidObjects(TSet<UObject*>& objects, const bool checkInterface, const bool checkCompatible) {
	UClass* objectClass = nullptr;
	for (TSet<UObject*>::TIterator it = objects.CreateIterator(); it; ++it) {
		UObject* obj = *it;
		if (IsValid(obj)) {
			if (checkInterface) {
				if (!obj->Implements<USelectableObject>() || !ISelectableObject::Execute_CanSelect(obj)) {
					TOOLSYSTEM_LOG(Warning, "Object '%s' either does not implement ISelectableObject or cannot be selected", *obj->GetName());
					it.RemoveCurrent();
					continue;
				}
			}

			if (checkCompatible) {
				UClass* otherClass = obj->GetClass();
				if (!objectClass) objectClass = otherClass;
				else if (!objectClass->IsChildOf(otherClass) && !otherClass->IsChildOf(objectClass)) {
					TOOLSYSTEM_LOG(Warning, "Object types '%s' and '%s' are not compatible", *otherClass->GetName(), *objectClass->GetName());
					it.RemoveCurrent();
					continue;
				}
			}
		}
		else it.RemoveCurrent();
	}

	return !objects.IsEmpty();
}

void UObjectSelection::InvokeObjectsSelected() {
	TOOLSYSTEM_LOG(Verbose, "Invoking selected event on %d objects", SelectedObjects.Num());
	for (UObject* obj : SelectedObjects) {
		if (obj->Implements<USelectableObject>()) ISelectableObject::SelectObject(obj);
	}
}

void UObjectSelection::InvokeObjectsUnselected() {
	TOOLSYSTEM_LOG(Verbose, "Invoking unselected event on %d objects", SelectedObjects.Num());
	for (UObject* obj : SelectedObjects) {
		if (obj->Implements<USelectableObject>()) ISelectableObject::UnselectObject(obj);
	}
}

bool UObjectSelection::ConvertHitResult(const FHitResults& hits, TSet<UObject*>& objects) {
	objects.Reset();
	objects.Reserve(hits.Num());
	for (const FHitResult& hit : hits) objects.Add(hit.GetActor());
	return !objects.IsEmpty();
}

UObjectSelection* UObjectSelection::FromHitResult(const FHitResults& hits) {
	TSet<UObject*> objects;
	if (ConvertHitResult(hits, objects)) return FromSelection(objects);
	else return nullptr;
}

UObjectSelection* UObjectSelection::FromSelection(const TSet<UObject*>& objects) {
	if (objects.IsEmpty()) return nullptr;

	UObjectSelection* selection = NewObject<UObjectSelection>();
	selection->SelectedObjects = objects;
	CheckValidObjects(selection->SelectedObjects, false);
	return selection;
}

void UObjectSelection::GetSelectedObjectsArray(TArray<UObject*>& objects) const {
	objects.Reset(GetObjectCount());
	for (UObject* obj : SelectedObjects) objects.Add(obj);
}

UObjectSelection* UObjectSelection::ClearSelectedObjects(int32 size) const {
	TSet<UObject*> objects;
	if (size > 0) objects.Reserve(size);
	return FromSelection(objects);
}

UObjectSelection* UObjectSelection::SelectedObjectUnion(const TSet<UObject*>& objects) const {
	TSet<UObject*> newSet = SelectedObjects.Union(objects);
	if (!CheckValidObjects(newSet)) return nullptr;
	return FromSelection(newSet);
}

UObjectSelection* UObjectSelection::SelectedObjectDifference(const TSet<UObject*>& objects) const {
	TSet<UObject*> newSet = SelectedObjects.Difference(objects);
	if (!CheckValidObjects(newSet)) return nullptr;
	return FromSelection(newSet);
}

UObjectSelection* UObjectSelection::SetSelectedObjects(TSet<UObject*>& objects) const {
	if (!CheckValidObjects(objects)) return nullptr;
	return FromSelection(objects);
}

bool UObjectSelection::operator==(const UObjectSelection* other) const {
	const bool thisValid = IsValid(this), otherValid = IsValid(other);
	if (!thisValid || !otherValid) return thisValid == otherValid;
	return SelectedObjects.Difference(other->SelectedObjects).IsEmpty();
}

FString UObjectSelection::ToLogString(const bool advanced) const {
	FString result = FString::Format(TEXT("[Object Selection] {0} objects selected"), { GetObjectCount() });
	if (advanced) {
		FStringBuilderBase builder;
		builder.Append(result);
		for (const UObject* obj : SelectedObjects)
			if (obj) builder.Appendf(TEXT("\n - %s"), *obj->GetName());
		return FString(builder);
	}
	else return result;
}

#include "ObjectSelection.h"
#include "SelectableObject.h"

bool UObjectSelection::CheckValidObjects(TSet<UObject*>& objects, const bool checkInterface, const bool checkCompatible) {
	UClass* objectClass = nullptr;
	for (TSet<UObject*>::TIterator it = objects.CreateIterator(); it; ++it) {
		UObject* obj = *it;
		if (IsValid(obj)) {
			if (checkInterface) {
				if (!obj->Implements<USelectableObject>() || !ISelectableObject::Execute_CanSelect(obj)) {
					UE_LOG(LogTemp, Log, TEXT("BuildTool: Object '%s' either does not implement ISelectableObject or cannot be selected"), *obj->GetName());
					it.RemoveCurrent();
					continue;
				}
			}

			if (checkCompatible) {
				UClass* otherClass = obj->GetClass();
				if (!objectClass) objectClass = otherClass;
				else if (!objectClass->IsChildOf(otherClass) && !otherClass->IsChildOf(objectClass)) {
					UE_LOG(LogTemp, Log, TEXT("BuildTool: Object types '%s' and '%s' are not compatible"), *otherClass->GetName(), *objectClass->GetName());
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
	UE_LOG(LogTemp, Log, TEXT("Invoking selected event on %d objects"), SelectedObjects.Num());
	for (UObject* obj : SelectedObjects) {
		if (obj->Implements<USelectableObject>()) ISelectableObject::Execute_OnObjectSelected(obj);
	}
}

void UObjectSelection::InvokeObjectsUnselected() {
	UE_LOG(LogTemp, Log, TEXT("Invoking unselected event on %d objects"), SelectedObjects.Num());
	for (UObject* obj : SelectedObjects) {
		if (obj->Implements<USelectableObject>()) ISelectableObject::Execute_OnObjectUnselected(obj);
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

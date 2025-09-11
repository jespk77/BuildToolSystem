#include "SelectableObject.h"
#include "BuildToolSystem/BuildToolSystem.h"

void ISelectableObject::SelectObject(UObject* obj) {
	if (!IsValid(obj)) return;

	UE_LOG(LogToolSystem, Log, TEXT("Object '%s' selected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = actor->GetComponentByClass<UMeshComponent>()) {
			mesh->SetCustomDepthStencilValue(255.f);
			mesh->SetRenderCustomDepth(true);
		}
	}

	Execute_OnObjectSelected(obj);
}

void ISelectableObject::UnselectObject(UObject* obj) {
	if (!IsValid(obj)) return;

	UE_LOG(LogToolSystem, Log, TEXT("Object '%s' unselected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = actor->GetComponentByClass<UMeshComponent>()) {
			mesh->SetRenderCustomDepth(false);
			mesh->SetCustomDepthStencilValue(0.f);
		}
	}

	Execute_OnObjectUnselected(obj);
}

#include "SelectableObject.h"

bool ISelectableObject::CanSelect_Implementation() const { return true; }

void ISelectableObject::OnObjectSelected_Implementation() {
	UObject* obj = _getUObject();
	if (!IsValid(obj)) return;

	UE_LOG(LogTemp, Log, TEXT("Object '%s' selected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = actor->GetComponentByClass<UMeshComponent>()) {
			mesh->SetCustomDepthStencilValue(255.f);
			mesh->SetRenderCustomDepth(true);
		}
	}
}

void ISelectableObject::OnObjectUnselected_Implementation() {
	UObject* obj = _getUObject();
	if (!IsValid(obj)) return;

	UE_LOG(LogTemp, Log, TEXT("Object '%s' unselected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = actor->GetComponentByClass<UMeshComponent>()) {
			mesh->SetRenderCustomDepth(false);
			mesh->SetCustomDepthStencilValue(0.f);
		}
	}
}

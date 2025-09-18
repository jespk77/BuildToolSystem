#include "SelectableObject.h"
#include "BuildToolSystem/BuildToolSystem.h"

UMeshComponent* ISelectableObject::FindBestMeshComponent(AActor* actor) {
	UMeshComponent* result = nullptr;
	for (UActorComponent* component : actor->GetComponents()) {
		if (UMeshComponent* mesh = Cast<UMeshComponent>(component)) {
			// try to find the non-empty mesh
			if (FBodyInstance* body = mesh->GetBodyInstance())
				if (!body->SourceObject.IsExplicitlyNull()) return mesh;

			// when all meshes on the actor are empty just return the first one
			if (!result) result = mesh;
		}
	}

	return result;
}

void ISelectableObject::SelectObject(UObject* obj) {
	if (!IsValid(obj)) return;

	UE_LOG(LogToolSystem, Log, TEXT("Object '%s' selected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = FindBestMeshComponent(actor)) {
			UE_LOG(LogToolSystem, Log, TEXT("Highlighting mesh component '%s'"), *mesh->GetName());
			mesh->SetCustomDepthStencilValue(255.f);
			mesh->SetRenderCustomDepth(true);
		}
		else UE_LOG(LogToolSystem, Warning, TEXT("Selected object does not have a mesh component to highlight :("));
	}

	Execute_OnObjectSelected(obj);
}

void ISelectableObject::UnselectObject(UObject* obj) {
	if (!IsValid(obj)) return;

	UE_LOG(LogToolSystem, Log, TEXT("Object '%s' unselected"), *obj->GetName());
	if (AActor* actor = Cast<AActor>(obj)) {
		if (UMeshComponent* mesh = FindBestMeshComponent(actor)) {
			UE_LOG(LogToolSystem, Log, TEXT("Removing highlight from mesh component '%s'"), *mesh->GetName());
			mesh->SetRenderCustomDepth(false);
			mesh->SetCustomDepthStencilValue(0.f);
		}
	}

	Execute_OnObjectUnselected(obj);
}

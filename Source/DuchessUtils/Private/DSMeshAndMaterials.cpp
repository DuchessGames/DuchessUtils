
#include "DSMeshAndMaterials.h"

void UDSMeshAndMaterials::Apply(UStaticMeshComponent* MeshComponent)
{
	if (!IsValid(MeshComponent))
	{
		return;
	}

	MeshComponent->SetStaticMesh(Mesh);
	for (int i = 0; i < Materials.Num(); ++i)
	{
		MeshComponent->SetMaterial(i, Materials[i]);
	}
}

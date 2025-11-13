
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSMeshAndMaterials.generated.h"

UCLASS()
class DUCHESSUTILS_API UDSMeshAndMaterials : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	UFUNCTION(BlueprintCallable)
	void Apply(UStaticMeshComponent* MeshComponent);
};

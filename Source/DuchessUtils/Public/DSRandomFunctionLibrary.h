#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSRandomFunctionLibrary.generated.h"


UCLASS()
class DUCHESSUTILS_API UDSRandomFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DSUtils")
	[[nodiscard]] static TArray<int32> GetShuffleIndexesByWeights(const TArray<float>& Weights, int32 OutputSize = -1, const FRandomStream& RandomStream = FRandomStream());

	template <typename T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<T>().RandomWeight), float>>>
	[[nodiscard]] static TArray<int32> GetShuffleIndexesByWeights(const TArray<T>& Weights, int32 OutputSize = -1, const FRandomStream& RandomStream = FRandomStream())
	{
		TArray<float> WeightMap;
		for (T Weight : Weights)
		{
			WeightMap.Emplace(Weight.RandomWeight);
		}
		return GetShuffleIndexesByWeights(WeightMap, OutputSize, RandomStream);
	}

	UFUNCTION(BlueprintCallable, Category = "DSUtils")
	[[nodiscard]] static int32 GetRandomIndexByWeights(const TArray<float>& Weights, const FRandomStream& RandomStream = FRandomStream());
	
	template <typename T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<T>().RandomWeight), float>>>
	[[nodiscard]] static int32 GetRandomIndexByWeights(const TArray<T>& Weights, const FRandomStream& RandomStream = FRandomStream())
	{
		TArray<float> WeightMap;
		for (T Weight : Weights)
		{
			WeightMap.Emplace(Weight.RandomWeight);
		}
		return GetRandomIndexByWeights(WeightMap, RandomStream);
	}

	UFUNCTION(BlueprintCallable, Category = "DSUtils")
	[[nodiscard]] static TArray<int32> GetShuffleIndexesByRarity(const TArray<float>& Rarities, int32 OutputSize = -1, const FRandomStream& RandomStream = FRandomStream());

	template <typename T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<T>().RandomRarity), float>>>
	[[nodiscard]] static TArray<int32> GetShuffleIndexesByRarity(const TArray<T>& Rarities, int32 OutputSize = -1, const FRandomStream& RandomStream = FRandomStream())
	{
		TArray<float> RarityMap;
		for (T Rarity : Rarities)
		{
			RarityMap.Emplace(Rarity.RandomRarity);
		}
		return GetShuffleIndexesByRarity(RarityMap, OutputSize, RandomStream);
	}

	UFUNCTION(BlueprintCallable, Category = "DSUtils")
	[[nodiscard]] static int32 GetRandomIndexByRarity(const TArray<float>& Rarities, const FRandomStream& RandomStream = FRandomStream());

	template <typename T, typename = std::enable_if_t<std::is_same_v<decltype(std::declval<T>().RandomRarity), float>>>
	[[nodiscard]] static int32 GetRandomIndexByRarity(const TArray<T>& Rarities, const FRandomStream& RandomStream = FRandomStream())
	{
		TArray<float> RarityMap;
		for (T Rarity : Rarities)
		{
			RarityMap.Emplace(Rarity.RandomRarity);
		}
		return GetRandomIndexByRarity(RarityMap, RandomStream);
	}
	UFUNCTION(BlueprintCallable, Category = "DSUtils")
	static FString GetAssetDataTest();
};

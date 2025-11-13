
#include "DSRandomFunctionLibrary.h"

#include "Engine/AssetManager.h"

TArray<int32> UDSRandomFunctionLibrary::GetShuffleIndexesByWeights(const TArray<float>& Weights, int32 OutputSize, const FRandomStream& RandomStream)
{
	const int32 N = Weights.Num();

	float WeightSum = 0.f;
	for (float Weight : Weights)
	{
		ensureMsgf(Weight >= 0.f, TEXT("Weight must be non-negative"));
		WeightSum += Weight;
	}
	if (WeightSum <= KINDA_SMALL_NUMBER)
	{
		return {};
	}

	TArray<int32> Counts;
	Counts.Reserve(N);

	if (OutputSize == -1)
	{
		float MinWeight = FLT_MAX;
		for (float Weight : Weights)
		{
			if (Weight > KINDA_SMALL_NUMBER && Weight < MinWeight)
			{
				MinWeight = Weight;
			}
		}

		float Scale = 1.f / MinWeight;

		int32 TotalInt = 0;
		for (int32 i = 0; i < N; ++i)
		{
			int32 Count = FMath::RoundToInt(Weights[i] * Scale);
			Counts.Add(Count);
			TotalInt += Count;
		}

		OutputSize = TotalInt;
	}
	else
	{
		TArray<float> Exact;
		Exact.SetNumUninitialized(N);
		for (int32 i = 0; i < N; ++i)
		{
			Exact[i] = (Weights[i] / WeightSum) * static_cast<float>(OutputSize);
		}

		struct FRem
		{
			float Rem;
			int32 Index;
		};
		TArray<FRem> Remainders;
		int32 Allocated = 0;
		for (int32 i = 0; i < N; ++i)
		{
			int32 Count = FMath::FloorToInt(Exact[i]);
			Counts.Add(Count);
			Allocated += Count;
			Remainders.Add({Exact[i] - Counts[i], i});
		}

		Remainders.Sort([](const FRem& A, const FRem& B)
		{
			return A.Rem > B.Rem;
		});

		for (int32 k = 0; Allocated < OutputSize; ++k, ++Allocated)
		{
			Counts[Remainders[k].Index]++;
		}
	}

	TArray<int32> Result;
	Result.Reserve(OutputSize);
	for (int32 i = 0; i < N; ++i)
	{
		for (int32 c = 0; c < Counts[i]; ++c)
		{
			Result.Add(i);
		}
	}
	for (int32 i = OutputSize - 1; i > 0; --i)
	{
		int32 j = RandomStream.RandRange(0, i);
		Result.Swap(i, j);
	}

	return Result;
}

int32 UDSRandomFunctionLibrary::GetRandomIndexByWeights(const TArray<float>& Weights, const FRandomStream& RandomStream)
{
	if (Weights.Num() == 0)
	{
		return -1;
	}

	TArray<int32> Shuffle =  GetShuffleIndexesByWeights(Weights, -1, RandomStream);
	const int32 RandomIndex = RandomStream.RandRange(0, Shuffle.Num() - 1);
	return Shuffle[RandomIndex];
}

TArray<int32> UDSRandomFunctionLibrary::GetShuffleIndexesByRarity(const TArray<float>& Rarities, int32 OutputSize, const FRandomStream& RandomStream
)
{
	TArray<float> Weights;
	Weights.Reserve(Rarities.Num());

	for (float Rarity : Rarities)
	{
		ensureMsgf(Rarity > 0.f, TEXT("Rarity must be > 0"));
		Weights.Add(1.0f / Rarity);
	}

	return GetShuffleIndexesByWeights(Weights, OutputSize, RandomStream);
}

int32 UDSRandomFunctionLibrary::GetRandomIndexByRarity(const TArray<float>& Rarities, const FRandomStream& RandomStream)
{
	if (Rarities.Num() == 0)
	{
		return -1;
	}

	TArray<int32> Shuffle =  GetShuffleIndexesByRarity(Rarities, -1, RandomStream);
	const int32 RandomIndex = RandomStream.RandRange(0, Shuffle.Num() - 1);
	return Shuffle[RandomIndex];
}

FString UDSRandomFunctionLibrary::GetAssetDataTest()
{

	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> IdList;

	Manager.GetPrimaryAssetIdList(FPrimaryAssetType(FName("DSItemType.Entity.Enemy")), IdList);
	for (auto Element : IdList)
	{
		FAssetData DataAsset;
		Manager.GetPrimaryAssetData(Element, DataAsset);

		FString Quer;
		DataAsset.GetTagValue(FName("ItemRarity"), Quer);

		return Quer;
	}
	return TEXT("");
}

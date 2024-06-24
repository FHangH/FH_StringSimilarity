#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FuncLib_StringSimilarity.generated.h"

UENUM(BlueprintType)
enum class ESimilarityType : uint8
{
	EST_Levenshtein		UMETA(Displayname="莱温斯特编辑距离"),
	EST_Jaccard			UMETA(Displayname="Jaccard")
};

UCLASS(BlueprintType)
class FH_STRINGSIMILARITY_API UFuncLib_StringSimilarity : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	UFUNCTION()
	static int LevenshteinDistance(const FString& Str1, const FString& Str2);
	
public:
	UFUNCTION(BlueprintPure, Category="FH|Similarity", meta=(ToolTip="比较两个字符串的相似度，中英文数字都能算，莱温斯特编辑距离精准(考虑顺序)， Jaccard(不考虑顺序，只考虑是否包含)"))
	static float StringSimilarity(const ESimilarityType EST, const FString& Str1, const FString& Str2);

	UFUNCTION(BlueprintPure, Category="FH|Similarity", meta=(ToolTip="一组字符串和待比较的字符串，计算所有的相似度，返回最大相似度和字符串，中英文数字都能算，莱温斯特编辑距离精准(考虑顺序)， Jaccard(不考虑顺序，只考虑是否包含)"))
	static FString StrArraySimilarity(const ESimilarityType EST, const TArray<FString>& CompareStrArr, const FString& NewStr, float& Similarity);

	static bool IsChinesePunctuationOrSpace(const wchar_t C);
	
	UFUNCTION(BlueprintPure, Category="FH|Similarity", meta=(ToolTip="删除字符串中的空格和标点符号"))
	static FString RemoveSpacesAndSymbolsByString(const FString& Str);

	UFUNCTION(BlueprintPure, Category="FH|Similarity", meta=(ToolTip="字符串转float数组, 支持样式："[1.0, 2.0, 3.0]", [1.0, 2.0, 3.0]"))
	static void StringToFloatArray(const FString& str, bool& IsSucceed, TArray<float>& outArray);
};

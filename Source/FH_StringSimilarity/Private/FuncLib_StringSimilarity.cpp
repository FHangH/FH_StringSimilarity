#include "FuncLib_StringSimilarity.h"
#include <string>
#include <ctype.h>
#include <algorithm>
#include <unordered_set>
#include <vector>

int UFuncLib_StringSimilarity::LevenshteinDistance(const FString& Str1, const FString& Str2)
{
	if (Str1 == Str2)
	{
		return 0;
	}
	const int m = Str1.Len();
	const int n = Str2.Len();
	std::vector T(m + 1, std::vector(n + 1, 0));

	for (int i = 1; i <= m; i++)
	{
		T[i][0] = i;
	}
	for (int j = 1; j <= n; j++)
	{
		T[0][j] = j;
	}
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			const int Weight = Str1[i - 1] == Str2[j - 1] ? 0 : 1;
			T[i][j] = std::min(std::min(T[i-1][j] + 1, T[i][j-1] + 1), T[i-1][j-1] + Weight);
		}
	}
	return T[m][n];
}

float UFuncLib_StringSimilarity::StringSimilarity(const ESimilarityType EST, const FString& Str1, const FString& Str2)
{
	if (EST == ESimilarityType::EST_Jaccard)
	{
		std::string S1(TCHAR_TO_UTF8(*Str1));
		std::string S2(TCHAR_TO_UTF8(*Str2));
		const std::unordered_set Set_S1(S1.begin(), S1.end());
		const std::unordered_set Set_S2(S2.begin(), S2.end());
		int Intersection = 0;
		for (auto c : Set_S1)
		{
			if (Set_S2.find(c) != Set_S2.end())
			{
				++Intersection;
			}
		}
		return static_cast<float>(Intersection) / (Set_S1.size() + Set_S2.size() - Intersection);
	}
	if (EST == ESimilarityType::EST_Levenshtein)
	{
		const int Distance = LevenshteinDistance(Str1, Str2);
		const int MaxLength = std::max(Str1.Len(), Str2.Len());
		if (MaxLength == 0)
		{
			return 0.0f;
		}
		return 1.0f - static_cast<float>(Distance) / static_cast<float>(MaxLength);
	}
	return 0.f;
}

FString UFuncLib_StringSimilarity::StrArraySimilarity(const ESimilarityType EST, const TArray<FString>& CompareStrArr, const FString& NewStr, float& Similarity)
{
	if (CompareStrArr.Num() == 0) return "";
	
	int Index = 0;
	float Temp = 0.f;
	for (int i = 0; i < CompareStrArr.Num(); ++i)
	{
		if (StringSimilarity(EST, CompareStrArr[i], NewStr) > Temp)
		{
			Index = i;
			Temp = StringSimilarity(EST, CompareStrArr[i], NewStr);
		}
	}
	Similarity = Temp;
	return CompareStrArr[Index];
}

bool UFuncLib_StringSimilarity::IsChinesePunctuationOrSpace(const wchar_t C)
{
	const auto Punctuation[] = {
		L'。', L'，', L'、', L'；', L'：', L'‘', L'’', L'“', L'”',
		L'（', L'）', L'【', L'】', L'《', L'》', L'—', L'·', L'…',
		L'！', L' '};

	for (const auto& item : Punctuation)
	{
		if (C == item) return true;
	}
	return false;
}

FString UFuncLib_StringSimilarity::RemoveSpacesAndSymbolsByString(const FString& Str)
{
	std::wstring NewStr = TCHAR_TO_WCHAR(*Str);
	NewStr.erase(
		std::remove_if(
			NewStr.begin(), NewStr.end(), static_cast<int(*)(int)>(&ispunct)),
			NewStr.end()
	);
	NewStr.erase(
		std::remove_if(
			NewStr.begin(), NewStr.end(), static_cast<int(*)(int)>(&isspace)),
			NewStr.end()
	);
	NewStr.erase(
		std::remove_if(
			NewStr.begin(), NewStr.end(), IsChinesePunctuationOrSpace),
			NewStr.end()
	);
	return WCHAR_TO_TCHAR(NewStr.c_str());
}

void UFuncLib_StringSimilarity::StringToFloatArray(const FString& str, bool& IsSucceed, TArray<float>& outArray)
{
	// 确保输入字符串不为空并且长度大于2（至少包含两个方括号）
	if (str.Len() > 2)
	{
		// 去掉字符串的方括号
		FString CleanStr = str;

		// 修改这段，可以适应更多格式的字符串，用于转成float数组
		CleanStr.RemoveFromStart(TEXT("\""));
		CleanStr.RemoveFromEnd(TEXT("\""));
		CleanStr.RemoveFromStart(TEXT("["));
		CleanStr.RemoveFromEnd(TEXT("]"));

		// 分割字符串
		TArray<FString> StringArray;
		CleanStr.ParseIntoArray(StringArray, TEXT(","), true);

		// 将字符串数组中的每个元素转换为浮点数并添加到输出数组中
		for (FString& Elem : StringArray)
		{
			// 去除字符串周围的空格
			Elem = Elem.TrimStartAndEnd();
			outArray.Add(FCString::Atof(*Elem));
		}
		IsSucceed = true;
		return;
	}

	IsSucceed = false;
}

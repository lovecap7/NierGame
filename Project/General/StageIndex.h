#pragma once
#include <string>
enum class StageIndex
{
	Stage1,
	Stage2,
	Stage3,
	Tutorial,
};

inline std::wstring GetStageNameByIndex(StageIndex index)
{
	std::wstring name;
	switch (index)
	{
	case StageIndex::Stage1:
		name = L"Stage1";
		break;
	case StageIndex::Stage2:
		name = L"Stage2";
		break;
	case StageIndex::Stage3:
		name = L"Stage3";
		break;
	case StageIndex::Tutorial:
		name = L"Tutorial";
		break;
	default:
		name = L"Stage1";
		break;
	}
	return name;
}

inline StageIndex GetStageIndexByName(const std::wstring stageName)
{
	StageIndex index;
	if (stageName == L"Stage1")
	{
		index = StageIndex::Stage1;
	}
	else if (stageName == L"Stage2")
	{
		index = StageIndex::Stage2;
	}
	else if (stageName == L"Stage3")
	{
		index = StageIndex::Stage3;
	}
	else if (stageName == L"Tutorial")
	{
		index = StageIndex::Tutorial;
	}
	else
	{
		index = StageIndex::Stage1;
	}

	return index;
}
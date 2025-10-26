#pragma once
#include <string>
enum class StageIndex
{
	Stage1,
	Stage2,
	Stage3,
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
	default:
		name = L"Stage1";
		break;
	}
	return name;
}
#include "AssetManager.h"
#include <cassert>
#include <DxLib.h>
namespace
{
    //モデル
    const std::wstring kModelPath = L"Data/Model/";
    const std::wstring kMV1 = L".mv1";
}

int AssetManager::GetModelHandle(std::wstring path)
{
    int handle = -1;
    //モデルがあったら
    if (m_modelHandles.find(path) != m_modelHandles.end())
    {
        handle = m_modelHandles[path];
    }
    else
    {
        //ハンドルをロードする
        std::wstring loadPath = kModelPath + path + kMV1;
        handle = MV1LoadModel(loadPath.c_str());
        m_modelHandles[path] = handle;
    }
    //ハンドルチェック
    assert(handle >= 0);
    //コピーを渡す
    return MV1DuplicateModel(handle);
}

void AssetManager::DeleteModelHandle()
{
    //ハンドルをすべて削除
    for (const auto& [key, value] : m_modelHandles) 
    {
        MV1DeleteModel(value);
    }
    m_modelHandles.clear();
}

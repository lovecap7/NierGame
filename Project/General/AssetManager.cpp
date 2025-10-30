#include "AssetManager.h"
#include <cassert>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
namespace
{
    //モデル
    const std::wstring kModelPath = L"Data/Model/";
    const std::wstring kMV1 = L".mv1";

    //エフェクト
    const std::wstring kEffectPath = L"Data/Effect/";
    const std::wstring kEfk = L".efkefc";

}

int AssetManager::GetModelHandle(std::wstring path)
{
    int handle = -1;
    //モデルがあったら
    if (m_modelHandles.find(path) != m_modelHandles.end())
    {
        handle = m_modelHandles.at(path);
    }
    else
    {
        //ハンドルをロードする
        std::wstring loadPath = kModelPath + path + kMV1;
        handle = MV1LoadModel(loadPath.c_str());
        m_modelHandles[path] = handle;
    }
    //ハンドルチェック
    //assert(handle >= 0);

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

int AssetManager::GetEffectHandle(std::wstring path)
{
    int handle = -1;
    //モデルがあったら
    if (m_effectHandle.find(path) != m_effectHandle.end())
    {
        handle = m_effectHandle.at(path);
    }
    else
    {
        //ハンドルをロードする
        std::wstring loadPath = kEffectPath + path + kEfk;
        handle = LoadEffekseerEffect(loadPath.c_str());
        m_effectHandle[path] = handle;
    }
    //ハンドルチェック
    assert(handle >= 0);

    //ハンドルを渡す
    return PlayEffekseer3DEffect(handle);


    return 0;
}

void AssetManager::DeleteEffectHandle()
{
    //ハンドルをすべて削除
    for (const auto& [key, value] : m_effectHandle) {
        DeleteEffekseerEffect(value);
    }
    m_effectHandle.clear();
}

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

    //画像
    const std::wstring kImagePath = L"Data/UI/";
    const std::wstring kPng = L".png";
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
    if (m_effectHandles.find(path) != m_effectHandles.end())
    {
        handle = m_effectHandles.at(path);
    }
    else
    {
        //ハンドルをロードする
        std::wstring loadPath = kEffectPath + path + kEfk;
        handle = LoadEffekseerEffect(loadPath.c_str());
        m_effectHandles[path] = handle;
    }

    //ハンドルチェック
    assert(handle >= 0);

    int playHandle = PlayEffekseer3DEffect(handle);
    assert(playHandle >= 0);

    //ハンドルを渡す
    return playHandle;
}

void AssetManager::DeleteEffectHandle()
{
    //ハンドルをすべて削除
    for (const auto& [key, value] : m_effectHandles) {
        DeleteEffekseerEffect(value);
    }
    m_effectHandles.clear();
}

int AssetManager::GetImageHandle(std::wstring path)
{
    int handle = -1;
    //モデルがあったら
    if (m_imageHandles.find(path) != m_imageHandles.end())
    {
        handle = m_imageHandles.at(path);
    }
    else
    {
        //ハンドルをロードする
        std::wstring loadPath = kImagePath + path + kPng;
        handle = LoadGraph(loadPath.c_str());
        m_imageHandles[path] = handle;
    }

    //ハンドルチェック
    assert(handle >= 0);

    return handle;
}

void AssetManager::DeleteImageHandle()
{
    //ハンドルをすべて削除
    for (const auto& [key, value] : m_imageHandles) {
        DeleteGraph(value);
    }
    m_imageHandles.clear();
}

void AssetManager::AllDelete()
{
    DeleteImageHandle();
    DeleteEffectHandle();
    DeleteModelHandle();
}

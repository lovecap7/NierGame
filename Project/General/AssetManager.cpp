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

    //フォントパス
    const std::string kCinzelPath = "Data/Font/Cinzel-Medium.ttf";
    const std::string kNotoPath = "Data/Font/NotoSansJP-Light.ttf";
    const std::string kRobotoPath = "Data/Font/Roboto-Light.ttf";
    //フォントの名前
    const std::wstring kCinzelName = L"Cinzel Medium";
    const std::wstring kNotoName = L"Noto Sans JP Light";
    const std::wstring kRobotoName = L"Roboto Light";

}

void AssetManager::Init()
{
    //フォントの準備
    assert(AddFontResourceExA(kCinzelPath.c_str(), FR_PRIVATE, NULL) > 0);
    assert(AddFontResourceExA(kNotoPath.c_str(), FR_PRIVATE, NULL) > 0);
    assert(AddFontResourceExA(kRobotoPath.c_str(), FR_PRIVATE, NULL) > 0);
}

void AssetManager::End()
{
    //ウィンドウズに一時的に保持していたフォントデータを削除
    RemoveFontResourceExA("", FR_PRIVATE, NULL);
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
    //画像があったら
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
    //assert(handle >= 0);

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

int AssetManager::GetFontHandle(Font font)
{
    int handle = -1;
    //フォントがあったら
    if (m_fontHandles.find(font) != m_fontHandles.end())
    {
        handle = m_fontHandles.at(font);
    }
    else
    {
        auto type = font.type;
        std::wstring name;
        switch (type)
        {
        case AssetManager::FontType::Cinzel:
            name = kCinzelName;
            break;
        case AssetManager::FontType::NotoSansJP:
            name = kNotoName;
            break;
        case AssetManager::FontType::Roboto:
            name = kRobotoName;
            break;
        default:
            name = kCinzelName;
            break;
        }

        //ハンドルをロードする
        handle = CreateFontToHandle(name.c_str(), static_cast<int>(font.size), 5, DX_FONTTYPE_ANTIALIASING);
        m_fontHandles[font] = handle;
    }

    //ハンドルチェック
    assert(handle >= 0);

    return handle;
}

void AssetManager::DeleteFontHandle()
{
    for (auto& [key, value] : m_fontHandles) {
        DeleteFontToHandle(value);
    }
    m_fontHandles.clear();
}

void AssetManager::AllDelete()
{
    DeleteImageHandle();
    DeleteEffectHandle();
    DeleteModelHandle();
    DeleteFontHandle();
}

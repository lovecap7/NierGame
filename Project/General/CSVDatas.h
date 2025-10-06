#pragma once
#include <string>
#include "Battle.h"
#include "../Game/Camera/GameCamera/GameCamera.h"
#include "Math/MyMath.h"
#include "StageIndex.h"
namespace
{
    //アクター毎のスコア
    const std::string kActorScoreDataPath = "Data/CSV/ActorScoreData.csv";
    //ハイスコア
    const std::string kHighScoreDataPath = "Data/CSV/HighScoreData.csv";
    //攻撃データ
    const std::string kCharacterAttackDataPath = "Data/CSV/CharacterAttackData.csv";
    //リザルトスコアUI
    const std::string kResutlScoreDataPath = "Data/CSV/ResutlScoreData.csv";
    //セーブデータ
    const std::string kSaveDataPath = "Data/CSV/SaveData/SaveData.csv";
    //音量
    const std::string kVolumeDataPath = "Data/CSV/VolumeData.csv";
}


//オブジェクトの情報を格納する構造体
struct ActorData {
    std::string     name;               //オブジェクト名
    VECTOR          pos;                //位置
    VECTOR          rot;                //回転（ラジアン）
    VECTOR          scale;              //スケール
    std::string     gropeTag;           //グループタグ
    int             hp;                 //体力
    Battle::Armor   armor;              //アーマー
    int             attackPower;        //攻撃力
    float           speed;              //速度
    float           searchPlaceRang;    //探索範囲
    float           viewingAngle;       //視野角
    float           searchDistance;     //索敵距離
    float           modelRotateSpeed;   //モデル旋回速度
};
//オブジェクトの情報を格納する構造体
struct AttackData {
    std::string                 ownerName;     //キャラクター名
    std::string                 attackName;	   //攻撃名
    int                         damege;		   //ダメージ
    int                         keepFrame;	   //持続
    int                         startFrame;	   //発生
    float                       knockBackPower;//ノックバックの大きさ
    std::string                 anim;	       //アニメーション
    float                       animSpeed;	   //アニメーションの速度
    float                       radius;        //攻撃の半径
    int                         addUltGage;    //攻撃がヒットしたときの加算ゲージ量
    int                         moveFrame;     //前進するフレーム
    float                       moveSpeed;     //移動量
    Battle::AttackWeight        attackWeight;  //攻撃の重さ(これを基準に怯むかどうかを計算)
    int                         hitStopFrame;  //ヒットストップの長さ
    ShakePower                  shakePower;    //カメラの揺れ
    int                         attackNum;     //攻撃回数
};
//スコアの情報を格納する構造体
struct ScoreData {
    std::string dataName;  //データ名
    int         score;     //スコア
};
//リザルトスコアUI情報を格納する構造体
struct ResultScoreUIData {
    std::string     name;   //スコア名
    Vector2         pos;    //位置
    float           scale;  //大きさ
    std::wstring    text;   //幅
};
//UI
struct UIData
{
    std::wstring text;  //テキスト
    Vector2      pos;   //座標
};

struct SaveDatas
{
    //クリアステージ
    bool stage1Clear;
    bool stage2Clear;
    bool stage3Clear;
    //プレイ時間
    unsigned int totalPlayTime;
    //日付と時間
    std::wstring nowTime;
};
//音量
struct VolumeData
{
    int masterVolume;
    int bgmVolume;
    int seVolume;
    int voiceVolume;
};
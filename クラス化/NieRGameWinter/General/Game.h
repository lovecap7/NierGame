#pragma once
//基本16:9 (4:3もOK)
//このサイズ以外は特別な理由がない限り基本ダメ
namespace Game//属性
{
	//画面の大きさを定数定義	16:9
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr int kColorBitNum = 32;
	constexpr int kScreenCenterX = kScreenWidth / 2;
	constexpr int kScreenCenterY = kScreenHeight / 2;
}
#include "DxLib.h"
#include "SceneMgr.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//初期化と裏画面設定と画面の設定
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(640, 480, 16);
	SceneMgr_Read();	////画像の読み込み

	//画面更新 & メッセージ処理 & 画面消去
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		SceneMgr_Update();  //更新
		SceneMgr_Draw();    //描画
	}

	DxLib_End(); // DXライブラリ終了処理

	return 0;
}
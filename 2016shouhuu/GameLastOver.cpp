#include "Dxlib.h"
#include "Scenemgr.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "GameFirst.h"
#include "GameLast.h"
#include "GameSecond.h"
#include "SgameFirst.h"
#include "SgameSecond.h"
#include "Sound.h"

//コントローラー
int GLOpad;

//更新
void GameOverLast_Update()
{
	PlaySoundMem(S_BGM.GameOverS, DX_PLAYTYPE_LOOP, FALSE);
	GLOpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GLOpad & PAD_INPUT_7) && (GLOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (GLOpad & PAD_INPUT_7)   //BACKキーが押されていたら
	{
		StopSoundMem(S_BGM.GameOverS);
		Background_R();
		Enemy_R();
		GameFirst_R();
		GameLast_R();
		GameSecond_R();
		Player_R();
		SGameFirst_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
}

//描画
void GameOverLast_Draw()
{
	LastBackground_Draw();
	Player_LastDraw();
	DrawFormatString(300, 220, GetColor(255, 0, 255), "Game Over");
	DrawFormatString(300, 270, GetColor(150, 150, 150), "矢印をよく見るんだ！");
	DrawFormatString(300, 320, GetColor(255, 255, 255), "BACK でタイトルへ戻ります");
}
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
int GOpad;

//更新
void GameOver_Update()
{
	PlaySoundMem(S_BGM.GameOverS, DX_PLAYTYPE_LOOP, FALSE);
	GOpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GOpad & PAD_INPUT_7) && (GOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (GOpad & PAD_INPUT_7)   //BACKキーが押されていたら
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
void GameOver_Draw()
{
	Background_Draw();
	Player_Draw();
	DrawFormatString(300, 220, GetColor(255, 0, 255), "Game Over");
	DrawFormatString(300, 270, GetColor(255, 0, 255), "%d / 6 体倒しました", EnemyBreak());
	DrawFormatString(300, 320, GetColor(255, 255, 255), "BACK で終了します");
}
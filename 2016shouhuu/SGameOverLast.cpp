#include "Dxlib.h"
#include "Scenemgr.h"
#include "Background.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "SgameLast.h"
#include "SgameSecond.h"
#include "Sound.h"

//コントローラー
int SGLOpad;	//コントローラー

//更新
void SGameOverLast_Update()
{
	PlaySoundMem(S_BGM.GameOverS, DX_PLAYTYPE_LOOP, FALSE);
	SGLOpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGLOpad & PAD_INPUT_7) && (SGLOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	if (SGLOpad & PAD_INPUT_7)   //BACKが押されていたら
	{
		StopSoundMem(S_BGM.GameOverS);
		Background_R();
		Enemy_R();
		PlayerShield_R();
		PlayerSword_R();
		SGameFirst_R();
		SGameLast_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);	//メニュー画面
	}
}

//描画
void SGameOverLast_Draw()
{
	LastBackground_Draw();
	PlayerSword_LastDraw();
	PlayerShield_LastDraw();
	DrawFormatString(300, 220, GetColor(255, 0, 255), "Game Over");
	DrawFormatString(300, 270, GetColor(150, 150, 150), "矢印をよく見て！連携をガンバれ！！");
	DrawFormatString(300, 320, GetColor(255, 255, 255), "BACK でタイトルへ戻ります");
}
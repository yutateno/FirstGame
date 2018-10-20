#include "Background.h"
#include "PlayerSword.h"
#include "PlayerShield.h"
#include "Enemy.h"
#include "DxLib.h"

//コントローラー
int SGpad;

//更新
void SGame_Update()
{
	SGpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGpad & PAD_INPUT_7) && (SGpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	PlayerShield_Update();	//二人プレイの盾の勇者の更新
	Enemy_Update();			//二人プレイの敵の更新
	PlayerSword_Update();	//二人プレイの剣の勇者の更新
}

//描画
void SGame_Draw()
{
	Background_Draw();		//背景の描画
	Limit();				//制限時間のやつ。PlayerSwordにてやってます。ダメージ関連はあそこなので。
	Enemy_Draw();			//二人プレイの敵の描画
	PlayerSword_Draw();		//二人プレイの剣の勇者の描画
	PlayerShield_Draw();	//二人プレイの盾の勇者の描画
}
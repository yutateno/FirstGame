#include "DxLib.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"

//コントローラー
int Gpad;

//更新
void Game_Update()
{
	Gpad = GetJoypadInputState(DX_INPUT_PAD1);
	//コントローラーstart&backかキーボードESCが押されていたら
	if (((Gpad & PAD_INPUT_7) && (Gpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
	{
		DxLib_End(); // DXライブラリを終了する
	}
	Player_Update();	//一人用のプレイヤーの更新
	ENEMY_Update();		//一人用の敵の更新
}

//描画
void Game_Draw()
{
	Background_Draw();	//背景の描画
	LIMIT();			//playerにて行っています。
	ENEMY_Draw();		//一人用の敵の描画
	Player_Draw();		//一人用のプレイヤーの描画
}
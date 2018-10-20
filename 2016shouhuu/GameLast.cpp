#include "DxLib.h"
#include "Background.h"
#include "Enemy.h"
#include "Player.h"
#include "Media.h"

//コントローラー
int GLpad;

//内容
bool LastFlag = 0;	//ゲームオーバーをしっかりするように

//更新
void GameLast_Update()
{
	GLpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GLpad & PAD_INPUT_7) && (GLpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	ENEMY_Update();			//一人用のラスボスの更新
	Player_LastUpdate();	//一人用のプレイヤー（ラスボス用）の更新
	LastFlag = 1;
}

//描画
void GameLast_Draw()
{
	LastBackground_Draw();								//一人用の背景の描画
	LIMIT();											//一人用の制限時間
	LastENEMY_Draw();									//一人用のラスボスの描画
	Player_LastDraw();									//一人用のプレイヤーの描画
	DrawGraph(590, 300, M_Character.PrincessM, TRUE);	//姫
}

//リセット
void GameLast_R()
{
	LastFlag = 0;
}

//ラスボスに入ってる
bool GetLastFlag()
{
	return LastFlag;
}
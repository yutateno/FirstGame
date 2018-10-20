#include "DxLib.h"
#include "Background.h"
#include "Enemy.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "Media.h"

//コントローラー
int SGLpad;	//コントローラー

//内容
bool SLastFlag = 0;	//ラスボスに到達しているかどうか


//更新
void SgameLast_Update()
{
	SGLpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGLpad & PAD_INPUT_7) && (SGLpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキーが押されていたら
	{
		DxLib_End(); // DXライブラリを終了する
	}
	Enemy_Update();				//二人用のラスボスの更新
	PlayerSword_LastUpdate();	//二人用のラスボスでの剣の勇者の更新
	PlayerShield_LastUpdate();	//二人用のラスボスでの盾の勇者の更新
	SLastFlag = 1;
}

//描画
void SgameLast_Draw()
{
	LastBackground_Draw();								//ラスボスでの背景の描画
	Limit();											//二人用の制限時間
	LastEnemy_Draw();									//二人用のラスボスの描画
	PlayerSword_LastDraw();								//二人用のラスボスでの剣の勇者の描画
	PlayerShield_LastDraw();							//二人用のラスボスでの盾の勇者の描画
	DrawGraph(590, 300, M_Character.PrincessM, TRUE);	//姫
}

//リセット
void SGameLast_R()
{
	SLastFlag = 0;
}

//ゲームオーバーをしっかりするためのもの
bool GetSLastFlag()
{
	return SLastFlag;
}
#include "SceneMgr.h"
#include "Background.h"
#include "DxLib.h"
#include "SgameFirst.h"
#include "Sound.h"
#include "Media.h"

//内容
int GFvol;		//BGMのボリューム
int McounT;		//最初の無言移動から終わりまで(ここでの動きを司っている
int ey = 225;	//敵のY座標
double EXMc;	//敵の出現カウント

//コントローラー
int GFpad;

//更新
void GameFirst_Update()
{
	GFpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GFpad & PAD_INPUT_7) && (GFpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Escキー,START&BACK
	{
		DxLib_End(); // DXライブラリを終了する
	}
	Update();	//二人用と同じなので併用（中身はフェードインの処理
	Background_Update();
	McounT++;
}

//描画
void GameFirst_Draw()
{
	//音量の設定(だんだん音を出していく
	GFvol++;
	ChangeVolumeSoundMem(GFvol*2/3, S_BGM.ZakoS);

	//音の再生
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);
	Bright();		//二人用と同じなので併用（中身はフェードインの処理
	Background_Draw();

	if (McounT < 100)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (McounT >= 100 && McounT < 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[1], TRUE);
	}
	if (McounT >= 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(670 - (int)EXMc, ey, M_Enemy.EnemyoM[0], TRUE);
		EXMc += 1.5;
		if ((670 - (int)EXMc) <= 410)
		{
			StopSoundMem(S_BGM.ZakoS);
			SceneMgr_ChangeScene(eScene_Game);	//戦闘画面に映る
		}
	}
}

//リセット
void GameFirst_R()
{
	McounT = 0;
	ey = 225;
	EXMc = 0;
	GFvol = 0;
}
#include "SceneMgr.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//コントローラー
int MSubpad;

//内容
int MenuSubUp;		//上移動
int MenuSubDown;	//下移動
bool SForceEnd;		//強制終了
int SSPoint;		//選択カーソルの位置

//各処理の関数
void OneSub(void);		//一人プレイをする関数
void TwoSub(void);		//二人プレイをする関数
void EndSub(void);		//終了する関数

//処理ポインタ配列
void(*SubMethod[])() =
{
	OneSub,TwoSub,EndSub
};

//描画
void MenuSub_Draw()
{
	SForceEnd = 0;
	// 選択ポインタの位置初期化
	SSPoint = 0;

	// ループ
	while (true)
	{
		// 音量の設定
		ChangeVolumeSoundMem(255, S_BGM.MenuSubS);
		PlaySoundMem(S_BGM.MenuSubS, DX_PLAYTYPE_LOOP, FALSE);
		DrawGraph(0, 0, M_BackGround.TitleSubM, FALSE);
		// カーソル描画
		DrawBox(430, 320 + (SSPoint * 32), 446, 336 + (SSPoint * 32), GetColor(255, 255, 0), TRUE);
		DrawFormatString(462, 320, GetColor(0, 0, 0), "姫で遊ぶ");
		DrawFormatString(462, 352, GetColor(0, 0, 0), "戻る");
		DrawFormatString(462, 384, GetColor(0, 0, 0), "終了する");

		// キー入力に応じて処理
		MSubpad = GetJoypadInputState(DX_INPUT_PAD1);
		//Escキーが押されていたら
		if (((MSubpad & PAD_INPUT_7) && (MSubpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			SForceEnd = 1;
			break;
		}
		//上ボタンが押されたら
		if (((MSubpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
		{
			MenuSubUp++;
		}
		else
		{
			MenuSubUp = 0;
		}
		//下ボタンが押されたら
		if (((MSubpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
		{
			MenuSubDown++;
		}
		else
		{
			MenuSubDown = 0;
		}
		//上移動処理
		if (MenuSubUp == 1 && SSPoint != 0)
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			SSPoint--;
		}
		//下移動処理
		if (MenuSubDown == 1 && SSPoint != 2)	//項目が３つなので
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			SSPoint++;
		}
		//決定キー時処理
		if ((MSubpad & PAD_INPUT_X) || CheckHitKey(KEY_INPUT_X) != 0)	//実際Y決定
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			StopSoundMem(S_BGM.MenuSubS);
			//項目に応じた処理を行う
			SubMethod[SSPoint]();
			break;
		}
	}
	//強制終了
	if (SForceEnd == 1)
	{
		DxLib_End(); // DXライブラリを終了する
	}
}

void OneSub()
{
	SceneMgr_ChangeScene(eScene_UraGame);   //シーンを裏ゲーム画面に変更
}

void TwoSub()
{
	SceneMgr_ChangeScene(eScene_Menu);		//シーンをメニューへ
}

void EndSub()
{
	DxLib_End();							//DXライブラリを終了する
}
#include "Dxlib.h"
#include <math.h>
#include "Scenemgr.h"
#include "Enemy.h"
#include "GameSecond.h"
#include "GameLast.h"
#include "Sound.h"
#include "Media.h"

#define up -2			//上移動値
#define down 2			//下移動値
#define G 9.8067		//重力
#define X_MAX 2.000		//攻撃の移動速度

//内容：勇者
int X;								//X座標
int Y;								//Y座標
int PMoveUFlag;						//上が押されたらフラッグという名のカウント
int PMoveDFlag;						//下が押されたらフラッグという名のカウント
int TIME1;							//攻撃に関するカウント
bool FLAG;							//攻撃ボタンが押されたかどうか
int TIME2;							//攻撃に関するカウント
double T;							//攻撃に関するカウント
int DWeakcoun;						//弱攻撃があたった
int DAmageY;						//ダメージの表示をちょっと遊ぶ
int DStrongcoun;					//強攻撃(火球)が当たったら
int DFLag;							//ダメージ蓄積
int Pattack;						//敵に攻撃が届いたかどうかに関するカウント
int YWeakcoun;						//火球攻撃が当たった
double PlayAttackBar;				//ゲージの長さ
bool RabitFlag;						//攻撃速度上昇可能フラッグ
bool RabitUseFlag;					//攻撃速度上昇を使ったフラッグ
double LastPlayAttackBar = 120;		//ゲージの長さ
bool LastRabitFlag;					//攻撃速度上昇可能フラッグ
bool LastRabitUseFlag;				//攻撃速度上昇を使ったフラッグ

//内容：制限時間
int CCOunt;				//制限時間を始めるフラッグのようなもの
int LimitTimE;			//制限時間の補佐
int LimitStartTimE;		//制限時間の大本
int OneBGMStop;			//ゲームオーバーへのシーンの切り替え時にBGMを止めるためのもの
bool PGOFlag;			//制限時間切れたフラッグ

//コントローラー
int Ppad;		//コントローラー


//雑魚戦の描画
void Player_Draw()
{
	//ゲージ(４体目から使える新しい技
	if (EnemyBreak() >= 3)
	{
		DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
		DrawRectGraph(80, 120, (int)PlayAttackBar, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);
	}
	//新たな剣の特殊技を使ってないとき
	if (RabitUseFlag == 0)
	{
		//初期値
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//真ん中より左の時
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//真ん中より右の時
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
		}
		//ラスボスに入った瞬間おかしくなるのを防ぐ用
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
		}
	}
	//新たな剣の特殊技を使っているとき
	else
	{
		//初期値
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//真ん中より左の時
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//真ん中より右の時
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[1], TRUE);
		}
		//ラスボスに入った瞬間おかしくなるのを防ぐ用
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[2], TRUE);
		}
	}
	//新たな剣を手に入れてないとき
	if (EnemyBreak() < 3)
	{
		//敵の弱攻撃が勇者に通った
		if (EAttackHit() == 1)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
		}
		//敵の強攻撃が勇者に通った
		if (EAttackHit() == 2)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DStrongcoun++;
			SetDrawBright(75, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20秒");
		}
		//剣が火球攻撃をうけた
		if (GetYAttackHit() == 1)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			YWeakcoun++;
			SetDrawBright(125, 125, 125);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20秒");
		}
		//前方で罠
		if (EAttackHit() == 3)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
		}
	}
	//新たな剣を手に入れた時
	else
	{
		//敵の弱攻撃が勇者に通った
		if (EAttackHit() == 1)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
			if (RabitUseFlag == 0)		//ゲージをゼロにする
			{
				PlayAttackBar = 120;
			}
		}
		//敵の強攻撃が勇者に通った
		if (EAttackHit() == 2)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DStrongcoun++;
			SetDrawBright(75, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20秒");
			if (RabitUseFlag == 0)		//ゲージをゼロにする
			{
				PlayAttackBar = 120;
			}
		}
		//剣が稲妻攻撃をうけた
		if (GetYAttackHit() == 1)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			YWeakcoun++;
			SetDrawBright(125, 125, 125);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20秒");
			if (RabitUseFlag == 0)		//ゲージをゼロにする
			{
				PlayAttackBar = 120;
			}
		}
		//前方で罠
		if (EAttackHit() == 3)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
			if (RabitUseFlag == 0)		//ゲージをゼロにする
			{
				PlayAttackBar = 120;
			}
		}
	}
	//弱攻撃の制限時間減少
	if (DWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag++;
		DWeakcoun = 0;
	}
	//強攻撃の制限時間減少
	if (DStrongcoun == 24)
	{
		DAmageY = 0;
		DFLag += 2;
		DStrongcoun = 0;
	}
	//魔法攻撃の制限時間減少
	if (YWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag += 2;
		YWeakcoun = 0;
	}
}

//ラスボスの描画
void Player_LastDraw()
{
	//ゲージ（Y座表はこのままかな
	DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
	DrawRectGraph(80, 120, (int)LastPlayAttackBar, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);

	//新たな剣の特殊技を使ってないとき
	if (LastRabitUseFlag == 0)
	{
		//初期値
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//真ん中より左の時
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//真ん中より右の時
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
		}
		//ラスボスに入ったときおなしくなるのを防ぐ用
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
		}
	}
	//新たな剣の特殊技を使っているとき
	else
	{
		//初期値
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//真ん中より左の時
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//真ん中より右の時
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[1], TRUE);
		}
		//ラスボスに入ったときおなしくなるのを防ぐ用
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[2], TRUE);
		}
	}
	//敵の弱攻撃が勇者に通った
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcoun++;
		SetDrawBright(125, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
		if (LastRabitUseFlag == 0)			//ゲージを０にする
		{
			LastPlayAttackBar = 120;
		}
	}
	//敵の強攻撃が勇者に通った
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcoun++;
		SetDrawBright(75, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20秒");
		if (LastRabitUseFlag == 0)			//ゲージを０にする
		{
			LastPlayAttackBar = 120;
		}
	}
	//剣が稲妻攻撃をうけた
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcoun++;
		SetDrawBright(125, 125, 125);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20秒");
		if (LastRabitUseFlag == 0)			//ゲージを０にする
		{
			LastPlayAttackBar = 120;
		}
	}
	//罠に当たったら
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcoun++;
		SetDrawBright(125, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10秒");
		if (LastRabitUseFlag == 0)			//ゲージを０にする
		{
			LastPlayAttackBar = 120;
		}
	}
	//弱攻撃の制限時間減少
	if (DWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag++;
		DWeakcoun = 0;
	}
	//強攻撃の制限時間減少
	if (DStrongcoun == 24)
	{
		DAmageY = 0;
		DFLag += 2;
		DStrongcoun = 0;
	}
	//魔法攻撃の制限時間減少
	if (YWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag += 2;
		YWeakcoun = 0;
	}
}

//雑魚戦の更新
void Player_Update()
{
	Ppad = GetJoypadInputState(DX_INPUT_PAD1);
	CCOunt++;	//ここで制限時間を始める
	if (CCOunt == 1)
	{
		LimitTimE = GetNowCount();
	}
	//カーソル移動
	if ((Ppad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//上(コントローラー上,キーボード上)
	{
		PMoveUFlag++;
	}
	else
	{
		PMoveUFlag = 0;
	}
	if ((Ppad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)	//下(コントローラー下,キーボード下)
	{
		PMoveDFlag++;
	}
	else
	{
		PMoveDFlag = 0;
	}
	//↑を押されたら上に動く
	if (PMoveUFlag == 1 && Y != up)
	{
		Y -= 2;
		if (RabitUseFlag == 0)	//移動でゲージが増える
		{
			PlayAttackBar -= 5;
		}
	}
	//↓を押されたら下に動く
	if (PMoveDFlag == 1 && Y != down)
	{
		Y += 2;
		if (RabitUseFlag == 0)	//移動でゲージが増える
		{
			PlayAttackBar -= 5;
		}
	}
	//新たな剣の特殊技を使っている状態で攻撃していないとき
	if (RabitUseFlag == 0)
	{
		//aを押されたとき攻撃する
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//コントローラーB,キーボードZ
		{
			Pattack = 0;
			//押されたときの時間をカウントする
			TIME1 = GetNowCount();
			//動いてるフラグを保存する
			FLAG = 1;
		}
		//動いているとき
		if (FLAG == 1)
		{
			//経過時間をカウントする
			TIME2 = GetNowCount();
			//秒に変換して押されたときの経過時間を計算する
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x座標を計算※最後の数値で速さが変わる
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*175.000 / X_MAX);	
			//敵の位置でやめる
			if (X <= 250)
			{
				Pattack++;
			}
			//敵に届いたら動きをやめて初期値に戻る
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//新たな剣の特殊技を使っている状態で攻撃したとき
	else
	{
		//aを押されたとき攻撃する
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//コントローラーB,キーボードZ
		{
			Pattack = 0;
			//押されたときの時間をカウントする
			TIME1 = GetNowCount();
			//動いてるフラグを保存する
			FLAG = 1;
		}
		//動いているとき
		if (FLAG == 1)
		{
			//経過時間をカウントする
			TIME2 = GetNowCount();
			//秒に変換して押されたときの経過時間を計算する
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x座標を計算※最後の数値で速さが変わる
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*300.000 / X_MAX);
			//敵の位置でやめる
			if (X <= 250)
			{
				Pattack++;
			}
			//敵に届いたら動きをやめて初期値に戻る
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//ゲージがたまったら
	if (PlayAttackBar <= 0)
	{
		PlayAttackBar = 0;
		RabitFlag = 1;
	}
	//ゲージを維持させる
	if (RabitFlag == 1)
	{
		PlayAttackBar = 0;
	}
	//特殊技（攻撃速度上昇）が押されたら
	if (EnemyBreak() >= 3 && RabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X == 0)
	{
		RabitUseFlag = 1;
	}
	//初期値以外で押されたら反映しないように
	else if (EnemyBreak() >= 3 && RabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X > 0)
	{
		RabitFlag = 1;
	}
	//ゲージを減らしていく
	if (RabitUseFlag == 1)
	{
		RabitFlag = 0;
		PlayAttackBar += 0.5;
		if (PlayAttackBar >= 120)
		{
			PlayAttackBar = 120;
			RabitUseFlag = 0;
		}
	}
}

//ラスボスでの更新
void Player_LastUpdate()
{
	Ppad = GetJoypadInputState(DX_INPUT_PAD1);
	// カーソル移動
	if ((Ppad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//上(コントローラー上,キーボード上)
	{
		PMoveUFlag++;
	}
	else
	{
		PMoveUFlag = 0;
	}
	if ((Ppad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)	//下(コントローラー下,キーボード下)
	{
		PMoveDFlag++;
	}
	else
	{
		PMoveDFlag = 0;
	}
	//↑を押されたら上に動く
	if (PMoveUFlag == 1 && Y != up)
	{
		Y -= 2;
		if (LastRabitUseFlag == 0)
		{
			LastPlayAttackBar -= 5;
		}
	}
	//↓を押されたら下に動く
	if (PMoveDFlag == 1 && Y != down)
	{
		Y += 2;
		if (LastRabitUseFlag == 0)
		{
			LastPlayAttackBar -= 5;
		}
	}
	if (LastRabitUseFlag == 0)
	{
		//aを押されたときに攻撃する
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//コントローラーB,キーボードA
		{
			Pattack = 0;
			//押されたときの時間をカウントする
			TIME1 = GetNowCount();
			//動いてるフラグを保存する
			FLAG = 1;
		}
		//動いているとき
		if (FLAG == 1)
		{
			//経過時間をカウントする
			TIME2 = GetNowCount();
			//秒に変換して押されたときの経過時間を計算する
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x座標を計算※最後の数値で速さが変わる
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*175.000 / X_MAX);
			//敵の位置でやめる
			if (X <= 250)
			{
				Pattack++;
			}
			//敵に届いたら動きをやめて初期値に戻る
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	else
	{
		//aを押されたとき攻撃する
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//コントローラーB,キーボードZ
		{
			Pattack = 0;
			//押されたときの時間をカウントする
			TIME1 = GetNowCount();
			//動いてるフラグを保存する
			FLAG = 1;
		}
		//動いているとき
		if (FLAG == 1)
		{
			//経過時間をカウントする
			TIME2 = GetNowCount();
			//秒に変換して押されたときの経過時間を計算する
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x座標を計算※最後の数値で速さが変わる
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*300.000 / X_MAX);
			//敵の位置でやめる
			if (X <= 250)
			{
				Pattack++;
			}
			//敵に届いたら動きをやめて初期値に戻る
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//ゲージがたまったら
	if (LastPlayAttackBar <= 0)
	{
		LastPlayAttackBar = 0;
		LastRabitFlag = 1;
	}
	//ゲージを維持させる
	if (LastRabitFlag == 1)
	{
		LastPlayAttackBar = 0;
	}
	//速度上昇が押されたら
	if (LastRabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X == 0)
	{
		LastRabitUseFlag = 1;
	}
	//初期値以外で押されたら
	else if (LastRabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X > 0)
	{
		LastRabitFlag = 1;
	}
	//ゲージを減らしていく
	if (LastRabitUseFlag == 1)
	{
		LastRabitFlag = 0;
		LastPlayAttackBar += 0.5;
		if (LastPlayAttackBar >= 120)
		{
			LastPlayAttackBar = 120;
			LastRabitUseFlag = 0;
		}
	}
}

//制限時間
void LIMIT()
{
	LimitStartTimE = ((GetNowCount() + (DFLag * 10000)) - (GSecondFlag() * 10000));
	OneBGMStop = LimitStartTimE - LimitTimE;
	
	//ゲームオーバー処理
	if ((LimitStartTimE - LimitTimE) >= 300000)
	{
		if (GetLastFlag() == 0)
		{
			PGOFlag = 1;
			SceneMgr_ChangeScene(eScene_GameOver);   //シーンをゲームオーバーに移る
		}
		else
		{
			PGOFlag = 1;
			SceneMgr_ChangeScene(eScene_GameOverLast);   //シーンをゲームオーバーに移る
		}
	}
	else if ((LimitStartTimE - LimitTimE) > 240000)	//最後1
	{
		DrawFormatString(270, 0, GetColor(255, 255, 125), "%d秒", (300000 - (LimitStartTimE - LimitTimE)) / 1000);
	}
	else if ((LimitStartTimE - LimitTimE) <= 240000 && (LimitStartTimE - LimitTimE) > 180000)	//2番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 60);
	}
	else if ((LimitStartTimE - LimitTimE) <= 180000 && (LimitStartTimE - LimitTimE) > 120000)	//3番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 120);
	}
	else if ((LimitStartTimE - LimitTimE) <= 120000 && (LimitStartTimE - LimitTimE) > 60000)	//4番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 180);
	}
	else//最初
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 240);
	}
}

//リセット
void Player_R()
{
	X = 0;
	Y = 0;
	PMoveUFlag = 0;
	PMoveDFlag = 0;
	TIME1 = 0;
	FLAG = 0;
	TIME2 = 0;
	T = 0;
	DWeakcoun = 0;
	DAmageY = 0;
	DStrongcoun = 0;
	DFLag = 0;
	CCOunt = 0;
	LimitTimE = 0;
	LimitStartTimE = 0;
	Pattack = 0;
	YWeakcoun = 0;
	OneBGMStop = 0;
	PGOFlag = 0;
	PlayAttackBar = 0;
	RabitFlag = 0;
	RabitUseFlag = 0;
	LastPlayAttackBar = 120;
	LastRabitFlag = 0;
	LastRabitUseFlag = 0;
}

//位置取得
int GetPlayerXpoint()
{
	return X;
}

int GetPlayerYpoint()
{
	return Y;
}

int GetPattack()
{
	return Pattack;
}

bool GetOneBGMStop()
{
	return PGOFlag;
}

//攻撃速度上昇を使っているかどうか
bool GetRabitUseFlag()
{
	return RabitUseFlag;
}

//攻撃速度上昇を使っているかどうか
bool GetLastRabitUseFlag()
{
	return LastRabitUseFlag;
}
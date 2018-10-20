#include "Dxlib.h"
#include <math.h>
#include "Scenemgr.h"
#include "PlayerShield.h"
#include "Enemy.h"
#include "SgameSecond.h"
#include "SgameLast.h"
#include "Sound.h"
#include "Media.h"

#define g 9.8067	//重力
#define x_max 2.000	//攻撃移動速度
#define UP -2	//上移動値
#define DOWN 2	//下移動値

//内容(勇者)
int x;							//x座標
int y;							//y座標
int time1, time2, flag = 0;		//攻撃モーションに関する変数
double t;						//攻撃に関するカウント
bool AttackShieldHit = 0;		//防御に当たってるか判定
int AttackShieldHitcount = 0;	//数秒当たってるか見せつけ
int PSwMoveUBFlag = 0;			//上移動ボタンが押されたらという名のカウント
int PSwMoveDBFlag = 0;			//下移動ボタンが押されたらという名のカウント
int DFlag;						//どちらかの勇者がダメージを受けた
int DWeakcount = 0;				//弱攻撃の場合の制限時間減少
int DStrongcount = 0;			//今日攻撃の場合の制限時間減少
int damageY;					//ダメージの表示をちょっとかっこよく
int PSattack;					//敵に攻撃が届いたかどうか
int YWeakcount = 0;				//稲妻(火球)に当たった

//内容(制限時間)
int LimitTime;				//制限時間に関する時間を記録
int ccount;					//制限時間の開始を正確にするため
int LimitStartTime;			//制限時間の始まり
int TwoBGMStop;				//ゲームオーバーになった時のBGMを止めるカウント
bool PSGOFlag;				//ゲームオーバーフラッグ

//コントローラー
int PSwpad;		//コントローラー


//プレイヤーの描画
void PlayerSword_Draw()
{
	//初期値と真ん中より左の時と真ん中より右の時で分ける
	if (x == 0)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x < 0)
	{
		DrawGraph(100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x > 0 && x <= 200)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	else
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
	//敵の弱攻撃が勇者に通った
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-10秒");
	}
	//敵の強攻撃が勇者に通った
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcount++;
		SetDrawBright(75, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-20秒");
	}
	//盾が罠に引っかかる
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
	}
	//剣が火球攻撃をうけた
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcount++;
		SetDrawBright(125, 125, 125);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 255, 0), "-10秒");
	}
	//弱攻撃の制限時間減少
	if (DWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		DWeakcount = 0;
	}
	if (YWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		YWeakcount = 0;
	}
	//強攻撃の制限時間減少
	if (DStrongcount == 24)
	{
		damageY = 0;
		DFlag += 2;
		DStrongcount = 0;
	}
}

//ラスボスの描画
void PlayerSword_LastDraw()
{
	//初期値と真ん中より左の時と真ん中より右の時で分ける
	if (x == 0)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x < 0)
	{
		DrawGraph(100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x > 0 && x <= 200)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	else
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
	//敵の弱攻撃が勇者に通った
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-10秒");
	}
	//敵の強攻撃が勇者に通った
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcount++;
		SetDrawBright(75, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-20秒");
	}
	//盾が罠に引っかかる、稲妻攻撃を浮ける
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
	}
	//剣が火球攻撃をうけた
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 255, 0), "-10秒");
	}
	//弱攻撃の体力減少
	if (DWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		DWeakcount = 0;
	}
	if (YWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		YWeakcount = 0;
	}
	//強攻撃の体力減少
	if (DStrongcount == 24)
	{
		damageY = 0;
		DFlag += 2;
		DStrongcount = 0;
	}
}

//更新
void PlayerSword_Update()
{
	PSwpad = GetJoypadInputState(DX_INPUT_PAD1);
	ccount++;	//ここで制限時間をお始める
	if (ccount == 1)
	{
		LimitTime = GetNowCount();
	}
	// カーソル移動
	if ((PSwpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_S) != 0)	//上(コントローラー上,キーボードS)
	{
		PSwMoveUBFlag++;
	}
	else
	{
		PSwMoveUBFlag = 0;
	}
	if ((PSwpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_X) != 0)	//下(コントローラー下,キーボードX)
	{
		PSwMoveDBFlag++;
	}
	else
	{
		PSwMoveDBFlag = 0;
	}
	//sを押されたら上に動く
	if (PSwMoveUBFlag == 1 && y != UP)
	{
		y -= 2;
	}
	//xを押されたら下に動く
	if (PSwMoveDBFlag == 1 && y != DOWN)
	{
		y += 2;
	}
	//aを押されたときに防御がいないとき攻撃する
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y != Getshieldpoint())	//コントローラーB,キーボードA
	{
		PSattack = 0;
		//押されたときの時間をカウントする
		time1 = GetNowCount();
		//動いてるフラグを保存する
		flag = 1;
	}
	//防御がいないときに動いているとき
	if (flag == 1)
	{
		//経過時間をカウントする
		time2 = GetNowCount();
		//秒に変換して押されたときの経過時間を計算する
		t = (double)(time2 - time1) / 1000.000;
		//x座標を計算※最後の数値で速さが変わる
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*200.000 / x_max);
		//敵の位置でやめる
		if (x <= 250)
		{
			PSattack++;
		}
		//敵に届いたら動きをやめて初期値に戻る
		else
		{
			flag = 0;
			x = 0;
			PSattack = 0;
		}
	}
	//防御と位置が被っていたとき
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y == Getshieldpoint())	//コントローラーB,キーボードA
	{
		//押されたときの時間をカウントする
		time1 = GetNowCount();
		//動いてるフラグを保存する
		flag = 2;
	}
	if (flag == 2)
	{
		//経過時間をカウントする
		time2 = GetNowCount();
		//秒に変換して押されたときの経過時間を計算する
		t = (double)(time2 - time1) / 1000.000;
		//x座標を計算※最後の数値で速度が変わる
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*150.000 / x_max);
		//防御の位置でやめる
		if (x <= 90 && x >= 0)
		{
		}
		//防御に届いたら動きをやめて初期値に戻る
		else
		{
			DFlag++;	//味方への攻撃のペナルティ
			AttackShieldHit = 1;
			flag = 0;
			x = 0;
		}
	}
	//防御に当たってるとき数秒当たってる判定を残す
	if (AttackShieldHit == 1)
	{
		AttackShieldHitcount++;
		if (AttackShieldHitcount == 20)
		{
			AttackShieldHit = 0;
			AttackShieldHitcount = 0;
		}
	}
}

//ラスボスでの更新
void PlayerSword_LastUpdate()
{
	PSwpad = GetJoypadInputState(DX_INPUT_PAD1);
	// カーソル移動
	if ((PSwpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_S) != 0)	//上(コントローラー上,キーボードS)
	{
		PSwMoveUBFlag++;
	}
	else
	{
		PSwMoveUBFlag = 0;
	}
	if ((PSwpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_X) != 0)	//下(コントローラー下,キーボードX)
	{
		PSwMoveDBFlag++;
	}
	else
	{
		PSwMoveDBFlag = 0;
	}
	//sを押されたら上に動く
	if (PSwMoveUBFlag == 1 && y != UP)
	{
		y -= 2;
	}
	//xを押されたら下に動く
	if (PSwMoveDBFlag == 1 && y != DOWN)
	{
		y += 2;
	}
	//aを押されたときに防御がいないとき攻撃する
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y != Getshieldpoint())	//コントローラーB,キーボードA
	{
		PSattack = 0;
		//押されたときの時間をカウントする
		time1 = GetNowCount();
		//動いてるフラグを保存する
		flag = 1;
	}
	//防御がいないときに動いているとき
	if (flag == 1)
	{
		//経過時間をカウントする
		time2 = GetNowCount();
		//秒に変換して押されたときの経過時間を計算する
		t = (double)(time2 - time1) / 1000.000;
		//x座標を計算※最後の数値で速さが変わる
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*200.000 / x_max);
		//敵の位置でやめる
		if (x <= 250)
		{
			PSattack++;
		}
		//敵に届いたら動きをやめて初期値に戻る
		else
		{
			flag = 0;
			x = 0;
			PSattack = 0;
		}
	}
	//防御と位置が被っていたとき
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y == Getshieldpoint())	//コントローラーB,キーボードA
	{
		//押されたときの時間をカウントする
		time1 = GetNowCount();
		//動いてるフラグを保存する
		flag = 2;
	}
	if (flag == 2)
	{
		//経過時間をカウントする
		time2 = GetNowCount();
		//秒に変換して押されたときの経過時間を計算する
		t = (double)(time2 - time1) / 1000.000;
		//x座標を計算※最後の数値で速度が変わる
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*150.000 / x_max);
		//防御の位置でやめる
		if (x <= 90 && x >= 0)
		{
		}
		//防御に届いたら動きをやめて初期値に戻る
		else
		{
			DFlag++;	//味方への攻撃のペナルティ
			AttackShieldHit = 1;
			flag = 0;
			x = 0;
		}
	}
	//防御に当たってるとき数秒当たってる判定を残す
	if (AttackShieldHit == 1)
	{
		AttackShieldHitcount++;
		if (AttackShieldHitcount == 20)
		{
			AttackShieldHit = 0;
			AttackShieldHitcount = 0;
		}
	}
}

//制限時間(プレイヤーのダメージに関することがここなので)
void Limit()
{
	LimitStartTime = ((GetNowCount() + (DFlag * 10000) + (Getleftcount() * 10000)) - (SGSecondFlag() * 10000));
	TwoBGMStop = LimitStartTime - LimitTime;
	
	//ゲームオーバー処理
	if ((LimitStartTime - LimitTime) >= 360000)
	{
		if (GetSLastFlag() == 0)
		{
			PSGOFlag = 1;
			SceneMgr_ChangeScene(eScene_SGameOver);   //シーンをゲームオーバーに移る
		}
		else
		{
			PSGOFlag = 1;
			SceneMgr_ChangeScene(eScene_SGameOverLast);
		}
	}
	else if ((LimitStartTime - LimitTime) > 300000)	//最後
	{
		DrawFormatString(300, 0, GetColor(255, 255, 125), "%d秒", (360000 - (LimitStartTime - LimitTime)) / 1000);
	}
	else if ((LimitStartTime - LimitTime) <= 300000 && (LimitStartTime - LimitTime)>240000)	//5番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 125), "%d分%d秒", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 60);
	}
	else if ((LimitStartTime - LimitTime) <= 240000 && (LimitStartTime - LimitTime)>180000)	//4番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 120);
	}
	else if ((LimitStartTime - LimitTime) <= 180000 && (LimitStartTime - LimitTime)>120000)	//3番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 180);
	}
	else if ((LimitStartTime - LimitTime) <= 120000 && (LimitStartTime - LimitTime)>60000)	//2番目
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 240);
	}
	else	//最初
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d分%d秒", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 300);
	}
}

//リセット
void PlayerSword_R()
{
	x = 0;
	y = 0;
	time1 = 0;
	time2 = 0;
	flag = 0;
	t = 0;
	AttackShieldHit = 0;
	AttackShieldHitcount = 0;
	PSwMoveUBFlag = 0;
	PSwMoveDBFlag = 0;
	LimitTime = 0;
	ccount = 0;
	DFlag = 0;
	LimitStartTime = 0;
	DWeakcount = 0;
	DStrongcount = 0;
	damageY = 0;
	PSattack = 0;
	YWeakcount = 0;
	TwoBGMStop = 0;
	PSGOFlag = 0;
}

//位置を得る関数
int GetswordXpoint(void)
{
	return x;
}
int GetswordYpoint(void)
{
	return y;
}
//防御に当たってるかどうか
bool AttackShield()
{
	return AttackShieldHit;
}

int GetPSattack(void)
{
	return PSattack;
}

bool GetTwoBGMStop()
{
	return PSGOFlag;
}
#include "Dxlib.h"
#include "PlayerSword.h"
#include "PlayerShield.h"
#include "Scenemgr.h"
#include "Player.h"
#include "Background.h"
#include "Sound.h"
#include "Media.h"

//敵の座標
#define EAXMove 1	//E(nemy)A(敵)X(座標)Move
#define EAYMove 2	//E(nemy)A(敵)Y(座標)Move
#define EASXMove 2	//E(nemy)A(敵)S(trong)X(座標)Move
#define EAXWait 30	//E(nemy)A(敵)X(座標)Wait
#define EAYWait 60	//だけど一直線に攻撃するので攻撃中のY座標は同じなので併用
#define EASXWait 60	//E(nemy)A(敵)S(trong)X(座標)Wait
#define EAAC 200	//E(nemy)A(敵)A(ttack)C(an)
#define EAACN 250	//E(nemy)A(敵)A(ttack)C(an)N(ot)
#define EX 440		//E(nemy)X(座標)初期値
#define EY 270		//E(nemy)Y(座標)初期値
#define DCC 12		//DamegeColorCount
#define TrapNum 2	//ラスボスでのトラップの個数

//敵に関すること
int count;					//プレイヤーの攻撃数
int Attackcount = 0;		//攻撃するカウント
int Deathcount = 250;		//死亡後の間
int Waitcount = 0;			//攻撃するまでの数秒の間
int EnemyX = 0;				//モーションの移動カウント(X座標
int EnemyY = 0;				//モーションの移動カウント(Y座標
int attackrand = 0;				//攻撃のために移動したか
int check = 7;				//攻撃パターンの乱数を入れる値
int AttackHit = 0;			//敵の攻撃が通った
int AttackHitcount = 0;		//敵の攻撃のカウント
double EnemyXM = 0;			//敵の出現カウント
int AttackMoveCount;		//攻撃の動き
int EnemyArea = 0;			//0真ん中,1上,2下
int EnemyLastArea = 0;		//0真ん中,1上,2下
int YAttackHit = 0;			//火球の攻撃ダメージ
int ATTACKHITcount = 0;		//盾のダメージカウント
int YAttackHITcount = 0;	//火球のダメージカウント
int DamegeColorCount = 12;	//ダメージ受けた時の色変え
bool DamegeColor = 0;		//ダメージ受けた判定
int ShieldMove;				//落ちてる新しい盾の動き

//罠に関すること
int TenmetsuCount;		//罠の発生カウント
int TenmetsuDis;		//点滅として表示させる
int TrapCount;			//罠の出現時間
int TenmetsuFlag;		//罠の発生カウントが値に達したら
int TrapFlag;			//点滅のカウントが値に達したら
int TrapRand;			//どこで罠を出すか
int TrapX, TrapY;		//出すトラップの座標
int WerningY = 140;		//点滅するカウント数(使ってないかも
int ATTACKHIT;			//盾が罠に引っかかった
int TenmetsuTime;		//点滅の表示させるための時間
int TrapRandd[2];		//ラスボスでの二つ目の罠の乱数

//敵の絵に関すること
struct Enemy_E{
	int Gr[4];	//画像データ
	int AC;		//攻撃カウントを割るカウント
};
int EneI = 0;	//添え字
Enemy_E Ene[6];	//6体の敵キャラ

//ラスボス絵の関数に関すること
enum AttackDraw {
	LongNormal, UpAttackCan, MiddleAttackCan, UpAttackCannot
	, MiddleAttackCannot, UnderAttackCan, UnderAttackCannot, ShortNormal
	, MiddleAttackBefore, UnderAttackBefore, UpAttackBefore, UpNormal
	, UnderNormal, UpAttackMoveUpDown, UpAttackMoveMiddle
	, UnderAttackMoveUpDown, UnderAttackMoveMiddle
};

//矢印の絵の関数にかかわること
enum ArrowDraw {
	Strong, Up, Middle, Under
	, MagicUp, MagicMiddle, MagicUnder
};

//読み込み
void Enemy_Read()
{
	//敵１体目から６体目まで
	LoadDivGraph("media\\enemy2.png", 4, 4, 1, 60, 90, Ene[0].Gr);
	LoadDivGraph("media\\enemy3.png", 4, 4, 1, 80, 84, Ene[4].Gr);
	LoadDivGraph("media\\enemy4.png", 4, 4, 1, 80, 80, Ene[2].Gr);
	LoadDivGraph("media\\enemy5.png", 4, 4, 1, 120, 120, Ene[3].Gr);
	LoadDivGraph("media\\enemy6.png", 4, 4, 1, 80, 80, Ene[1].Gr);
	LoadDivGraph("media\\enemy6.png", 4, 4, 1, 80, 80, Ene[5].Gr);
}

//攻撃カウント挿入関数（同じ文を一回で済むように
void Enemy_ReadRead()
{
	//攻撃カウント
	Ene[0].AC = 150;
	Ene[1].AC = 140;
	Ene[2].AC = 130;
	Ene[3].AC = 120;
	Ene[4].AC = 110;
	Ene[5].AC = 100;
}

//二人用の更新
void Enemy_Update()
{
	Enemy_ReadRead();

	//敵の弱攻撃と罠攻撃と強攻撃と魔法攻撃に関する剣と盾のプレイヤーの判定
	if (AttackHit == 1)	//弱攻撃と罠にが当たった
	{
		AttackHitcount++;
		if (AttackHitcount == 20)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (AttackHit == 2)	//強攻撃が当たった
	{
		AttackHitcount++;
		if (AttackHitcount == 25)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (YAttackHit == 1)	//稲妻攻撃に当たった
	{
		YAttackHITcount++;
		if (YAttackHITcount == 20)
		{
			YAttackHit = 0;
			YAttackHITcount = 0;
		}
	}
	if (ATTACKHIT == 1)	//盾が罠に引っかかった
	{
		AttackHit = 3;
		ATTACKHITcount++;
		if (ATTACKHITcount == 20)
		{
			ATTACKHIT = 0;
			AttackHit = 0;
			ATTACKHITcount = 0;
		}
	}
}

//一人用の更新
void ENEMY_Update()
{
	Enemy_ReadRead();

	//敵の弱攻撃と罠攻撃と強攻撃と魔法攻撃に関するプレイヤーの判定
	if (AttackHit == 1 || AttackHit == 3)	//弱攻撃と罠に当たったとき
	{
		AttackHitcount++;
		if (AttackHitcount == 20)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (AttackHit == 2)						//強攻撃に当たったとき
	{
		AttackHitcount++;
		if (AttackHitcount == 25)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (YAttackHit == 1)	//稲妻攻撃に当たった
	{
		YAttackHITcount++;
		if (YAttackHITcount == 20)
		{
			YAttackHit = 0;
			YAttackHITcount = 0;
		}
	}
}

//罠に関する（０なら一人プレイ、１なら二人プレイ
void Enemy_Trap(bool gamemode, int random) 
{
	//罠を出現させるカウントが達したら
	if (TenmetsuCount % 400 == 0 && TenmetsuCount != 0)
	{
		TrapRand = random;	//乱数を入れて出現座標を決める
		TenmetsuFlag = 1;
	}
	//床の点滅させる
	if (TenmetsuFlag == 1)
	{
		TenmetsuDis++;
		TenmetsuTime = GetNowCount();
		if ((TenmetsuTime / 100) % 2 == 0)
		{
			
			if (TrapRand == 1 || TrapRand == 7)			//左
			{
				DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
			}
			else if (TrapRand == 2 || TrapRand == 8)	//左下
			{
				DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
			}
			else if (TrapRand == 3 || TrapRand == 9)	//右上
			{
				if (gamemode == 1)	//二人プレイ
				{
					DrawGraph(200, 170, M_Enemy.TenmetsuM, FALSE);
				}
				else				//一人プレイ
				{
					DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else if (TrapRand == 4)										//右
			{
				if (gamemode == 1)	//二人プレイ
				{
					DrawGraph(200, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else				//一人プレイ
				{
					DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else if (TrapRand == 5)										//右下
			{
				if (gamemode == 1)	//二人プレイ
				{
					DrawGraph(200, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else				//一人プレイ
				{
					DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else																			//左上 0,6
			{
				DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
			}
		}
	}
	//それなりの合図したら罠を出現させる旗をたたせる
	if (TenmetsuDis == 100)
	{
		TenmetsuDis = 0;
		TenmetsuFlag = 0;
		TrapFlag = 1;
	}
	//罠を出現させる
	if (TrapFlag == 1)
	{
		TrapCount++;
		if (gamemode == 0)	//一人プレイ
		{
			if (TrapRand == 0 || TrapRand == 3 || TrapRand == 6 || TrapRand == 9)	//上
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 1 || TrapRand == 4 || TrapRand == 7)				//真ん中
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else																	//下
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
		}
		else //二人プレイ
		{
			if (TrapRand == 0 || TrapRand == 6)	//左上
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 1 || TrapRand == 7)	//左
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 2 || TrapRand == 8)	//左下
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 3 || TrapRand == 9)	//右上
			{
				DrawGraph(200, 170, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == -2)
				{
					ATTACKHIT = 1;
				}
			}
			else if (TrapRand == 4)										//右
			{
				DrawGraph(200, 250, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 0)
				{
					ATTACKHIT = 1;
				}
			}
			else//右下
			{
				DrawGraph(200, 330, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 2)
				{
					ATTACKHIT = 1;
				}
			}
		}
	}
	//罠の出現を終わらせる
	if (TrapCount == 55)
	{
		TrapCount = 0;
		TrapFlag = 0;
	}
}

//二人プレイのラスボスでの罠二つ用
void Enemy_TrapSecond(int random)
{
	//罠を出現させるカウントが達したら
	if (TenmetsuCount % 400 == 0 && TenmetsuCount != 0)
	{
		TrapRandd[0] = random;	//乱数を入れて出現座標を決める
		//二つ目の罠
		if (random <= 2)
		{
			TrapRandd[1] = (random + 2);
		}
		if (random > 2)
		{
			TrapRandd[1] = (random - 3);
		}
		TenmetsuFlag = 1;
	}
	//床の点滅させる
	if (TenmetsuFlag == 1)
	{
		TenmetsuDis++;
		TenmetsuTime = GetNowCount();
		if ((TenmetsuTime / 100) % 2 == 0)
		{
			for (int i = 0; i < TrapNum; i++)
			{
				if (TrapRandd[i] == 1 || TrapRandd[i] == 7)			//左
				{
					DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 2 || TrapRandd[i] == 8)	//左下
				{
					DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 3 || TrapRandd[i] == 9)	//右上
				{
					DrawGraph(200, 170, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 4)										//右
				{
					DrawGraph(200, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 5)										//右下
				{
					DrawGraph(200, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else																			//左上 0,6
				{
					DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
				}
			}
		}
	}
	//それなりの合図したら罠を出現させる旗をたたせる
	if (TenmetsuDis == 100)
	{
		TenmetsuDis = 0;
		TenmetsuFlag = 0;
		TrapFlag = 1;
	}
	//罠を出現させる
	if (TrapFlag == 1)
	{
		TrapCount++;
		for (int i = 0; i < TrapNum; i++)
		{
			if (TrapRandd[i] == 0 || TrapRandd[i] == 6)	//左上
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 1 || TrapRandd[i] == 7)	//左
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 2 || TrapRandd[i] == 8)	//左下
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 3 || TrapRandd[i] == 9)	//右上
			{
				DrawGraph(200, 170, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == -2)
				{
					ATTACKHIT = 1;
				}
			}
			else if (TrapRandd[i] == 4)										//右
			{
				DrawGraph(200, 250, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 0)
				{
					ATTACKHIT = 1;
				}
			}
			else//右下
			{
				DrawGraph(200, 330, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 2)
				{
					ATTACKHIT = 1;
				}
			}
		}
	}
	//罠の出現を終わらせる
	if (TrapCount == 55)
	{
		TrapCount = 0;
		TrapFlag = 0;
	}
}

//ラスボス敵の絵
void Enemy_AttackDraw(AttackDraw AttackD)
{
	switch (AttackD) {
	case LongNormal:	//通常時
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;
		
	case UpAttackCan:	//右上からプレイヤーに攻撃可能時
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackCan:	//真ん中からプレイヤーに攻撃可能時
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackCannot:	//右上からプレイヤーに攻撃できないとき（盾の勇者によって剣の勇者がで守られているなどのとき
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackCannot:	//真ん中からプレイヤーに攻撃できないとき
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackCan:	//右下からプレイヤーに攻撃可能時
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackCannot:	//右下からプレイヤーに攻撃できないとき
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case ShortNormal:	//通常時
		if (count < 30)
		{
			DrawRotaGraph(EX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackBefore:	//真ん中から攻撃直前時
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackBefore:	//右下から攻撃直前時
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackBefore:	//右上から攻撃直前時
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpNormal:	//右上にいる通常時
		if (count < 30)
		{
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderNormal:	//右下にいる通常時
		if (count < 30)
		{
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackMoveUpDown:	//右上での攻撃準備のムーブ時（上下に動く
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackMoveMiddle:	//右上での攻撃準備のムーブ時（横のみ動く
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackMoveUpDown:	//右下での攻撃準備のムーブ時（上下に動く
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackMoveMiddle:	//右下での攻撃準備のムーブ時（横のみ動く
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	default:
		break;
	}
}

//矢印の絵
void Enemy_ArrowDraw(ArrowDraw ArrowD)
{
	switch (ArrowD) {
		
	case Strong:	//強攻撃
		DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
		break;

	case Up:	//右上攻撃
		DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		break;

	case Middle:	//真ん中攻撃
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		break;

	case Under:	//右下攻撃
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
		break;

	case MagicUp:	//右上魔法攻撃
		DrawGraph(300, 170, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	case MagicMiddle:	//真ん中魔法攻撃
		DrawGraph(300, 250, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	case MagicUnder:	//右下魔法攻撃
		DrawGraph(300, 330, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	default:
		break;
	}
}

//二人プレイの攻撃パターン
void Enemy_AttackRand()
{
	if (attackrand == 1)
	{
		Enemy_ArrowDraw(Strong);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
		Waitcount++;
		if (Waitcount >= 45)
		{
			AttackMoveCount++;
			EnemyX -= 50;
			if (AttackMoveCount == 5)
			{
				if (GetswordYpoint() == Getshieldpoint()) //守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //左上で守られていないとき
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //左で守られていないとき
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else //左下で守られていないとき
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 0;
			}
		}
	}
	else if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)) //上で守られていないとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if ((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2)) //左で守られていないとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else													//守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 1;
			}
		}
	}
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == Getshieldpoint())
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2)))	//守られている場合
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//守られていない場合
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 0;
			}
		}
	}
	else if (attackrand == 4)
	{
		//敵の攻撃のチュートリアル的な
		Enemy_ArrowDraw(Under);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2))) //下で守られていないとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == -2))) //左で守られていないとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else //守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 2;
			}
		}
	}
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& GetShieldMahouFlag() == 0 && ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるときで魔法反射可能
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 11;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10
				&& GetShieldMahouFlag() == 1 && ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2))) //上で盾がいないとき（盾と剣が全部同じではないので'＝'でまとめられない
			{
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2))) //剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 1;
			}
		}
	}
	else if (attackrand == 11)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 170, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyArea = 1;
		}
	}
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 12;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)))	//盾がいないとき
			{
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)))	//剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 12)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 250, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
		}
	}
	else if (attackrand == 7)
	{
		//敵の攻撃のチュートリアル的な
		Enemy_ArrowDraw(MagicUnder);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				ATTACKHIT = 1;
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(250, 330, M_Enemy.YellowAttackM, TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 13;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				ATTACKHIT = 1;
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)))		//盾がいないとき
			{
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);

			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 2;
			}
		}
	}
	else if (attackrand == 13)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 330, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyArea = 2;
		}
	}
	//適当な値に入れる形で制御してるのでelseは入れない
}

//二人プレイのラスボスの攻撃パターン
void LastEnemy_AttackRand()
{
	if (attackrand == 1)
	{
		Enemy_ArrowDraw(Strong);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 45)
		{
			AttackMoveCount++;
			EnemyX -= 50;
			if (AttackMoveCount == 5)
			{
				if (GetswordYpoint() == Getshieldpoint()) //守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //左上で守られていないとき
				{
					AttackHit = 2;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //左で守られていないとき
				{
					AttackHit = 2;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else //左下で守られていないとき
				{
					AttackHit = 2;
					Enemy_AttackDraw(UnderAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 0;
			}
		}
	}
	else if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //上で守られていないとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //左で守られていないとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else //守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(UpAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 1;
			}
		}
	}
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == Getshieldpoint())
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2)))	//守られている場合
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else	//守られていない場合
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 0;
			}
		}
	}
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2))) //下で守られていないとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(UnderAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == -2))) //左で守られていないとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else  //守られているとき
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(UnderAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 2;
			}
		}
	}
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UpAttackBefore);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるとき
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 11;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //上で盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2))) //上で盾がいないとき
			{
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2))) //剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(UpNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(UpNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 1;
			}
		}
	}
	else if (attackrand == 11)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(UpAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 170, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyLastArea = 1;
		}
	}
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(MiddleAttackBefore);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 12;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//盾がいるとき
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)))	//盾がいないとき
			{
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)))	//剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 12)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(MiddleAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 250, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
		}
	}
	else if (attackrand == 7)
	{
		Enemy_ArrowDraw(MagicUnder);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UnderAttackBefore);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				ATTACKHIT = 1;
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(250, 330, M_Enemy.YellowAttackM, TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 13;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //盾がいるとき
			{
				ATTACKHIT = 1;
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //盾がいないとき
			{
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);

			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(UnderNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(UnderNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 2;
			}
		}
	}
	else if (attackrand == 13)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(UnderAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 330, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyLastArea = 2;
		}
	}
	//ここにない数字を死亡後と死亡前で制御しているのでelseはないです。
}

//二人プレイのプレイヤーの描画
void Enemy_Draw()
{
	// 音量の設定
	ChangeVolumeSoundMem(255, S_BGM.ZakoS);
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);

	//攻撃パターンの乱数
	int random = GetRand(9);

	Enemy_Trap(1, random);

	//一体目に関すること
	//通常時
	if (count < 10)
	{
		if (EneI == 0)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "操作　攻撃：Ｂ / 移動：左スティック上下");
		}
		if (EneI >= 2)	//3体目から出す
		{
			TenmetsuCount++;	//罠を出現させるカウント
		}
		if (EneI == 3)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "操作　魔法反射：Ｂ (クールダウンに気を付けて)");
		}
		//体力
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPM, FALSE);
		DrawRectGraph(380, 120, (12 * count) - 120, 0, 120, 20, M_Enemy.DAMAGEM, FALSE, FALSE);
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			Deathcount = 250;	//死亡後の間
			check = random;		//攻撃準備の位置パターン
			Attackcount++;
		}
	}
	if (EnemyArea == 0)	//攻撃での敵の位置真ん中---------------------------------------------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//通常時
		{
			DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 0 && EnemyArea == 0)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//一体目攻撃パターン(30倍に来たら攻撃させる)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//強い攻撃パターン
			if (check == 6 || check == 9)
			{
				Enemy_ArrowDraw(Strong);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 3 || check == 7)
			{
				Enemy_ArrowDraw(Up);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else if (check == 1 || check == 4 || check == 8)
			{
				Enemy_ArrowDraw(Middle);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else
			{
				Enemy_ArrowDraw(Under);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//強い攻撃パターン
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else if (check == 1 || check == 4)
			{
				Enemy_ArrowDraw(Middle);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else if (check == 2 || check == 5)
			{
				Enemy_ArrowDraw(Under);//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else if (check == 7)	//魔法攻撃上
			{
				Enemy_ArrowDraw(MagicUp);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
			else if (check == 8)	//魔法攻撃真ん中
			{
				Enemy_ArrowDraw(MagicMiddle);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 6;
				}
			}
			else					//魔法攻撃下
			{
				Enemy_ArrowDraw(MagicUnder);	//敵の攻撃のチュートリアル的な
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		Enemy_AttackRand();	//攻撃パターン
	}
	else if (EnemyArea == 1)	//攻撃に関すること、上にいる-------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//通常時
		{
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == -2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//一体目攻撃パターン(30倍に来たら攻撃させる)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//強い攻撃パターン
			if (check == 6 || check == 7)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 2 || check == 3 || check == 8)
			{
				Enemy_ArrowDraw(Up);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//強い攻撃パターン
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 2 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else if (check == 1 || check == 4 || check == 5)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//稲妻上
			else
			{
				Enemy_ArrowDraw(MagicUp);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
		}
		Enemy_AttackRand();	//攻撃パターン
	}
	else if (EnemyArea == 2)	//攻撃に関すること、下にいる-----------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//通常時
		{
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//一体目攻撃パターン(30倍に来たら攻撃させる)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//強い攻撃パターン
			if (check == 6 || check == 7)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右へ
			else if (check == 0 || check == 1 || check == 4 || check == 8)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else
			{
				Enemy_ArrowDraw(Under);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//強い攻撃パターン
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右へ
			else if (check == 0 || check == 1 || check == 4)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else if (check == 2 || check == 3 || check == 5)
			{
				Enemy_ArrowDraw(Under);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else	//稲妻下
			{
				Enemy_ArrowDraw(MagicUnder);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		Enemy_AttackRand();	//攻撃パターン
	}
	//○体目に関すること
	//○体目の出現に少し間を作る
	if (count == 10 && EneI != 6)
	{
		PlaySoundMem(S_SE.NextenemyS, DX_PLAYTYPE_BACK, FALSE);
		Deathcount--;
		Attackcount = 0;
		EnemyArea = 3;	//消す
		TenmetsuCount = 0;
		Background_Update();	//背景を動かす
		if (EneI == 1)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "罠の予感。点滅が目印だ！");
		}
		//盾手に入れる前
		if (EneI == 2 && Deathcount > 125)
		{
			ShieldMove -= 2;
			DrawRotaGraph(EX + ShieldMove, EY, 1, 0, M_Enemy.ShieldM, TRUE);
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "黄色には気を付けて！普通の盾では防げない！");
		}
		//盾手に入れた後
		if (EneI == 2 && Deathcount <= 125)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "魔法を反射する重い盾を手に入れた！いいね？");
		}
		//カーンを一回だけに
		if (EneI == 2 && Deathcount <= 125 && Deathcount > 85)
		{
			PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, FALSE);
		}
		if (Deathcount <= 100)
		{
			DrawRotaGraph(640 - (int)EnemyXM, EY, 1, 0, Ene[EneI + 1].Gr[0], TRUE);
			EnemyXM += 2;
		}
	}
	//○体目～
	if (Deathcount <= 0 && count >= 10 && count < 20)
	{
		Deathcount = 0;
		count = 0;
		EnemyXM = 0;
		DamegeColor = 0;
		DamegeColorCount = DCC;
		EneI++;
		EnemyArea = 0;
	}
	if (GetTwoBGMStop() == 1)
	{
		StopSoundMem(S_BGM.ZakoS);
	}
	if (EneI == 5 && count == 10)
	{
		DamegeColor = 0;
		DamegeColorCount = DCC;
		TenmetsuCount = 0;
		StopSoundMem(S_BGM.ZakoS);
		SceneMgr_ChangeScene(eScene_SgameSecond);	//ボス前シーン
	}
}

//一人用の描画
void ENEMY_Draw()
{
	// 音量の設定
	ChangeVolumeSoundMem(255, S_BGM.ZakoS);
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);

	//攻撃パターンの乱数
	int random = GetRand(9);

	Enemy_Trap(0, random);

	//一体目に関すること
	//通常時
	if (count < 10)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			Deathcount = 250;	//死亡後の間
			check = random;
			Attackcount++;
			DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		if (EneI == 0)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "操作　攻撃：Ｂ / 移動：左スティック上下");
		}
		if (EneI >= 2)
		{
			TenmetsuCount++;	//罠を出現させるカウント
		}
		if (EneI == 3)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "操作　攻撃速度上昇：Ａ（最初だけ貯めといたよ）");
		}
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPM, FALSE);
		DrawRectGraph(380, 120, (12 * count) - 120, 0, 120, 20, M_Enemy.DAMAGEM, FALSE, FALSE);
	}
	//攻撃が届いたらダメージの絵に変える
	if (count < 10 && attackrand == 0 && EnemyX == 0 && GetPlayerYpoint() == 0)
	{
		if (GetRabitUseFlag() == 0 && GetPattack() == 23)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		if (GetRabitUseFlag() == 1 && GetPattack() == 12)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
	}
	if (DamegeColor == 1 && count != 10)
	{
		DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
		DamegeColorCount--;
		if (DamegeColorCount <= 0)
		{
			DamegeColorCount = DCC;
			DamegeColor = 0;
		}
	}
	//一体目攻撃パターン(30倍に来たら攻撃させる)
	if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
	{
		//通常の攻撃
		//右上へ
		if (check == 0 || check == 3 || check == 6 || check == 9)
		{
			Enemy_ArrowDraw(Up);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//右へ
		else if (check == 1 || check == 4 || check == 7)
		{
			Enemy_ArrowDraw(Middle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//右下へ
		else
		{
			Enemy_ArrowDraw(Under);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
	}
	else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
	{
		//通常の攻撃
		//右上へ
		if (check == 0 || check == 3 || check == 6)
		{
			Enemy_ArrowDraw(Up);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//右へ
		else if (check == 1 || check == 4)
		{
			Enemy_ArrowDraw(Middle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//右下へ
		else if (check == 2 || check == 5)
		{
			Enemy_ArrowDraw(Under);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
		else if (check == 7)	//稲妻攻撃上
		{
			Enemy_ArrowDraw(MagicUp);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 5;
			}
		}
		else if (check == 8)	//稲妻攻撃真ん中
		{
			Enemy_ArrowDraw(MagicMiddle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 6;
			}
		}
		else	//稲妻攻撃下
		{
			Enemy_ArrowDraw(MagicUnder);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 7;
			}
		}
	}
	//通常の攻撃の準備を終えたら
	//左上と左
	if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //上にいたとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (GetPlayerYpoint() == 0) //左にいたとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//下にいたとき
				{
					DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//左
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetPlayerYpoint() == -2 || GetPlayerYpoint() == 2))	//左以外にいたとき
				{
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//左にいたとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				attackrand = 0;
			}
		}
	}
	//左と左下
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //下にいたとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (GetPlayerYpoint() == 0) //左にいたとき
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else //上にいたとき
				{
					DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 5)	//稲妻上
	{
		Enemy_ArrowDraw(MagicUp);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 6)//稲妻真ん中
	{
		Enemy_ArrowDraw(MagicMiddle);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 0)	//剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 7)	//稲妻下
	{
		Enemy_ArrowDraw(MagicUnder);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //剣がいるとき
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//○体目に関すること
	//○体目の出現に少し間を作る
	if (count == 10 && EneI != 6)
	{
		PlaySoundMem(S_SE.NextenemyS, DX_PLAYTYPE_BACK, FALSE);
		Deathcount--;
		TenmetsuCount = 0;
		Attackcount = 0;
		Background_Update();	//背景を動かす
		if (EneI == 1)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "罠の予感。点滅が目印だ！");
		}
		//剣手に入れる前
		if (EneI == 2 && Deathcount > 125)
		{
			ShieldMove -= 2;
			DrawRotaGraph(EX + ShieldMove, EY, 1, 0, M_Character.SwordM, TRUE);	//ShieldMoveは二人用の盾を流用したため
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "黄色い矢印は本気で避けろ！");
		}
		//剣手に入れた後
		if (EneI == 2 && Deathcount <= 125)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "新たな剣を手に入れた！なにやらゲージがついている");
			DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
			DrawGraph(80, 120, M_Character.PAttackBarM, FALSE);
		}
		//カーンを一回だけに
		if (EneI == 2 && Deathcount <= 125 && Deathcount > 85)
		{
			PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, FALSE);
		}
		if (Deathcount <= 100)
		{
			DrawRotaGraph(640 - (int)EnemyXM, EY, 1, 0, Ene[EneI + 1].Gr[0], TRUE);
			EnemyXM += 2;
		}
	}
	//○体目～
	if (Deathcount <= 0 && count >= 10 && count < 20)
	{
		Deathcount = 0;
		count = 0;
		EnemyXM = 0;
		DamegeColorCount = DCC;
		DamegeColor = 0;
		TenmetsuCount = 0;
		EneI++;
	}
	if (GetOneBGMStop() == 1)
	{
		StopSoundMem(S_BGM.ZakoS);
	}
	if (EneI == 5 && count == 10)
	{
		DamegeColorCount = DCC;
		DamegeColor = 0;
		TenmetsuCount = 0;
		StopSoundMem(S_BGM.ZakoS);
		SceneMgr_ChangeScene(eScene_GameSecond);	//ボス前シーン
	}
}

//二人用のラスボスの描画
void LastEnemy_Draw()
{
	PlaySoundMem(S_BGM.BossS, DX_PLAYTYPE_LOOP, FALSE);

	//攻撃パターンの乱数
	int random = GetRand(9);

	Enemy_TrapSecond(random);
	
	//通常時
	if (count < 50)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			check = random;
			Attackcount++;
		}
		TenmetsuCount++;	//罠を出現させるカウント
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
		//体力バー
		if (count <= 30)
		{
			DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
			DrawRectGraph(380, 120, (6 * (count-10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
		else
		{
			DrawGraph(380, 120, M_Enemy.DAMAGEM, FALSE);
			DrawRectGraph(380, 120, (6 * (count-10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
	}
	if (EnemyLastArea == 0)	//真ん中---------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(ShortNormal);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 0)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//攻撃パターン
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//強い攻撃パターン
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else if (check == 1)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else if (check == 2 || check == 5)
			{
				Enemy_ArrowDraw(Under);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else if (check == 7)	//稲妻上
			{
				Enemy_ArrowDraw(MagicUp);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
			else if (check == 4 || check == 8)	//稲妻真ん中
			{
				Enemy_ArrowDraw(MagicMiddle);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 6;
				}
			}
			else		//稲妻下
			{
				Enemy_ArrowDraw(MagicUnder);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		LastEnemy_AttackRand();	//攻撃パターン
	}
	else if (EnemyLastArea == 1)	//上-------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(UpNormal);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == -2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//攻撃パターン
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//強い攻撃パターン
			if (check == 2 || check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(UpAttackMoveUpDown);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右上へ
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				Enemy_AttackDraw(UpAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//右へ
			else if (check == 1 || check == 5)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(UpAttackMoveUpDown);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//稲妻上
			else
			{
				Enemy_ArrowDraw(MagicUp);
				Enemy_AttackDraw(UpAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
		}
		LastEnemy_AttackRand();	//攻撃パターン
	}
	if (EnemyLastArea == 2)	//下-------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(UnderNormal);
		}
		//攻撃が届いたらダメージの絵に変える
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//攻撃パターン
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//強い攻撃パターン
			if (check == 1 || check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(UnderAttackMoveUpDown);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//通常の攻撃
			//右へ
			else if (check == 0 || check == 4)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(UnderAttackMoveUpDown);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//右下へ
			else if (check == 2  || check == 5)
			{
				Enemy_ArrowDraw(Under);
				Enemy_AttackDraw(UnderAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			//稲妻下
			else
			{
				Enemy_ArrowDraw(MagicUnder);
				Enemy_AttackDraw(UnderAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		LastEnemy_AttackRand();	//攻撃パターン
	}
	if (GetTwoBGMStop() == 1)
	{
		StopSoundMem(S_BGM.BossS);
	}
	if (count >= 50)
	{
		StopSoundMem(S_BGM.BossS);
		SceneMgr_ChangeScene(eScene_SgameEnd);	//end
	}
}

//一人用のラスボスの描画
void LastENEMY_Draw()
{
	PlaySoundMem(S_BGM.BossS, DX_PLAYTYPE_LOOP, FALSE);

	//攻撃パターンの乱数
	int random = GetRand(9);

	Enemy_Trap(0, random);

	//通常時
	if (count < 50)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			check = random;
			Attackcount++;
		}
		TenmetsuCount++;	//罠を出現させるカウント
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
		//体力バー
		if (count <= 30)
		{
			DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
			DrawRectGraph(380, 120, (6 * (count - 10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
		else
		{
			DrawGraph(380, 120, M_Enemy.DAMAGEM, FALSE);
			DrawRectGraph(380, 120, (6 * (count - 10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
	}
	if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
	{
		Enemy_AttackDraw(LongNormal);
	}
	//攻撃が届いたらダメージの絵に変える
	if (count < 50 && attackrand == 0 && EnemyX == 0 && GetPlayerYpoint() == 0)
	{
		if (GetLastRabitUseFlag() == 0 && GetPattack() == 23)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		else if (GetLastRabitUseFlag() == 0 && GetPattack() == 23 && count == 29)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
			attackrand = 15;
		}
		if (GetLastRabitUseFlag() == 1 && GetPattack() == 12)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		else if (GetLastRabitUseFlag() == 1 && GetPattack() == 12 && count == 29)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
			attackrand = 15;
		}
	}
	if (DamegeColor == 1)
	{
		SetDrawBright(125, 0, 0);
		DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		DamegeColorCount--;
		if (DamegeColorCount <= 0)
		{
			DamegeColorCount = DCC;
			DamegeColor = 0;
		}
	}
		//一体目攻撃パターン(30倍に来たら攻撃させる)
	if ((Attackcount % 95) == 0 && count < 50)
	{
		//通常の攻撃
		//右上へ
		if (check == 0 || check == 6 || check == 8)
		{
			Enemy_ArrowDraw(Up);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//右へ
		else if (check == 1 || check == 4)
		{
			Enemy_ArrowDraw(Middle);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//右下へ
		else if (check == 2 || check == 5)
		{
			Enemy_ArrowDraw(Under);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
		//稲妻上
		else if (check == 7)
		{
			Enemy_ArrowDraw(MagicUp);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 5;
			}
		}
		//稲妻真ん中
		else if (check == 3)
		{
			Enemy_ArrowDraw(MagicMiddle);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 6;
			}
		}
		//稲妻下
		else
		{
			Enemy_ArrowDraw(MagicUnder);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 7;
			}
		}
	}
		//通常の攻撃の準備を終えたら
		//左上と左
	if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //上にいるとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (GetPlayerYpoint() == 0) //左にいるとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else	//下にいるとき
				{
					Enemy_AttackDraw(UpAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
		//左
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetPlayerYpoint() == -2 || GetPlayerYpoint() == 2))	//左以外にいる
				{
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else	//左にいるとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				attackrand = 0;
			}
		}
	}
		//左と左下
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //下にいるとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(UnderAttackCan);
				}
				else if (GetPlayerYpoint() == 0) //左にいるとき
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else  //上にいるとき
				{
					Enemy_AttackDraw(UnderAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//稲妻上
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UpAttackBefore);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//稲妻真ん中
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(MiddleAttackBefore);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 0)	//剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//稲妻下
	else if (attackrand == 7)
	{
		Enemy_ArrowDraw(MagicUnder);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UnderAttackBefore);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //剣がいるとき
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//いないとき
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	if (GetOneBGMStop() == 1)
	{
		StopSoundMem(S_BGM.BossS);
	}
	if (count >= 50)
	{
		StopSoundMem(S_BGM.BossS);
		SceneMgr_ChangeScene(eScene_GameEnd);	//end
	}
}

//リセット
void Enemy_R()
{
	ShieldMove = 0;
	count = 0;
	Attackcount = 0;
	Deathcount = 250;
	Waitcount = 0;
	EnemyX = 0;
	EnemyY = 0;
	attackrand = 0;
	check = 7;
	AttackHit = 0;
	AttackHitcount = 0;
	EnemyXM = 0;
	AttackMoveCount = 0;
	EnemyArea = 0;
	EnemyLastArea = 0;
	YAttackHit = 0;
	ATTACKHITcount = 0;
	YAttackHITcount = 0;
	TenmetsuCount = 0;
	TenmetsuDis = 0;
	TrapCount = 0;
	TenmetsuFlag = 0;
	TrapFlag = 0;
	TrapRand = 0;
	TrapX = 0;
	TrapY = 0;
	WerningY = 140;
	ATTACKHIT = 0;
	TenmetsuTime = 0;
	EneI = 0;
	DamegeColorCount = 12;
	DamegeColor = 0;
	for (int i = 0; i < TrapNum; i++)
	{
		TrapRandd[i] = 0;
	}
}

//攻撃が通った
int EAttackHit(void)
{
	return AttackHit;
}

//何体目か
int EnemyBreak(void)
{
	return EneI;
}

//罠で盾へ攻撃
int PShATTACKHIT(void)
{
	return ATTACKHIT;
}

int GetYAttackHit(void)
{
	return YAttackHit;
}
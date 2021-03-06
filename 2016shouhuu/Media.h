#pragma once

//背景
struct Media_BackGround {
	int BackGroundM;		//雑魚戦背景
	int LastBackGroundM;	//ラスボス戦背景
	int IntroM;				//説明画面背景
	int BlackM;				//エンディングの背景
	int TitleM;				//メニューの背景
	int TitleSubM;			//裏メニューの背景
	int UraBackGroundM;		//裏の背景
	int SecondBackGroundM;	//雑魚戦後半の背景
};

extern Media_BackGround M_BackGround;

//キャラ
struct Media_Character {
	int PlayerSwordM[3];		//剣の勇者の絵
	int PrincessM;				//プリンセスの絵
	int PlayerShieldM;			//盾の勇者の絵
	int PlayerShieldMahouM;		//魔法反射の盾を持ってる勇者の絵
	int PlayerShieldCoolDownM;	//クールダウン中の盾の勇者の絵
	int SwordM;					//剣の絵
	int PAttackBarM;			//一人用の勇者のゲージ
	int PlayerSwordYellowM[3];	//剣の勇者の早いとき
};

extern Media_Character M_Character;

//敵画像
struct Media_Enemy {
	//敵
	int LastEnemyM;				//ラスボスの絵
	int EnemyoM[4];				//一体目の敵
	int UraEnemyM;				//裏の敵
	//敵関連
	int UraSentakuM;			//裏の選択ボタン
	int AttackArrowM;			//矢印の絵
	int HPM;					//体力の画像
	int DAMAGEM;				//体力のダメージ画像
	int TrapM;					//罠の画像
	int TenmetsuM;				//点滅とする画像
	int HPsecondM;				//ラスボスでの二つ目の体力の描画
	int HelpM;					//助言
	int AttackYellowArrowM;		//敵の火球での攻撃合図
	int YellowAttackM;			//火球
	int AttackRedArrowM;		//敵の火球改での攻撃合図
	int RedAttackM;				//火球改
	int ShieldM;				//魔法シールド
	int ShieldMoveM;			//盾移動
};

extern Media_Enemy M_Enemy;

//読み込み
void Media_Read();
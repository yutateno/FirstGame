#pragma once

//BGM
struct Sound_BGM {
	int SetumeiS;	//説明画面でのBGM
	int GameOverS;	//ゲームオーバーのBGM
	int MenuS;		//メニュー画面件エンディングのBGM
	int ZakoS;		//雑魚戦のBGM
	int BossS;		//BOSS戦のBGM
	int MenuSubS;	//裏メニューのBGM
	int UraBGMM;	//裏ゲームのBGM
	int AsiotoS;	//ボス前のBGM
};

extern Sound_BGM S_BGM;

//SE
struct Sound_SE {
	int BotanS;		//操作ボタンのSE
	int AttackS;	//剣の攻撃のSE
	int HitS;		//敵の攻撃のSE
	int ShieldS;	//盾の防御のSE
	int NextenemyS;	//敵が死んだときのSE
	int BonS;		//火球のSE
	int ReturnHitS;	//魔法反射で敵がダメージ受けたときのSE
};

extern Sound_SE S_SE;

//読み込み
void Sound_Read();
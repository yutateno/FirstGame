#include "DxLib.h"
#include "SGame.h"
#include "Game.h"
#include "Menu.h"
#include "SOpening.h"
#include "SceneMgr.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "Opening.h"
#include "SGameOver.h"
#include "SgameSecond.h"
#include "SgameLast.h"
#include "SGameOverLast.h"
#include "SgameEnd.h"
#include "GameFirst.h"
#include "GameOver.h"
#include "GameSecond.h"
#include "GameLast.h"
#include "GameLastOver.h"
#include "GameEnd.h"
#include "Sound.h"
#include "Media.h"
#include "Menusub.h"
#include "UraGame.h"

//画像の読み込み
void SceneMgr_Read()
{
	Enemy_Read();
	Sound_Read();
	Media_Read();
}

static eScene Scene = eScene_Menu;    //シーン管理変更

//更新
void SceneMgr_Update()
{
	//シーンによって処理を分岐
	switch (Scene)
	{ 
	case eScene_UraGame:		//裏ゲーム
		UraGame_Update();		//更新処理
		break;
	
	case eScene_Game:			//一人プレイ画面
		Game_Update();			//更新処理
		break;

	case eScene_GameFirst:		//一人用のオープニング後
		GameFirst_Update();		//更新処理
		break;

	case eScene_GameOver:		//一人用のゲームオーバー
		GameOver_Update();		//更新処理
		break;

	case eScene_GameSecond:		//一人用の雑魚戦後
		GameSecond_Update();	//更新処理
		break;

	case eScene_GameLast:		//一人用のラスボス
		GameLast_Update();		//更新処理
		break;

	case eScene_GameOverLast:	//一人用のボス戦ゲームオーバー
		GameOverLast_Update();	//更新処理
		break;

	case eScene_GameEnd:		//一人用のエンディング
		GameEnd_Update();		//更新処理
		break;

	case eScene_SGame:			//二人プレイ画面
		SGame_Update();			//更新処理
		break;

	case eScene_SgameFirst:		//二人プレイのオープニング後の画面
		SgameFirst_Update();	//更新処理
		break;

	case eScene_SGameOver:		//二人プレイの雑魚戦でのゲームオーバー画面
		SGameOver_Update();		//更新処理
		break;

	case eScene_SgameSecond:	//二人プレイのボス前画面
		SgameSecond_Update();	//更新処理
		break;

	case eScene_SgameLast:		//二人プレイのボス戦
		SgameLast_Update();		//更新処理
		break;

	case eScene_SGameOverLast:	//二人プレイのラスボスのゲームオーバー
		SGameOverLast_Update();	//更新処理
		break;

	case eScene_SgameEnd:		//二人用のエンディング
		SgameEnd_Update();		//更新処理
		break;

	default:
		break;
	}
}

//描画
void SceneMgr_Draw()
{
	//シーンによって処理を分岐
	switch (Scene)
	{ 
	case eScene_Menu:			//メニュー画面
		Menu_Draw();			//描画処理
		break;

	case eScene_MenuSub:		//裏メニュー
		MenuSub_Draw();			//描画処理
		break;

	case eScene_UraGame:		//裏ゲーム
		UraGame_Draw();			//描画処理
		break;

	case eScene_Game:			//一人プレイ画面
		Game_Draw();			//描画処理
		break;

	case eScene_Opening:		//一人プレイのオープニング画面
		Opening_Draw();			//描画処理
		break;

	case eScene_GameFirst:		//一人用の最初
		GameFirst_Draw();		//描画処理
		break;

	case eScene_GameOver:		//一人用のゲームオーバー
		GameOver_Draw();		//描画処理
		break;

	case eScene_GameSecond:		//一人用の雑魚戦後
		GameSecond_Draw();		//描画処理
		break;

	case eScene_GameLast:		//一人用のラスボス戦
		GameLast_Draw();		//描画処理
		break;

	case eScene_GameOverLast:	//一人用のゲームオーバー
		GameOverLast_Draw();	//描画処理
		break;

	case eScene_GameEnd:		//一人用のエンディング
		GameEnd_Draw();			//描画処理
		break;

	case eScene_SGame:			//二人プレイ画面
		SGame_Draw();			//描画処理
		break;

	case eScene_SOpening:		//二人プレイのオープニング画面
		SOpening_Draw();		//描画処理
		break;

	case eScene_SgameFirst:		//二人プレイの最初の画面
		SgameFirst_Draw();		//描画処理
		break;

	case eScene_SGameOver:		//二人プレイのゲームオーバー画面
		SGameOver_Draw();		//描画処理
		break;

	case eScene_SgameSecond:	//二人プレイのボス前画面
		SgameSecond_Draw();		//描画処理	
		break;

	case eScene_SgameLast:		//二人用のラスボス戦
		SgameLast_Draw();		//描画処理
		break;

	case eScene_SGameOverLast:	//二人用のラスボスのゲームオーバー
		SGameOverLast_Draw();	//描画処理
		break;

	case eScene_SgameEnd:		//二人用のエンディング
		SgameEnd_Draw();		//描画処理
		break;

	default:
		break;
	}
}

//nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene NextScene)
{
	Scene = NextScene;
}
#pragma once

typedef enum
{
	eScene_Menu,			//メニュー画面
	eScene_MenuSub,			//裏メニュー画面
	eScene_UraGame,			//裏ゲーム
	
	//一人に関すること
	eScene_Game,			//一人プレイ画面
	eScene_Opening,			//一人用のオープニング
	eScene_GameFirst,		//一人用の最初
	eScene_GameOver,		//一人用の雑魚戦でのゲームオーバー
	eScene_GameSecond,		//一人用のラスボス前
	eScene_GameLast,		//一人用のゲーム最後
	eScene_GameOverLast,	//一人用のラスボスでのゲームオーバー
	eScene_GameEnd,			//一人用のエンド

	//二人プレイに関すること
	eScene_SGame,			//二人プレイ画面(second)
	eScene_SOpening,		//二人プレイのオープニング画面
	eScene_SgameFirst,		//二人プレイ用ゲームの最初
	eScene_SGameOver,		//二人用の雑魚戦闘中のゲームオーバー
	eScene_SgameSecond,		//二人用のボス前
	eScene_SgameLast,		//二人用のラスボス
	eScene_SGameOverLast,	//二人用のラスボスのゲームオーバー
	eScene_SgameEnd,		//二人用の最後
} eScene;

//画像の読み込み
void SceneMgr_Read();

//シーンの更新
void SceneMgr_Update();

//シーンの描画
void SceneMgr_Draw();

//nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene nextScene);
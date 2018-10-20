#include "Dxlib.h"
#include "Scenemgr.h"
#include "Sound.h"
#include "Media.h"

#define MOJI_SIZE 23

//内容
int DrawPX, DrawPY;		//列に関すること
int sp, cp;				//文字に関すること
char  OMoji;			//文の挿入先
bool OEndFlag;			//終了フラグ
char OOneMojiBuf[3];	//１文字分一時記憶配列
bool OForceEnd;			//強制終了

//コントローラー
int Opad;


char string[][256] =
{
	//Ｆは入力待ち、＠は改行、Ｃは消して最初から、Ｅは終わり、Ｄは＠に追加として絵を入れる
	"　とある城で姫が暮らしている。F",
	"@また、とある所で魔王がある城を根城にしている。F",
	"@その城は毒が満ちていて、むやみに人が近づけないところになっている。F",
	"@　そんなある日、とても悲しいことに姫が魔王にさらわれてしまった。F",
	"C　と、いうわけで君には勇者として姫を救ってほしい。F",
	"D君は最強の剣を持っている。F",
	"@分かっているとは思うが姫も君も毒の城では長く持たない。F",
	"@長く持って５分でコロッと死んでしまうと考えてくれ。F",
	"@頼んだぞ。F",
	"C操作と戦闘説明F",
	"@　まず配置としては左に勇者、右に敵がいる。F",
	"@　君は攻撃のみで頑張らなければならない。F",
	"@@　左スティックで縦に移動してＢで攻撃だ。F",
	"@　そして攻撃が来そうになったらとにかく逃げろ。F",
	"@@大変だが頑張ってくれ。"
	"@始まります。FE"
};

//改行する処理
void Opening_Update()
{
	//描画行位置を一つ下げる
	DrawPY++;

	//描画列を最初に戻す
	DrawPX = 0;
}

//描画
void Opening_Draw()
{
	Opad = GetJoypadInputState(DX_INPUT_PAD1);
	DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
	PlaySoundMem(S_BGM.SetumeiS, DX_PLAYTYPE_LOOP, FALSE);
	//描画位置の初期位置セット
	DrawPX = 0;
	DrawPY = 0;

	//参照文字位置をセット
	sp = 0;	//１行目の
	cp = 0;	//０文字

	//終了フラグを倒す
	OEndFlag = 0;
	//強制終了フラグを倒す
	OForceEnd = 0;

	//キーボードのスペースかコントローラーのスタートが押されたらスキップする
	while (OEndFlag == 0 && CheckHitKey(KEY_INPUT_SPACE) == 0 && (Opad & PAD_INPUT_8) == 0)
	{
		Opad = GetJoypadInputState(DX_INPUT_PAD1);
		//Escキーが押されていたら
		if (((Opad & PAD_INPUT_7) && (Opad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			OForceEnd = 1;
			break;
		}

		//文字の描画
		OMoji = string[sp][cp];
		switch (OMoji)
		{
		case '@':	//改行文字

			//改行処理および参照文字位置を一つ進める
			Opening_Update();
			cp++;

			break;

		case 'F':	//ボタン押し待ち文字

			//コントローラーBかキーボードKを入力すると進む
			if ((Opad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_K) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				cp++;
			}
			break;

		case 'E':	//終了文字

			//終了フラグを立てるおよび参照文字位置を一つ進める
			OEndFlag = 1;
			cp++;

			break;

		case 'C':	//クリア文字

			//画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
			DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
			DrawPY = 0;
			DrawPX = 0;
			cp++;

			break;

		case 'D':  //'Q'の動作に追加で勇者の絵を入れる

			//改行処理および参照文字位置を一つ進める
			Opening_Update();
			DrawGraph(300, 300, M_Character.PlayerSwordM[2], TRUE);
			cp++;

			break;

		default:	//その他の文字

			//１文字分抜き出す
			OOneMojiBuf[0] = string[sp][cp];
			OOneMojiBuf[1] = string[sp][cp + 1];
			OOneMojiBuf[2] = '\0';

			//１文字描画
			DrawString(DrawPX * MOJI_SIZE, DrawPY * MOJI_SIZE, OOneMojiBuf, GetColor(0, 0, 0));	//文字色をここで

			//参照文字位置を２バイト勧める
			cp += 2;

			//カーソルを一文字文進める
			DrawPX++;

			//少し待つ
			WaitTimer(30);

			//画面からはみ出たら改行する
			if (DrawPX * MOJI_SIZE + MOJI_SIZE > 640)
			{
				Opening_Update();
			}

			break;
		}
		if (string[sp][cp] == '\0')
		{
			sp++;
			cp = 0;
		}
		//強制終了
		if (OForceEnd == 1)
		{
			DxLib_End(); // DXライブラリを終了する
		}
		if (OEndFlag == 1)
		{
			break;
		}
	}
	StopSoundMem(S_BGM.SetumeiS);
	SceneMgr_ChangeScene(eScene_GameFirst);   //シーンを一人用の戦闘前に移る
}
#include "Dxlib.h"
#include "Scenemgr.h"
#include "Sound.h"
#include "Media.h"

#define MOJI_SIZE 23	//文字の大きさ

//内容
int DrawPointX, DrawPointY;		//文字の位置
int SP, CP;						//文字の表示制限
char  SOMoji;					//文の挿入先
bool SOForceEnd;				//強制終了
bool SOEndFlag;					//終了フラグ
char SOOneMojiBuf[3];			//１文字分一時記憶配列

//コントローラー
int SOpad;		//コントローラー


char String[][256] =
{
	//Ｆは入力待ち、＠は改行、Ｃは消して最初から、Ｅは終わり、Ｄは＠に追加として絵を入れる
	"　とある城でとてもきれい好きな姫が暮らしている。F",
	"@また、とある所で魔王がある城を根城にしている。F",
	"@その城は臭くてたまらないと人々の間で知れ渡っていた。F",
	"@@　そんなある日、姫が魔王にさらわれてしまった。F",
	"@とても臭い城にF",
	"C　と、いうわけで君たちは勇者として姫を救ってほしい。F",
	"D一人は最強の剣を持ち、一人は最強の盾を持っている。F",
	"@分かっていると思うが君たちも姫も臭い城では長く持たないF",
	"@長く持っても５分半でコロッと死んでしまうと考えてくれ。F",
	"@頼んだぞ。F",
	"C操作と戦闘説明F",
	"@　まずは配置として左から剣の勇者、盾の勇者、敵となっている。F",
	"@　剣を持った勇者は左スティックで縦の移動ができ、Ｂで攻撃する。F",
	"@　盾を持った勇者は左スティックの縦移動のみだ。F",
	"@　たまに助言が出ます。F"
	"@@ということで頑張ってください。"
	"@始まります。FE"
};

//改行する処理
void SOpening_Update()
{
	// 描画行位置を一つ下げる
	DrawPointY++;

	// 描画列を最初に戻す
	DrawPointX = 0;
}

//描画
void SOpening_Draw()
{
	SOpad = GetJoypadInputState(DX_INPUT_PAD1);
	DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
	PlaySoundMem(S_BGM.SetumeiS, DX_PLAYTYPE_LOOP, FALSE);
	//描画位置の初期位置セット
	DrawPointX = 0;
	DrawPointY = 0;

	//参照文字位置をセット
	SP = 0;	// １行目の
	CP = 0;	// ０文字

	//終了フラグを倒す
	SOEndFlag = 0;
	SOForceEnd = 0;

	//キーボードのスペースかコントローラーのスタートが押されたらスキップする
	while (SOEndFlag == 0 && (SOpad & PAD_INPUT_8) == 0 && CheckHitKey(KEY_INPUT_SPACE) == 0)
	{
		SOpad = GetJoypadInputState(DX_INPUT_PAD1);
		//Escキーが押されていたら
		if (((SOpad & PAD_INPUT_7) && (SOpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			SOForceEnd = 1;
			break;
		}

		//文字の描画
		SOMoji = String[SP][CP];
		switch (SOMoji)
		{
		case '@':	//改行文字

			//改行処理および参照文字位置を一つ進める
			SOpening_Update();
			CP++;

			break;

		case 'F':	//ボタン押し待ち文字

			//Kを入力すると進む
			if ((SOpad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_K) != 0)
			{
				PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
				CP++;
			}
			break;

		case 'E':	//終了文字

			//終了フラグを立てるおよび参照文字位置を一つ進める
			SOEndFlag = 1;
			CP++;

			break;

		case 'C':	//クリア文字

			//画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
			DrawGraph(0, 0, M_BackGround.IntroM, FALSE);
			DrawPointY = 0;
			DrawPointX = 0;
			CP++;

			break;

		case 'D':  //'Q'の動作に追加で勇者の絵を入れる

			//改行処理および参照文字位置を一つ進める
			SOpening_Update();
			CP++;
			
			//勇者のグラフィック
			DrawGraph(210, 300, M_Character.PlayerSwordM[2], TRUE);
			DrawGraph(400, 300, M_Character.PlayerShieldM, TRUE);

			break;

		default:	//その他の文字

			//１文字分抜き出す
			SOOneMojiBuf[0] = String[SP][CP];
			SOOneMojiBuf[1] = String[SP][CP + 1];
			SOOneMojiBuf[2] = '\0';

			//１文字描画
			DrawString(DrawPointX * MOJI_SIZE, DrawPointY * MOJI_SIZE, SOOneMojiBuf, GetColor(0, 0, 0));	//文字色をここで

			//参照文字位置を２バイト勧める
			CP += 2;

			//カーソルを一文字文進める
			DrawPointX++;

			//少し待つ
			WaitTimer(30);

			//画面からはみ出たら改行する
			if (DrawPointX * MOJI_SIZE + MOJI_SIZE > 640)
			{
				SOpening_Update();
			}

			break;
		}

		if (String[SP][CP] == '\0')
		{
			SP++;
			CP = 0;
		}
		if (SOForceEnd == 1)
		{
			DxLib_End(); //DXライブラリを終了する
		}
		if (SOEndFlag == 1)
		{
			break;
		}
	}
	StopSoundMem(S_BGM.SetumeiS);
	SceneMgr_ChangeScene(eScene_SgameFirst);   //シーンを二人用の戦闘前にに移る
}
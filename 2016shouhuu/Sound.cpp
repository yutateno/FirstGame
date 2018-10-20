#include "DxLib.h"
#include "Sound.h"

Sound_BGM S_BGM;

Sound_SE S_SE;

//ì«Ç›çûÇ›
void Sound_Read()
{
	//BGM
	S_BGM.SetumeiS = LoadSoundMem("sound\\setumei.wav");
	S_BGM.GameOverS = LoadSoundMem("sound\\gameover.wav");
	S_BGM.MenuS = LoadSoundMem("sound\\menu.wav");
	S_BGM.ZakoS = LoadSoundMem("sound\\zako.wav");
	S_BGM.BossS = LoadSoundMem("sound\\boss.wav");
	S_BGM.MenuSubS = LoadSoundMem("sound\\menusub.wav");
	S_BGM.UraBGMM = LoadSoundMem("sound\\bosssub.wav");
	S_BGM.AsiotoS = LoadSoundMem("sound\\asioto.wav");

	//SE
	S_SE.BotanS = LoadSoundMem("sound\\botan.wav");
	S_SE.HitS = LoadSoundMem("sound\\ponn.wav");
	S_SE.AttackS = LoadSoundMem("sound\\attack.wav");
	S_SE.ShieldS = LoadSoundMem("sound\\shield.wav");
	S_SE.NextenemyS = LoadSoundMem("sound\\nextenemy.wav");
	S_SE.ReturnHitS = LoadSoundMem("sound\\enemyattack.wav");
	S_SE.BonS = LoadSoundMem("sound\\bon.wav");
}
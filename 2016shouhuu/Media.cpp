#include "DxLib.h"
#include "Media.h"

Media_BackGround M_BackGround;

Media_Character M_Character;

Media_Enemy M_Enemy;

//ì«Ç›çûÇ›
void Media_Read()
{
	//îwåi
	M_BackGround.BackGroundM = LoadGraph("media\\Background.png");
	M_BackGround.LastBackGroundM = LoadGraph("media\\LastBackground.png");
	M_BackGround.IntroM = LoadGraph("media\\intro.png");
	M_BackGround.BlackM = LoadGraph("media\\black.png");
	M_BackGround.TitleM = LoadGraph("media\\title.png");
	M_BackGround.TitleSubM = LoadGraph("media\\title2.png");
	M_BackGround.UraBackGroundM = LoadGraph("media\\NoEndBackground.png");
	M_BackGround.SecondBackGroundM = LoadGraph("media\\SecondBackground.png");

	//ÉLÉÉÉâ
	LoadDivGraph("media\\playersword.png", 3, 3, 1, 50, 60, M_Character.PlayerSwordM);
	M_Character.PrincessM = LoadGraph("media\\princess.png");
	M_Character.PlayerShieldM = LoadGraph("media\\playershield.png");
	M_Character.PlayerShieldMahouM = LoadGraph("media\\playershieldmahou.png");
	M_Character.PlayerShieldCoolDownM = LoadGraph("media\\playershieldcooldown.png");
	M_Character.SwordM = LoadGraph("media\\sword.png");
	M_Character.PAttackBarM = LoadGraph("media\\attackbar.png");
	LoadDivGraph("media\\playeryellowsword.png", 3, 3, 1, 50, 60, M_Character.PlayerSwordYellowM);
	
	//ìG
	M_Enemy.LastEnemyM = LoadGraph("media\\LastEnemy.png");
	LoadDivGraph("media\\enemy2.png", 4, 4, 1, 60, 90, M_Enemy.EnemyoM);
	M_Enemy.UraEnemyM = LoadGraph("media\\PrincessEnemy.png");
	M_Enemy.UraSentakuM = LoadGraph("media\\sentakunum.png");
	M_Enemy.AttackArrowM = LoadGraph("media\\Attack.png");
	M_Enemy.HPM = LoadGraph("media\\heartbar.png");
	M_Enemy.HPsecondM = LoadGraph("media\\heartbar2.png");
	M_Enemy.DAMAGEM = LoadGraph("media\\damage.png");
	M_Enemy.TrapM = LoadGraph("media\\toge.png");
	M_Enemy.TenmetsuM = LoadGraph("media\\tenmetsu.png");
	M_Enemy.AttackYellowArrowM = LoadGraph("media\\AttackYellow.png");
	M_Enemy.AttackRedArrowM = LoadGraph("media\\AttackRed.png");
	M_Enemy.HelpM = LoadGraph("media\\underbar.png");
	M_Enemy.ShieldM = LoadGraph("media\\mahoushield.png");
	M_Enemy.YellowAttackM = LoadGraph("media\\YellowAttack.png");
	M_Enemy.RedAttackM = LoadGraph("media\\RedAttack.png");
}
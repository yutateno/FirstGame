#pragma once

//”wŒi
struct Media_BackGround {
	int BackGroundM;		//G‹›í”wŒi
	int LastBackGroundM;	//ƒ‰ƒXƒ{ƒXí”wŒi
	int IntroM;				//à–¾‰æ–Ê”wŒi
	int BlackM;				//ƒGƒ“ƒfƒBƒ“ƒO‚Ì”wŒi
	int TitleM;				//ƒƒjƒ…[‚Ì”wŒi
	int TitleSubM;			//— ƒƒjƒ…[‚Ì”wŒi
	int UraBackGroundM;		//— ‚Ì”wŒi
	int SecondBackGroundM;	//G‹›íŒã”¼‚Ì”wŒi
};

extern Media_BackGround M_BackGround;

//ƒLƒƒƒ‰
struct Media_Character {
	int PlayerSwordM[3];		//Œ•‚Ì—EÒ‚ÌŠG
	int PrincessM;				//ƒvƒŠƒ“ƒZƒX‚ÌŠG
	int PlayerShieldM;			//‚‚Ì—EÒ‚ÌŠG
	int PlayerShieldMahouM;		//–‚–@”½Ë‚Ì‚‚ğ‚Á‚Ä‚é—EÒ‚ÌŠG
	int PlayerShieldCoolDownM;	//ƒN[ƒ‹ƒ_ƒEƒ“’†‚Ì‚‚Ì—EÒ‚ÌŠG
	int SwordM;					//Œ•‚ÌŠG
	int PAttackBarM;			//ˆêl—p‚Ì—EÒ‚ÌƒQ[ƒW
	int PlayerSwordYellowM[3];	//Œ•‚Ì—EÒ‚Ì‘‚¢‚Æ‚«
};

extern Media_Character M_Character;

//“G‰æ‘œ
struct Media_Enemy {
	//“G
	int LastEnemyM;				//ƒ‰ƒXƒ{ƒX‚ÌŠG
	int EnemyoM[4];				//ˆê‘Ì–Ú‚Ì“G
	int UraEnemyM;				//— ‚Ì“G
	//“GŠÖ˜A
	int UraSentakuM;			//— ‚Ì‘I‘ğƒ{ƒ^ƒ“
	int AttackArrowM;			//–îˆó‚ÌŠG
	int HPM;					//‘Ì—Í‚Ì‰æ‘œ
	int DAMAGEM;				//‘Ì—Í‚Ìƒ_ƒ[ƒW‰æ‘œ
	int TrapM;					//ã©‚Ì‰æ‘œ
	int TenmetsuM;				//“_–Å‚Æ‚·‚é‰æ‘œ
	int HPsecondM;				//ƒ‰ƒXƒ{ƒX‚Å‚Ì“ñ‚Â–Ú‚Ì‘Ì—Í‚Ì•`‰æ
	int HelpM;					//•Œ¾
	int AttackYellowArrowM;		//“G‚Ì‰Î‹…‚Å‚ÌUŒ‚‡}
	int YellowAttackM;			//‰Î‹…
	int AttackRedArrowM;		//“G‚Ì‰Î‹…‰ü‚Å‚ÌUŒ‚‡}
	int RedAttackM;				//‰Î‹…‰ü
	int ShieldM;				//–‚–@ƒV[ƒ‹ƒh
	int ShieldMoveM;			//‚ˆÚ“®
};

extern Media_Enemy M_Enemy;

//“Ç‚İ‚İ
void Media_Read();
#pragma once

//�w�i
struct Media_BackGround {
	int BackGroundM;		//�G����w�i
	int LastBackGroundM;	//���X�{�X��w�i
	int IntroM;				//������ʔw�i
	int BlackM;				//�G���f�B���O�̔w�i
	int TitleM;				//���j���[�̔w�i
	int TitleSubM;			//�����j���[�̔w�i
	int UraBackGroundM;		//���̔w�i
	int SecondBackGroundM;	//�G����㔼�̔w�i
};

extern Media_BackGround M_BackGround;

//�L����
struct Media_Character {
	int PlayerSwordM[3];		//���̗E�҂̊G
	int PrincessM;				//�v�����Z�X�̊G
	int PlayerShieldM;			//���̗E�҂̊G
	int PlayerShieldMahouM;		//���@���˂̏��������Ă�E�҂̊G
	int PlayerShieldCoolDownM;	//�N�[���_�E�����̏��̗E�҂̊G
	int SwordM;					//���̊G
	int PAttackBarM;			//��l�p�̗E�҂̃Q�[�W
	int PlayerSwordYellowM[3];	//���̗E�҂̑����Ƃ�
};

extern Media_Character M_Character;

//�G�摜
struct Media_Enemy {
	//�G
	int LastEnemyM;				//���X�{�X�̊G
	int EnemyoM[4];				//��̖ڂ̓G
	int UraEnemyM;				//���̓G
	//�G�֘A
	int UraSentakuM;			//���̑I���{�^��
	int AttackArrowM;			//���̊G
	int HPM;					//�̗͂̉摜
	int DAMAGEM;				//�̗͂̃_���[�W�摜
	int TrapM;					//㩂̉摜
	int TenmetsuM;				//�_�łƂ���摜
	int HPsecondM;				//���X�{�X�ł̓�ڂ̗̑͂̕`��
	int HelpM;					//����
	int AttackYellowArrowM;		//�G�̉΋��ł̍U�����}
	int YellowAttackM;			//�΋�
	int AttackRedArrowM;		//�G�̉΋����ł̍U�����}
	int RedAttackM;				//�΋���
	int ShieldM;				//���@�V�[���h
	int ShieldMoveM;			//���ړ�
};

extern Media_Enemy M_Enemy;

//�ǂݍ���
void Media_Read();
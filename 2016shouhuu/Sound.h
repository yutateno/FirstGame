#pragma once

//BGM
struct Sound_BGM {
	int SetumeiS;	//������ʂł�BGM
	int GameOverS;	//�Q�[���I�[�o�[��BGM
	int MenuS;		//���j���[��ʌ��G���f�B���O��BGM
	int ZakoS;		//�G�����BGM
	int BossS;		//BOSS���BGM
	int MenuSubS;	//�����j���[��BGM
	int UraBGMM;	//���Q�[����BGM
	int AsiotoS;	//�{�X�O��BGM
};

extern Sound_BGM S_BGM;

//SE
struct Sound_SE {
	int BotanS;		//����{�^����SE
	int AttackS;	//���̍U����SE
	int HitS;		//�G�̍U����SE
	int ShieldS;	//���̖h���SE
	int NextenemyS;	//�G�����񂾂Ƃ���SE
	int BonS;		//�΋���SE
	int ReturnHitS;	//���@���˂œG���_���[�W�󂯂��Ƃ���SE
};

extern Sound_SE S_SE;

//�ǂݍ���
void Sound_Read();
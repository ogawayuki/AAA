#include "Main.h"
#include "CanMove.h"
#include "ObsCollision.h"
#include "ItemCollision.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{			

	ChangeWindowMode(FALSE);				//�E�B���h�E���[�h�ɂ���

	SetGraphMode(WINDOW_X, WINDOW_Y, 32);	//�E�B���h�E�T�C�Y��ݒ肷��

	if (DxLib_Init() == -1)			// �c�w���C�u��������������
	{
		return -1;					// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);	//����ʂ�`��Ώۂ�

	//�����������̕ӂɏ�����������������

	//���@�̍\����
	struct Player
	{	
		int iX;					//���@��X���W
		int iY;					//���@��Y���W
		int iSize = 60;			//���@�̑傫��
		int iSize2 = 30;		//���@�̑傫���̔���
		int iJump;				//���@�̃W�����v��
		int iJumpFlag;			//���@�̃W�����v�t���O
		int iJumpLevel;			//���@�̃W�����v���x��
		int iJumpLevel2;		//���@�̃W�����v���x��2
		int iLandingFlag;		//���@�̒��n�t���O
		bool bJumpFlag = TRUE;
		int iobsUpFlag;         //���@�̏�Q���̏�ł̃W�����v�t���O
		int iGraph;				//���@�̉摜
		int iAnicnt;			//���@�̃A�j���[�V�����J�E���g
		int LoseFlag;           //���@�̕����t���O1�͕���
		bool iLive = TRUE;	//���@�̐�������
	};
	Player player;
	
	int iPlayerGraph[4]; 
	LoadDivGraph("media\\player_01.png", 4, 2, 2, 60, 60, iPlayerGraph);	//�摜�̓ǂݍ���

	//�G�̍\����
	struct Enemy
	{
		int iX;					//�G��X���W
		int iY;					//�G��Y���W
		int iSize = 60;			//�G�̑傫��
		int iSize2 = 30;		//�G�̑傫���̔���
		int iJump;				//�G�̃W�����v��
		int iJumpFlag;			//�G�̃W�����v�t���O
		int iJumpFlag2;			//�G�̃W�����v�t���O2
		int iJumpLevel;			//�G�̃W�����v���x��
		int iJumpLevel2;		//�G�̃W�����v���x��2
		int iLandingFlag;		//�G�̒��n�t���O
		int iobsUpFlag;         //�G�̏�Q���̏�ł̃W�����v�t���O
		int playerpointY;
		int iGraph;				//�G�̉摜
		int iAnicnt;			//�G�̃A�j���[�V�����J�E���g
		int LoseFlag;			//�G�̕����t���O
		bool iLive = TRUE;   //�G�̐�������
	};
	Enemy enemy;

	int iEnemyGraph[4];
	LoadDivGraph("media\\enemyDivAnime1.png", 4, 2, 2, 60, 60, iEnemyGraph);	//�摜�̓ǂݍ���
	
	//�A�C�e���̍\����
	struct Item
	{
		int iX;					//�A�C�e����X���W
		int iY;					//�A�C�e����Y���W
		int iSize = 70;			//�A�C�e���̑傫��
		int iGraph;				//�A�C�e���̉摜
		bool iLive;				//�A�C�e���̐�������
	};
	
	Item item[ITEM];	//�A�C�e���̐���100��

	int iItemGraph = LoadGraph("media\\item.png");	//�摜�̓ǂݍ���
	for (int i = 0; i < ITEM; i++){
		item[i].iGraph = iItemGraph;				//�摜�̊��蓖��
	}

	//��Q��1�̍\����
	struct Obs1
	{
		int iX;				//��Q��1��X���W
		int iY;				//��Q��1��Y���W
		int iGraph;			//��Q��1�̉摜
		int iSize_X = 70;	//��Q��1�̉����̉摜�T�C�Y
		int iSize_Y = 70;	//��Q��1�̏c���̉摜�T�C�Y
		bool iLive;		//��Q��1�̐�������
	};

	Obs1 obs1[OBS1];//��Q��1�̐���100��
	int obs1Graph = LoadGraph("media\\obs1_01_01.jpg");	//�摜�̓ǂݍ���
	for (int i = 0; i < OBS1; i++){
		obs1[i].iGraph = obs1Graph;					//�摜�̊��蓖��
	}

	//��Q��2�̍\����
	struct Obs2
	{
		int iX;				//��Q��2��X���W
		int iY;				//��Q��2��Y���W
		int iGraph;			//��Q��2�̉摜
		int iSize_X = 45;	//��Q��2�̉����̉摜�T�C�Y
		int iSize_Y = 150;	//��Q��2�̏c���̉摜�T�C�Y
		bool iLive;		//��Q��2�̐�������
	};

	Obs2 obs2[OBS2];		//��Q��2�̐���20��
	int obs2Graph = LoadGraph("media\\obs_02.png");	//�摜�̓ǂݍ���
	for (int i = 0; i < OBS2; i++){
		obs2[i].iGraph = obs2Graph;					//�摜�̊��蓖��
	}

	//�S�[���̍\����
	struct Goal
	{
		int iX;			//�S�[����X���W
		int iY;			//�S�[����Y���W
		int iGraph;		//�S�[���̉摜
		int iSize = 100;//�S�[���̉摜�T�C�Y
	};

	Goal goal;
	int GoalGraph = LoadGraph("media\\Goal_01.png");	//�S�[���̉摜�̓ǂݍ���
	goal.iGraph = GoalGraph;							//�S�[���̉摜�̊��蓖��

	//�G�t�F�N�g�̍\����
	struct Effect
	{
		int iX;
		int iY;
		int iGraph;
		int iAniCnt;
	};
	Effect effect;
	
	effect.iAniCnt = 0;

	int EffectGraph[4];
	LoadDivGraph("media\\Efect_Ani.png", 4, 2, 2, 70, 70, EffectGraph);

	//�G�t�F�N�g2�̍\����
	struct Effect2
	{
		int iX;
		int iY;
		int iGraph;
		int iAniCnt;
	};
	Effect2 effect2;

	effect2.iAniCnt = 0;

	int EffectGraph2[4];
	LoadDivGraph("media\\Efect_Ani2.png", 4, 2, 2, 70, 70, EffectGraph2);

	//��ʑJ�ڂ̗񋓌^
	enum eGameState
	{
		Title,			//�^�C�g�����
		Title_Help,		//�^�C�g����ʂŃw���v�{�^���I��
		Main,			//���C�����
		GameOver,		//�Q�[���I�[�o�[
		Lose,			//�������
		GameClear,		//�Q�[���N���A
		Help,           //�w���v���
		Help2,			//�w���v���2
		Help3,			//�w���v���3
		Help4,			//�w���v���4
		End				//�Q�[���I��
	};

	int iGameState = Title;

	//���������������ςȂ����聥������

	bool bPreesingSPASEkey = FALSE;		//�X�y�[�X�L�[�̉������ςȂ�����
	bool bPreesingZkey = FALSE;			//Z�L�[�̉������ςȂ�����
	bool bPreesingUPkey = FALSE;			//UP�L�[�̉������ςȂ�����
	bool bPreesingDOWNkey = FALSE;		//DOWN�L�[�̉������ςȂ�����
	bool bPreesingRIGHTkey = FALSE;		//RIGHT�L�[�̉������ςȂ�����
	bool bPreesingLEFTkey = FALSE;		//LEFT�L�[�̉������ςȂ�����

	//���������������ςȂ����聣������

	//�w�i�֌W
	int iBackGraph;
	iBackGraph = LoadGraph("media\\BackGraph_02.png");//�w�i�摜�̓ǂݍ���

		//�w�i�̍\����
	struct Back
	{
		int iX;				//�w�i��X���W
		int iGraph;			//�w�i�̉摜
	};
	Back bkGround[BACK];
	bkGround[0].iX = 0;
	bkGround[1].iX = 1280;

	for (int i = 0; i < BACK; i++){
		bkGround[i].iGraph = iBackGraph;	//�w�i�摜�̊��蓖��
	}

	//���d�w�i
	struct MultiBack
	{
		int iX;				//���d�w�i��X���W
		int iY;
		int iSize_X = 1280;	//���d�w�i�̉����̉摜�T�C�Y
		int iSize_Y = 153;	//���d�w�i�̏c���̉摜�T�C�Y
		int iGraph;			//���d�w�i�̉摜
	};
	MultiBack multiBack[MULTIBACK];
	int iMultiBackGraph = LoadGraph("media\\yuka_011.png");
	multiBack[0].iX = 0;
	multiBack[1].iX = WINDOW_X;

	for (int i = 0; i < MULTIBACK; i++){
		multiBack[i].iGraph = iMultiBackGraph;
		multiBack[i].iY = 720 - multiBack[i].iSize_Y;
	}

	//�摜�ǂݍ���
	int StartGraph_01 = LoadGraph("media\\START_01.jpg");//�^�C�g����ʂ̓ǂݍ���
	int StartGraph_02 = LoadGraph("media\\START_02.jpg");//�^�C�g�����2�̓ǂݍ���
	int StartGraph_03 = LoadGraph("media\\START_03.jpg");//�^�C�g�����3�̓ǂݍ���
	int HelpGraph_01 = LoadGraph("media\\Help_01.jpg");//�w���v���01�̓ǂݍ���
	int HelpGraph_02 = LoadGraph("media\\Help_02.jpg");//�w���v���02�̓ǂݍ���
	int HelpGraph_03 = LoadGraph("media\\Help_03.jpg");//�w���v���03�̓ǂݍ���
	int HelpGraph_04 = LoadGraph("media\\Help_04.jpg");//�w���v���04�̓ǂݍ���
	int GameClearGraph = LoadGraph("media\\GAMECLEAR1.jpg");//�N���A��ʂ̓ǂݍ���
	int GameOverGraph = LoadGraph("media\\GAMEOVER1.jpg");//�Q�[���I�[�o�[��ʂ̓ǂݍ���
	int LoseGraph = LoadGraph("media\\Lose1.jpg");//������ʂ̓ǂݍ���
	
	//���y�֌W
	struct Sounds
	{
		//[1]��1�Ȃ�Đ��J�n
		int BGMDate[2];
		int Jump[2];	//�W�����v��
		int Damage[2];	//�_���[�W��
		int Main[2];	//���C�����y
		int Title[2]; //�^�C�g�����y
		int GameClear[2];//�Q�[���N���A
		int GameOver[2];//�Q�[���I�[�o�[
		int  Powerup[2]; //�A�C�e���擾���̉�
	};
	Sounds sounds;

	sounds.Jump[0] = LoadSoundMem("media\\sound\\Jump.mp3");			//�W�����v���̓ǂݍ���
	sounds.Damage[0] = LoadSoundMem("media\\sound\\Damage.mp3");		//�_���[�W���̓ǂݍ���
	sounds.Title[0] = LoadSoundMem("media\\sound\\title.mp3");			//�^�C�g�����̓ǂݍ���
	sounds.GameClear[0] = LoadSoundMem("media\\sound\\clear.mp3");		//�Q�[���N���A���̓ǂݍ���
	sounds.GameOver[0] = LoadSoundMem("media\\sound\\gemaover.mp3");	//�Q�[���I�[�o�[���̓ǂݍ���
	sounds.Main[0] = LoadSoundMem("media\\sound\\main.mp3");			//���C�����y�̓ǂݍ���
	sounds.Powerup[0] = LoadSoundMem("media\\sound\\PowerUP.mp3");		//�A�C�e���擾���̉��̓ǂݍ���

	sounds.Damage[1] = 0;
	sounds.Jump[1] = 0;
	sounds.Powerup[1] = 0;
	sounds.Title[1] = 1;
	sounds.GameClear[1] = 1;
	sounds.GameOver[1] = 1;
	sounds.Main[1] = 1;

	//�������̕ӂɏ�����������������

	//���C������
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		if (CheckHitKey(KEY_INPUT_SPACE) == 0)
			bPreesingSPASEkey = FALSE;

		if (CheckHitKey(KEY_INPUT_Z) == 0)
			bPreesingZkey = FALSE;

		if (CheckHitKey(KEY_INPUT_UP) == 0)
			bPreesingUPkey = FALSE;

		if (CheckHitKey(KEY_INPUT_DOWN) == 0)
			bPreesingDOWNkey = FALSE;

		if (CheckHitKey(KEY_INPUT_RIGHT) == 0)
			bPreesingRIGHTkey = FALSE;

		if (CheckHitKey(KEY_INPUT_LEFT) == 0)
			bPreesingLEFTkey = FALSE;

		switch (iGameState)
		{

		case End:		//�G���h���
			DrawGraph(0, 0, StartGraph_03, TRUE);	//������ʂ̕`��

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){	//DOWN�L�[����������
				iGameState = Title;			//�^�C�g����ʂɂ���
				bPreesingDOWNkey = TRUE;	//DOWN�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){	//UP�L�[����������
				iGameState = Title_Help;		//�^�C�g���w���v�I����ʂɂ���
				bPreesingUPkey = TRUE;	//UP�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������				
				bPreesingZkey = TRUE;	//Z�L�[�̉������ςȂ������TRUE�ɂ���
				return 1;
			}

			break;

		case Lose://�������
			
			sounds.Title[1] = 1;
			StopSoundMem(sounds.Title[0]);
			StopSoundMem(sounds.Main[0]);
			StopSoundMem(sounds.GameClear[0]);
			
			if (sounds.GameOver[1] == 1){
				PlaySoundMem(sounds.GameOver[0], DX_PLAYTYPE_LOOP);
				sounds.GameOver[1] = 0;
			}

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Title;				//�^�C�g����ʂɈړ�����
				bPreesingZkey = TRUE;			//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}

			DrawGraph(0, 0, LoseGraph, TRUE);	//������ʂ̕`��

			break;//������ʂ𔲂���

		case Help:		//�w���v���

			//�����w���v��ʂł̍X�V������������

			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//�E�L�[����������
				iGameState = Help2;				//�w���v���2�Ɉړ�����
				bPreesingRIGHTkey = TRUE;
			}

			//�����w���v��ʂł̍X�V������������

			//�����w���v��ʂł̕`�揈����������

			DrawGraph(0, 0, HelpGraph_01, TRUE);	//�w���v��ʂ̕`��

			//�����w���v��ʂł̕`�揈����������

			break;      //�w���v��ʂ𔲂���

		case Title:		//�^�C�g�����

			//�����^�C�g����ʂł̏�����������������

			 //���@
			player.iX = 640;					//���@��X���W�̏���������
			player.iY = 566 - player.iSize;		//���@��Y���W�̏���������
			player.iJump = 0;					//���@�̃W�����v�͂̏���������
			player.iJumpFlag = 0;				//���@�̃W�����v�t���O�̏�����
			player.iJumpLevel = 1;				//���@�̃W�����v���x���̏�����
			player.iJumpLevel2 = 0;				//���@�̃W�����v���x��2�̏�����
			player.iLandingFlag = 0;			//���@�̒��n�t���O�̏�����
			player.iobsUpFlag = 0;				//���@�̏�Q���̏㕔������t���O�̏�����
			player.iAnicnt = 0;					//���@�̃A�j���[�V�����J�E���g�̏�����
			player.LoseFlag = 0;				//���@�̕����t���O�̏�����
			
			 //�G
			enemy.iX = 500;						//�G��X���W�̏���������
			enemy.iY = 566 - player.iSize;		//�G��X���W�̏���������
			enemy.iJump = 0;					//�G�̃W�����v�͂̏���������
			enemy.iJumpFlag = 0;				//�G�̃W�����v�t���O�̏�����
			enemy.iJumpLevel = 1;				//�G�̃W�����v���x���̏�����
			enemy.iJumpLevel2 = 0;				//�G�̃W�����v���x��2�̏�����
			enemy.iLandingFlag = 0;				//�G�̒��n�t���O�̏�����
			enemy.iobsUpFlag = 0;				//�G�̏�Q���̏㕔������t���O�̏�����
			enemy.iAnicnt = 1;					//�G�̃A�j���[�V�����J�E���g�̏�����
			enemy.LoseFlag = 0;					//�G�̕����t���O�̏�����

			//�G�t�F�N�g�̏�����
			effect.iAniCnt = 0;
			effect2.iAniCnt = 0;

			//�A�C�e��
			for (int i = 0; i < ITEM; i++)
				item[i].iLive = TRUE;           //�A�C�e���̐�����������ׂ�TRUE�ɂ���
			
			//�A�C�e���̍��W�w��
			item[0].iX = 2000;
			item[0].iY = 300;

			item[1].iX = 3200;
			item[1].iY = 3000;

			item[2].iX = 3800;
			item[2].iY = 100;

			item[3].iX = 5100;
			item[3].iY = 200;

			item[4].iX = 8000;
			item[4].iY = 300;

			//��Q��
			for (int i = 0; i < OBS1; i++)
				obs1[i].iLive = TRUE;		//��Q��1�̐��������TRUE�ɂ���
			
			for (int i = 0; i < OBS2; i++)
				obs2[i].iLive = TRUE;		//��Q��2�̐��������TRUE�ɂ���
			
			//��Q��1�̍��W�w��

			obs1[0].iX = 1000;
			obs1[0].iY = multiBack[0].iY - obs1[0].iSize_Y;
			obs1[1].iX = 1000 + obs1[0].iSize_X;
			obs1[1].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[2].iX = 1000 + obs1[0].iSize_X;
			obs1[2].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[3].iX = 1000 + obs1[0].iSize_X * 2;
			obs1[3].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[4].iX = 1000 + obs1[0].iSize_X * 2;
			obs1[4].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[5].iX = 1000 + obs1[0].iSize_X * 2;
			obs1[5].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;
			obs1[6].iX = 1700;

			obs1[6].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[7].iX = 2500;

			obs1[7].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[8].iX = 2500;
			obs1[8].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[9].iX = 2500;
			obs1[9].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;
			obs1[10].iX = 2700;

			obs1[10].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[11].iX = 2700;

			obs1[11].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[12].iX = 2700;
			obs1[12].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[13].iX = 3200;
			obs1[13].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[14].iX = 3500;

			obs1[14].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[15].iX = 3500;
			obs1[15].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;

			obs1[16].iX = 3800;
			obs1[16].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[17].iX = 3800;
			obs1[17].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[18].iX = 3800;
			obs1[18].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[19].iX = 4500;
			obs1[19].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[20].iX = 4500;
			obs1[20].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[21].iX = 4500;
			obs1[21].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[22].iX = 4700;
			obs1[22].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[23].iX = 4700;
			obs1[23].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[24].iX = 4700;
			obs1[24].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[25].iX = 4900;
			obs1[25].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[26].iX = 4900;
			obs1[26].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[27].iX = 4900;
			obs1[27].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[28].iX = 5100;
			obs1[28].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[29].iX = 5100;
			obs1[29].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[30].iX = 5100;
			obs1[30].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[31].iX = 5550;
			obs1[31].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[32].iX = 5550;
			obs1[32].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[33].iX = 5550;
			obs1[33].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[35].iX = 6000;
			obs1[35].iY =  multiBack[0].iY - obs1[0].iSize_Y;
			obs1[36].iX = 6000;
			obs1[36].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[37].iX = 6000;
			obs1[37].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;
			obs1[38].iX = 6300;
			obs1[38].iY = multiBack[0].iY - obs1[0].iSize_Y;

			obs1[39].iX = 6700;
			obs1[39].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[40].iX = 7000;
			obs1[40].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[41].iX = 7300;
			obs1[41].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[42].iX = 7600;
			obs1[42].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[43].iX = 7600;
			obs1[43].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;

			obs1[44].iX = 7900;
			obs1[44].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[45].iX = 7900;
			obs1[45].iY =  multiBack[0].iY - obs1[0].iSize_Y * 2;

			obs1[46].iX = 7900;
			obs1[46].iY =  multiBack[0].iY - obs1[0].iSize_Y * 3;

			obs1[47].iX = 8100;
			obs1[47].iY =  multiBack[0].iY - obs1[0].iSize_Y;

			obs1[48].iX = 8500;
			obs1[48].iY = multiBack[0].iY - obs1[0].iSize_Y;
			obs1[49].iX = 8500 + obs1[0].iSize_X;
			obs1[49].iY = multiBack[0].iY - obs1[0].iSize_Y;
			obs1[50].iX = 8500 + obs1[0].iSize_X;
			obs1[50].iY = multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[51].iX = 8500 + obs1[0].iSize_X * 2;
			obs1[51].iY = multiBack[0].iY - obs1[0].iSize_Y;
			obs1[52].iX = 8500 + obs1[0].iSize_X * 2;
			obs1[52].iY = multiBack[0].iY - obs1[0].iSize_Y * 2;
			obs1[53].iX = 8500 + obs1[0].iSize_X * 2;
			obs1[53].iY = multiBack[0].iY - obs1[0].iSize_Y * 3;
			
			obs1[54].iX = 9000;
			obs1[54].iY = multiBack[0].iY - obs1[0].iSize_Y;
			obs1[55].iX = 9000;
			obs1[55].iY = multiBack[0].iY - obs1[0].iSize_Y * 2;

			obs1[56].iX = 9600;
			obs1[56].iY = multiBack[0].iY - obs1[0].iSize_Y;

			obs1[57].iX = 10000;
			obs1[57].iY = multiBack[0].iY - obs1[0].iSize_Y;

			obs1[58].iX = 10000;
			obs1[58].iY = multiBack[0].iY - obs1[0].iSize_Y * 2;

			//��Q���Q�̍��W�w��
			obs2[0].iX = 1450;
			obs2[1].iX = 2590;
			obs2[2].iX = 4590;
			obs2[3].iX = 4790;
			obs2[4].iX = 4990; 
			obs2[5].iX = 6390;
			obs2[6].iX = 7790;
			obs2[7].iX = 7990;
			obs2[8].iX = 8750;
			obs2[9].iX = 10500;

			for (int i = 0; i < OBS2; i++)
				obs2[i].iY =  multiBack[0].iY - obs2[i].iSize_Y;	//��Q��2��Y���W�̎w��

			//�S�[��
				//�S�[���̍��W�w��
			goal.iX = 11000;
			goal.iY =  multiBack[0].iY - goal.iSize;

			//�����^�C�g����ʂł̏�����������������

			//�����^�C�g����ʂł̍X�V������������
			
			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Main;				//���C����ʂɈړ�����
				bPreesingZkey = TRUE;			//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){//DOWN�L�[����������
				iGameState = Title_Help;		//�^�C�g��(�w���v�I��)�ɂ���
				bPreesingDOWNkey = TRUE;		//DOWN�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){//UP�L�[����������
				iGameState = End;		//�G���h��ʂɂ���
				bPreesingUPkey = TRUE;	//UP�L�[�̉������ςȂ������TRUE�ɂ���
			}

			//���y
			sounds.GameClear[1] = 1;
			sounds.GameOver[1] = 1;
			sounds.Main[1] = 1;

			StopSoundMem(sounds.GameOver[0]);
			StopSoundMem(sounds.GameClear[0]);
			StopSoundMem(sounds.Main[0]);

			//�^�C�g���̉��y�Đ��J�n

			if (sounds.Title[1] == 1){
				PlaySoundMem(sounds.Title[0], DX_PLAYTYPE_LOOP);
				sounds.Title[1] = 0;
			}

			//�����^�C�g����ʂł̍X�V������������

			//�����^�C�g����ʂł̕`�揈����������

			DrawGraph(0, 0, StartGraph_01, TRUE);		//�^�C�g����ʂ̕`��

			//�����^�C�g����ʂł̕`�揈����������

			break;	//�^�C�g����ʂ𔲂���

		case Title_Help:	//�^�C�g�����(�w���v�I��)

			//�����^�C�g�����(�w���v�I��)�ł̍X�V������������

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Help;		//�w���v��ʂɂ���
				bPreesingZkey = TRUE;	//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){	//UP�L�[����������
				iGameState = Title;		//�^�C�g����ʂɂ���
				bPreesingUPkey = TRUE;	//UP�L�[�̉������ςȂ������TRUE�ɂ���
			}

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){	//DOWN�L�[����������
				iGameState = End;			//�G���h��ʂɂ���
				bPreesingDOWNkey = TRUE;	//DOWN�L�[�̉������ςȂ������TRUE�ɂ���
			}

			//�����^�C�g�����(�w���v�I��)�ł̍X�V������������

			//�����^�C�g�����(�w���v�I��)�ł̕`�揈����������

			DrawGraph(0, 0, StartGraph_02, TRUE);//�^�C�g�����2�̕`��

			//�����^�C�g�����(�w���v�I��)�ł̕`�揈����������

			break;//�^�C�g�����(�w���v�I��)�𔲂���

		case Help2:	//�w���v���2

			//�����w���v���2�ł̍X�V������������
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//�E�L�[����������
				iGameState = Help3;				//�w���v���3�Ɉړ�����
				bPreesingRIGHTkey = TRUE;		//RIGHT�L�[�̉������ςȂ������TRUE�ɂ���
			}
			
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//���L�[����������
				iGameState = Help;				//�w���v���2�Ɉړ�����
				bPreesingLEFTkey = TRUE;		//LEFT�L�[�̉������ςȂ������TRUE�ɂ���
			}

			//�����w���v���2�ł̍X�V������������

			//�����w���v���2�ł̕`�揈����������

			DrawGraph(0, 0, HelpGraph_02, TRUE);
			
			//�����w���v���2�ł̕`�揈����������

			break;	//�w���v���2�𔲂���

		case Help3:	//�w���v���3

			//�����w���v���3�ł̍X�V������������
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//���L�[����������
				iGameState = Help2;				//�w���v���2�Ɉړ�����
				bPreesingLEFTkey = TRUE;		//LEFT�L�[�̉������ςȂ������TRUE�ɂ���
			}
			
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//�E�L�[����������
				iGameState = Help4;				//�w���v���4�Ɉړ�����
				bPreesingRIGHTkey = TRUE;		//RIGHT�L�[�̉������ςȂ������TRUE�ɂ���
			}
			//�����w���v���3�ł̍X�V������������

			//�����w���v���3�ł̕`�揈����������

			DrawGraph(0, 0, HelpGraph_03, TRUE);

			//�����w���v���3�ł̕`�揈����������

			break;//�w���v���3�𔲂���

		case Help4:	//�w���v���4

			//�����w���v���4�ł̍X�V������������
			
			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Title;				//�^�C�g����ʂɈړ�����
				bPreesingZkey = TRUE;			//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}
			
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//���L�[����������
				iGameState = Help3;				//�w���v���3�Ɉړ�����
				bPreesingLEFTkey = TRUE;		//LEFT�L�[�̉������ςȂ������TRUE�ɂ���
			}

			//�����w���v���4�ł̍X�V������������

			//�����w���v���4�ł̕`�揈����������

			DrawGraph(0, 0, HelpGraph_04, TRUE);
			//�����w���v���4�ł̕`�揈����������

			break;//�w���v���4�𔲂���

		case  GameClear://�Q�[���N���A���

			StopSoundMem(sounds.Main[0]);
			sounds.Title[1] = 1;

			if (sounds.GameClear[1] == 1){
				PlaySoundMem(sounds.GameClear[0], DX_PLAYTYPE_LOOP);
				sounds.GameClear[1] = 0;
			}

			DrawGraph(0, 0, GameClearGraph, TRUE);//�N���A��ʂ̕`��

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Title;			//�^�C�g����ʂɂ���
				bPreesingZkey = TRUE;		//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}

			break;		//�Q�[���N���A��ʂ𔲂���

		case GameOver://�Q�[���I�[�o�[���

			sounds.Title[1] = 1;
			StopSoundMem(sounds.Main[0]);
		
			if (sounds.GameOver[1] == 1){
				PlaySoundMem(sounds.GameOver[0], DX_PLAYTYPE_LOOP);
				sounds.GameOver[1] = 0;
			}

			DrawGraph(0, 0, GameOverGraph, TRUE);

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Z�L�[����������
				iGameState = Title;		//�^�C�g����ʂɂ���
				bPreesingZkey = TRUE;	//Z�L�[�̉������ςȂ������TRUE�ɂ���
			}

			break;//�Q�[���I�[�o�[��ʂ𔲂���

		case Main:		//���C�����
			
		//�������C����ʂł̍X�V������������

			//�G�t�F�N�g�̍X�V����
			if (effect.iAniCnt > 0){
				effect.iAniCnt += 1;
			}
			//�G�t�F�N�g��4�R�}�ڂ�`�悵����Đ����I��
			if (effect.iAniCnt > 12){
				effect.iAniCnt = 0;
			}

			//�G�t�F�N�g2�̍X�V����
			if (effect2.iAniCnt > 0){
				effect2.iAniCnt += 1;
			}
			//�G�t�F�N�g2��4�R�}�ڂ�`�悵����Đ����I��
			if (effect2.iAniCnt > 12){
				effect2.iAniCnt = 0;
			}

		//���@�̃W�����v

			//���@�̃W�����v���x��1
			if (player.iJumpLevel == 1){					//���@�̃W�����v���x����1�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++)
				{
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}
				
				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 20;					//�W�����v�l��20�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}
			
			//���@�̃W�����v���x��2
			if (player.iJumpLevel == 2){					//���@�̃W�����v���x����2�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++)
				{
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 21;					//�W�����v�l��21�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//���@�̃W�����v���x��3
			if (player.iJumpLevel == 3){					//���@�̃W�����v���x����3�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++){
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 22;					//�W�����v�l��22�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//���@�̃W�����v���x��4
			if (player.iJumpLevel == 4){					//���@�̃W�����v���x����4�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++)
				{
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 23;					//�W�����v�l��23�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//���@�̃W�����v���x��5
			if (player.iJumpLevel == 5){					//���@�̃W�����v���x����5�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++)
				{
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 24;					//�W�����v�l��24�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//���@�̃W�����v���x��6
			if (player.iJumpLevel == 6){					//���@�̃W�����v���x����6�̎�

				player.iY -= player.iJump;				//���@��Y���W����W�����v�̒l���}�C�i�X����
				bool bCheckGroundHit = FALSE;
				for (int i = 0; i < OBS1; i++)
				{
					if (player.iJump < 0 && (player.iY + player.iSize > obs1[i].iY && player.iX + player.iSize > obs1[i].iX && player.iX < obs1[i].iX + obs1[i].iSize_X))
					{
						bCheckGroundHit = TRUE;
						player.bJumpFlag = TRUE;
						player.iJump = 0;
						break;
					}
				}

				if (bCheckGroundHit == FALSE){
					player.iJump -= 1;					//�W�����v�l����|1����
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
				{
					player.iJump = 0;					//�W�����v�l��0�ɂ���
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//�X�y�[�X�L�[��������Ă��āA���@�̒��n�t���O��1�̎�
				{
					player.iJump = 25;					//�W�����v�l��25�ɂ���
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//���@�̃W�����v�͂̑���
			if (player.iJumpFlag == 0 && player.iJumpLevel2 > 0)	//���@�̃W�����v�t���O��0�A�W�����v�t���O2��0���傫���Ƃ�
			{
				player.iJumpLevel += 1;		//���@�̃W�����v���x����1�v���X����
				player.iJumpLevel2 -= 1;	//���@�̃W�����v���x��2��1�}�C�i�X����
			}

		//�G�̃W�����v

			//�G�̃W�����v���x��1
			if (enemy.iJumpLevel == 1){				//�G�̃W�����v���x����1�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����

				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){		//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}
				
				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX+enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 20;					//�W�����v�l��20�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//�G�̃W�����v���x��2
			if (enemy.iJumpLevel == 2){					//�G�̃W�����v���x����1�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����

				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 50 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 21;					//�W�����v�l��21�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//�G�̃W�����v���x��3
			if (enemy.iJumpLevel == 3){					//�G�̃W�����v���x����1�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����
				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){	//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;						//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 50 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 22;					//�W�����v�l��22�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//�G�̃W�����v���x��4
			if (enemy.iJumpLevel == 4){				//�G�̃W�����v���x����4�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����

				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){		//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 23;					//�W�����v�l��23�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//�G�̃W�����v���x��5
			if (enemy.iJumpLevel == 5){				//�G�̃W�����v���x����5�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����

				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){		//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 24;					//�W�����v�l��20�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//�G�̃W�����v���x��6
			if (enemy.iJumpLevel == 6){				//�G�̃W�����v���x����6�̎�
				enemy.iY -= enemy.iJump;				//�G��Y���W����W�����v�̒l���}�C�i�X����

				enemy.iJump -= 1;						//�W�����v�l����|1����

				if (enemy.iY + enemy.iSize >= 566){		//���@��Y���W�{���@�̉摜�T�C�Y������566�ȏ�̎�
					enemy.iJump = 0;					//�W�����v�l��0�ɂ���
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//���@�̃W�����v�t���O��0�ɂ���
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//�G�ƃA�C�e�����߂Â�����
							enemy.iJumpFlag2 = 1;						//�W�����v�t���O��2��1�ɂ���
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//�G�Ə�Q��1���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//�G�Ə�Q��2���߂Â�����
						enemy.iJumpFlag2 = 1;						//�W�����v�t���O2��1�ɂ���
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//�W�����v�t���O2��1�ŁA�G��Y���W�{�G�̉摜�T�C�Y������566�̎�
					enemy.iJump = 25;					//�W�����v�l��25�ɂ���
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

		//�G�̃W�����v�͂̑���
		if (enemy.iJumpFlag == 0 &&enemy.iJumpLevel2 > 0)//�G�̃W�����v�t���O��0�ŃW�����v���x��2��0���傫���Ƃ�
		{
			enemy.iJumpLevel += 1;		//�G�̃W�����v���x����1�v���X����
			enemy.iJumpLevel2 -= 1;		//�G�̃W�����v���x��2��1�}�C�i�X����
		}

		for (int i = 0; i < ITEM; i++)
		{
			if (item[i].iX + item[i].iSize < -100)	//�A�C�e������ʊO�ɏo����
				item[i].iLive = FALSE;				//�A�C�e���̐��������FALSE�ɂ���
		}

		effect.iX = player.iX;	//�G�t�F�N�g�̈ʒu
		effect.iY = player.iY;	//�G�t�F�N�g�̈ʒu

		effect2.iX = enemy.iX;	//�G�t�F�N�g�̈ʒu
		effect2.iY = enemy.iY;	//�G�t�F�N�g�̈ʒu

			//���@�ƃA�C�e���Ƃ̂����蔻��
				//���@�̓��ƃA�C�e���̉��Ƃ̓����蔻��
		for (int i = 0; i < ITEM; i++){
			if (ItemHead(item[i].iLive, player.iY, item[i].iY, item[i].iSize, player.iSize, player.iX, player.iSize2,item[i].iX)){
				item[i].iLive = FALSE;		//�A�C�e���̐��������FALSE�ɂ���
				player.iJumpLevel2 += 1;	//���@�̃W�����v���x��2��1�v���X����
				sounds.Powerup[1] = 1;
				if (effect.iAniCnt == 0){
					effect.iAniCnt = 1;
					break;
				}
			}
		}
		//���@�̉E���ƃA�C�e���̍����Ƃ̂����蔻��
		for (int i = 0; i < ITEM; i++){
			if (ItemSide(item[i].iLive, player.iX, player.iSize, item[i].iX, player.iSize2, player.iY, item[i].iY, item[i].iSize)){
				item[i].iLive = FALSE;		//�A�C�e���̐��������FALSE�ɂ���
				player.iJumpLevel2 += 1;	//���@�̃W�����v���x��2��1�v���X����
				sounds.Powerup[1] = 1;
				if (effect.iAniCnt == 0){
					effect.iAniCnt = 1;
					break;
				}
			}
		}

			//�G�ƃA�C�e���̂����蔻��
		//�G�̓��ƃA�C�e���̉��Ƃ̓����蔻��
		for (int i = 0; i < ITEM; i++){
			if (ItemHead(item[i].iLive, enemy.iY, item[i].iY, item[i].iSize, enemy.iSize, enemy.iX, enemy.iSize2, item[i].iX)){
				item[i].iLive = FALSE;		//�A�C�e���̐��������FALSE�ɂ���
				enemy.iJumpLevel2 += 1;		//�G�̃W�����v���x��2��1�v���X����
				sounds.Powerup[1] = 1;
				if (effect2.iAniCnt == 0){
					effect2.iAniCnt = 1;
					break;
				}
			}
		}
		//�G�̉E���ƃA�C�e���̍����Ƃ̂����蔻��
		for (int i = 0; i < ITEM; i++){
			if (ItemSide(item[i].iLive, enemy.iX, enemy.iSize, item[i].iX, enemy.iSize2, enemy.iY, item[i].iY, item[i].iSize)){
				item[i].iLive = FALSE;		//�A�C�e���̐��������FALSE�ɂ���
				enemy.iJumpLevel2 += 1;	//�G�̃W�����v���x��2��1�v���X����
				sounds.Powerup[1] = 1;
				if (effect2.iAniCnt == 0){
					effect2.iAniCnt = 1;
					break;
				}
			}
		}
		
		//��Q��1�Ǝ��@�̂����蔻��
		for (int i = 0; i < OBS1; i++){
			if (ObsSide(obs1[i].iLive, player.iX, player.iSize, obs1[i].iX, player.iY, obs1[i].iY, obs1[i].iSize_Y))
				player.iX -= ISPEED;
		}

		//��Q��1�̏�Ǝ��@�̂����蔻��
		for (int i = 0; i < OBS1; i++){
			if (ObsTop(obs1[i].iLive, player.iY, player.iSize, obs1[i].iY,player.iX, obs1[i].iX, obs1[i].iSize_X)){
				player.iY = obs1[i].iY - player.iSize;	//���@�̍��W����Q���̏�ɌŒ肷��
				player.iobsUpFlag = 1;
			}
		}
		
		//��Q��1�ƓG�̂����蔻��
		for (int i = 0; i < OBS1; i++){
			if (ObsSide(obs1[i].iLive, enemy.iX, enemy.iSize, obs1[i].iX, enemy.iY, obs1[i].iY, obs1[i].iSize_Y))
				enemy.iX -= ISPEED;
		}

		//��Q��1�̏�ƓG�̂����蔻��
		for (int i = 0; i < OBS1; i++){
			if (ObsTop(obs1[i].iLive, enemy.iY, enemy.iSize, obs1[i].iY, enemy.iX, obs1[i].iX, obs1[i].iSize_X)){
				enemy.iY = obs1[i].iY - enemy.iSize;	//���@�̍��W����Q���̏�ɌŒ肷��
				enemy.iobsUpFlag = 1;
			}
		}

		//��Q��2�Ǝ��@�̂����蔻��
		for (int i = 0; i < OBS2; i++){
			if (ObsSide(obs2[i].iLive, player.iX, player.iSize, obs2[i].iX, player.iY, obs2[i].iY, obs2[i].iSize_Y))
				player.iX -= ISPEED;
		}

		//��Q��2�̏�Ǝ��@�̂����蔻��
		for (int i = 0; i < OBS2; i++){
			if (ObsTop(obs2[i].iLive, player.iY, player.iSize, obs2[i].iY, player.iX, obs2[i].iX, obs2[i].iSize_X)){
				player.iY = obs2[i].iY - player.iSize;	//���@�̍��W����Q���̏�ɌŒ肷��
				player.iobsUpFlag = 1;
				iGameState = GameOver;
				sounds.Damage[1] = 1;
			}
		}

		//��Q��2�ƓG�̂����蔻��
		for (int i = 0; i < OBS2; i++){
			if (ObsSide(obs2[i].iLive, enemy.iX, enemy.iSize, obs2[i].iX, enemy.iY, obs2[i].iY, obs2[i].iSize_Y))
				enemy.iX -= ISPEED;
		}

		//��Q��2�̏�ƓG�̂����蔻��
		for (int i = 0; i < OBS2; i++){
			if (ObsTop(obs2[i].iLive, enemy.iY, enemy.iSize, obs2[i].iY, enemy.iX, obs2[i].iX, obs2[i].iSize_X)){
				enemy.iY = obs2[i].iY - enemy.iSize;	//���@�̍��W����Q���̏�ɌŒ肷��
				enemy.iobsUpFlag = 1;
			}
		}

		for (int i = 0; i < OBS1; i++){
		if (CanMove(obs1[i].iLive))		//��Q��1�̈ړ�
			obs1[i].iX -= ISPEED;
		}

		for (int i = 0; i < OBS2; i++){
			if (CanMove(obs2[i].iLive))	//��Q��2�̈ړ�
				obs2[i].iX -= ISPEED;
		}

		for (int i = 0; i < ITEM; i++){
			if (CanMove(item[i].iLive))
				item[i].iX -= ISPEED;	//�A�C�e���̈ړ�
		}

		//�S�[���̈ړ�
		goal.iX -= ISPEED;			//�S�[����X���W���}�C�i�X����

		//���@�ƃS�[���̂����蔻��
		if (player.LoseFlag == 0 && player.iX + player.iSize >= goal.iX)	//���@���S�[���ɂ�����
			enemy.LoseFlag = 1;
		
		//�G�ƃS�[���̂����蔻��
		if (enemy.LoseFlag == 0 && enemy.iX + enemy.iSize >= goal.iX)		//�G���S�[���ɂ�����
			player.LoseFlag = 1;
		
		if (player.iX <= enemy.iX)		//���@�ƓG���d�Ȃ�����
			iGameState = GameOver;		//�Q�[���I�[�o�[��ʂɂ���
		
		//���@����ɂ�����
		if (enemy.iX + enemy.iSize > goal.iX && enemy.LoseFlag == 1)
			iGameState = GameClear;					//�Q�[���N���A��ʂɂ���
		
		//�G����ɃS�[���ɂ�����
		if (player.iX + player.iSize > goal.iX && player.LoseFlag == 1)
			iGameState = Lose;						//������ʂɂ���
		
		for (int i = 0; i < OBS1; i++){
			if (obs1[i].iX + obs1[i].iSize_X < -500)	//��Q��1����ʊO�ɏo����
				obs1[i].iLive = FALSE;					//��Q��1�̐��������FALSE�ɂ���
		}
		
		for (int i = 0; i < OBS2; i++){
			if (obs2[i].iX + obs2[i].iSize_X < -500)	//��Q��2����ʊO�ɏo����
				obs2[i].iLive = FALSE;					//��Q��2�̐��������FALSE�ɂ���
		}

		if (player.iX < -10)				//���@����ʊO�ɏo����
			iGameState = GameOver;			//�Q�[���I�[�o�[��ʂֈړ�����

		if (enemy.iX < 0){					//�G����ʊO�ɏo����
			enemy.iX = player.iX - 300;		//���@�̌��ɏo��������	
			enemy.iY = 0;
			if (enemy.iY > 300)				//�G��Y���W��300��菬�����Ƃ���
				enemy.iJumpFlag2 = 0;		//�G�̃W�����v�t���O��0�ɂ���
		}

		//�w�i�̍X�V����
		for (int i = 0; i < BACK; i++){
			bkGround[i].iX -= ISPEED;		//�w�i�𓮂���
			if (bkGround[i].iX <= -1280)	//�w�i����ʊO�֏o����
				bkGround[i].iX = WINDOW_X;		//�w�i��X���W��WINDOW_X�ɂ���
		}

		//���d�w�i�̍X�V����
		for (int i = 0; i < MULTIBACK; i++){
			multiBack[i].iX -= ISPEED;
			if (multiBack[i].iX <= -1280)
				multiBack[i].iX = WINDOW_X;
		}

		//���@�̍X�V����
		player.iAnicnt = (player.iAnicnt + 1) % (4 * 10);

		//�G�̍X�V����
		enemy.iAnicnt = (enemy.iAnicnt + 1) % (4 * 10);

		//���y�̍X�V����
		StopSoundMem(sounds.Title[0]);

		//���C�����y�̍Đ�
		if (sounds.Main[1] == 1){
			PlaySoundMem(sounds.Main[0], DX_PLAYTYPE_LOOP);
			sounds.Main[1] = 0;
		}

		//�W�����v���̍Đ��J�n
		if (sounds.Jump[1] == 1){
			PlaySoundMem(sounds.Jump[0], DX_PLAYTYPE_BACK);
			sounds.Jump[1] = 0;
		}

		//�_���[�W���̍Đ��J�n
		if (sounds.Damage[1] == 1){
			PlaySoundMem(sounds.Damage[0], DX_PLAYTYPE_BACK);
			sounds.Damage[1] = 0;
		}

		//�A�C�e���擾���̉��̍Đ��J�n
		if (sounds.Powerup[1] == 1){
			PlaySoundMem(sounds.Powerup[0], DX_PLAYTYPE_BACK);
			sounds.Powerup[1] = 0;
		}

		//�������C����ʂł̍X�V������������

		//�������C����ʂł̕`�揈����������

		for (int i = 0; i < BACK; i++)
			DrawGraph(bkGround[i].iX, 0, bkGround[i].iGraph, FALSE);				//�w�i�̕`��
		
		DrawGraph(player.iX, player.iY, iPlayerGraph[player.iAnicnt / 10], TRUE);	//���@�̕`��

		DrawGraph(enemy.iX, enemy.iY, iEnemyGraph[enemy.iAnicnt / 10], TRUE);		//�G�̕`��

		for (int i = 0; i < MULTIBACK; i++)
			DrawGraph(multiBack[i].iX, 720 - 153, multiBack[i].iGraph, TRUE);		//���̕`��

		for (int i = 0; i < ITEM; i++){
			if (item[i].iLive == TRUE)										//�A�C�e���̐������肪TRUE�Ȃ�
				DrawGraph(item[i].iX, item[i].iY, item[i].iGraph, TRUE);	//�A�C�e����`�悷��
		}

		for (int i = 0; i < OBS1; i++){
			if (obs1[i].iLive == TRUE)										//��Q��1�̐������肪TRUE�Ȃ�
				DrawGraph(obs1[i].iX, obs1[i].iY, obs1[i].iGraph, TRUE);	//��Q��1��`�悷��
		}
		
		for (int i = 0; i < OBS2; i++){
			if (obs2[i].iLive == TRUE)										//��Q��2�̐������肪TRUE�Ȃ�
				DrawGraph(obs2[i].iX, obs2[i].iY, obs2[i].iGraph, TRUE);	//��Q��2��`�悷��
		}

		DrawGraph(goal.iX, goal.iY, goal.iGraph, TRUE);						//�S�[���̕`��

		DrawGraph(effect.iX, effect.iY, EffectGraph[(effect.iAniCnt - 1) % 4], TRUE);//�G�t�F�N�g�̕`��
		DrawGraph(effect2.iX, effect2.iY, EffectGraph2[(effect2.iAniCnt - 1) % 4], TRUE);//�G�t�F�N�g�̕`��

		//�������C����ʂł̕`�揈����������
			break;
		default:
			break;
		}
		ScreenFlip();		//����ʂƕ\��ʂ̓��� 
		ClearDrawScreen();	//����ʂ̕`���S�ď���
	}
	DxLib_End();			// �c�w���C�u�����g�p�̏I������
  	return 0;				// �\�t�g�̏I�� 
}
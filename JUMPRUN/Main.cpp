#include "Main.h"
#include "CanMove.h"
#include "ObsCollision.h"
#include "ItemCollision.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{			

	ChangeWindowMode(FALSE);				//ウィンドウモードにする

	SetGraphMode(WINDOW_X, WINDOW_Y, 32);	//ウィンドウサイズを設定する

	if (DxLib_Init() == -1)			// ＤＸライブラリ初期化処理
	{
		return -1;					// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);	//裏画面を描画対象へ

	//▼▼▼▼この辺に初期化処理▼▼▼▼

	//自機の構造体
	struct Player
	{	
		int iX;					//自機のX座標
		int iY;					//自機のY座標
		int iSize = 60;			//自機の大きさ
		int iSize2 = 30;		//自機の大きさの半分
		int iJump;				//自機のジャンプ力
		int iJumpFlag;			//自機のジャンプフラグ
		int iJumpLevel;			//自機のジャンプレベル
		int iJumpLevel2;		//自機のジャンプレベル2
		int iLandingFlag;		//自機の着地フラグ
		bool bJumpFlag = TRUE;
		int iobsUpFlag;         //自機の障害物の上でのジャンプフラグ
		int iGraph;				//自機の画像
		int iAnicnt;			//自機のアニメーションカウント
		int LoseFlag;           //自機の負けフラグ1は負け
		bool iLive = TRUE;	//自機の生存判定
	};
	Player player;
	
	int iPlayerGraph[4]; 
	LoadDivGraph("media\\player_01.png", 4, 2, 2, 60, 60, iPlayerGraph);	//画像の読み込み

	//敵の構造体
	struct Enemy
	{
		int iX;					//敵のX座標
		int iY;					//敵のY座標
		int iSize = 60;			//敵の大きさ
		int iSize2 = 30;		//敵の大きさの半分
		int iJump;				//敵のジャンプ力
		int iJumpFlag;			//敵のジャンプフラグ
		int iJumpFlag2;			//敵のジャンプフラグ2
		int iJumpLevel;			//敵のジャンプレベル
		int iJumpLevel2;		//敵のジャンプレベル2
		int iLandingFlag;		//敵の着地フラグ
		int iobsUpFlag;         //敵の障害物の上でのジャンプフラグ
		int playerpointY;
		int iGraph;				//敵の画像
		int iAnicnt;			//敵のアニメーションカウント
		int LoseFlag;			//敵の負けフラグ
		bool iLive = TRUE;   //敵の生存判定
	};
	Enemy enemy;

	int iEnemyGraph[4];
	LoadDivGraph("media\\enemyDivAnime1.png", 4, 2, 2, 60, 60, iEnemyGraph);	//画像の読み込み
	
	//アイテムの構造体
	struct Item
	{
		int iX;					//アイテムのX座標
		int iY;					//アイテムのY座標
		int iSize = 70;			//アイテムの大きさ
		int iGraph;				//アイテムの画像
		bool iLive;				//アイテムの生存判定
	};
	
	Item item[ITEM];	//アイテムの数は100個

	int iItemGraph = LoadGraph("media\\item.png");	//画像の読み込み
	for (int i = 0; i < ITEM; i++){
		item[i].iGraph = iItemGraph;				//画像の割り当て
	}

	//障害物1の構造体
	struct Obs1
	{
		int iX;				//障害物1のX座標
		int iY;				//障害物1のY座標
		int iGraph;			//障害物1の画像
		int iSize_X = 70;	//障害物1の横幅の画像サイズ
		int iSize_Y = 70;	//障害物1の縦幅の画像サイズ
		bool iLive;		//障害物1の生存判定
	};

	Obs1 obs1[OBS1];//障害物1の数は100個
	int obs1Graph = LoadGraph("media\\obs1_01_01.jpg");	//画像の読み込み
	for (int i = 0; i < OBS1; i++){
		obs1[i].iGraph = obs1Graph;					//画像の割り当て
	}

	//障害物2の構造体
	struct Obs2
	{
		int iX;				//障害物2のX座標
		int iY;				//障害物2のY座標
		int iGraph;			//障害物2の画像
		int iSize_X = 45;	//障害物2の横幅の画像サイズ
		int iSize_Y = 150;	//障害物2の縦幅の画像サイズ
		bool iLive;		//障害物2の生存判定
	};

	Obs2 obs2[OBS2];		//障害物2の数は20個
	int obs2Graph = LoadGraph("media\\obs_02.png");	//画像の読み込み
	for (int i = 0; i < OBS2; i++){
		obs2[i].iGraph = obs2Graph;					//画像の割り当て
	}

	//ゴールの構造体
	struct Goal
	{
		int iX;			//ゴールのX座標
		int iY;			//ゴールのY座標
		int iGraph;		//ゴールの画像
		int iSize = 100;//ゴールの画像サイズ
	};

	Goal goal;
	int GoalGraph = LoadGraph("media\\Goal_01.png");	//ゴールの画像の読み込み
	goal.iGraph = GoalGraph;							//ゴールの画像の割り当て

	//エフェクトの構造体
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

	//エフェクト2の構造体
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

	//画面遷移の列挙型
	enum eGameState
	{
		Title,			//タイトル画面
		Title_Help,		//タイトル画面でヘルプボタン選択
		Main,			//メイン画面
		GameOver,		//ゲームオーバー
		Lose,			//負け画面
		GameClear,		//ゲームクリア
		Help,           //ヘルプ画面
		Help2,			//ヘルプ画面2
		Help3,			//ヘルプ画面3
		Help4,			//ヘルプ画面4
		End				//ゲーム終了
	};

	int iGameState = Title;

	//▼▼▼▼押しっぱなし判定▼▼▼▼

	bool bPreesingSPASEkey = FALSE;		//スペースキーの押しっぱなし判定
	bool bPreesingZkey = FALSE;			//Zキーの押しっぱなし判定
	bool bPreesingUPkey = FALSE;			//UPキーの押しっぱなし判定
	bool bPreesingDOWNkey = FALSE;		//DOWNキーの押しっぱなし判定
	bool bPreesingRIGHTkey = FALSE;		//RIGHTキーの押しっぱなし判定
	bool bPreesingLEFTkey = FALSE;		//LEFTキーの押しっぱなし判定

	//▲▲▲▲押しっぱなし判定▲▲▲▲

	//背景関係
	int iBackGraph;
	iBackGraph = LoadGraph("media\\BackGraph_02.png");//背景画像の読み込み

		//背景の構造体
	struct Back
	{
		int iX;				//背景のX座標
		int iGraph;			//背景の画像
	};
	Back bkGround[BACK];
	bkGround[0].iX = 0;
	bkGround[1].iX = 1280;

	for (int i = 0; i < BACK; i++){
		bkGround[i].iGraph = iBackGraph;	//背景画像の割り当て
	}

	//多重背景
	struct MultiBack
	{
		int iX;				//多重背景のX座標
		int iY;
		int iSize_X = 1280;	//多重背景の横幅の画像サイズ
		int iSize_Y = 153;	//多重背景の縦幅の画像サイズ
		int iGraph;			//多重背景の画像
	};
	MultiBack multiBack[MULTIBACK];
	int iMultiBackGraph = LoadGraph("media\\yuka_011.png");
	multiBack[0].iX = 0;
	multiBack[1].iX = WINDOW_X;

	for (int i = 0; i < MULTIBACK; i++){
		multiBack[i].iGraph = iMultiBackGraph;
		multiBack[i].iY = 720 - multiBack[i].iSize_Y;
	}

	//画像読み込み
	int StartGraph_01 = LoadGraph("media\\START_01.jpg");//タイトル画面の読み込み
	int StartGraph_02 = LoadGraph("media\\START_02.jpg");//タイトル画面2の読み込み
	int StartGraph_03 = LoadGraph("media\\START_03.jpg");//タイトル画面3の読み込み
	int HelpGraph_01 = LoadGraph("media\\Help_01.jpg");//ヘルプ画面01の読み込み
	int HelpGraph_02 = LoadGraph("media\\Help_02.jpg");//ヘルプ画面02の読み込み
	int HelpGraph_03 = LoadGraph("media\\Help_03.jpg");//ヘルプ画面03の読み込み
	int HelpGraph_04 = LoadGraph("media\\Help_04.jpg");//ヘルプ画面04の読み込み
	int GameClearGraph = LoadGraph("media\\GAMECLEAR1.jpg");//クリア画面の読み込み
	int GameOverGraph = LoadGraph("media\\GAMEOVER1.jpg");//ゲームオーバー画面の読み込み
	int LoseGraph = LoadGraph("media\\Lose1.jpg");//負け画面の読み込み
	
	//音楽関係
	struct Sounds
	{
		//[1]が1なら再生開始
		int BGMDate[2];
		int Jump[2];	//ジャンプ音
		int Damage[2];	//ダメージ音
		int Main[2];	//メイン音楽
		int Title[2]; //タイトル音楽
		int GameClear[2];//ゲームクリア
		int GameOver[2];//ゲームオーバー
		int  Powerup[2]; //アイテム取得時の音
	};
	Sounds sounds;

	sounds.Jump[0] = LoadSoundMem("media\\sound\\Jump.mp3");			//ジャンプ音の読み込み
	sounds.Damage[0] = LoadSoundMem("media\\sound\\Damage.mp3");		//ダメージ音の読み込み
	sounds.Title[0] = LoadSoundMem("media\\sound\\title.mp3");			//タイトル音の読み込み
	sounds.GameClear[0] = LoadSoundMem("media\\sound\\clear.mp3");		//ゲームクリア音の読み込み
	sounds.GameOver[0] = LoadSoundMem("media\\sound\\gemaover.mp3");	//ゲームオーバー音の読み込み
	sounds.Main[0] = LoadSoundMem("media\\sound\\main.mp3");			//メイン音楽の読み込み
	sounds.Powerup[0] = LoadSoundMem("media\\sound\\PowerUP.mp3");		//アイテム取得時の音の読み込み

	sounds.Damage[1] = 0;
	sounds.Jump[1] = 0;
	sounds.Powerup[1] = 0;
	sounds.Title[1] = 1;
	sounds.GameClear[1] = 1;
	sounds.GameOver[1] = 1;
	sounds.Main[1] = 1;

	//▲▲この辺に初期化処理▲▲▲▲

	//メイン処理
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

		case End:		//エンド画面
			DrawGraph(0, 0, StartGraph_03, TRUE);	//負け画面の描画

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){	//DOWNキーを押したら
				iGameState = Title;			//タイトル画面にする
				bPreesingDOWNkey = TRUE;	//DOWNキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){	//UPキーを押したら
				iGameState = Title_Help;		//タイトルヘルプ選択画面にする
				bPreesingUPkey = TRUE;	//UPキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら				
				bPreesingZkey = TRUE;	//Zキーの押しっぱなし判定をTRUEにする
				return 1;
			}

			break;

		case Lose://負け画面
			
			sounds.Title[1] = 1;
			StopSoundMem(sounds.Title[0]);
			StopSoundMem(sounds.Main[0]);
			StopSoundMem(sounds.GameClear[0]);
			
			if (sounds.GameOver[1] == 1){
				PlaySoundMem(sounds.GameOver[0], DX_PLAYTYPE_LOOP);
				sounds.GameOver[1] = 0;
			}

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Title;				//タイトル画面に移動する
				bPreesingZkey = TRUE;			//Zキーの押しっぱなし判定をTRUEにする
			}

			DrawGraph(0, 0, LoseGraph, TRUE);	//負け画面の描画

			break;//負け画面を抜ける

		case Help:		//ヘルプ画面

			//▼▼ヘルプ画面での更新処理▼▼▼▼

			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//右キーを押したら
				iGameState = Help2;				//ヘルプ画面2に移動する
				bPreesingRIGHTkey = TRUE;
			}

			//▲▲ヘルプ画面での更新処理▲▲▲▲

			//▼▼ヘルプ画面での描画処理▼▼▼▼

			DrawGraph(0, 0, HelpGraph_01, TRUE);	//ヘルプ画面の描画

			//▲▲ヘルプ画面での描画処理▲▲▲▲

			break;      //ヘルプ画面を抜ける

		case Title:		//タイトル画面

			//▼▼タイトル画面での初期化処理▼▼▼▼

			 //自機
			player.iX = 640;					//自機のX座標の初期化処理
			player.iY = 566 - player.iSize;		//自機のY座標の初期化処理
			player.iJump = 0;					//自機のジャンプ力の初期化処理
			player.iJumpFlag = 0;				//自機のジャンプフラグの初期化
			player.iJumpLevel = 1;				//自機のジャンプレベルの初期化
			player.iJumpLevel2 = 0;				//自機のジャンプレベル2の初期化
			player.iLandingFlag = 0;			//自機の着地フラグの初期化
			player.iobsUpFlag = 0;				//自機の障害物の上部当たりフラグの初期化
			player.iAnicnt = 0;					//自機のアニメーションカウントの初期化
			player.LoseFlag = 0;				//自機の負けフラグの初期化
			
			 //敵
			enemy.iX = 500;						//敵のX座標の初期化処理
			enemy.iY = 566 - player.iSize;		//敵のX座標の初期化処理
			enemy.iJump = 0;					//敵のジャンプ力の初期化処理
			enemy.iJumpFlag = 0;				//敵のジャンプフラグの初期化
			enemy.iJumpLevel = 1;				//敵のジャンプレベルの初期化
			enemy.iJumpLevel2 = 0;				//敵のジャンプレベル2の初期化
			enemy.iLandingFlag = 0;				//敵の着地フラグの初期化
			enemy.iobsUpFlag = 0;				//敵の障害物の上部当たりフラグの初期化
			enemy.iAnicnt = 1;					//敵のアニメーションカウントの初期化
			enemy.LoseFlag = 0;					//敵の負けフラグの初期化

			//エフェクトの初期化
			effect.iAniCnt = 0;
			effect2.iAniCnt = 0;

			//アイテム
			for (int i = 0; i < ITEM; i++)
				item[i].iLive = TRUE;           //アイテムの生存判定をすべてTRUEにする
			
			//アイテムの座標指定
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

			//障害物
			for (int i = 0; i < OBS1; i++)
				obs1[i].iLive = TRUE;		//障害物1の生存判定をTRUEにする
			
			for (int i = 0; i < OBS2; i++)
				obs2[i].iLive = TRUE;		//障害物2の生存判定をTRUEにする
			
			//障害物1の座標指定

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

			//障害物２の座標指定
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
				obs2[i].iY =  multiBack[0].iY - obs2[i].iSize_Y;	//障害物2のY座標の指定

			//ゴール
				//ゴールの座標指定
			goal.iX = 11000;
			goal.iY =  multiBack[0].iY - goal.iSize;

			//▲▲タイトル画面での初期化処理▲▲▲▲

			//▼▼タイトル画面での更新処理▼▼▼▼
			
			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Main;				//メイン画面に移動する
				bPreesingZkey = TRUE;			//Zキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){//DOWNキーを押したら
				iGameState = Title_Help;		//タイトル(ヘルプ選択)にする
				bPreesingDOWNkey = TRUE;		//DOWNキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){//UPキーを押したら
				iGameState = End;		//エンド画面にする
				bPreesingUPkey = TRUE;	//UPキーの押しっぱなし判定をTRUEにする
			}

			//音楽
			sounds.GameClear[1] = 1;
			sounds.GameOver[1] = 1;
			sounds.Main[1] = 1;

			StopSoundMem(sounds.GameOver[0]);
			StopSoundMem(sounds.GameClear[0]);
			StopSoundMem(sounds.Main[0]);

			//タイトルの音楽再生開始

			if (sounds.Title[1] == 1){
				PlaySoundMem(sounds.Title[0], DX_PLAYTYPE_LOOP);
				sounds.Title[1] = 0;
			}

			//▲▲タイトル画面での更新処理▲▲▲▲

			//▼▼タイトル画面での描画処理▼▼▼▼

			DrawGraph(0, 0, StartGraph_01, TRUE);		//タイトル画面の描画

			//▲▲タイトル画面での描画処理▲▲▲▲

			break;	//タイトル画面を抜ける

		case Title_Help:	//タイトル画面(ヘルプ選択)

			//▼▼タイトル画面(ヘルプ選択)での更新処理▼▼▼▼

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Help;		//ヘルプ画面にする
				bPreesingZkey = TRUE;	//Zキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_UP) == 1 && bPreesingUPkey == FALSE){	//UPキーを押したら
				iGameState = Title;		//タイトル画面にする
				bPreesingUPkey = TRUE;	//UPキーの押しっぱなし判定をTRUEにする
			}

			if (CheckHitKey(KEY_INPUT_DOWN) == 1 && bPreesingDOWNkey == FALSE){	//DOWNキーを押したら
				iGameState = End;			//エンド画面にする
				bPreesingDOWNkey = TRUE;	//DOWNキーの押しっぱなし判定をTRUEにする
			}

			//▲▲タイトル画面(ヘルプ選択)での更新処理▲▲▲▲

			//▼▼タイトル画面(ヘルプ選択)での描画処理▼▼▼▼

			DrawGraph(0, 0, StartGraph_02, TRUE);//タイトル画面2の描画

			//▲▲タイトル画面(ヘルプ選択)での描画処理▲▲▲▲

			break;//タイトル画面(ヘルプ選択)を抜ける

		case Help2:	//ヘルプ画面2

			//▼▼ヘルプ画面2での更新処理▼▼▼▼
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//右キーを押したら
				iGameState = Help3;				//ヘルプ画面3に移動する
				bPreesingRIGHTkey = TRUE;		//RIGHTキーの押しっぱなし判定をTRUEにする
			}
			
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//左キーを押したら
				iGameState = Help;				//ヘルプ画面2に移動する
				bPreesingLEFTkey = TRUE;		//LEFTキーの押しっぱなし判定をTRUEにする
			}

			//▲▲ヘルプ画面2での更新処理▲▲▲▲

			//▼▼ヘルプ画面2での描画処理▼▼▼▼

			DrawGraph(0, 0, HelpGraph_02, TRUE);
			
			//▲▲ヘルプ画面2での描画処理▲▲▲▲

			break;	//ヘルプ画面2を抜ける

		case Help3:	//ヘルプ画面3

			//▼▼ヘルプ画面3での更新処理▼▼▼▼
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//左キーを押したら
				iGameState = Help2;				//ヘルプ画面2に移動する
				bPreesingLEFTkey = TRUE;		//LEFTキーの押しっぱなし判定をTRUEにする
			}
			
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && bPreesingRIGHTkey == FALSE){	//右キーを押したら
				iGameState = Help4;				//ヘルプ画面4に移動する
				bPreesingRIGHTkey = TRUE;		//RIGHTキーの押しっぱなし判定をTRUEにする
			}
			//▲▲ヘルプ画面3での更新処理▲▲▲▲

			//▼▼ヘルプ画面3での描画処理▼▼▼▼

			DrawGraph(0, 0, HelpGraph_03, TRUE);

			//▲▲ヘルプ画面3での描画処理▲▲▲▲

			break;//ヘルプ画面3を抜ける

		case Help4:	//ヘルプ画面4

			//▼▼ヘルプ画面4での更新処理▼▼▼▼
			
			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Title;				//タイトル画面に移動する
				bPreesingZkey = TRUE;			//Zキーの押しっぱなし判定をTRUEにする
			}
			
			if (CheckHitKey(KEY_INPUT_LEFT) == 1 && bPreesingLEFTkey == FALSE){	//左キーを押したら
				iGameState = Help3;				//ヘルプ画面3に移動する
				bPreesingLEFTkey = TRUE;		//LEFTキーの押しっぱなし判定をTRUEにする
			}

			//▲▲ヘルプ画面4での更新処理▲▲▲▲

			//▼▼ヘルプ画面4での描画処理▼▼▼▼

			DrawGraph(0, 0, HelpGraph_04, TRUE);
			//▲▲ヘルプ画面4での描画処理▲▲▲▲

			break;//ヘルプ画面4を抜ける

		case  GameClear://ゲームクリア画面

			StopSoundMem(sounds.Main[0]);
			sounds.Title[1] = 1;

			if (sounds.GameClear[1] == 1){
				PlaySoundMem(sounds.GameClear[0], DX_PLAYTYPE_LOOP);
				sounds.GameClear[1] = 0;
			}

			DrawGraph(0, 0, GameClearGraph, TRUE);//クリア画面の描画

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Title;			//タイトル画面にする
				bPreesingZkey = TRUE;		//Zキーの押しっぱなし判定をTRUEにする
			}

			break;		//ゲームクリア画面を抜ける

		case GameOver://ゲームオーバー画面

			sounds.Title[1] = 1;
			StopSoundMem(sounds.Main[0]);
		
			if (sounds.GameOver[1] == 1){
				PlaySoundMem(sounds.GameOver[0], DX_PLAYTYPE_LOOP);
				sounds.GameOver[1] = 0;
			}

			DrawGraph(0, 0, GameOverGraph, TRUE);

			if (CheckHitKey(KEY_INPUT_Z) == 1 && bPreesingZkey == FALSE){	//Zキーを押したら
				iGameState = Title;		//タイトル画面にする
				bPreesingZkey = TRUE;	//Zキーの押しっぱなし判定をTRUEにする
			}

			break;//ゲームオーバー画面を抜ける

		case Main:		//メイン画面
			
		//▼▼メイン画面での更新処理▼▼▼▼

			//エフェクトの更新処理
			if (effect.iAniCnt > 0){
				effect.iAniCnt += 1;
			}
			//エフェクトの4コマ目を描画したら再生を終了
			if (effect.iAniCnt > 12){
				effect.iAniCnt = 0;
			}

			//エフェクト2の更新処理
			if (effect2.iAniCnt > 0){
				effect2.iAniCnt += 1;
			}
			//エフェクト2の4コマ目を描画したら再生を終了
			if (effect2.iAniCnt > 12){
				effect2.iAniCnt = 0;
			}

		//自機のジャンプ

			//自機のジャンプレベル1
			if (player.iJumpLevel == 1){					//自機のジャンプレベルが1の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}
				
				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 20;					//ジャンプ値を20にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}
			
			//自機のジャンプレベル2
			if (player.iJumpLevel == 2){					//自機のジャンプレベルが2の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 21;					//ジャンプ値を21にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//自機のジャンプレベル3
			if (player.iJumpLevel == 3){					//自機のジャンプレベルが3の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 22;					//ジャンプ値を22にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//自機のジャンプレベル4
			if (player.iJumpLevel == 4){					//自機のジャンプレベルが4の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 23;					//ジャンプ値を23にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//自機のジャンプレベル5
			if (player.iJumpLevel == 5){					//自機のジャンプレベルが5の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 24;					//ジャンプ値を24にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//自機のジャンプレベル6
			if (player.iJumpLevel == 6){					//自機のジャンプレベルが6の時

				player.iY -= player.iJump;				//自機のY座標からジャンプの値分マイナスする
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
					player.iJump -= 1;					//ジャンプ値から－1する
					player.bJumpFlag = FALSE;
				}

				if (player.iY + player.iSize >= 566)	//自機のY座標＋自機の画像サイズが高さ566以上の時
				{
					player.iJump = 0;					//ジャンプ値を0にする
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
					player.iY = 566 - player.iSize;
					player.bJumpFlag = TRUE;
				}

				if (player.iobsUpFlag == 1){
					player.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					player.iLandingFlag = 1;
				}

				if (CheckHitKey(KEY_INPUT_SPACE) == 1 && player.iLandingFlag == 1 && player.bJumpFlag == TRUE)//スペースキーが押されていて、自機の着地フラグが1の時
				{
					player.iJump = 25;					//ジャンプ値を25にする
					player.iLandingFlag = 0;
					player.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//自機のジャンプ力の増加
			if (player.iJumpFlag == 0 && player.iJumpLevel2 > 0)	//自機のジャンプフラグが0、ジャンプフラグ2が0より大きいとき
			{
				player.iJumpLevel += 1;		//自機のジャンプレベルを1プラスする
				player.iJumpLevel2 -= 1;	//自機のジャンプレベル2を1マイナスする
			}

		//敵のジャンプ

			//敵のジャンプレベル1
			if (enemy.iJumpLevel == 1){				//敵のジャンプレベルが1の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする

				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){		//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}
				
				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX+enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 20;					//ジャンプ値を20にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//敵のジャンプレベル2
			if (enemy.iJumpLevel == 2){					//敵のジャンプレベルが1の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする

				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){	//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 50 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 21;					//ジャンプ値を21にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//敵のジャンプレベル3
			if (enemy.iJumpLevel == 3){					//敵のジャンプレベルが1の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする
				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){	//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;						//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 50 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 22;					//ジャンプ値を22にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//敵のジャンプレベル4
			if (enemy.iJumpLevel == 4){				//敵のジャンプレベルが4の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする

				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){		//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 23;					//ジャンプ値を23にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//敵のジャンプレベル5
			if (enemy.iJumpLevel == 5){				//敵のジャンプレベルが5の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする

				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){		//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 24;					//ジャンプ値を20にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

			//敵のジャンプレベル6
			if (enemy.iJumpLevel == 6){				//敵のジャンプレベルが6の時
				enemy.iY -= enemy.iJump;				//敵のY座標からジャンプの値分マイナスする

				enemy.iJump -= 1;						//ジャンプ値から－1する

				if (enemy.iY + enemy.iSize >= 566){		//自機のY座標＋自機の画像サイズが高さ566以上の時
					enemy.iJump = 0;					//ジャンプ値を0にする
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
					enemy.iY = 566 - enemy.iSize;
					enemy.iJumpFlag2 = 0;
				}

				if (enemy.iobsUpFlag == 1){
					enemy.iJumpFlag = 0;				//自機のジャンプフラグを0にする
					enemy.iLandingFlag = 1;
				}

				for (int i = 0; i < ITEM; i++){
					if (item[i].iLive == TRUE){
						if (enemy.iX + enemy.iSize + 50 == item[i].iX)	//敵とアイテムが近づいたら
							enemy.iJumpFlag2 = 1;						//ジャンプフラグを2を1にする
					}
				}

				for (int i = 0; i < OBS1; i++){
					if (enemy.iX + enemy.iSize + 30 == obs1[i].iX)	//敵と障害物1が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				for (int i = 0; i < OBS2; i++){
					if (enemy.iX + enemy.iSize + 50 == obs2[i].iX)	//敵と障害物2が近づいたら
						enemy.iJumpFlag2 = 1;						//ジャンプフラグ2を1にする
				}

				if (enemy.iJumpFlag2 == 1 && enemy.iLandingFlag == 1){	//ジャンプフラグ2が1で、敵のY座標＋敵の画像サイズが高さ566の時
					enemy.iJump = 25;					//ジャンプ値を25にする
					enemy.iLandingFlag = 0;
					enemy.iobsUpFlag = 0;
					sounds.Jump[1] = 1;
				}
			}

		//敵のジャンプ力の増加
		if (enemy.iJumpFlag == 0 &&enemy.iJumpLevel2 > 0)//敵のジャンプフラグが0でジャンプレベル2が0より大きいとき
		{
			enemy.iJumpLevel += 1;		//敵のジャンプレベルを1プラスする
			enemy.iJumpLevel2 -= 1;		//敵のジャンプレベル2を1マイナスする
		}

		for (int i = 0; i < ITEM; i++)
		{
			if (item[i].iX + item[i].iSize < -100)	//アイテムが画面外に出たら
				item[i].iLive = FALSE;				//アイテムの生存判定をFALSEにする
		}

		effect.iX = player.iX;	//エフェクトの位置
		effect.iY = player.iY;	//エフェクトの位置

		effect2.iX = enemy.iX;	//エフェクトの位置
		effect2.iY = enemy.iY;	//エフェクトの位置

			//自機とアイテムとのあたり判定
				//自機の頭とアイテムの下との当たり判定
		for (int i = 0; i < ITEM; i++){
			if (ItemHead(item[i].iLive, player.iY, item[i].iY, item[i].iSize, player.iSize, player.iX, player.iSize2,item[i].iX)){
				item[i].iLive = FALSE;		//アイテムの生存判定をFALSEにする
				player.iJumpLevel2 += 1;	//自機のジャンプレベル2を1プラスする
				sounds.Powerup[1] = 1;
				if (effect.iAniCnt == 0){
					effect.iAniCnt = 1;
					break;
				}
			}
		}
		//自機の右側とアイテムの左側とのあたり判定
		for (int i = 0; i < ITEM; i++){
			if (ItemSide(item[i].iLive, player.iX, player.iSize, item[i].iX, player.iSize2, player.iY, item[i].iY, item[i].iSize)){
				item[i].iLive = FALSE;		//アイテムの生存判定をFALSEにする
				player.iJumpLevel2 += 1;	//自機のジャンプレベル2を1プラスする
				sounds.Powerup[1] = 1;
				if (effect.iAniCnt == 0){
					effect.iAniCnt = 1;
					break;
				}
			}
		}

			//敵とアイテムのあたり判定
		//敵の頭とアイテムの下との当たり判定
		for (int i = 0; i < ITEM; i++){
			if (ItemHead(item[i].iLive, enemy.iY, item[i].iY, item[i].iSize, enemy.iSize, enemy.iX, enemy.iSize2, item[i].iX)){
				item[i].iLive = FALSE;		//アイテムの生存判定をFALSEにする
				enemy.iJumpLevel2 += 1;		//敵のジャンプレベル2を1プラスする
				sounds.Powerup[1] = 1;
				if (effect2.iAniCnt == 0){
					effect2.iAniCnt = 1;
					break;
				}
			}
		}
		//敵の右側とアイテムの左側とのあたり判定
		for (int i = 0; i < ITEM; i++){
			if (ItemSide(item[i].iLive, enemy.iX, enemy.iSize, item[i].iX, enemy.iSize2, enemy.iY, item[i].iY, item[i].iSize)){
				item[i].iLive = FALSE;		//アイテムの生存判定をFALSEにする
				enemy.iJumpLevel2 += 1;	//敵のジャンプレベル2を1プラスする
				sounds.Powerup[1] = 1;
				if (effect2.iAniCnt == 0){
					effect2.iAniCnt = 1;
					break;
				}
			}
		}
		
		//障害物1と自機のあたり判定
		for (int i = 0; i < OBS1; i++){
			if (ObsSide(obs1[i].iLive, player.iX, player.iSize, obs1[i].iX, player.iY, obs1[i].iY, obs1[i].iSize_Y))
				player.iX -= ISPEED;
		}

		//障害物1の上と自機のあたり判定
		for (int i = 0; i < OBS1; i++){
			if (ObsTop(obs1[i].iLive, player.iY, player.iSize, obs1[i].iY,player.iX, obs1[i].iX, obs1[i].iSize_X)){
				player.iY = obs1[i].iY - player.iSize;	//自機の座標を障害物の上に固定する
				player.iobsUpFlag = 1;
			}
		}
		
		//障害物1と敵のあたり判定
		for (int i = 0; i < OBS1; i++){
			if (ObsSide(obs1[i].iLive, enemy.iX, enemy.iSize, obs1[i].iX, enemy.iY, obs1[i].iY, obs1[i].iSize_Y))
				enemy.iX -= ISPEED;
		}

		//障害物1の上と敵のあたり判定
		for (int i = 0; i < OBS1; i++){
			if (ObsTop(obs1[i].iLive, enemy.iY, enemy.iSize, obs1[i].iY, enemy.iX, obs1[i].iX, obs1[i].iSize_X)){
				enemy.iY = obs1[i].iY - enemy.iSize;	//自機の座標を障害物の上に固定する
				enemy.iobsUpFlag = 1;
			}
		}

		//障害物2と自機のあたり判定
		for (int i = 0; i < OBS2; i++){
			if (ObsSide(obs2[i].iLive, player.iX, player.iSize, obs2[i].iX, player.iY, obs2[i].iY, obs2[i].iSize_Y))
				player.iX -= ISPEED;
		}

		//障害物2の上と自機のあたり判定
		for (int i = 0; i < OBS2; i++){
			if (ObsTop(obs2[i].iLive, player.iY, player.iSize, obs2[i].iY, player.iX, obs2[i].iX, obs2[i].iSize_X)){
				player.iY = obs2[i].iY - player.iSize;	//自機の座標を障害物の上に固定する
				player.iobsUpFlag = 1;
				iGameState = GameOver;
				sounds.Damage[1] = 1;
			}
		}

		//障害物2と敵のあたり判定
		for (int i = 0; i < OBS2; i++){
			if (ObsSide(obs2[i].iLive, enemy.iX, enemy.iSize, obs2[i].iX, enemy.iY, obs2[i].iY, obs2[i].iSize_Y))
				enemy.iX -= ISPEED;
		}

		//障害物2の上と敵のあたり判定
		for (int i = 0; i < OBS2; i++){
			if (ObsTop(obs2[i].iLive, enemy.iY, enemy.iSize, obs2[i].iY, enemy.iX, obs2[i].iX, obs2[i].iSize_X)){
				enemy.iY = obs2[i].iY - enemy.iSize;	//自機の座標を障害物の上に固定する
				enemy.iobsUpFlag = 1;
			}
		}

		for (int i = 0; i < OBS1; i++){
		if (CanMove(obs1[i].iLive))		//障害物1の移動
			obs1[i].iX -= ISPEED;
		}

		for (int i = 0; i < OBS2; i++){
			if (CanMove(obs2[i].iLive))	//障害物2の移動
				obs2[i].iX -= ISPEED;
		}

		for (int i = 0; i < ITEM; i++){
			if (CanMove(item[i].iLive))
				item[i].iX -= ISPEED;	//アイテムの移動
		}

		//ゴールの移動
		goal.iX -= ISPEED;			//ゴールのX座標をマイナスする

		//自機とゴールのあたり判定
		if (player.LoseFlag == 0 && player.iX + player.iSize >= goal.iX)	//自機がゴールについたら
			enemy.LoseFlag = 1;
		
		//敵とゴールのあたり判定
		if (enemy.LoseFlag == 0 && enemy.iX + enemy.iSize >= goal.iX)		//敵がゴールについたら
			player.LoseFlag = 1;
		
		if (player.iX <= enemy.iX)		//自機と敵が重なったら
			iGameState = GameOver;		//ゲームオーバー画面にする
		
		//自機が先についたら
		if (enemy.iX + enemy.iSize > goal.iX && enemy.LoseFlag == 1)
			iGameState = GameClear;					//ゲームクリア画面にする
		
		//敵が先にゴールについたら
		if (player.iX + player.iSize > goal.iX && player.LoseFlag == 1)
			iGameState = Lose;						//負け画面にする
		
		for (int i = 0; i < OBS1; i++){
			if (obs1[i].iX + obs1[i].iSize_X < -500)	//障害物1が画面外に出たら
				obs1[i].iLive = FALSE;					//障害物1の生存判定をFALSEにする
		}
		
		for (int i = 0; i < OBS2; i++){
			if (obs2[i].iX + obs2[i].iSize_X < -500)	//障害物2が画面外に出たら
				obs2[i].iLive = FALSE;					//障害物2の生存判定をFALSEにする
		}

		if (player.iX < -10)				//自機が画面外に出たら
			iGameState = GameOver;			//ゲームオーバー画面へ移動する

		if (enemy.iX < 0){					//敵が画面外に出たら
			enemy.iX = player.iX - 300;		//自機の後ろに出現させる	
			enemy.iY = 0;
			if (enemy.iY > 300)				//敵のY座標が300より小さいときは
				enemy.iJumpFlag2 = 0;		//敵のジャンプフラグを0にする
		}

		//背景の更新処理
		for (int i = 0; i < BACK; i++){
			bkGround[i].iX -= ISPEED;		//背景を動かす
			if (bkGround[i].iX <= -1280)	//背景が画面外へ出たら
				bkGround[i].iX = WINDOW_X;		//背景のX座標をWINDOW_Xにする
		}

		//多重背景の更新処理
		for (int i = 0; i < MULTIBACK; i++){
			multiBack[i].iX -= ISPEED;
			if (multiBack[i].iX <= -1280)
				multiBack[i].iX = WINDOW_X;
		}

		//自機の更新処理
		player.iAnicnt = (player.iAnicnt + 1) % (4 * 10);

		//敵の更新処理
		enemy.iAnicnt = (enemy.iAnicnt + 1) % (4 * 10);

		//音楽の更新処理
		StopSoundMem(sounds.Title[0]);

		//メイン音楽の再生
		if (sounds.Main[1] == 1){
			PlaySoundMem(sounds.Main[0], DX_PLAYTYPE_LOOP);
			sounds.Main[1] = 0;
		}

		//ジャンプ音の再生開始
		if (sounds.Jump[1] == 1){
			PlaySoundMem(sounds.Jump[0], DX_PLAYTYPE_BACK);
			sounds.Jump[1] = 0;
		}

		//ダメージ音の再生開始
		if (sounds.Damage[1] == 1){
			PlaySoundMem(sounds.Damage[0], DX_PLAYTYPE_BACK);
			sounds.Damage[1] = 0;
		}

		//アイテム取得時の音の再生開始
		if (sounds.Powerup[1] == 1){
			PlaySoundMem(sounds.Powerup[0], DX_PLAYTYPE_BACK);
			sounds.Powerup[1] = 0;
		}

		//▲▲メイン画面での更新処理▲▲▲▲

		//▼▼メイン画面での描画処理▼▼▼▼

		for (int i = 0; i < BACK; i++)
			DrawGraph(bkGround[i].iX, 0, bkGround[i].iGraph, FALSE);				//背景の描画
		
		DrawGraph(player.iX, player.iY, iPlayerGraph[player.iAnicnt / 10], TRUE);	//自機の描画

		DrawGraph(enemy.iX, enemy.iY, iEnemyGraph[enemy.iAnicnt / 10], TRUE);		//敵の描画

		for (int i = 0; i < MULTIBACK; i++)
			DrawGraph(multiBack[i].iX, 720 - 153, multiBack[i].iGraph, TRUE);		//床の描画

		for (int i = 0; i < ITEM; i++){
			if (item[i].iLive == TRUE)										//アイテムの生存判定がTRUEなら
				DrawGraph(item[i].iX, item[i].iY, item[i].iGraph, TRUE);	//アイテムを描画する
		}

		for (int i = 0; i < OBS1; i++){
			if (obs1[i].iLive == TRUE)										//障害物1の生存判定がTRUEなら
				DrawGraph(obs1[i].iX, obs1[i].iY, obs1[i].iGraph, TRUE);	//障害物1を描画する
		}
		
		for (int i = 0; i < OBS2; i++){
			if (obs2[i].iLive == TRUE)										//障害物2の生存判定がTRUEなら
				DrawGraph(obs2[i].iX, obs2[i].iY, obs2[i].iGraph, TRUE);	//障害物2を描画する
		}

		DrawGraph(goal.iX, goal.iY, goal.iGraph, TRUE);						//ゴールの描画

		DrawGraph(effect.iX, effect.iY, EffectGraph[(effect.iAniCnt - 1) % 4], TRUE);//エフェクトの描画
		DrawGraph(effect2.iX, effect2.iY, EffectGraph2[(effect2.iAniCnt - 1) % 4], TRUE);//エフェクトの描画

		//▲▲メイン画面での描画処理▲▲▲▲
			break;
		default:
			break;
		}
		ScreenFlip();		//裏画面と表画面の入替 
		ClearDrawScreen();	//裏画面の描画を全て消去
	}
	DxLib_End();			// ＤＸライブラリ使用の終了処理
  	return 0;				// ソフトの終了 
}
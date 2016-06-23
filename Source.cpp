#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//ところどころにある3連printf改行は画面をみやすくする調整です
//→時間があればそもそも画面をリフレッシュしてダンジョン描画したいね

//部屋の情報
typedef struct
{
	int syujinko; //主人公の位置 
	int boss1; //ボス1の位置
	int boss2; //ボス2の位置
	int boss3; //ボス3の位置
	int lastBoss; //ラスボスの位置
	int doorInfo[4]; //壁の情報
	int items; //アイテムイベントの位置
	int rest1; //休憩所1の位置
	int rest2; //休憩所2の位置
	int hints; //ヒントの位置
	int syujinkoVisited; //部屋に主人公が訪問したか 

} Room;

Room dungeon[6][6];

typedef struct
{
	int roomX; //主人公のいる部屋のX
	int roomY; //主人公のいる部屋のY
	int hasItems; //回復アイテムを持っているか(最大5つまで持てる？ので制限が必要
	int str1; //威圧ステータス
	int str2; //論理ステータス
	int str3; //丁寧ステータス
	int health; //HPだよ
	int jobCounter; //仕事を終えた数を加算しておくよ

} Hero;

Hero syujinko;


//ボスの初期HP設定
const int boss1StartHP = 3;
const int boss2StartHP = 10;
const int boss3StartHP = 20;
const int lastBossStartHP = 30;

//主人公の初期ステ設定
const int syujinkoStartHP = 0;
const int syujinkoStartStr1 = 1;
const int syujinkoStartStr2 = 1;
const int syujinkoStartStr3 = 1;

//必要な仕事数 これとHero.jobcounterを比較して戦闘条件を満たす
const int needJobs1 = 2;
const int needJobs2 = 5;

// 勝ち負け決めるためのグローーバル変数
const int syujinkoWins = 1;
const int syujinkoLoses = 2;
const int escape = 3;

//壁
const int noDoor = 0;
const int openDoor = 1;

//方向
const int North = 0;
const int East = 1;
const int South = 2;
const int West = 3;

//ボスのhpいれる変数
int boss1HP;
int boss2HP;
int boss3HP;
int lastBossHP;
int fightBoss1HP;
int fightBoss2HP;
int fightBoss3HP;
int fightLastBossHP;

//ランダムダンジョン用
int ranA, ranB, ranC, ranD, ranE, ranF, ranG, ranH, ranI, ranJ, ranK, ranL;
int ranMode = 0;

//やり込み要素
int addItems = 0;
int addRests = 0;


//テキスト表示用
int checkPrintText;

//ボス倒したら発生するフラグ
int boss1Clear;
int boss2Clear;
int boss3Clear;

//休憩所使ったらこれ加算でも減算でもいいからやって次から使えなくする
int useRestRoom;

//テキストいれる変数
char text;

//終了条件
int bossDead;
int syujinkoDead;

//関数のプロトタイプ
void resetDungeon(); //ダンジョン初期化
void resetSyujinko(); //主人公の状態初期化
void resetDungeonRan(); //ダンジョン初期化
void resetSyujinkoRan(); //主人公の状態初期化
void resetBoss(); //ボス初期化
void resetGameEnding(); //終了条件初期化

						//char print_string(char);
void printRule(); //ルール表示
void showDungeon(); //ダンジョンの表示
void showRoomInfo(); //部屋情報の表示
void showStatus(); //ステータス表示
int getUserMove(); //ユーザー入力による移動の処理 moveSyujinkoと関連付け有
void moveSyujinko(int);
void checkBossFight(void); //ボスとの戦闘条件を満たすか
void printText();
void itemDice(); //アイテムイベントのやつ
int randomDice(); //歩くたびに発生するやつ
void tryUseRestRoom(void); //休憩室つかう？
int boss1Fight(int); //ボス1と戦うときのつまってる
int boss2Fight(int); //2
int boss3Fight(int); //3
int lastBossFight(int); //らすぼす
void manyThingsBoss1(); //ボス戦のaaとか
void manyThingsBoss2();
void manyThingsBoss3();
void manyThingsLastBoss();
void takeDamage(); //damage
void yarikomi(); //称号判定


int main(void)
{
	char c;
	int moveDirection; //移動方向


	//初期化
	printRule();
	if (ranMode == 1) {
		resetDungeonRan();
		resetSyujinkoRan();
	}
	else {
		resetDungeon();
		resetSyujinko();
	}
	resetBoss();
	resetGameEnding();


	//ゲームのループ...終了条件のいずれかが満たされるまでループする
	while (1)
	{
		showDungeon();	// ダンジョンの情報を表示
		showRoomInfo();	// 部屋の情報を表示

		checkBossFight(); // ボスがいるなら戦闘
		if (syujinkoDead) // ボスに負けた（敗北）
		{
			break;
		}
		if (bossDead) //ボスを倒した（勝利）
		{
			break;
		}

		showStatus(); //ステータス表示
		tryUseRestRoom(); // 休憩所があれば使うかユーザーに問う	
		printText();

		moveDirection = getUserMove(); // ユーザーから移動方向を入力してもらう
		moveSyujinko(moveDirection); // ユーザが選んだ方向に主人公を移動させる

		randomDice(); //ランダムダイスを発生させる

	}

	// 勝利か敗北のメッセージでゲーム終了する

	if (bossDead)
	{
		if (syujinko.jobCounter > 10)
		{
			printf("\n**********************\n");
			printf("*   有給を取った!!   *\n");
			printf("**********************\n\n");
			printf("おめでとう！晴れて主人公は有給を手に入れることが出来た。\n");
			printf("有給で海外旅行でも行くか、そんなことを考えながら帰路へ着いた・・。\n");
			c = getchar();
			yarikomi();
		}
		else if (syujinko.jobCounter > 5)
		{
			printf("\n**********************\n");
			printf("*   有給を取った!!   *\n");
			printf("**********************\n\n");
			printf("おめでとう！晴れて主人公は有給を手に入れることが出来た。\n");
			printf("有給で国内温泉旅行でも行くか、そんなことを考えながら帰路へ着いた・・。\n");
			c = getchar();
			yarikomi();
		}
		else if (syujinko.jobCounter == 5)
		{
			printf("\n**********************\n");
			printf("*   有給を取った!!   *\n");
			printf("**********************\n\n");
			printf("おめでとう！晴れて主人公は有給を手に入れることが出来た。\n");
			printf("有給で積んであったゲームでも消化するか、そんなことを考えながら帰路へ着いた・・。\n");
			c = getchar();
			yarikomi();
		}
	}
	else if (syujinkoDead)
	{
		printf("\n主人公の疲労度が限界を越えてしまった!\n");
		printf("\n***************************\n");
		printf("*   君は病院へ運ばれた...  *\n");
		printf("***************************\n\n");
		c = getchar();
		printf("君は働きすぎて病院へ運ばれていたようだ。\n");
		printf("倒れるほど働いたが休みは無い。通院が終わればまた社畜の日々が始まる...\n");
		printf("\n\n........\n");
		c = getchar();
		printf("\n...そして君は\n");
		c = getchar();
		printf("\n\n\n\nBAD END    社畜\n");
	}

	printf("\n Thank you for playing!!\n");
	c = getchar();

	return 0;
}

void resetDungeon()
{
	int y;

	// 主人公は最初(1,5)の部屋にいる
	dungeon[1][5].syujinko = 1;
	// 主人公は(1,1)の部屋を訪問している
	dungeon[1][5].syujinkoVisited = 1;
	// boss1は(3,5)の部屋にいる
	dungeon[3][5].boss1 = 1;
	// boss2は(3,2)の部屋にいる
	dungeon[3][2].boss2 = 1;
	// boss3は33にいるよ
	dungeon[3][3].boss3 = 1;
	// lastbossは(5,1)の部屋にいる
	dungeon[5][1].lastBoss = 1;
	// 休憩室を(2,2)と(4,4)に配置する
	dungeon[2][2].rest1 = 2;
	dungeon[4][4].rest2 = 2;
	// ヒントを(2,1)と(5,3)に配置する。
	//dungeon[ 5 ][ 3 ].hints = 1;
	//dungeon[ 2 ][ 1 ].hints = 1;
	// アイテムを(3,1)に配置する
	//dungeon[3][1].items = 1;

	// ドアの情報

	// 1列目のドア情報
	dungeon[1][1].doorInfo[East] = openDoor;
	dungeon[1][1].doorInfo[South] = openDoor;
	dungeon[1][1].doorInfo[North] = noDoor;
	dungeon[1][1].doorInfo[West] = noDoor;

	dungeon[1][3].doorInfo[West] = noDoor;
	dungeon[1][4].doorInfo[West] = noDoor;
	dungeon[1][2].doorInfo[West] = noDoor;

	dungeon[2][1].doorInfo[East] = openDoor;
	dungeon[2][1].doorInfo[South] = openDoor;
	dungeon[2][1].doorInfo[West] = openDoor;
	dungeon[2][1].doorInfo[North] = noDoor;
	dungeon[3][1].doorInfo[East] = openDoor;
	dungeon[3][1].doorInfo[South] = openDoor;
	dungeon[3][1].doorInfo[West] = openDoor;
	dungeon[3][1].doorInfo[North] = noDoor;
	dungeon[4][1].doorInfo[East] = openDoor;
	dungeon[4][1].doorInfo[South] = openDoor;
	dungeon[4][1].doorInfo[West] = openDoor;
	dungeon[4][1].doorInfo[North] = noDoor;

	dungeon[5][1].doorInfo[South] = openDoor;
	dungeon[5][1].doorInfo[West] = openDoor;
	dungeon[5][1].doorInfo[North] = noDoor;
	dungeon[5][1].doorInfo[East] = noDoor;


	// 2列目から4列目までの情報は同じのでforループで初期化
	for (y = 2; y < 5; y++)
	{
		dungeon[1][y].doorInfo[North] = openDoor;
		dungeon[1][y].doorInfo[East] = openDoor;
		dungeon[1][y].doorInfo[South] = openDoor;

		dungeon[2][y].doorInfo[North] = openDoor;
		dungeon[2][y].doorInfo[East] = openDoor;
		dungeon[2][y].doorInfo[South] = openDoor;
		dungeon[2][y].doorInfo[West] = openDoor;
		dungeon[3][y].doorInfo[North] = openDoor;
		dungeon[3][y].doorInfo[East] = openDoor;
		dungeon[3][y].doorInfo[South] = openDoor;
		dungeon[3][y].doorInfo[West] = openDoor;
		dungeon[4][y].doorInfo[North] = openDoor;
		dungeon[4][y].doorInfo[East] = openDoor;
		dungeon[4][y].doorInfo[South] = openDoor;
		dungeon[4][y].doorInfo[West] = openDoor;

		dungeon[5][y].doorInfo[North] = openDoor;
		dungeon[5][y].doorInfo[South] = openDoor;
		dungeon[5][y].doorInfo[West] = openDoor;
	}

	// 5列目のドア情報
	dungeon[1][5].doorInfo[North] = openDoor;
	dungeon[1][5].doorInfo[East] = openDoor;
	dungeon[1][5].doorInfo[West] == noDoor;
	dungeon[1][5].doorInfo[South] == noDoor;

	dungeon[2][5].doorInfo[North] = openDoor;
	dungeon[2][5].doorInfo[East] = openDoor;
	dungeon[2][5].doorInfo[West] = openDoor;
	dungeon[2][5].doorInfo[South] = noDoor;
	dungeon[3][5].doorInfo[North] = openDoor;
	dungeon[3][5].doorInfo[East] = openDoor;
	dungeon[3][5].doorInfo[West] = openDoor;
	dungeon[3][5].doorInfo[South] = noDoor;
	dungeon[4][5].doorInfo[North] = openDoor;
	dungeon[4][5].doorInfo[East] = openDoor;
	dungeon[4][5].doorInfo[West] = openDoor;
	dungeon[4][5].doorInfo[South] = noDoor;

	dungeon[5][5].doorInfo[North] = openDoor;
	dungeon[5][5].doorInfo[West] = openDoor;
	dungeon[5][5].doorInfo[South] = noDoor;
	dungeon[5][5].doorInfo[East] = noDoor;
}


// 主人公の情報を初期化
void resetSyujinko()
{
	// 主人公が最初にいる部屋は(1,5)
	syujinko.roomX = 1;
	syujinko.roomY = 5;
	// 主人公のエナジードリンクストック数をリセット
	syujinko.hasItems = 2;
	// 仕事カウンターをリセット
	syujinko.jobCounter = 0;
	// 主人公のステータスのリセット
	syujinko.str1 = syujinkoStartStr1;
	syujinko.str2 = syujinkoStartStr2;
	syujinko.str3 = syujinkoStartStr3;
	// 最初は最大のＨＰに設定
	syujinko.health = syujinkoStartHP;
}

void resetDungeonRan()
{
	int y;

	srand((unsigned)time(NULL));
	ranA = rand() % 5 + 1;
	ranB = rand() % 5 + 1;
	ranC = rand() % 5 + 1;
	ranD = rand() % 5 + 1;
	ranE = rand() % 5 + 1;
	ranF = rand() % 5 + 1;
	ranG = rand() % 5 + 1;
	ranH = rand() % 5 + 1;
	ranI = rand() % 5 + 1;
	ranJ = rand() % 5 + 1;
	ranK = rand() % 5 + 1;
	ranL = rand() % 5 + 1;


	// 主人公は最初(a,b)の部屋にいる
	dungeon[ranA][ranB].syujinko = 1;
	// 主人公は(a,b)の部屋を訪問している
	dungeon[ranA][ranB].syujinkoVisited = 1;
	// boss1は(,)の部屋にいる
	dungeon[ranE][ranF].boss1 = 1;
	// boss2は(,)の部屋にいる
	dungeon[ranG][ranH].boss2 = 1;
	// boss3は====
	dungeon[ranJ][ranC].boss3 = 1;
	// lastbossは(5,1)の部屋にいる
	dungeon[ranI][ranJ].lastBoss = 1;
	// 休憩室を(2,2)と(4,4)に配置する
	dungeon[ranK][ranL].rest1 = 2;
	dungeon[ranC][ranD].rest2 = 2;
	if (ranK == ranC && ranL == ranD)
	{
		dungeon[2][2].rest1 = 2;
		dungeon[4][4].rest2 = 2;
	}


	// ドアの情報

	// 1列目のドア情報
	dungeon[1][1].doorInfo[East] = openDoor;
	dungeon[1][1].doorInfo[South] = openDoor;
	dungeon[1][1].doorInfo[North] = noDoor;
	dungeon[1][1].doorInfo[West] = noDoor;

	dungeon[1][3].doorInfo[West] = noDoor;
	dungeon[1][4].doorInfo[West] = noDoor;
	dungeon[1][2].doorInfo[West] = noDoor;

	dungeon[2][1].doorInfo[East] = openDoor;
	dungeon[2][1].doorInfo[South] = openDoor;
	dungeon[2][1].doorInfo[West] = openDoor;
	dungeon[2][1].doorInfo[North] = noDoor;
	dungeon[3][1].doorInfo[East] = openDoor;
	dungeon[3][1].doorInfo[South] = openDoor;
	dungeon[3][1].doorInfo[West] = openDoor;
	dungeon[3][1].doorInfo[North] = noDoor;
	dungeon[4][1].doorInfo[East] = openDoor;
	dungeon[4][1].doorInfo[South] = openDoor;
	dungeon[4][1].doorInfo[West] = openDoor;
	dungeon[4][1].doorInfo[North] = noDoor;

	dungeon[5][1].doorInfo[South] = openDoor;
	dungeon[5][1].doorInfo[West] = openDoor;
	dungeon[5][1].doorInfo[North] = noDoor;
	dungeon[5][1].doorInfo[East] = noDoor;


	// 2列目から4列目までの情報は同じのでforループで初期化
	for (y = 2; y < 5; y++)
	{
		dungeon[1][y].doorInfo[North] = openDoor;
		dungeon[1][y].doorInfo[East] = openDoor;
		dungeon[1][y].doorInfo[South] = openDoor;

		dungeon[2][y].doorInfo[North] = openDoor;
		dungeon[2][y].doorInfo[East] = openDoor;
		dungeon[2][y].doorInfo[South] = openDoor;
		dungeon[2][y].doorInfo[West] = openDoor;
		dungeon[3][y].doorInfo[North] = openDoor;
		dungeon[3][y].doorInfo[East] = openDoor;
		dungeon[3][y].doorInfo[South] = openDoor;
		dungeon[3][y].doorInfo[West] = openDoor;
		dungeon[4][y].doorInfo[North] = openDoor;
		dungeon[4][y].doorInfo[East] = openDoor;
		dungeon[4][y].doorInfo[South] = openDoor;
		dungeon[4][y].doorInfo[West] = openDoor;

		dungeon[5][y].doorInfo[North] = openDoor;
		dungeon[5][y].doorInfo[South] = openDoor;
		dungeon[5][y].doorInfo[West] = openDoor;
	}

	// 5列目のドア情報
	dungeon[1][5].doorInfo[North] = openDoor;
	dungeon[1][5].doorInfo[East] = openDoor;
	dungeon[1][5].doorInfo[West] == noDoor;
	dungeon[1][5].doorInfo[South] == noDoor;

	dungeon[2][5].doorInfo[North] = openDoor;
	dungeon[2][5].doorInfo[East] = openDoor;
	dungeon[2][5].doorInfo[West] = openDoor;
	dungeon[2][5].doorInfo[South] = noDoor;
	dungeon[3][5].doorInfo[North] = openDoor;
	dungeon[3][5].doorInfo[East] = openDoor;
	dungeon[3][5].doorInfo[West] = openDoor;
	dungeon[3][5].doorInfo[South] = noDoor;
	dungeon[4][5].doorInfo[North] = openDoor;
	dungeon[4][5].doorInfo[East] = openDoor;
	dungeon[4][5].doorInfo[West] = openDoor;
	dungeon[4][5].doorInfo[South] = noDoor;

	dungeon[5][5].doorInfo[North] = openDoor;
	dungeon[5][5].doorInfo[West] = openDoor;
	dungeon[5][5].doorInfo[South] = noDoor;
	dungeon[5][5].doorInfo[East] = noDoor;
}


// 主人公の情報を初期化
void resetSyujinkoRan()
{
	int a = ranA;
	int b = ranB;
	// 主人公が最初にいる部屋は(1,5)
	syujinko.roomX = a;
	syujinko.roomY = b;
	// 主人公のエナジードリンクストック数をリセット
	syujinko.hasItems = 2;
	// 仕事カウンターをリセット
	syujinko.jobCounter = 0;
	// 主人公のステータスのリセット
	syujinko.str1 = syujinkoStartStr1;
	syujinko.str2 = syujinkoStartStr2;
	syujinko.str3 = syujinkoStartStr3;
	// 最初は最大のＨＰに設定
	syujinko.health = syujinkoStartHP;
}


// bossの情報を初期化
void resetBoss()
{
	boss1HP = boss1StartHP;	//boss1のHP
	boss2HP = boss2StartHP;	// boss2のHP
	boss3HP = boss3StartHP;
	lastBossHP = lastBossStartHP;// lastbossのHP
	fightBoss1HP = 3;
	fightBoss2HP = 10;
	fightBoss3HP = 20;
	fightLastBossHP = 30;
	boss1Clear = 0;
	boss2Clear = 0;
}

//ゲームの終了条件初期化
void resetGameEnding()
{
	bossDead = 0;
	syujinkoDead = 0;
}

void printRule()
{
	char c, userSelection;

	printf(" ******************************************\n");
	printf(" *                                        *\n");
	printf(" *       SHA☆ CHI☆ KUへようこそ!        *\n");
	printf(" *                                        *\n");
	printf(" *      SHA☆ CHI☆ KUはシンプルな        *\n");
	printf(" *      テキストベースのRPGです           *\n");
	printf(" *    Design: CS応用実験テーマA チーム９  *\n");
	printf(" *                                        *\n");
	printf(" ******************************************\n");

	printf("\n");
	printf("1) ルールを表示します\n");
	printf("2) ゲームスタート!\n");
	printf("3) 配置ランダムモード\n");
	printf("1か2か3を入力してください: ");

	// ユーザの選択を獲得
	c = getchar();
	userSelection = c;
	while ((c = getchar()) != '\n')
		;
	if (userSelection == '1')
	{
		printf("\nSHA☆ CHI☆ KUのルール:\n");
		printf("==========================\n");
		printf("1) 入社2年目の社畜である主人公は、あくどい上司に立ち向かい、有休を勝ち取るため、戦います。\n");
		c = getchar();
		printf("2) 主人公の初期位置は(1,5)となっています(左下)。\n");
		c = getchar();
		printf("3) 進める方向が表示されるので、対象のキーを入力するとその方向に1マス進みます。\n");
		c = getchar();
		printf("4) 道中止まったマスによってイベントが発生することがあります。\n");
		c = getchar();
		printf("5) 体力は、疲労度という概念のため、0からスタートとなり、主人公の疲労度が100に到達すると、ゲームオーバーとなります。\n");
		c = getchar();
		printf("6) 仕事について\n  道中、「仕事」というものが発生することがあります。一部のボスと戦うためには、この「仕事」を一定数こなす必要があります。また、「仕事 」が発生すると、主人公の疲労度が10増加するかわりに、好きなステータスを1だけ上昇させることができます。\n");
		c = getchar();
		printf("7) ステータスについて\n  ボスとの戦闘に関する3つの攻撃力のことを指します。これはじゃんけんの手に則っており、「威圧（グー ）」、「論理（チョキ ）」、「丁寧（パー ）」の3つの手があります。初期ステータスは全て「1」となっており、仕事をしたりアイテムを使うことによって最大「6(+5)」まで上げることができます。\n");
		c = getchar();
		printf("8) 回復アイテムについて\n  回復アイテムを使うと主人公の疲労度を10回復することができ、最大で5つまでストックすることができます。なお、回復アイテムの使用は任意となっています。(ベータ版ではストック機能は実装していないので、制限なく持つことができます。)\n");
		c = getchar();
		printf("9) ボスは道中2体登場し、それら2体のボスを順番に倒し、さらに一定数仕事をこなすことによって、ラスボスに挑むことができます。ラスボスを倒すと、あなたの勝利となります。また、ボスによって出しやすい手があるので、道中のヒントを頼りにしながらうまく戦いを進めましょう。\n");
		c = getchar();
		printf("10) ボス戦では任意で逃げるコマンドを入力することで戦闘から逃げることができます。戦闘中に回復アイテムを使うことはできないため、疲労度が100に近づいてきたときなどは、一度戦闘から逃げて回復アイテムを使用したり、道中どこかのマスにある休憩室イベントを利用して疲労度を回復しましょう。但し、同様にボスの体力も復活してしまいます。 \n");

		printf("\nゲームを開始します。何かキーを押してください\n");
		c = getchar();
	}
	else if(userSelection == '3'){
		ranMode = 1;
		printf("\nオブジェクトの配置がランダムです。通常モードより少し死にやすいかも・・・？");
		printf("\nゲームを開始します。何かキーを押してください\n");
		c = getchar();
	}

	//～開始時セリフ～

	printf("\n主人公「今日も残業で辛いなぁ… \n");
	printf("そろそろ休みが欲しいしとりあえず課長のところに相談に行こう。」 \n");
	printf("\nあなたの力でぜひとも有休を獲得しましょう!\n");
	c = getchar();
	printf("\n");
	printf("\n");
	printf("\n");
}

void showDungeon()
{
	int x, y;

	// 部屋を一つずつ表示する
	for (y = 1; y <= 5; y++)
	{

		// 1列目：北ドア
		for (x = 1; x <= 5; x++)
		{
			if (dungeon[x][y].syujinkoVisited)
			{
				if (dungeon[x][y].doorInfo[North] == noDoor)
				{
					printf("*******");
				}
				else if (dungeon[x][y].doorInfo[North] == openDoor)
				{
					printf("***D***");
				}
			}
			else
			{
				printf("       ");
			}
		}
		printf("\n");

		// 2列目：見やすくするための空きスペース
		for (x = 1; x <= 5; x++)
		{
			if (dungeon[x][y].syujinkoVisited)
			{
				printf("*     *");
			}
			else
			{
				printf("       ");
			}
		}
		printf("\n");

		// 3列目：西ドア、主人公とボス、東ドア
		for (x = 1; x <= 5; x++)
		{
			if (dungeon[x][y].syujinkoVisited)
			{
				if (dungeon[x][y].doorInfo[West] == noDoor)
				{
					printf("* ");
				}
				else if (dungeon[x][y].doorInfo[West] == openDoor)
				{
					printf("D ");
				}
				if (dungeon[x][y].syujinko)
				{
					printf("P ");
				}
				else
				{
					printf("  ");
				}
				if (dungeon[x][y].boss1)
				{
					printf("B1");
				}
				else if (dungeon[x][y].boss2)
				{
					printf("B2");
				}
				else if (dungeon[x][y].boss3)
				{
					printf("B3");
				}
				else if (dungeon[x][y].lastBoss)
				{
					printf("LB");
				}
				else
				{
					printf("  ");
				}
				if (dungeon[x][y].doorInfo[East] == noDoor)
				{
					printf("*");
				}
				else if (dungeon[x][y].doorInfo[East] == openDoor)
				{
					printf("D");
				}
			}
			else {
				printf("       ");
			}
		}
		printf("\n");

		// 4列目: ヒントと休憩室とアイテムイベント
		for (x = 1; x <= 5; x++)
		{
			if (dungeon[x][y].syujinkoVisited)
			{
				if (dungeon[x][y].rest1)
				{
					printf("* R ");
				}
				else
				{
					printf("*   ");
				}
				if (dungeon[x][y].rest2)
				{
					printf("R *");
				}
				else
				{
					printf("  *");
				}
				/*	      if( dungeon[ x ][ y ].hints )
				{
				printf( "H" );
				}
				if( dungeon[ x ][ y ].items )
				{
				printf( "I *" );
				}*/
			}
			else
			{
				printf("       ");
			}
		}
		printf("\n");


		// 5列目：南ドア
		for (x = 1; x <= 5; x++)
		{
			if (dungeon[x][y].syujinkoVisited)
			{
				if (dungeon[x][y].doorInfo[South] == noDoor)
				{
					printf("*******");
				}
				else if (dungeon[x][y].doorInfo[South] == openDoor)
				{
					printf("***D***");
				}
			}
			else
			{
				printf("       ");
			}
		}
		printf("\n");

	}

	// 記号の説明
	printf("P = プレイヤー, B1 = 課長, B2 = 部長, LB = 社長, R = 休憩所 D = ドア, * = 壁 \n");

}


// 部屋の情報を表示する
void showRoomInfo()
{
	int x = syujinko.roomX;
	int y = syujinko.roomY;

	// この部屋に上司か社長がいることを表示
	if (dungeon[x][y].boss1 || dungeon[x][y].boss2)
	{
		printf("この部屋には上司(敵)がいます。\n");
	}
	if (dungeon[x][y].lastBoss)
	{
		printf("この部屋には社長がいます。\n");
	}
	// この部屋に休憩所1があることを表示
	if (dungeon[x][y].rest1)
	{
		printf("ここは休憩所です。疲労度を下げることができます。\n");
	}
	//休憩所2
	if (dungeon[x][y].rest2)
	{
		printf("ここは休憩所です。疲労度を下げることができます。\n");
	}
	// この部屋がアイテム取得マスであることを表示
	if (dungeon[x][y].items)
	{
		printf("あなたはアイテムを発見しました!\n");
	}
	// この部屋にヒントがあることを表示
	if (dungeon[x][y].hints)
	{
		printf("あなたはヒントを見つけました!\n");
	}
	// この部屋のドア情報を表示
	if (dungeon[x][y].doorInfo[North] == openDoor)
	{
		printf(" この部屋は北側に進めます。 ");
	}
	else if (dungeon[x][y].doorInfo[North] == noDoor)
	{
		printf(" この部屋は北側に進めません。 ");
	}
	if (dungeon[x][y].doorInfo[East] == openDoor)
	{
		printf(" この部屋は東側に進めます。\n");
	}
	else if (dungeon[x][y].doorInfo[East] == noDoor)
	{
		printf(" この部屋は東側に進めません。\n");
	}
	if (dungeon[x][y].doorInfo[South] == openDoor)
	{
		printf(" この部屋は南側に進めます。 ");
	}
	else if (dungeon[x][y].doorInfo[South] == noDoor)
	{
		printf(" この部屋は南側に進めません。 ");
	}
	if (dungeon[x][y].doorInfo[West] == openDoor)
	{
		printf(" この部屋は西側に進めます。\n");
	}
	else if (dungeon[x][y].doorInfo[West] == noDoor)
	{
		printf(" この部屋は西側に進めません。\n");
	}
}

void showStatus()
{
	printf("--------------------------------------------------------------------------------");
	printf("                     現在の主人公のステータス 疲労度:%d          \n", syujinko.health);
	printf("                 威圧(グー):%d, 論理(チョキ):%d, 丁寧(パー):%d \n", syujinko.str1, syujinko.str2, syujinko.str3);
	printf("                    回復アイテム所持数:%d, 仕事カウンタ:%d    \n", syujinko.hasItems, syujinko.jobCounter);
	printf("--------------------------------------------------------------------------------\n");
}

// ユーザーから移動方向を入力してもらう
int getUserMove()//井上拓実 修正:江上
{
	int direction = -1;
	int legalDirection = 0;
	char c;
	//char po[2];
	int x = syujinko.roomX;
	int y = syujinko.roomY;

	// 正しい方向が入力されてないまで繰り返す
	while (!legalDirection)
	{
		// ユーザーから方向を入力
		printf("\n w,a,s,dで移動します。xで回復アイテム使用(所持している場合のみ): ");
		c = getchar();
		// 改行文字をバッファーから削除
		scanf_s("%*c");

		// 入力は正しいかどうかを確認
		switch (c)
		{
		case 'X':
		case 'x':
			if (syujinko.hasItems > 0) {
				if (syujinko.health > 0) {

					syujinko.health = syujinko.health - 10;
					syujinko.hasItems = syujinko.hasItems - 1;
					printf(" ->回復アイテムを使用しました。疲労度が10減少します。\n");
					showDungeon();
					showStatus();
					addItems++;
					break;
				}
				else {
					printf("疲労度はこれ以上回復しません。\n");
					c = getchar();
					showDungeon();
					showStatus();
					break;
				}
			}
			else {
				printf(" ->アイテムを持っていません。\n ");
				break;
			}

		case 'W':
		case 'w':
			// 北方向にドアがあれば方向決定
			if (dungeon[x][y].doorInfo[North] == openDoor)
			{
				direction = North;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[North] == noDoor)
			{
				printf("こっちは行き止まりです。\n");
			}
			//ボス2がいる部屋
			else if (dungeon[x][y].doorInfo[North] == dungeon[x][y].boss2)
				//boss2よりneedJobs1かな？
			{
				if (syujinko.jobCounter == 2)
				{
					direction = North;
					legalDirection = 1;
				}
				else
				{
					printf("仕事を2回こなすことで開きます。\n");
				}
			}
			//ラスボスがいる部屋
			else if (dungeon[x][y].doorInfo[North] == dungeon[x][y].lastBoss)
				//lastbossよりneedJobs2かな？
			{
				if (syujinko.jobCounter == 5)
				{
					direction = North;
					legalDirection = 1;
				}
				else
				{
					printf("仕事を5回こなすことで開きます。\n");
				}
			}
			/*	  else
			{
			printf( "こっちは行き止まりです。\n" );
			} */
			break;

		case 'D':
		case 'd':
			// 東方向にドアがあれば方向決定
			if (dungeon[x][y].doorInfo[East] == openDoor)
			{
				direction = East;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[East] == noDoor)
			{
				printf("こっちは行き止まりです。\n");
			}
			//ボス2がいる部屋。必要仕事数２
			else if (dungeon[x][y].doorInfo[East] == dungeon[x][y].boss2)
				//boss2よりneedJobs1かな？
			{
				if (syujinko.jobCounter == 2)
				{
					direction = East;
					legalDirection = 1;
				}
				else
				{
					printf("仕事を2回こなすことで開きます。\n");
				}
			}
			//ラスボスがいる部屋。必要仕事数５
			else if (dungeon[x][y].doorInfo[East] == dungeon[x][y].lastBoss)
				//lastbossよりneedJobs2かな？
			{
				if (syujinko.jobCounter == 5)
				{
					direction = East;
					legalDirection = 1;
				}
				else
				{
					printf("仕事を5回こなすことで開きます。\n");
				}
			}
			/*	  else
			{
			printf( "こっちは行き止まりです。\n" );
			}*/
			break;

		case 'S':
		case 's':
			// 南方向にドアがあれば方向決定
			if (dungeon[x][y].doorInfo[South] == openDoor)
			{
				direction = South;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[South] == noDoor)
			{
				printf("こっちは行き止まりです。\n");
			}
			//ボス2がいる部屋。必要仕事数２
			else if (dungeon[x][y].doorInfo[South] == dungeon[x][y].boss2)
				//boss2よりneedJobs1かな？
			{
				if (syujinko.jobCounter == 2)
				{
					direction = South;
					legalDirection = 1;
				}
				else
				{
					printf("仕事を2回こなすことで開きます。\n");
				}
			}
			//ラスボスがいる部屋。必要仕事数５
			else if (dungeon[x][y].doorInfo[South] == dungeon[x][y].lastBoss)
				//lastbossよりneedJobs2かな？
			{
				if (syujinko.jobCounter == 5)
				{
					direction = South;
					legalDirection = 1;
				}
				else
				{
					printf(" 仕事を5回こなすことで開きます。\n ");
				}
			}
			/*	  else
			{
			printf( " こっちは行き止まりです。\n " );
			}*/
			break;

		case 'A':
		case 'a':
			// 西方向にドアがあれば方向決定
			if (dungeon[x][y].doorInfo[West] == openDoor)
			{
				direction = West;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[West] == noDoor)
			{
				printf("こっちは行き止まりです。\n");
			}
			//ボス2がいる部屋。必要仕事数２
			else if (dungeon[x][y].doorInfo[West] == dungeon[x][y].boss2)
				//boss2よりneedJobs1かな？
			{
				if (syujinko.jobCounter == 2)
				{
					direction = West;
					legalDirection = 1;
				}
				else
				{
					printf(" 仕事を2回こなすことで開きます。\n ");
				}
			}
			//ラスボスがいる部屋。必要仕事数５
			else if (dungeon[x][y].doorInfo[West] == dungeon[x][y].lastBoss)
				//lastbossよりneedJobs2かな？
			{
				if (syujinko.jobCounter == 5)
				{
					direction = West;
					legalDirection = 1;
				}
				else
				{
					printf(" 仕事を5回こなすことで開きます。\n ");
				}
			}
			/*	  else
			{
			printf(" こっちは行き止まりです。\n ");
			}*/
			break;
		default:
			// 不正入力
			printf(" 進む方向を入力してください。\n ");
			c = getchar();
			break;
		}
	}
	return direction;
}

// ユーザが選んだ方向に主人公を移動させる
void moveSyujinko(int direction)//井上拓実 修正:江上
{
	int currentX = syujinko.roomX;
	int currentY = syujinko.roomY;
	int newX, newY;

	// 主人公を元の部屋から消す
	dungeon[currentX][currentY].syujinko = 0;

	//　新しい部屋のX軸とY軸を方向から獲得
	if (direction == North)
	{
		newX = currentX;
		newY = currentY - 1;
	}
	else if (direction == East)
	{
		newX = currentX + 1;
		newY = currentY;
	}
	else if (direction == South)
	{
		newX = currentX;
		newY = currentY + 1;
	}
	else if (direction == West)
	{
		newX = currentX - 1;
		newY = currentY;
	}

	// 新しい場所の情報を追加
	syujinko.roomX = newX;
	syujinko.roomY = newY;
	dungeon[newX][newY].syujinko = 1;
	dungeon[newX][newY].syujinkoVisited = 1;

}

void printText() {

	if (checkPrintText == 1) {
		printf("\nxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("x特に何もなかった....x\n");
		printf("xxxxxxxxxxxxxxxxxxxxxx\n");
		checkPrintText = 0;
	}

}

void itemDice() //田代
{
	char c;
	int random;
	srand((unsigned)time(NULL));
	random = rand() % 6 + 1;

	switch (random) {
	case 1:
	case 2:
	case 3: {
		printf(" *回復アイテムを手に入れた。*\n ");
		syujinko.hasItems += 1;
		c = getchar();
		printf("\n");
		printf("\n");
		printf("\n");
		break;
	}
	case 4: {
		if (syujinko.str1 >= 6)
		{
			printf("->これ以上ステータスは上がりません。\n");
			syujinko.str1 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *威圧教本を手に入れた。威圧ステータスが上昇します。*\n ");
			syujinko.str1 += 1;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		break;
	}
	case 5: {
		if (syujinko.str2 >= 6)
		{
			printf("これ以上ステータスは上がりません。\n");
			syujinko.str2 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *論理教本を手に入れた。論理ステータスが上昇します。*\n ");
			syujinko.str2 += 1;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		break;
	}
	case 6: {
		if (syujinko.str3 >= 6)
		{
			printf("これ以上ステータスは上がりません。\n");
			syujinko.str3 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *丁寧教本を手に入れた。丁寧ステータスが上昇します。*\n ");
			syujinko.str3 += 1;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		break;
	}
	}
	return;
}

int randomDice(void)//田代 
{
	int random = 0;
	int x;
	char y[4];

	random = rand() % 7 + 1;

	switch (random) {
	case 1:
	case 2: {
		printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("x 仕事発生。疲労度が10増加し、仕事カウンタを1上昇させます。x\n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		syujinko.health += 10;
		syujinko.jobCounter += 1;

		if (syujinko.health >= 100) {
			syujinkoDead = 1;
			break;
		}
		if (syujinko.str1 == 6 && syujinko.str2 == 6 && syujinko.str3 == 6)
		{
			printf("->全ステータス最大値です。\n");
			break;

		}

		printf("->任意のステータスを上昇させることが出来ます。該当の数字キーを押して決定してください。\n");
		printf("1.威圧, 2.論理, 3.丁寧 : ");

		fgets(y, 4, stdin);
		x = atoi(y);

		switch (x) {

		case 1:
			if (syujinko.str1 == 6)
			{
				printf("->これ以上威圧ステータスは上がりません。\n");
				syujinko.str1 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str2 == 6 && syujinko.str3 < 6)
				{
					printf("->丁寧ステータスを上昇させました。\n");
					syujinko.str2 == 6;
					syujinko.str3 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");

				}
				else if (syujinko.str3 == 6 && syujinko.str2 < 6)
				{
					printf("->倫理ステータスを上昇させました。\n");
					syujinko.str3 == 6;
					syujinko.str2 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");

				}
				else if (syujinko.str2 < 6 || syujinko.str3 < 6)
				{
					printf("2.論理, 3.丁寧：");
					scanf_s("%d", &x);
					switch (x)
					{
					case 2:
						printf("->論理ステータスを上昇させました。\n");
						syujinko.str2 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 3:
						printf("->丁寧ステータスを上昇させました。\n");
						syujinko.str3 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					}
				}
				else if (syujinko.str2 == 6 && syujinko.str3 == 6)
				{
					printf("->これ以上ステータスは上がりません。\n");
					syujinko.str2 == 6;
					syujinko.str3 == 6;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
			}
			else
			{
				printf("->威圧ステータスを上昇させました。\n");
				syujinko.str1 += 1;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
			}
			break;
		case 2:
			if (syujinko.str2 == 6)
			{
				printf("->これ以上論理ステータスは上がりません。\n");
				syujinko.str2 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str1 == 6 && syujinko.str3 < 6)
				{
					printf("->丁寧ステータスを上昇させました。\n");
					syujinko.str1 == 6;
					syujinko.str3 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str3 == 6 && syujinko.str1 < 6)
				{
					printf("->威圧ステータスを上昇させました。\n");
					syujinko.str3 == 6;
					syujinko.str1 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str1 < 6 || syujinko.str3 < 6)
				{
					printf("1.威圧, 3.丁寧：");
					scanf_s("%d", &x);
					switch (x)
					{
					case 1:
						printf("->威圧ステータスを上昇させました。\n");
						syujinko.str1 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 3:
						printf("->丁寧ステータスを上昇させました。\n");
						syujinko.str3 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					}
				}
				else if (syujinko.str1 == 6 && syujinko.str3 == 6)
				{
					printf("->これ以上ステータスは上がりません。\n");
					syujinko.str1 == 6;
					syujinko.str3 == 6;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
			}
			else
			{
				printf("->論理ステータスを上昇させました。\n");
				syujinko.str2 += 1;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
			}
			break;

		case 3:
			if (syujinko.str3 == 6)
			{
				printf("->これ以上丁寧ステータスは上がりません。\n");
				syujinko.str3 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str1 == 6 && syujinko.str2 < 6)
				{
					printf("->論理ステータスを上昇させました。\n");
					syujinko.str1 == 6;
					syujinko.str2 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str2 == 6 && syujinko.str1 < 6)
				{
					printf("->威圧ステータスを上昇させました。\n");
					syujinko.str2 == 6;
					syujinko.str1 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str1 < 6 || syujinko.str2 < 6)
				{
					printf("1.威圧, 2.丁寧：");
					scanf_s("%d", &x);
					switch (x)
					{
					case 1:
						printf("->威圧ステータスを上昇させました。\n");
						syujinko.str1 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 2:
						printf("->論理ステータスを上昇させました。\n");
						syujinko.str2 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					}
				}
				else if (syujinko.str1 == 6 && syujinko.str2 == 6)
				{
					printf("->これ以上ステータスは上がりません。\n");
					syujinko.str1 == 6;
					syujinko.str2 == 6;
					printf("\n");
					printf("\n");
					printf("\n");
					x = getchar();
				}
			}
			else
			{
				printf("->丁寧ステータスを上昇させました。\n");
				syujinko.str3 += 1;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
			}
			break;

		default:
			printf("->無効な処理です。\n");
			syujinko.health -= 10;
			syujinko.jobCounter -= 1;
			x = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		break;
	}
	case 3:
	case 4:
		printf(" *アイテムイベント発生！アイテムの抽選をします。*\n ");
		x = getchar();
		itemDice();
		//x = getchar();
		break;

	case 5:
	case 6:
	case 7:
		checkPrintText = 1;
		break;
	}


	return 0;
}

void tryUseRestRoom(void) //江上　修正:田代
{
	int x = syujinko.roomX;
	int y = syujinko.roomY;
	int choose1 = 0;
	int choose2 = 0;
	char z[3];
	char v[3];

	//休憩所1

	if (dungeon[x][y].rest1) {
		printf("\n **休憩室を発見した!**\n");
		printf(" 休憩室を利用しますか ? (疲労度30減少)各休憩室毎に2回まで使用できます。\n ");
		printf(" 利用しなかった場合には、次にマスに入った際にもう一度使うことができます。\n");
		printf(" \n利用する場合には1を、しない場合には2を入力してください。\n ");
		fgets(z, 3, stdin);
		choose1 = atoi(z);
		if (choose1 == 1) {
			if (syujinko.health < 30) {
				syujinko.health = 0;
				dungeon[x][y].rest1 -= 1;
				printf(" ->休憩室を利用しました。疲労度が減少します。\n ");
				printf(" 現在の疲労度: %d", syujinko.health);
				addRests++;

				choose1 = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");

			}
			else {
				syujinko.health = syujinko.health - 30;
				dungeon[x][y].rest1 -= 1;
				printf(" ->休憩室を利用しました。疲労度が減少します。\n ");
				printf(" 現在の疲労度: %d", syujinko.health);
				addRests++;

				choose1 = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");

			}

		}
		else if (choose1 == 2) {
			printf(" ->休憩室を利用しませんでした。\n ");
			choose1 = getchar();
			printf("\n");



		}
		else {
			printf(" ->無効な入力です。\n\n ");
			choose1 = getchar();
			printf("\n");
		}
		showDungeon();
		showStatus();

	}


	if (dungeon[x][y].rest2) {
		printf("\n **休憩室を発見した!**\n");
		printf(" 休憩室を利用しますか ? (疲労度30減少)各休憩室毎に2回まで使用できます。\n ");
		printf(" 利用しなかった場合には、次にマスに入った際にもう一度使うことができます。\n");
		printf(" \n利用する場合には1を、しない場合には2を入力してください。\n ");
		fgets(v, 3, stdin);
		choose2 = atoi(v);
		if (choose2 == 1) {
			if (syujinko.health < 30) {
				syujinko.health = 0;
				dungeon[x][y].rest2 -= 1;
				printf(" ->休憩室を利用しました。疲労度が減少します。\n ");
				printf(" 現在の疲労度: %d", syujinko.health);
				addRests++;

				choose2 = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
	

			}
			else {
				syujinko.health = syujinko.health - 30;
				dungeon[x][y].rest2 -= 1;
				printf(" ->休憩室を利用しました。疲労度が減少します。\n ");
				printf(" 現在の疲労度: %d", syujinko.health);
				addRests++;

				choose2 = getchar();
				printf("\n");		
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");


			}

		}
		else if (choose2 == 2) {
			printf(" ->休憩室を利用しませんでした。\n ");
			choose2 = getchar();
			printf("\n");

		}
		else {
			printf(" ->無効な入力です。とりあえず利用しません。\n\n ");
			choose2 = getchar();
			printf("\n");
		}
		showDungeon();
		showRoomInfo();
		showStatus();
	}
}

void checkBossFight(void) //全体:田代, 台詞:佐藤
{
	/////////////////1番めのぼす/////////////////
	int x = syujinko.roomX;
	int y = syujinko.roomY;
	int result;
	char c;

	if (dungeon[x][y].boss1)
	{
		//ボス台詞募集 AAとかいれてもいいかもしれない
		//戦闘前
		printf("\n[課長のデスク]\n");
		printf("主人公「課長!ここのところずっと残業続きなので有給休暇が取りたいです!!」\n");
		c = getchar();

		printf("\n ￣￣￣￣￣￣￣￣￣￣彡⌒ ミ ￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("          ―――― と(´･ω･`) 　　　　　　￣￣￣￣\n");
		printf("￣￣￣　　――――‐ / ⊂_ノ 　　　―――＿＿　　　　――\n");
		printf("    ――　　　　　. / ／⌒ｿ \n");
		printf("       ￣￣　　　  -'´ 　　　　　￣￣￣ \n");
		printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
		printf(" \n課長「私の一存では決められませんね。\n");
		printf(" 私との戦いに勝利すれば部長に推薦してあげますよ!」\n");
		printf("\nキーを押して戦闘に入ります!\n");
		c = getchar();
		result = boss1Fight(boss1HP);

		if (result == syujinkoWins)
		{
			// 死に台詞
			printf("\n課長「君の話はよく分かりました。\n");
			printf("部長は部長室に居るのでそこで彼を説得するといいでしょう。\n");
			printf("ただ部長は実績のない社員を見下しているので\n");
			printf("いくつか仕事をこなしてからの方がいいかもしれません。」\n");
			printf("\n ￣￣￣￣￣￣￣￣￣￣彡⌒ ミ ￣￣￣￣￣￣￣￣￣￣￣\n");
			printf("          ―――― と(´XωX`) 　　　　　　￣￣￣￣\n");
			printf("￣￣￣　　――――‐ / ⊂_ノ 　　　―――＿＿　　　　――\n");
			printf("    ――　　　　　. / ／⌒ｿ \n");
			printf("       ￣￣　　　  -'´ 　　　　　￣￣￣ \n");
			printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
			printf("\n主人公「こんなところに紙きれが落ちてる…」\n");
			printf("****************************************\n");
			printf("*                ヒント                *\n");
			printf("*          部長はかなり気が短い        *\n");
			printf("*         いつも怒っているから         *\n");
			printf("*           皆に嫌われている       　  *\n");
			printf("*                                      *\n");
			printf("****************************************\n");
			boss1Clear = 1;
			dungeon[x][y].boss1 = 0;

			printf("キーを押して探索画面へ戻ります。\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\nボス戦から撤退しました...\n");
		}
		else {
			// ボスに負けたの敗北条件達成
			syujinkoDead = 1;
		}
	}

	//****************ここからboss2*****************

	if (dungeon[x][y].boss2 && boss1Clear == 1)
	{
		printf("\n[部長の部屋]\n");
		printf("主人公「課長から話を聞いているかもしれませんが有給休暇の話をしに来ました。」\n");
		c = getchar();


		printf("				 ）ｿ）\n");
		printf("				ｯλ　ﾉ（., ﾉ)\n");
		printf("		         （ﾞ - ..::.::..（\n");
		printf("			    (ソ..彡⌒ミ.）ｿ)\n");
		printf("			）.::'; (´･ω･`) 　 ( \n");
		printf("		  ｿ　....::; ';'（つ ⊂）::; ';'`～､. \n");
		printf("			(:; ';'  |_∧_| ::; ';' 　ヽ）\n");
		printf("     '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");

		printf(" \n部長「お前が社会人のくせに休みを欲しがってる奴か!!\n");
		printf(" 二度とそんな考えを起こさないように俺が社会の厳しさを教えてやる!!」\n");
		printf("キーを押して戦闘に入ります！\n");
		c = getchar();
		result = boss2Fight(boss2HP);

		if (result == syujinkoWins)
		{
			// 死に台詞
			printf("主人公「部長とはまともな話にならなかった…\n");
			printf("次は副社長と話をしないとな。」\n");

			printf("\n				 彡⌒ミ\n");
			printf("				 (´XωX｀)\n");
			printf("				| ￣￣￣ |\n");
			printf("				| 二二二 |\n");
			printf("				| ＿＿＿ |\n");
			printf("				从从从从\n");

			printf("\n主人公「ん、また紙切れが落ちてるな。」\n");

			printf("*******************************************\n");
			printf("*                                         *\n");
			printf("*          副社長は部長と実の兄弟         *\n");
			printf("*             兄弟ということは            *\n");
			printf("*           そういうことかも...           *\n");
			printf("*                                         *\n");
			printf("*******************************************\n");
			boss2Clear = 1;
			dungeon[x][y].boss2 = 0;

			printf("キーを押して探索画面へ戻ります。\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\nボス戦から撤退しました...\n");
		}
		else {
			// ボスに負けたの敗北条件達成
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].boss2) {
		//仕事数を満たしていない
		printf("\n[部長の部屋]\n");
		printf("主人公「課長から話を聞いているかもしれませんが有給休暇の話をしに来ました。」\n");
		c = getchar();
		printf("部長「その程度の実績でわざわざ来たのか…　\n");
		printf("もっと実績を積んで出直して来い！」\n");
		printf("->まだ挑むには早いようだ・・・他のボスを倒していないか、仕事数が足りてない。\n");

		c = getchar();
		showDungeon();
		showRoomInfo();
	}

	//////////////副社長///////////////

	if (dungeon[x][y].boss3 && boss2Clear == 1)
	{
		printf("\n[副社長の部屋]\n");
		printf("主人公「部長がどうしても聞き入れてくれないため、有給休暇の話をしに来ました。」\n");
		c = getchar();

		printf("                 , ｲ \n");
		printf(" ￣　-- = ＿　　 / |            --'' \n");
		printf("     , , , , r‐､λノ　 ﾞi､_, ､ﾉゝ　　　　　 - ￣\n");
		printf("          ﾞl               ﾞ､_ \n");
		printf("         .j´.彡⌒ミ　　　（. \n");
		printf("─ 　　─{ (´・ω・｀) / ─　　　＿　　　　　─ \n");
		printf("         ).c /      ,つ 　 ,l~ \n");
		printf("      ´y   ｛ ,、｛　 　 < \n");
		printf("        ゝ   lﾉ ヽ,)　　 , \n");
		printf(" \n副社長「バッカもーーーーーーん!!\n");
		printf(" 有休なんぞ下っ端には必要あらん!!」\n");
		printf("キーを押して戦闘に入ります！\n");
		c = getchar();
		result = boss3Fight(boss3HP);

		if (result == syujinkoWins)
		{

			printf("主人公「副社長ともまともな話をすることは出来なかったな…\n");
			printf("こうなったら直接社長と話をしないとな。」\n");

			printf("\n  彡⌒ミ        彡⌒ミ\n");
			printf("   (´XωX｀)    (´XωX｀)\n");
			printf("   | ￣￣￣ |    | ￣￣￣ |\n");
			printf("   | ￣￣￣ |    | 二二二 |\n");
			printf("   | ￣￣￣ |    | ＿＿＿ |\n");
			printf("    从从从从       从从从从\n");

			printf("\n主人公「ん、また紙切れが落ちてるな。」\n");

			printf("*******************************************\n");
			printf("*                                         *\n");
			printf("*      社長は中身はともかく外面はいい     *\n");
			printf("*            声を荒げたりしないで         *\n");
			printf("*             丁寧な話し方をする          *\n");
			printf("*                                         *\n");
			printf("*******************************************\n");
			boss3Clear = 1;
			dungeon[x][y].boss3 = 0;

			printf("キーを押して探索画面へ戻ります。\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\nボス戦から撤退しました...\n");
		}
		else {
			// ボスに負けたの敗北条件達成
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].boss3) {
		//仕事数を満たしていない
		printf("\n[副社長の部屋]\n");
		printf("主人公「部長がどうしても聞き入れてくれないため、有給休暇の話をしに来ました。\n");
		c = getchar();
		printf("副社長「まだまだ仕事が残っておるではないか！　\n");
		printf("さっさと実績を積んで来んか！」\n");
		printf("->まだ挑むには早いようだ・・・他のボスを倒していないか、仕事数が足りてない。\n");
		c = getchar();
		showDungeon();
		showRoomInfo();
	}

	//***********ここからラスボス************

	if (dungeon[x][y].lastBoss && boss3Clear == 1)
	{
		printf("\n[社長室]\n");
		printf("社長「君が幸せを知らない社員ですね\n");
		printf("良いですか？仕事はお金を得るためだけにやるものではないですよ\n");
		printf("働いた分だけ\"ありがとう\"が帰ってきます。その感謝の言葉があれば\n");
		printf("休みなどなくても一生懸命働くことが出来ます。」\n");
		c = getchar();


		printf("	      	*'``・* 。 \n");
		printf("			| `*。\n");
		printf(".		,  ,., ｡∩彡 ⌒ ミ　* 　　　\n");
		printf("			+ (´･ω･`)　*｡ + ﾟ \n");
		printf("			`*｡ ヽ、　 つ *ﾟ* \n");
		printf("			`・ + ｡*・' ﾟ⊃ +ﾟ \n");
		printf("			☆　　 ∪~｡*ﾟ \n");
		printf("				`・ + ｡*・ ﾟ \n");

		printf("「あなたには直接指導してあげますから感謝してくださいね。」\n");

		printf("\nキーを押して戦闘に入ります！\n");
		c = getchar();
		result = lastBossFight(lastBossHP);

		if (result == syujinkoWins)
		{
			//戦闘後
			if (syujinko.jobCounter == 5)
			{
				printf("社長「仕事を5回こなして来ましたか。\n");
			}
			else if (syujinko.jobCounter > 5)
			{
				printf("社長「仕事を%d回もこなして来ましたか！\n", syujinko.jobCounter);
			}
			else if (syujinko.jobCounter > 10)
			{
				printf("社長「なんと！　仕事を%d回もこなして来たのですか！！\n", syujinko.jobCounter);
			}

			printf("なるほど…あなたの熱意は伝わりました。\n");
			printf("人間は適度な休みがあるからこそ仕事を頑張ることができる…\n");
			printf("これからはみんなが幸せになれる会社を目指します。\n");
			c = getchar();

			if (syujinko.jobCounter == 5)
			{
				printf("あなたの熱意に応え、有休を2日与えましょう。」\n");
			}
			else if (syujinko.jobCounter > 5)
			{
				printf("よくそんな数の仕事をこなしてくれました。\n");
				printf("あなたには有休を1週間与えましょう。」\n");
			}
			else if (syujinko.jobCounter > 10)
			{
				printf("凄まじい数の仕事をよく1人でこなしてくれました。\n");
				printf("これで我が社の業務がより円滑に進むことでしょう。\n");
				printf("あなたには2週間の有休を与えましょう。」\n");
			}
			bossDead = 1;

			// ラスボスを削除
			dungeon[x][y].lastBoss = 0;
		}
		else if (result == escape) {
			printf("\nボス戦から撤退しました...\n");
		}
		else {
			// ボスに負けたの敗北条件達成
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].lastBoss) {
		printf("\n[社長室前]\n");
		printf("主人公「社長室の前まで来たけれども…\n");
		printf("さすがにこの程度の実績じゃ話を聞いてくれないだろうな\n");
		printf("もっと仕事をこなしてから出直そう。」\n");
		printf(" ->社長に挑むにはまだ早いようだ・・・他のボスの撃破数か仕事数が足りてない。\n ");
		c = getchar();
		showDungeon();
		showRoomInfo();
	}
}

int boss1Fight(int boss1HP)//田代
{
	int player, com = 0;
	char c;
	boss1HP = fightBoss1HP;
	printf("最初のボスとの戦闘だ！\n");
	srand((unsigned)time(NULL));

	while (1) //どっちか死ぬまでじゃんけんすんぞ!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 主人公の疲労度: %d , ボス1のHP: %d  \n", syujinko.health, boss1HP);
		printf("                       威圧:%d, 論理:%d, 丁寧:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n出す手を選んでね！ 1:威圧(グー), 2:論理(チョキ), 3:丁寧(パー) \n ");
		printf("逃げたい場合は4を選んでね!\n");
		scanf_s("%d", &player);

		com = rand() % 3 + 1;

		switch (player) {
		case 1:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 2) {
				boss1HP -= syujinko.str1;
				manyThingsBoss1();
				break;
			}
			if (player && com == 3) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 2:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 3) {
				boss1HP -= syujinko.str2;
				manyThingsBoss1();
				break;
			}
			if (player && com == 1) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 3:
			if (player == com) {
				printf("\n\n\nあいこだ！\n");
				player = getchar();
				break;
			}
			if (player && com == 1) {
				boss1HP -= syujinko.str3;
				manyThingsBoss1();
				break;
			}
			if (player && com == 2) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 4: {
			//撤退
			player = getchar();
			goto OUT;
		}
		default:
			printf("無効な手だよ\n");
			player = getchar();
			break;
		}
		// ボスを倒したかどうかのチェック
		if (boss1HP <= 0) {
			printf("\n**課長を倒した!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// 主人公が死んだかどうかのチェック
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ありえない条件をつけることでとりあえずGOTOだけ
		if (boss1HP == 400) {
		OUT:
			fightBoss1HP = boss1HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int boss2Fight(int boss2HP) //田代
{
	int player, com = 0;
	char c;
	boss2HP = fightBoss2HP;
	printf("2番目のボスとの戦闘だ！\n");
	srand((unsigned)time(NULL));

	while (1) //どっちか死ぬまでじゃんけんすんぞ!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 主人公の疲労度: %d , ボス2のHP: %d  \n", syujinko.health, boss2HP);
		printf("                       威圧:%d, 論理:%d, 丁寧:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n出す手を選んでね！ 1:威圧(グー), 2:論理(チョキ), 3:丁寧(パー) \n ");
		printf("逃げたい場合は4を選んでね!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com || com == 4) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 2) {
				boss2HP -= syujinko.str1;
				manyThingsBoss2();
				break;
			}
			if (player && com == 3) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 2:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 3) {
				boss2HP -= syujinko.str2;
				manyThingsBoss2();
				break;
			}
			if (player && com == 1 || com == 4) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 3:
			if (player == com) {
				printf("\n\n\nあいこだ！\n");
				player = getchar();
				break;
			}
			if (player && com == 1 || com == 4) {
				boss2HP -= syujinko.str3;
				manyThingsBoss2();
				break;
			}
			if (player && com == 2) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 4: {
			//撤退
			player = getchar();
			goto OUT;
		}
		default:
			printf("無効な手だよ\n");
			player = getchar();
			break;
		}
		// ボスを倒したかどうかのチェック
		if (boss2HP <= 0) {
			printf("\n**ボスを倒した!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// 主人公が死んだかどうかのチェック
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ありえない条件をつけることでとりあえずGOTOだけ
		if (boss2HP == 400) {
		OUT:
			fightBoss2HP = boss2HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int boss3Fight(int boss3HP) //井上
{
	int player, com = 0;
	char c;
	boss3HP = fightBoss3HP;
	printf("3番目のボスとの戦闘だ！\n");
	srand((unsigned)time(NULL));

	while (1) //どっちか死ぬまでじゃんけんすんぞ!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 主人公の疲労度: %d , ボス3のHP: %d  \n", syujinko.health, boss3HP);
		printf("                       威圧:%d, 論理:%d, 丁寧:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n出す手を選んでね！ 1:威圧(グー), 2:論理(チョキ), 3:丁寧(パー) \n ");
		printf("逃げたい場合は4を選んでね!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com || com == 4) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 2) {
				boss3HP -= syujinko.str1;
				manyThingsBoss3();
				break;
			}
			if (player && com == 3) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 2:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 3) {
				boss3HP -= syujinko.str2;
				manyThingsBoss3();
				break;
			}
			if (player && com == 1 || com == 4) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 3:
			if (player == com) {
				printf("\n\n\nあいこだ！\n");
				player = getchar();
				break;
			}
			if (player && com == 1 || com == 4) {
				boss3HP -= syujinko.str3;
				manyThingsBoss3();
				break;
			}
			if (player && com == 2) {
				syujinko.health += 10;
				takeDamage();
				break;
			}
		case 4: {
			//撤退
			player = getchar();
			goto OUT;
		}
		default:
			printf("無効な手だよ\n");
			player = getchar();
			break;
		}
		// ボスを倒したかどうかのチェック
		if (boss3HP <= 0) {
			printf("\n**ボスを倒した!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// 主人公が死んだかどうかのチェック
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ありえない条件をつけることでとりあえずGOTOだけ
		if (boss3HP == 400) {
		OUT:
			fightBoss3HP = boss3HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int lastBossFight(int lastBossHP) //田代
{
	int player, com = 0;
	char c;
	lastBossHP = fightLastBossHP;
	printf("\n **ラスボスとの戦闘だ！**\n");
	srand((unsigned)time(NULL));

	while (1) //どっちか死ぬまでじゃんけんすんぞ!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 主人公の疲労度: %d , ラスボスのHP: %d  \n", syujinko.health, lastBossHP);
		printf("                       威圧:%d, 論理:%d, 丁寧:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n出す手を選んでね！ 1:威圧(グー), 2:論理(チョキ), 3:丁寧(パー) \n ");
		printf("逃げたい場合は4を選んでね!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 2) {
				lastBossHP -= syujinko.str1;
				manyThingsLastBoss();
				break;
			}
			if (player && com == 3 || com == 4) {
				syujinko.health += 20;
				takeDamage();
				break;
			}
		case 2:
			if (player == com) {
				printf("\n\n\nあいこだ！\n ");
				player = getchar();
				break;
			}
			if (player && com == 3 || com == 4) {
				lastBossHP -= syujinko.str2;
				manyThingsLastBoss();
				break;
			}
			if (player && com == 1) {
				syujinko.health += 20;
				takeDamage();
				break;
			}
		case 3:
			if (player == com || com == 4) {
				printf("\n\n\nあいこだ！\n");
				player = getchar();
				break;
			}
			if (player && com == 1) {
				lastBossHP -= syujinko.str3;
				manyThingsLastBoss();
				break;
			}
			if (player && com == 2) {
				syujinko.health += 20;
				takeDamage();
				break;
			}
		case 4: {
			//撤退
			player = getchar();
			goto OUT;
		}
		default:
			printf("無効な手だよ\n");
			player = getchar();
			break;
		}
		// ボスを倒したかどうかのチェック
		if (lastBossHP <= 0) {
			printf("\n**ボスを倒した!**\n");
			return syujinkoWins;
		}
		// 主人公が死んだかどうかのチェック
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ありえない条件をつけることでとりあえずGOTOだけ
		if (lastBossHP == 400) {
		OUT:
			fightLastBossHP = lastBossHP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

//tasiro
void manyThingsBoss1() { 
	char t;
	int r;

	r = rand() % 3 + 1;
	switch (r) {
	case 1:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\n ￣￣￣￣￣￣￣￣￣￣彡⌒ ミ ￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("          ―――― と(`･ω･´) 　　　　　　￣￣￣￣\n");
		printf("￣￣￣　　――――‐ / ⊂_ノ 　　　―――＿＿　　　　――\n");
		printf("    ――　　　　　. / ／⌒ｿ \n");
		printf("       ￣￣　　　  -'´ 　　　　　￣￣￣ \n");
		printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("\n 「なかなか腕をあげましたね新人君...!」\n");
		printf("攻撃成功だ！\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\n ￣￣￣￣￣￣￣￣￣￣彡⌒ ミ ￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("          ―――― と(´･ω･`) 　　　　　　￣￣￣￣\n");
		printf("￣￣￣　　――――‐ / ⊂_ノ 　　　―――＿＿　　　　――\n");
		printf("    ――　　　　　. / ／⌒ｿ \n");
		printf("       ￣￣　　　  -'´ 　　　　　￣￣￣ \n");
		printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("\n 「まだまだ若いものには負けませんよ...！」\n");
		printf("攻撃成功だ！\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\n ￣￣￣￣￣￣￣￣￣￣彡⌒ ミ ￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("          ―――― と(`･ω･´) 　　　　　　￣￣￣￣\n");
		printf("￣￣￣　　――――‐ / ⊂_ノ 　　　―――＿＿　　　　――\n");
		printf("    ――　　　　　. / ／⌒ｿ \n");
		printf("       ￣￣　　　  -'´ 　　　　　￣￣￣ \n");
		printf("￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣\n");
		printf("\n 「君は大物になる....そんな気がするな！」\n");
		printf("攻撃成功だ！\n");
		t = getchar();
		break;

	}
}

//tasiro
void manyThingsBoss2() {
	char t;
	int r;

	r = rand() % 3 + 1;
	switch (r) {
	case 1:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("				 ）ｿ）\n");
		printf("				ｯλ　ﾉ（., ﾉ)\n");
		printf("		         （ﾞ - ..::.::..（\n");
		printf("			    (ソ..彡⌒ミ.）ｿ)\n");
		printf("			）.::'; (`･ω･´) 　 ( \n");
		printf("		  ｿ　....::; ';'（つ ⊂）::; ';'`～､. \n");
		printf("			(:; ';'  |_∧_| ::; ';' 　ヽ）\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n「新人如きが上司に逆らっていいのか！」\n");
		printf("攻撃成功だ！...いくら上司とはいえ少しウザいよなぁ...。\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("				 ）ｿ）\n");
		printf("				ｯλ　ﾉ（., ﾉ)\n");
		printf("		         （ﾞ - ..::.::..（\n");
		printf("			    (ソ..彡⌒ミ.）ｿ)\n");
		printf("			）.::'; (´･ω･`) 　 ( \n");
		printf("		  ｿ　....::; ';'（つ ⊂）::; ';'`～､. \n");
		printf("			(:; ';'  |_∧_| ::; ';' 　ヽ）\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n「新人のくせに生意気なことを言うな！」\n");
		printf("攻撃成功だ！しかしなんでこんな奴が部長なんだろうか...。\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("				 ）ｿ）\n");
		printf("				ｯλ　ﾉ（., ﾉ)\n");
		printf("		         （ﾞ - ..::.::..（\n");
		printf("			    (ソ..彡⌒ミ.）ｿ)\n");
		printf("			）.::'; (´･ω･`) 　 ( \n");
		printf("		  ｿ　....::; ';'（つ ⊂）::; ';'`～､. \n");
		printf("			(:; ';'  |_∧_| ::; ';' 　ヽ）\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n 「大人しく上の言葉に従え！歯向かうんじゃない！」\n");
		printf("攻撃成功だ！部長は微塵も話を聞こうとしてないな...。\n");
		t = getchar();
		break;

	}
}

//tasiro
void manyThingsBoss3() {
	char t;
	int r;

	r = rand() % 3 + 1;
	switch (r) {
	case 1:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("                 , ｲ \n");
		printf(" ￣　-- = ＿　　 / |            --'' \n");
		printf("     , , , , r‐､λノ　 ﾞi､_, ､ﾉゝ　　　　　 - ￣\n");
		printf("          ﾞl               ﾞ､_ \n");
		printf("         .j´.彡⌒ミ　　　（. \n");
		printf("─ 　　─{ (´・ω・｀) / ─　　　＿　　　　　─ \n");
		printf("         ).c /      ,つ 　 ,l~ \n");
		printf("      ´y   ｛ ,、｛　 　 < \n");
		printf("        ゝ   lﾉ ヽ,)　　 , \n");
		printf("\n「まともに働きもしない若造に休みは無い!!」\n");
		printf("攻撃成功だ！副社長のオーラが迸っている...。\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("                 , ｲ \n");
		printf(" ￣　-- = ＿　　 / |            --'' \n");
		printf("     , , , , r‐､λノ　 ﾞi､_, ､ﾉゝ　　　　　 - ￣\n");
		printf("          ﾞl               ﾞ､_ \n");
		printf("         .j´.彡⌒ミ　　　（. \n");
		printf("─ 　　─{ (´・ω・｀) / ─　　　＿　　　　　─ \n");
		printf("         ).c /      ,つ 　 ,l~ \n");
		printf("      ´y   ｛ ,、｛　 　 < \n");
		printf("        ゝ   lﾉ ヽ,)　　 , \n");
		printf("\n「私は社長になるために休まん!!」\n");
		printf("攻撃成功だ！休まないおかげで禿の進行が早いみたいだ...。\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("                 , ｲ \n");
		printf(" ￣　-- = ＿　　 / |            --'' \n");
		printf("     , , , , r‐､λノ　 ﾞi､_, ､ﾉゝ　　　　　 - ￣\n");
		printf("          ﾞl               ﾞ､_ \n");
		printf("         .j´.彡⌒ミ　　　（. \n");
		printf("─ 　　─{ (´・ω・｀) / ─　　　＿　　　　　─ \n");
		printf("         ).c /      ,つ 　 ,l~ \n");
		printf("      ´y   ｛ ,、｛　 　 < \n");
		printf("        ゝ   lﾉ ヽ,)　　 , \n");
		printf("\n 「バッカもーーーーーーーーーん!」\n");
		printf("攻撃成功だ！とりあえず怒ってるみたいだ。\n");
		t = getchar();
		break;

	}
}

//tasiro
void manyThingsLastBoss() {
	char t;
	int r;

	r = rand() % 3 + 1;
	switch (r) {
	case 1:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("	      	*'``・* 。 \n");
		printf("			| `*。\n");
		printf(".		,  ,., ｡∩彡 ⌒ ミ　* 　　　\n");
		printf("			+ (´･ω･`)　*｡ + ﾟ \n");
		printf("			`*｡ ヽ、　 つ *ﾟ* \n");
		printf("			`・ + ｡*・' ﾟ⊃ +ﾟ \n");
		printf("			☆　　 ∪~｡*ﾟ \n");
		printf("				`・ + ｡*・ ﾟ \n");
		printf("\n「まだ働くことの有難みがわからないのですか...。」\n");
		printf("攻撃成功だ！長期戦になるほど洗脳されてしまいそうだ。\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("	      	*'``・* 。 \n");
		printf("			| `*。\n");
		printf(".		,  ,., ｡∩彡 ⌒ ミ　* 　　　\n");
		printf("			+ (´･ω･`)　*｡ + ﾟ \n");
		printf("			`*｡ ヽ、　 つ *ﾟ* \n");
		printf("			`・ + ｡*・' ﾟ⊃ +ﾟ \n");
		printf("			☆　　 ∪~｡*ﾟ \n");
		printf("				`・ + ｡*・ ﾟ \n");
		printf("\n「感謝。感謝感謝感謝感謝感謝感謝感謝。」\n");
		printf("攻撃成功だ！どうして社長はこんなにも...。\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("	      	*'``・* 。 \n");
		printf("			| `*。\n");
		printf(".		,  ,., ｡∩彡 ⌒ ミ　* 　　　\n");
		printf("			+ (´･ω･`)　*｡ + ﾟ \n");
		printf("			`*｡ ヽ、　 つ *ﾟ* \n");
		printf("			`・ + ｡*・' ﾟ⊃ +ﾟ \n");
		printf("			☆　　 ∪~｡*ﾟ \n");
		printf("				`・ + ｡*・ ﾟ \n");
		printf("\n 「仕事こそ正義...。」\n");
		printf("攻撃成功だ！確かに働くことは大事だけど...。\n");
		t = getchar();
		break;

	}
}

//tasiro
void takeDamage() {

	char t;
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("      ＿＿＿＿＿＿＿＿ \n");
	printf("    ／ﾒ\":ﾞ; , ., ; .ﾒ＼\n");
	printf("  ／ﾒ ;; .; #; ﾒ;; #;; .＼\n");
	printf("／; .；(;;ﾒ);; . （×）.；＼\n");
	printf("| ;; .; ﾒ（__人__）; .;; ﾒﾒ |\n");
	printf("＼ﾒ#;;;; .ﾒ　ﾒ; ;; ﾒ;; .; .／\n");
	printf("／ﾒ;;　ﾒ; ;; .ﾒ; ;;;; ; ﾒ.＼\n");
	printf(" 論破された・・疲労度が増加した。\n ");

	t = getchar();
}

//tasiro
void yarikomi() {
	char c;

	int finallyStatus = syujinko.str1 + syujinko.str2 + syujinko.str3;
	printf("ゲーム中に使った回復アイテムの数:%d\n", addItems);
	printf("ゲーム中に休憩所を利用した回数:%d\n", addRests);
	printf("ステータスの合計値:%d\n", finallyStatus);

	printf("あなたの称号は,");
	c = getchar();

	if (addItems == 0 && addRests == 0 && finallyStatus <= 6)
	{
		printf(" とんでもない縛りプレイをしてくれた ");

	}
	else if (addItems == 0 && addRests == 0)
	{
		printf(" 素晴らしくラッキーで ");

	}
	else if (addItems == 0)
	{
		printf(" 結構ラッキーで ");

	}
	else if (addItems >= 6)
	{
		printf(" 栄養ドリンク漬けで ");
	}
	else if (addRests >= 3)
	{
		printf(" 休憩室の守護神で ");
	}
	else
	{
		printf(" 普通にプレイしてくれた ");
	}

	if (finallyStatus == 18) {
		printf(" 論破の神となった ");
	}
	else if (finallyStatus >= 15) {
		printf(" 交渉に長けている ");
	}
	else if (finallyStatus >= 10) {
		printf(" 普通に喋れる ");
	}
	else if (finallyStatus >= 7) {
		printf(" コミュニティに弊害がある ");
	}

	printf("社畜です。\n");

}



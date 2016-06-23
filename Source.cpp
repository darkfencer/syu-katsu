#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//�Ƃ���ǂ���ɂ���3�Aprintf���s�͉�ʂ��݂₷�����钲���ł�
//�����Ԃ�����΂���������ʂ����t���b�V�����ă_���W�����`�悵������

//�����̏��
typedef struct
{
	int syujinko; //��l���̈ʒu 
	int boss1; //�{�X1�̈ʒu
	int boss2; //�{�X2�̈ʒu
	int boss3; //�{�X3�̈ʒu
	int lastBoss; //���X�{�X�̈ʒu
	int doorInfo[4]; //�ǂ̏��
	int items; //�A�C�e���C�x���g�̈ʒu
	int rest1; //�x�e��1�̈ʒu
	int rest2; //�x�e��2�̈ʒu
	int hints; //�q���g�̈ʒu
	int syujinkoVisited; //�����Ɏ�l�����K�₵���� 

} Room;

Room dungeon[6][6];

typedef struct
{
	int roomX; //��l���̂��镔����X
	int roomY; //��l���̂��镔����Y
	int hasItems; //�񕜃A�C�e���������Ă��邩(�ő�5�܂Ŏ��Ă�H�̂Ő������K�v
	int str1; //�Ј��X�e�[�^�X
	int str2; //�_���X�e�[�^�X
	int str3; //���J�X�e�[�^�X
	int health; //HP����
	int jobCounter; //�d�����I�����������Z���Ă�����

} Hero;

Hero syujinko;


//�{�X�̏���HP�ݒ�
const int boss1StartHP = 3;
const int boss2StartHP = 10;
const int boss3StartHP = 20;
const int lastBossStartHP = 30;

//��l���̏����X�e�ݒ�
const int syujinkoStartHP = 0;
const int syujinkoStartStr1 = 1;
const int syujinkoStartStr2 = 1;
const int syujinkoStartStr3 = 1;

//�K�v�Ȏd���� �����Hero.jobcounter���r���Đ퓬�����𖞂���
const int needJobs1 = 2;
const int needJobs2 = 5;

// �����������߂邽�߂̃O���[�[�o���ϐ�
const int syujinkoWins = 1;
const int syujinkoLoses = 2;
const int escape = 3;

//��
const int noDoor = 0;
const int openDoor = 1;

//����
const int North = 0;
const int East = 1;
const int South = 2;
const int West = 3;

//�{�X��hp�����ϐ�
int boss1HP;
int boss2HP;
int boss3HP;
int lastBossHP;
int fightBoss1HP;
int fightBoss2HP;
int fightBoss3HP;
int fightLastBossHP;

//�����_���_���W�����p
int ranA, ranB, ranC, ranD, ranE, ranF, ranG, ranH, ranI, ranJ, ranK, ranL;
int ranMode = 0;

//��荞�ݗv�f
int addItems = 0;
int addRests = 0;


//�e�L�X�g�\���p
int checkPrintText;

//�{�X�|�����甭������t���O
int boss1Clear;
int boss2Clear;
int boss3Clear;

//�x�e���g�����炱����Z�ł����Z�ł������������Ď�����g���Ȃ�����
int useRestRoom;

//�e�L�X�g�����ϐ�
char text;

//�I������
int bossDead;
int syujinkoDead;

//�֐��̃v���g�^�C�v
void resetDungeon(); //�_���W����������
void resetSyujinko(); //��l���̏�ԏ�����
void resetDungeonRan(); //�_���W����������
void resetSyujinkoRan(); //��l���̏�ԏ�����
void resetBoss(); //�{�X������
void resetGameEnding(); //�I������������

						//char print_string(char);
void printRule(); //���[���\��
void showDungeon(); //�_���W�����̕\��
void showRoomInfo(); //�������̕\��
void showStatus(); //�X�e�[�^�X�\��
int getUserMove(); //���[�U�[���͂ɂ��ړ��̏��� moveSyujinko�Ɗ֘A�t���L
void moveSyujinko(int);
void checkBossFight(void); //�{�X�Ƃ̐퓬�����𖞂�����
void printText();
void itemDice(); //�A�C�e���C�x���g�̂��
int randomDice(); //�������тɔ���������
void tryUseRestRoom(void); //�x�e�������H
int boss1Fight(int); //�{�X1�Ɛ키�Ƃ��̂܂��Ă�
int boss2Fight(int); //2
int boss3Fight(int); //3
int lastBossFight(int); //�炷�ڂ�
void manyThingsBoss1(); //�{�X���aa�Ƃ�
void manyThingsBoss2();
void manyThingsBoss3();
void manyThingsLastBoss();
void takeDamage(); //damage
void yarikomi(); //�̍�����


int main(void)
{
	char c;
	int moveDirection; //�ړ�����


	//������
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


	//�Q�[���̃��[�v...�I�������̂����ꂩ�����������܂Ń��[�v����
	while (1)
	{
		showDungeon();	// �_���W�����̏���\��
		showRoomInfo();	// �����̏���\��

		checkBossFight(); // �{�X������Ȃ�퓬
		if (syujinkoDead) // �{�X�ɕ������i�s�k�j
		{
			break;
		}
		if (bossDead) //�{�X��|�����i�����j
		{
			break;
		}

		showStatus(); //�X�e�[�^�X�\��
		tryUseRestRoom(); // �x�e��������Ύg�������[�U�[�ɖ₤	
		printText();

		moveDirection = getUserMove(); // ���[�U�[����ړ���������͂��Ă��炤
		moveSyujinko(moveDirection); // ���[�U���I�񂾕����Ɏ�l�����ړ�������

		randomDice(); //�����_���_�C�X�𔭐�������

	}

	// �������s�k�̃��b�Z�[�W�ŃQ�[���I������

	if (bossDead)
	{
		if (syujinko.jobCounter > 10)
		{
			printf("\n**********************\n");
			printf("*   �L���������!!   *\n");
			printf("**********************\n\n");
			printf("���߂łƂ��I����Ď�l���͗L������ɓ���邱�Ƃ��o�����B\n");
			printf("�L���ŊC�O���s�ł��s�����A����Ȃ��Ƃ��l���Ȃ���A�H�֒������E�E�B\n");
			c = getchar();
			yarikomi();
		}
		else if (syujinko.jobCounter > 5)
		{
			printf("\n**********************\n");
			printf("*   �L���������!!   *\n");
			printf("**********************\n\n");
			printf("���߂łƂ��I����Ď�l���͗L������ɓ���邱�Ƃ��o�����B\n");
			printf("�L���ō������򗷍s�ł��s�����A����Ȃ��Ƃ��l���Ȃ���A�H�֒������E�E�B\n");
			c = getchar();
			yarikomi();
		}
		else if (syujinko.jobCounter == 5)
		{
			printf("\n**********************\n");
			printf("*   �L���������!!   *\n");
			printf("**********************\n\n");
			printf("���߂łƂ��I����Ď�l���͗L������ɓ���邱�Ƃ��o�����B\n");
			printf("�L���Őς�ł������Q�[���ł��������邩�A����Ȃ��Ƃ��l���Ȃ���A�H�֒������E�E�B\n");
			c = getchar();
			yarikomi();
		}
	}
	else if (syujinkoDead)
	{
		printf("\n��l���̔�J�x�����E���z���Ă��܂���!\n");
		printf("\n***************************\n");
		printf("*   �N�͕a�@�։^�΂ꂽ...  *\n");
		printf("***************************\n\n");
		c = getchar();
		printf("�N�͓��������ĕa�@�։^�΂�Ă����悤���B\n");
		printf("�|���قǓ��������x�݂͖����B�ʉ@���I���΂܂��В{�̓��X���n�܂�...\n");
		printf("\n\n........\n");
		c = getchar();
		printf("\n...�����ČN��\n");
		c = getchar();
		printf("\n\n\n\nBAD END    �В{\n");
	}

	printf("\n Thank you for playing!!\n");
	c = getchar();

	return 0;
}

void resetDungeon()
{
	int y;

	// ��l���͍ŏ�(1,5)�̕����ɂ���
	dungeon[1][5].syujinko = 1;
	// ��l����(1,1)�̕�����K�₵�Ă���
	dungeon[1][5].syujinkoVisited = 1;
	// boss1��(3,5)�̕����ɂ���
	dungeon[3][5].boss1 = 1;
	// boss2��(3,2)�̕����ɂ���
	dungeon[3][2].boss2 = 1;
	// boss3��33�ɂ����
	dungeon[3][3].boss3 = 1;
	// lastboss��(5,1)�̕����ɂ���
	dungeon[5][1].lastBoss = 1;
	// �x�e����(2,2)��(4,4)�ɔz�u����
	dungeon[2][2].rest1 = 2;
	dungeon[4][4].rest2 = 2;
	// �q���g��(2,1)��(5,3)�ɔz�u����B
	//dungeon[ 5 ][ 3 ].hints = 1;
	//dungeon[ 2 ][ 1 ].hints = 1;
	// �A�C�e����(3,1)�ɔz�u����
	//dungeon[3][1].items = 1;

	// �h�A�̏��

	// 1��ڂ̃h�A���
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


	// 2��ڂ���4��ڂ܂ł̏��͓����̂�for���[�v�ŏ�����
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

	// 5��ڂ̃h�A���
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


// ��l���̏���������
void resetSyujinko()
{
	// ��l�����ŏ��ɂ��镔����(1,5)
	syujinko.roomX = 1;
	syujinko.roomY = 5;
	// ��l���̃G�i�W�[�h�����N�X�g�b�N�������Z�b�g
	syujinko.hasItems = 2;
	// �d���J�E���^�[�����Z�b�g
	syujinko.jobCounter = 0;
	// ��l���̃X�e�[�^�X�̃��Z�b�g
	syujinko.str1 = syujinkoStartStr1;
	syujinko.str2 = syujinkoStartStr2;
	syujinko.str3 = syujinkoStartStr3;
	// �ŏ��͍ő�̂g�o�ɐݒ�
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


	// ��l���͍ŏ�(a,b)�̕����ɂ���
	dungeon[ranA][ranB].syujinko = 1;
	// ��l����(a,b)�̕�����K�₵�Ă���
	dungeon[ranA][ranB].syujinkoVisited = 1;
	// boss1��(,)�̕����ɂ���
	dungeon[ranE][ranF].boss1 = 1;
	// boss2��(,)�̕����ɂ���
	dungeon[ranG][ranH].boss2 = 1;
	// boss3��====
	dungeon[ranJ][ranC].boss3 = 1;
	// lastboss��(5,1)�̕����ɂ���
	dungeon[ranI][ranJ].lastBoss = 1;
	// �x�e����(2,2)��(4,4)�ɔz�u����
	dungeon[ranK][ranL].rest1 = 2;
	dungeon[ranC][ranD].rest2 = 2;
	if (ranK == ranC && ranL == ranD)
	{
		dungeon[2][2].rest1 = 2;
		dungeon[4][4].rest2 = 2;
	}


	// �h�A�̏��

	// 1��ڂ̃h�A���
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


	// 2��ڂ���4��ڂ܂ł̏��͓����̂�for���[�v�ŏ�����
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

	// 5��ڂ̃h�A���
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


// ��l���̏���������
void resetSyujinkoRan()
{
	int a = ranA;
	int b = ranB;
	// ��l�����ŏ��ɂ��镔����(1,5)
	syujinko.roomX = a;
	syujinko.roomY = b;
	// ��l���̃G�i�W�[�h�����N�X�g�b�N�������Z�b�g
	syujinko.hasItems = 2;
	// �d���J�E���^�[�����Z�b�g
	syujinko.jobCounter = 0;
	// ��l���̃X�e�[�^�X�̃��Z�b�g
	syujinko.str1 = syujinkoStartStr1;
	syujinko.str2 = syujinkoStartStr2;
	syujinko.str3 = syujinkoStartStr3;
	// �ŏ��͍ő�̂g�o�ɐݒ�
	syujinko.health = syujinkoStartHP;
}


// boss�̏���������
void resetBoss()
{
	boss1HP = boss1StartHP;	//boss1��HP
	boss2HP = boss2StartHP;	// boss2��HP
	boss3HP = boss3StartHP;
	lastBossHP = lastBossStartHP;// lastboss��HP
	fightBoss1HP = 3;
	fightBoss2HP = 10;
	fightBoss3HP = 20;
	fightLastBossHP = 30;
	boss1Clear = 0;
	boss2Clear = 0;
}

//�Q�[���̏I������������
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
	printf(" *       SHA�� CHI�� KU�ւ悤����!        *\n");
	printf(" *                                        *\n");
	printf(" *      SHA�� CHI�� KU�̓V���v����        *\n");
	printf(" *      �e�L�X�g�x�[�X��RPG�ł�           *\n");
	printf(" *    Design: CS���p�����e�[�}A �`�[���X  *\n");
	printf(" *                                        *\n");
	printf(" ******************************************\n");

	printf("\n");
	printf("1) ���[����\�����܂�\n");
	printf("2) �Q�[���X�^�[�g!\n");
	printf("3) �z�u�����_�����[�h\n");
	printf("1��2��3����͂��Ă�������: ");

	// ���[�U�̑I�����l��
	c = getchar();
	userSelection = c;
	while ((c = getchar()) != '\n')
		;
	if (userSelection == '1')
	{
		printf("\nSHA�� CHI�� KU�̃��[��:\n");
		printf("==========================\n");
		printf("1) ����2�N�ڂ̎В{�ł����l���́A�����ǂ���i�ɗ����������A�L�x��������邽�߁A�킢�܂��B\n");
		c = getchar();
		printf("2) ��l���̏����ʒu��(1,5)�ƂȂ��Ă��܂�(����)�B\n");
		c = getchar();
		printf("3) �i�߂�������\�������̂ŁA�Ώۂ̃L�[����͂���Ƃ��̕�����1�}�X�i�݂܂��B\n");
		c = getchar();
		printf("4) �����~�܂����}�X�ɂ���ăC�x���g���������邱�Ƃ�����܂��B\n");
		c = getchar();
		printf("5) �̗͂́A��J�x�Ƃ����T�O�̂��߁A0����X�^�[�g�ƂȂ�A��l���̔�J�x��100�ɓ��B����ƁA�Q�[���I�[�o�[�ƂȂ�܂��B\n");
		c = getchar();
		printf("6) �d���ɂ���\n  �����A�u�d���v�Ƃ������̂��������邱�Ƃ�����܂��B�ꕔ�̃{�X�Ɛ키���߂ɂ́A���́u�d���v����萔���Ȃ��K�v������܂��B�܂��A�u�d�� �v����������ƁA��l���̔�J�x��10�������邩���ɁA�D���ȃX�e�[�^�X��1�����㏸�����邱�Ƃ��ł��܂��B\n");
		c = getchar();
		printf("7) �X�e�[�^�X�ɂ���\n  �{�X�Ƃ̐퓬�Ɋւ���3�̍U���͂̂��Ƃ��w���܂��B����͂���񂯂�̎�ɑ����Ă���A�u�Ј��i�O�[ �j�v�A�u�_���i�`���L �j�v�A�u���J�i�p�[ �j�v��3�̎肪����܂��B�����X�e�[�^�X�͑S�āu1�v�ƂȂ��Ă���A�d����������A�C�e�����g�����Ƃɂ���čő�u6(+5)�v�܂ŏグ�邱�Ƃ��ł��܂��B\n");
		c = getchar();
		printf("8) �񕜃A�C�e���ɂ���\n  �񕜃A�C�e�����g���Ǝ�l���̔�J�x��10�񕜂��邱�Ƃ��ł��A�ő��5�܂ŃX�g�b�N���邱�Ƃ��ł��܂��B�Ȃ��A�񕜃A�C�e���̎g�p�͔C�ӂƂȂ��Ă��܂��B(�x�[�^�łł̓X�g�b�N�@�\�͎������Ă��Ȃ��̂ŁA�����Ȃ������Ƃ��ł��܂��B)\n");
		c = getchar();
		printf("9) �{�X�͓���2�̓o�ꂵ�A�����2�̂̃{�X�����Ԃɓ|���A����Ɉ�萔�d�������Ȃ����Ƃɂ���āA���X�{�X�ɒ��ނ��Ƃ��ł��܂��B���X�{�X��|���ƁA���Ȃ��̏����ƂȂ�܂��B�܂��A�{�X�ɂ���ďo���₷���肪����̂ŁA�����̃q���g�𗊂�ɂ��Ȃ��炤�܂��킢��i�߂܂��傤�B\n");
		c = getchar();
		printf("10) �{�X��ł͔C�ӂœ�����R�}���h����͂��邱�ƂŐ퓬���瓦���邱�Ƃ��ł��܂��B�퓬���ɉ񕜃A�C�e�����g�����Ƃ͂ł��Ȃ����߁A��J�x��100�ɋ߂Â��Ă����Ƃ��Ȃǂ́A��x�퓬���瓦���ĉ񕜃A�C�e�����g�p������A�����ǂ����̃}�X�ɂ���x�e���C�x���g�𗘗p���Ĕ�J�x���񕜂��܂��傤�B�A���A���l�Ƀ{�X�̗̑͂��������Ă��܂��܂��B \n");

		printf("\n�Q�[�����J�n���܂��B�����L�[�������Ă�������\n");
		c = getchar();
	}
	else if(userSelection == '3'){
		ranMode = 1;
		printf("\n�I�u�W�F�N�g�̔z�u�������_���ł��B�ʏ탂�[�h��菭�����ɂ₷�������E�E�E�H");
		printf("\n�Q�[�����J�n���܂��B�����L�[�������Ă�������\n");
		c = getchar();
	}

	//�`�J�n���Z���t�`

	printf("\n��l���u�������c�ƂŐh���Ȃ��c \n");
	printf("���낻��x�݂��~�������Ƃ肠�����ے��̂Ƃ���ɑ��k�ɍs�����B�v \n");
	printf("\n���Ȃ��̗͂ł��ЂƂ��L�x���l�����܂��傤!\n");
	c = getchar();
	printf("\n");
	printf("\n");
	printf("\n");
}

void showDungeon()
{
	int x, y;

	// ����������\������
	for (y = 1; y <= 5; y++)
	{

		// 1��ځF�k�h�A
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

		// 2��ځF���₷�����邽�߂̋󂫃X�y�[�X
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

		// 3��ځF���h�A�A��l���ƃ{�X�A���h�A
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

		// 4���: �q���g�Ƌx�e���ƃA�C�e���C�x���g
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


		// 5��ځF��h�A
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

	// �L���̐���
	printf("P = �v���C���[, B1 = �ے�, B2 = ����, LB = �В�, R = �x�e�� D = �h�A, * = �� \n");

}


// �����̏���\������
void showRoomInfo()
{
	int x = syujinko.roomX;
	int y = syujinko.roomY;

	// ���̕����ɏ�i���В������邱�Ƃ�\��
	if (dungeon[x][y].boss1 || dungeon[x][y].boss2)
	{
		printf("���̕����ɂ͏�i(�G)�����܂��B\n");
	}
	if (dungeon[x][y].lastBoss)
	{
		printf("���̕����ɂ͎В������܂��B\n");
	}
	// ���̕����ɋx�e��1�����邱�Ƃ�\��
	if (dungeon[x][y].rest1)
	{
		printf("�����͋x�e���ł��B��J�x�������邱�Ƃ��ł��܂��B\n");
	}
	//�x�e��2
	if (dungeon[x][y].rest2)
	{
		printf("�����͋x�e���ł��B��J�x�������邱�Ƃ��ł��܂��B\n");
	}
	// ���̕������A�C�e���擾�}�X�ł��邱�Ƃ�\��
	if (dungeon[x][y].items)
	{
		printf("���Ȃ��̓A�C�e���𔭌����܂���!\n");
	}
	// ���̕����Ƀq���g�����邱�Ƃ�\��
	if (dungeon[x][y].hints)
	{
		printf("���Ȃ��̓q���g�������܂���!\n");
	}
	// ���̕����̃h�A����\��
	if (dungeon[x][y].doorInfo[North] == openDoor)
	{
		printf(" ���̕����͖k���ɐi�߂܂��B ");
	}
	else if (dungeon[x][y].doorInfo[North] == noDoor)
	{
		printf(" ���̕����͖k���ɐi�߂܂���B ");
	}
	if (dungeon[x][y].doorInfo[East] == openDoor)
	{
		printf(" ���̕����͓����ɐi�߂܂��B\n");
	}
	else if (dungeon[x][y].doorInfo[East] == noDoor)
	{
		printf(" ���̕����͓����ɐi�߂܂���B\n");
	}
	if (dungeon[x][y].doorInfo[South] == openDoor)
	{
		printf(" ���̕����͓쑤�ɐi�߂܂��B ");
	}
	else if (dungeon[x][y].doorInfo[South] == noDoor)
	{
		printf(" ���̕����͓쑤�ɐi�߂܂���B ");
	}
	if (dungeon[x][y].doorInfo[West] == openDoor)
	{
		printf(" ���̕����͐����ɐi�߂܂��B\n");
	}
	else if (dungeon[x][y].doorInfo[West] == noDoor)
	{
		printf(" ���̕����͐����ɐi�߂܂���B\n");
	}
}

void showStatus()
{
	printf("--------------------------------------------------------------------------------");
	printf("                     ���݂̎�l���̃X�e�[�^�X ��J�x:%d          \n", syujinko.health);
	printf("                 �Ј�(�O�[):%d, �_��(�`���L):%d, ���J(�p�[):%d \n", syujinko.str1, syujinko.str2, syujinko.str3);
	printf("                    �񕜃A�C�e��������:%d, �d���J�E���^:%d    \n", syujinko.hasItems, syujinko.jobCounter);
	printf("--------------------------------------------------------------------------------\n");
}

// ���[�U�[����ړ���������͂��Ă��炤
int getUserMove()//����� �C��:�]��
{
	int direction = -1;
	int legalDirection = 0;
	char c;
	//char po[2];
	int x = syujinko.roomX;
	int y = syujinko.roomY;

	// ���������������͂���ĂȂ��܂ŌJ��Ԃ�
	while (!legalDirection)
	{
		// ���[�U�[������������
		printf("\n w,a,s,d�ňړ����܂��Bx�ŉ񕜃A�C�e���g�p(�������Ă���ꍇ�̂�): ");
		c = getchar();
		// ���s�������o�b�t�@�[����폜
		scanf_s("%*c");

		// ���͂͐��������ǂ������m�F
		switch (c)
		{
		case 'X':
		case 'x':
			if (syujinko.hasItems > 0) {
				if (syujinko.health > 0) {

					syujinko.health = syujinko.health - 10;
					syujinko.hasItems = syujinko.hasItems - 1;
					printf(" ->�񕜃A�C�e�����g�p���܂����B��J�x��10�������܂��B\n");
					showDungeon();
					showStatus();
					addItems++;
					break;
				}
				else {
					printf("��J�x�͂���ȏ�񕜂��܂���B\n");
					c = getchar();
					showDungeon();
					showStatus();
					break;
				}
			}
			else {
				printf(" ->�A�C�e���������Ă��܂���B\n ");
				break;
			}

		case 'W':
		case 'w':
			// �k�����Ƀh�A������Ε�������
			if (dungeon[x][y].doorInfo[North] == openDoor)
			{
				direction = North;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[North] == noDoor)
			{
				printf("�������͍s���~�܂�ł��B\n");
			}
			//�{�X2�����镔��
			else if (dungeon[x][y].doorInfo[North] == dungeon[x][y].boss2)
				//boss2���needJobs1���ȁH
			{
				if (syujinko.jobCounter == 2)
				{
					direction = North;
					legalDirection = 1;
				}
				else
				{
					printf("�d����2�񂱂Ȃ����ƂŊJ���܂��B\n");
				}
			}
			//���X�{�X�����镔��
			else if (dungeon[x][y].doorInfo[North] == dungeon[x][y].lastBoss)
				//lastboss���needJobs2���ȁH
			{
				if (syujinko.jobCounter == 5)
				{
					direction = North;
					legalDirection = 1;
				}
				else
				{
					printf("�d����5�񂱂Ȃ����ƂŊJ���܂��B\n");
				}
			}
			/*	  else
			{
			printf( "�������͍s���~�܂�ł��B\n" );
			} */
			break;

		case 'D':
		case 'd':
			// �������Ƀh�A������Ε�������
			if (dungeon[x][y].doorInfo[East] == openDoor)
			{
				direction = East;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[East] == noDoor)
			{
				printf("�������͍s���~�܂�ł��B\n");
			}
			//�{�X2�����镔���B�K�v�d�����Q
			else if (dungeon[x][y].doorInfo[East] == dungeon[x][y].boss2)
				//boss2���needJobs1���ȁH
			{
				if (syujinko.jobCounter == 2)
				{
					direction = East;
					legalDirection = 1;
				}
				else
				{
					printf("�d����2�񂱂Ȃ����ƂŊJ���܂��B\n");
				}
			}
			//���X�{�X�����镔���B�K�v�d�����T
			else if (dungeon[x][y].doorInfo[East] == dungeon[x][y].lastBoss)
				//lastboss���needJobs2���ȁH
			{
				if (syujinko.jobCounter == 5)
				{
					direction = East;
					legalDirection = 1;
				}
				else
				{
					printf("�d����5�񂱂Ȃ����ƂŊJ���܂��B\n");
				}
			}
			/*	  else
			{
			printf( "�������͍s���~�܂�ł��B\n" );
			}*/
			break;

		case 'S':
		case 's':
			// ������Ƀh�A������Ε�������
			if (dungeon[x][y].doorInfo[South] == openDoor)
			{
				direction = South;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[South] == noDoor)
			{
				printf("�������͍s���~�܂�ł��B\n");
			}
			//�{�X2�����镔���B�K�v�d�����Q
			else if (dungeon[x][y].doorInfo[South] == dungeon[x][y].boss2)
				//boss2���needJobs1���ȁH
			{
				if (syujinko.jobCounter == 2)
				{
					direction = South;
					legalDirection = 1;
				}
				else
				{
					printf("�d����2�񂱂Ȃ����ƂŊJ���܂��B\n");
				}
			}
			//���X�{�X�����镔���B�K�v�d�����T
			else if (dungeon[x][y].doorInfo[South] == dungeon[x][y].lastBoss)
				//lastboss���needJobs2���ȁH
			{
				if (syujinko.jobCounter == 5)
				{
					direction = South;
					legalDirection = 1;
				}
				else
				{
					printf(" �d����5�񂱂Ȃ����ƂŊJ���܂��B\n ");
				}
			}
			/*	  else
			{
			printf( " �������͍s���~�܂�ł��B\n " );
			}*/
			break;

		case 'A':
		case 'a':
			// �������Ƀh�A������Ε�������
			if (dungeon[x][y].doorInfo[West] == openDoor)
			{
				direction = West;
				legalDirection = 1;
			}
			else if (dungeon[x][y].doorInfo[West] == noDoor)
			{
				printf("�������͍s���~�܂�ł��B\n");
			}
			//�{�X2�����镔���B�K�v�d�����Q
			else if (dungeon[x][y].doorInfo[West] == dungeon[x][y].boss2)
				//boss2���needJobs1���ȁH
			{
				if (syujinko.jobCounter == 2)
				{
					direction = West;
					legalDirection = 1;
				}
				else
				{
					printf(" �d����2�񂱂Ȃ����ƂŊJ���܂��B\n ");
				}
			}
			//���X�{�X�����镔���B�K�v�d�����T
			else if (dungeon[x][y].doorInfo[West] == dungeon[x][y].lastBoss)
				//lastboss���needJobs2���ȁH
			{
				if (syujinko.jobCounter == 5)
				{
					direction = West;
					legalDirection = 1;
				}
				else
				{
					printf(" �d����5�񂱂Ȃ����ƂŊJ���܂��B\n ");
				}
			}
			/*	  else
			{
			printf(" �������͍s���~�܂�ł��B\n ");
			}*/
			break;
		default:
			// �s������
			printf(" �i�ޕ�������͂��Ă��������B\n ");
			c = getchar();
			break;
		}
	}
	return direction;
}

// ���[�U���I�񂾕����Ɏ�l�����ړ�������
void moveSyujinko(int direction)//����� �C��:�]��
{
	int currentX = syujinko.roomX;
	int currentY = syujinko.roomY;
	int newX, newY;

	// ��l�������̕����������
	dungeon[currentX][currentY].syujinko = 0;

	//�@�V����������X����Y�����������l��
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

	// �V�����ꏊ�̏���ǉ�
	syujinko.roomX = newX;
	syujinko.roomY = newY;
	dungeon[newX][newY].syujinko = 1;
	dungeon[newX][newY].syujinkoVisited = 1;

}

void printText() {

	if (checkPrintText == 1) {
		printf("\nxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("x���ɉ����Ȃ�����....x\n");
		printf("xxxxxxxxxxxxxxxxxxxxxx\n");
		checkPrintText = 0;
	}

}

void itemDice() //�c��
{
	char c;
	int random;
	srand((unsigned)time(NULL));
	random = rand() % 6 + 1;

	switch (random) {
	case 1:
	case 2:
	case 3: {
		printf(" *�񕜃A�C�e������ɓ��ꂽ�B*\n ");
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
			printf("->����ȏ�X�e�[�^�X�͏オ��܂���B\n");
			syujinko.str1 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *�Ј����{����ɓ��ꂽ�B�Ј��X�e�[�^�X���㏸���܂��B*\n ");
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
			printf("����ȏ�X�e�[�^�X�͏オ��܂���B\n");
			syujinko.str2 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *�_�����{����ɓ��ꂽ�B�_���X�e�[�^�X���㏸���܂��B*\n ");
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
			printf("����ȏ�X�e�[�^�X�͏オ��܂���B\n");
			syujinko.str3 == 6;
			c = getchar();
			printf("\n");
			printf("\n");
			printf("\n");
		}
		else {
			printf(" *���J���{����ɓ��ꂽ�B���J�X�e�[�^�X���㏸���܂��B*\n ");
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

int randomDice(void)//�c�� 
{
	int random = 0;
	int x;
	char y[4];

	random = rand() % 7 + 1;

	switch (random) {
	case 1:
	case 2: {
		printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		printf("x �d�������B��J�x��10�������A�d���J�E���^��1�㏸�����܂��Bx\n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

		syujinko.health += 10;
		syujinko.jobCounter += 1;

		if (syujinko.health >= 100) {
			syujinkoDead = 1;
			break;
		}
		if (syujinko.str1 == 6 && syujinko.str2 == 6 && syujinko.str3 == 6)
		{
			printf("->�S�X�e�[�^�X�ő�l�ł��B\n");
			break;

		}

		printf("->�C�ӂ̃X�e�[�^�X���㏸�����邱�Ƃ��o���܂��B�Y���̐����L�[�������Č��肵�Ă��������B\n");
		printf("1.�Ј�, 2.�_��, 3.���J : ");

		fgets(y, 4, stdin);
		x = atoi(y);

		switch (x) {

		case 1:
			if (syujinko.str1 == 6)
			{
				printf("->����ȏ�Ј��X�e�[�^�X�͏オ��܂���B\n");
				syujinko.str1 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str2 == 6 && syujinko.str3 < 6)
				{
					printf("->���J�X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str2 == 6;
					syujinko.str3 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");

				}
				else if (syujinko.str3 == 6 && syujinko.str2 < 6)
				{
					printf("->�ϗ��X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str3 == 6;
					syujinko.str2 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");

				}
				else if (syujinko.str2 < 6 || syujinko.str3 < 6)
				{
					printf("2.�_��, 3.���J�F");
					scanf_s("%d", &x);
					switch (x)
					{
					case 2:
						printf("->�_���X�e�[�^�X���㏸�����܂����B\n");
						syujinko.str2 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 3:
						printf("->���J�X�e�[�^�X���㏸�����܂����B\n");
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
					printf("->����ȏ�X�e�[�^�X�͏オ��܂���B\n");
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
				printf("->�Ј��X�e�[�^�X���㏸�����܂����B\n");
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
				printf("->����ȏ�_���X�e�[�^�X�͏オ��܂���B\n");
				syujinko.str2 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str1 == 6 && syujinko.str3 < 6)
				{
					printf("->���J�X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str1 == 6;
					syujinko.str3 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str3 == 6 && syujinko.str1 < 6)
				{
					printf("->�Ј��X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str3 == 6;
					syujinko.str1 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str1 < 6 || syujinko.str3 < 6)
				{
					printf("1.�Ј�, 3.���J�F");
					scanf_s("%d", &x);
					switch (x)
					{
					case 1:
						printf("->�Ј��X�e�[�^�X���㏸�����܂����B\n");
						syujinko.str1 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 3:
						printf("->���J�X�e�[�^�X���㏸�����܂����B\n");
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
					printf("->����ȏ�X�e�[�^�X�͏オ��܂���B\n");
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
				printf("->�_���X�e�[�^�X���㏸�����܂����B\n");
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
				printf("->����ȏ㒚�J�X�e�[�^�X�͏オ��܂���B\n");
				syujinko.str3 == 6;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");

				if (syujinko.str1 == 6 && syujinko.str2 < 6)
				{
					printf("->�_���X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str1 == 6;
					syujinko.str2 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str2 == 6 && syujinko.str1 < 6)
				{
					printf("->�Ј��X�e�[�^�X���㏸�����܂����B\n");
					syujinko.str2 == 6;
					syujinko.str1 += 1;
					x = getchar();
					printf("\n");
					printf("\n");
					printf("\n");
				}
				else if (syujinko.str1 < 6 || syujinko.str2 < 6)
				{
					printf("1.�Ј�, 2.���J�F");
					scanf_s("%d", &x);
					switch (x)
					{
					case 1:
						printf("->�Ј��X�e�[�^�X���㏸�����܂����B\n");
						syujinko.str1 += 1;
						x = getchar();
						printf("\n");
						printf("\n");
						printf("\n");
						break;
					case 2:
						printf("->�_���X�e�[�^�X���㏸�����܂����B\n");
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
					printf("->����ȏ�X�e�[�^�X�͏オ��܂���B\n");
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
				printf("->���J�X�e�[�^�X���㏸�����܂����B\n");
				syujinko.str3 += 1;
				x = getchar();
				printf("\n");
				printf("\n");
				printf("\n");
			}
			break;

		default:
			printf("->�����ȏ����ł��B\n");
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
		printf(" *�A�C�e���C�x���g�����I�A�C�e���̒��I�����܂��B*\n ");
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

void tryUseRestRoom(void) //�]��@�C��:�c��
{
	int x = syujinko.roomX;
	int y = syujinko.roomY;
	int choose1 = 0;
	int choose2 = 0;
	char z[3];
	char v[3];

	//�x�e��1

	if (dungeon[x][y].rest1) {
		printf("\n **�x�e���𔭌�����!**\n");
		printf(" �x�e���𗘗p���܂��� ? (��J�x30����)�e�x�e������2��܂Ŏg�p�ł��܂��B\n ");
		printf(" ���p���Ȃ������ꍇ�ɂ́A���Ƀ}�X�ɓ������ۂɂ�����x�g�����Ƃ��ł��܂��B\n");
		printf(" \n���p����ꍇ�ɂ�1���A���Ȃ��ꍇ�ɂ�2����͂��Ă��������B\n ");
		fgets(z, 3, stdin);
		choose1 = atoi(z);
		if (choose1 == 1) {
			if (syujinko.health < 30) {
				syujinko.health = 0;
				dungeon[x][y].rest1 -= 1;
				printf(" ->�x�e���𗘗p���܂����B��J�x���������܂��B\n ");
				printf(" ���݂̔�J�x: %d", syujinko.health);
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
				printf(" ->�x�e���𗘗p���܂����B��J�x���������܂��B\n ");
				printf(" ���݂̔�J�x: %d", syujinko.health);
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
			printf(" ->�x�e���𗘗p���܂���ł����B\n ");
			choose1 = getchar();
			printf("\n");



		}
		else {
			printf(" ->�����ȓ��͂ł��B\n\n ");
			choose1 = getchar();
			printf("\n");
		}
		showDungeon();
		showStatus();

	}


	if (dungeon[x][y].rest2) {
		printf("\n **�x�e���𔭌�����!**\n");
		printf(" �x�e���𗘗p���܂��� ? (��J�x30����)�e�x�e������2��܂Ŏg�p�ł��܂��B\n ");
		printf(" ���p���Ȃ������ꍇ�ɂ́A���Ƀ}�X�ɓ������ۂɂ�����x�g�����Ƃ��ł��܂��B\n");
		printf(" \n���p����ꍇ�ɂ�1���A���Ȃ��ꍇ�ɂ�2����͂��Ă��������B\n ");
		fgets(v, 3, stdin);
		choose2 = atoi(v);
		if (choose2 == 1) {
			if (syujinko.health < 30) {
				syujinko.health = 0;
				dungeon[x][y].rest2 -= 1;
				printf(" ->�x�e���𗘗p���܂����B��J�x���������܂��B\n ");
				printf(" ���݂̔�J�x: %d", syujinko.health);
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
				printf(" ->�x�e���𗘗p���܂����B��J�x���������܂��B\n ");
				printf(" ���݂̔�J�x: %d", syujinko.health);
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
			printf(" ->�x�e���𗘗p���܂���ł����B\n ");
			choose2 = getchar();
			printf("\n");

		}
		else {
			printf(" ->�����ȓ��͂ł��B�Ƃ肠�������p���܂���B\n\n ");
			choose2 = getchar();
			printf("\n");
		}
		showDungeon();
		showRoomInfo();
		showStatus();
	}
}

void checkBossFight(void) //�S��:�c��, �䎌:����
{
	/////////////////1�Ԃ߂̂ڂ�/////////////////
	int x = syujinko.roomX;
	int y = syujinko.roomY;
	int result;
	char c;

	if (dungeon[x][y].boss1)
	{
		//�{�X�䎌��W AA�Ƃ�����Ă�������������Ȃ�
		//�퓬�O
		printf("\n[�ے��̃f�X�N]\n");
		printf("��l���u�ے�!�����̂Ƃ��낸���Ǝc�Ƒ����Ȃ̂ŗL���x�ɂ���肽���ł�!!�v\n");
		c = getchar();

		printf("\n �P�P�P�P�P�P�P�P�P�P�c�� �~ �P�P�P�P�P�P�P�P�P�P�P\n");
		printf("          �\�\�\�\ ��(�L��֥`) �@�@�@�@�@�@�P�P�P�P\n");
		printf("�P�P�P�@�@�\�\�\�\�] / ��_�m �@�@�@�\�\�\�Q�Q�@�@�@�@�\�\\n");
		printf("    �\�\�@�@�@�@�@. / �^�ܿ \n");
		printf("       �P�P�@�@�@  -'�L �@�@�@�@�@�P�P�P \n");
		printf("�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P\n");
		printf(" \n�ے��u���̈ꑶ�ł͌��߂��܂���ˁB\n");
		printf(" ���Ƃ̐킢�ɏ�������Ε����ɐ��E���Ă����܂���!�v\n");
		printf("\n�L�[�������Đ퓬�ɓ���܂�!\n");
		c = getchar();
		result = boss1Fight(boss1HP);

		if (result == syujinkoWins)
		{
			// ���ɑ䎌
			printf("\n�ے��u�N�̘b�͂悭������܂����B\n");
			printf("�����͕������ɋ���̂ł����Ŕނ��������Ƃ����ł��傤�B\n");
			printf("���������͎��т̂Ȃ��Ј����������Ă���̂�\n");
			printf("�������d�������Ȃ��Ă���̕���������������܂���B�v\n");
			printf("\n �P�P�P�P�P�P�P�P�P�P�c�� �~ �P�P�P�P�P�P�P�P�P�P�P\n");
			printf("          �\�\�\�\ ��(�LX��X`) �@�@�@�@�@�@�P�P�P�P\n");
			printf("�P�P�P�@�@�\�\�\�\�] / ��_�m �@�@�@�\�\�\�Q�Q�@�@�@�@�\�\\n");
			printf("    �\�\�@�@�@�@�@. / �^�ܿ \n");
			printf("       �P�P�@�@�@  -'�L �@�@�@�@�@�P�P�P \n");
			printf("�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P\n");
			printf("\n��l���u����ȂƂ���Ɏ����ꂪ�����Ă�c�v\n");
			printf("****************************************\n");
			printf("*                �q���g                *\n");
			printf("*          �����͂��Ȃ�C���Z��        *\n");
			printf("*         �����{���Ă��邩��         *\n");
			printf("*           �F�Ɍ����Ă���       �@  *\n");
			printf("*                                      *\n");
			printf("****************************************\n");
			boss1Clear = 1;
			dungeon[x][y].boss1 = 0;

			printf("�L�[�������ĒT����ʂ֖߂�܂��B\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\n�{�X�킩��P�ނ��܂���...\n");
		}
		else {
			// �{�X�ɕ������̔s�k�����B��
			syujinkoDead = 1;
		}
	}

	//****************��������boss2*****************

	if (dungeon[x][y].boss2 && boss1Clear == 1)
	{
		printf("\n[�����̕���]\n");
		printf("��l���u�ے�����b�𕷂��Ă��邩������܂��񂪗L���x�ɂ̘b�����ɗ��܂����B�v\n");
		c = getchar();


		printf("				 �j��j\n");
		printf("				��Ɂ@Ɂi., �)\n");
		printf("		         �i� - ..::.::..�i\n");
		printf("			    (�\..�c�܃~.�j�)\n");
		printf("			�j.::'; (�L��֥`) �@ ( \n");
		printf("		  ��@....::; ';'�i�� ���j::; ';'`�`�. \n");
		printf("			(:; ';'  |_��_| ::; ';' �@�R�j\n");
		printf("     '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");

		printf(" \n�����u���O���Љ�l�̂����ɋx�݂�~�������Ă�z��!!\n");
		printf(" ��x�Ƃ���ȍl�����N�����Ȃ��悤�ɉ����Љ�̌������������Ă��!!�v\n");
		printf("�L�[�������Đ퓬�ɓ���܂��I\n");
		c = getchar();
		result = boss2Fight(boss2HP);

		if (result == syujinkoWins)
		{
			// ���ɑ䎌
			printf("��l���u�����Ƃ͂܂Ƃ��Șb�ɂȂ�Ȃ������c\n");
			printf("���͕��В��Ƙb�����Ȃ��ƂȁB�v\n");

			printf("\n				 �c�܃~\n");
			printf("				 (�LX��X�M)\n");
			printf("				| �P�P�P |\n");
			printf("				| ���� |\n");
			printf("				| �Q�Q�Q |\n");
			printf("				��������\n");

			printf("\n��l���u��A�܂����؂ꂪ�����Ă�ȁB�v\n");

			printf("*******************************************\n");
			printf("*                                         *\n");
			printf("*          ���В��͕����Ǝ��̌Z��         *\n");
			printf("*             �Z��Ƃ������Ƃ�            *\n");
			printf("*           �����������Ƃ���...           *\n");
			printf("*                                         *\n");
			printf("*******************************************\n");
			boss2Clear = 1;
			dungeon[x][y].boss2 = 0;

			printf("�L�[�������ĒT����ʂ֖߂�܂��B\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\n�{�X�킩��P�ނ��܂���...\n");
		}
		else {
			// �{�X�ɕ������̔s�k�����B��
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].boss2) {
		//�d�����𖞂����Ă��Ȃ�
		printf("\n[�����̕���]\n");
		printf("��l���u�ے�����b�𕷂��Ă��邩������܂��񂪗L���x�ɂ̘b�����ɗ��܂����B�v\n");
		c = getchar();
		printf("�����u���̒��x�̎��тł킴�킴�����̂��c�@\n");
		printf("�����Ǝ��т�ς�ŏo�����ė����I�v\n");
		printf("->�܂����ނɂ͑����悤���E�E�E���̃{�X��|���Ă��Ȃ����A�d����������ĂȂ��B\n");

		c = getchar();
		showDungeon();
		showRoomInfo();
	}

	//////////////���В�///////////////

	if (dungeon[x][y].boss3 && boss2Clear == 1)
	{
		printf("\n[���В��̕���]\n");
		printf("��l���u�������ǂ����Ă���������Ă���Ȃ����߁A�L���x�ɂ̘b�����ɗ��܂����B�v\n");
		c = getchar();

		printf("                 , � \n");
		printf(" �P�@-- = �Q�@�@ / |            --'' \n");
		printf("     , , , , r�]��Ƀm�@ �i�_, �ɁT�@�@�@�@�@ - �P\n");
		printf("          �l               ޤ_ \n");
		printf("         .j�L.�c�܃~�@�@�@�i. \n");
		printf("�� �@�@��{ (�L�E�ցE�M) / ���@�@�@�Q�@�@�@�@�@�� \n");
		printf("         ).c /      ,�� �@ ,l~ \n");
		printf("      �Ly   �o ,�A�o�@ �@ < \n");
		printf("        �T   l� �R,)�@�@ , \n");
		printf(" \n���В��u�o�b�J���[�[�[�[�[�[��!!\n");
		printf(" �L�x�Ȃ񂼉����[�ɂ͕K�v�����!!�v\n");
		printf("�L�[�������Đ퓬�ɓ���܂��I\n");
		c = getchar();
		result = boss3Fight(boss3HP);

		if (result == syujinkoWins)
		{

			printf("��l���u���В��Ƃ��܂Ƃ��Șb�����邱�Ƃ͏o���Ȃ������ȁc\n");
			printf("�����Ȃ����璼�ڎВ��Ƙb�����Ȃ��ƂȁB�v\n");

			printf("\n  �c�܃~        �c�܃~\n");
			printf("   (�LX��X�M)    (�LX��X�M)\n");
			printf("   | �P�P�P |    | �P�P�P |\n");
			printf("   | �P�P�P |    | ���� |\n");
			printf("   | �P�P�P |    | �Q�Q�Q |\n");
			printf("    ��������       ��������\n");

			printf("\n��l���u��A�܂����؂ꂪ�����Ă�ȁB�v\n");

			printf("*******************************************\n");
			printf("*                                         *\n");
			printf("*      �В��͒��g�͂Ƃ������O�ʂ͂���     *\n");
			printf("*            �����r�����肵�Ȃ���         *\n");
			printf("*             ���J�Șb����������          *\n");
			printf("*                                         *\n");
			printf("*******************************************\n");
			boss3Clear = 1;
			dungeon[x][y].boss3 = 0;

			printf("�L�[�������ĒT����ʂ֖߂�܂��B\n");
			c = getchar();
			showDungeon();
		}
		else if (result == escape) {
			printf("\n�{�X�킩��P�ނ��܂���...\n");
		}
		else {
			// �{�X�ɕ������̔s�k�����B��
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].boss3) {
		//�d�����𖞂����Ă��Ȃ�
		printf("\n[���В��̕���]\n");
		printf("��l���u�������ǂ����Ă���������Ă���Ȃ����߁A�L���x�ɂ̘b�����ɗ��܂����B\n");
		c = getchar();
		printf("���В��u�܂��܂��d�����c���Ă���ł͂Ȃ����I�@\n");
		printf("�������Ǝ��т�ς�ŗ��񂩁I�v\n");
		printf("->�܂����ނɂ͑����悤���E�E�E���̃{�X��|���Ă��Ȃ����A�d����������ĂȂ��B\n");
		c = getchar();
		showDungeon();
		showRoomInfo();
	}

	//***********�������烉�X�{�X************

	if (dungeon[x][y].lastBoss && boss3Clear == 1)
	{
		printf("\n[�В���]\n");
		printf("�В��u�N���K����m��Ȃ��Ј��ł���\n");
		printf("�ǂ��ł����H�d���͂����𓾂邽�߂����ɂ����̂ł͂Ȃ��ł���\n");
		printf("������������\"���肪�Ƃ�\"���A���Ă��܂��B���̊��ӂ̌��t�������\n");
		printf("�x�݂ȂǂȂ��Ă��ꐶ�����������Ƃ��o���܂��B�v\n");
		c = getchar();


		printf("	      	*'``�E* �B \n");
		printf("			| `*�B\n");
		printf(".		,  ,., ����c �� �~�@* �@�@�@\n");
		printf("			+ (�L��֥`)�@*� + � \n");
		printf("			`*� �R�A�@ �� *�* \n");
		printf("			`�E + �*�E' ߁� +� \n");
		printf("			���@�@ ��~�*� \n");
		printf("				`�E + �*�E � \n");

		printf("�u���Ȃ��ɂ͒��ڎw�����Ă����܂����犴�ӂ��Ă��������ˁB�v\n");

		printf("\n�L�[�������Đ퓬�ɓ���܂��I\n");
		c = getchar();
		result = lastBossFight(lastBossHP);

		if (result == syujinkoWins)
		{
			//�퓬��
			if (syujinko.jobCounter == 5)
			{
				printf("�В��u�d����5�񂱂Ȃ��ė��܂������B\n");
			}
			else if (syujinko.jobCounter > 5)
			{
				printf("�В��u�d����%d������Ȃ��ė��܂������I\n", syujinko.jobCounter);
			}
			else if (syujinko.jobCounter > 10)
			{
				printf("�В��u�Ȃ�ƁI�@�d����%d������Ȃ��ė����̂ł����I�I\n", syujinko.jobCounter);
			}

			printf("�Ȃ�قǁc���Ȃ��̔M�ӂ͓`���܂����B\n");
			printf("�l�Ԃ͓K�x�ȋx�݂����邩�炱���d�����撣�邱�Ƃ��ł���c\n");
			printf("���ꂩ��݂͂�Ȃ��K���ɂȂ���Ђ�ڎw���܂��B\n");
			c = getchar();

			if (syujinko.jobCounter == 5)
			{
				printf("���Ȃ��̔M�ӂɉ����A�L�x��2���^���܂��傤�B�v\n");
			}
			else if (syujinko.jobCounter > 5)
			{
				printf("�悭����Ȑ��̎d�������Ȃ��Ă���܂����B\n");
				printf("���Ȃ��ɂ͗L�x��1�T�ԗ^���܂��傤�B�v\n");
			}
			else if (syujinko.jobCounter > 10)
			{
				printf("���܂������̎d�����悭1�l�ł��Ȃ��Ă���܂����B\n");
				printf("����ŉ䂪�Ђ̋Ɩ������~���ɐi�ނ��Ƃł��傤�B\n");
				printf("���Ȃ��ɂ�2�T�Ԃ̗L�x��^���܂��傤�B�v\n");
			}
			bossDead = 1;

			// ���X�{�X���폜
			dungeon[x][y].lastBoss = 0;
		}
		else if (result == escape) {
			printf("\n�{�X�킩��P�ނ��܂���...\n");
		}
		else {
			// �{�X�ɕ������̔s�k�����B��
			syujinkoDead = 1;
		}
	}
	else if (dungeon[x][y].lastBoss) {
		printf("\n[�В����O]\n");
		printf("��l���u�В����̑O�܂ŗ�������ǂ��c\n");
		printf("�������ɂ��̒��x�̎��т���b�𕷂��Ă���Ȃ����낤��\n");
		printf("�����Ǝd�������Ȃ��Ă���o�������B�v\n");
		printf(" ->�В��ɒ��ނɂ͂܂������悤���E�E�E���̃{�X�̌��j�����d����������ĂȂ��B\n ");
		c = getchar();
		showDungeon();
		showRoomInfo();
	}
}

int boss1Fight(int boss1HP)//�c��
{
	int player, com = 0;
	char c;
	boss1HP = fightBoss1HP;
	printf("�ŏ��̃{�X�Ƃ̐퓬���I\n");
	srand((unsigned)time(NULL));

	while (1) //�ǂ��������ʂ܂ł���񂯂񂷂�!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 ��l���̔�J�x: %d , �{�X1��HP: %d  \n", syujinko.health, boss1HP);
		printf("                       �Ј�:%d, �_��:%d, ���J:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n�o�����I��łˁI 1:�Ј�(�O�[), 2:�_��(�`���L), 3:���J(�p�[) \n ");
		printf("���������ꍇ��4��I��ł�!\n");
		scanf_s("%d", &player);

		com = rand() % 3 + 1;

		switch (player) {
		case 1:
			if (player == com) {
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n");
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
			//�P��
			player = getchar();
			goto OUT;
		}
		default:
			printf("�����Ȏ肾��\n");
			player = getchar();
			break;
		}
		// �{�X��|�������ǂ����̃`�F�b�N
		if (boss1HP <= 0) {
			printf("\n**�ے���|����!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// ��l�������񂾂��ǂ����̃`�F�b�N
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ���肦�Ȃ����������邱�ƂłƂ肠����GOTO����
		if (boss1HP == 400) {
		OUT:
			fightBoss1HP = boss1HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int boss2Fight(int boss2HP) //�c��
{
	int player, com = 0;
	char c;
	boss2HP = fightBoss2HP;
	printf("2�Ԗڂ̃{�X�Ƃ̐퓬���I\n");
	srand((unsigned)time(NULL));

	while (1) //�ǂ��������ʂ܂ł���񂯂񂷂�!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 ��l���̔�J�x: %d , �{�X2��HP: %d  \n", syujinko.health, boss2HP);
		printf("                       �Ј�:%d, �_��:%d, ���J:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n�o�����I��łˁI 1:�Ј�(�O�[), 2:�_��(�`���L), 3:���J(�p�[) \n ");
		printf("���������ꍇ��4��I��ł�!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com || com == 4) {
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n");
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
			//�P��
			player = getchar();
			goto OUT;
		}
		default:
			printf("�����Ȏ肾��\n");
			player = getchar();
			break;
		}
		// �{�X��|�������ǂ����̃`�F�b�N
		if (boss2HP <= 0) {
			printf("\n**�{�X��|����!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// ��l�������񂾂��ǂ����̃`�F�b�N
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ���肦�Ȃ����������邱�ƂłƂ肠����GOTO����
		if (boss2HP == 400) {
		OUT:
			fightBoss2HP = boss2HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int boss3Fight(int boss3HP) //���
{
	int player, com = 0;
	char c;
	boss3HP = fightBoss3HP;
	printf("3�Ԗڂ̃{�X�Ƃ̐퓬���I\n");
	srand((unsigned)time(NULL));

	while (1) //�ǂ��������ʂ܂ł���񂯂񂷂�!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 ��l���̔�J�x: %d , �{�X3��HP: %d  \n", syujinko.health, boss3HP);
		printf("                       �Ј�:%d, �_��:%d, ���J:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n�o�����I��łˁI 1:�Ј�(�O�[), 2:�_��(�`���L), 3:���J(�p�[) \n ");
		printf("���������ꍇ��4��I��ł�!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com || com == 4) {
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n");
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
			//�P��
			player = getchar();
			goto OUT;
		}
		default:
			printf("�����Ȏ肾��\n");
			player = getchar();
			break;
		}
		// �{�X��|�������ǂ����̃`�F�b�N
		if (boss3HP <= 0) {
			printf("\n**�{�X��|����!**\n");
			c = getchar();
			return syujinkoWins;
		}
		// ��l�������񂾂��ǂ����̃`�F�b�N
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ���肦�Ȃ����������邱�ƂłƂ肠����GOTO����
		if (boss3HP == 400) {
		OUT:
			fightBoss3HP = boss3HP;
			showDungeon();
			return escape;
		}
	}
	return 0;
}

int lastBossFight(int lastBossHP) //�c��
{
	int player, com = 0;
	char c;
	lastBossHP = fightLastBossHP;
	printf("\n **���X�{�X�Ƃ̐퓬���I**\n");
	srand((unsigned)time(NULL));

	while (1) //�ǂ��������ʂ܂ł���񂯂񂷂�!!
	{
		printf("\n--------------------------------------------------------------------------------");
		printf("                 ��l���̔�J�x: %d , ���X�{�X��HP: %d  \n", syujinko.health, lastBossHP);
		printf("                       �Ј�:%d, �_��:%d, ���J:%d        \n", syujinko.str1, syujinko.str2, syujinko.str3);
		printf("--------------------------------------------------------------------------------");
		printf("\n�o�����I��łˁI 1:�Ј�(�O�[), 2:�_��(�`���L), 3:���J(�p�[) \n ");
		printf("���������ꍇ��4��I��ł�!\n");
		scanf_s("%d", &player);

		com = rand() % 4 + 1;

		switch (player) {
		case 1:
			if (player == com) {
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n ");
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
				printf("\n\n\n���������I\n");
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
			//�P��
			player = getchar();
			goto OUT;
		}
		default:
			printf("�����Ȏ肾��\n");
			player = getchar();
			break;
		}
		// �{�X��|�������ǂ����̃`�F�b�N
		if (lastBossHP <= 0) {
			printf("\n**�{�X��|����!**\n");
			return syujinkoWins;
		}
		// ��l�������񂾂��ǂ����̃`�F�b�N
		if (syujinko.health >= 100) {
			return syujinkoLoses;
		}
		// ���肦�Ȃ����������邱�ƂłƂ肠����GOTO����
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
		printf("\n �P�P�P�P�P�P�P�P�P�P�c�� �~ �P�P�P�P�P�P�P�P�P�P�P\n");
		printf("          �\�\�\�\ ��(`��֥�L) �@�@�@�@�@�@�P�P�P�P\n");
		printf("�P�P�P�@�@�\�\�\�\�] / ��_�m �@�@�@�\�\�\�Q�Q�@�@�@�@�\�\\n");
		printf("    �\�\�@�@�@�@�@. / �^�ܿ \n");
		printf("       �P�P�@�@�@  -'�L �@�@�@�@�@�P�P�P \n");
		printf("�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P\n");
		printf("\n �u�Ȃ��Ȃ��r�������܂����ːV�l�N...!�v\n");
		printf("�U���������I\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\n �P�P�P�P�P�P�P�P�P�P�c�� �~ �P�P�P�P�P�P�P�P�P�P�P\n");
		printf("          �\�\�\�\ ��(�L��֥`) �@�@�@�@�@�@�P�P�P�P\n");
		printf("�P�P�P�@�@�\�\�\�\�] / ��_�m �@�@�@�\�\�\�Q�Q�@�@�@�@�\�\\n");
		printf("    �\�\�@�@�@�@�@. / �^�ܿ \n");
		printf("       �P�P�@�@�@  -'�L �@�@�@�@�@�P�P�P \n");
		printf("�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P\n");
		printf("\n �u�܂��܂��Ⴂ���̂ɂ͕����܂����...�I�v\n");
		printf("�U���������I\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("\n �P�P�P�P�P�P�P�P�P�P�c�� �~ �P�P�P�P�P�P�P�P�P�P�P\n");
		printf("          �\�\�\�\ ��(`��֥�L) �@�@�@�@�@�@�P�P�P�P\n");
		printf("�P�P�P�@�@�\�\�\�\�] / ��_�m �@�@�@�\�\�\�Q�Q�@�@�@�@�\�\\n");
		printf("    �\�\�@�@�@�@�@. / �^�ܿ \n");
		printf("       �P�P�@�@�@  -'�L �@�@�@�@�@�P�P�P \n");
		printf("�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P�P\n");
		printf("\n �u�N�͑啨�ɂȂ�....����ȋC������ȁI�v\n");
		printf("�U���������I\n");
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
		printf("				 �j��j\n");
		printf("				��Ɂ@Ɂi., �)\n");
		printf("		         �i� - ..::.::..�i\n");
		printf("			    (�\..�c�܃~.�j�)\n");
		printf("			�j.::'; (`��֥�L) �@ ( \n");
		printf("		  ��@....::; ';'�i�� ���j::; ';'`�`�. \n");
		printf("			(:; ';'  |_��_| ::; ';' �@�R�j\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n�u�V�l�@������i�ɋt����Ă����̂��I�v\n");
		printf("�U���������I...�������i�Ƃ͂��������E�U����Ȃ�...�B\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("				 �j��j\n");
		printf("				��Ɂ@Ɂi., �)\n");
		printf("		         �i� - ..::.::..�i\n");
		printf("			    (�\..�c�܃~.�j�)\n");
		printf("			�j.::'; (�L��֥`) �@ ( \n");
		printf("		  ��@....::; ';'�i�� ���j::; ';'`�`�. \n");
		printf("			(:; ';'  |_��_| ::; ';' �@�R�j\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n�u�V�l�̂����ɐ��ӋC�Ȃ��Ƃ������ȁI�v\n");
		printf("�U���������I�������Ȃ�ł���ȓz�������Ȃ񂾂낤��...�B\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("				 �j��j\n");
		printf("				��Ɂ@Ɂi., �)\n");
		printf("		         �i� - ..::.::..�i\n");
		printf("			    (�\..�c�܃~.�j�)\n");
		printf("			�j.::'; (�L��֥`) �@ ( \n");
		printf("		  ��@....::; ';'�i�� ���j::; ';'`�`�. \n");
		printf("			(:; ';'  |_��_| ::; ';' �@�R�j\n");
		printf("      '`'`'`~'~~`~~'`'`''''\"`'`'`''`''''\"`'``'~`''`'~`'''`'`'`~ \n");
		printf("\n �u��l������̌��t�ɏ]���I���������񂶂�Ȃ��I�v\n");
		printf("�U���������I�����͔��o���b�𕷂����Ƃ��ĂȂ���...�B\n");
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
		printf("                 , � \n");
		printf(" �P�@-- = �Q�@�@ / |            --'' \n");
		printf("     , , , , r�]��Ƀm�@ �i�_, �ɁT�@�@�@�@�@ - �P\n");
		printf("          �l               ޤ_ \n");
		printf("         .j�L.�c�܃~�@�@�@�i. \n");
		printf("�� �@�@��{ (�L�E�ցE�M) / ���@�@�@�Q�@�@�@�@�@�� \n");
		printf("         ).c /      ,�� �@ ,l~ \n");
		printf("      �Ly   �o ,�A�o�@ �@ < \n");
		printf("        �T   l� �R,)�@�@ , \n");
		printf("\n�u�܂Ƃ��ɓ��������Ȃ��ᑢ�ɋx�݂͖���!!�v\n");
		printf("�U���������I���В��̃I�[����瞂��Ă���...�B\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("                 , � \n");
		printf(" �P�@-- = �Q�@�@ / |            --'' \n");
		printf("     , , , , r�]��Ƀm�@ �i�_, �ɁT�@�@�@�@�@ - �P\n");
		printf("          �l               ޤ_ \n");
		printf("         .j�L.�c�܃~�@�@�@�i. \n");
		printf("�� �@�@��{ (�L�E�ցE�M) / ���@�@�@�Q�@�@�@�@�@�� \n");
		printf("         ).c /      ,�� �@ ,l~ \n");
		printf("      �Ly   �o ,�A�o�@ �@ < \n");
		printf("        �T   l� �R,)�@�@ , \n");
		printf("\n�u���͎В��ɂȂ邽�߂ɋx�܂�!!�v\n");
		printf("�U���������I�x�܂Ȃ��������œÂ̐i�s�������݂�����...�B\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("                 , � \n");
		printf(" �P�@-- = �Q�@�@ / |            --'' \n");
		printf("     , , , , r�]��Ƀm�@ �i�_, �ɁT�@�@�@�@�@ - �P\n");
		printf("          �l               ޤ_ \n");
		printf("         .j�L.�c�܃~�@�@�@�i. \n");
		printf("�� �@�@��{ (�L�E�ցE�M) / ���@�@�@�Q�@�@�@�@�@�� \n");
		printf("         ).c /      ,�� �@ ,l~ \n");
		printf("      �Ly   �o ,�A�o�@ �@ < \n");
		printf("        �T   l� �R,)�@�@ , \n");
		printf("\n �u�o�b�J���[�[�[�[�[�[�[�[�[��!�v\n");
		printf("�U���������I�Ƃ肠�����{���Ă�݂������B\n");
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
		printf("	      	*'``�E* �B \n");
		printf("			| `*�B\n");
		printf(".		,  ,., ����c �� �~�@* �@�@�@\n");
		printf("			+ (�L��֥`)�@*� + � \n");
		printf("			`*� �R�A�@ �� *�* \n");
		printf("			`�E + �*�E' ߁� +� \n");
		printf("			���@�@ ��~�*� \n");
		printf("				`�E + �*�E � \n");
		printf("\n�u�܂��������Ƃ̗L��݂��킩��Ȃ��̂ł���...�B�v\n");
		printf("�U���������I������ɂȂ�قǐ��]����Ă��܂��������B\n");
		t = getchar();
		break;

	case 2:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("	      	*'``�E* �B \n");
		printf("			| `*�B\n");
		printf(".		,  ,., ����c �� �~�@* �@�@�@\n");
		printf("			+ (�L��֥`)�@*� + � \n");
		printf("			`*� �R�A�@ �� *�* \n");
		printf("			`�E + �*�E' ߁� +� \n");
		printf("			���@�@ ��~�*� \n");
		printf("				`�E + �*�E � \n");
		printf("\n�u���ӁB���ӊ��ӊ��ӊ��ӊ��ӊ��ӊ��ӁB�v\n");
		printf("�U���������I�ǂ����ĎВ��͂���Ȃɂ�...�B\n");
		t = getchar();
		break;

	case 3:
		printf("\n\n\n\n\n\n\n\n\n\n");
		printf("	      	*'``�E* �B \n");
		printf("			| `*�B\n");
		printf(".		,  ,., ����c �� �~�@* �@�@�@\n");
		printf("			+ (�L��֥`)�@*� + � \n");
		printf("			`*� �R�A�@ �� *�* \n");
		printf("			`�E + �*�E' ߁� +� \n");
		printf("			���@�@ ��~�*� \n");
		printf("				`�E + �*�E � \n");
		printf("\n �u�d���������`...�B�v\n");
		printf("�U���������I�m���ɓ������Ƃ͑厖������...�B\n");
		t = getchar();
		break;

	}
}

//tasiro
void takeDamage() {

	char t;
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("      �Q�Q�Q�Q�Q�Q�Q�Q \n");
	printf("    �^�\":�; , ., ; .ҁ_\n");
	printf("  �^� ;; .; #; �;; #;; .�_\n");
	printf("�^; .�G(;;�);; . �i�~�j.�G�_\n");
	printf("| ;; .; ҁi__�l__�j; .;; �� |\n");
	printf("�_�#;;;; .ҁ@�; ;; �;; .; .�^\n");
	printf("�^�;;�@�; ;; .�; ;;;; ; �.�_\n");
	printf(" �_�j���ꂽ�E�E��J�x�����������B\n ");

	t = getchar();
}

//tasiro
void yarikomi() {
	char c;

	int finallyStatus = syujinko.str1 + syujinko.str2 + syujinko.str3;
	printf("�Q�[�����Ɏg�����񕜃A�C�e���̐�:%d\n", addItems);
	printf("�Q�[�����ɋx�e���𗘗p������:%d\n", addRests);
	printf("�X�e�[�^�X�̍��v�l:%d\n", finallyStatus);

	printf("���Ȃ��̏̍���,");
	c = getchar();

	if (addItems == 0 && addRests == 0 && finallyStatus <= 6)
	{
		printf(" �Ƃ�ł��Ȃ�����v���C�����Ă��ꂽ ");

	}
	else if (addItems == 0 && addRests == 0)
	{
		printf(" �f���炵�����b�L�[�� ");

	}
	else if (addItems == 0)
	{
		printf(" ���\���b�L�[�� ");

	}
	else if (addItems >= 6)
	{
		printf(" �h�{�h�����N�Ђ��� ");
	}
	else if (addRests >= 3)
	{
		printf(" �x�e���̎��_�� ");
	}
	else
	{
		printf(" ���ʂɃv���C���Ă��ꂽ ");
	}

	if (finallyStatus == 18) {
		printf(" �_�j�̐_�ƂȂ��� ");
	}
	else if (finallyStatus >= 15) {
		printf(" ���ɒ����Ă��� ");
	}
	else if (finallyStatus >= 10) {
		printf(" ���ʂɒ���� ");
	}
	else if (finallyStatus >= 7) {
		printf(" �R�~���j�e�B�ɕ��Q������ ");
	}

	printf("�В{�ł��B\n");

}



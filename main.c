//  main.c
//�÷��̾� ���� �� ������ �ֿ� ���� �帧 ���� 
//  SMMarble

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt" //���� ĭ(���) ���� 
#define FOODFILEPATH "marbleFoodConfig.txt" //���� ī�� ���� 
#define FESTFILEPATH "marbleFestivalConfig.txt" //���� ī�� ���� 

//board configuration parameters
static int board_nr;  //���� ĭ �� 
static int food_nr;  //���� ī�� �� 
static int festival_nr; //���� ī�� �� 
static int player_nr; //�÷��̾� �� 

//�÷��̾� ���� 
typedef struct player {
        int energy;  //������ 
        int position;  //��ġ 
        char name[MAX_CHARNAME];  //�̸� 
        int accumCredit;  //���� 
        int flag_graduate;  //���� ���� 
} player_t;

static player_t *cur_player; //���� �÷��̾� 
//static player_t cur_player[MAX_PLAYER];


//function prototypes
int isGraduated(void); //�÷��̾� �� ������ �����ߴ��� Ȯ�� 
 //print grade history of the player
void goForward(int player, int step); //�÷��̾� �̵� (���� ���� Ȯ�� ��) 
void printPlayerStatus(void); //�� ���� �� ��� �÷��̾��� ���� ��� 
float calcAverageGrade(int player); //�÷��̾��� ��� ���� ��� 
smmGrade_e takeLecture(int player, char *lectureName, int credit); //���� ����, �÷��̾� ���� �Է� 
void* findGrade(int player, char *lectureName); //�÷��̾��� ���� ��� �� Ư�� ���� ���� ã�� 
void printGrades(int player); //�÷��̾��� ��� ���� ��� Ȯ�� 

//���� ��� �Լ� 
void printGrades(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++) 
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);  //i��° ���� ������ ���� ������ 
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr)); //�����Ϳ��� ���� �̸��� ���� ��� 
     }
}

void printPlayerStatus(void) //���� �÷��̾� ���� ��� 
{
     int i;
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
}

void generatePlayers(int n, int initEnergy) //�� �÷��̾� ������ �⺻�� �ʱ�ȭ 
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //�̸� �Է� 
         printf("Input player %i's name:", i); //i��° �÷��̾��� �̸��� �Է��Ͻÿ�. 
         scanf("%s", cur_player[i].name); //�̸� �Է°� ���� 
         fflush(stdin);  
         
         //���� ��ġ ���� 
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //������ ���� 
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy; //���� ������ 
         cur_player[i].accumCredit = 0; //�̼� ���� 
         cur_player[i].flag_graduate = 0; //���� ���� 
     }
}


int rolldie(int player) //�ֻ��� ������ 
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): "); // �ƹ� Ű�� ���� �ֻ��� ������ (gŰ�� ���� ���� Ȯ��) 
    c = getchar();
    fflush(stdin);
    
    if (c == 'g') //g�� ������ ���� ��� 
        printGrades(player);
    
    return (rand()%MAX_DIE + 1); //�ֻ��� ���� ��� 
}

//action code when a player stays at a node 
//�÷��̾ ��忡 �ӹ��� �� �����ϴ� �ڵ� 
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position ); 
	//���� �÷��̾ ��ġ�� ��忡 ���� ���� 
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr ); //�ش� ����� ���� 
    char *name = smmObj_getNodeName( boardPtr );  //�ش� ����� �̸� 
    void *gradePtr;  //�ش� ����� ���� 
    
    
    switch(type)
       {
        case SMMNODE_TYPE_LECTURE:  //��� ����=���� 
        if(cur_player[player].energy>=smmObj_getNodeEnergy( boardPtr )&& ) 
		//���� ������>=�ҿ信����& ������ ���� ���� ������ ��� 
            {cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr ); //���� �߰� 
            cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr ); //������ �Ҹ� 
            
            //grade generation
            // ���� ���� ���� �Լ�
            double getRandomGrade() {
            	int randomIndex = rand() % 9;
            	  switch (randomIndex) {
				  case 0: return Ap;
				  case 1: return A0;
				  case 2: return Am;
				  case 3: return Bp;
				  case 4: return B0;
				  case 5: return Bm;
				  case 6: return Cp;
				  case 7: return C0;
				  case 8: return Cm;
				  default: return 0.0; // ���� ó��
				  }
		// ���� ����� ����ϴ� �Լ�
		double calculateAverageGrade(double *grades, int player_nr) {
			double sum = 0.0;
			for (int i = 0; i < player_nr; ++i) {
				double selectedGrade = getRandomGrade();
				double storedGrade = selectedGrade + LISTNO_OFFSET_GRADE;
				grades[i] = storedGrade;
				sum += storedGrade;
			 }
	    // ��� ���
	    double average = sum / player_nr;
	    double *playerGrades = (double *)malloc(player_nr * sizeof(double)); // �� �÷��̾��� ������ ������ �迭
        double average = calculateAverageGrade(playerGrades, player_nr); // ��� ���� ���
	    }
        break;
        
        case SMMNODE_TYPE_RESTAURANT: //��� ����=�Ĵ� 
        //���� ���� ������ ������ ���� 
            cur_player[player].energy += smmObj_getNodeEnergy( boardPtr );
            break;
            
		case SMMNODE_TYPE_GOTOLAB: //��� ����=����
		    cur_player[i].position= //LABORATORY type ĭ���� ���� �̵� 
		
		case SMMNODE_TYPE_FOODCHANCE; //��� ����=����ī��
		
		case SMMNODE_TYPE_FESTIVAL; //��� ����=����ī�� 
		 
        
        
        }
        default:
            break;
    }
}

void goForward(int player, int step) //�÷��̾� �̵� 
{
     void *boardPtr; //��� ������ ���� 
     cur_player[player].position += step;  //��ġ�� step��ŭ �̵� 
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position ); //���ο� ��ġ�� ��� ������ ���� 
     
     //���ο� ��ġ ���,  ��� �̸� ��� 
     printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr));
}


int main(int argc, const char * argv[]) {
    //���� ���� 
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    //���� ���� �ʱ�ȭ 
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    //���� ���� 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL) //���� ���⿡ �������� ��� 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH); //���� �޼��� ��� 
        getchar(); //�ƹ� Ű�� �Է�
        return -1; //-1 ��� 
    }
    
    printf("Reading board component......\n"); //���� ���� ��� Ȯ�� ��... 
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //��� ���� �б� 
    {
    	
        //�Ű����� �����ϱ� 
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj); //�о�� ������ �������� ���ο� ��ü ���� 
        
        
        if (type == SMMNODE_TYPE_HOME) //�� ������ ��� 
           initEnergy = energy; //�� ����� �������� �ʱ� �������� ���� 
        board_nr++; //������ �� ������ ���� ���� �ϳ��� ���� 
    }
    fclose(fp); //���� �ݱ� 
    printf("Total number of board nodes : %i\n", board_nr); //�о���� ���� ����� �� ���� ��� 
    
    
    for (i = 0;i<board_nr;i++) 
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    } //��� �̸�, ����, ����, ������ ��� 
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //1-2. food card config (���� ī�� ���� �б�)
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL) //���� ���� ���� �� ���� �޼��� ��� 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n"); //���� ī�� ������ �д� ��...�޼��� ��� 
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr); //���� ī���� �� ��� 
    
    
    
    //1-3. festival card config (���� ī�� ���� �б�) 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL) //���� ���� ���� �� ���� �޼��� ��� 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n"); //���� ī�� ������ �д� ��...�޼��� ��� 
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr); //���� ī���� �� ��� 
    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr 
        printf("input player no.:"); //�÷��̾� ��ȣ �Է� ��û 
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER); //�Էµ� �÷��̾� ��ȣ�� 0���� �۰ų� �ִ񰪺��� ū ��� ���û 
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); //���� �޸� �Ҵ� 
    generatePlayers(player_nr, initEnergy);  //���� �Ҵ� �޸𸮿� �÷��̾� ��ü ����  (�÷��̾� ���, �ʱ� ������)
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated? (������ ������ ������ �ݺ�) 
    {
        int die_result;
        
        
        //4-1. initial printing (�ʱ� ���� ���) 
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)   (���� ���� ��츦 �����ϰ� �ֻ��� ������)     
        die_result = rolldie(turn);
        
        //4-3. go forward (������ �̵�) 
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board (������ ��忡 �´� �ൿ ����) 
        actionNode(turn);
        
        //4-5. next turn (���� ���ʷ� �Ѿ��) 
        turn = (turn + 1)%player_nr;
    }
    
  //5. SM Marble game ending ---------------------------------------------------------------------------------
    //���� ��� ���
    printf("Grades for each player:\n");
    for (int i = 0; i < player_nr; ++i) {
        printf("Player %d: %.1lf\n", i + 1, playerGrades[i]);
    }
    printf("Average grade for %d players: %.1lf\n", player_nr, average);
    free(playerGrades);    // ���� �޸� ����
    return 0;
}
    free(cur_player); //���� �޸� ���� 
    system("PAUSE");
    return 0;
}



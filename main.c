//  main.c
//플레이어 관리 및 게임의 주요 동작 흐름 실현 
//  SMMarble

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt" //보드 칸(노드) 파일 
#define FOODFILEPATH "marbleFoodConfig.txt" //음식 카드 파일 
#define FESTFILEPATH "marbleFestivalConfig.txt" //축제 카드 파일 

//board configuration parameters
static int board_nr;  //보드 칸 수 
static int food_nr;  //음식 카드 수 
static int festival_nr; //축제 카드 수 
static int player_nr; //플레이어 수 

//플레이어 정보 
typedef struct player {
        int energy;  //에너지 
        int position;  //위치 
        char name[MAX_CHARNAME];  //이름 
        int accumCredit;  //학점 
        int flag_graduate;  //도착 여부 
} player_t;

static player_t *cur_player; //현재 플레이어 
//static player_t cur_player[MAX_PLAYER];


//function prototypes
int isGraduated(void); //플레이어 중 누구라도 졸업했는지 확인 
 //print grade history of the player
void goForward(int player, int step); //플레이어 이동 (졸업 여부 확인 후) 
void printPlayerStatus(void); //턴 시작 시 모든 플레이어의 상태 출력 
float calcAverageGrade(int player); //플레이어의 평균 성적 계산 
smmGrade_e takeLecture(int player, char *lectureName, int credit); //강의 수강, 플레이어 성적 입력 
void* findGrade(int player, char *lectureName); //플레이어의 성적 기록 중 특정 강의 성적 찾기 
void printGrades(int player); //플레이어의 모든 성적 기록 확인 

//성적 출력 함수 
void printGrades(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++) 
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);  //i번째 강의 성적에 대한 포인터 
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr)); //포인터에서 강의 이름과 성적 출력 
     }
}

void printPlayerStatus(void) //현재 플레이어 정보 출력 
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

void generatePlayers(int n, int initEnergy) //새 플레이어 생성과 기본값 초기화 
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //이름 입력 
         printf("Input player %i's name:", i); //i번째 플레이어의 이름을 입력하시오. 
         scanf("%s", cur_player[i].name); //이름 입력과 저장 
         fflush(stdin);  
         
         //시작 위치 지정 
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //에너지 설정 
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy; //시작 에너지 
         cur_player[i].accumCredit = 0; //이수 학점 
         cur_player[i].flag_graduate = 0; //졸업 여부 
     }
}


int rolldie(int player) //주사위 굴리기 
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): "); // 아무 키나 눌러 주사위 굴리기 (g키를 눌러 학점 확인) 
    c = getchar();
    fflush(stdin);
    
    if (c == 'g') //g를 누르면 학점 출력 
        printGrades(player);
    
    return (rand()%MAX_DIE + 1); //주사위 숫자 출력 
}

//action code when a player stays at a node 
//플레이어가 노드에 머무를 때 실행하는 코드 
void actionNode(int player)
{
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position ); 
	//현재 플레이어가 위치한 노드에 대한 정보 
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr ); //해당 노드의 유형 
    char *name = smmObj_getNodeName( boardPtr );  //해당 노드의 이름 
    void *gradePtr;  //해당 노드의 학점 
    
    
    switch(type)
       {
        case SMMNODE_TYPE_LECTURE:  //노드 유형=강의 
        if(cur_player[player].energy>=smmObj_getNodeEnergy( boardPtr )&& ) 
		//현재 에너지>=소요에너지& 이전에 듣지 않은 강의인 경우 
            {cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr ); //학점 추가 
            cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr ); //에너지 소모 
            
            //grade generation
            // 랜덤 성적 선택 함수
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
				  default: return 0.0; // 예외 처리
				  }
		// 성적 평균을 계산하는 함수
		double calculateAverageGrade(double *grades, int player_nr) {
			double sum = 0.0;
			for (int i = 0; i < player_nr; ++i) {
				double selectedGrade = getRandomGrade();
				double storedGrade = selectedGrade + LISTNO_OFFSET_GRADE;
				grades[i] = storedGrade;
				sum += storedGrade;
			 }
	    // 평균 계산
	    double average = sum / player_nr;
	    double *playerGrades = (double *)malloc(player_nr * sizeof(double)); // 각 플레이어의 성적을 저장할 배열
        double average = calculateAverageGrade(playerGrades, player_nr); // 평균 성적 계산
	    }
        break;
        
        case SMMNODE_TYPE_RESTAURANT: //노드 유형=식당 
        //조건 없이 무조건 에너지 충전 
            cur_player[player].energy += smmObj_getNodeEnergy( boardPtr );
            break;
            
		case SMMNODE_TYPE_GOTOLAB: //노드 유형=실험
		    cur_player[i].position= //LABORATORY type 칸으로 강제 이동 
		
		case SMMNODE_TYPE_FOODCHANCE; //노드 유형=음식카드
		
		case SMMNODE_TYPE_FESTIVAL; //노드 유형=축제카드 
		 
        
        
        }
        default:
            break;
    }
}

void goForward(int player, int step) //플레이어 이동 
{
     void *boardPtr; //노드 데이터 수집 
     cur_player[player].position += step;  //위치를 step만큼 이동 
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position ); //새로운 위치의 노드 데이터 수집 
     
     //새로운 위치 출력,  노드 이름 출력 
     printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr));
}


int main(int argc, const char * argv[]) {
    //변수 선언 
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    //게임 정보 초기화 
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    //파일 열기 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL) //파일 열기에 실패했을 경우 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH); //오류 메세지 출력 
        getchar(); //아무 키나 입력
        return -1; //-1 출력 
    }
    
    printf("Reading board component......\n"); //보드 구성 요소 확인 중... 
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //노드 변수 읽기 
    {
    	
        //매개변수 저장하기 
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj); //읽어온 정보를 바탕으로 새로운 객체 생성 
        
        
        if (type == SMMNODE_TYPE_HOME) //집 유형일 경우 
           initEnergy = energy; //집 노드의 에너지를 초기 에너지로 설정 
        board_nr++; //루프가 돌 때마다 보드 개수 하나씩 세기 
    }
    fclose(fp); //파일 닫기 
    printf("Total number of board nodes : %i\n", board_nr); //읽어들인 보드 노드의 총 개수 출력 
    
    
    for (i = 0;i<board_nr;i++) 
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    } //노드 이름, 유형, 학점, 에너지 출력 
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //1-2. food card config (음식 카드 정보 읽기)
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL) //파일 오픈 실패 시 에러 메세지 출력 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n"); //음식 카드 정보를 읽는 중...메세지 출력 
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr); //음식 카드의 수 출력 
    
    
    
    //1-3. festival card config (축제 카드 정보 읽기) 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL) //파일 오픈 실패 시 에러 메세지 출력 
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n"); //축제 카드 정보를 읽는 중...메세지 출력 
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr); //축제 카드의 수 출력 
    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr 
        printf("input player no.:"); //플레이어 번호 입력 요청 
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER); //입력된 플레이어 번호가 0보다 작거나 최댓값보다 큰 경우 재요청 
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t)); //동적 메모리 할당 
    generatePlayers(player_nr, initEnergy);  //동적 할당 메모리에 플레이어 객체 생성  (플레이어 명수, 초기 에너지)
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated? (누군가 졸업할 때까지 반복) 
    {
        int die_result;
        
        
        //4-1. initial printing (초기 상태 출력) 
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)   (실험 중일 경우를 제외하고 주사위 굴리기)     
        die_result = rolldie(turn);
        
        //4-3. go forward (앞으로 이동) 
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board (도착한 노드에 맞는 행동 수행) 
        actionNode(turn);
        
        //4-5. next turn (다음 차례로 넘어가기) 
        turn = (turn + 1)%player_nr;
    }
    
  //5. SM Marble game ending ---------------------------------------------------------------------------------
    //성적 평균 출력
    printf("Grades for each player:\n");
    for (int i = 0; i < player_nr; ++i) {
        printf("Player %d: %.1lf\n", i + 1, playerGrades[i]);
    }
    printf("Average grade for %d players: %.1lf\n", player_nr, average);
    free(playerGrades);    // 동적 메모리 해제
    return 0;
}
    free(cur_player); //동적 메모리 해제 
    system("PAUSE");
    return 0;
}



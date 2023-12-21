//
//  smm_object.c
//  SMMarble
// 게임에필요한 객체(노드, 음식카드, 축제카드, 수강 이력) 정보 

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


// 성적에 해당하는 매크로 정의
#define Ap 4.3
#define A0 4.0
#define Am 3.7
#define Bp 3.3
#define B0 3.0
#define Bm 2.7
#define Cp 2.3
#define C0 2.0
#define Cm 1.7

// 저장할 소수 자릿수 정의
#define LISTNO_OFFSET_GRADE 10

//노드 유형 정의 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험",
       "보충찬스",
       "축제"
};

//유형 번호에 맞는 노드 유형 출력 
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}



//1. 강의 이름, 유형, 학점, 소모 에너지 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;
//object generation

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade) 
//보드 파일 객체 생성과 초기화 
{    
    smmObject_t* ptr;
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;     
}

char* smmObj_getNodeName(void* obj) //노드 이름 문자열 포인터 반환 
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

int smmObj_getNodeType(int node_nr) //노드 유형 반환 
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr) //노드 학점 반환 
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr) //소요, 보충 에너지 반환 
{
    return smm_node[node_nr].energy;
}

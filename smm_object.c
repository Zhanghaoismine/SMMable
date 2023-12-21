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

typedef enum smmObjGrade {
	smmObjGrade_Ap = 0,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
} smmObjGrade_e;

static char smmGradeName [9][2]={
	"A+",
	"A0",
	"A-",
	"B+",
	"B0",
	"B-",
	"C+",
	"C0",
	"C-",
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

static smmObject_t smm_node[MAX_NODE];
static int smmObj_noNode = 0;
//object generation

//보드 파일 객체 생성과 초기화 
void smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy)
{    
    smmObject_t* ptr;
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    
    return ptr;     
}
#if 0
char* smmObj_getNodeName(void* obj) //노드 이름 문자열 포인터 반환 
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}
#endif

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

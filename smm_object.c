//
//  smm_object.c
//  SMMarble
// ���ӿ��ʿ��� ��ü(���, ����ī��, ����ī��, ���� �̷�) ���� 

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


//��� ���� ���� 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "����",
       "�Ĵ�",
       "�����",
       "��",
       "����",
       "��������",
       "����"
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


//���� ��ȣ�� �´� ��� ���� ��� 
char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}



//1. ���� �̸�, ����, ����, �Ҹ� ������ 
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

//���� ���� ��ü ������ �ʱ�ȭ 
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
char* smmObj_getNodeName(void* obj) //��� �̸� ���ڿ� ������ ��ȯ 
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}
#endif

int smmObj_getNodeType(int node_nr) //��� ���� ��ȯ 
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr) //��� ���� ��ȯ 
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr) //�ҿ�, ���� ������ ��ȯ 
{
    return smm_node[node_nr].energy;
}

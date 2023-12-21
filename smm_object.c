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


// ������ �ش��ϴ� ��ũ�� ����
#define Ap 4.3
#define A0 4.0
#define Am 3.7
#define Bp 3.3
#define B0 3.0
#define Bm 2.7
#define Cp 2.3
#define C0 2.0
#define Cm 1.7

// ������ �Ҽ� �ڸ��� ����
#define LISTNO_OFFSET_GRADE 10

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

//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;
//object generation

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade) 
//���� ���� ��ü ������ �ʱ�ȭ 
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

char* smmObj_getNodeName(void* obj) //��� �̸� ���ڿ� ������ ��ȯ 
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

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

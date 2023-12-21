//  smm_database.h
//  infection Path


#ifndef smm_database_h
#define smm_database_h

#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

int smmdb_addTail(int list_nr, void* obj);          //����Ʈ ���� ������ �߰� 
int smmdb_deleteData(int list_nr, int index);       //����Ʈ���� �ش� �ε��� ������ ���� 
int smmdb_len(int list_nr);                   //����Ʈ ����(������ ����) ��ȯ 
void* smmdb_getData(int list_nr, int index);        //Ư�� ����Ʈ�� �ε��� ������ ��ȯ 

#endif /* smm_database_h */
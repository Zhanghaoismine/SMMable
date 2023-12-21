//  smm_database.h
//  infection Path


#ifndef smm_database_h
#define smm_database_h

#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

int smmdb_addTail(int list_nr, void* obj);          //리스트 끝에 데이터 추가 
int smmdb_deleteData(int list_nr, int index);       //리스트에서 해당 인덱스 데이터 삭제 
int smmdb_len(int list_nr);                   //리스트 길이(데이터 개수) 반환 
void* smmdb_getData(int list_nr, int index);        //특정 리스트의 인덱스 데이터 반환 

#endif /* smm_database_h */

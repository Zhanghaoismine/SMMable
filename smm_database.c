//  smm_database.c
//  Sookmyung Marble
//  Database platform code for storing marble board elements
//보드의 노드, 음식카드, 축제카드, 플레이어의 수강 강의 이력을 저장하는 데이터베이스 
// 수정 없이 그대로 활용 

#include "smm_common.h"
#include "smm_database.h"
#define LIST_END            -1
#define MAX_LIST            LISTNO_OFFSET_GRADE+MAX_PLAYER

//node definition for linked list
typedef struct node{
    int index;      //노드의 인덱스 
    void* obj;      //노드에 저장된 데이터 
    void* next;         //다음 노드 
    void* prev;         //이전 노드 
} node_t;

static node_t* list_database[MAX_LIST]; //리스트 데이터베이스로 활용할 배열 
static node_t* listPtr[MAX_LIST]; //현재 작업 중인 리스트의 포인터 저장 
static int list_cnt[MAX_LIST];  //각 리스트의 현재 노드 수 

//Inner functions (cannot used at the outside of this file)
static node_t* genNode(void)  
{
    //allocate memory for creating the node
    node_t* ndPtr = (node_t*)malloc(sizeof(node_t)); //node_t 구조체 크기만큼 동적 메모리 할당 
    if (ndPtr != NULL) //메모리 할당에 성공한 경우 
    {
        ndPtr->next = NULL;
        ndPtr->obj = NULL;
        ndPtr->prev = NULL;
    } //새 노드의 다음노드, 저장된 데이터, 이전 노드를 NULL로 설정 
    
    return ndPtr; //새 노드의 포인터 반환 
}
//주어진 리스트에서 특정 인덱스에 해당하는 노드를 찾아 반환하는 함수 
static node_t* smmList(int list_nr, int index) //매개변수: 리스트 번호, 노드의 인덱스 
{
    node_t* ndPtr = list_database[list_nr]; //ndPtr을 리스트의 헤드 노드로 설정 
    if (listPtr[list_nr] != NULL && listPtr[list_nr]->index <= index) //검색 범위를 작업 중인 리스트로 제한 
    {
        ndPtr = listPtr[list_nr]; //작업 중인 리스트의 헤드 노드를 ndPtr 포인터에 할당 
    }
    
    //return for wrong input
    if (index <-1) //인덱스 값이 -1보다 작은 경우 
    {
        printf("[ERROR] smmList() : either list is null or index is wrong! (offset : %i)\n", index); //오류 메세지 출력 
        return NULL;
    }
    if (index >= list_cnt[list_nr]) //인덱스 값이 리스트 길이보다 큰 경우 
    {
        printf("[ERROR] smmList() : index is larger than length (len:%i, index:%i)\n", list_cnt[list_nr], index);
        //오류 메세지 출력 
        return NULL;
    }
    
    //repeat travelling until the node is the end or offset becomes 0
    while (ndPtr != NULL) //ndPtr 
    {
        if (ndPtr->index == index)
            break;
        ndPtr = ndPtr->next;
    }
    
    return ndPtr;
}

static int updateIndex(int list_nr)
{
    int index=0;
    node_t* ndPtr = list_database[list_nr];
    
    while ( ndPtr != NULL )//travel until it is the end node
    {
        ndPtr->index = index++;
        ndPtr = ndPtr->next; //travel once
    }
    
    return index;
}
/*
    설명 : 리스트 끝에 데이터 객체 추가
    입력 매개변수 : obj - 리스트에 추가할 데이터 객체
    반환 값 : 추가 결과 (0 - 성공, -1 - 실패)
    
    작업 : 1. 새로운 노드를 만듭니다.
           2. 리스트에서 마지막 노드를 찾습니다.
           3. 마지막 노드의 다음 포인터를 새로운 노드를 가리키도록 업데이트합니다.
           4. 인덱스를 업데이트합니다.
*/

int smmdb_addTail(int list_nr, void* obj)
{
    node_t* ndPtr;
    node_t* newNdPtr;
    
    //parameter checking
    if (obj == NULL)
    {
        printf("[ERROR] smmdb_addTail() : Failed to do addTail : input object indicates NULL!\n");
        return -1;
    }
    
    //generate new node
    newNdPtr = genNode();
    if (newNdPtr == NULL)
    {
        printf("[ERROR] smmdb_addTail() : Failed to do addTail : Failed to create a node\n");
        return -1; //indicate to the upper function that node addition is failed
    }
    newNdPtr->obj = obj;
    
    //add node to the list tail
    if (list_database[list_nr] == NULL)
    {
        list_database[list_nr] = newNdPtr;
        newNdPtr->index = 0;
    }
    else
    {
        ndPtr = smmList(list_nr, list_cnt[list_nr]-1);
        ndPtr->next = newNdPtr;
        newNdPtr->prev = ndPtr;
        newNdPtr->index = ndPtr->index+1;
    }
    
    listPtr[list_nr] = newNdPtr;
    
    list_cnt[list_nr]++;
    
    return 0;
}


/*
    description : delete data object from the list
    input parameters : index - index'th data to delete
    return value : deletion result (0 - succeeded, -1 - failed)
*/
int smmdb_deleteData(int list_nr, int index)
{
    node_t* ndPrevPtr;
    node_t* ndNextPtr;
    node_t* delNdPtr;
    
    //parameter checking
    if ( (delNdPtr = smmList(list_nr, index)) == NULL)
    {
        printf("[ERROR] smmdb_deleteData() : Failed to do deleteData : input index is invalid (%i)!\n", index);
        return -1;
    }
    
    ndPrevPtr = delNdPtr->prev;
    if (ndPrevPtr != NULL)
    {
        ndPrevPtr->next = delNdPtr->next;
    }
    ndNextPtr = delNdPtr->next;
    if (ndNextPtr != NULL)
    {
        ndNextPtr->prev = delNdPtr->prev;
    }
    
    free(delNdPtr->obj);
    free(delNdPtr);
    
    list_cnt[list_nr]--;
    
    if (list_cnt[list_nr] == 0)
    {
        list_database[list_nr] = NULL;
    }
    else
    {
        updateIndex(list_nr);
    }
    
    return 0;
}


//functions for list observation -----------------------------

/*
    description : return the number of data objects in the list
    return value : length
*/
int smmdb_len(int list_nr)
{
    return list_cnt[list_nr];
}

/*
    description : get the object data
    input parameters : index
    return value : object pointer
*/
void* smmdb_getData(int list_nr, int index)
{
    void* obj = NULL;
    node_t* ndPtr;
    
    //parameter checking
    if ((ndPtr = smmList(list_nr, index)) != NULL)
    {
        obj = (void*)ndPtr->obj;
        listPtr[list_nr] = ndPtr;
    }
    
    if (obj == NULL)
        printf("[ERROR] smmdb_getData() : there is no data of index %i\n", index);
    
    return obj;
}


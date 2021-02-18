#include <stdio.h>
#include<stdlib.h>
#include<string.h>

char* result;				//compressing 과정에서 text에서 읽은 값을 이진코드로변환해서 저장하는 변수, decompressing과정에서 읽어온 char에 해당하는 이진코드를 모두 저장하는 변수
int result_size = 0;			//result 배열의 끝을 지정해주는 변수
unsigned char* temp_result;		//decompressing 과정에서 text에서 읽은 이진수를 저장하는 벼수
int temp_result_size=0;			//temp_result 배열의 끝을 지정해주는 변수
int transfer = 0;			//decompressing에서 이진수가 나타내는 char를 찾는 과정에서 사용되는 변수

typedef struct _node {			//노드 선언
	char data;			//char 값을 저장
	int freq;			//빈도수 저장
	struct _node* left;		
	struct _node* right;
	struct _node* line_up;		//초기에 트리를 연결하기 전에 사용
}NODE;

NODE* root;					//root노드와 
FILE* fp;					//파일변수 선언
FILE* fp2;

void initTree(char data) {			//root node를 생성해준다.
	NODE* New;
	New = (NODE*)malloc(sizeof(NODE));
	New->left = NULL;
	New->right = NULL;
	New->line_up = NULL;
	root = New;
	root->data = data;
}


NODE* AddChild_for_line_up(NODE* parent, int data) {	//읽어온  각 char을  node에 할당하고 일렬로 연결하는 함수
	NODE* New;
	New = (NODE*)(malloc(sizeof(NODE)));
	New->left = NULL;
	New->right = NULL;
	New->freq = 1;
	New->data = data;
	New->line_up = NULL;
	parent->line_up = New;					//이전의 노드와 line_up으로 연결한다.

	return New;
}

void AddChild(NODE* parent, char data, char temp_string[], int temp_string_length,int compare) {	//decompressing에서 트리를 생성할 때 사용한다.
	if (compare==temp_string_length) {								//모든 값을 비교했다면 data를 그 노드에 저장한다.	
		parent->data = data;
	}	
	else {
		if (temp_string[compare] == '0') {							//0의 값을 확인하고 있다면 ->right으로 이동한다.
			if (parent->right == NULL) {							//이동할 곳이 없다면 새로 만들어 준다.
				NODE* New = (NODE*)malloc(sizeof(NODE));	
				New->right = NULL;
				New->left = NULL;
				parent->right = New;
			}
			AddChild(parent->right, data, temp_string, temp_string_length,compare+1);
		}
		else if(temp_string[compare]=='1'){							//1의 값을 확인하고 있다면 ->left으로 이동한다.
			if (parent->left == NULL) {							//이동할 곳이 없다면 새로 만들어 준다.
				NODE* New = (NODE*)malloc(sizeof(NODE));
				New->right = NULL;
				New->left = NULL;
				parent->left = New;
			}
			AddChild(parent->left, data, temp_string, temp_string_length,compare+1);
		}
	}
}


void combine(NODE* First, NODE* Second) {				//compressing에서 처음 일렬로 정렬된 노드 중 가장 작은 두개를 더해 새로운 노드로 만들며
	NODE* New = (NODE*)(malloc(sizeof(NODE)));
	New->left = First->line_up;
	New->right = Second->line_up;
	New->freq = First->line_up->freq + Second->line_up->freq;	//그 빈도수는 두 노드의 빈도수의 합이다.
	Second->line_up = Second->line_up->line_up;
	New->line_up = First->line_up->line_up;
	First->line_up = New;
}




void sort_nodes() {							//compressing에서 일렬로 정렬된 노드를 크기순서로 정렬한다.
	NODE* New_root = (NODE*)malloc(sizeof(NODE));
	New_root->line_up = NULL;
	New_root->freq = 0;
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	NODE* temp_2 = (NODE*)malloc(sizeof(NODE));

	temp = root->line_up;
	
	while (temp != NULL) {
		temp_2 = New_root;						//root부터 탐색하면서 
		while(1){
			if (temp_2->line_up == NULL) {				//끝까지 비교했다면 마지막에 값을 넣어준다.
				NODE* temp_3 = (NODE*)malloc(sizeof(NODE));
				temp_3->freq = temp->freq;
				temp_3->right = temp->right;
				temp_3->left = temp->left;
				temp_3->data = temp->data;
				temp_3->line_up = NULL;
				temp_2->line_up = temp_3;
				break;
			}
			else {
				if (temp->freq >= temp_2->line_up->freq)	//비교한 값이 더 작다면 다음 노드를 비교한다.
					temp_2 = temp_2->line_up;
				else {
					NODE* temp_3 = (NODE*)malloc(sizeof(NODE));	//비교한 값이 더 크다면 그 노드 앞에 값을 넣어준다. 
					temp_3->freq = temp->freq;
					temp_3->right = temp->right;
					temp_3->left = temp->left;
					temp_3->data = temp->data;

					temp_3->line_up = temp_2->line_up;
					temp_2->line_up = temp_3;
					break;
				}
			}
		}
		temp = temp->line_up;
	}
	root = New_root;
}



void DFS(NODE* search,char a,char temp_string[],int temp_string_length) {		//노드에 저장되어있는 값들을 이진코드로바꿔서 result에 모두 저장한다.
	int i;
	if (search->right == NULL && search->left == NULL && search->data==a){			//더이상 자식이 없다면 여태까지 쌓아온 temp_string의 값을 result에 붙인다.
		result = (char*)realloc(result, sizeof(char) * (result_size+temp_string_length));
		for (i = 0; i < temp_string_length; i++)
			result[i + result_size] = temp_string[i];
		result_size += (temp_string_length);
	}
	if (search->left != NULL) {							//왼쪽에 자식이 있다면 temp_string에 1을 붙이고 DFS를 재귀호출한다.
		temp_string[temp_string_length] = '1';
		DFS(search->left,a,temp_string,temp_string_length+1);
	}
	if (search->right != NULL) {							//오른쪽에 자식이 있다면 temp_string에 0을 붙이고 DFS를 재귀호출한다.
		temp_string[temp_string_length] = '0';	
		DFS(search->right, a, temp_string, temp_string_length + 1);
	}

}






void DFS_fprintf(NODE* search, char temp_string[], int temp_string_length) {		//압축후 파일 가장 앞에 어떤 char가 어떤 이진수를 나타내는지를 적는 함수
	int i;
	if (search->right == NULL && search->left == NULL ) {				//자식 노드가 없다면 fp파일에 char(data)와 이진수로 나타낸 코드의 길이
		fprintf(fp2, "%c%c", search->data,(char)temp_string_length+'0');
		for (i = 0; i < temp_string_length; i++)
			fprintf(fp2, "%c", temp_string[i]);				//그리고 이진수로 나타낸 코드를 출력한다.
	}
	if (search->left != NULL) {							//왼쪽에 자식이 있다면 1을 붙이고 재귀호출한다.
		temp_string[temp_string_length] = '1';			
		DFS_fprintf(search->left, temp_string, temp_string_length + 1);
	}
	if (search->right != NULL) {							//오른쪽에 자식이 있다면 0을 붙이고 재귀호출한다.
		temp_string[temp_string_length] = '0';
		DFS_fprintf(search->right,temp_string, temp_string_length + 1);
	}
}




unsigned char make_decade(int a, int b) {		//result배열의 a~b까지의 이진수를 하나의 unsigned char에 저장하는 과정이다.
	unsigned char decade=0;
	int i;
	for (i = a; i < b; i++) {
		if (result[i] == '0') {			//현재 조사하는 값이 0이라면 decade를 1bit만큼 옆으로 shifting한다.
			decade <<= 1;
		}
		else {					//현재 조사하는 값이 1이라면 decade를 1bit만큼 옆으로 shifting하고 1을 더해준다.
			decade <<= 1;
			decade |= 1;
		}
	}
	return decade;
}



char make_binary(unsigned char a) {						//decompressing에서 읽어온 char를 이진수로 해독하는 함수이다.
	int temp = a;
	int i;
	result = (char*)realloc(result, sizeof(char) * (result_size +16));	//해독한 함수는 result에 저장한다.
	for (i = 0; i < 8; i++) {						//a를 총 8번 2로 나누면서 10진수를 2진수로 변환하는 과정을 거친다.
		if (temp == 0) {
			result[result_size + 7 - i] = '0';
		}
		else if (temp == 1) {
			result[result_size + 7 - i] = '1';
			temp = 0;
		}
		else if (temp % 2 == 0 && temp !=1) {
			result[result_size + 7 - i]='0';
		}
		else if(temp %2==1 && temp !=1) {
			result[result_size + 7 - i] = '1';
		}
		temp /= 2;
	}
	result_size += 8;
}




int DFS_find_char(NODE* parent) {						//decompressing에서 result에 저장된 이진코드를 순서대로 읽는 함수이다.
	if (parent->right == NULL && parent->left == NULL) {
		fprintf(fp2, "%c", parent->data);				//앞에서부터 읽었을 때 해당하는 char가 있으면 그 char를 결과 파일에  출력한다.
		return 0;
	}
	if (result[transfer] == '0') {						//현재 보고있는 값이 0이라면 ->right를 하고 재귀호출을 한다.
		transfer++;
		DFS_find_char(parent->right);
	}
	else if (result[transfer] == '1') {					//현재 보고있는 값이 1이라면 ->left를 하고 재귀호출을 한다.
		transfer++;
		DFS_find_char(parent->left);
	}
	return 0;
}




int main(int argc, char* argv[]) {
	char c_d;							//compressing, decompressing을 구분해주는 변수
	char filename[200];						//input 파일 이름
	char result_filename[200];					//output 파일 이름
	char* story = (char*)malloc(sizeof(char)*1);			//compressing에서 읽어온 모든 char들을 저장하는 배열
	int cnt = 0;							//story배열의 총 길이
	int i,j;
	int pass = 0;							//decompressing에서암호화된 char의 정보를 읽을 때 쓰레기 값을 처리해주기 위한 변수
	int num_of_char = 0;						//decompressing에서암호화된 char의 정보의 개수
	char temp_string[8];						//중간중간의 짧은 이진코드를 저장하는 변수
	int temp_string_length = 0;					//temp_string의 길이
	unsigned char store;						//compressing에서 output file에 출력하는 char을 저장하는 변수
	unsigned char temp_data;					//decompressing에서 읽어온 char를 임시 저장하는 변수


	NODE* temp = (NODE*)(malloc(sizeof(NODE)));			//node들의 초기 선언		
	NODE* temp2=(NODE*)(malloc(sizeof(NODE)));
	NODE* temp3= (NODE*)(malloc(sizeof(NODE)));
	

	//----------------------------------compressing or decompressing, input파일을 받는 구간
	if (argc != 3) {				
		printf("input is wrong\n");
		return 0;
	}

	c_d = argv[1][1];
	strcpy(filename, argv[2]);
	//-----------------------------------
			

	//----------------------------------------------------------------------compressing하는 부분
	if (c_d == 'c') {
		printf("compressing start\n");
	    	
		fp = fopen(filename, "r");		//파일을 연다
		if(fp==NULL){
		    printf("no file");
		    return 0;
		}
		
				
		while (!feof(fp)) {							//파일의 끝까지 읽어서 모든 값을 story배열에 저장한다.
			story = (char*)realloc(story, sizeof(char) * (cnt + 1));
			fscanf(fp, "%c", &story[cnt]);
			cnt++;
		}
		
		
		result = (char*)malloc(sizeof(char));					//result를 선언하고
		initTree(128);								//root를 선언한다.
		cnt--;
		
		
		for (i = 0; i < cnt; i++) {						//root에 story에 저장된 char값들을 담은 노드를 연결한다.
			temp = root;
			while (1) {
				if (temp->data == story[i]) {				//현재 넣으려는 char가 이미 노드에 있다면 frequency(빈도수)를 올려준다.
					temp->freq++;
					break;
				}
				else {
					if (temp->line_up != NULL) {			//현재 값과 다르면 다음 값을 비교한다.
						temp = temp->line_up;
					}
					else {						//현재 값과 다르며, 다음 값이 없다면 현재 넣으려는 char를 담은 노든는
						num_of_char++;				//없다는 뜻임으로 새로 노드를 AddChild_for_line_up을 사용해서 추가한다.
						AddChild_for_line_up(temp, story[i]);
						break;
					}
				}
			}
		}
		
		sort_nodes();								//나열한 노드를 빈도수를 기준으로 sorting한다.
		
		while (root->line_up->line_up != NULL) {				//노드가 하나가 남을 때까지 가장 작은 노드를 합쳐준다.
			temp = root;
			temp2 = (NODE*)(malloc(sizeof(NODE)));
			temp3 = (NODE*)(malloc(sizeof(NODE)));

			temp2 = temp;
			temp3 = temp2->line_up;
			combine(temp2, temp3);						//노드를 합쳐줄 때 combine함수를 호출한다.
			sort_nodes();							//합친 이후 생긴 노드와 같이 모든 노드를 다시 sorting한다.
		}




		for (i = 0; i < cnt; i++) {						//story에 담겨있는 모든 char를 거치면서 해당하는 이진 코드를 resul 배열에 담는다.
			temp = root->line_up;
			DFS(temp, story[i], temp_string, temp_string_length);
		}



		strcpy(result_filename, filename);					//출력파일 이름을 생성한다.
		strcat(result_filename, ".zz");
		fp2 = fopen(result_filename, "wb");
		
		fprintf(fp2, "%d %d %c%c", result_size, num_of_char,'A','B');		//출력파일에 이진코드의 크기와, 암호화된 char의 개수, 그리고 쓰레기 값 두개를 넣는다.
											//이때 쓰레기 값은, decompressing에서 생기는 오류를 막아준다.
		temp = root->line_up;							
		temp_string_length=0;
		DFS_fprintf(temp, temp_string, temp_string_length);			//암호화된 char의 data와 이진코드를 파일에 출력파일에 넣는다.

		if (result_size % 8 != 0) {						//만약 result의 총길이가 8의 배수가 아니라면 부족한 만큼 0으로 채워넣는다.
			temp_result_size = result_size;
			for (i = 0; i < (8 - temp_result_size % 8); i++) {
				result = (char*)realloc(result, sizeof(char) * (result_size + 1));
				result[result_size] = '0';
				result_size++;
			}				
		}

		for (i = 0; i < (result_size / 8); i++) { 				//모든 result를 8개씩 unsigned char에 저장하여 출력파일에 출력한다.
			store = make_decade(i * 8, (i + 1) * 8);
			fprintf(fp2, "%c", store);
		}
		free(result);
		
		printf("compressing end\n");
	}


	//-------------------------------------------------------------------decompressing하는 부분
	else if (c_d == 'd') {
	    	printf("decompressing start\n");
		
		
		initTree(128);					//root 선언
		
		fp = fopen(filename, "rb");			//파일을 연다.
		if(fp==NULL){
		    printf("no file\n");
		    return 0;
		}
		
		fscanf(fp, "%d %d ", &temp_result_size, &num_of_char);	//압축 파일 맨 앞의 전체 이진코드로 풀었을 때의 크기와, 저장된 암호화된 char의 개수를 저장한다.

		for (i = 0; i < num_of_char+1; i++) {			//저장되어 있는 암호화된 char들을 저장하여 트리형태로 연결한다.
		    	
				char tempp;
				fscanf(fp, "%c%c", &temp_data,&tempp);	//char두개를 받는다, 앞의 값은 data, 두 번째 값은 그 data가 암호화된 이진수의 길이이다.
				temp_string_length=(int)tempp-'0';	
				if (pass == 0) {			//처음에 바로 암호화된 char를 받으면 못 받는 char가 있기 때문에 앞에서 저장해준 쓰레기값을 대신 받고 넘긴다.
					pass = 1;
				}
				else {
					for (j = 0; j < temp_string_length; j++)		//data에 해당하는 이진코드를 저장한다.
						fscanf(fp, "%c", &temp_string[j]);

					temp = root;
					AddChild(temp, temp_data, temp_string, temp_string_length, 0);	//저장한 이진코드를 바탕으로 data에 해당하는 tree에서의 자리를 찾아간다.
				}
		
		}
		
		temp_result = (unsigned char*)malloc(sizeof(unsigned char)*temp_result_size); //temp_result에는 압축파일의 char을 그대로 받는다.
		result=(char*)malloc(sizeof(char));
		
		if(temp_result_size%8==0){				//전체 이진코드의 길이가 8의 배수라면 temp_result_size/8만큼 읽어온다.
		    for(i=0;i<(temp_result_size/8);i++)
			fscanf(fp,"%c",&temp_result[i]);
		}
		else{							//전체 이진코드의 길이가 8의 배수가 아니라면 1을 더해서 부족한 부분도 읽어준다.
		    for (i = 0; i < (temp_result_size / 8)+1; i++)
			fscanf(fp, "%c", &temp_result[i]);

		}

		strcpy(result_filename, filename);			//출력파일을 연다.
		strcat(result_filename, ".yy");
		fp2 = fopen(result_filename, "w");
		
		if(temp_result_size%8==0){				//전체 이진코드의 길이가 8의 배수라면 temp_result_size/8만큼 실행한다.
			for(i=0;i<(temp_result_size/8);i++)
				make_binary(temp_result[i]);		//char을 이진코드로바꾼다.또한 그 값을 result에 저장한다.
		}
		else{
			for(i=0;i<(temp_result_size/8)+1;i++)		//+1만큼 실행하며
				make_binary(temp_result[i]);		//char을 이진 코드로 바꾼다. 그 값을 result에 저장한다.
		}

		while (1) {
			temp = root;					
			DFS_find_char(temp);				//DFS_find_char함수를 재귀 호출하여 result의 모든 값들에 해당하는 char을 출력파일에 출력한다.

			if (transfer == result_size)
				break;
		}


		printf("decompressing end\n");
		
	}



	else{
	    printf("wrong input\n");
	    return 0;
	}

}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main(int argc, char* argv[]) {
	
		
	int n, m,i,j,left,right;					//n,m --> 행과 열의 갯수 / left,right --> O(n^3)에서 좌측,우측columnt 정하기위한 변수
	int i_s, j_s, i_e, j_e;						// O(n^6), O(n^4)에서 시작점과 끝점을 나타내기위한 변수
	int** table;							// 읽어온 배열을 저장하는 배열
	int* kadane;							// O(n^3)에서 사용하는 1차원 배열

	int algorithm;							//algorithm index
	int sum=0;							//최종 합	
	int temp_sum=0;							//임시 합

	int initial = 0;						//sum이 비어있는지 확인해주는 값
	int temp_initial = 0;						//temp_sum이 비어있는지 확인해주는 값

	int temp=0;								
	int flag = 0;							
									
	char filename[200];						//input파일 (200자 이하)
	char result_filename[207];					//output파일

	clock_t start_time=clock();					//시간 측정을 위한 start, end,
	clock_t end_time;						//시간 측정을 시작한다.
	

	
	if(argc != 3){							//input이 2개가 아닐때
		printf("wrong input");
		return 0;
	}
	
	strcpy(filename, argv[1]);					//filename과 algorithm에 값을 넣어준다
	algorithm=atoi(argv[2]);
	

	FILE* fp = fopen(filename, "r");					//file을 열고 n,m을 입력받는다
	fscanf(fp,"%d %d", &n, &m);
										//배열들을 동적할당한다.
	kadane = (int*)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
		kadane[i] = 0;

	table = (int**)malloc(sizeof(int*) * n);			
	for (i = 0; i < n; i++)
		table[i] = (int*)malloc(sizeof(int) * m);


	if (algorithm == 1) {                                   		//O(n^6)인 경우
						
		for (i = 0; i < n; i++)						//table에 배열을 그대로 저장한다.						  
			for (j = 0; j < m; j++)
				fscanf(fp, "%d", &table[i][j]);			
		
		for (i_s = 0; i_s < n; i_s++)                         			//시작점과 끝점의 x,y좌표를 지정하는 4중 for문
			for (j_s = 0; j_s < m; j_s++)                      
				for (i_e = 0; i_e < n; i_e++)              
					for (j_e = 0; j_e < m; j_e++) {            
						for (i = i_s; i <= i_e; i++)          	//구간 내의 모든 수들의 합을 임시저장한다(temp_sum).
							for (j = j_s; j <= j_e; j++) {  
								temp_sum += table[i][j];          
								flag = 1;               //flag --> 한번이라도 temp_sum이 더해졌는지 확인 
							}
						if (initial == 0) {			//sum의 값이 비어있으면sum에temp_sum을 넣어준다.
							sum = temp_sum;
							initial = 1;
						}
						if (temp_sum >= sum && flag==1)          //sum보다 temp_sum이 크면 sum에 넣는다.
							sum = temp_sum;                        
						temp_sum = 0;				//변수들을 초기화해준다.
						flag = 0;
					}
	}



	else if (algorithm == 2) {							//O(n^4)인 경우					  
		for (i = 0; i < n; i++)							//table에 값을 저장할때 
			for (j = 0; j < m; j++) {					//(0,0)을 한 꼭지점으로 갖고, (i,j)를 한 꼭지점으로 갖는
				fscanf(fp, "%d", &temp);				//사각형 내부의 값들의 합을 table[i][j]에 저장한다.
				if (i == 0 && j == 0)					//i,j가 0일때, i-1,j-1은  없는 배열을 호출 할 수 있기 때문에 따로 지정해준다.
					table[0][0] = temp;				
				else if (i == 0)
					table[0][j] = temp + table[0][j - 1];
				else if (j == 0)
					table[i][0] = temp + table[i - 1][0];
				else
					table[i][j] = temp + table[i][j - 1] + table[i - 1][j] - table[i - 1][j - 1];
			}

		for(i_s=0;i_s<n;i_s++)							//시작점과 끝점의 x,y 좌표를 지정한다.
			for(j_s=0;j_s<m;j_s++)
				for(i_e=0;i_e<=i_s;i_e++)
					for (j_e = 0; j_e <= j_s; j_e++) {


						if (i_e == 0 && j_e == 0) {		//끝점에 저장된 값과 시작점에 저장된 값으로	
							temp_sum = table[i_s][j_s];	//그 사이의 값들을 유추할 수 있다.
							flag = 1;			//(끝점에 저장된 값)-(끝점의 i,시작점의j)-(끝점의j,시작점의 i) + (시작점에 저장된값)
						}					//i,j가 0일때 , i-1,j-1은 없는 배열을 호출 할 수있기 때문에 따로 지정해준다.
						else if (i_e == 0) {																
							temp_sum = table[i_s][j_s] - table[i_s][j_e-1];
							flag = 1;
						}
						else if (j_e == 0) {																
							temp_sum = table[i_s][j_s] - table[i_e-1][j_s];
							flag = 1;
						}
						else {																				
							temp_sum = table[i_s][j_s] - table[i_e - 1][j_s] - table[i_s][j_e - 1] + table[i_e - 1][j_e - 1];
							flag = 1;
						}
						if (initial ==0) {					//sum이 비어있으면 temp_sum을 대입해준다.
							sum = temp_sum;
							initial = 1;
						}
						if (temp_sum >= sum && flag==1)				//temp_sum이 더 크면 sum에 대입해준다,
						    sum = temp_sum;
						temp_sum = 0;						//변수를 초기화시켜준다.
						flag = 0;
					}
	}


	else if (algorithm == 3) {					//O(n^3)일경우

		for (i = 0; i < n; i++)						//table에값을 그대로 저장해준다.								  
			for (j = 0; j < m; j++)
				fscanf(fp, "%d", &table[i][j]);


		for (left = 0; left < m; left++) {			//좌측끝으로 삼을 column을 하나 지정한다.				   
			for (i = 0; i < n; i++)				//1차원 배열을 초기화해준다.
				kadane[i] = 0;
			
			for (right = left; right < m; right++) {	//우측끝으로 삼을 column을 하나 지정한다.			
				for (i = 0; i < n; i++) {		//일차원 배열에 구간내 같은 column값을 가진 row를 다 더해준다.
					kadane[i] += table[i][right];
				}
				temp = 0;				//변수들을 초기화해준다.
				temp_sum=0;
				temp_initial = 0;

				for (i = 0; i < n; i++) {		//1차원 배열내에서 연속하는 최대합을 구한다.		
					temp += kadane[i];		//temp에 1차원배열의 값들을 계속 더해준다.
					if (temp_initial == 0) {	//temp_sum이 비어있다면 temp를 대입해준다.
						temp_sum = temp;
						temp_initial = 1;
						if(temp<0)		//temp가 0보다 작으면 0으로 초기화해준다,
							temp=0;
					}
					else if (temp > temp_sum)	//temp_sum이 sum보다 크면 sum에 대입해준다.
						temp_sum = temp;
					if (temp < 0)			//temp가 0보다 작으면 0으로 초기화해준다.
						temp = 0;
				}

				if (initial == 0) {			//1차원 배열에서 구한 최댓값을 sum이 비어있으면 넣어준다.
					sum = temp_sum;
					initial = 1;
				}
				else if (temp_sum > sum)		//1차원 배열에서 구한 최댓값이 sum보다 크면 넣어준다.
					sum = temp_sum;
			}
		}

	}


	else								//algorithm index의 값이 틀릴때 에러 문구를 출력한다.
		printf("Wrong input\n");
	end_time = clock();						//종료시간을 측정한다.
	fclose(fp);							//파일을 닫는다
	
	strcpy(result_filename,"result_");				//출력파일의 이름을 만든다.
	strcat(result_filename,filename);
	
	fp=fopen(result_filename,"w");					//출력파일을 만들고 연다.
	
	fprintf(fp,"%s\n",filename);					//출력파일에 입력파일을 출력
      	fprintf(fp,"%d\n",algorithm);					//출력파일에 algorithm index 출력
	fprintf(fp,"%d\n",n);						//출력파일에 n을 출력
	fprintf(fp,"%d\n",m);						//출력파일에 m을 출력
	fprintf(fp,"%d\n",sum);						//출려파일에 총합을 출력
	fprintf(fp,"%.6f\n",(double)(end_time-start_time)/CLOCKS_PER_SEC);		//출려파일에 총 소요시간 출력

	return 0;
}

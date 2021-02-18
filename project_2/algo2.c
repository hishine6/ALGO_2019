#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

int n;
static int size_threshold = 50;				//best sort에서 n의 길이가 이 값보다 작으면 insertion sort를 실시한다.

void SWAP(int* a, int* b) {					//a와 b를 받아서 그 값을 스왑한다.
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int find_pivot(int* A, int left, int right) {	//A행렬에서 left, right 값을 받아서 left,right,(left+right)/2에 해당하는 행렬 값을 비교하여 중간 값을 반환한다.
	int middle = (right + left) / 2;
	if (A[left] < A[right]) {
		if (A[left] > A[middle])
			return left;
		else {
			if (A[right] > A[middle])
				return middle;
			else
				return right;
		}
	}
	else {
		if (A[left] > A[middle]) {
			if (A[middle] < A[right])
				return right;
			else
				return middle;
		}
		else
			return left;
	}
}

int make_pivot(int* A, int left, int right) { //quick sort를 실시하고 pivot값을 반환한다.
	int i, pivot;
	int temp;
	pivot = left;
	temp = find_pivot(A, left, right);		//pivot으로 사용할 수를 찾는다.
	SWAP(&A[temp], &A[right]);				//pivot으로 사용할 수와 right에 해당하는 값을 SWAP한다.
	for (i = left; i < right; i++) {		//left에서 right까지 값들을 비교하여 pivot보다 작은 값을 pivot의 좌측에 배치한다.
		if (A[i] < A[right]) {
			SWAP(&A[i], &A[pivot]);
			pivot++;
		}
	}
	SWAP(&A[right], &A[pivot]);				//pivot 값과 right값을 스왑한다.
	return(pivot);
}

void quick_sort(int* A, int left, int right) {	//quick sort를 실질적으로 실행한다.
	int pivot;
	if (right - left > 0) {
		pivot = make_pivot(A, left, right);		//pivot의 값을 반환 받는다.
		quick_sort(A, left, pivot - 1);			//left와 pivot사이의 행렬을 quick sort한다.
		quick_sort(A, pivot + 1, right);		//pivot과 right 사이의 행렬을 quick sort한다.
	}
}

void merge(int* A, int left, int middle, int right) {	//merge sorting을 실질적으로 실행하는 부분이다.
	int i, i_left, i_right;
	int* temp;											//A의 값중 left와 right사이의 정렬할 값들을 temp에 임시 저장한다.
	temp = (int*)malloc(sizeof(int) * n);
	for (i = left; i <= right; i++)
		temp[i] = A[i];
	
	i_left = left;										
	i_right = middle + 1;
	i = left;

	while ((i_left <= middle) && (i_right <= right)) {	//i_left~middle 사이의 값과 i_right~right 사이의 값을 비교하여 작은 값을 순서에 맞게 넣어준다.
		if (temp[i_left] < temp[i_right])
			A[i++] = temp[i_left++];
		else
			A[i++] = temp[i_right++];
	}

	while (i_left <= middle)							//i_left~middle 또는 I_right~right 사이의 값이 더 이상 존재하지 않으면 나머지 값들을 순서대로 넣어준다.
		A[i++] = temp[i_left++];
	while (i_right <= right)
		A[i++] = temp[i_right++];
}

void merge_sort(int* A, int left, int right) {			//정렬하고하 하는 배열을 2등분하여 merge_sort를 재귀호출한다.
	int middle;

	if (left < right) {
		middle = (left + right) / 2;

		merge_sort(A, left, middle);
		merge_sort(A, middle + 1, right);

		merge(A, left, middle, right);					//정렬된 작은 두 배열을 하나의 정렬된 큰 배열로 합친다.
	}
}

void insertion_sort(int* A,int left, int right){		//insertion sort를 실행한다.
	int i, j, key;										//key값은 현재 sorting 하는 요소를 뜻한다.
	for (i = left; i <= right; i++) {					//sorting을 하는 구간 내에서~
		j = i;											
		key = A[i];
		while (j != left && key < A[j - 1]) {			//소팅을 하고자 하는 요소가 앞의 요소보다 작으면 다음 요소와 비교를 하고, 크면 그 자리에 대입한다.
			A[j] = A[j - 1];
			j--;
		}
		A[j] = key;										
	}
}



void best_sort(int* A, int left, int right, int depth_limit) { //best sorting 을 실행한다.
	int temp;													//pivot을 저장한다.
	while(right - left > size_threshold) {						//소팅하는 구간이 50보다 클 경우 while문이 반복된다.
		if (depth_limit == 0) {									//depth_limit이 0이면 merge_sort를
			merge_sort(A, left, right);
		}
		else {													//depth_limit이 0이 아니면 quick-sort를 실시한다.
			depth_limit--;
			temp = make_pivot(A, left, right);					//quick sort를 실시하는 부분.
			best_sort(A, temp, right, depth_limit);				//best_srot를 재귀호출한다.
			right = temp;
		}
	}
	insertion_sort(A,left, right);								//소팅 구간이 50보다 작을 경우 insertion_sort를 실시한다.
}


int main(int argc, char* argv[]) {

	int i, j;

	int algorithm;												//algorithm index를 저장한다.
	int* sort;													//정렬하고자하는 list를 저장하는 배열이다.
	char filename[200];											//input 파일 이름을 저장한다.
	char result_filename[207];									//output 파일 이름을 저장한다.

	clock_t start_time;											//시간 측정구간의 시작시간을 저장한다.
	clock_t end_time;											//시간 측정 구간의 끝 시간을 저장한다.



	if (argc != 3) {											//input형식이 잘못되면 오류를 출력한다.
		printf("wrong input");
		return 0;
	}

	strcpy(filename, argv[1]);									//input파일을 입력받는다.
	algorithm = atoi(argv[2]);									//algorithm 변수에 algorithm index를 저장한다.


	FILE* fp = fopen(filename, "r");							//input file을 읽는다.
	fscanf(fp, "%d", &n);
	sort = (int*)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
		fscanf(fp, "%d", &sort[i]);								//정렬하고자 하는 배열을 저장한다.

	start_time = clock();										//시작시간을 저장한다.

	if (algorithm == 1) {										//algorithm 이 1이면 insertion sort를 실행한다.
		insertion_sort(sort,0,n-1);
	}
	else if (algorithm == 2) {									//algorithm 이 2이면 quick sort를 실행한다.
		quick_sort(sort, 0, n - 1);
	}
	else if (algorithm == 3) {									//algorithm 이 3이면 merge sort를 실행한다.
		merge_sort(sort, 0, n - 1);
	}
	else if (algorithm == 4) {									//algorithm 이 4이면 직접 만든 best sort를 실행한다.
		best_sort(sort, 0, n - 1, 2*(int)(sqrt(n)/sqrt(2)));
	}
	else														//그 외의 값에 대해서는 오류를 출력한다.
		printf("Wrong input\n");

	end_time = clock();											//끝나는 시간을 저장한다.
	fclose(fp);													//input파일을 닫는다.

	
	strcpy(result_filename, "result_ ");						//output file을 작성한다.
	result_filename[7]='0'+algorithm;							
	strcat(result_filename,"_");
	strcat(result_filename, filename);

	fp = fopen(result_filename, "w");							//output file을 write로 오픈한다.

	fprintf(fp, "%s\n", filename);								//output file에 inputfile 이름을 입력한다.
	fprintf(fp, "%d\n", algorithm);								//output file에 algorithm을 입력한다.
	fprintf(fp, "%d\n", n);										//output file에 총 list의 길이를 입력한다.
	fprintf(fp, "%.6f\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);		//총 실행시간을 입력한다.
	for(i=0;i<n;i++)											//sorting된 list를 입력한다.
		fprintf(fp,"%d ",sort[i]);

	return 0;
}

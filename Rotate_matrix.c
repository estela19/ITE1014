#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
void print_matrix(int** matrix, int angle);
int** rotate(int** matrix);
int** newmat(int row, int col);

int main() {
	int row, col;
	
	// 행렬의 크기를 입력받음
	printf("Input row size of the matrix : ");
	scanf("%d", &row);
	printf("Input column size of the matrix : ");
	scanf("%d", &col);

	//크기에 맞는 새로운 행렬을 생성
	int** mat = newmat(row, col);
	
	//n은 각도
	int n = 0;
	while (n < 4) {
		print_matrix(mat, n * 90);
		//만든 행렬을 회전
		mat = rotate(mat);
		n++;
	}

	//메모리 정리
	for (int i = 0; i < row; i++) {
		free(mat[i]);
		mat[i] = 0;
	}
	free(mat);
	mat = 0;

	return 0;
}

//새로운 행렬을 만드는 함수
int** newmat(int row, int col) {
	//크기에 맞는 이차원 포인터 생성 및 동적할당
	int** mat = NULL;
	mat = (int**)malloc((row) * sizeof(int*));

	//만든 이차원 포인터배열에 일차원 포인터 배열을 만듬
	int num = 0;
	for (int i = 0; i < row; i++) {
		mat[i] = (int*)malloc((col) * sizeof(int));
		
		//행렬에 숫자를 채워 넣음
		for (int j = 0; j < col; j++)
		{
			num++;
			mat[i][j] = num;
		}
	}
	return mat;
}


int** rotate(int** matrix) {
	int rowsize, colsize;
	
	//가로와 세로의 크기를 구함
	rowsize = _msize(matrix) / sizeof(int*);
	colsize = _msize(matrix[0]) / sizeof(int);

	//회전하면 가로와 세로의 크기가 변하므로 가로세로 반대로 행렬 생성
	int** new_matrix;
	new_matrix = newmat(colsize, rowsize);

	//만든 행렬을 90도 회전
	for (int i = 0; i < rowsize; i++) {
		for (int j = 0; j < colsize; j++) {
			new_matrix[j][rowsize - i - 1] = matrix[i][j];
		}
	}

	//원래 있던 행렬 할당해제
	for (int i = 0; i < rowsize; i++) {
		free(matrix[i]);
		matrix[i] = 0;
	}
	free(matrix);
	matrix = 0;
	return new_matrix;
}

//행렬 출력함수
void print_matrix(int** matrix, int angle) {
	int rowsize, colsize;
	
	//행렬의 가로 세로 크기를 구함
	rowsize = _msize(matrix) / sizeof(int*);
	colsize = _msize(matrix[0]) / sizeof(int);

	//구한 크기를 바탕으로 각각의 칸 출력
	printf("< degree : %d >\n", angle);
	for (int i = 0; i < rowsize; i++) {
		for (int j = 0; j < colsize; j++) {
			printf("%4d", matrix[i][j]);
		}
		printf("\n");
	}
}
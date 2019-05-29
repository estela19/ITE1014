#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <time.h>

typedef struct lotto_ {
	int now_size;
	int rank_n[10];
	int* rank_m;
	int* correct_num;
	int** lotto_buy;

}lotto_set;

//사용하는 함수에 대한 선언들

int printMenu(lotto_set* lotto);
int menu1(lotto_set* lotto);
int menu2(lotto_set* lotto);
int menu3(lotto_set* lotto);
int menu4(lotto_set* lotto);
int end_lotto(lotto_set* lotto);
int rank(int* numbers, int* correct_numbers);
int* correctnum(void);
int* newTicket(void);
int (*menu[])(lotto_set*) = { end_lotto, menu1, menu2, menu3, menu4, printMenu };

int main(void)
{
	lotto_set lotto;
	lotto_set* plotto = &lotto;
	lotto.now_size = 0;
	for (int i = 0; i < 10; i++) lotto.rank_n[i] = 0;
	lotto.lotto_buy = (int**)malloc(sizeof(int*) * 0);
	int rank_money[7] = { 0, 2000000000, 100000000, 2000000, 50000, 5000, 0 };
	lotto.rank_m = rank_money;
	lotto.correct_num = correctnum();

	//시간에 따라 난수생성방법 변경
	srand(time(NULL));
	int tmp = 5;
	while (tmp != -1) tmp = menu[tmp](plotto);

	return 0;
}

// 구매한 티켓의 6개의 숫자와 정답 숫자를 비교해 등수를 리턴해주는 함수
int rank(int* numbers, int* correct_numbers)
{
	//지금까지 맞힌 번호의 수
	int cnt = 0;
	//보너스 번호
	int bonus = 0;
	int i, j;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 6; j++)
		{
			//받은 숫자와 정답 숫자가 같으면 cnt추가
			if (numbers[i] == correct_numbers[j]) cnt++;
		}
	}
	for (i = 0; i < 6; i++)
	{
		//만약 보너스 번호가 맞다면 bonus에 1 대입
		if (correct_numbers[6] == numbers[i])
			bonus = 1;
	}
	//꽝 이라면 등수는 6
	int rankk = 6;
	if (cnt == 6) rankk = 1;
	else if (cnt == 5 && bonus == 1) rankk = 2;
	else if (cnt == 5 && bonus == 0) rankk = 3;
	else if (cnt == 4) rankk = 4;
	else if (cnt == 3) rankk = 5;

	return rankk;
}

//메뉴를 출력하는 함수
int printMenu(lotto_set* lotto){
	printf("-------------Lottery Ticket-----------------\n"
		"--------------------------------------------\n"
		"1. Buy tickets\n"
		"2. Spent money\n"
		"3. Earned money\n"
		"4. Buy until I get 4th or higher prize\n"
		"0. exit\n"
		"--------------------------------------------\n"
		">>>");

	int choice_menu = 0;
	scanf("%d", &choice_menu);

	return choice_menu;
}

int menu1(lotto_set* lotto) {
	//구입할 티켓 수
	int how_many;
	//rank함수에서 리턴되는 등수롤 받는 변수
	int rank_;

	printf("How many lines will you buy? : ");
	scanf("%d", &how_many);
	//총 구입 수에 지금 구매한 티켓 수 추가
	(*lotto).now_size += how_many;

	//전체 구입한 로또 번호 배열을 새로 구입할 크기만큼 추가해 재할당
	lotto->lotto_buy = (int**)realloc(lotto->lotto_buy, sizeof(int*) * lotto->now_size);
	for (int i = lotto->now_size - how_many; i < lotto->now_size; i++)
	{
		//빈 배열에 순서대로 번호 6개 생성
		lotto->lotto_buy[i] = newTicket();
		//새로 생성한 번호의 등수 리턴후 전체 등수별 당첨 횟수에 추가
		rank_ = rank(lotto->lotto_buy[i], lotto->correct_num);
		lotto->rank_n[rank_]++;
		//방금 구입한 로또의 번호 출력
		for (int j = 0; j < 6; j++)
		{
			printf("%4d", lotto->lotto_buy[i][j]);
		}
		//만약 꽝이 아니면 등수 출력
		if (rank_ != 6) printf("  rank : %d \n", rank_);
		else printf("\n");
	}
	return 5;
}

int menu2(lotto_set* lotto) {
	//현재까지 구입한 티켓 수와 총 금액 출력
	printf("You bought %d tickets, and the total cost is %d won\n", lotto->now_size, lotto->now_size * 1000);
	return 5;
}

int menu3(lotto_set* lotto) {
	//각 등수별 당첨횟수와 그에따른 상금 출력
	//총 상금
	int total = 0;
	for (int j = 1; j < 6; j++)
	{
		printf("you get <rank %d> tickets %d times, you get %d money!!\n", j, lotto->rank_n[j], lotto->rank_n[j] * lotto->rank_m[j]);
		total += lotto->rank_n[j] * lotto->rank_m[j];
	}
	printf("So you get total %d money\n", total);
	return 5;
}

int menu4(lotto_set* lotto) {
	//입력을 저장하는 배열
	char input[10];
	//menu4를 실행했을때 각 등수가 몇변 나왔는지 기록하는 변수
	int rank_n4[10] = { 0, };

	printf("Are you sure? (y or n) : ");
	scanf("%s", input);
	//만약 입력 받은 것이 y이면 실행
	if (!strcmp(input, "y"))
	{
		//menu4에서 티켓을 구매한 횟수를 저장하는 변수
		int n4 = 0;
		int rank_ = 5;
		while (rank_ > 4)
		{
			//한번 구입시 마다 지금까지 구입한 전체횟수 증가
			lotto->now_size++;
			//한번 구입시 마다 menu4에서 구입 횟수 증가
			n4++;
			//전체 로또 번호를 저장하는 배열을 재할당
			lotto->lotto_buy = (int**)realloc(lotto->lotto_buy, sizeof(int*) * lotto->now_size);
			//새로만든 배열에 새로운 6개 번호 생성해 전달
			lotto->lotto_buy[lotto->now_size - 1] = newTicket();
			//새로 만든 수의 당첨여부 확인
			rank_ = rank(lotto->lotto_buy[lotto->now_size - 1], lotto->correct_num);

			//전체 횟수중 당첨횟수 카운트
			lotto->rank_n[rank_]++;
			//menu4중 당첨횟수 카운트
			rank_n4[rank_]++;

			//새로 구입한 로또 번호 출력
			for (int j = 0; j < 6; j++)
			{
				printf("%4d", lotto->lotto_buy[lotto->now_size - 1][j]);
			}
			//당첨된 등수가 있다면 출력, 꽝이라면 출력하지 않음.
			if (rank_ != 6)
				printf("   rank %d \n", rank_);
			else printf("\n");
		}

		//지금까지 산 티켓 수와 그 가격(티켓수*1000)출력
		printf("\nyou bought %d tickets, and the total cost is %d won\n", n4, n4 * 1000);
		//총상금 (등수별 당첨횟수*상금) 출력
		printf("your prize is %d won\n", rank_n4[5] * lotto->rank_m[5] + rank_n4[4] * lotto->rank_m[4] + rank_n4[3] * lotto->rank_m[3] + rank_n4[2] * lotto->rank_m[2] + rank_n4[1] * lotto->rank_m[1]);
	}
	return 5;
}

int end_lotto(lotto_set* lotto) {
	printf("Good bye!\n");

	//메모리 정리
	free(lotto->correct_num);
	for (int i = 0; i < lotto->now_size; i++)
	{
		free(lotto->lotto_buy[i]);
		lotto->lotto_buy[i] = 0;
	}
	free(lotto->lotto_buy);
	lotto->lotto_buy = 0;

}

//새로운 티켓을 만드는 함수
int* newTicket(void)
{
	//동적할당으로 6칸짜리 배열 생성
	int* num = (int*)malloc(sizeof(int) * 6);

	//1~45의 숫자를 확인할수 있는 숫자보드 생성 후 초기화
	int num_board[46];
	memset(num_board, 0, sizeof(num_board));

	//현재까지 만든 숫자의 개수
	int cnt = 0;
	while (cnt < 6)
	{
		int a = rand() % 45 + 1;
		//만약 num_board[a]가 0이면 즉 한번도 생성한적 없는 숫자면 그 수를 생성
		if (num_board[a] == 0)
		{
			num_board[a]++;
			cnt++;
		}
	}

	//생성 한 수를 오름차순으로 읽으며 num에 정렬해 넣음
	int n = 0;
	int i;
	for (i = 1; i < 46; i++)
	{
		if (num_board[i] == 1)
		{
			num[n] = i;
			n++;
		}
	}
	return num;
}
//당첨 번호 생성하는 함수
int* correctnum(void)
{
	//1~45까지의 숫자를 확인할 수 있는 숫자 배열 생성 후 초기화
	int num[46] = { 0, };
	//만들어야 하는 번호 수
	int n = 6;
	while (n)
	{
		srand(time(NULL));
		int a = rand() % 45 + 1;
		//만약 num[a]가 0, 즉 생성한 적 없는 숫자면 생성
		if (num[a] == 0)
		{
			num[a]++;
			n--;
		}
	}
	//새로 만든 숫자를 넣을 배열 생성
	int* numbers = malloc(sizeof(int) * 7);
	//1부터 차례대로 읽으며 생성된 숫자를 new배열에 추가
	int b = 0;
	int i;
	for (i = 1; i < 46; i++)
	{
		if (num[i] == 1)
		{
			numbers[b] = i;
			b++;
		}
	}

	//보너스 숫자 생성
	//만약 보너스 숫자가 만든 적 없는 숫자면 생성
	do
	{
		b = rand() % 45 + 1;
	} while (num[b] != 0);
	numbers[6] = b;
	return numbers;
}

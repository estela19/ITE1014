#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>

typedef struct _time {
	int minute;
	int second;
}Time;

typedef struct _song {
	char title[100];
	char artist[100];
	Time length;
}Song;

typedef struct _songList {
	Song *songs;
	int size;
	int capacity;
}Songlist;

//사용하는 함수들에 대한 선언
Songlist dataload(void);
int datasave(Songlist*);
int printmenu(Songlist*);
int printsong(Song*);
void printsongmenu(Songlist*, Song*);
Song* searchsong(Songlist*, char*, char*);
int editsong(Song*);
int deletesong(Songlist*, Song*);
int addsong(Songlist*);
int printallsongs(Songlist*);
int deleteall(Songlist*);
int searchmenu(Songlist*);

//for문 사용 변수
int i, j, k;
//전체 메뉴 함수포인터배열 (5는 메뉴출력)
int(*funcs[])(Songlist*) = { datasave, searchmenu, addsong, printallsongs, deleteall, printmenu };

int main() {
	printf("-----------Mini Music Library-----------\n\n");
	//dataload하며 생성한 구조체를 songlist 라는 구조체에 받음
	Songlist songlist = dataload();
	Songlist* p_songlist = &songlist;
	// 기본메뉴는 5(메뉴출력)
	int state = 5; 
	//함수의 리턴값이 -1이 아닐때까지 그 함수에서 반환하는 int 값이 인덱스인 함수 실행
	while (state != -1) state = funcs[state](p_songlist);
	//state가 -1이면 종료한다. 그전에 동적할당한 songlist 할당해제
	free(songlist.songs);
	return 0;
}

//내가 찾는 곡이 songlist에 존재하면 그 구조체포인터를 반환하고 없으면 NULL을 반환하는 함수
Song* searchsong(Songlist* p_songlist, char* title, char* artist) {
	//songlist의 capacity만큼 반복하면서
	for (i = 0; i < p_songlist -> capacity; i++) {
		//songlist의 곡이 입력한 타이틀과 같고
		if (!strcmp(p_songlist->songs[i].title, title))
			//아티스트도 같으면서
			if (!strcmp(p_songlist->songs[i].artist, artist))
				//곡 길이도 0이 아니면
				if (p_songlist->songs[i].length.minute != 0 && p_songlist->songs[i].length.second != 0) {
					//그 song 구조체 포인터 반환
					Song* p_song = &(p_songlist->songs[i]);
					return p_song;
				}
	}
	return NULL;
}

//printmenu에서 search를 선택하면 호출되는 함수. 해당 곡이 있는지 없는지 확인한다
int searchmenu(Songlist* p_songlist) {
	char title[100];
	char artist[100];
	printf("Input title : ");
	scanf(" %[^\n]s ", title);
	printf("Input artist : ");
	scanf(" %[^\n]s ", artist);

	int result;
	//해당곡이 있는지 검색
	Song* p_song = searchsong(p_songlist, title, artist);
	//해당 곡이 없으면
	if (p_song == NULL) {
		printf("<ERROR> Can't find song.\n");
		//5를 리턴해서 printmenu로 돌아감
		return 5;
	}
	//해당 곡이 있으면
	else {
		//곡 정보를 출력하고 printsongmenu호출
		printsong(p_song);
		printsongmenu(p_songlist, p_song);
	}
}

//선택한 곡을 지우는 함수
int deletesong(Songlist* p_songlist, Song* p_song) {
	// 곡길이를 0으로 바꾸고
	p_song->length.minute = 0;
	p_song->length.second = 0;
	//곡 개수도 1줄인다
	p_songlist->size--;
	printf("...delete success\n");
}

//p_song에 해당하는 정보를 출력하는 함수
int printsong(Song* p_song) {
	printf("\nTitle : %s\n", p_song->title);
	printf("Artist : %s\n", p_song->artist);
	printf("Length : %d min %d sec\n", p_song->length.minute, p_song->length.second);
	printf("\n");
}

//곡을 추가하는 함수 (2.newsong일때 실행됨)
int addsong(Songlist* p_songlist) {
	Song* check;
	char new_title[100];
	char new_artist[100];
	int new_minute = 0;
	int new_second = 0;
	printf("Input title : ");
	scanf(" %[^\n]s", new_title);
	printf("Input artist : ");
	scanf( " %[^\n]s", new_artist);

	//입력받은 타이틀과 아티스트가 songlist에 존재하는지 확인
	check = searchsong(p_songlist, new_title, new_artist);

	//확인결과 없던 곡이면 길이를 입력받는다
	if (check == NULL) {
		printf("Input length  \n");
		printf("minute : ");
		scanf(" %d", &new_minute);
		printf("second : ");
		scanf(" %d", &new_second);

		//초가 60초를 넘거나 음수이면 에러를 출력하고 다시 입력받는다
		while (new_second < 0 || new_second > 59) {
			printf("<EROOR> Wrong second!\n");
			printf("Input new length\n");
			printf("minute : ");
			scanf("%d", &new_minute);
			printf("second : ");
			scanf("%d", &new_second);
		}

		//곡을 추가하기 위해 songs를 한칸 늘린다. 추가한 사실을 size를 증가시켜 표시한다.
		p_songlist->songs = realloc(p_songlist->songs, sizeof(Song) * (p_songlist->capacity + 1));
		p_songlist->capacity++;
		p_songlist->size++;
		//새로 만든 자리에 아티스트, 타이틀, 길이를 저장한다.
		Song * p_song = &p_songlist->songs[p_songlist->capacity - 1];
		strcpy(p_song->artist, new_artist);
		strcpy(p_song->title, new_title);
		p_song->length.minute = new_minute;
		p_song->length.second = new_second;
		printf("...add success\n");
	}
	//만약 이미 존재했던 곡이면 오류를 출력한다
	else printf("<ERROR> This song already exsit.\n");
	//5를 리턴해 printmenu로 돌아간다
	return 5;
}

//search 에서 찾은 곡을 수정하는 함수
int editsong(Songlist* p_songlist, Song* p_song) {
	printf("Input new title : ");
	scanf(" %[^\n]s", &p_song->title);
	printf("Input new artist : ");
	scanf(" %[^\n]s", &p_song->artist);
	printf("\nInput new length\n");
	printf("minute : ");
	scanf("%d", &p_song->length.minute);
	printf("second : ");
	int tmp;
	scanf("%d", &tmp);
	//60초를 넘거나 음수이면 에러를 출력하고 다시 입력받는다
	while (tmp < 0 || tmp > 59) {
		printf("<EROOR> Wrong second!\n");
		printf("Input new length\n");
		printf("minute : ");
		scanf("%d", &p_song->length.minute);
		printf("second : ");
		scanf("%d", &tmp);
	}
	p_song->length.second = tmp;
	printf("...Edit song succes\n");
}

//시작할때 데이터를 불러오는 함수
Songlist dataload(void) {
	//데이터를 받을 구조체를 생성한다
	Songlist songlist;
	FILE* f = NULL;
	f = fopen("example.txt", "r");
	fscanf(f, "%d", &songlist.size);
	//처음 파일을 열었을때는 size와 capacity가 같다.
	songlist.capacity = songlist.size;
	//곡의 개수만큼 메모리를 할당한다
	songlist.songs = (Song*)malloc(sizeof(Song) * songlist.size);
	//할당한 메모리에 곡을 집어 넣는다.
	for (i = 0; i < songlist.size; i++) {
		fscanf(f, "%d %d", &songlist.songs[i].length.minute, &songlist.songs[i].length.second);
		fscanf(f, " %[^\n]s", &songlist.songs[i].artist);
		fscanf(f, " %[^\n]s", &songlist.songs[i].title);
	}
	printf("...Data load success\n");
	//데이터를 받은 구조체를 리턴한다.
	return songlist;
}

//전체 곡을 지우는 함수
int deleteall(Songlist* p_songlist) {
	char ans[10];
	printf("Are you sure? (y or n) : ");
	scanf("%s", ans);
	//만약 입력이 y 였다면
	if (!strcmp(ans, "y")) {
		//전체 songs를 돌면서 곡 길이를 0으로 초기화한다
		for (i = 0; i < p_songlist->capacity ; i++) {
			p_songlist->songs[i].length.minute = 0;
			p_songlist->songs[i].length.second = 0;
		}
		//곡 개수도 0으로 바꾼다
		p_songlist->size = 0;
		printf("...Delete success\n");
	}
	//5를 리턴해 printmenu로 돌아간다
	return 5;
}

//메뉴를 출력하는 함수
int printmenu(Songlist* p_songlist) {
	char input[10] = { 0, };
	int menu = 0;
	printf("------------------------------------------\n"
		"1. Search\n"
		"2. New song\n"
		"3. Print all\n"
		"4. Delete all\n"
		"0. Exit\n"
		"-------------------------------------------\n"
		">>>");
	scanf("%s", input);
	// 문자열로 받아서 int형식으로 변환한다
	menu = atoi(input);
	//menu범위를 벗어났거나 문자였으면(변환값이 0이고 문자열의 첫번째가 0이아니면) 에러를 출력한다
	if (menu < 0 || menu > 4 ||(menu == 0 && input[0] != '0')) {
		printf("<ERROR> You can select 0~4\n");
		//5를 리턴해 printmenu를 다시 부른다
		return 5;
	}
	//입력받은 menu를 리턴해 해당 숫자의 함수를 호출한다
	return menu;
}

//search메뉴에서 해당곡을 찾았을때 메뉴를 출력하는 함수
void printsongmenu(Songlist* p_songlist, Song* p_song) {
	//search메뉴에서 각 선택에 해당하는 함수포인터배열
	int(*searchfuncs[])(Songlist * p_songlist, Song * p_song) = { printmenu, editsong, deletesong, printsongmenu };
	char input[10] = { 0, };
	int menu = 0;
	printf("----------------------------------------\n"
		"1. Edit\n"
		"2. Delete\n"
		"0. Previous\n"
		"----------------------------------------\n"
		">>>");
	//문자열로 입력받은 후 int로 변환
	scanf("%s", input);
	menu = atoi(input);
	//숫자 범위 밖이거나 문자였다면 에러 출력후 자기 자신 호출
	if (menu < 0 || menu > 2 ||(menu == 0 && input[0] != '0')) {
		printf("<ERROR> You can select 0~2\n");
		searchfuncs[3](p_songlist, p_song);
	}
	//입력이 정상적이었으면 해당 menu를 호출한 후 전체 메뉴(printmenu)호출
	searchfuncs[menu](p_songlist, p_song);
	searchfuncs[0](p_songlist, p_song);
}

//songlist에 존재하는 모든 곡들을 출력하는 함수
int printallsongs(Songlist* p_songlist) {
	//전체 songlist크기만큼 돌면서
	for (i = 0; i < p_songlist->capacity; i++) {
		//해당 곡의 길이가 0이 아니면 출력
		if(p_songlist->songs[i].length.minute != 0 || p_songlist->songs[i].length.second != 0)
			printf("%s - %s (%d : %d)\n", p_songlist->songs[i].title, p_songlist->songs[i].artist, p_songlist->songs[i].length.minute, p_songlist->songs[i].length.second);
	}
	//만약 곡이 없다면 nothing출력
	if (p_songlist->size == 0) printf("<Nothing>\n");
	//5를 리턴해서 printmenu로 돌아감
	return 5;
}

//종료전 현재 songlist에 있는 곡들을 저장하는 함수
int datasave(Songlist* p_songlist) {
	FILE* f = NULL;
	f = fopen("example.txt", "w");
	//곡 개수를 먼저 저장하고
	fprintf(f, "%d\n", p_songlist->size);
	//총 크기만큼 돌면서
	for (i = 0; i < p_songlist->capacity; i++) {
		// 곡 길이가 0이 아니면 저장
		if (p_songlist->songs[i].length.minute != 0 && p_songlist->songs[i].length.second != 0) {
			fprintf(f, "%d %d\n", p_songlist->songs[i].length.minute, p_songlist->songs[i].length.second);
			fprintf(f, "%s\n", p_songlist->songs[i].artist);
			fprintf(f, "%s\n", p_songlist->songs[i].title);
		}
	}
	printf("...Data save success\n");
	//-1을 리턴해서 프로그램이 종료 됬음을 알림.
	return -1;
}
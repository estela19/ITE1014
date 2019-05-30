#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

typedef struct _student {
	char name[4];
	unsigned int id;
	float score;
	char grade;
}student;

student* fillstudents( const int a);
void print_students(student*, const int a);
student onestudent(student* student_new, const int a);
char grade_check(float a);

int main() {
	srand(time(NULL));
	int n = 0;
	printf("Input numver of students : ");
	scanf("%d", &n);
	
	student* student_s = fillstudents(n);
	for (int i = 1; i <= n; i++)
		print_students(student_s, i);
	return 0;

}

student* fillstudents( const int a) {
	student* student_new = (student*)malloc(sizeof(student) * (a + 1));
	for (int i = 1; i <= a; i++) {
		student_new[i] = onestudent(student_new, i);
	}
	return student_new;
}

student onestudent(student* student_new, const int a) {
	char new_name[3] = { 0, };
	for (int i = 0; i < 3; i++) {
		new_name[i] = 'A' + rand() % 26;
		student_new[a].name[i] = new_name[i];
	}
	student_new[a].name[3] = '\0';

	student_new[a].id = 2019000000 + rand();

	int score = rand() % 450;
	student_new[a].score = (float) score / 100;
	student_new[a].grade = grade_check(student_new[a].score);
	return student_new[a];
}

void print_students(student* student_new, const int a) {
	printf("	studnet %d\n", a);
	printf("name  : %s\n", student_new[a].name);
	printf("id    : %d\n", student_new[a].id);
	printf("score : %.2f\n", student_new[a].score);
	printf("grade : %c\n", student_new[a].grade);
	printf("------------------------------\n");
}

char grade_check(float a) {
	if (a > 4.00) return 'A';
	else if (a > 3.00) return 'B';
	else return 'F';
}

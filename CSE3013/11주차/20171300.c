#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define OPEN 1
#define CLOSE 0
#define BIGCLOSE -1
#define wall -2
#define min(x,y) (x<y)?x:y
int row, col;
typedef struct maze_s {
	int num;
	int up;
	int down;
	int left;
	int right;
}mm;
void checking_line(int, int, mm**); //열마다 위아래 뚫기
void make_col(int, int, mm**); //행마다 뚫기 작업
void draw(mm**); //그냥 그리기
void final_make(mm**); //최종 행 뚫기
void chek_num(mm**); //숫자체크 함수
//void onemore(int, int, mm**); 때려친 함수
void make_file(mm**); //파일로 만들기
void main() {
	srand((unsigned int)time(NULL));
	mm** maze;
	scanf("%d %d", &row, &col);
	int i, j, count = 1, random;
	int nu;
	maze = (mm**)malloc(sizeof(mm*) * row);
	//배열 초기화
	for (i = 0; i < row; i++) {
		maze[i] = (mm*)malloc(sizeof(mm) * col);
		for (j = 0; j < col; j++) {
			maze[i][j].num = count;
			count++;
			maze[i][j].down = CLOSE;
			maze[i][j].up = CLOSE;
			maze[i][j].right = CLOSE;
			maze[i][j].left = CLOSE;
			if (i == 0)
				maze[i][j].up = wall;
			if (i == row - 1)
				maze[i][j].down = wall;
			if (j == 0)
				maze[i][j].left = wall;
			if (j == col - 1)
				maze[i][j].right = wall;
		}
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			//1행에선 마지막 열까지 검사 필요없음. 단지 오른쪽을 뚫을지 말지만 결정
			if (i == 0) {
				if (j == col - 1)//마지막열 불필요
					break;
				random = rand() % 2;
				maze[i][j].right = random;
				maze[i][j + 1].left = random;
				if (random == 1) {
					nu = min(maze[i][j + 1].num, maze[i][j].num);
					maze[i][j + 1].num = nu;
					maze[i][j].num = nu;
				}
			}
			else {
				//위아래 뚫을지 말지 결정하고
				random = rand() % 2;
				maze[i][j].up = random;
				maze[i - 1][j].down = random;
				if (random == 1) { //뚫릴시 숫자 동기화
					nu = min(maze[i - 1][j].num, maze[i][j].num);
					maze[i - 1][j].num = nu;
					maze[i][j].num = nu;
				}
				else {
					//안뚫렸을시, 만약 경계선이라면(위의 행이 달라지는 경계)
					if (j != col - 1) {
						if (maze[i - 1][j].num != maze[i - 1][j + 1].num)
							//하나도 안뚫린 건지 검사
							checking_line(i, j, maze);
					}
					else//마지막 열이라면 그냥 즉시 검사(경계가 자기자신)
					{
						checking_line(i, j, maze);
					}
				}
				if (j != 0)//만약 위의 행 두칸이 같고 현재행 두칸도 같다면 절대 뚫리면 안됨(생략가능)
				{
					if (maze[i - 1][j].num == maze[i - 1][j - 1].num && maze[i][j].num == maze[i][j - 1].num) {
						maze[i][j].left = BIGCLOSE;
						maze[i][j - 1].right = BIGCLOSE;
					}
				}
				//마지막 열에서는 그 행을 뚫을지 말지 검사
				if (j == col - 1) {
					make_col(i, j, maze);
					//onemore(i, j, maze);
				}
				/*if (j != 0)
				{
					if (maze[i - 1][j].num == maze[i - 1][j - 1].num && maze[i][j].num == maze[i][j - 1].num) {
						maze[i][j].left = BIGCLOSE;
						maze[i][j - 1].right = BIGCLOSE;
					}
				}*/
			}
			//			draw(maze);
			//			printf("\n\n");
					/*	if (i == row - 1 && j == col - 1) {
							draw(maze);
							printf("\n\n");
							chek_num(maze);
						}*/
						//			chek_num(maze);
						//			printf("\nIN%d %d\n\n", i, j);

		}

	}
	//모든 배열 만들고, 마지막 행 뚫기 작업
	final_make(maze);
	draw(maze);
	printf("\n\n");
	chek_num(maze);
	printf("\nend\n\n");
	make_file(maze);
	for (i = 0; i < row; i++)
		free(maze[i]);
	free(maze);
	return;

}
void make_file(mm** maze) {
	int i, j;
	FILE* fp = fopen("20171300.maz", "w");
	if (!fp)
	{
		printf("error");
		exit(-1);
	}
	//그냥 경우에 따라 열렸을때, 닫혔을때 그림. draw와 유사
	for (i = 0; i < row; i++) {
		if (i == 0) {
			fprintf(fp, "+");
			for (j = 0; j < col; j++) {
				fprintf(fp, "-+");
			}
			fprintf(fp, "\n");
		}
		fprintf(fp, "|");
		for (j = 0; j < col; j++) {
			if (maze[i][j].right == OPEN)
				fprintf(fp, "  ");
			/*			else if (ma[i][j].right == BIGCLOSE)
						{
							printf("  ;");
						}*/
			else
			{
				fprintf(fp, " |");
			}
		}
		fprintf(fp, "\n+");
		for (j = 0; j < col; j++) {
			if (maze[i][j].down == OPEN) {
				fprintf(fp, " +");
			}
			else
			{
				fprintf(fp, "-+");
			}
		}
		fprintf(fp, "\n");

	}
	fclose(fp);
}
/*
void onemore(int i, int j, mm** ma) { //윗줄과 아랫줄 동기화
	int t, nu, k, u = i;
	for (t = 0; t < col - 1; t++) {
		while (ma[u][t].up == OPEN) {
			nu = min(ma[u][t].num, ma[u - 1][t].num);
			ma[u][t].num = nu;
			ma[u - 1][t].num = nu;
			k = t;
			while (ma[u - 1][k].right == OPEN) {
				nu = min(ma[u - 1][k].num, ma[u - 1][k + 1].num);
				ma[u - 1][k].num = nu;
				ma[u - 1][k + 1].num = nu;
				k++;
			}
			k = t;
			while (ma[u][k].right == OPEN) {
				nu = min(ma[u][k].num, ma[u][k + 1].num);
				ma[u][k].num = nu;
				ma[u][k + 1].num = nu;
				k++;
			}
			u--;

		}
	}
	if (ma[i][t].up == OPEN) {//마지막 col도 동기화
		nu = min(ma[i][t].num, ma[i - 1][t].num);
		ma[i][t].num = nu;
		ma[i - 1][t].num = nu;
	}
	return;
}
*/
void chek_num(mm** ma) {
	int i, j;
	//숫자검사(동기화 잘되었는지)
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++)
			printf("%3d ", ma[i][j].num);
		printf("\n");
	}

}
void final_make(mm** ma) {
	int j, i = row - 1;
	int tem, t, nu;
	for (j = 0; j < col - 1; j++) {
		//이미 뚫려있다면 패스
		if (ma[i][j].right) {
			continue;
		}
		//안뚫려있고, 숫자도 다르다면 
		if (ma[i][j].num != ma[i][j + 1].num) {
			//목표 수 찾고
			tem = ma[i][j + 1].num;
			nu = min(ma[i][j].num, ma[i][j + 1].num);
			//목표수를 가진 모든 것들 동기화
			for (t = j + 1; t < col; t++) {
				if (ma[i][t].num == tem) {
					ma[i][t].num = nu;
				}
				if (t == j + 1) {
					ma[i][t].left = OPEN;
					ma[i][j].right = OPEN;
				}
			}
		}
	}
}
void draw(mm** ma) {
	int i, j;
	for (i = 0; i < row; i++) {
		if (i == 0) {
			printf("+");
			for (j = 0; j < col; j++) {
				printf("ㅡ+");
			}
			printf("\n");
		}
		printf("l");
		for (j = 0; j < col; j++) {
			if (ma[i][j].right == OPEN)
				printf("   ");
			/*			else if (ma[i][j].right == BIGCLOSE)
						{
							printf("  ;");
						}*/
			else
			{
				printf("  l");
			}
		}
		printf("\n+");
		for (j = 0; j < col; j++) {
			if (ma[i][j].down == OPEN) {
				printf("  +");
			}
			else
			{
				printf("ㅡ+");
			}
		}
		printf("\n");

	}
}
void make_col(int i, int j, mm** m) {
	int same_end, same_start, left_onemore = 0;
	int random, nu, t, q;
	int back, front;
	for (t = 0; t < col - 1; t++) {
		//이미 같은 수거나, 닫기로 결심할시 pass. 이 외엔 open해야 하므로 준비시작
		if (m[i][t].num == m[i][t + 1].num)
			continue;
		random = rand() % 2;
		if (random == CLOSE)
			continue;
		same_start = t + 1;
		same_end = t + 1;
		//앞의 목표수
		front = m[i][same_start].num;
		if (t != 0) {
			//뒤의 목표수
			if (m[i][t].num > m[i][t + 1].num)
				left_onemore = 1;
			back = m[i][t].num;
		}
		/*
		for (same_end; same_end<col && m[i][same_start].num == m[i][same_end].num; same_end++); //어디까지 같은것이었는지.
		same_end--;*/
		//동기화할 수는 최솟값
		nu = min(m[i][t + 1].num, m[i][t].num);
		m[i][t].num = nu;
		for (q = same_start; q < col; q++) { //앞으로 같은얘들 동기ㅗ하
			if (m[i][q].num == front)
				m[i][q].num = nu;
			if (q == same_start) {
				m[i][q].left = OPEN;
				m[i][t].right = OPEN;
			}
		}
		if (left_onemore) {//뒤로도 같은부분 동기화
			q = t - 1;
			while (q >= 0) {
				if (m[i][q].num == back) {
					m[i][q].num = nu;
				}
				q--;
			}
			left_onemore = 0;
		}
		//t = same_end;
	}

}
void checking_line(int i, int j, mm** m) {
	int target = m[i - 1][j].num;
	int start, rands = 0;
	for (start = 0; start < col; start++) {
		if (m[i - 1][start].num == target) //시작위치찾기
			break;
	}
	int t, diff = 0, random, nu;
	for (t = start; t <= j; t++) {//시작부터 경계까지 다 다른지 검사(모두 닫혀있는지)
		if (m[i - 1][t].num != m[i][t].num)
			diff = 1;
		else
		{
			diff = 0;
			break;
		}
	}
	if (diff == 1) {//다 닫혀있다면, 다시 random으로 뚫을지 말지 결정
		for (t = start; t <= j; t++) {
			random = rand() % 2;
			m[i][t].up = random;
			m[i - 1][t].down = random;
			if (random == 1) {
				nu = min(m[i][t].num, m[i - 1][t].num);
				m[i][t].num = nu;
				m[i - 1][t].num = nu;
				rands = 1;  //위아래 뚫기 성공
				//printf("\n%d %d %d\n", nu, m[i][t].num, m[i - 1][t].num);
			}
		}
		//하나도 안뚫렸다면 다시 검사실행
		if (rands == 0)
			checking_line(i, j, m);
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

}
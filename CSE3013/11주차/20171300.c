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
void checking_line(int, int, mm**); //������ ���Ʒ� �ձ�
void make_col(int, int, mm**); //�ึ�� �ձ� �۾�
void draw(mm**); //�׳� �׸���
void final_make(mm**); //���� �� �ձ�
void chek_num(mm**); //����üũ �Լ�
//void onemore(int, int, mm**); ����ģ �Լ�
void make_file(mm**); //���Ϸ� �����
void main() {
	srand((unsigned int)time(NULL));
	mm** maze;
	scanf("%d %d", &row, &col);
	int i, j, count = 1, random;
	int nu;
	maze = (mm**)malloc(sizeof(mm*) * row);
	//�迭 �ʱ�ȭ
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
			//1�࿡�� ������ ������ �˻� �ʿ����. ���� �������� ������ ������ ����
			if (i == 0) {
				if (j == col - 1)//�������� ���ʿ�
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
				//���Ʒ� ������ ���� �����ϰ�
				random = rand() % 2;
				maze[i][j].up = random;
				maze[i - 1][j].down = random;
				if (random == 1) { //�ո��� ���� ����ȭ
					nu = min(maze[i - 1][j].num, maze[i][j].num);
					maze[i - 1][j].num = nu;
					maze[i][j].num = nu;
				}
				else {
					//�ȶշ�����, ���� ��輱�̶��(���� ���� �޶����� ���)
					if (j != col - 1) {
						if (maze[i - 1][j].num != maze[i - 1][j + 1].num)
							//�ϳ��� �ȶո� ���� �˻�
							checking_line(i, j, maze);
					}
					else//������ ���̶�� �׳� ��� �˻�(��谡 �ڱ��ڽ�)
					{
						checking_line(i, j, maze);
					}
				}
				if (j != 0)//���� ���� �� ��ĭ�� ���� ������ ��ĭ�� ���ٸ� ���� �ո��� �ȵ�(��������)
				{
					if (maze[i - 1][j].num == maze[i - 1][j - 1].num && maze[i][j].num == maze[i][j - 1].num) {
						maze[i][j].left = BIGCLOSE;
						maze[i][j - 1].right = BIGCLOSE;
					}
				}
				//������ �������� �� ���� ������ ���� �˻�
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
	//��� �迭 �����, ������ �� �ձ� �۾�
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
	//�׳� ��쿡 ���� ��������, �������� �׸�. draw�� ����
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
void onemore(int i, int j, mm** ma) { //���ٰ� �Ʒ��� ����ȭ
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
	if (ma[i][t].up == OPEN) {//������ col�� ����ȭ
		nu = min(ma[i][t].num, ma[i - 1][t].num);
		ma[i][t].num = nu;
		ma[i - 1][t].num = nu;
	}
	return;
}
*/
void chek_num(mm** ma) {
	int i, j;
	//���ڰ˻�(����ȭ �ߵǾ�����)
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
		//�̹� �շ��ִٸ� �н�
		if (ma[i][j].right) {
			continue;
		}
		//�ȶշ��ְ�, ���ڵ� �ٸ��ٸ� 
		if (ma[i][j].num != ma[i][j + 1].num) {
			//��ǥ �� ã��
			tem = ma[i][j + 1].num;
			nu = min(ma[i][j].num, ma[i][j + 1].num);
			//��ǥ���� ���� ��� �͵� ����ȭ
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
				printf("��+");
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
				printf("��+");
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
		//�̹� ���� ���ų�, �ݱ�� ����ҽ� pass. �� �ܿ� open�ؾ� �ϹǷ� �غ����
		if (m[i][t].num == m[i][t + 1].num)
			continue;
		random = rand() % 2;
		if (random == CLOSE)
			continue;
		same_start = t + 1;
		same_end = t + 1;
		//���� ��ǥ��
		front = m[i][same_start].num;
		if (t != 0) {
			//���� ��ǥ��
			if (m[i][t].num > m[i][t + 1].num)
				left_onemore = 1;
			back = m[i][t].num;
		}
		/*
		for (same_end; same_end<col && m[i][same_start].num == m[i][same_end].num; same_end++); //������ �������̾�����.
		same_end--;*/
		//����ȭ�� ���� �ּڰ�
		nu = min(m[i][t + 1].num, m[i][t].num);
		m[i][t].num = nu;
		for (q = same_start; q < col; q++) { //������ ������� �������
			if (m[i][q].num == front)
				m[i][q].num = nu;
			if (q == same_start) {
				m[i][q].left = OPEN;
				m[i][t].right = OPEN;
			}
		}
		if (left_onemore) {//�ڷε� �����κ� ����ȭ
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
		if (m[i - 1][start].num == target) //������ġã��
			break;
	}
	int t, diff = 0, random, nu;
	for (t = start; t <= j; t++) {//���ۺ��� ������ �� �ٸ��� �˻�(��� �����ִ���)
		if (m[i - 1][t].num != m[i][t].num)
			diff = 1;
		else
		{
			diff = 0;
			break;
		}
	}
	if (diff == 1) {//�� �����ִٸ�, �ٽ� random���� ������ ���� ����
		for (t = start; t <= j; t++) {
			random = rand() % 2;
			m[i][t].up = random;
			m[i - 1][t].down = random;
			if (random == 1) {
				nu = min(m[i][t].num, m[i - 1][t].num);
				m[i][t].num = nu;
				m[i - 1][t].num = nu;
				rands = 1;  //���Ʒ� �ձ� ����
				//printf("\n%d %d %d\n", nu, m[i][t].num, m[i - 1][t].num);
			}
		}
		//�ϳ��� �ȶշȴٸ� �ٽ� �˻����
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
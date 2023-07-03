#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#define INF 999999999
typedef struct a {
	int weight;
	int next_node;
	bool complete;
}node;
node s_e[100][100];
int distances[100][100];
int number_of_vertex;

char messages[101][2000];

FILE* result;

void make_shortestpath() {
	int i, j, k;
	int min,min_index;
	int checking_arr[107];
	for (i = 0; i < number_of_vertex; i++) {
		checking_arr[0] = i;

		for (j = 0; j < number_of_vertex-1; j++) {
			int jump_base = checking_arr[j];
			min = INF;
			min_index = i;
			for (k = 0; k < number_of_vertex; k++) {
				if (s_e[i][k].complete == true) {
					continue;
				}
				if (s_e[i][k].weight > s_e[i][jump_base].weight + distances[jump_base][k]) {
					s_e[i][k].weight = s_e[i][jump_base].weight + distances[jump_base][k];
					if (i == jump_base) {
						s_e[i][k].next_node = k;
					}
					else {
						s_e[i][k].next_node = s_e[i][jump_base].next_node;
					}
				}
				if (min > s_e[i][k].weight) {
					min = s_e[i][k].weight;
					min_index = k;
				}
			}
			checking_arr[j + 1] = min_index;
			s_e[i][min_index].complete = true;
		}
	}
//	for (int u = 0; u < number_of_vertex; u++) {
//		for (int v = 0; v < number_of_vertex; v++) {
//			printf("%3d,%-10d ", s_e[u][v].next_node, s_e[u][v].weight);
//		}
//		printf("\n");
	//}
	//printf("\n\n\n");
	for (int u = 0; u < number_of_vertex; u++) {
		for (int v = 0; v < number_of_vertex; v++) {
			if (s_e[u][v].weight >= INF) {
				continue;
			}
			fprintf(result, "%d %d %d\n", v, s_e[u][v].next_node, s_e[u][v].weight);
		}
		fprintf(result, "\n");
	}
}
void find_hop(int line) {

	int start, end;
	char temp[100];
	int go;
	int leng;
	int count = 0;
	for (int i = 0; i < line; i++) {
		char real_message[1000];
		char output_message[2000];
		int spaceck = 0;
		count = 0;
		sscanf(messages[i], "%d %d", &start, &end);
		leng = strlen(messages[i]);
		for (int q = 0; q < leng; q++) {
			if (messages[i][q] == ' ')
				spaceck++;
			if (spaceck >= 2) {
				real_message[count++] = messages[i][q];
			}
		}
		real_message[count] = '\0';
		//printf("%s", real_message);
		sprintf(output_message, "from %d to %d cost ", start, end);
		go = start;
		if (s_e[start][end].weight == INF) {
			strcat(output_message, "infinite hops unreachable message");
		}
		else {
			sprintf(temp, "%d hops ", s_e[start][end].weight);
			strcat(output_message, temp);
			while (go != end) {
				sprintf(temp, "%d ", go);
				strcat(output_message, temp);
				go = s_e[start][end].next_node;
				start = go;
			}
			strcat(output_message, "message");

		}
		strcat(output_message, real_message);
		//printf("%s\n\n", output_message);
		fprintf(result, "%s", output_message);

	}
	fprintf(result, "\n");

}
int main(int argc, char* argv[]) {

	if (argc != 4) {
		printf("usage: ./linkstate topologyfile messagesfile changesfile\n");
		return -1;
	}

	FILE* fp1 = fopen(argv[1], "r");
	FILE* changer = fopen(argv[3], "r");

	int ms_line = 0;
	FILE* message = fopen(argv[2], "r");
	result = fopen("output_ls.txt", "w");

	if ((!fp1) || (!changer) || (!message)) {
		printf("Error: open input file.\n");
		return -1;
	}

	while (!feof(message))
	{
		fgets(messages[ms_line++], 2000, message);
	}
	ms_line--;

	fscanf(fp1, "%d", &number_of_vertex);
	int start, end, weight;
	//initialize
	for (int i = 0; i < number_of_vertex; i++) {
		for (int j = 0; j < number_of_vertex; j++) {
			if (i == j) {
				distances[i][j] = 0;
				s_e[i][j].complete = true;
				s_e[i][j].next_node = i;
				s_e[i][j].weight = 0;
			}
			else {
				distances[i][j] = INF;
				s_e[i][j].complete = false;
				s_e[i][j].weight = INF;
				s_e[i][j].next_node = -1;



			}
		}
	}
	while (fscanf(fp1, "%d %d %d", &start, &end, &weight) != EOF) {
		distances[end][start] = weight;
		distances[start][end] = weight;
	}
	make_shortestpath();
	find_hop(ms_line);
	fclose(fp1);
	while (fscanf(changer, "%d %d %d", &start, &end, &weight) != EOF) {
		//this mean this connected
		if (weight == -999) {
			distances[start][end] = INF;
			distances[end][start] = INF;

		}
		else {
			distances[start][end] = weight;
			distances[end][start] = weight;
		}
		//initialize
		for (int i = 0; i < number_of_vertex; i++) {
			for (int j = 0; j < number_of_vertex; j++) {
				if (i == j) {
					s_e[i][j].complete = true;
					s_e[i][j].next_node = i;
					s_e[i][j].weight = 0;
				}
				else {
					s_e[i][j].complete = false;
					s_e[i][j].weight = INF;
					s_e[i][j].next_node = -1;



				}
			}
		}
		make_shortestpath();
		find_hop(ms_line);

	}
	fclose(changer);
	fclose(message);
	fclose(result);

    
	printf("Complete. Output file written to output_ls.txt.\n");
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#define INF 999999999
typedef struct a {
	int weight;
	int next_vertex;
}ace;
using namespace std;
int number_of_vertex;
ace s_e[100][100];
int distances[100][100];
char messages[101][2000];

FILE* result;

void make_shortestpath() {
	int check = 1;
	int i, k;
	int neighbor;
	//check=0 means no changing.
	while (check >= 1)
	{
		check = 0;
		//base vertext i
		for (i = 0; i < number_of_vertex; i++) {
			//with neighbor of i..
			for (int j = 0; j < number_of_vertex;j++) {
				if (distances[i][j] == INF)
					continue;
				neighbor = j;
				//neighbor is i'neighbor
				for (k = 0; k < number_of_vertex; k++) {
					//if neighber's next_vertex is me,, then pass(split horizon)
					if (s_e[neighbor][k].next_vertex == i) {
						continue;
					}
					int compare_weight = s_e[i][neighbor].weight + s_e[neighbor][k].weight;
					if (s_e[i][k].weight > compare_weight) {
						s_e[i][k].next_vertex = s_e[i][neighbor].next_vertex;
						s_e[i][k].weight = compare_weight;
						check++;
					}
				}
			}

		}
	}
//	for (int u = 0; u < number_of_vertex; u++) {
//		for (int v = 0; v < number_of_vertex; v++) {
//			printf("%3d,%-10d ", s_e[u][v].next_vertex, s_e[u][v].weight);
//		}
//		printf("\n");
	//}
	//printf("\n\n\n");
	for (int u = 0; u < number_of_vertex; u++) {
		for (int v = 0; v < number_of_vertex; v++) {
			if (s_e[u][v].weight >= INF) {
				continue;
			}
			fprintf(result,"%d %d %d\n",v, s_e[u][v].next_vertex, s_e[u][v].weight);
		}
		fprintf(result,"\n");
	}
}
void initialize_s_e() {
	for (int i = 0; i < number_of_vertex; i++) {
		for (int j = 0; j < number_of_vertex; j++) {
			if (i == j) {
				s_e[i][j].next_vertex = i;
				s_e[i][j].weight = 0;
			}
			else if (distances[i][j] != INF) {
				s_e[i][j].weight = distances[i][j];
				s_e[i][j].next_vertex = j;
				s_e[j][i].weight = distances[j][i];
				s_e[j][i].next_vertex = i;
			}
			else {
				s_e[i][j].next_vertex = -1;
				s_e[i][j].weight = INF;
			}

		}
	}
}
/*void make_shortestpath2() {
	int check = 1;
	int i, j, k;
	int num_of_neighbor;
	int neighbor;
	set<int>::iterator itr;
	int min;
	//check=0 means no changing.
	while (check >= 1)
	{
		check = 0;
		//base vertext i
		for (i = 0; i < number_of_vertex; i++) {

			//목표: s_e[i][j]의 최단경로
			for (j = 0; j < number_of_vertex; j++) {
				min = INF;
				if (i == j) {
					continue;
				}
				//이웃들에게서.. 정보 get
				//i=2 j=3
				for (k = 0; k < number_of_vertex; k++) {
					//i의 이웃은 distance[i][k]가 inf가 아니다.
					if (distances[i][k] != INF) {
						if (s_e[i][k].next_vertex == j) {
							continue;
						}
						if (min > s_e[i][k].weight + distances[k][j]) {
							
							min = s_e[i][k].weight+ distances[k][j];
							if (min == s_e[i][j].weight) {
								check--;
							}
							s_e[i][j].weight = min;
							if(i==k)
								s_e[i][j].next_vertex = j;
							else
								s_e[i][j].next_vertex = k;

							check++;
						}
					}
				}
			}
		}
		for (int u = 0; u < number_of_vertex; u++) {
			for (int v = 0; v < number_of_vertex; v++) {
				printf("%3d,%-10d ", s_e[u][v].next_vertex, s_e[u][v].weight);
			}
			printf("\n");
		}
		printf("\n%d\n\n",check);
	}


}*/

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
				go = s_e[start][end].next_vertex;
				start = go;
			}
			strcat(output_message, "message");

		}
		strcat(output_message, real_message);
	//	printf("%s\n\n", output_message);
		fprintf(result,"%s", output_message);

	}
	fprintf(result, "\n");

}

int main(int argc,char *argv[]) {

	if(argc!=4){
		printf("usage: ./distvec topologyfile messagesfile changesfile\n");
		return -1;
	}

	FILE* fp1 = fopen(argv[1], "r");
	FILE* changer = fopen(argv[3], "r");
	
	int ms_line = 0;
	FILE* message = fopen(argv[2], "r");

    if((!fp1)||(!changer)||(!message)){
		printf("Error: open input file.\n");
		return -1;
	}



	result = fopen("output_dv.txt", "w");

	while (!feof(message))
	{
		fgets(messages[ms_line++], 2000, message);
	}
	ms_line--;
	fscanf(fp1,"%d", &number_of_vertex);
	int start, end, weight;
	//initialize
	for (int i = 0; i < number_of_vertex; i++) {
		for (int j = 0; j < number_of_vertex; j++) {
			if (i == j) {
				distances[i][j] = 0;
				s_e[i][j].next_vertex = i;
				s_e[i][j].weight = 0;
			}
			else {
				distances[i][j] = INF;
				s_e[i][j].next_vertex = -1;
				s_e[i][j].weight = INF;
			}
		}
	}
	while (fscanf(fp1, "%d %d %d", &start, &end, &weight) != EOF) {
		distances[end][start] = weight;
		distances[start][end] = weight;
	}
	initialize_s_e();
	/*for (int i = 0; i < number_of_vertex; i++) {
		for (int j = 0; j < number_of_vertex; j++) {
			printf("%10d,%3d ", s_e[i][j].weight, s_e[i][j].next_vertex);
		}
		printf("\n");
	}*/
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
		initialize_s_e();
		make_shortestpath();
		find_hop(ms_line);
	}
	fclose(changer);
	fclose(message);
	fclose(result);


    printf("Complete. Output file written to output_dv.txt.\n");
    return 0;
}
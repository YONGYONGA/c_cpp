#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
char decoding[1000] = { '0' };
char gen[300];
int data_length;
int gen_size;
void make_binary(unsigned char a);
int de_calcu(int, int, int);
unsigned char make_decade(int a, int b);
unsigned char a_make_decade(int a, int b);
int cal_cut = 0;
int now = 0;
char WORD[8];
int W = 0;
int du=0;
char for_copy[500];
char for_calcu[500];
int dummy;
int ttemp;
int main(int argc,char *argv[]) {
	if(argc!=6){
		cout<<"usage: ./crc_decoder input_file output_file result_file generator dataword_size"<<endl;
		return -1;
	}
	if(*(argv[5])!='4' && *(argv[5])!='8'){
		cout<<"dataword size must be 4 or 8"<<endl;
		return -1;
	}
	data_length=*(argv[5])-'0';

	int code_word_length;
	strcpy(gen, argv[4]);
	gen_size = strlen(gen);
	int first = 1;

	unsigned char temp;
	code_word_length = data_length + gen_size - 1;
	FILE* testing = fopen(argv[1], "rb");
	if(!testing){
		cout<<"input file open error."<<endl;
		return -1;
	}
	dummy = fgetc(testing) - '0';
	cal_cut = -dummy;
	int count = 0;
	int calculation;
	FILE* fp2 = fopen(argv[2], "wb");
	if(!fp2){
		cout<<"output file open error";
		return -1;
	}
	int i;
	int error=0;
	unsigned long long code_num = 0;
	while (1) {
		count = getc(testing);
		if (first == 1) {
			ttemp = 0;
		}
		if (count == -1) {
			break;
		}
		temp = count;
		make_binary(temp);
		now++;
		if (data_length == 4) {
			if (cal_cut >= 4 + gen_size - 1) {
				first = 0;
				ttemp = dummy;
				now = 0;
				calculation = cal_cut / (4 + gen_size - 1);
				/*or (int k = 0; k <cal_cut; k++) {
					printf("%c", decoding[dummy+k]);
				}
				for (int k = 0; k < cal_cut; k++) {
					printf("%d at %c\n",dummy+k, decoding[dummy + k]);
				}*/
				for (i=0; i < calculation; i++) {
					code_num++;
					//error+=de_calcu(4, dummy * (i + 1), (dummy + 4 + gen_size - 1) * (i + 1));
					error += de_calcu(4, dummy + (i)*(4 + gen_size - 1), dummy + (i+1) * (4 + gen_size - 1));

					W++;
					if (W == 2) {
						fprintf(fp2, "%c", a_make_decade(0,8));
						W = 0;
					}
				}
				du = cal_cut % (4 + gen_size - 1);
				//printf("�̶� ���� %dbit�� ������\n\n�׷��ٺ���", du);
				for (int j = 0; j < du; j++) {
					//printf("%d:%c go to %d\n", dummy + (4+ gen_size - 1) * i + j, decoding[dummy + (4 + gen_size - 1) * i + j], dummy + j);
					decoding[dummy + j] = decoding[dummy + (4 + gen_size - 1) * i + j];
				}
				cal_cut = du;

			}
		}
		else if (data_length == 8) {
			if (cal_cut >= 8 + gen_size - 1) {
				first = 0;
				ttemp = dummy;
				now = 0;
				calculation = cal_cut / (8 + gen_size - 1);
				/*for (int k = 0; k <cal_cut; k++) {
					//printf("%c", decoding[dummy+k]);
				}
				for (int k = 0; k < cal_cut; k++) {
					printf("%d at %c\n",dummy+k, decoding[dummy + k]);
				}*/
				///printf("\n");
				for (i = 0; i < calculation; i++) {
					code_num++;
					//error += de_calcu(8, dummy * (i + 1), (dummy + 8 + gen_size - 1) * (i + 1));
					error += de_calcu(8, dummy + (i) * (8 + gen_size - 1), dummy + (i + 1) * (8 + gen_size - 1));

					fprintf(fp2, "%c", a_make_decade(0, 8));

				}
				du = cal_cut % (8 + gen_size - 1);
				///printf("�̶� ���� %dbit�� ������\n\n�׷��ٺ���", du);
				for (int j = 0; j < du; j++) {
					///printf("%d:%c go to %d\n", dummy+(8 + gen_size - 1) * i + j, decoding[dummy + (8 + gen_size - 1) * i + j], dummy + j);
					decoding[dummy + j] = decoding[dummy+(8 + gen_size - 1) * i + j];
				}
				cal_cut = du;

			}
		}
	}
	fclose(testing);
	fclose(fp2);
	FILE *result=fopen(argv[3],"w");
	if(!result){
		cout<<"result file open error."<<endl;
		return -1;
	}
	fprintf(result,"%lld %d\n",code_num,error);
	fclose(result);
	return 0;
	//encoding.erase(0, du);
	/*cout << endl << "dummy: " << du << endl << endl << encoding << endl << endl;
	cout << "�ڵο��� ����:" << code_word_length << "  �׸��� encoding����:" << encoding.length();
	FILE* output;
	output = fopen("output.txt", "wb");

	int numof_code = encoding.length() / code_word_length;
	int error = 0;
	for (int i = 0; i < numof_code; i++) {
		error += de_calcu(data_length, i * code_word_length, i * (code_word_length + 1));
	}
	int real_num = 0;
	if (data_length == 4) {
		real_num = numof_code / 2;
	}
	else {
		real_num = numof_code;
	}
	//cout << "\n\n\n\n\noriginal data: " << translate<<endl<<"length: "<<translate.length()<<endl;
	unsigned char ffinal;
	for (int i = 0; i < real_num; i++) {
		ffinal = a_make_decade(i * 8, (i + 1) * (8), translate);
		fprintf(output, "%c", ffinal);
		//cout << ffinal << endl;

	}
	fclose(output);
	cout << "�ڵ���� ������: " << numof_code << "\n������ ������: " << error << endl;
	//FILE* fp2;
	//fp2 = fopen("yy.txt", "w");
	//for*/
}
//������ codeword length���� ����ȴ�. ��, codeword�� ���۰� ���� a,b��. �̶� size�� data������ 4,8
int de_calcu(int size, int a, int b) {
	int codew_size = size + gen_size - 1;
	//string save = data;
	/*for (int j = 0; j < gen_size - 1; j++) {
		data.append("0");
	}*/
	//data.append(gen_size - 1, '0');
	//generator�ͺ�Ʈ�� ���� �����,
	//��
	//���� input�� ������ 4�� 8 �״�� ����
	//printf("������ %d ~ %d", a, b);
	for (int q = 0; q < gen_size; q++) {
		//+a?
		for_calcu[q] = decoding[a+q];
	}
	//���� ������ word����(���� 4��Ʈ Ȥ�� 8��Ʈ)
	if (size == 4) {
		for (int q = 0; q < 4; q++) {
			//WORD[4 * W + q] = decoding[dummy +a+ q];
			WORD[4 * W + q] = decoding[a + q];
		}
	}
	else if (size == 8) {
		for (int q = 0; q < 8; q++) {
			WORD[q] = decoding[dummy + q];
		}
	}

	for (int i = 0; i < size; i++) {
		if (for_calcu[0] == '0') {
			for (int j = 0; j < gen_size - 1; j++) {
				for_calcu[j] = for_calcu[j + 1];
			}

		}
		else {
			for (int j = 0; j < gen_size - 1; j++) {
				if (for_calcu[j + 1] == gen[j + 1]) {
					for_calcu[j] = '0';
				}
				else
					for_calcu[j] = '1';
			}

		}
		//�ڸ��� ������ ����
		/*if (i + gen_size == 11) {
			cout << "hi";
		}*/
		if (i + gen_size < codew_size) {
			for_calcu[gen_size - 1] = decoding[(i + a) + gen_size];
		}
		else {
			for_calcu[gen_size - 1] = '0';
		}
	}

	for (int q = 0; q < gen_size; q++) {
		if (for_calcu[q] == '1') {
			return 1;
		}
	}
	return 0;
}
//translate char to bit
void make_binary(unsigned char a) {
	int temp = a;
	int i;

	//0 1 2 3 
	// 7 6 5 4   swap.
	for (i = 0; i < 8; i++) {
		if (temp == 0) {
			decoding[ttemp +du+now*8+7 - i] = '0';

		}
		else if (temp == 1) {
			decoding[ttemp +du+now*8 + 7 - i] = '1';
			temp = 0;

		}
		else if (temp % 2 == 0 && temp != 1) {
			decoding[ttemp +du+now*8 + 7 - i] = '0';
		}
		else if (temp % 2 == 1 && temp != 1) {
			decoding[ttemp +du+now*8 + 7 - i] = '1';

		}
		temp /= 2;
	}
	//cout << "���� ���� bit��: " << text<<endl;
	cal_cut += 8;
}
//translate bit to char.
unsigned char a_make_decade(int a, int b) {
	unsigned char decade = 0;
	int i;
	//int leng = strlen(go);

	for (i = a; i < b; i++) {
		if (WORD[i] == '0') {
			decade <<= 1;
		}
		else {
			decade <<= 1;
			decade |= 1;
		}
	}
	return decade;
}
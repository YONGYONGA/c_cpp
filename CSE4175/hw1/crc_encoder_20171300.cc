#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include<fstream>
using namespace std;
void calcu(char *, int);
int de_calcu(int, int, int);
using namespace std;
void make_binary(unsigned char a);
int data_length ;
unsigned char make_decade(int a, int b);
char gen[300];
int gen_size;
unsigned long long en_leng;
long byte_size;
int num_c = 0;

int dummy_length;
char encoding[1000];
char encode_word[8];
int first_time = 0;
char for_calcu[500];
char for_copy[500];

int main(int argc,char *argv[]) {
	if(argc!=5){
		cout<<"usage: ./crc_encoder input_file output_file generator dataword_size"<<endl;
		return -1;
	}
	if(*(argv[4])!='4' && *(argv[4])!='8'){
		cout<<"dataword size must be 4 or 8"<<endl;
		return -1;
	}
	data_length=*(argv[4])-'0';
	FILE* fp = fopen(argv[1], "rb");
	if(!fp){
		cout<<"input file open error."<<endl;
		return -1;
	}
	strcpy(gen,argv[3]);
	//gen = "11011";
	//strcpy(gen, "11111111111111111111010111000");
	gen_size = strlen(gen);
	int temp;
	long filesize_s;
	long filesize_e;
	fseek(fp, 0, SEEK_SET);
	filesize_s = ftell(fp);
	fseek(fp, 0, SEEK_END);
	filesize_e = ftell(fp);
	//get byte
	byte_size = filesize_e - filesize_s;
	fseek(fp, 0, SEEK_SET);


	//calculate padding bit.
	unsigned long long word_num;
	if (data_length == 4) {
		word_num = (4 + gen_size - 1) * 2 * byte_size;
	}
	else {
		word_num = (8 + gen_size - 1) * byte_size;
	}
	dummy_length = (8 - (word_num % 8)) % 8;
	
	//make padding first;
	for (int i = 0; i < dummy_length; i++) {
		encoding[i] = '0';
	}
	FILE* fp2 = fopen(argv[2],"wb");
	if(!fp2){
		cout<<"output file open error"<<endl;
		return -1;
	}
	fprintf(fp2, "%d", dummy_length);
	int calculation;
	int i;
	while (1) {
		temp = getc(fp);
		if (temp == EOF) {
			//cout << "end of file" << endl;
			break;
		}
		make_binary(temp);
		if (data_length == 4) {
			calculation = (dummy_length + (4 + gen_size - 1) * 2) / 8;
			dummy_length = (dummy_length + (4 + gen_size - 1) * 2) % 8;
			for (i = 0; i < calculation; i++) {
				fprintf(fp2, "%c", make_decade(i * 8, (i + 1) * 8));
			}
			for (int j = 0; j < dummy_length; j++) {
				encoding[j] = encoding[(i) * 8 + j];
			}
		}
		else if (data_length == 8) {
			calculation = (dummy_length + 8 + gen_size - 1) / 8;
			dummy_length = (dummy_length + 8 + gen_size - 1) % 8;
			for (i = 0; i < calculation; i++) {
				fprintf(fp2, "%c", make_decade(i * 8, (i + 1) * 8));
			}
			for (int j = 0; j < dummy_length; j++) {
				encoding[j] = encoding[(i) * 8 + j];
			}
		}

	}
	fclose(fp);
	fclose(fp2);
}

void make_binary(unsigned char a) {
	int temp = a;
	int i;
	for (i = 0; i < 8; i++) {
		if (temp == 0) {
			encode_word[7 - i] = '0';
			//printf("%d", 0);
		}
		else if (temp == 1) {
			encode_word[7 - i] = '1';
			//printf("%d", 1);
			temp = 0;
		}
		else if (temp % 2 == 0 && temp != 1) {
			encode_word[7 - i] = '0';
			//printf("%d", 0);
		}
		else if (temp % 2 == 1 && temp != 1) {
			encode_word[7 - i] = '1';
			//printf("%d", 1);
		}
		temp /= 2;
	}
	if (data_length == 8) {
		first_time = 0;
		calcu(encode_word, 8);
	}
	else {
		first_time = 0;
		calcu(encode_word, 4);
		calcu(encode_word+4, 4);
	}
}

void calcu(char* data, int size) {
	int codew_size = size + gen_size - 1;

	//���� input�� ������ 4�� 8 �״�� ����
	for (int q = 0; q < size; q++) {
		for_copy[q] = data[q];
	}
	/*for (int j = 0; j < gen_size - 1; j++) {
		data.append("0");
	}*/
	//�ڿ� 0
	for (int q = 0; q < gen_size - 1; q++) {
		for_copy[size + q] = '0';
	}
	//���� for_calcu����, �� data��Ʈ �ڿ� 0�ִ°��� generatorũ�⸸ŭ ����. �װɷ� generator�� ����ϴ°���
	for (int q = 0; q < gen_size; q++) {
		for_calcu[q] = for_copy[q];
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
			for_calcu[gen_size - 1] = for_copy[i + gen_size];
		}
		else {
			for_calcu[gen_size - 1] = '0';
		}
		//cout << k<<endl;
		/*for (int u = 0; u < gen_size; u++) {
			cout << k[u];
		}
		cout << endl;*/
	}
	///for (int qq = 0; qq < gen_size; qq++) {
	///	cout << for_calcu[qq];
	///}
	///cout << endl;
	if (first_time == 0) {
		for (int t = 0; t < size; t++) {
			encoding[dummy_length+t] = data[t];
			///printf("save %c at %d\n", data[t], dummy_length + t);

		}
		for (int t = 0; t < gen_size - 1; t++) {
			encoding[dummy_length+size + t] = for_calcu[t];
			///printf("save %c at %d\n", for_calcu[t], dummy_length + size + t);

		}
		first_time = 1;
	}

	else if (first_time == 1) {
		for (int t = 0; t < size; t++) {
			encoding[dummy_length + size + gen_size - 1 + t] = data[t];
			///printf("save %c at %d\n", data[t], dummy_length + size + gen_size - 1 + t);
		}
		for (int t = 0; t < gen_size - 1; t++) {
			encoding[dummy_length + size+size + gen_size - 1 + t] = for_calcu[t];
			///printf("save %c at %d\n", for_calcu[t], dummy_length + size + size + gen_size - 1 + t);
		}
	}
	//encoding.append(save);
	//�̰� 3�̸� �ȵǳ� ����!
	//encoding.append(k.substr(0, gen_size - 1));
	//cout << "generator: "<<gen<<"\n ��ȯ�� �����ʹ�:"<< encoding << endl;


}
unsigned char make_decade(int a, int b) {
	unsigned char decade = 0;
	int i;
	//int leng = strlen(go);
	/*if (encoding.substr(a, 8) == "00011010") {
		cout << endl;
	}*/
	for (i = a; i < b; i++) {
		if (encoding[i] == '0') {
			decade <<= 1;
		}
		else {
			decade <<= 1;
			decade |= 1;
		}
	}
	///cout << "\n\nthis is : \n" << (int)decade << endl;
	return decade;
}
//������
/*
1. �е���Ʈ�� �׳� �ѹ��� �ϸ� �ɱ�??
2. �������(encode)������, ���� pdf�� ������� ����ó�� ��� ��Ʈ
�� �� ǥ�����־�� �Ѵ�? 3p
3. �Ӹ��ƴ϶� 8��Ʈ���� ���⵵ ���־�� �ϳ�?

�� abc�� ����������, (a=00000000 , b=00000001 �̶� ����)
8��Ʈ���� codeword��������ϰ�, generoator�� 4�ڸ� ������,
�� codeword�� ���� 11bit���״�, �׳� codeword�� 2��Ʈ �����
���� 00000010/00~~~~~~/~~~~~~~~ �̶� ����� ��������,
��������� ���� ��Ʈ�� ���ڿ��� �ٲ��ְ� �����ϸ� �Ǵ°�����.
(�翬�� /�� �����ϱ���. �� ���⵵ ����), �ƴϸ� 3p�� �ִ� ��ó��
00000010 00~~~~~~ ~~~~~~~~ ��װ� �״�� char�� ������ �Ǵ��� �ñ�
*/




/*int main(void) {
	int generator[4] = { 1,1,0,1 };
	int* data, data_code[4] = {0,1,0,0};
	data = new int[4+3];
	for (int i = 0; i < 4; i++) {
		data[i] = data_code[i];
	}
	for (int i = 0; i < 3; i++) {
		data[4 + i] = 0;
	}
	int gen_size = sizeof(generator) / sizeof(int);
	//cout << gen_size;
	calcu(data, generator,gen_size);
	delete[] data;

	return 0;
}*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����

#define max  100000000
unsigned char* temp_result;
int now;
int end;
FILE* one;
FILE* two;
typedef struct tree {
    char c;
    long long count;
    char encode[300];
    int lenght;
    // int left; //�ƽ�Ű�ڵ带 �̿��� ����
    // int right;
    // struct tree* link;
}T;
typedef struct element {
    struct element* right;
    struct element* left;
    struct element* link;
    long long cc;
    char data;
}E;
T arr[300] = { '\0' };  //������ ����ִ� �迭
T sort[300];
int data_count = 0;   //�� ������ ����.
int c_c = 0; //���๮�ڰ���
E* linklist = NULL;
int transfer = 0;
unsigned char int_to_c(int);
unsigned char make_decade(int a, int b, char* go);
unsigned char int_to_c(int ii) {
    int temp = ii;
    char dump[9];
    dump[8] = '\0';
    for (int i = 0; i < 8; i++) {
        if (temp % 2 == 0 || temp == 0) {
            dump[7 - i] = '0';
        }
        else {
            dump[7 - i] = '1';
        }
        temp /= 2;
    }
    return make_decade(0, 8, dump);
}
int c_to_int(unsigned char);
int c_to_int(unsigned char cc) {
    int temp = cc;
    int dump[8];

    for (int tt = 0; tt < 8; tt++) {						//a�� �� 8�� 2�� �����鼭 10������ 2������ ��ȯ�ϴ� ������ ��ģ��.
        if (temp == 0) {
            dump[7 - tt] = 0;
        }
        else if (temp == 1) {
            dump[7 - tt] = 1;
            temp = 0;
        }
        else if (temp % 2 == 0 && temp != 1) {
            dump[7 - tt] = 0;
        }
        else if (temp % 2 == 1 && temp != 1) {
            dump[7 - tt] = 1;
        }
        temp /= 2;
    }
    int data = 0;
    int j = 1;
    for (int i = 0; i < 8; i++) {

        data += dump[7 - i] * j;
        j *= 2;
    }
    return data;
}
void insertionsort(T* arr, int start, int end);
void make_encode(E* ll, char* str, int len);
void delete_tree(E*);
E* min_and_out();
void chekkk(E* ll, int len);
void make_decode(E* ll, int, int);

char make_binary(unsigned char a);
char* en_de;
int dummy_num = 0;
int        lenggg;
int main(int argc, char *argv[])
{
    if(argc!=3){
        printf("error! ./executefile type file\n");
        return -1;
    }
    if(strcmp(argv[1],"-d")!=0 &&strcmp(argv[1],"-c")!=0){
        printf("error! type must be -c(to compress), or -d (to decompress)'\n");
        return -1;
    }
    char type=(argv[1])[1];
    if (type == 'c') {
        for (int i = 0; i < 300; i++) {
            arr[i].c = '\0';
            arr[i].count = 0;
        }
        FILE* fp = fopen(argv[2], "r");
        if (!fp) {
            printf("error! no file\n");
            return -1;
        }
        char temp;
        while (temp = fgetc(fp)) {
            if (temp == EOF)
                break;
            arr[(int)temp].c = temp;
            arr[(int)temp].count++;
            //�񱳿� �迭�� �ϳ� �����.
            sort[(int)temp] = arr[(int)temp];
        }

        int i = 0;

        insertionsort(sort, 0, 299);
        linklist = (E*)malloc(sizeof(E));
        linklist = NULL;
        //��ũ����Ʈ�� ���ʷ� �־��ش�.
        for (i = 299; i >= 0; i--) {
            if (sort[i].count != 0) {
                data_count++;
                E* temp = (E*)malloc(sizeof(E));
                temp->data = sort[i].c;
                temp->cc = sort[i].count;
                temp->right = NULL; temp->left = NULL;
                temp->link = NULL;
                if (!linklist) {
                    linklist = temp;
                }
                else {
                    temp->link = linklist;
                    linklist = temp;
                }

            }
            else {
                break;
            }
        }
        if (data_count == 0) {
            printf("no data");
            return -1;
        }
        E* tem = linklist;
        E* tempp = linklist;
        E* prev = tempp;
        E* one, * two;
        while (1) {
            one = min_and_out();
            if (!linklist) {
                linklist = one;
                break;
            }
            two = min_and_out(linklist);
            E* new = (E*)malloc(sizeof(E));
            new->cc = one->cc + two->cc;
            new->data = '\0';
            new->left = one;
            new->right = two;
            new->link = NULL;
            tempp = linklist;
            prev = tempp;
            while (tempp != NULL) {
                //  printf(" %d\n", tempp->cc);//���� ����.
                if (tempp->cc >= new->cc) {
                    if (prev == linklist) {
                        new->link = linklist;
                        linklist = new;
                    }
                    else {
                        new->link = tempp;
                        prev->link = new;
                    }

                    break;
                }
                prev = tempp;
                tempp = tempp->link;
                if (tempp == NULL) {
                    prev->link = new;
                }
            }
            if (!linklist) {
                linklist = new;
                break;
            }
        }
        char str[300];
        make_encode(linklist, str, 0);
        char filename[100];
        strcpy(filename,argv[2]);
        strcat(filename,".zz");
        FILE* result = fopen(filename, "wb");
        if (!result) {
            printf("error!");
            return -1;
        }

        //2���� ������ ���ο� tree(�ƽ�Ű�ڵ尪�� right,left�� �ֱ�)�� �����, �װ� ��ũ����Ʈ�� ���ʷ� �ڸ��� ã�� �ִ´�.
        //���� linklist�� ������ tree�ϳ��� ������ ����. link�� NULL.
        fseek(fp, 0, SEEK_SET);
        en_de = NULL;
        while (temp = fgetc(fp)) {
            if (temp == EOF)
                break;
            if (en_de == NULL) {

                //en_de = (char*)malloc(sizeof(char)*(strlen(arr[(int)temp].encode) + 1));
                en_de = (char*)malloc(sizeof(char) * (strlen(arr[(int)temp].encode) + 1));
                strncpy(en_de, arr[(int)temp].encode, strlen(arr[(int)temp].encode) + 1);
                lenggg = strlen(en_de);
            }
            else {
                //printf("%s & now, leg is %d\n", en_de,lenggg);
                int lt = arr[(int)temp].lenght;
                lenggg += (lt + 1);
                en_de=(char*)realloc(en_de,sizeof(char)*lenggg);
                // en_de = (char*)realloc(en_de,sizeof(char)*lenggg);
                 //strcat(en_de, arr[(int)temp].encode);
                 //printf("  %d  %d  ::", lenggg, lt);
                for (int tt = 0; tt <= lt; tt++) {
                    //printf("%c & ", arr[(int)temp].encode[lt - tt]);
                    en_de[lenggg - tt - 1] = arr[(int)temp].encode[lt - tt];
                    //printf("%c  \n", en_de[lenggg - tt-1]);
                }
                lenggg--;
                // printf("%s", en_de);
            }
        }
        //"abcdefg"printf("%s encoding..s. total leg is%d \n", en_de, lenggg);
        fprintf(result, "%d ", lenggg);
        if ((lenggg) % 8 != 0) {
            dummy_num = 8 - ((lenggg) % 8);
            lenggg = (strlen(en_de) + dummy_num + 1);
            en_de = (char*)realloc(en_de, sizeof(char) * lenggg);
            for (int p = 0; p < dummy_num; p++) {
                strcat(en_de, "0");
            }
            lenggg--;
        }

        if ((lenggg) % 8 != 0) {
            printf("lenggg : %d dummy : %d", lenggg, dummy_num);
            printf("error");
            return 1;
        }
        else {
            c_c = ((lenggg - 1) / 8) + 1;
        }
        // printf("\n\n%d : %s\n", lenggg, en_de);
        unsigned char power;
        power = int_to_c(data_count);
        fprintf(result, "%c", power);
        //printf("%d", (int)power);
        for (i = 0; i < 300; i++) {
            if (arr[i].count != 0) {
                // arr[i].lenght = strlen(arr[i].encode);
                
                power = int_to_c(arr[i].lenght);
                fprintf(result, "%c%c", arr[i].c, power);
                if (arr[i].lenght % 8 != 0) {

                    //printf("%d  ", arr[i].lenght);
                    if (arr[i].lenght % 8 != 0) {
                        int we = 8 - arr[i].lenght % 8;
                        int ri;
                        for (ri = 0; ri < we; ri++) {
                            arr[i].encode[ri + arr[i].lenght] = '1';
                        }
                        arr[i].encode[ri + arr[i].lenght] = '\0';
                    }
                }
                //1126 1�ڸ����� 8�ڸ����� �ѹ�. 
                for (int k = 0; k < ((arr[i].lenght - 1) / (8)) + 1; k++) {
                    unsigned char ohoh = make_decade(k * 8, (k + 1) * 8, arr[i].encode);
                    //1123 ������� �ϴ� ����. �ڿ� ���ڸ��� 1�־ �ݵ����. 
                    fprintf(result, "%c", ohoh);
                }
                //fprintf(result, "%c",'=');
                //printf("and indeed this is :");
                //make_binary(ohoh,arr[i].lenght);
            }
        }
          //here some error
        //fprintf(result, "%c", '=');
        for (i = 0; i < (lenggg / 8); i++) { 				//��� result�� 8���� unsigned char�� �����Ͽ� ������Ͽ� ����Ѵ�.
            unsigned char store = make_decade(i * 8, (i + 1) * 8, en_de);
            //printf("\nindeed, this is : ");
            //make_binary(store,8);
            fprintf(result, "%c", store);
        }
        // printf("\n%s",en_de);
        //fprintf(result, "%d", dummy_num);
        if (linklist)
            delete_tree(linklist);
        fclose(fp);
        fclose(result);
        if (en_de)
            free(en_de);
        //printf("\n%d  encode complete", data_count);
        /*FILE* fr = fopen("aa.txt", "w");
        for (i = 0; i < 300; i++) {

            if (arr[i].count != 0) {

                fprintf(fr, "%c : %s(lenght is %d )\n", arr[i].c, arr[i].encode, arr[i].lenght);
            }
        }
        fclose(fr);*/
    }
    else if (type == 'd') {
        T* fo_decode;
        FILE* start = fopen(argv[2], "rb");
        if (!start) {
            printf("error! no file!\n");
            return -1;
        }
        char filename[100];
        strcpy(filename,argv[2]);
        strcat(filename,".yy");
        fscanf(start, "%d", &lenggg);
        fgetc(start);
        unsigned char power;
        fscanf(start, "%c", &power);
        data_count = (int)power;
        if (data_count == 0) {
            printf("no data!");
            return -1;
        }
        //printf("%d %d", lenggg, data_count);
            char hey;
        unsigned heyy;
        unsigned char amho;
        fo_decode = (T*)malloc(sizeof(T) * data_count);
        int i;
        for (i = 0; i < data_count; i++) {
            hey = fgetc(start);
            heyy = fgetc(start); //��ĭ ���� ĳġ

            fo_decode[i].c = hey;
            fo_decode[i].lenght = (int)heyy;
            //printf("%d", fo_decode[i].lenght);

            for (int o = 0; o < ((heyy - 1) / 8) + 1; o++) {
                amho = (unsigned char)fgetc(start);
                int temp = amho;
                for (int tt = 0; tt < 8; tt++) {						//a�� �� 8�� 2�� �����鼭 10������ 2������ ��ȯ�ϴ� ������ ��ģ��.
                    if (temp == 0) {
                        fo_decode[i].encode[8 * (o + 1) - tt-1] = '0';
                    }
                    else if (temp == 1) {
                        fo_decode[i].encode[8 * (o + 1) - tt-1] = '1';
                        temp = 0;
                    }
                    else if (temp % 2 == 0 && temp != 1) {
                        fo_decode[i].encode[8 * (o + 1) - tt-1] = '0';
                    }
                    else if (temp % 2 == 1 && temp != 1) {
                        fo_decode[i].encode[8 * (o + 1) - tt-1] = '1';
                    }
                    temp /= 2;
                }
            }
            fo_decode[i].encode[heyy] = '\0';
            //printf("%c : %s   (lenght is %d)\n", fo_decode[i].c, fo_decode[i].encode,fo_decode[i].lenght);
  
            //fgetc(start);
            //printf("%c %c", fo_decode[i].c, hey);
            //printf(fo_decode[i].c == hey);
            //1124 ���ں����Ϸ�.
        }
        linklist = (E*)malloc(sizeof(E));
        linklist->right = NULL;
        linklist->left = NULL;
        for (i = 0; i < data_count; i++) {
            //printf("%c : %s\n", fo_decode[i].c, fo_decode[i].encode);
            char uu = '2';
            int j = 0;

            E* go = linklist;
            //�����Ͱ����� Ʈ������.
            while ((uu = fo_decode[i].encode[j]) != '\0') {
                //printf("%c", uu);
                if (uu == '0') {
                    if (!(go->left)) {
                        E* tt = (E*)malloc(sizeof(E));
                        tt->left = NULL;
                        tt->right = NULL;
                        go->left = tt;
                    }
                    go = go->left;
                }
                else if (uu == '1') {
                    if (!(go->right)) {
                        E* tt = (E*)malloc(sizeof(E));
                        tt->left = NULL;
                        tt->right = NULL;
                        go->right = tt;
                    }
                    go = go->right;
                }
                j++;
            }
            if (uu == '\0') {
                go->data = fo_decode[i].c;
            }
        }
        //chekkk(linklist, 0);
        unsigned char temp;
        temp_result = (unsigned char*)malloc(sizeof(unsigned char) * lenggg);
        en_de = (char*)malloc(sizeof(char) * (lenggg+22));
        if (lenggg % 8 == 0) {				//��ü �����ڵ��� ���̰� 8�� ������ temp_result_size/8��ŭ �о�´�.
            for (i = 0; i < (lenggg / 8); i++)
                fscanf(start, "%c", &temp_result[i]);
        }
        else {							//��ü �����ڵ��� ���̰� 8�� ����� �ƴ϶�� 1�� ���ؼ� ������ �κе� �о��ش�.
            for (i = 0; i < (lenggg / 8) + 1; i++)
                fscanf(start, "%c", &temp_result[i]);

        }
        if (lenggg % 8 == 0) {				//��ü �����ڵ��� ���̰� 8�� ������ temp_result_size/8��ŭ �����Ѵ�.
            for (i = 0; i < (lenggg / 8); i++)
                make_binary(temp_result[i]);		//char�� �����ڵ�ιٲ۴�.���� �� ���� result�� �����Ѵ�.
        }
        else {
            for (i = 0; i < (lenggg / 8) + 1; i++)		//+1��ŭ �����ϸ�
                make_binary(temp_result[i]);		//char�� ���� �ڵ�� �ٲ۴�. �� ���� result�� �����Ѵ�.
        }
        en_de[lenggg] = '\0';
        //printf("%s", en_de);
        free(temp_result);
        //1126 �νĸ�ǥ___________________________________________________������ �𸣰�����, ��û���� ������ �ϰ� ���ĺ����ν���.
        one= fopen(filename, "w");
        make_decode(linklist, 2, dummy_num);
        free(fo_decode);
        if (linklist)
            delete_tree(linklist);
        fclose(start);
        fclose(one);
        //printf("\ndecode done");
    }
    

}
void make_decode(E* ll, int encode_leng, int dummy_num) {
    unsigned char temp;
    E* go = ll;
    now = 0;
    char tempp;
    for (;;) {
        if (now == lenggg || en_de[now] == '\0') {
            break;
        }
        tempp = en_de[now];
        if (tempp == '0') {
            go = go->left;
        }
        else if (tempp == '1') {
            go = go->right;
        }
        if (!go) {
            printf("error at, %d", now);
            break;
        }
        if (go->left == NULL && go->right == NULL) {
            fprintf(one, "%c", go->data);
            go = ll;
        }
        now++;
    }
    free(en_de);
}
void chekkk(E* ll, int len) {
    if (ll->left == NULL && ll->right == NULL) {
        printf("len: %d and : %c\n", len, ll->data);
    }
    else {
        if (ll->left != NULL) {
            chekkk(ll->left, len + 1);
        }
        if (ll->right != NULL) {
            chekkk(ll->right, len + 1);
        }
    }
}


void make_encode(E* ll, char* str, int len) {
    if (ll->left == NULL && ll->right == NULL) {
        if (len == 0) {
            strcpy(arr[(int)ll->data].encode, "1");
            arr[(int)ll->data].lenght = 1;
            return;
        }
        str[len] = '\0';
        strcpy(arr[(int)ll->data].encode, str);
        arr[(int)ll->data].lenght = strlen(arr[(int)ll->data].encode);
    }
    else {
        if (ll->left != NULL) {
            str[len] = '0';
            make_encode(ll->left, str, len + 1);
        }
        if (ll->right != NULL) {
            str[len] = '1';
            make_encode(ll->right, str, len + 1);
        }
    }
}
void delete_tree(E* root) {
    // �⺻ ���̽�: �� Ʈ��
    if (root == NULL) {
        return;
    }

    // ���� �� ������ ���� Ʈ���� ���� ����(Postorder)
    delete_tree(root->left);
    delete_tree(root->right);

    // ���� �� ������ ���� Ʈ���� ������ �� ���� ��带 �����մϴ�.
    free(root);

}
E* min_and_out() {
    E* temp = linklist;
    linklist = linklist->link;
    return temp;
}
void insertionsort(T* sort, int start, int end) {
    int i = start + 1, j;
    T key;
    for (; i <= end; i++) {
        key = sort[i];
        j = i - 1;
        while (j >= 0 && sort[j].count > key.count) {
            sort[j + 1] = sort[j];
            j--;
        }
        sort[j + 1] = key;
    }
}
unsigned char make_decade(int a, int b, char* go) {		//result�迭�� a~b������ �������� �ϳ��� unsigned char�� �����ϴ� �����̴�.
    unsigned char decade = 0;
    int i;
    //int leng = strlen(go);

    for (i = a; i < b; i++) {
        if (go[i] == '0') {			//���� �����ϴ� ���� 0�̶�� decade�� 1bit��ŭ ������ shifting�Ѵ�.
            decade <<= 1;
        }
        else {					//���� �����ϴ� ���� 1�̶�� decade�� 1bit��ŭ ������ shifting�ϰ� 1�� �����ش�.
            decade <<= 1;
            decade |= 1;
        }
    }
    return decade;
}
char make_binary(unsigned char a) {						//decompressing���� �о�� char�� �������� �ص��ϴ� �Լ��̴�.
    int temp = a;
    int i;
    //0 1 2 3 
    // 7 6 5 4   swap.
    // �� �ص��ϰ�, len!=0�̸�, ����������(len��+�ִ� 4��) swap. ������ len�� '\0'�Ҵ�.
    //result = (char*)realloc(result, sizeof(char) * (result_size + 16));	//�ص��� �Լ��� result�� �����Ѵ�.
    for (i = 0; i < 8; i++) {						//a�� �� 8�� 2�� �����鼭 10������ 2������ ��ȯ�ϴ� ������ ��ģ��.
        if (temp == 0) {
            en_de[now + 7 - i] = '0';
            //printf("%d", 0);
        }
        else if (temp == 1) {
            en_de[now + 7 - i] = '1';
            // printf("%d", 1);
            temp = 0;
        }
        else if (temp % 2 == 0 && temp != 1) {
            en_de[now + 7 - i] = '0';
            //printf("%d", 0);
        }
        else if (temp % 2 == 1 && temp != 1) {
            en_de[now + 7 - i] = '1';
            //printf("%d", 1);
        }
        temp /= 2;
    }
    now += 8;
}
//https://gist.github.com/fpdjsns/a46dede3cbd4a05599c94fcda562a0e0
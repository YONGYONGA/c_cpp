#include "tetris.h"
#include "redblack.h"

static struct sigaction act, oact;

int main(){
	int exit=0;
	FILE *fp=fopen("rank.txt","r");
	if(fp==NULL)
		fp=fopen("rank.txt","w");
	fclose(fp);
	initscr();
	noecho();
	keypad(stdscr, TRUE);	
	
	srand((unsigned int)time(NULL));
	createRankList();
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank();break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}
	writeRankFile();
	clearlink();
	endwin();
	system("clear");

}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	//2번째 블럭생성
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	//shadow까지 함께그리는 drawblockwithfeatrues
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	//두번째블럭 박스 추가
	DrawBox(9,WIDTH+10,4,8);
	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(17,WIDTH+10);
	printw("SCORE");
	DrawBox(18,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(19,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	//두번째 블록도 그리기
		move(10+i,WIDTH+13);	
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
							
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
			//강제 종료시에도 게임정보 저장
			newRank(score);
			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	clear();
	//게임 오버시에 게임정보 저장
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int he,wi;
	if(blockY==-1 && currentBlock==0 && blockRotate==1)
		return 0;
	//총 4가지 경우에 0반환. 그외엔 1반환	
	for(he=0;he<4;he++){
		for(wi=0;wi<4;wi++){
			if(block[currentBlock][blockRotate][he][wi]==1 && f[blockY+he][blockX+wi]==1)
			{
	
				return 0;
			}
			if(blockY+he>=HEIGHT && block[currentBlock][blockRotate][he][wi]==1)
			{
				return 0;
			}
		
			if(blockX+wi<0 && block[currentBlock][blockRotate][he][wi]==1)
			{
	
				return 0;
			}
			if(blockX+wi>=WIDTH && block[currentBlock][blockRotate][he][wi]==1)
			{
	
				return 0;
			}
		}
	}
	return 1;	
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	int lastrotate=blockRotate,lastX=blockX,lastY=blockY;//이전정보를 얻기위한 현재정보 대입
	int i,j;
	//현재 정보와 comman로 이전정보 추출	
	switch(command){
	case KEY_UP:
		if(blockRotate==0)
			lastrotate=3;
		else
			lastrotate=blockRotate-1;
		break;
	case KEY_RIGHT:
		lastX=blockX-1;
		break;
	case KEY_LEFT:
		lastX=blockX+1;
		break;
	case KEY_DOWN:
		lastY=blockY-1;
		break;
	}
	//이전정보 블록 지우기
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][lastrotate][i][j]==1 && i+lastY>=0){
				move(i+lastY+1,j+lastX+1);
				printw(".");
	//추가적으로 이전에 생성된 shawdow도 지워버린다.
				move(i+shadowcheckpoint+1,j+lastX+1);
				printw(".");
			}
		}
	}
	//현재정보 블록과 shadow그리기.
	DrawBlockWithFeatures(blockY,blockX,currentBlock,blockRotate);
	
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	int chk;
	int i,j;
	//아래로 내려갈수 있으면 내린다.
	if(CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX))
	{
		
		blockY++;
		DrawChange(field,KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX);
	}
	else
	{	
		//그 외에 못내리거나, 그 자리에 있을수가 없다면 게임오버.
		if(blockY==-1 && CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX)==0)
		{
			gameOver=1;
		}
		//현재의 블록을 필드에 추가 및 필드에 꽉찬줄 지우면서 socre를 계산해준다.
		score+=10*AddBlockToField(field,nextBlock[0],blockRotate,blockY,blockX);
		score+=DeleteLine(field);
		//게임정보 초기화
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=nextBlock[2];
		//2번블록 생성
		nextBlock[2]=rand()%7;
		blockRotate=0;blockY=-1;blockX=WIDTH/2-2;
		DrawField();
		PrintScore(score);
		//게임오버 아니면 다음 게임준비
		if(gameOver==0){
			DrawNextBlock(nextBlock);
			DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
		}
	}
	timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	int touched=0;
	int i,j;
	if(gameOver==1)
		return touched;
	//게임오버가 아니라면 필드에 현재 블록을 쌓는다.
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(block[currentBlock][blockRotate][i][j]==1){
				f[blockY+i][blockX+j]+=1;
			//맞닿은 부분이 필드의 가장아래라면 닿는 면적(touched)++
				if(blockY+i==HEIGHT-1)
					touched++;
			}
			
		}
	}
	return touched;	
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i,j,ii,chs,delline=0;
	int t=(blockY>0)?blockY:0;
	if(gameOver==1)
		return 0;
	//게임오버가 아니라면 블록의 현재 위치부터 4줄 꽉찬지 검사
	for(i=t;i<blockY+4;i++){
		if(i>=HEIGHT)
			break;
		chs=1;
		for(j=0;j<WIDTH;j++){
			if(f[i][j]==0)
			{
				chs=0;
				break;
			}
		}
		// 검사 통과시
		if(chs==1)
		{
			delline++;
			//꽉찬줄 윗줄부터 싹다 한칸씩 밑으로. 다만 삭제라인 만큼 상윗줄은 건들지 않아도 된다.
			for(ii=i;ii>=delline;ii--){
				for(j=0;j<WIDTH;j++){
					if(ii-1>=0){
						f[ii][j]=f[ii-1][j];
					}
				}
			}
		}	
	}
	ii=0;
	//삭제할 만큼윗줄부터 모두 -으로 초기화.
	while(ii<delline){
		for(j=0;j<WIDTH;j++)
			f[ii][j]=0;
		ii++;
	}
	//점수 반환
	return delline*delline*100;
}

///////////////////////////////////////////////////////////////////////////
void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	
	int j,chek=2,i=0;
	//shadow가 찍힐수 있는 Y위치를 구하기 위해 shadowcheckpoint를 사용한다. block의 Y좌표부터 한판씩 아래로 내려가며 checktomove를 이용해 내려가는것이 가능한지 구하고,  내려가는것이 불가능한 경우 반복문을 탈출해서 그 위치에 shadow를 그릴 준비를 한다.
	for(shadowcheckpoint=y+1;shadowcheckpoint<HEIGHT;shadowcheckpoint++){
		chek=CheckToMove(field,blockID,blockRotate,shadowcheckpoint,x);
		if(chek==0)
			break;
	}
//shadowcheckpoint는 블럭이 더이상 내려가는 것이 불가능하거나 겹치는 부분이다. 즉 shadow를 그것보다 한칸 위를 그려야 하므로 --시킨후 drawblock에 tile을 /을 준다.
	DrawBlock(--shadowcheckpoint,x,blockID,blockRotate,'/');

}
void DrawBlockWithFeatures(int y,int x, int blockID,int blockRotate){
	//block의 tile은 ''  shadow tile은 /.
	DrawBlock(y,x,blockID,blockRotate,' ');
	DrawShadow(y,x,blockID,blockRotate);

}
void createRankList(){
	FILE *ran=fopen("rank.txt","r");
	int i,childscore;
	howmany=0;
	char childname[NAMELEN];
	//사실 파일이 없는 경우는 main에서 해서 업을수는 없다. 단지 한번 더쓴것임.
	if(!ran){
		FILE *ran=fopen("rank.txt","w");
		if(!ran){
			printw("error");
			exit(-1);
		}
		fprintf(ran,"\n");
	}
	else{//파일 있다면 얼마나 랭킹 저장되었는지 체크
		fscanf(ran,"%d",&howmany);
	}
	//링크리스트 자료구조에 점수 저장
	for(i=0;i<howmany;i++){
		fscanf(ran,"%s %d",childname,&childscore);
		insertlink(childscore,childname);
	}
	fclose(ran);
	
}

void rank(){
	// user codeii
	if(!head)
		return;
	int x,y,rankmenu=-1,i,findnamere=0;
	char name[NAMELEN];
	L*temp=tail;
	clear();
	//메뉴선택
	while(!(rankmenu>='1' &&rankmenu<='3')){
		printw("1. list ranks from X to Y\n");
		printw("2. list ranks by a specific name\n");
		printw("3. delete a specific rank\n");			
		rankmenu=wgetch(stdscr);
	}
	echo();
	if(rankmenu=='1'){
		printw("X: ");
		scanw("%d",&x);
		printw("Y: ");
		scanw("%d",&y);
		printw("       name       |   score   \n");
		printw("-------------------------------\n");
		//입력을 받지 않는 수는 0이 될때도, 32700을 넘을때도 있다.
		//32700이상의 수를 검색하거나(어짜피 int형 오류가 난다.) 음수랭크를 검색하는 경우는 생각하지 않음.
		if(x<1||x>=32700)
			x=1;
		if(y<1||y>howmany)
			y=howmany;
		if(x>y){//잘봇된 입력값
			printw("%d %d\n",x,y);
			printw("search failure: no rank in the list\n");
		}
		else{//링크리스트 이동하며 찾기
			for(i=1;i<=y;i++){
				if(i>=x){
					printw("% -17s| %d\n",temp->name,temp->data);
				}
				temp=temp->before;
			}	
		forfind=1;
		}
	}
	else if(rankmenu=='2'){
		printw("input the name: ");
		scanw("%s",name);
		printw("       name       |   score   \n");
		printw("-------------------------------\n");
		//이름찾으면 변소 증가시키며 출력
		for(;temp;temp=temp->before){//끝까지 가며
			if(!(strcmp(name,temp->name))){//같은 걸 찾으면 출력, 변수 증가
				printw("%-17s| %d\n",temp->name,temp->data);
				findnamere++;
			}
		}
		//하나도 못찾았으면 에러
		if(findnamere==0)
			printw("\nsearch failure: no name in the list\n");
	}
	else if(rankmenu=='3'){
		printw("input the rank:");
		scanw("%d",&x);
		//잘못된 등수. n명이 있다면n개의 등수가 존재한다.
		if(x>howmany || x<1)
			printw("\nsearch failure: the rank note in the list\n");
		else{//그외 삭제와 몇명인지 따지는 변수 --
			removelink(x);//x등 삭제
			printw("\nresult: the rank deleted\n");
			howmany--;
		}
	}
	noecho();
	getch();
}
void writeRankFile(){
	FILE *fp=fopen("rank.txt","w");
	int i;
	L*temp=tail;
	if(fp==NULL)
		exit(-1);
	//등수와 정보를 모두 파일에 출력한다. 이때 아무 정보가 없다면 0을 rank에 저장시킨다.
	fprintf(fp,"%d\n",howmany);
	for(i=0;i<howmany;i++){
		fprintf(fp,"%s %d\n",temp->name,temp->data);
		temp=temp->before;
	}
	fclose(fp);

}
void newRank(int score){
	// user code
	clear();
	int i;
	char name[NAMELEN];
	echo();
	//정보 입력받고
	printw("your name: ");
	scanw("%s",name);
	//그 정보를 링크리스트에 저장
	insertlink(score,name);
	howmany++;
	noecho();
	getch();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
void removelink(int r){
	int i;
	L*temp=tail;
	//사실 이 경우는 성립하지 않는다. (애시당초 정보 없을시 rank함수에도 못들어옴.) 단지 그냥 쓴것이다.
	if(!temp)
		return;
	//저장된 값이 하나뿐이라면. 초기화
	if(head==tail){
		head=NULL;
		tail=NULL;
		free(temp);
		return;
	}
	//그 외 그 등수에 해당하는 곳으로 이동하며 케이스 별로 링크리스트 삭제,연결
	for(i=1;i<=r;i++){
		if(i==r){//만약 1등을 삭제하는 것이라면 (tail의 변경)
			if(temp==tail){
				temp->before->next=NULL;
				tail=temp->before;
				free(temp);
			}//만약 꼴찌를 삭제하는 것이라면(haed의 변경)
			else if(temp==head){
				temp->next->before=NULL;
				head=temp->next;
				free(temp);
			}
			else{ //그외 중간에 있는 값 삭제. 모든 링크를 temp가 없었을때의 경우로 link시키고 temp삭제
				temp->before->next=temp->next;
				temp->next->before=temp->before;
				free(temp);
			}
		}
	}
}
void insertlink(int data, char* name) {
	L* curr = head, * prev = NULL;
	//데이터 저장
	L* temp = (L*)malloc(sizeof(L));
	temp->data = data; strcpy(temp->name, name);
	temp->next = NULL; temp->before = NULL;
	//첫 데이터라면
	if (!curr) {
		head = temp;
		tail = temp;
		return;
	}//그외 이동하면서 끝에 오거나 데이터 큰것을 발견시 탈출
	while (curr) {
		if (!curr) {
			break;
		}
		else if (curr->data > data)
			break;
		prev = curr;
		curr = curr->next;
	}//끝에 도달한것이라면 이는 저장할 데이터가 가장크다는 뜻.
	if (curr == NULL) {
		prev->next = temp;
		temp->before = prev;
		tail = temp;
	}//하나도 이동못하고 탈출한 경우는 현재 데이터가 가장 작다는 뜻
	else if (curr == head) {
		temp->next = curr;
		curr->before = temp;
		head = temp;
	}
	else{//단 한번 디오한 경우에 만약 prev가 큰것이라면 현재 데이터가 가장 작다는 뜻
		if (prev== head) {
			if (prev->data > data) {
				temp->next = prev;
				curr->before = temp;
				head = temp;
			}//아니라면 data는 두번째로 작다는 뜻
			else
			{
				temp->next = curr;
				temp->before = prev;
				prev->next = temp;
				curr->before = temp;
			}
		}// 그외에 중간에 멈췄을 경우
		else
		{
			temp->next = curr;
			temp->before = prev;
			prev->next = temp;
			curr->before = temp;
		}

	}
	head->before = NULL;
	tail->next = NULL;
}
void clearlink(){//모든 링크 초기화
	L*move=head,*temp;
	while(move){
		temp=move;
		move=move->next;
		free(temp);
	}
}
/*  포기한 레드블랙..
node* create_newnode(int data,char *name)
{
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	strcpy(newnode->name, name);
	newnode->data = data;
	newnode->left_c = NULL;
	newnode->right_c = NULL;
	newnode->color = red;
	newnode->parent = NULL;
	return newnode;
}
void insert(int data,char *name)
{

	node* currnode = root, * prevnode=NULL;
	node* newnode = create_newnode(data,name);
	if (currnode==NULL) {
		root = newnode;
		root->color = black;
		return;
	}
	while (currnode) {
		prevnode = currnode;
		if (currnode->data < data)
			currnode = currnode->right_c;
		else
			currnode = currnode->left_c;
	}
	newnode->parent = prevnode;
	if (prevnode->data < data)
		prevnode->right_c = newnode;
	else
	{

		prevnode->left_c = newnode;
	}
}
void fixtree(node* newnode) {
	int uncleblack = 0;
	while (newnode != root&&newnode->parent->color == red &&newnode!=root->right_c &&newnode!=root->left_c){
		uncleblack = 0;
		//삼촌이 부모의 오른쪽에 존재
		if (newnode->parent == newnode->parent->parent->left_c && newnode != root) {
			node* uncle = newnode->parent->parent->right_c;
			if (!uncle || uncle->color == black) {
				uncleblack = 1;
			}
			else if (uncle->color == red) {
				uncleblack = 0;
				uncle->color = black;
				newnode->parent->color = black;
				newnode->parent->parent->color = red;
				newnode = newnode->parent->parent;
			}
			//          할
			//      부모   삼
			//                           의경우
			if(uncleblack==1){
				if (newnode == newnode->parent->right_c) {	
					newnode->color = newnode->parent->parent->color;
					newnode->parent->parent->color = red;
					left_rotate(newnode->parent);
					right_rotate(newnode->parent);
				}
				else
				{
					newnode->parent->color = newnode->parent->parent->color;
					newnode->parent->parent->color = red;
					right_rotate(newnode->parent->parent);
				}
			}
		}
		else {//삼촌은 부모의 왼쪽에 존재
			node* uncle = newnode->parent->parent->left_c;
			printf("%d",newnode->data);
			if (!uncle ||uncle->color==black) {
				uncleblack=1;
			}
			else if (uncle->color == red) {
				uncle->color = black;
				newnode->parent->color = black;
				newnode->parent->parent->color = red;
				newnode = newnode->parent->parent;
			}
			if(uncleblack==1)
			{
				if (newnode == newnode->parent->left_c) {
					newnode->color = newnode->parent->parent->color;
					newnode->parent->parent->color = red;
					right_rotate(newnode->parent);
					left_rotate(newnode->parent);
				}
				else {
					newnode->parent->color = newnode->parent->parent->color;
					newnode->parent->parent->color = red;
					left_rotate(newnode->parent->parent);
				}
			}
		}
	}
	root->color = black;
	//temp = root;
	//print2D(temp);
}
void right_rotate(node* n) {
	node* newparent;
	newparent = n->left_c;
	if (!newparent)
		return;
	if (newparent->right_c == NULL)
		n->left_c = NULL;
	else
		n->left_c = newparent->right_c;
	if (n->left_c != NULL)
	{
		n->left_c->parent = n;
	}
	newparent->parent = n->parent;
	if (n == root)
		root = newparent;
	else
	{ //위치에 맞게 새로운 자식 할당
		if (n == n->parent->right_c) {
			n->parent->right_c = newparent;
			//n->right_c = newparent->left_c;
		}
		else {
			n->parent->left_c = newparent;
			//	n->left_c = newparent->right_c;
		}
	}
	n->parent = newparent;
	newparent->right_c = n;
}
void left_rotate(node* n) {
	node* newparent;
	newparent = n->right_c;
	if (!newparent)
		return;
	if (newparent->left_c == NULL)
		n->right_c = NULL;
	else
		n->right_c = newparent->left_c;
	if (n->right_c != NULL)
	{
		n->right_c->parent = n;
	}
	newparent->parent = n->parent;
	if (n == root)
		root = newparent;
	else
	{ //위치에 맞게 새로운 자식 할당
		if (n == n->parent->right_c) {
			n->parent->right_c = newparent;
			//	n->right_c = newparent->left_c;
		}
		else {
			n->parent->left_c = newparent;
			//	n->left_c = newparent->right_c;
		}
	}
	n->parent = newparent;
	newparent->left_c = n;
}
void clearstart() {
	cleartree(root);
	clearroot();
}
void cleartree(node *temp) {
	if (temp!=NULL) {
		cleartree(temp->left_c);
		cleartree(temp->right_c);
		if(temp!=root)
			free(temp);
	}
}
void clearroot() {
	root = NULL;
}
void find(node* N, int x, int y) {
	if (N != NULL) {
		find(N->left_c,x,y);
		if (x <= forfind && forfind <= y) {
			printw(" %-17s| %s ",N->name,N->data);
		}
		else if (forfind > y)
			return;
		forfind++;
		find(N->right_c,x,y);
	}
}
*/

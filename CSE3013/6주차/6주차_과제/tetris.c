﻿#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

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
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
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

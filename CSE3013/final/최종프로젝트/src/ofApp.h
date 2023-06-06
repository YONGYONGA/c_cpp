/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

	#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon
#include <ctime>
#define OPEN 1
#define CLOSE 0
#define WALL -1
class ofApp : public ofBaseApp {

public:
	int path_w = 22;  //미로 길의 너비. 
	void setup();
	void update();
	void draw();

	void keyPressed(int key); // Traps escape key if exit disabled
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool readFile();
	void freeMemory();   //모든 메모리 해제 및 전역변수 초기화
	bool DFS();   //DFS?
	void dfsdraw();   //스택가지고 경로그리기
	void bfsdraw();   //B_visit가지고 경로 그리기
	int HEIGHT;//미로의 높이
	int WIDTH;//미로의 너비
	char** input;//텍스트 파일의 모든 정보를 담는 이차원 배열이다.
	int** visited;//방문여부를 저장할 포인
	int maze_col;//미로칸의 열의 인덱스를 가리킨다.
	int maze_row;//미로칸의 행의 인덱스를 가리킨다.
	int k;
	int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
	int isdfs;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
	int isbfs;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
	// Menu
	ofxWinMenu* menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function
	typedef struct ma { //미로 배열 원소들
		int num;
		int up, down, left, right;
	}M;
	M** maze; //미로
	typedef struct node { //큐, 스택 노드
		int num;
		struct node* next;
	}N;
	//dfs에선 생성된 스택으로 길추척 가능하지만 bfs는 큐로 길추적이 불가능
	//고로 visit배열을 역추적하기 위해 새로운 B_visit형 정의
	typedef struct ar {   //BFS visit용 구조체
		int num;
		int from; //역추적을 위한 from.
	}ar;
	N* stack;   //스택 헤드용
	N* Q_h, * Q_t;  //큐 헤드, 테일
	int* D_visit;
	ar* B_visit;
	// Used by example app
	ofTrueTypeFont myFont;
	ofImage myImage;
	float windowWidth, windowHeight;
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	// Example menu variables
	bool bShowInfo;
	bool bFullscreen;  //풀스크린용 변수
	bool bTopmost;   //가장앞으로 띄우기용 변수
	//bool isdfs;
	// Example functions
	void doFullScreen(bool bFull); //풀스크린
	void doTopmost(bool bTop); //가장앞으로

	int target_c;    //목적지 target y,x좌표
	int target_r;
	//dfs스택용
	int df(int);  //DFS
	void s_push(int);  //스택 in
	void s_pop();  //스택 out
	bool s_empty();  //스택 비었는지
//bfs큐용
	bool BFS();  //BFS
	int Q_empty();  //큐 빔
	void Q_delete(); //큐 out
	void Q_add(int); //큐 in
//pj. 11주차 것 그대로 이용
	void make_maze();     //미로 만들기
	void final_make(M**);    //미로의 마지막행
	void checking_line(int, int, M**);  //미로의 행마다 뚫을지 말지
	void make_col(int, int, M**);    //미로의 열을 뚫을지 말지

	//게임변수, 함수
	int key_gaming = 0;    //키 게임시 1, 그외 0
	int mouse_gaming = 0;   //마우스 게임시 1, 그외 0
	int now_c;    //키게임용 현재 위치 파악 변수
	int now_r;
	int breaking = 0;   //키게임용 벽 부술려고 할땐 1, 그외 0
	int quitg = 0; //0이면 게임종료, 및 게임안함. 1일때는 게임중
	void end_g();   //키게임용 움직일때마다 target에 도달했는지 파악하는 함수. 끝에 도달하면 succ, quit조정
	int moving = 0;  //얼마나 움직였는지 파악
	int br = 0;   //얼마나 벽을 부쉈는지 파악
	int succ = 0;  //게임 성공시 1, 그외 0
	clock_t start, u;   //start는 계속 바뀌는 시간, u는 초기 start실시간변화 시간 저장(게임 시간 추출용)
	double fi = 0;   //게임시간 출력용 (start-u)로 계속 변화하는 변수
	int inm = 0;   //마우스 게임용 변수. 0이면 게임시작 불가능(사직 위치까지 이동 필수), 1이면 시작장소에 왔기에 시작준비, 2면 실질적 게임시작
	void draw_gamebox();  //마우스게임용, 게임 박스 그리기
	bool touch_wall(int, int);  //마우스 게임용. 커서가 벽에 닿거나 이상한 위치에 있다면 false, 그외 true 
};

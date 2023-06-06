/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = 0;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup); //실선
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS", false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		if (!maze)
			freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			isbfs = 0;
			if (isdfs)
				isdfs = 0;
			else
				isdfs = 1;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			isdfs = 0;
			if (isbfs)
				isbfs = 0;
			else
				isbfs = 1;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	//처음부터 끝까지 4방향을 탐색하며 각각 열려있는것이 아니라면 라인그리기
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].up != OPEN)
				ofDrawLine(j * 22, i * 22, j * 22 + 22, i * 22);
			if (maze[i][j].down != OPEN)
				ofDrawLine(j * 22, i * 22+22, j * 22+22, i * 22+22);
			if (maze[i][j].left != OPEN)
				ofDrawLine(j * 22, i * 22, j * 22, i * 22+22);
			if (maze[i][j].right != OPEN)
				ofDrawLine(j * 22+22, i * 22, j * 22+22, i * 22+22);

		}
	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			if(DFS())
				dfsdraw();
			if (D_visit)
				free(D_visit);
			D_visit = NULL;
			if (stack)
				free(stack);
			stack = NULL;
		}
		else
			cout << "You must open file first" << endl;
	}
	if (isbfs) {
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			if (BFS())
				bfsdraw();
			if (B_visit)
				free(B_visit);
			B_visit = NULL;
			if (Q_h) {
				free(Q_h);
			}
			if (Q_t) {
				free(Q_t);
			}
			Q_h = NULL;
			Q_t = NULL;
		}
		else
			cout << "You must open file first" << endl;
	}
	
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)  //정확히는 모르겠지만 상태창을 가장 앞으로 띄워주는 함수인듯함.(모든 실행중인 프로그램, 심지어 작업표시줄보다도 앞으로)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			if (maze)
				freeMemory();
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		//menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << fileName << endl;
		//printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");//?!
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			if (maze) {
				freeMemory();
			}

			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0; //짝수 홀수 라인 탐색
			int c = 0; //행 탐색, 생성
			int n = 0; //배열의 원소 num
			WIDTH = 0, HEIGHT = 0;
			int i;
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			maze = (M**)malloc(sizeof(M*));
			ofBuffer::Line s = buffer.getLines().begin(), e = buffer.getLines().end();
			for (; s != e; s++) {
				string line = *s;
				if (WIDTH == 0) { //문자열에서 문자크기 /2가 너비이다. (열의 갯수)
					WIDTH = (line.size())/2;
				}

				if (cnt == 0) { //첫행에선 그냥
					cnt++;
					continue;
				}
				else if (cnt%2 == 1) { //홀수번째 라인은 오른쪽, 왼쪽으로 뚫려있는지 확인 및, 높이(총 행의 갯수)를 측정할수 있는 라인이다.
					if(c==0) //첫번째에선 행의 열만 동적할당(이미 이차원 배열 첫행은 동적할등을 했다.)
						maze[c] = (M*)malloc(sizeof(M) * WIDTH);
					else //두개의 행 이상 있다는 뜻이므로, 홀수 라인마다 realoc으로 재할당 및 행의 열 동적할당
					{
						maze = (M**)realloc(maze, sizeof(M*) * (c + 1));
						maze[c] = (M*)malloc(sizeof(M) * WIDTH);

					}
					for (i=0; i < WIDTH; i++) {
						if (line[i*2] == ' ') { //경우에 따라 open, close저장시키기. 당연히 i=0일땐 left만 필요하다. 이때 주요 문자가 저장되는 위치는 i*2번째 인덱스에 있다는 것을 주의.(이부분 이외에는 그냥 경로용 blank임)
							if (i == 0) {
								maze[c][i].left = OPEN;
							}
							else
							{
								maze[c][i].left = OPEN;
								maze[c][i - 1].right = OPEN;
							}
						}
						else
						{
							if (i == 0) {
								maze[c][i].left = WALL;
							}
							else
							{
								maze[c][i].left = CLOSE;
								maze[c][i - 1].right = CLOSE;
							}
						}
					}
					//마지막 열에서는 right만 필요하다.
					maze[c][WIDTH - 1].right = WALL;
				}
				else if (cnt%2 == 0) {//짝수 번째 라인(첫번째 라인 제외)은 위 아래가 뚫려있는지 확인하는 라인. 추가적으로 배열의 num초기화 및 변수조정을 한다.
					if (c == 0) { //행이 하나밖에 없는때라면, 당연히 up부분은 벽
						for (i=0; i < WIDTH; i++) { //주요 문자가 저장되는 위치는 홀수번째 인덱스이다. 이것 이외에는 칸구별을 위한 +임. 
							if (line[i * 2 + 1] == ' ') {
								maze[c][i].up =WALL;
								maze[c][i].down = OPEN;
								maze[c][i].num = n++;
							}
							else
							{
								maze[c][i].up = WALL;
								maze[c][i].down = CLOSE;
								maze[c][i].num = n++;
							}
						}
					}
					else //행이 2개 이상 있다면 각 원소의 up은 윗행 down을 따른다.
					{
						for (i=0; i < WIDTH; i++) {
							if (line[i * 2 + 1] == ' ') {
								maze[c][i].up = maze[c-1][i].down;
								maze[c][i].down = OPEN;
								maze[c][i].num = n++;
							}
							else
							{
								maze[c][i].up = maze[c - 1][i].down;
								maze[c][i].down = CLOSE;
								maze[c][i].num = n++;
							}
						}
					}
					c++; //다음 행 생성 그리고 탐색 준비 변수
				}
				cnt++; //짝수라인인지 홀수라인인지 구분
			}
			HEIGHT = c; //높이동기화
			target_c = HEIGHT - 1;
			target_r = WIDTH - 1;
			for (i=0; i < WIDTH; i++) { //마지막 행에서의 down을 close에서 wall로 바꿔준다.
				maze[HEIGHT - 1][i].down = WALL;
			}
			input_flag = 1;
			isOpen = 1;
			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}

	}
}
bool ofApp::BFS() {
	int v = 0;
	B_visit = (ar*)calloc(HEIGHT * WIDTH, sizeof(ar));
	B_visit[v].num = 1;
	Q_add(v);
	int r1, c1,r,c, u, ck = 0;
	ofSetColor(ofColor::blue);
	while (Q_h) {
		ck = 0;
		//탈출 조건. 가장 최근에 추가된것이 target이면.
		if (Q_t->num == WIDTH * HEIGHT - 1) {
			return true;
			break;
		}
		//탐색 num은 가장 처음에 추가된(가장 나중에 추가된) Q_h의 num
		u = Q_h->num;
		r1 = u % WIDTH;
		c1 = u / WIDTH;
		//bfs는 뚫려있는곳 모든 부분을 탐색하는 알고리즘. 즉 if문만 필요.
		//현재 탐색 num 부분 maze의 윗부분이 열려있고, 방문한적도 없다면 (4개의 if문 모두 동일. BFS이기 때문에 한 while문당 4방향 전체 탐색. 4방향 탐색 모두 실패시 ck=0)
		if (maze[c1][r1].up == OPEN && B_visit[maze[c1 - 1][r1].num].num == 0) {
			//탐색 num 초기화
			u = maze[c1 - 1][r1].num;
			r = u % WIDTH;
			c = u / WIDTH;
			//갔다고 길그리기
			ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
			//같다고 표시 및, 어디서 온건지 표시
			B_visit[u].num = 1;
			B_visit[u].from = Q_h->num;
			//큐 추가
			Q_add(u);
			ck = 1;
			//printf("%d  ", u);
		}
		if (maze[c1][r1].down == OPEN && B_visit[maze[c1 + 1][r1].num].num == 0) {
			u = maze[c1 + 1][r1].num;
			r = u % WIDTH;
			c = u / WIDTH;
			ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
			B_visit[u].num = 1;
			B_visit[u].from = Q_h->num;
			ck = 1;
			Q_add(u);
			//printf("%d  ", u);
		}
		if (maze[c1][r1].right == OPEN && B_visit[maze[c1][r1 + 1].num].num == 0) {
			u = maze[c1][r1 + 1].num;
			r = u % WIDTH;
			c = u / WIDTH;
			ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
			B_visit[u].num = 1;
			B_visit[u].from = Q_h->num;
			ck = 1;
			Q_add(u);
			//printf("%d  ", u);
		}
		if (maze[c1][r1].left == OPEN && B_visit[maze[c1][r1 - 1].num].num == 0) {
			u = maze[c1][r1 - 1].num;
			r = u % WIDTH;
			c = u / WIDTH;
			ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
			B_visit[u].num = 1;
			B_visit[u].from = Q_h->num;
			ck = 1;
			Q_add(u);
			//printf("%d  ", u);
		}
		//4방향 탐색 실패. 즉 움직일수가 없음. 이럴땐 Q에서 삭제.
		if (ck == 0)
		{
			Q_delete();
		}

	}
	//큐가 빌때까지 탐색을 못했으므로 길찾기 실패했다는 뜻.
	return false;
}
void ofApp::bfsdraw() {
	int i, r, c,r1,c1;
	ofSetColor(ofColor::red);
	for (i =  WIDTH* HEIGHT - 1; i != 0;) {
		
		//2차원 원소로 변환
		r = i % WIDTH;
		c = i / WIDTH;
		//이전 방문 num으로 초기화
		i = B_visit[i].from;
		//2차원원소로 변환
		r1 = i % WIDTH;
		c1 = i / WIDTH;
		//선그리기
		ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
	}
}
int ofApp::Q_empty() {
	if (!Q_h)
		return 1;
	return 0;
}
void ofApp::Q_add(int data) {
	N* temp = (N*)malloc(sizeof(N));
	temp->num = data;
	temp->next = NULL;
	//Q추가 방법. 아무것도 없으면 head 부터, 있다면 tail의 next가 새로운 원소가 들어갈 곳
	if (Q_h)
		Q_t->next = temp;
	else
	{
		Q_h = temp;
	}
	Q_t = temp;
}
void ofApp::Q_delete() {
	if (!Q_h)
		return;
	//헤드 이동후 원래 헤드 삭제(큐의 삭제)
	N* temp = Q_h;
	Q_h = Q_h->next;
	free(temp);
}
void ofApp::freeMemory() {
	//미로 free(행)
	for (int i = 0; i < HEIGHT; i++)
		free(maze[i]);
	//미로 전체 free
	free(maze);
	//미로 초기화
	maze = NULL;
	//dfs때 사용하는 visit.free
	if (D_visit)
		free(D_visit);
	//초기화
	D_visit = NULL;
	N* temp;
	//스택초기화
	if (stack) {
		temp = stack;
		stack = stack->next;
		free(temp);
	}
	stack = NULL;
	//DFS와 동일 맥락
	if (B_visit)
		free(B_visit);
	B_visit = NULL;
	if (Q_h) {
		temp = Q_h;
		Q_h = Q_h->next;
		free(Q_h);
	}
	Q_h = NULL;
	Q_t = NULL;
	//사용한 전역변수들 초기화
	isdfs = 0;
	isbfs = 0;
	isOpen = 0;
	HEIGHT = 0;
	WIDTH=0;
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	int ck;
	D_visit = (int*)calloc(HEIGHT * WIDTH, sizeof(int));
	ck=df(0);
	//길찾기 성공
	if (ck == 1) {
		isdfs = 1;
		return TRUE;
	}
	else//실패
	{
		isdfs = -1;
		return false;
	}
}
int ofApp::df(int v) { //실수로 만듬. 그냥 DFS에 넣어도 됨.
	D_visit[v] = 1;
	s_push(v);
	int c, r, u;
	//printf("%d  ", v);
	ofSetColor(ofColor::green);
	while (stack != NULL) {
		//탈출 조건. 스택의 head(가장 최근 추가된 것이)의 num이  target이면 도착했다는 뜻이니 탈출
		if (stack->num == HEIGHT * WIDTH - 1) {
			return 1;
			break;
		}
		//2차원 원소로 변환
		r = stack->num % WIDTH;
		c = stack->num / WIDTH;
		//dfs는 일단 뚫려있는 곳 한방향을 못갈때까지 가며 탐색하는 방식. 즉 if else문 필요
		//최대한 실행을 줄이기 위해 down과 right가 먼저 실행되게(이 방향으로 가면 그나마 높은 가능성으로 목적지 도달). down으로 갈수 있고, 방문한 적이 없는 경우의 if
		if (maze[c][r].down == OPEN && D_visit[maze[c + 1][r].num] == 0) {
			//탐색 num초기화
			u = maze[c + 1][r].num;
			//방문 표시
			D_visit[u] = 1;
			//선그리고 스택에 추가
			ofDrawLine(r * 22 + 11, c * 22 + 11, r * 22 + 11, c * 22 + 33);
			s_push(u);
		}
		//down탐색 불가능 및 right 탐색 가능경우. 모든 경우가 다 같은 방식이다. 
		else if (maze[c][r].right == OPEN && D_visit[maze[c][r + 1].num] == 0) {
			u = maze[c][r + 1].num;
			D_visit[u] = 1;
			ofDrawLine(r * 22 + 11, c * 22 + 11, r * 22 + 33, c * 22 + 11);
			s_push(u);
		}
		else if (maze[c][r].up == OPEN && D_visit[maze[c - 1][r].num] == 0) {
			u = maze[c - 1][r].num;
			D_visit[u] = 1;
			ofDrawLine(r * 22 + 11, c * 22 - 11, r * 22 + 11, c * 22 + 11);
			s_push(u);
		}
		else if (maze[c][r].left == OPEN && D_visit[maze[c][r - 1].num] == 0) {
			u = maze[c][r - 1].num;
			D_visit[u] = 1;
			ofDrawLine(r * 22 + 11, c * 22 + 11, r * 22 - 11, c * 22 + 11);
			s_push(u);
		}
		//전체방향 탐색 실패. 즉 갈길이 없으므로 스택에서 삭제(이전에 탐색했던 것으로 돌아가기위함)
		else
		{
			s_pop();
		}
	}
	return 0;
}
void ofApp::s_push(int data) {
	N* temp= (N*)malloc(sizeof(N));;
	temp->num = data;
	temp->next = NULL;
	//스택 추가 함수. 스택은 단순히 앞에 추가해주면 됨.
	if (s_empty()) {
		stack=temp;
	}
	else
	{
		temp->next = stack;
		stack = temp;
	}
}
void ofApp::s_pop() {
	if (!stack)
		return;
	//스택 삭제. 단순히 앞 삭제
	N* temp = stack;
	stack = stack->next;
	free(temp);
}
bool ofApp::s_empty() { //비었음을 나타내는 함수. 다만 if(stack)으로 더 쉽게 표기 가능하므로 큐의 empty는 만들이 않음
	if (stack == NULL)
		return true;
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	//dfs는 남아있는 스택으로 그대로 경로 추적이 가능하다.(가능한 경로라면 pop이 되지 않기 때문)
	N* temp = stack;
	int c, r,c2,r2;
	ofSetColor(ofColor::red);
	//원소가 2개씩 필요하므로 next가 빌때까지 탐색.
	while (temp->next) {
		//2차원  원소 변환
		r = temp->num % WIDTH;
		c = temp->num / WIDTH;
		r2= temp->next->num % WIDTH;
		c2 = temp->next->num / WIDTH;
		//선그리기
		ofDrawLine(r*22+11, c*22+11, r2*22+11, c2*22+11);
		temp = temp->next;
	}
}


/*
int i, j;
for (i = 0; i < HEIGHT; i++) {
	if (i == 0) {
		printf("+");
		for (j = 0; j < WIDTH; j++) {
			printf("ㅡ+");
		}
		printf("\n");
	}
	printf("l");
	for (j = 0; j < WIDTH; j++) {
		if (maze[i][j].right == OPEN)
			printf("   ");
		/*			else if (ma[i][j].right == BIGCLOSE)
					{
						printf("  ;");
					}
		else
		{
			printf("  l");
		}
	}
	printf("\n+");
	for (j = 0; j < WIDTH; j++) {
		if (maze[i][j].down == OPEN) {
			printf("  +");
		}
		else
		{
			printf("ㅡ+");
		}
	}
	printf("\n");

}
*/
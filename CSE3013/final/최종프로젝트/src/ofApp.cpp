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
#define min(x,y) (x<y)?x:y
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
	//새로운 미로 자동생성
	menu->AddPopupItem(hPopup, "Make maze", false, false);

	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	//false가 없다면 클릭시에 메뉴에 체크표시가 생성된다. 나머지도 동일
	menu->AddPopupItem(hPopup, "Show DFS");
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check
	
	//게임 메뉴 생성. 키게임과 마우스게임
	hPopup = menu->AddPopupMenu(hMenu, "Game");
	menu->AddPopupItem(hPopup, "key_game");
	menu->AddPopupItem(hPopup, "mouse_game");

	//세팅 메뉴 생성. 목표지점이나 길의 너비 조정 가능.
	hPopup = menu->AddPopupMenu(hMenu, "Setting");
	menu->AddPopupItem(hPopup, "change target", false, false);
	menu->AddPopupItem(hPopup, "change width", false, false);

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
		menu->SetPopupItem("key_game", false);
		menu->SetPopupItem("mouse_game", false);
		menu->SetPopupItem("Show BFS", false);
		menu->SetPopupItem("Show DFS", false);
		readFile();
	}
	//SETpopup...("false")는 메뉴창에 체크된 것을 해제하는 명령 코드이다. 실행에 큰 영향을 주는 코드는 아니지만, 미관상 아래 4개중 하나라도 실행중 체크표시가 되어있다면, 그 체크표시를 풀어준다.
	if (title == "Make maze") {
		menu->SetPopupItem("key_game", false);
		menu->SetPopupItem("mouse_game", false);
		menu->SetPopupItem("Show BFS", false);
		menu->SetPopupItem("Show DFS", false);
		make_maze();
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
		if (isOpen)
		{
			quitg = 0;//만약 게임중이면 게임 종료
			//DFS제외 나머지 체크표시 해제.
			menu->SetPopupItem("key_game", false);
			menu->SetPopupItem("mouse_game", false);
			menu->SetPopupItem("Show BFS", false);
			//BFS중이라면 BFS도 종료
			isbfs = 0;
			//dfs중이면 dfs종료, dfs실행중이지 않다면 dfs. 다시말해 클릭으로 DFS를 껏다 켯다 할수 잇게함
			if (isdfs)
				isdfs = 0;
			else
				isdfs = 1;
		}
		else {
			//에러메시지 및, DFS체크되는 것 해제
			cout << "you must open file first" << endl;
			menu->SetPopupItem("Show DFS", false);
		}
		
	}

	if(title == "Show BFS") {
		if (isOpen)
		{
			//DFS와 흐름 완전 동일. 
			quitg = 0;
			menu->SetPopupItem("key_game", false);
			menu->SetPopupItem("mouse_game", false);
			menu->SetPopupItem("Show DFS", false);
			isdfs = 0;
			if (isbfs)
				isbfs = 0;
			else
				isbfs = 1;
		}
		else {
			cout << "you must open file first" << endl;
			menu->SetPopupItem("Show BFS", false);
		}
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	if (title == "key_game") {
		if (isOpen) {
			//나머지 3개 메뉴 체크 되어있다면 체크해제
			menu->SetPopupItem("mouse_game", false);
			menu->SetPopupItem("Show BFS", false);
			menu->SetPopupItem("Show DFS", false);
			printf("\n벽 부수기가 가능한 키보드로 이동하는 길찾기 미로입니다. \nb를 누른후, ,w,s,a,d에 따라, 각각 상하좌우의 벽이 부숴지고 이동가능해 집니다.(가장 바깥족 벽은 부술수 없습니다.)");
			printf("b를 누른이후, 다른 키를 누르거나 부술수 없는 벽방향을 누른다면 벽이 부숴지지 않습니다. 다시 벽을 부수고 싶다면 다시 b를 누른후, wasd를 눌러주세요.");
			printf("\n현재의 높이: %d 너비: %d\n", HEIGHT, WIDTH);
			//유효한 target입력할때까지 계속 입력받음
			do {
				printf("0보다 크고, 높이와 너비보단 작거나 같게 목적지를 입력하세요 (y축,x축) : ");
				scanf("%d %d", &target_r, &target_c);
			} while ((target_r > HEIGHT || target_c > WIDTH) || (target_r <= 0 || target_c <= 0));
			//배열로써의 타겟 조정
			target_c--;
			target_r--;
			//키 게임시작
			key_gaming = 1;
			//시작 위치
			now_r = 0;
			now_c = 0;
			//게임시작
			quitg = 1;
			//움직인횟수, 부순횟수 초기화
			moving = 0;
			br = 0;
			//성공변수 초기화
			succ = 0;
			//시간 변수 초기화
			start = clock();
			u = start;
			fi = 0;
			//DFs,BFS모두 종료
			isdfs = 0; isbfs = 0;
		}
		else {//미로가 열려있지도 않다면 에러메시지 및 체크해제.
			cout << "you must open file first" << endl;
			menu->SetPopupItem("key_game", false);
		}
	}
	if (title == "mouse_game") {
		if (isOpen) {
			menu->SetPopupItem("key_game", false);
			menu->SetPopupItem("Show BFS", false);
			menu->SetPopupItem("Show DFS", false);
			printf("\n마우스로 이동하는 길찾기 미로입니다. 마우스가 벽에 닿으면 게임오버이니 주의해주세요.");
			printf("\n현재의 높이: %d 너비: %d\n", HEIGHT, WIDTH);
			//유효한 타겟, 및 길의 너비 받을때까지 계속 반복
			do {
				printf("0보다 크고, 높이와 너비보단 작거나 같게 목적지를 입력하세요 (y축,x축) : ");
				scanf("%d %d", &target_r, &target_c);
			} while ((target_r > HEIGHT || target_c > WIDTH) || (target_r <= 0 || target_c <= 0));
			do {
				printf("10이상의 길의 폭을 지정해주세요(20이상을 추천드립니다. 폭이 클수록 쉽습니다.): ");
				scanf("%d", &path_w);
			} while (path_w < 10);
			target_c--;
			target_r--;
			//마우스 게임, 게임, 성공, inm 변수들 초기화
			mouse_gaming = 1;
			quitg = 1;
			succ = 0;
			inm = 0;
			//시간 초기화
			start = clock();
			u = start;
			fi = 0;
			//DFs,BFS모두 종료
			isdfs = 0; isbfs = 0;
		}
		else {//미로 열려있지 않을 경우
			cout << "you must open file first" << endl;
			menu->SetPopupItem("mouse_game", false);
		}
	}
	if (title == "change target") {
		if (isOpen) {
			int aa, bb;
			aa = target_c;
			bb = target_r;
			printf("\n현재 타겟: (%d,%d)\n", bb+1, aa+1);
			do {
				printf("변경할 타겟을 입력하세요 (y축,x축): ");
				scanf("%d %d", &target_r, &target_c);
			} while (target_r > HEIGHT || target_c > WIDTH);
			//이전의 정보와 변경된 정보 표기위해 aa,bb변수 설정
			printf("y축: %d -> %d , x축: %d -> %d", bb+1, target_r, aa+1, target_c);
			target_r--;
			target_c--;
		}
		else
			cout << "you must open file first" << endl;
	}
	if (title == "change width") {
		if (isOpen) {
			int aa = path_w;
			printf("\n변경할 길의 너비를 입력하세요: ");
			scanf("%d", &path_w);
			//이전의 정보와 변경된 정보 표기위해 aa변수 설정
			printf("길의 너비: %d -> %d", aa, path_w);
		}
		else
			cout << "you must open file first" << endl;
	}
	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("read readme.txt");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {
	//스페이스용 변수. 본인은 사용안함
	char str[256];
	//시간용 변수
	string times;
	//마우스의 x,y위치 저장용 변수
	int mX, mY;
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	if (quitg == 1) {
		//DFs,BFS종료
		isdfs = 0;
		isbfs = 0;
		//타임 출력용변수 (절대 바뀌지 않는 부분)
		times = "game times: ";
		//키게임 이라면
		if (key_gaming == 1) {
			//시작위치와 끝 위치 점으로 표시
			ofSetColor(127, 23, 31);
			ofDrawCircle(target_c * path_w + path_w / 2, target_r * path_w + path_w / 2, 2);
			ofSetColor(22, 22, 22);
			ofDrawCircle( now_c * path_w + path_w / 2, now_r * path_w + path_w / 2, 2);
			ofSetColor(77, 77, 77);
			//start시간 변경
			start = clock();
			//u는 고정된 시간이니, 변경되는 시간과의 차를 이용해 소요시간 구함
			fi = (start - u) / (double)1000;
			times += to_string(fi);
			//소요시간 윈도우창에 표시
			myFont.drawString(times, ofGetWidth() - 200, ofGetHeight() - 70);
		}
		//마우스 게임 및, 가장 초기 상황이라면(inm=0)
		if (mouse_gaming == 1 && inm==0) {
			//마우스 게임용 미로 그리기
			draw_gamebox();
			ofSetColor(ofColor::red);
			//경고메시지. 마우스를 시작점에 같다 놓으라는 뜻
			myFont.drawString("Put mouse in blue box for start game", ofGetWidth() - 333, ofGetHeight() - 50);
			//마우스 커서가 시작점에 오면
			if (ofGetMouseX() < (path_w / 5 * 4) && ofGetMouseY() < (path_w / 5 * 4) && ofGetMouseX() > 3 && ofGetMouseY() > 3) {
				inm = 1; //게임 시작변수 조정
				return;
			}
			else
			{
				inm = 0; //그외 커서가 올때까지 계속 return
				return;
			}
		}
		if (mouse_gaming == 1 && inm == 1) {//마우스 커서가 시작위치에 들어왔다. 초기 세팅
			start = clock(); //시간 초기화. u는 고정이다.
			u = start;
			ofSetBackgroundAuto(false); //마우스 이동경로를 지우지 않기 위함
			ofSetFrameRate(10000);//마우스 위치 인식을 초당 10000번. 정교한 게임을 원할수록 높이면 된다.
			inm++;//진짜 게임 시작하게 변수 조정
			draw_gamebox(); //초기세팅
			ofSetColor(127, 23, 31);
			ofDrawRectangle((target_c)*path_w+2, (target_r)*path_w+2, path_w-2, path_w-2); //타겟 위치까지 박스로 표시
			return;
		}
		if (mouse_gaming == 1 && inm == 2) {//게임중
			fi= (start - u) / (double)1000;    //시간 출력, 저장용
			ofSetColor(255, 255, 255);    //backgroundauto 가 false이기에, 계속 변하는 시간을 표현하기 위해선, 이전 프레임에서 표현된 시간을 지워주어야 한다. 아니면 겹쳐보임. 즉,먼저 그냥 위에 흰색 상자로 덮어버림
			ofDrawRectangle(ofGetWidth() - 200, ofGetHeight() - 100, ofGetWidth(), ofGetHeight() - 70);

			ofSetColor(77, 77, 77);
			times += to_string(fi);
			myFont.drawString(times, ofGetWidth() - 200, ofGetHeight() - 70); //시간 출력
			start = clock(); //start 변경
			ofSetColor(ofColor::purple);//선은 보라색으로
			mX = ofGetMouseX();  //마우스 위치는 실시간으로 계속 변하므로, 한번 if문 돌때마다 위치를 고정시킨 변수를 생성해, 이를 이용해서 모든 검사실행
			mY = ofGetMouseY();
			ofDrawCircle(mX, mY, 2); //마우스 위치에 원그리기
			if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) { //왼쪽 마우스 클릭시 게임 종료
				quitg = 0;
			}
			if (!touch_wall(mX,mY)) { //마우스 이동가능한지 체크. 벽에 부딛힌 것이라 판단되면 if문으로 들어간다.
				ofSetColor(ofColor::red);//충돌위치 표시는 빨간색
				ofDrawCircle(mX, mY, 4); //충돌위치 표시
				quitg = 0;   //충돌했으니 게임 실패
			}
			if (mX < (target_c+1) * path_w-2 && mY < (target_r+1) * path_w-2 && mX > ((target_c) * path_w)+2 && mY > ((target_r) * path_w)+2) { //목적지에 마우스 커서 도달
				ofSetColor(77,77,77); //성공위치 표시
				ofDrawCircle(mX, mY, 4);  //''
				succ = 1;  //성공
				quitg = 0; //게임종료
			}
			return;
		}
			
	}
	else
	{    //만약 종료된게 키게임이라면
		if (key_gaming == 1) {
			/*
			ofSetColor(100);
			for (i = 0; i < HEIGHT; i++) {
				for (j = 0; j < WIDTH; j++) {
					if (maze[i][j].up != OPEN)
						ofDrawLine(j * path_w, i * path_w, j * path_w + path_w, i * path_w);
					if (maze[i][j].down != OPEN)
						ofDrawLine(j * path_w, i * path_w + path_w, j * path_w + path_w, i * path_w + path_w);
					if (maze[i][j].left != OPEN)
						ofDrawLine(j * path_w, i * path_w, j * path_w, i * path_w + path_w);
					if (maze[i][j].right != OPEN)
						ofDrawLine(j * path_w + path_w, i * path_w, j * path_w + path_w, i * path_w + path_w);

				}
			}
			
			ofSetColor(22, 22, 22);
			ofDrawCircle(now_c * path_w + path_w / 2, now_r * path_w + path_w / 2, 2);
			*/
			//게임 종료시 출력할 문자열. 성공혹은 실패, 움직인 횟수, 부순 횟수, 걸린시간 을 모두 합한다.
			string str1;
			if (succ == 1)
				str1="success!!\n";
			else
				str1="fail!!\n";
			str1 += "moving:";
			str1 += to_string(moving);
			str1 += "\nbreaking: ";
			str1 += to_string(br);
			str1 += "\ntotal time: ";
			str1 += to_string(fi);
			//키게임 끝
			key_gaming = 0;
			//알람창
			ofSystemAlertDialog(str1);
			//게임체크표시 해제
			menu->SetPopupItem("key_game", false);
		}
		
	}
	//끝난게 마우스 게임이라면
	if (mouse_gaming == 1) {
		//게임체크표시 해제
		menu->SetPopupItem("mouse_game", false);
		string str1;
		//키게임과 마찬가지로 알람창. 대신 성공 혹은 실패와 시간만 표시
		if (succ == 1)
			str1 = "success!!\n";
		else
			str1 = "fail!!\n";
		str1 += "total time: ";
		str1 += to_string(fi);
		ofSystemAlertDialog(str1);
		//마우스 게임 끝나면 다시 원래대로 모든것 기본 옵션들 초기화.(프레임 속도, 배경 상시 초기화)
		ofSetBackgroundAuto(true);
		ofSetFrameRate(15);
		ofBackground(255, 255, 255);
		//마우스 게임끝
		mouse_gaming = 0;
	}

	//처음부터 끝까지 4방향을 탐색하며 각각 열려있는것이 아니라면 라인그리기
	ofSetColor(100);
	ofSetLineWidth(5);
	//그냥 모든 인덱스마다 4개의 선 그리기.
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].up != OPEN)
				ofDrawLine(j * path_w, i * path_w, j * path_w + path_w, i * path_w);
			if (maze[i][j].down != OPEN)
				ofDrawLine(j * path_w, i * path_w + path_w, j * path_w + path_w, i * path_w + path_w);
			if (maze[i][j].left != OPEN)
				ofDrawLine(j * path_w, i * path_w, j * path_w, i * path_w + path_w);
			if (maze[i][j].right != OPEN)
				ofDrawLine(j * path_w + path_w, i * path_w, j * path_w + path_w, i * path_w + path_w);

		}
	}

	if (isdfs)//DFS클릭시 1
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		//a만약 파일이 열려있다면..
		if (isOpen) {
			N* temp;
			//DFS로 간 경로 표시
			if(DFS())
				//경로 찾기 성공시 경로표시
				dfsdraw();
			//vist배열과 stack배열 상시 초기화
			if (D_visit)
				free(D_visit);
			D_visit = NULL;
			while(stack) {
				temp = stack;
				stack = stack->next;
				free(temp);
			}
			stack = NULL;
		}
		else
			cout << "You must open file first" << endl;
	}
	if (isbfs) {//BFS클릭시 1
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {//DFS와 완전 동일
			N* temp;
			if (BFS())
				bfsdraw();
			//visit와 큐 초기화
			if (B_visit)
				free(B_visit);
			B_visit = NULL;
			while(Q_h) {
				temp = Q_h;
				Q_h = Q_h->next;
				free(temp);
			}
			Q_h = NULL;
			Q_t = NULL;
		}
		else
			cout << "You must open file first" << endl;
	}
	//스페이스 누를때 뜸
	if(bShowInfo) {
		if (quitg == 0) {
			sprintf(str, " comsil project");
			myFont.drawString(str, 15, ofGetHeight() - 20);
		}

	}

} // end Draw


void ofApp::doFullScreen(bool bFull)//풀스크린 함수임. 사실상 손대지 않았고 프로젝트 내용이아님.
{
	// Enter full screen
	if(bFull) {//그냥 풀스크일때는 위의 메뉴창 없애고 화면꽉채우는 것
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


void ofApp::doTopmost(bool bTop)  //손대지 않음. 정확히는 모르겠지만 상태창을 가장 앞으로 띄워주는 함수인듯함.(모든 실행중인 프로그램, 심지어 작업표시줄보다도 앞으로)
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
	if(key == VK_ESCAPE) { //esc시 탈출. 풀스크린일때는 전체화면 종료
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
		breaking = 0;
		// Update the menu check mark because the item state has been changed here
		//menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') { //풀스크린
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		breaking = 0;
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
	if (key == 'b')  //벽부술준비
		breaking = 1;
	if (key == 'q') { //게임 종료
		quitg = 0;
	}
	if (key == 'w') { //위의 벽 부수기
		if (breaking == 1) {
			if (maze[now_r][now_c].up == CLOSE) {
				maze[now_r][now_c].up = OPEN;
				maze[now_r-1][now_c].down = OPEN;
				br++;
			}
		}
		breaking = 0;
	}
	if (key == 'a') { //왼쪽벽 부수기
		if (breaking == 1) {
			if (maze[now_r][now_c].left == CLOSE) {
				maze[now_r][now_c].left = OPEN;
				maze[now_r][now_c-1].right = OPEN;
				br++;
	
			}
		}
		breaking = 0;
	}
	if (key == 's') {//아래벽 부수기
		if (breaking == 1) {
			if (maze[now_r][now_c].down == CLOSE) {
				maze[now_r][now_c].down = OPEN;
				maze[now_r+1][now_c].up = OPEN;
				br++;
			}
		}
		breaking = 0;
	}
	if (key == 'd') {//오른쪽 벽부수기
		if (breaking == 1) {
			if (maze[now_r][now_c].right == CLOSE) {
				maze[now_r][now_c].right = OPEN;
				maze[now_r][now_c+1].left = OPEN;
				br++;
			}
		}
		breaking = 0;
	}
} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_DOWN) {//아래로 이동
		breaking = 0;
		if (key_gaming == 1) {
			if (maze[now_r][now_c].down == OPEN) {
				now_r++;  //위치조정
				moving++; //움직인 횟수
				end_g(); //게임이 종료되었는지 확인
			}
		}
	}
	if (key == OF_KEY_RIGHT) { //오른쪽 이동. 방법은 동일
		breaking = 0;
		if (key_gaming == 1) {
			if (maze[now_r][now_c].right == OPEN) {
				now_c++;
				moving++;
				end_g();
			}
		}
	}
	if (key == OF_KEY_LEFT) {
		breaking = 0;
		if (key_gaming == 1) {
			if (maze[now_r][now_c].left == OPEN) {
				now_c--;
				moving++;
				end_g();
			}
		}
	}
	if (key == OF_KEY_UP) {
		breaking = 0;
		if (key_gaming == 1) {
			if (maze[now_r][now_c].up == OPEN) {
				now_r--;
				moving++;
				end_g();
			}
		}
	}

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
void ofApp::draw_gamebox() {
	ofSetColor(100);  //마우스게임용 화면그리기. 시작블락을 파란색으로 표시해줌. (미로표시는 당연)
	ofSetLineWidth(5);
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].up != OPEN)
				ofDrawLine(j * path_w, i * path_w, j * path_w + path_w, i * path_w);
			if (maze[i][j].down != OPEN)
				ofDrawLine(j * path_w, i * path_w + path_w, j * path_w + path_w, i * path_w + path_w);
			if (maze[i][j].left != OPEN)
				ofDrawLine(j * path_w, i * path_w, j * path_w, i * path_w + path_w);
			if (maze[i][j].right != OPEN)
				ofDrawLine(j * path_w + path_w, i * path_w, j * path_w + path_w, i * path_w + path_w);
		}
	}
	ofSetColor(ofColor::blue);
	ofDrawRectangle(2,2 ,path_w-2 , path_w-2);
}
void ofApp::end_g() {  //현재 위치가 타겟에 도달했다면
	if (now_c == target_c && now_r == target_r) {
		succ = 1; //게임성공
		quitg = 0; //및 게임종료
	}
}
bool ofApp::touch_wall(int x, int y) {  //마우스 게임용, 벽에 부딪힐때 게임종료하기 위함
	int rx=x, ry=y;
	int numx,numy;
	//약간씩 판단위치 조정. 정확히 선이 아니라 선 중앙 좌표의 +-2정도에만 닿아도 선 중앙에 닿는것으로 판단케함. 이렇게 할시, 실제 게임상의 선에 살짝만 닿아도 탈락가능.
	if (x % path_w >= path_w - 2) {
		rx += (path_w-x % path_w);
	}
	else if (x % path_w <= 2) {
		rx -= (x % path_w);
	}
	if (y % path_w >= path_w - 2) {
		ry += (path_w - y % path_w);
	}
	else if (y % path_w <= 2) {
		ry -= (y % path_w);
	}
	//좌표가 현재 미로 안에 있다면 검사시작
	if (x >= 0 && x <= WIDTH * path_w && y >= 0 && y <= HEIGHT * path_w) {
		if (rx % path_w == 0 || ry % path_w == 0) {
			numx = rx / path_w; //정확히 미로의 어느번째 인덱스의 선과 부딛혔는지 확인하기 위함
			numy = ry / path_w;
			if (rx == 0 || ry == 0) {//가장 윗부분이거나, 가낭 왼쪽부분의 벽
				return false;
			}
			else if (ry % path_w == 0) { //아래 (혹은 위의 벽이)벽이 막혀있음. a를 기준으로 살짝 아래에 있던, 위에있던, 판단위치를 조정했기에 ry는 정확히 a선 중심의 y좌표 이다.
				if (maze[numy - 1][numx].down != OPEN) { //가장 위일때는 위의 if에서 검사했으므로, down만 검사시켜주면 됨
					return false;  //부딛힌것이니 false
				}
			}
			else if (rx % path_w == 0) { //오른쪽(혹은 옆)의 벽이 막혀잇음. 마찬가지로 중심의 x좌표임
				if (maze[numy][numx - 1].right != OPEN) { //가장 왼쪽일때는 검사했으므로 right만 검사
					return false;
				}
			}
			else //벽이 모두 open이고, 끝부분도 아님. ㅇ열린 부분이라는 뜻
			{
				return true;
			}
		}
		else //그냥 길 (각 미로 칸)
		{
			return true;
		}
	}
	else { //미로 밖이면 당연히 실패다
		return false;
	}
	return true; //위에 게임종료 경우는 모두 false리턴 처리를 시켰다.즉, 나머지 경우는 true;
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

		printf("file name is ");
		cout << fileName << endl;
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
			//미로가 이미 존재한다면, 초기화는 필수
			if (maze) {
				freeMemory();
			}
			//그냥 종료시 1. 안쓰긴함
			int input_flag = 0;

			// Idx is a variable for index of array.

			// Read file line by line
			int cnt = 0; //짝수 홀수 라인 탐색
			int c = 0; //행 탐색, 생성
			int n = 0; //배열의 원소 num
			WIDTH = 0, HEIGHT = 0;
			int i;
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			//여기까지 왔으면 미로가 있는 것이니 일단 1행 생성
			maze = (M**)malloc(sizeof(M*));
			//s에 시작,  e에 끝 저장
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
			target_r = HEIGHT - 1;
			target_c = WIDTH - 1;
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
	//visit 생성되며 0으로 초기화시키는 calloc
	B_visit = (ar*)calloc(HEIGHT * WIDTH, sizeof(ar));
	B_visit[v].num = 1;
	Q_add(v);
	int r1, c1,r,c, u, ck = 0;
	ofSetColor(ofColor::blue);
	while (Q_h) {
		ck = 0;
		//탈출 조건. 가장 최근에 추가된것이 target이면.
		if (Q_t->num == target_r * WIDTH + target_c) {
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
			ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
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
			ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
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
			ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
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
			ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
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
	for (i = target_r * WIDTH + target_c; i != 0;) {
		
		//2차원 원소로 변환
		r = i % WIDTH;
		c = i / WIDTH;
		//이전 방문 num으로 초기화
		i = B_visit[i].from;
		//2차원원소로 변환
		r1 = i % WIDTH;
		c1 = i / WIDTH;
		//선그리기
		ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
	}
}
int ofApp::Q_empty() {
	if (!Q_h)//없다면 1반환
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
	//미로가 있다면(HEIGHT가 1이상이면 미로가 존재한다는 뜻임) 행free, 및 전체 free
	for (int i = 0; i < HEIGHT; i++)
		free(maze[i]);
	free(maze);
	maze = NULL;
	//visit free
	if (D_visit)
		free(D_visit);
	D_visit = NULL;
	N* temp;
	//스택 있다면 끝까지 스택이동시키며 free(사실 draw에서 그릴때마다 그린후 free시키기 때문에 이용되지 않음)
	while(stack) {
		temp = stack;
		stack = stack->next;
		free(temp);
	}
	stack = NULL;
	//D_visit, stack과 동일
	if (B_visit)
		free(B_visit);
	B_visit = NULL;
	while(Q_h) {
		temp = Q_h;
		Q_h = Q_h->next;
		free(temp);
	}
	Q_h = NULL;
	Q_t = NULL;
	menu->SetPopupItem("Show BFS", false);  //체크 초기화
	menu->SetPopupItem("Show DFS", false);
	menu->SetPopupItem("key_game", false);
	menu->SetPopupItem("mouse_game", false);
	//사용한 전역변수들 초기화.
	isdfs = 0;
	isbfs = 0;
	quitg = 0;
	isOpen = 0;
	HEIGHT = 0;
	WIDTH=0;
	fi = 0;
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수. 그냥 성공시 true반환
	int ck;
	D_visit = (int*)calloc(HEIGHT * WIDTH, sizeof(int));
	ck=df(0); //실질적으로 df에서 DFS위한 모든 것 생성함
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
		if (stack->num == target_r*WIDTH+target_c) {
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
			ofDrawLine(r * path_w + path_w/2, c * path_w + path_w/2, r * path_w + path_w/2, c * path_w + (path_w+ path_w/2));
			s_push(u);
		}
		//down탐색 불가능 및 right 탐색 가능경우. 모든 경우가 다 같은 방식이다. 
		else if (maze[c][r].right == OPEN && D_visit[maze[c][r + 1].num] == 0) {
			u = maze[c][r + 1].num;
			D_visit[u] = 1;
			ofDrawLine(r * path_w + path_w/2, c * path_w + path_w/2, r * path_w + (path_w+ path_w/2), c * path_w + path_w/2);
			s_push(u);
		}
		else if (maze[c][r].up == OPEN && D_visit[maze[c - 1][r].num] == 0) {
			u = maze[c - 1][r].num;
			D_visit[u] = 1;
			ofDrawLine(r * path_w + path_w/2, c * path_w - path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
			s_push(u);
		}
		else if (maze[c][r].left == OPEN && D_visit[maze[c][r - 1].num] == 0) {
			u = maze[c][r - 1].num;
			D_visit[u] = 1;
			ofDrawLine(r * path_w + path_w/2, c * path_w + path_w/2, r * path_w - path_w/2, c * path_w + path_w/2);
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
bool ofApp::s_empty() { //비었음을 나타내는 함수. 다만 if(stack)으로 더 쉽게 표기 가능하므로 큐의 empty는 만들지 않음
	if (stack == NULL)
		return true;
	return false;
}
void ofApp::dfsdraw()
{
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
		ofDrawLine(r* path_w + path_w/2, c* path_w + path_w/2, r2* path_w + path_w/2, c2* path_w + path_w/2);
		temp = temp->next;
	}
}
//pj.미로생성용. 11주차 코드 그대로 사용.
void ofApp::make_maze(){
	//당연히 이 생성도 가장 초기 단계이므로 일단 free시켜줌
	freeMemory();
	srand((unsigned int)time(NULL));
	printf("\n높이,너비 입력:");
	scanf("%d %d", &HEIGHT, &WIDTH);
	int i, j, count = 1, random;
	int nu;
	maze = (M**)malloc(sizeof(M*) * HEIGHT);
	//배열 초기화
	for (i = 0; i < HEIGHT; i++) {
		maze[i] = (M*)malloc(sizeof(M) * WIDTH);
		for (j = 0; j < WIDTH; j++) {
			maze[i][j].num = count;
			count++;
			maze[i][j].down = CLOSE;
			maze[i][j].up = CLOSE;
			maze[i][j].right = CLOSE;
			maze[i][j].left = CLOSE;
			if (i == 0)
				maze[i][j].up = WALL;
			if (i == WIDTH - 1)
				maze[i][j].down = WALL;
			if (j == 0)
				maze[i][j].left = WALL;
			if (j == WIDTH - 1)
				maze[i][j].right = WALL;
		}
	}
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			//1행에선 마지막 열까지 검사 필요없음. 단지 오른쪽을 뚫을지 말지만 결정
			if (i == 0) {
				if (j == WIDTH - 1)//마지막열 불필요
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
				//위아래 뚫을지 말지 결정하고
				random = rand() % 2;
				maze[i][j].up = random;
				maze[i - 1][j].down = random;
				if (random == 1) { //뚫릴시 숫자 동기화
					nu = min(maze[i - 1][j].num, maze[i][j].num);
					maze[i - 1][j].num = nu;
					maze[i][j].num = nu;
				}
				else {
					//안뚫렸을시, 만약 경계선이라면(위의 행이 달라지는 경계)
					if (j != WIDTH - 1) {
						if (maze[i - 1][j].num != maze[i - 1][j + 1].num)
							//하나도 안뚫린 건지 검사
							checking_line(i, j, maze);
					}
					else//마지막 열이라면 그냥 즉시 검사(경계가 자기자신)
					{
						checking_line(i, j, maze);
					}
				}
				if (j != 0)//만약 위의 행 두칸이 같고 현재행 두칸도 같다면 절대 뚫리면 안됨(생략가능)
				{
					if (maze[i - 1][j].num == maze[i - 1][j - 1].num && maze[i][j].num == maze[i][j - 1].num) {
						maze[i][j].left = CLOSE;
						maze[i][j - 1].right = CLOSE;
					}
				}
				//마지막 열에서는 그 행을 뚫을지 말지 검사
				if (j == WIDTH - 1) {
					make_col(i, j, maze);
					//onemore(i, j, maze);
				}
			}
		}
	}
	//모든 배열 만들고, 마지막 행 뚫기 작업
	final_make(maze);
	//각 인덱스 원소 초기화
	count = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			maze[i][j].num = count++;
		}
	}
	target_r = HEIGHT - 1;
	target_c = WIDTH - 1;
	isOpen = 1;
}
void ofApp::make_col(int i, int j, M** m) {
	int same_end, same_start, left_onemore = 0;
	int random, nu, t, q;
	int back, front;
	for (t = 0; t < WIDTH - 1; t++) {
		//이미 같은 수거나, 닫기로 결심할시 pass. 이 외엔 open해야 하므로 준비시작
		if (m[i][t].num == m[i][t + 1].num)
			continue;
		random = rand() % 2;
		if (random == CLOSE)
			continue;
		same_start = t + 1;
		same_end = t + 1;
		//앞의 목표수
		front = m[i][same_start].num;
		if (t != 0) {
			//뒤의 목표수
			if (m[i][t].num > m[i][t + 1].num)
				left_onemore = 1;
			back = m[i][t].num;
		}
		/*
		for (same_end; same_end<col && m[i][same_start].num == m[i][same_end].num; same_end++); //어디까지 같은것이었는지.
		same_end--;*/
		//동기화할 수는 최솟값
		nu = min(m[i][t + 1].num, m[i][t].num);
		m[i][t].num = nu;
		for (q = same_start; q < WIDTH; q++) { //앞으로 같은얘들 동기ㅗ하
			if (m[i][q].num == front)
				m[i][q].num = nu;
			if (q == same_start) {
				m[i][q].left = OPEN;
				m[i][t].right = OPEN;
			}
		}
		if (left_onemore) {//뒤로도 같은부분 동기화
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
void ofApp::final_make(M** ma) {
	int j, i = HEIGHT - 1;
	int tem, t, nu;
	for (j = 0; j < WIDTH - 1; j++) {
		//이미 뚫려있다면 패스
		if (ma[i][j].right) {
			continue;
		}
		//안뚫려있고, 숫자도 다르다면 
		if (ma[i][j].num != ma[i][j + 1].num) {
			//목표 수 찾고
			tem = ma[i][j + 1].num;
			nu = min(ma[i][j].num, ma[i][j + 1].num);
			//목표수를 가진 모든 것들 동기화
			for (t = j + 1; t < WIDTH; t++) {
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
void ofApp::checking_line(int i, int j, M** m) {
	int target = m[i - 1][j].num;
	int start, rands = 0;
	for (start = 0; start < WIDTH; start++) {
		if (m[i - 1][start].num == target) //시작위치찾기
			break;
	}
	int t, diff = 0, random, nu;
	for (t = start; t <= j; t++) {//시작부터 경계까지 다 다른지 검사(모두 닫혀있는지)
		if (m[i - 1][t].num == target) {
			if (m[i - 1][t].num != m[i][t].num)
				diff = 1;
			else
			{
				diff = 0;
				break;
			}
		}
	}
	if (diff == 1) {//다 닫혀있다면, 다시 random으로 뚫을지 말지 결정
		for (t = start; t <= j; t++) {
			if (m[i - 1][t].num == target) {
				random = rand() % 2;
				m[i][t].up = random;
				m[i - 1][t].down = random;
				if (random == 1) {
					nu = min(m[i][t].num, m[i - 1][t].num);
					m[i][t].num = nu;
					m[i - 1][t].num = nu;
					rands = 1;  //위아래 뚫기 성공
					//printf("\n%d %d %d\n", nu, m[i][t].num, m[i - 1][t].num);
				}
			}
		}
		//하나도 안뚫렸다면 다시 검사실행
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

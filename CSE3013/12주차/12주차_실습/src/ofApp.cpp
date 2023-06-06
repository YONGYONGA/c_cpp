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
	menu->AddPopupSeparator(hPopup); //�Ǽ�
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
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here
	//ó������ ������ 4������ Ž���ϸ� ���� �����ִ°��� �ƴ϶�� ���α׸���
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


void ofApp::doTopmost(bool bTop)  //��Ȯ���� �𸣰����� ����â�� ���� ������ ����ִ� �Լ��ε���.(��� �������� ���α׷�, ������ �۾�ǥ���ٺ��ٵ� ������)
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
			int cnt = 0; //¦�� Ȧ�� ���� Ž��
			int c = 0; //�� Ž��, ����
			int n = 0; //�迭�� ���� num
			WIDTH = 0, HEIGHT = 0;
			int i;
			
			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			maze = (M**)malloc(sizeof(M*));
			ofBuffer::Line s = buffer.getLines().begin(), e = buffer.getLines().end();
			for (; s != e; s++) {
				string line = *s;
				if (WIDTH == 0) { //���ڿ����� ����ũ�� /2�� �ʺ��̴�. (���� ����)
					WIDTH = (line.size())/2;
				}

				if (cnt == 0) { //ù�࿡�� �׳�
					cnt++;
					continue;
				}
				else if (cnt%2 == 1) { //Ȧ����° ������ ������, �������� �շ��ִ��� Ȯ�� ��, ����(�� ���� ����)�� �����Ҽ� �ִ� �����̴�.
					if(c==0) //ù��°���� ���� ���� �����Ҵ�(�̹� ������ �迭 ù���� �����ҵ��� �ߴ�.)
						maze[c] = (M*)malloc(sizeof(M) * WIDTH);
					else //�ΰ��� �� �̻� �ִٴ� ���̹Ƿ�, Ȧ�� ���θ��� realoc���� ���Ҵ� �� ���� �� �����Ҵ�
					{
						maze = (M**)realloc(maze, sizeof(M*) * (c + 1));
						maze[c] = (M*)malloc(sizeof(M) * WIDTH);

					}
					for (i=0; i < WIDTH; i++) {
						if (line[i*2] == ' ') { //��쿡 ���� open, close�����Ű��. �翬�� i=0�϶� left�� �ʿ��ϴ�. �̶� �ֿ� ���ڰ� ����Ǵ� ��ġ�� i*2��° �ε����� �ִٴ� ���� ����.(�̺κ� �̿ܿ��� �׳� ��ο� blank��)
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
					//������ �������� right�� �ʿ��ϴ�.
					maze[c][WIDTH - 1].right = WALL;
				}
				else if (cnt%2 == 0) {//¦�� ��° ����(ù��° ���� ����)�� �� �Ʒ��� �շ��ִ��� Ȯ���ϴ� ����. �߰������� �迭�� num�ʱ�ȭ �� ���������� �Ѵ�.
					if (c == 0) { //���� �ϳ��ۿ� ���¶����, �翬�� up�κ��� ��
						for (i=0; i < WIDTH; i++) { //�ֿ� ���ڰ� ����Ǵ� ��ġ�� Ȧ����° �ε����̴�. �̰� �̿ܿ��� ĭ������ ���� +��. 
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
					else //���� 2�� �̻� �ִٸ� �� ������ up�� ���� down�� ������.
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
					c++; //���� �� ���� �׸��� Ž�� �غ� ����
				}
				cnt++; //¦���������� Ȧ���������� ����
			}
			HEIGHT = c; //���̵���ȭ
			target_c = HEIGHT - 1;
			target_r = WIDTH - 1;
			for (i=0; i < WIDTH; i++) { //������ �࿡���� down�� close���� wall�� �ٲ��ش�.
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
		//Ż�� ����. ���� �ֱٿ� �߰��Ȱ��� target�̸�.
		if (Q_t->num == WIDTH * HEIGHT - 1) {
			return true;
			break;
		}
		//Ž�� num�� ���� ó���� �߰���(���� ���߿� �߰���) Q_h�� num
		u = Q_h->num;
		r1 = u % WIDTH;
		c1 = u / WIDTH;
		//bfs�� �շ��ִ°� ��� �κ��� Ž���ϴ� �˰���. �� if���� �ʿ�.
		//���� Ž�� num �κ� maze�� ���κ��� �����ְ�, �湮������ ���ٸ� (4���� if�� ��� ����. BFS�̱� ������ �� while���� 4���� ��ü Ž��. 4���� Ž�� ��� ���н� ck=0)
		if (maze[c1][r1].up == OPEN && B_visit[maze[c1 - 1][r1].num].num == 0) {
			//Ž�� num �ʱ�ȭ
			u = maze[c1 - 1][r1].num;
			r = u % WIDTH;
			c = u / WIDTH;
			//���ٰ� ��׸���
			ofDrawLine(r1 * 22 + 11, c1 * 22 + 11, r * 22 + 11, c * 22 + 11);
			//���ٰ� ǥ�� ��, ��� �°��� ǥ��
			B_visit[u].num = 1;
			B_visit[u].from = Q_h->num;
			//ť �߰�
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
		//4���� Ž�� ����. �� �����ϼ��� ����. �̷��� Q���� ����.
		if (ck == 0)
		{
			Q_delete();
		}

	}
	//ť�� �������� Ž���� �������Ƿ� ��ã�� �����ߴٴ� ��.
	return false;
}
void ofApp::bfsdraw() {
	int i, r, c,r1,c1;
	ofSetColor(ofColor::red);
	for (i =  WIDTH* HEIGHT - 1; i != 0;) {
		
		//2���� ���ҷ� ��ȯ
		r = i % WIDTH;
		c = i / WIDTH;
		//���� �湮 num���� �ʱ�ȭ
		i = B_visit[i].from;
		//2�������ҷ� ��ȯ
		r1 = i % WIDTH;
		c1 = i / WIDTH;
		//���׸���
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
	//Q�߰� ���. �ƹ��͵� ������ head ����, �ִٸ� tail�� next�� ���ο� ���Ұ� �� ��
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
	//��� �̵��� ���� ��� ����(ť�� ����)
	N* temp = Q_h;
	Q_h = Q_h->next;
	free(temp);
}
void ofApp::freeMemory() {
	//�̷� free(��)
	for (int i = 0; i < HEIGHT; i++)
		free(maze[i]);
	//�̷� ��ü free
	free(maze);
	//�̷� �ʱ�ȭ
	maze = NULL;
	//dfs�� ����ϴ� visit.free
	if (D_visit)
		free(D_visit);
	//�ʱ�ȭ
	D_visit = NULL;
	N* temp;
	//�����ʱ�ȭ
	if (stack) {
		temp = stack;
		stack = stack->next;
		free(temp);
	}
	stack = NULL;
	//DFS�� ���� �ƶ�
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
	//����� ���������� �ʱ�ȭ
	isdfs = 0;
	isbfs = 0;
	isOpen = 0;
	HEIGHT = 0;
	WIDTH=0;
}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	//TO DO
	//DFSŽ���� �ϴ� �Լ� ( 3����)
	int ck;
	D_visit = (int*)calloc(HEIGHT * WIDTH, sizeof(int));
	ck=df(0);
	//��ã�� ����
	if (ck == 1) {
		isdfs = 1;
		return TRUE;
	}
	else//����
	{
		isdfs = -1;
		return false;
	}
}
int ofApp::df(int v) { //�Ǽ��� ����. �׳� DFS�� �־ ��.
	D_visit[v] = 1;
	s_push(v);
	int c, r, u;
	//printf("%d  ", v);
	ofSetColor(ofColor::green);
	while (stack != NULL) {
		//Ż�� ����. ������ head(���� �ֱ� �߰��� ����)�� num��  target�̸� �����ߴٴ� ���̴� Ż��
		if (stack->num == HEIGHT * WIDTH - 1) {
			return 1;
			break;
		}
		//2���� ���ҷ� ��ȯ
		r = stack->num % WIDTH;
		c = stack->num / WIDTH;
		//dfs�� �ϴ� �շ��ִ� �� �ѹ����� ���������� ���� Ž���ϴ� ���. �� if else�� �ʿ�
		//�ִ��� ������ ���̱� ���� down�� right�� ���� ����ǰ�(�� �������� ���� �׳��� ���� ���ɼ����� ������ ����). down���� ���� �ְ�, �湮�� ���� ���� ����� if
		if (maze[c][r].down == OPEN && D_visit[maze[c + 1][r].num] == 0) {
			//Ž�� num�ʱ�ȭ
			u = maze[c + 1][r].num;
			//�湮 ǥ��
			D_visit[u] = 1;
			//���׸��� ���ÿ� �߰�
			ofDrawLine(r * 22 + 11, c * 22 + 11, r * 22 + 11, c * 22 + 33);
			s_push(u);
		}
		//downŽ�� �Ұ��� �� right Ž�� ���ɰ��. ��� ��찡 �� ���� ����̴�. 
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
		//��ü���� Ž�� ����. �� ������ �����Ƿ� ���ÿ��� ����(������ Ž���ߴ� ������ ���ư�������)
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
	//���� �߰� �Լ�. ������ �ܼ��� �տ� �߰����ָ� ��.
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
	//���� ����. �ܼ��� �� ����
	N* temp = stack;
	stack = stack->next;
	free(temp);
}
bool ofApp::s_empty() { //������� ��Ÿ���� �Լ�. �ٸ� if(stack)���� �� ���� ǥ�� �����ϹǷ� ť�� empty�� ������ ����
	if (stack == NULL)
		return true;
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS�� ������ ����� �׸���. (3���� ����)
	//dfs�� �����ִ� �������� �״�� ��� ������ �����ϴ�.(������ ��ζ�� pop�� ���� �ʱ� ����)
	N* temp = stack;
	int c, r,c2,r2;
	ofSetColor(ofColor::red);
	//���Ұ� 2���� �ʿ��ϹǷ� next�� �������� Ž��.
	while (temp->next) {
		//2����  ���� ��ȯ
		r = temp->num % WIDTH;
		c = temp->num / WIDTH;
		r2= temp->next->num % WIDTH;
		c2 = temp->next->num / WIDTH;
		//���׸���
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
			printf("��+");
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
			printf("��+");
		}
	}
	printf("\n");

}
*/
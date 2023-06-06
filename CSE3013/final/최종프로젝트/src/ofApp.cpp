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
	menu->AddPopupSeparator(hPopup); //�Ǽ�
	//���ο� �̷� �ڵ�����
	menu->AddPopupItem(hPopup, "Make maze", false, false);

	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	//false�� ���ٸ� Ŭ���ÿ� �޴��� üũǥ�ð� �����ȴ�. �������� ����
	menu->AddPopupItem(hPopup, "Show DFS");
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check
	
	//���� �޴� ����. Ű���Ӱ� ���콺����
	hPopup = menu->AddPopupMenu(hMenu, "Game");
	menu->AddPopupItem(hPopup, "key_game");
	menu->AddPopupItem(hPopup, "mouse_game");

	//���� �޴� ����. ��ǥ�����̳� ���� �ʺ� ���� ����.
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
	//SETpopup...("false")�� �޴�â�� üũ�� ���� �����ϴ� ��� �ڵ��̴�. ���࿡ ū ������ �ִ� �ڵ�� �ƴ�����, �̰��� �Ʒ� 4���� �ϳ��� ������ üũǥ�ð� �Ǿ��ִٸ�, �� üũǥ�ø� Ǯ���ش�.
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
			quitg = 0;//���� �������̸� ���� ����
			//DFS���� ������ üũǥ�� ����.
			menu->SetPopupItem("key_game", false);
			menu->SetPopupItem("mouse_game", false);
			menu->SetPopupItem("Show BFS", false);
			//BFS���̶�� BFS�� ����
			isbfs = 0;
			//dfs���̸� dfs����, dfs���������� �ʴٸ� dfs. �ٽø��� Ŭ������ DFS�� ���� �ִ� �Ҽ� �հ���
			if (isdfs)
				isdfs = 0;
			else
				isdfs = 1;
		}
		else {
			//�����޽��� ��, DFSüũ�Ǵ� �� ����
			cout << "you must open file first" << endl;
			menu->SetPopupItem("Show DFS", false);
		}
		
	}

	if(title == "Show BFS") {
		if (isOpen)
		{
			//DFS�� �帧 ���� ����. 
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
			//������ 3�� �޴� üũ �Ǿ��ִٸ� üũ����
			menu->SetPopupItem("mouse_game", false);
			menu->SetPopupItem("Show BFS", false);
			menu->SetPopupItem("Show DFS", false);
			printf("\n�� �μ��Ⱑ ������ Ű����� �̵��ϴ� ��ã�� �̷��Դϴ�. \nb�� ������, ,w,s,a,d�� ����, ���� �����¿��� ���� �ν����� �̵������� ���ϴ�.(���� �ٱ��� ���� �μ��� �����ϴ�.)");
			printf("b�� ��������, �ٸ� Ű�� �����ų� �μ��� ���� �������� �����ٸ� ���� �ν����� �ʽ��ϴ�. �ٽ� ���� �μ��� �ʹٸ� �ٽ� b�� ������, wasd�� �����ּ���.");
			printf("\n������ ����: %d �ʺ�: %d\n", HEIGHT, WIDTH);
			//��ȿ�� target�Է��Ҷ����� ��� �Է¹���
			do {
				printf("0���� ũ��, ���̿� �ʺ񺸴� �۰ų� ���� �������� �Է��ϼ��� (y��,x��) : ");
				scanf("%d %d", &target_r, &target_c);
			} while ((target_r > HEIGHT || target_c > WIDTH) || (target_r <= 0 || target_c <= 0));
			//�迭�ν��� Ÿ�� ����
			target_c--;
			target_r--;
			//Ű ���ӽ���
			key_gaming = 1;
			//���� ��ġ
			now_r = 0;
			now_c = 0;
			//���ӽ���
			quitg = 1;
			//������Ƚ��, �μ�Ƚ�� �ʱ�ȭ
			moving = 0;
			br = 0;
			//�������� �ʱ�ȭ
			succ = 0;
			//�ð� ���� �ʱ�ȭ
			start = clock();
			u = start;
			fi = 0;
			//DFs,BFS��� ����
			isdfs = 0; isbfs = 0;
		}
		else {//�̷ΰ� ���������� �ʴٸ� �����޽��� �� üũ����.
			cout << "you must open file first" << endl;
			menu->SetPopupItem("key_game", false);
		}
	}
	if (title == "mouse_game") {
		if (isOpen) {
			menu->SetPopupItem("key_game", false);
			menu->SetPopupItem("Show BFS", false);
			menu->SetPopupItem("Show DFS", false);
			printf("\n���콺�� �̵��ϴ� ��ã�� �̷��Դϴ�. ���콺�� ���� ������ ���ӿ����̴� �������ּ���.");
			printf("\n������ ����: %d �ʺ�: %d\n", HEIGHT, WIDTH);
			//��ȿ�� Ÿ��, �� ���� �ʺ� ���������� ��� �ݺ�
			do {
				printf("0���� ũ��, ���̿� �ʺ񺸴� �۰ų� ���� �������� �Է��ϼ��� (y��,x��) : ");
				scanf("%d %d", &target_r, &target_c);
			} while ((target_r > HEIGHT || target_c > WIDTH) || (target_r <= 0 || target_c <= 0));
			do {
				printf("10�̻��� ���� ���� �������ּ���(20�̻��� ��õ�帳�ϴ�. ���� Ŭ���� �����ϴ�.): ");
				scanf("%d", &path_w);
			} while (path_w < 10);
			target_c--;
			target_r--;
			//���콺 ����, ����, ����, inm ������ �ʱ�ȭ
			mouse_gaming = 1;
			quitg = 1;
			succ = 0;
			inm = 0;
			//�ð� �ʱ�ȭ
			start = clock();
			u = start;
			fi = 0;
			//DFs,BFS��� ����
			isdfs = 0; isbfs = 0;
		}
		else {//�̷� �������� ���� ���
			cout << "you must open file first" << endl;
			menu->SetPopupItem("mouse_game", false);
		}
	}
	if (title == "change target") {
		if (isOpen) {
			int aa, bb;
			aa = target_c;
			bb = target_r;
			printf("\n���� Ÿ��: (%d,%d)\n", bb+1, aa+1);
			do {
				printf("������ Ÿ���� �Է��ϼ��� (y��,x��): ");
				scanf("%d %d", &target_r, &target_c);
			} while (target_r > HEIGHT || target_c > WIDTH);
			//������ ������ ����� ���� ǥ������ aa,bb���� ����
			printf("y��: %d -> %d , x��: %d -> %d", bb+1, target_r, aa+1, target_c);
			target_r--;
			target_c--;
		}
		else
			cout << "you must open file first" << endl;
	}
	if (title == "change width") {
		if (isOpen) {
			int aa = path_w;
			printf("\n������ ���� �ʺ� �Է��ϼ���: ");
			scanf("%d", &path_w);
			//������ ������ ����� ���� ǥ������ aa���� ����
			printf("���� �ʺ�: %d -> %d", aa, path_w);
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
	//�����̽��� ����. ������ ������
	char str[256];
	//�ð��� ����
	string times;
	//���콺�� x,y��ġ ����� ����
	int mX, mY;
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	if (quitg == 1) {
		//DFs,BFS����
		isdfs = 0;
		isbfs = 0;
		//Ÿ�� ��¿뺯�� (���� �ٲ��� �ʴ� �κ�)
		times = "game times: ";
		//Ű���� �̶��
		if (key_gaming == 1) {
			//������ġ�� �� ��ġ ������ ǥ��
			ofSetColor(127, 23, 31);
			ofDrawCircle(target_c * path_w + path_w / 2, target_r * path_w + path_w / 2, 2);
			ofSetColor(22, 22, 22);
			ofDrawCircle( now_c * path_w + path_w / 2, now_r * path_w + path_w / 2, 2);
			ofSetColor(77, 77, 77);
			//start�ð� ����
			start = clock();
			//u�� ������ �ð��̴�, ����Ǵ� �ð����� ���� �̿��� �ҿ�ð� ����
			fi = (start - u) / (double)1000;
			times += to_string(fi);
			//�ҿ�ð� ������â�� ǥ��
			myFont.drawString(times, ofGetWidth() - 200, ofGetHeight() - 70);
		}
		//���콺 ���� ��, ���� �ʱ� ��Ȳ�̶��(inm=0)
		if (mouse_gaming == 1 && inm==0) {
			//���콺 ���ӿ� �̷� �׸���
			draw_gamebox();
			ofSetColor(ofColor::red);
			//���޽���. ���콺�� �������� ���� ������� ��
			myFont.drawString("Put mouse in blue box for start game", ofGetWidth() - 333, ofGetHeight() - 50);
			//���콺 Ŀ���� �������� ����
			if (ofGetMouseX() < (path_w / 5 * 4) && ofGetMouseY() < (path_w / 5 * 4) && ofGetMouseX() > 3 && ofGetMouseY() > 3) {
				inm = 1; //���� ���ۺ��� ����
				return;
			}
			else
			{
				inm = 0; //�׿� Ŀ���� �ö����� ��� return
				return;
			}
		}
		if (mouse_gaming == 1 && inm == 1) {//���콺 Ŀ���� ������ġ�� ���Դ�. �ʱ� ����
			start = clock(); //�ð� �ʱ�ȭ. u�� �����̴�.
			u = start;
			ofSetBackgroundAuto(false); //���콺 �̵���θ� ������ �ʱ� ����
			ofSetFrameRate(10000);//���콺 ��ġ �ν��� �ʴ� 10000��. ������ ������ ���Ҽ��� ���̸� �ȴ�.
			inm++;//��¥ ���� �����ϰ� ���� ����
			draw_gamebox(); //�ʱ⼼��
			ofSetColor(127, 23, 31);
			ofDrawRectangle((target_c)*path_w+2, (target_r)*path_w+2, path_w-2, path_w-2); //Ÿ�� ��ġ���� �ڽ��� ǥ��
			return;
		}
		if (mouse_gaming == 1 && inm == 2) {//������
			fi= (start - u) / (double)1000;    //�ð� ���, �����
			ofSetColor(255, 255, 255);    //backgroundauto �� false�̱⿡, ��� ���ϴ� �ð��� ǥ���ϱ� ���ؼ�, ���� �����ӿ��� ǥ���� �ð��� �����־�� �Ѵ�. �ƴϸ� ���ĺ���. ��,���� �׳� ���� ��� ���ڷ� �������
			ofDrawRectangle(ofGetWidth() - 200, ofGetHeight() - 100, ofGetWidth(), ofGetHeight() - 70);

			ofSetColor(77, 77, 77);
			times += to_string(fi);
			myFont.drawString(times, ofGetWidth() - 200, ofGetHeight() - 70); //�ð� ���
			start = clock(); //start ����
			ofSetColor(ofColor::purple);//���� ���������
			mX = ofGetMouseX();  //���콺 ��ġ�� �ǽð����� ��� ���ϹǷ�, �ѹ� if�� �������� ��ġ�� ������Ų ������ ������, �̸� �̿��ؼ� ��� �˻����
			mY = ofGetMouseY();
			ofDrawCircle(mX, mY, 2); //���콺 ��ġ�� ���׸���
			if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) { //���� ���콺 Ŭ���� ���� ����
				quitg = 0;
			}
			if (!touch_wall(mX,mY)) { //���콺 �̵��������� üũ. ���� �ε��� ���̶� �ǴܵǸ� if������ ����.
				ofSetColor(ofColor::red);//�浹��ġ ǥ�ô� ������
				ofDrawCircle(mX, mY, 4); //�浹��ġ ǥ��
				quitg = 0;   //�浹������ ���� ����
			}
			if (mX < (target_c+1) * path_w-2 && mY < (target_r+1) * path_w-2 && mX > ((target_c) * path_w)+2 && mY > ((target_r) * path_w)+2) { //�������� ���콺 Ŀ�� ����
				ofSetColor(77,77,77); //������ġ ǥ��
				ofDrawCircle(mX, mY, 4);  //''
				succ = 1;  //����
				quitg = 0; //��������
			}
			return;
		}
			
	}
	else
	{    //���� ����Ȱ� Ű�����̶��
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
			//���� ����� ����� ���ڿ�. ����Ȥ�� ����, ������ Ƚ��, �μ� Ƚ��, �ɸ��ð� �� ��� ���Ѵ�.
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
			//Ű���� ��
			key_gaming = 0;
			//�˶�â
			ofSystemAlertDialog(str1);
			//����üũǥ�� ����
			menu->SetPopupItem("key_game", false);
		}
		
	}
	//������ ���콺 �����̶��
	if (mouse_gaming == 1) {
		//����üũǥ�� ����
		menu->SetPopupItem("mouse_game", false);
		string str1;
		//Ű���Ӱ� ���������� �˶�â. ��� ���� Ȥ�� ���п� �ð��� ǥ��
		if (succ == 1)
			str1 = "success!!\n";
		else
			str1 = "fail!!\n";
		str1 += "total time: ";
		str1 += to_string(fi);
		ofSystemAlertDialog(str1);
		//���콺 ���� ������ �ٽ� ������� ���� �⺻ �ɼǵ� �ʱ�ȭ.(������ �ӵ�, ��� ��� �ʱ�ȭ)
		ofSetBackgroundAuto(true);
		ofSetFrameRate(15);
		ofBackground(255, 255, 255);
		//���콺 ���ӳ�
		mouse_gaming = 0;
	}

	//ó������ ������ 4������ Ž���ϸ� ���� �����ִ°��� �ƴ϶�� ���α׸���
	ofSetColor(100);
	ofSetLineWidth(5);
	//�׳� ��� �ε������� 4���� �� �׸���.
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

	if (isdfs)//DFSŬ���� 1
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		//a���� ������ �����ִٸ�..
		if (isOpen) {
			N* temp;
			//DFS�� �� ��� ǥ��
			if(DFS())
				//��� ã�� ������ ���ǥ��
				dfsdraw();
			//vist�迭�� stack�迭 ��� �ʱ�ȭ
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
	if (isbfs) {//BFSŬ���� 1
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {//DFS�� ���� ����
			N* temp;
			if (BFS())
				bfsdraw();
			//visit�� ť �ʱ�ȭ
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
	//�����̽� ������ ��
	if(bShowInfo) {
		if (quitg == 0) {
			sprintf(str, " comsil project");
			myFont.drawString(str, 15, ofGetHeight() - 20);
		}

	}

} // end Draw


void ofApp::doFullScreen(bool bFull)//Ǯ��ũ�� �Լ���. ��ǻ� �մ��� �ʾҰ� ������Ʈ �����̾ƴ�.
{
	// Enter full screen
	if(bFull) {//�׳� Ǯ��ũ�϶��� ���� �޴�â ���ְ� ȭ���ä��� ��
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


void ofApp::doTopmost(bool bTop)  //�մ��� ����. ��Ȯ���� �𸣰����� ����â�� ���� ������ ����ִ� �Լ��ε���.(��� �������� ���α׷�, ������ �۾�ǥ���ٺ��ٵ� ������)
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
	if(key == VK_ESCAPE) { //esc�� Ż��. Ǯ��ũ���϶��� ��üȭ�� ����
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

	if(key == 'f') { //Ǯ��ũ��
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		breaking = 0;
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
	if (key == 'b')  //���μ��غ�
		breaking = 1;
	if (key == 'q') { //���� ����
		quitg = 0;
	}
	if (key == 'w') { //���� �� �μ���
		if (breaking == 1) {
			if (maze[now_r][now_c].up == CLOSE) {
				maze[now_r][now_c].up = OPEN;
				maze[now_r-1][now_c].down = OPEN;
				br++;
			}
		}
		breaking = 0;
	}
	if (key == 'a') { //���ʺ� �μ���
		if (breaking == 1) {
			if (maze[now_r][now_c].left == CLOSE) {
				maze[now_r][now_c].left = OPEN;
				maze[now_r][now_c-1].right = OPEN;
				br++;
	
			}
		}
		breaking = 0;
	}
	if (key == 's') {//�Ʒ��� �μ���
		if (breaking == 1) {
			if (maze[now_r][now_c].down == CLOSE) {
				maze[now_r][now_c].down = OPEN;
				maze[now_r+1][now_c].up = OPEN;
				br++;
			}
		}
		breaking = 0;
	}
	if (key == 'd') {//������ ���μ���
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
	if (key == OF_KEY_DOWN) {//�Ʒ��� �̵�
		breaking = 0;
		if (key_gaming == 1) {
			if (maze[now_r][now_c].down == OPEN) {
				now_r++;  //��ġ����
				moving++; //������ Ƚ��
				end_g(); //������ ����Ǿ����� Ȯ��
			}
		}
	}
	if (key == OF_KEY_RIGHT) { //������ �̵�. ����� ����
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
	ofSetColor(100);  //���콺���ӿ� ȭ��׸���. ���ۺ���� �Ķ������� ǥ������. (�̷�ǥ�ô� �翬)
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
void ofApp::end_g() {  //���� ��ġ�� Ÿ�ٿ� �����ߴٸ�
	if (now_c == target_c && now_r == target_r) {
		succ = 1; //���Ӽ���
		quitg = 0; //�� ��������
	}
}
bool ofApp::touch_wall(int x, int y) {  //���콺 ���ӿ�, ���� �ε����� ���������ϱ� ����
	int rx=x, ry=y;
	int numx,numy;
	//�ణ�� �Ǵ���ġ ����. ��Ȯ�� ���� �ƴ϶� �� �߾� ��ǥ�� +-2�������� ��Ƶ� �� �߾ӿ� ��°����� �Ǵ�����. �̷��� �ҽ�, ���� ���ӻ��� ���� ��¦�� ��Ƶ� Ż������.
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
	//��ǥ�� ���� �̷� �ȿ� �ִٸ� �˻����
	if (x >= 0 && x <= WIDTH * path_w && y >= 0 && y <= HEIGHT * path_w) {
		if (rx % path_w == 0 || ry % path_w == 0) {
			numx = rx / path_w; //��Ȯ�� �̷��� �����° �ε����� ���� �ε������� Ȯ���ϱ� ����
			numy = ry / path_w;
			if (rx == 0 || ry == 0) {//���� ���κ��̰ų�, ���� ���ʺκ��� ��
				return false;
			}
			else if (ry % path_w == 0) { //�Ʒ� (Ȥ�� ���� ����)���� ��������. a�� �������� ��¦ �Ʒ��� �ִ�, �����ִ�, �Ǵ���ġ�� �����߱⿡ ry�� ��Ȯ�� a�� �߽��� y��ǥ �̴�.
				if (maze[numy - 1][numx].down != OPEN) { //���� ���϶��� ���� if���� �˻������Ƿ�, down�� �˻�����ָ� ��
					return false;  //�ε������̴� false
				}
			}
			else if (rx % path_w == 0) { //������(Ȥ�� ��)�� ���� ��������. ���������� �߽��� x��ǥ��
				if (maze[numy][numx - 1].right != OPEN) { //���� �����϶��� �˻������Ƿ� right�� �˻�
					return false;
				}
			}
			else //���� ��� open�̰�, ���κе� �ƴ�. ������ �κ��̶�� ��
			{
				return true;
			}
		}
		else //�׳� �� (�� �̷� ĭ)
		{
			return true;
		}
	}
	else { //�̷� ���̸� �翬�� ���д�
		return false;
	}
	return true; //���� �������� ���� ��� false���� ó���� ���״�.��, ������ ���� true;
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
			//�̷ΰ� �̹� �����Ѵٸ�, �ʱ�ȭ�� �ʼ�
			if (maze) {
				freeMemory();
			}
			//�׳� ����� 1. �Ⱦ�����
			int input_flag = 0;

			// Idx is a variable for index of array.

			// Read file line by line
			int cnt = 0; //¦�� Ȧ�� ���� Ž��
			int c = 0; //�� Ž��, ����
			int n = 0; //�迭�� ���� num
			WIDTH = 0, HEIGHT = 0;
			int i;
			
			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			//������� ������ �̷ΰ� �ִ� ���̴� �ϴ� 1�� ����
			maze = (M**)malloc(sizeof(M*));
			//s�� ����,  e�� �� ����
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
			target_r = HEIGHT - 1;
			target_c = WIDTH - 1;
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
	//visit �����Ǹ� 0���� �ʱ�ȭ��Ű�� calloc
	B_visit = (ar*)calloc(HEIGHT * WIDTH, sizeof(ar));
	B_visit[v].num = 1;
	Q_add(v);
	int r1, c1,r,c, u, ck = 0;
	ofSetColor(ofColor::blue);
	while (Q_h) {
		ck = 0;
		//Ż�� ����. ���� �ֱٿ� �߰��Ȱ��� target�̸�.
		if (Q_t->num == target_r * WIDTH + target_c) {
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
			ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
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
	for (i = target_r * WIDTH + target_c; i != 0;) {
		
		//2���� ���ҷ� ��ȯ
		r = i % WIDTH;
		c = i / WIDTH;
		//���� �湮 num���� �ʱ�ȭ
		i = B_visit[i].from;
		//2�������ҷ� ��ȯ
		r1 = i % WIDTH;
		c1 = i / WIDTH;
		//���׸���
		ofDrawLine(r1 * path_w + path_w/2, c1 * path_w + path_w/2, r * path_w + path_w/2, c * path_w + path_w/2);
	}
}
int ofApp::Q_empty() {
	if (!Q_h)//���ٸ� 1��ȯ
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
	//�̷ΰ� �ִٸ�(HEIGHT�� 1�̻��̸� �̷ΰ� �����Ѵٴ� ����) ��free, �� ��ü free
	for (int i = 0; i < HEIGHT; i++)
		free(maze[i]);
	free(maze);
	maze = NULL;
	//visit free
	if (D_visit)
		free(D_visit);
	D_visit = NULL;
	N* temp;
	//���� �ִٸ� ������ �����̵���Ű�� free(��� draw���� �׸������� �׸��� free��Ű�� ������ �̿���� ����)
	while(stack) {
		temp = stack;
		stack = stack->next;
		free(temp);
	}
	stack = NULL;
	//D_visit, stack�� ����
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
	menu->SetPopupItem("Show BFS", false);  //üũ �ʱ�ȭ
	menu->SetPopupItem("Show DFS", false);
	menu->SetPopupItem("key_game", false);
	menu->SetPopupItem("mouse_game", false);
	//����� ���������� �ʱ�ȭ.
	isdfs = 0;
	isbfs = 0;
	quitg = 0;
	isOpen = 0;
	HEIGHT = 0;
	WIDTH=0;
	fi = 0;
}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	//TO DO
	//DFSŽ���� �ϴ� �Լ�. �׳� ������ true��ȯ
	int ck;
	D_visit = (int*)calloc(HEIGHT * WIDTH, sizeof(int));
	ck=df(0); //���������� df���� DFS���� ��� �� ������
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
		if (stack->num == target_r*WIDTH+target_c) {
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
			ofDrawLine(r * path_w + path_w/2, c * path_w + path_w/2, r * path_w + path_w/2, c * path_w + (path_w+ path_w/2));
			s_push(u);
		}
		//downŽ�� �Ұ��� �� right Ž�� ���ɰ��. ��� ��찡 �� ���� ����̴�. 
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
		ofDrawLine(r* path_w + path_w/2, c* path_w + path_w/2, r2* path_w + path_w/2, c2* path_w + path_w/2);
		temp = temp->next;
	}
}
//pj.�̷λ�����. 11���� �ڵ� �״�� ���.
void ofApp::make_maze(){
	//�翬�� �� ������ ���� �ʱ� �ܰ��̹Ƿ� �ϴ� free������
	freeMemory();
	srand((unsigned int)time(NULL));
	printf("\n����,�ʺ� �Է�:");
	scanf("%d %d", &HEIGHT, &WIDTH);
	int i, j, count = 1, random;
	int nu;
	maze = (M**)malloc(sizeof(M*) * HEIGHT);
	//�迭 �ʱ�ȭ
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
			//1�࿡�� ������ ������ �˻� �ʿ����. ���� �������� ������ ������ ����
			if (i == 0) {
				if (j == WIDTH - 1)//�������� ���ʿ�
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
				//���Ʒ� ������ ���� �����ϰ�
				random = rand() % 2;
				maze[i][j].up = random;
				maze[i - 1][j].down = random;
				if (random == 1) { //�ո��� ���� ����ȭ
					nu = min(maze[i - 1][j].num, maze[i][j].num);
					maze[i - 1][j].num = nu;
					maze[i][j].num = nu;
				}
				else {
					//�ȶշ�����, ���� ��輱�̶��(���� ���� �޶����� ���)
					if (j != WIDTH - 1) {
						if (maze[i - 1][j].num != maze[i - 1][j + 1].num)
							//�ϳ��� �ȶո� ���� �˻�
							checking_line(i, j, maze);
					}
					else//������ ���̶�� �׳� ��� �˻�(��谡 �ڱ��ڽ�)
					{
						checking_line(i, j, maze);
					}
				}
				if (j != 0)//���� ���� �� ��ĭ�� ���� ������ ��ĭ�� ���ٸ� ���� �ո��� �ȵ�(��������)
				{
					if (maze[i - 1][j].num == maze[i - 1][j - 1].num && maze[i][j].num == maze[i][j - 1].num) {
						maze[i][j].left = CLOSE;
						maze[i][j - 1].right = CLOSE;
					}
				}
				//������ �������� �� ���� ������ ���� �˻�
				if (j == WIDTH - 1) {
					make_col(i, j, maze);
					//onemore(i, j, maze);
				}
			}
		}
	}
	//��� �迭 �����, ������ �� �ձ� �۾�
	final_make(maze);
	//�� �ε��� ���� �ʱ�ȭ
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
		//�̹� ���� ���ų�, �ݱ�� ����ҽ� pass. �� �ܿ� open�ؾ� �ϹǷ� �غ����
		if (m[i][t].num == m[i][t + 1].num)
			continue;
		random = rand() % 2;
		if (random == CLOSE)
			continue;
		same_start = t + 1;
		same_end = t + 1;
		//���� ��ǥ��
		front = m[i][same_start].num;
		if (t != 0) {
			//���� ��ǥ��
			if (m[i][t].num > m[i][t + 1].num)
				left_onemore = 1;
			back = m[i][t].num;
		}
		/*
		for (same_end; same_end<col && m[i][same_start].num == m[i][same_end].num; same_end++); //������ �������̾�����.
		same_end--;*/
		//����ȭ�� ���� �ּڰ�
		nu = min(m[i][t + 1].num, m[i][t].num);
		m[i][t].num = nu;
		for (q = same_start; q < WIDTH; q++) { //������ ������� �������
			if (m[i][q].num == front)
				m[i][q].num = nu;
			if (q == same_start) {
				m[i][q].left = OPEN;
				m[i][t].right = OPEN;
			}
		}
		if (left_onemore) {//�ڷε� �����κ� ����ȭ
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
		//�̹� �շ��ִٸ� �н�
		if (ma[i][j].right) {
			continue;
		}
		//�ȶշ��ְ�, ���ڵ� �ٸ��ٸ� 
		if (ma[i][j].num != ma[i][j + 1].num) {
			//��ǥ �� ã��
			tem = ma[i][j + 1].num;
			nu = min(ma[i][j].num, ma[i][j + 1].num);
			//��ǥ���� ���� ��� �͵� ����ȭ
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
		if (m[i - 1][start].num == target) //������ġã��
			break;
	}
	int t, diff = 0, random, nu;
	for (t = start; t <= j; t++) {//���ۺ��� ������ �� �ٸ��� �˻�(��� �����ִ���)
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
	if (diff == 1) {//�� �����ִٸ�, �ٽ� random���� ������ ���� ����
		for (t = start; t <= j; t++) {
			if (m[i - 1][t].num == target) {
				random = rand() % 2;
				m[i][t].up = random;
				m[i - 1][t].down = random;
				if (random == 1) {
					nu = min(m[i][t].num, m[i - 1][t].num);
					m[i][t].num = nu;
					m[i - 1][t].num = nu;
					rands = 1;  //���Ʒ� �ձ� ����
					//printf("\n%d %d %d\n", nu, m[i][t].num, m[i - 1][t].num);
				}
			}
		}
		//�ϳ��� �ȶշȴٸ� �ٽ� �˻����
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

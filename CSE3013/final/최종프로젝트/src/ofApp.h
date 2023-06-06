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
	int path_w = 22;  //�̷� ���� �ʺ�. 
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
	void freeMemory();   //��� �޸� ���� �� �������� �ʱ�ȭ
	bool DFS();   //DFS?
	void dfsdraw();   //���ð����� ��α׸���
	void bfsdraw();   //B_visit������ ��� �׸���
	int HEIGHT;//�̷��� ����
	int WIDTH;//�̷��� �ʺ�
	char** input;//�ؽ�Ʈ ������ ��� ������ ��� ������ �迭�̴�.
	int** visited;//�湮���θ� ������ ����
	int maze_col;//�̷�ĭ�� ���� �ε����� ����Ų��.
	int maze_row;//�̷�ĭ�� ���� �ε����� ����Ų��.
	int k;
	int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
	int isdfs;//DFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.
	int isbfs;//BFS�Լ��� ������״��� �Ǵ��ϴ� ����. 0�̸� ������߰� 1�̸� �����ߴ�.
	// Menu
	ofxWinMenu* menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function
	typedef struct ma { //�̷� �迭 ���ҵ�
		int num;
		int up, down, left, right;
	}M;
	M** maze; //�̷�
	typedef struct node { //ť, ���� ���
		int num;
		struct node* next;
	}N;
	//dfs���� ������ �������� ����ô ���������� bfs�� ť�� �������� �Ұ���
	//��� visit�迭�� �������ϱ� ���� ���ο� B_visit�� ����
	typedef struct ar {   //BFS visit�� ����ü
		int num;
		int from; //�������� ���� from.
	}ar;
	N* stack;   //���� ����
	N* Q_h, * Q_t;  //ť ���, ����
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
	bool bFullscreen;  //Ǯ��ũ���� ����
	bool bTopmost;   //��������� ����� ����
	//bool isdfs;
	// Example functions
	void doFullScreen(bool bFull); //Ǯ��ũ��
	void doTopmost(bool bTop); //���������

	int target_c;    //������ target y,x��ǥ
	int target_r;
	//dfs���ÿ�
	int df(int);  //DFS
	void s_push(int);  //���� in
	void s_pop();  //���� out
	bool s_empty();  //���� �������
//bfsť��
	bool BFS();  //BFS
	int Q_empty();  //ť ��
	void Q_delete(); //ť out
	void Q_add(int); //ť in
//pj. 11���� �� �״�� �̿�
	void make_maze();     //�̷� �����
	void final_make(M**);    //�̷��� ��������
	void checking_line(int, int, M**);  //�̷��� �ึ�� ������ ����
	void make_col(int, int, M**);    //�̷��� ���� ������ ����

	//���Ӻ���, �Լ�
	int key_gaming = 0;    //Ű ���ӽ� 1, �׿� 0
	int mouse_gaming = 0;   //���콺 ���ӽ� 1, �׿� 0
	int now_c;    //Ű���ӿ� ���� ��ġ �ľ� ����
	int now_r;
	int breaking = 0;   //Ű���ӿ� �� �μ����� �Ҷ� 1, �׿� 0
	int quitg = 0; //0�̸� ��������, �� ���Ӿ���. 1�϶��� ������
	void end_g();   //Ű���ӿ� �����϶����� target�� �����ߴ��� �ľ��ϴ� �Լ�. ���� �����ϸ� succ, quit����
	int moving = 0;  //�󸶳� ���������� �ľ�
	int br = 0;   //�󸶳� ���� �ν����� �ľ�
	int succ = 0;  //���� ������ 1, �׿� 0
	clock_t start, u;   //start�� ��� �ٲ�� �ð�, u�� �ʱ� start�ǽð���ȭ �ð� ����(���� �ð� �����)
	double fi = 0;   //���ӽð� ��¿� (start-u)�� ��� ��ȭ�ϴ� ����
	int inm = 0;   //���콺 ���ӿ� ����. 0�̸� ���ӽ��� �Ұ���(���� ��ġ���� �̵� �ʼ�), 1�̸� ������ҿ� �Ա⿡ �����غ�, 2�� ������ ���ӽ���
	void draw_gamebox();  //���콺���ӿ�, ���� �ڽ� �׸���
	bool touch_wall(int, int);  //���콺 ���ӿ�. Ŀ���� ���� ��ų� �̻��� ��ġ�� �ִٸ� false, �׿� true 
};

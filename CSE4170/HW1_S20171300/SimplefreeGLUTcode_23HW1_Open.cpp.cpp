#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#define TO_RADIAN		0.017453292519943296 

int rightbuttonpressed = 0;
int leftbuttonpressed = 0, center_selected = 0;
void reshape(int width, int height);
float r, g, b; // Background color
//선분 좌표.
float px, py, qx, qy; // Line (px, py) --> (qx, qy)
//n각형, 각 꼭짓점 좌표, 무게중심
int n_object_points = 7;
float object[7][2], object_center_x, object_center_y;
float rotation_angle_in_degree;
int window_width, window_height;
int shift_on = 0;
int alt_on = 0;
int ctrl_on = 0;
float rec_5[5][2];
int win_z = 0;
int win_x = 0;
typedef struct point {
	double x;
	double y;
}P;
void initial_5rec() {
	rec_5[0][0] = -0.1f;
	rec_5[0][1] = -0.1f;
	rec_5[1][0] = 0.1f;
	rec_5[1][1] = 0.1f;
	rec_5[2][0] = 0.2f;
	rec_5[2][1] = -0.0f;
	//아래의 좌표가 튀어나온(꼭짓점?)
	rec_5[3][0] = 0.3f;
	rec_5[3][1] = -0.3f;
	rec_5[4][0] = 0.0f;
	rec_5[4][1] = -0.2f;
}
P matrix_multi(double  matrix[][3], double x, double y) {
	int i, j, k;
	double result[3][1] = {0};
	double m2[3][1] = { {x},{y},{1} };
	/*for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			fprintf(stdout, "%lf //  ", matrix[i][j]);
		}
		fprintf(stdout, "\n");
	}*/
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 1; j++) {
			for (k = 0; k < 3; k++) {
				result[i][j] += matrix[i][k] * m2[k][j];
			}
			//fprintf(stdout, "%lf \n  ", result[i][j]);
		}
	}
	P po;
	po.x = result[0][0];
	po.y = result[1][0];
	return po;
}
// 750,750 으로 바꿔놨을때니깐, 4사분면 에서 무게중심 먼저 랜덤으로 찾아주고(오차 계산해서 대충)
//너무 복잡함. 나중에 
/*class triangle {
private:
	double one[2];
	double two[2];
	double three[2];
	double speedx;
	double speedy;
	double center_x;
	double center_y;
public:
	triangle() {
		center_x = (double)(rand() % 350 + 10) / 250.0;
		center_y= (double)(rand() % 350 + 10) / -250.0;
		one[0] = center_x;

	}
};*/
void draw_axes() {
	double left_d_x;
	double left_d_y;
	double right_u_x;
	double right_u_y;
	left_d_x=  (- (double)window_width / 2) / 250.0;
	left_d_y = ( -(double)window_height / 2) / 250.0;
	right_u_x = ((double)window_width / 2) / 250.0;
	right_u_y = ((double)window_height / 2) / 250.0;
	glLineWidth(3.0f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.975f, 0.025f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(0.975f, -0.025f);
	glVertex2f(1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(0.025f, 0.975f);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(-0.025f, 0.975f);
	glVertex2f(0.0f, 1.0f);
	glLineWidth(1.0f);
	glColor3f(0.7f, 0.3f, 0.5f);
	//대각선
	glVertex2f(left_d_x,left_d_y);
	glVertex2f(right_u_x, right_u_y);
	glEnd();
	glLineWidth(1.0f);

	glPointSize(7.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPointSize(1.0f);
}
//선분 그리는 함수
void draw_line(float px, float py, float qx, float qy) {
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(px, py);
	glVertex2f(qx, qy);
	glEnd();
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(px, py);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(qx, qy);
	glEnd();
	glPointSize(1.0f);
}
//다각형 그리김
void draw_object(void) {
	glBegin(GL_LINE_LOOP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], object[i][1]);
	glEnd();
	if (win_z == 1) {
		//printf("!!!!!!!!!!!!!!!!SFDgadkfjhgtapsoidhjftapoishdnfak");
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < 5; i++)
			glVertex2f(rec_5[i][0], rec_5[i][1]);
		glEnd();
	}
	//z와 x키 동시에 눌렀을때..
	if (win_x == 1) {
		int tt;
		double transform_matrix[3][3] = { {-1,0,0} ,{0,1,0}, {0,0,1} };
		double rotation_matrix[3][3];
		double c1 = cos(45 * M_PI / (double)180);
		double s1 = sin(45 * M_PI / (double)180);
		P s_p;
		rotation_matrix[0][0] = c1;
		rotation_matrix[0][1] = -s1;
		rotation_matrix[0][2] = 0;
		rotation_matrix[1][0] = s1;
		rotation_matrix[1][1] = c1;
		rotation_matrix[1][2] = 0;
		rotation_matrix[2][0] = 0;
		rotation_matrix[2][1] = 0;
		rotation_matrix[2][2] = 1;
		double rotation_matrix2[3][3] = { {c1,s1,0},{-s1,c1,0},{0,0,1} };
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 1.0f, 0.7f);
		for (tt = 0; tt < 5; tt++) {
			s_p = matrix_multi(rotation_matrix, rec_5[tt][0], rec_5[tt][1]);
			s_p = matrix_multi(transform_matrix, s_p.x, s_p.y);
			s_p = matrix_multi(rotation_matrix2, s_p.x, s_p.y);
			glVertex2f(s_p.x, s_p.y);
		}
		glEnd();
	}
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 6; i++)
		glVertex2f(object[i][0], object[i][1]);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(object_center_x, object_center_y);
	glEnd();
	glPointSize(1.0f);
}

void display(void) {
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_axes();
	draw_line(px, py, qx, qy);
	draw_object();
	glFlush();
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case 'r':
		r = 1.0f; g = b = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		fprintf(stdout, "연속?");
		break;
	case 'g':
		g = 1.0f; r = b = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 'b':
		b = 1.0f; r = g = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 's':
		r = 250.0f / 255.0f, g = 128.0f / 255.0f, b = 114.0f / 255.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case 'z':
		//glutFullScreen();
		if (win_z==0) {
			glutReshapeWindow(750, 750);
			win_z = 1;
			//printf("win_z initialed\n");
			//5각형 초기화
			initial_5rec();
			glutPostRedisplay();
		}
		else {

		}
		break;
	case 'x':
		if (win_z == 1) {
			win_x = 1;
			glutPostRedisplay();
		}
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	//printf("%d", win_z);
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		r -= 0.1f;
		if (r < 0.0f) r = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		r += 0.1f;
		if (r > 1.0f) r = 1.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		g -= 0.1f;
		if (g < 0.0f) g = 0.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		g += 0.1f;
		if (g > 1.0f) g = 1.0f;
		fprintf(stdout, "$$$ The new window background color is (%5.3f, %5.3f, %5.3f).\n", r, g, b);
		glutPostRedisplay();
		break;
	}
	
}

int prevx, prevy;
// 회전 속도 구현하고 싶으면 전역변수로 radius double radius = 0;
int rotate = 0;
int z_she = 0;
double at_alt_po_x, at_alt_po_y;
//굳이opengl좌표 인식 필요없이 그냥 좌우 움직임만 알면 되니, window좌표계 사용
int at_ctrl_po_x;
void mousepress(int button, int state, int x, int y) {
	double transform_matrix[3][3] = { {1,0,0} ,{0,1,0}, {0,0,1} };
	double rotation_matrix[3][3];
	double she_matrix[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	double radius = 0;
	double go = 0;
	int specialKey = glutGetModifiers();
	if(button==3){
		if (win_z == 1) {
			go += 0.1;
			//스크롤 한번당 2번씩 실행되기에, 2되면 shearing 실행하게 코딩
			z_she ++;
		}
		else {
			radius += 0.2;
			rotate = 1;
		}
		//printf("업");
	}
	else if (button == 4) {
		if (win_z == 1) {
			go -= 0.5;
			z_she ++;
		}
		else {
			radius -= 0.5;
			rotate = 1;
		}
		//printf("다운");
	}
	//z누르고 휠 기능 shearing
	//한번 스크롤당 2번씩 실행이 되버림.  그래서 그냥 2일때 실행이 되게 설정. 각 환경에 따라 다를수 있으니 설정 필수.
	if (z_she == 2) {

		//printf("1\n");
		int tt = 0;
		
		double c1 = cos(45 * M_PI / (double)180);
		double s1 = sin(45 * M_PI / (double)180);
		P s_p;
		rotation_matrix[0][0] = c1;
		rotation_matrix[0][1] = -s1;
		rotation_matrix[0][2] = 0;
		rotation_matrix[1][0] = s1;
		rotation_matrix[1][1] = c1;
		rotation_matrix[1][2] = 0;
		rotation_matrix[2][0] = 0;
		rotation_matrix[2][1] = 0;
		rotation_matrix[2][2] = 1;
		double rotation_matrix2[3][3] = { {c1,s1,0},{-s1,c1,0},{0,0,1} };
		she_matrix[1][0] = go;
		for (tt = 0; tt < 5; tt++) {
			s_p = matrix_multi(rotation_matrix, rec_5[tt][0], rec_5[tt][1]);
			s_p=matrix_multi(she_matrix, s_p.x,s_p.y);
			s_p = matrix_multi(rotation_matrix2, s_p.x, s_p.y);
			rec_5[tt][0] = s_p.x;
			rec_5[tt][1] = s_p.y;


		}
		glutPostRedisplay();
		z_she = 0;
	}
	//단순 회전
	if (rotate == 1) {
		P r_p;
		transform_matrix[0][2] = -px;
		transform_matrix[1][2] = -py;
		r_p = matrix_multi(transform_matrix, qx, qy);
		//fprintf(stdout, "%lf  %lf", r_p.x, r_p.y);

		double c1 = cos(radius * M_PI / (double)180);
		double s1 = sin(radius * M_PI / (double)180);
		//printf("%lf", c1);
		rotation_matrix[0][0] = c1;
		rotation_matrix[0][1] = -s1;
		rotation_matrix[0][2] = 0;
		rotation_matrix[1][0] = s1;
		rotation_matrix[1][1] = c1;
		rotation_matrix[1][2] = 0;
		rotation_matrix[2][0] = 0;
		rotation_matrix[2][1] = 0;
		rotation_matrix[2][2] = 1;
		r_p = matrix_multi(rotation_matrix, r_p.x, r_p.y);
		transform_matrix[0][2] = px;
		transform_matrix[1][2] = py;
		r_p = matrix_multi(transform_matrix, r_p.x, r_p.y);
		qx = r_p.x;
		qy = r_p.y;
		rotate = 1;
		glutPostRedisplay();
		rotate = 0;
	}
	if(state == GLUT_DOWN) {
		//좌표 변환.
		double real_y;
		double real_x;
		//printf("\nx,y: %d %d\n", x, y);
		//printf("cc : %lf %lf\n",  ((double)x - (double)window_width / 2), ((double)window_height / 2 - (double)y));
		real_y = ((double)window_height / 2 - (double)y)/250.0;
		real_x = ((double)x - (double)window_width / 2) / 250.0;
		if((state==GLUT_DOWN)&&(button == GLUT_LEFT_BUTTON) && (specialKey==GLUT_ACTIVE_SHIFT)) {
			//printf("%lf, %lf",real_x,real_y);
			if (real_x >= px - 0.01f && real_x <= px + 0.01f && real_y>=py-0.01f && real_y <=py+0.01f) {
				//fprintf(stdout, "쉬프트와 레프트 그리고 파란 네모 근처");
				shift_on = 1;
			}
		}
		if((button == GLUT_RIGHT_BUTTON) && (specialKey == GLUT_ACTIVE_CTRL)) {
			at_ctrl_po_x = x;
			ctrl_on = 1;
		}
		if((button == GLUT_RIGHT_BUTTON) && (specialKey == GLUT_ACTIVE_ALT)) {
			//fprintf(stdout, "알트와 레프트 다각형의 움직임 총 7각형, 7개의 점 이동");
			at_alt_po_x = real_x;
			at_alt_po_y = real_y;
			alt_on = 1;
		}
	}
	if (state == GLUT_UP) {
		//fprintf(stdout, "mouse off");
		alt_on = 0;
		ctrl_on = 0;
		shift_on = 0;
	}
}

void mousemove(int x, int y) {
	double real_y;
	double real_x;
	int i;
	int specialKey = glutGetModifiers();
	P qq;
	double size;
	double transform_matrix[3][3] = { {1,0,0} ,{0,1,0}, {0,0,1} };
	double transform_matrix_2[3][3] = { {1,0,0} ,{0,1,0}, {0,0,1} };
	double scaling_matrix[3][3] = { {0,0,0},{0,0,0},{0,0,1} };
	if (shift_on == 1 &&specialKey==GLUT_ACTIVE_SHIFT) {
		real_y = ((double)window_height / 2 - (double)y) / 250.0;
		real_x = ((double)x - (double)window_width / 2) / 250.0;
		px = real_x;
		py = real_y;
		glutPostRedisplay();
	}
	else if (alt_on == 1 && specialKey == GLUT_ACTIVE_ALT) {
		//printf("알트 인식중		");
		double move_x, move_y;
		real_y = ((double)window_height / 2 - (double)y) / 250.0;
		real_x = ((double)x - (double)window_width / 2) / 250.0;
		move_x =  real_x- at_alt_po_x;
		move_y =  real_y- at_alt_po_y;
		at_alt_po_x = real_x;
		at_alt_po_y = real_y;
		//printf("이동 %lf %lf\n", move_x, move_y);
		transform_matrix[0][2] = move_x;
		transform_matrix[1][2] = move_y;
		object_center_x = object_center_y = 0.0f;
		for (i = 0; i < 7; i++) {
			qq = matrix_multi(transform_matrix, object[i][0], object[i][1]);
			object[i][0] = qq.x;
			object[i][1] = qq.y;
			object_center_x += object[i][0];
			object_center_y += object[i][1];
		}
		object_center_x /= n_object_points;
		object_center_y /= n_object_points;
		glutPostRedisplay();
	}
	else if (ctrl_on == 1 && specialKey == GLUT_ACTIVE_CTRL) {
		if (x < at_ctrl_po_x)//마우스 왼쪽이동
		{
			size = (at_ctrl_po_x - x)/250.0;
			at_ctrl_po_x = x;
			scaling_matrix[0][0] = 1.0-size;
			scaling_matrix[1][1] = 1.0-size;
			transform_matrix[0][2] = -object_center_x;
			transform_matrix[1][2] = -object_center_y;
			transform_matrix_2[0][2] = object_center_x;
			transform_matrix_2[1][2] = object_center_y;
			for (i = 0; i < 7; i++) {
				qq = matrix_multi(transform_matrix, object[i][0], object[i][1]);
				qq= matrix_multi(scaling_matrix, qq.x, qq.y);
				qq= matrix_multi(transform_matrix_2, qq.x, qq.y);
				object[i][0] = qq.x;
				object[i][1] = qq.y;
			}
			glutPostRedisplay();
		}
		else if (x > at_ctrl_po_x) {
			size = ( x- at_ctrl_po_x) / 250.0;
			at_ctrl_po_x = x;
			scaling_matrix[0][0] = 1.0 + size;
			scaling_matrix[1][1] = 1.0 + size;
			transform_matrix[0][2] = -object_center_x;
			transform_matrix[1][2] = -object_center_y;
			transform_matrix_2[0][2] = object_center_x;
			transform_matrix_2[1][2] = object_center_y;
			for (i = 0; i < 7; i++) {
				qq = matrix_multi(transform_matrix, object[i][0], object[i][1]);
				qq = matrix_multi(scaling_matrix, qq.x, qq.y);
				qq = matrix_multi(transform_matrix_2, qq.x, qq.y);
				object[i][0] = qq.x;
				object[i][1] = qq.y;
			}
			glutPostRedisplay();
		}
	}
	else {//특수키 땠을때의 경우 실시간 감지용.
		alt_on = 0;
		shift_on = 0;
		ctrl_on = 0;
	}
}

void reshape(int width, int height) {
	// DO NOT MODIFY THIS FUNCTION!!!
	window_width = width, window_height = height;
	glViewport(0.0f, 0.0f, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-window_width / 500.0f, window_width / 500.0f, -window_height / 500.0f, window_height / 500.0f, -1.0f, 1.0f);

	glutPostRedisplay();
}
//키보드 업 감지 함수
void KeyUp(unsigned char key, int x, int y) {
	//z키에서 손 때면 즉시 사라짐
	if (key=='z') {
		win_z = 0;
		glutPostRedisplay();
	}
	else if (key == 'x') {
		win_x = 0;
		glutPostRedisplay();
	}

}

void close(void) {
	fprintf(stdout, "\n^^^ The control is at the close callback function now.\n\n");
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mousepress);
	glutMotionFunc(mousemove);
	glutReshapeFunc(reshape);
	glutCloseFunc(close);
	glutKeyboardUpFunc(KeyUp);
}

void initialize_renderer(void) {
	register_callbacks();
	r = 250.0f / 255.0f, g = 128.0f / 255.0f, b = 114.0f / 255.0f; // Background color = Salmon
	//선분 초기화 좌표
	px = -0.1f, py = 1.0f, qx = 1.0f, qy = 1.0f;
	rotation_angle_in_degree = 1.0f; // 1 degree
	//다각형 좌표.
	float sq_cx = -0.3f, sq_cy = -0.3f, sq_side = 0.2f;
	object[0][0] = sq_cx + sq_side;
	object[0][1] = sq_cy + sq_side;
	object[1][0] = sq_cx + 0.3 * sq_side;
	object[1][1] = sq_cy + 0.3 * sq_side;
	object[2][0] = sq_cx - sq_side;
	object[2][1] = sq_cy + sq_side;
	object[3][0] = sq_cx - sq_side;
	object[3][1] = sq_cy - sq_side;
	object[4][0] = sq_cx - 0.3 * sq_side;
	object[4][1] = sq_cy - 0.4 * sq_side;
	object[5][0] = sq_cx + 0.3*sq_side;
	object[5][1] = sq_cy - 0.4*sq_side;
	object[6][0] = sq_cx + sq_side;
	object[6][1] = sq_cy - sq_side;
	object_center_x = object_center_y = 0.0f;
	for (int i = 0; i < n_object_points; i++) {
		object_center_x += object[i][0];
		object_center_y += object[i][1];
	}
	object_center_x /= n_object_points;
	object_center_y /= n_object_points;
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = TRUE;
	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 4
void main(int argc, char* argv[]) {
	srand(time(NULL));
	char program_name[64] = "Sogang CSE4170 Simple 2D Transformations";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'r', 'g', 'b', 's', 'q'",
		"    - Special keys used: LEFT, RIGHT, UP, DOWN",
		"    - Mouse used: SHIFT/L-click and move, ALT/R-click and move, CTRL/R-click and move",
		"    - Wheel used: up and down scroll"
		"    - Other operations: window size change"
	};

	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE); // <-- Be sure to use this profile for this example code!
	//	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA);

	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 200);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	// glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT); // default
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMainLoop();
	fprintf(stdout, "^^^ The control is at the end of main function now.\n\n");
}
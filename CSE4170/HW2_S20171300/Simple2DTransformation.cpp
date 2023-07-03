#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>
#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0; 
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;
float mousex=0.0f, mousey=0.0f;
float click_mousex = 0.0f, click_mousey = 0.0f;
GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

int leftbuttonpressed = 0;
int animation_mode = 1;
unsigned int timestamp = 0;

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	glutPostRedisplay();
	if (animation_mode)
		glutTimerFunc(10, timer, 0);
}
void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.5f; axes[1][0] = win_width / 2.5f; 
	axes[2][1] = -win_height / 2.5f;
	axes[3][1] = win_height / 2.5f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

GLfloat line[2][2];
GLfloat line_color[3] = { 1.0f, 0.0f, 0.0f };
GLuint VBO_line, VAO_line;

void prepare_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height; 
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f; 
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height; 
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f)*win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f; 
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_line(void) { // Draw line in its MC.
	// y = x - win_height/4
	glUniform3fv(loc_primitive_color, 1, line_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

//star!!
#define STAR 0
GLfloat star[10][2] = { { -0.5,0.0 },{ 0.0, sqrt(3)},{0.5, 0.0},{3.0, 0.0},{1.0, -1.0},{1.5, -2.5},{0.0, -1.7},{-1.5, -2.5},{-1.0, -1.0}, { -3.0, 0.0 } };
GLuint VBO_star, VAO_star;
GLfloat star_color[3] = { 255 / 255.0f, 255 / 255.0f, 0 / 255.0f };
void prepare_star() {
	GLsizeiptr buffer_size = sizeof(star);
	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_star);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_star);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(star), star);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_star);
	glBindVertexArray(VAO_star);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_star);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void draw_star() {
	glBindVertexArray(VAO_star);

	glUniform3fv(loc_primitive_color, 1, star_color);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 10);


	glBindVertexArray(0);
}

//steve!!

#define STEVE_FACE 0
#define S_EYE 1
#define S_EYE_IN 2
#define S_MOUSE 3
#define S_HAIR 4


GLfloat steve_face[8][2] = { { -3.0, 0.0 },{ -3.0, 1.0 },{ 3.0, 1.0 },{ 3.0, 0.0 },{ 4.0, 0.0 },{ 4.0, -5.0 },{ -4.0, -5.0 },{ -4.0, 0.0 } };
GLfloat setve_lefteye[4][2] = { { -3.0, -1.0 },{ -3.0, -2.0 } ,{ -2.0, -2.0 },{ -2.0, -1.0 } };
GLfloat setve_righteye[4][2] = { { 3.0, -1.0 },{ 3.0, -2.0 } ,{ 2.0, -2.0 },{ 2.0, -1.0 } };
GLfloat steve_lefteyein[4][2] = { { -2.0, -1.0 },{ -2.0, -2.0 } ,{ -1.0, -2.0 },{ -1.0, -1.0 } };
GLfloat steve_righteyein[4][2] = { { 2.0, -1.0 },{ 2.0, -2.0 } ,{ 1.0, -2.0 },{ 1.0, -1.0 } };
GLfloat steve_mouse[8][2] = { { -1.0, -3.0 } ,{ -1.0, -2.0 },{ 1.0, -2.0 },{ 1.0, -3.0 },{ 2.0, -3.0 },{ 2.0, -4.0 },{ -2.0, -4.0 },{ -2.0, -3.0 } };
GLfloat steve_hair[4][2] = { { 4.0, 3.0 } ,{ 4.0, 1.0 } ,{ -4.0, 1.0 },{ -4.0, 3.0 } };
GLfloat steve_hair1[4][2] = { { 4.0, 0.0 } ,{ 3.0, 0.0 }, { 3.0, 1.0 }, { 4.0, 1.0 } };
GLfloat steve_hair2[4][2] = { { -3.0, 1.0 },{ -3.0, 0.0 },{ -4.0, 0.0 },{ -4.0, 1.0 } };


GLfloat steve_color[5][3] = {
	{ 188 / 255.0f, 144 / 255.0f, 117 / 255.0f },
{ 255 / 255.0f, 255 / 255.0f, 255 / 255.0f }, //흰색
{ 89 / 255.0f, 63 / 255.0f, 142 / 255.0f }, //눈동자
{ 110 / 255.0f, 63 / 255.0f, 49 / 255.0f },//여기까지함,입색
{ 58 / 255.0f, 40 / 255.0f, 21 / 255.0f }
};

GLuint VBO_steve, VAO_steve;

void prepare_steve() {
	GLsizeiptr buffer_size = sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein) + sizeof(steve_righteyein) + sizeof(steve_mouse) + sizeof(steve_hair) + sizeof(steve_hair1) + sizeof(steve_hair2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_steve);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_steve);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(steve_face), steve_face);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face), sizeof(setve_lefteye), setve_lefteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye), sizeof(setve_righteye), setve_righteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye), sizeof(steve_lefteyein), steve_lefteyein);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein), sizeof(steve_righteyein), steve_righteyein);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein) + sizeof(steve_righteyein), sizeof(steve_mouse), steve_mouse);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein) + sizeof(steve_righteyein) + sizeof(steve_mouse), sizeof(steve_hair), steve_hair);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein) + sizeof(steve_righteyein) + sizeof(steve_mouse) + sizeof(steve_hair), sizeof(steve_hair1), steve_hair1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(steve_face) + sizeof(setve_lefteye) + sizeof(setve_righteye) + sizeof(steve_lefteyein) + sizeof(steve_righteyein) + sizeof(steve_mouse) + sizeof(steve_hair) + sizeof(steve_hair1), sizeof(steve_hair2), steve_hair2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_steve);
	glBindVertexArray(VAO_steve);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_steve);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_steve() {
	glBindVertexArray(VAO_steve);

	glUniform3fv(loc_primitive_color, 1, steve_color[STEVE_FACE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_EYE]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_EYE_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_EYE_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_MOUSE]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, steve_color[S_HAIR]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
	glUniform3fv(loc_primitive_color, 1, steve_color[S_HAIR]);
	glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
	glUniform3fv(loc_primitive_color, 1, steve_color[S_HAIR]);
	glDrawArrays(GL_TRIANGLE_FAN, 40, 4);
	glBindVertexArray(0);
}
//creper!!
#define BLACKs 0
#define DEEPGREEN 1
#define CRE_HEAD 2
GLfloat cre_face[4][2] = { { -4.0, 4.0 },{ 4.0, 4.0 },{ 4.0, -4.0 },{ -4.0, -4.0 } };
GLfloat cre_lefteye[4][2] = { { -1.0, 0.0 },{ -3.0, 0.0 } ,{ -3.0, 2.0 },{ -1.0, 2.0 } };
GLfloat cre_righteye[4][2] = { { 1.0, 0.0 },{ 3.0, 0.0 } ,{ 3.0, 2.0 },{ 1.0, 2.0 } };
GLfloat cre_lefteyein[4][2] = { { -1.0, 0.0 },{ -2.0, 0.0 } ,{ -2.0, 1.0 },{ -1.0, 1.0 } };
GLfloat cre_righteyein[4][2] = { { 1.0, 0.0 },{ 2.0, 0.0 } ,{ 2.0, 1.0 },{ 1.0, 1.0 } };
GLfloat cre_mouse[8][2] = { { -1.0, -2.0 } ,{ -1.0, -1.0 },{ 1.0, -1.0 },{ 1.0, -2.0 },{ 2.0, -2.0 },{ 2.0, -3.0 },{ -2.0, -3.0 },{ -2.0, -2.0 } };
GLfloat cre_mouse1[4][2] = { { -1.0, -2.0 } ,{ -2.0, -2.0 },{ -2.0, -1.0 },{ -1.0, -1.0 } };
GLfloat cre_mouse2[4][2] = { { -1.0, -3.0 } ,{ -1.0, -4.0 },{ -2.0, -4.0 },{ -2.0, -3.0 } };
GLfloat cre_mouse3[4][2] = { { 1.0, -3.0 } ,{ 1.0, -4.0 },{ 2.0, -4.0 },{ 2.0, -3.0 } };
GLfloat cre_mouse4[4][2] = { { 1.0, -2.0 } ,{ 2.0, -2.0 },{ 2.0, -1.0 },{ 1.0, -1.0 } };
GLfloat cre_mouse5[4][2] = { { -1.0, 00.0 } ,{ -1.0, -1.0 },{ 1.0, -1.0 },{ 1.0, 0.0 } };


GLfloat cre_color[3][3] = {
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 56 / 255.0f, 86 / 255.0f, 51 / 255.0f }, //딥그린
{ 68 / 255.0f, 182 / 255.0f, 59 / 255.0f }, //피부색
};

GLuint VBO_cre, VAO_cre;

void prepare_cre() {
	GLsizeiptr buffer_size = sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse)
		+ sizeof(cre_mouse1) + sizeof(cre_mouse2) + sizeof(cre_mouse3) + sizeof(cre_mouse4) + sizeof(cre_mouse5);// +sizeof(steve_hair) + sizeof(steve_hair1) + sizeof(steve_hair2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cre);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cre);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cre_face), cre_face);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face), sizeof(cre_lefteye), cre_lefteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye), sizeof(cre_righteye), cre_righteye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye), sizeof(cre_lefteyein), cre_lefteyein);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein), sizeof(cre_righteyein), cre_righteyein);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein), sizeof(cre_mouse), cre_mouse);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse), sizeof(cre_mouse1), cre_mouse1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse) + sizeof(cre_mouse1), sizeof(cre_mouse2), cre_mouse2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse) + sizeof(cre_mouse1) + sizeof(cre_mouse2), sizeof(cre_mouse3), cre_mouse3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse) + sizeof(cre_mouse1) + sizeof(cre_mouse2), sizeof(cre_mouse3), cre_mouse3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse) + sizeof(cre_mouse1) + sizeof(cre_mouse2) + sizeof(cre_mouse3), sizeof(cre_mouse4), cre_mouse4);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cre_face) + sizeof(cre_lefteye) + sizeof(cre_righteye) + sizeof(cre_lefteyein) + sizeof(cre_righteyein) + sizeof(cre_mouse) + sizeof(cre_mouse1) + sizeof(cre_mouse2) + sizeof(cre_mouse3) + sizeof(cre_mouse4), sizeof(cre_mouse5), cre_mouse5);


	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cre);
	glBindVertexArray(VAO_cre);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cre);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cre() {
	glBindVertexArray(VAO_cre);

	glUniform3fv(loc_primitive_color, 1, cre_color[CRE_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cre_color[BLACKs]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cre_color[BLACKs]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cre_color[BLACKs]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);
	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 40, 4);
	glUniform3fv(loc_primitive_color, 1, cre_color[DEEPGREEN]);
	glDrawArrays(GL_TRIANGLE_FAN, 44, 4);
	glBindVertexArray(0);
}
//airplane!
#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 }, { -20.0, 15.0 }, { -20.0, 20.0 }, { 0.0, 23.0 }, { 20.0, 20.0 }, { 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 }, { -11.0, -12.0 }, { -12.0, -7.0 }, { 0.0, -10.0 }, { 12.0, -7.0 }, { 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 }, { -6.0, 0.0 }, { -6.0, 22.0 }, { 6.0, 22.0 }, { 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 }, { -7.0, 24.0 }, { -7.0, 21.0 }, { 7.0, 21.0 }, { 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 }, { -18.0, 3.0 }, { -16.0, 10.0 }, { -18.0, 20.0 }, { -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 }, { 18.0, 3.0 }, { 16.0, 10.0 }, { 18.0, 20.0 }, { 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2)+sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}


// sword!

#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sword() {
	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}

//house!
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//hios!!
GLfloat hios_out[6][2] = { { -0.6f, 0.8f },{ -1.0f, 0.0f},{ -0.6f, -0.8f },{ 0.6f, -0.8f },{1.0f, 0.0f },{ 0.6f, 0.8f } };
GLfloat hios_tri1[3][2] = { { -0.4f, 0.65f },{ -0.65f, 0.35f},{ 0.5f, 0.47f } };
GLfloat hios_tri2[3][2] = { { -0.2f, -0.65f },{ -0.7f, 0.3f},{ -0.6f, -0.6f } };
GLfloat hios_tri3[3][2] = { { 0.38f, 0.17f },{ 0.78f, 0.30f},{ 0.23f, -0.7f } };
GLfloat hios_circle[360][2];

GLfloat hios_color[3][3] = {
	{ 162 / 255.0f, 247 / 255.0f, 244 / 255.0f }, //연하
{ 68 / 255.0f, 203 / 255.0f, 255 / 255.0f }, //삼하
{ 1 / 255.0f, 144 / 255.0f, 240 / 255.0f }, //피부색
};
GLuint VBO_hios, VAO_hios;
void prepare_hios() {
	for (int i = 0; i < 360; i++) {
		hios_circle[i][0] = { 0.3f * cosf(i * TO_RADIAN) - 0.07f };
		hios_circle[i][1] = { 0.3f * sinf(i * TO_RADIAN) - 0.05f };
	}
	GLsizeiptr buffer_size = sizeof(hios_out) + sizeof(hios_tri1) + sizeof(hios_tri2) + sizeof(hios_tri3) + sizeof(hios_circle);// +sizeof(steve_righteyein) + sizeof(steve_mouse) + sizeof(steve_hair) + sizeof(steve_hair1) + sizeof(steve_hair2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_hios);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hios);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(hios_out), hios_out);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hios_out), sizeof(hios_tri1), hios_tri1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hios_out) + sizeof(hios_tri1), sizeof(hios_tri2), hios_tri2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hios_out) + sizeof(hios_tri1) + sizeof(hios_tri2), sizeof(hios_tri3), hios_tri3);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(hios_out) + sizeof(hios_tri1) + sizeof(hios_tri2) + sizeof(hios_tri3), sizeof(hios_circle), hios_circle);
	glGenVertexArrays(1, &VAO_hios);
	glBindVertexArray(VAO_hios);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hios);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_hios() {
	glBindVertexArray(VAO_hios);

	glUniform3fv(loc_primitive_color, 1, hios_color[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, hios_color[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 3);

	glUniform3fv(loc_primitive_color, 1, hios_color[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);

	glUniform3fv(loc_primitive_color, 1, hios_color[1]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, hios_color[2]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 360);
	glBindVertexArray(0);
}
//car!
#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

GLfloat car2_body[8][2] = { { -18.0, -7.0 },{ -18.0, 0.0 },{ -13.0, 0.0 },{ -10.0, 8.0 },{ 10.0, 8.0 },{ 13.0, 0.0 },{ 18.0, 0.0 },{ 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 },{ -8.0, 6.0 },{ -2.0, 6.0 },{ -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 },{ 0.0, 6.0 },{ 8.0, 6.0 },{ 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 },{ -13.0, -8.0 },{ -13.0, -7.0 },{ -11.0, -4.0 },{ -7.0, -4.0 },{ -5.0, -7.0 },{ -5.0, -8.0 },{ -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 },{ 5.0, -8.0 },{ 5.0, -7.0 },{ 7.0, -4.0 },{ 11.0, -4.0 },{ 13.0, -7.0 },{ 13.0, -8.0 },{ 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 },{ -17.0, -2.0 },{ -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 },{ -17.0, -5.0 },{ -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1) + sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body) + sizeof(car2_front_window) + sizeof(car2_back_window) + sizeof(car2_front_wheel)
		+ sizeof(car2_back_wheel) + sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}
// cake!
#define CAKE_FIRE 0
#define CAKE_CANDLE 1
#define CAKE_BODY 2
#define CAKE_BOTTOM 3
#define CAKE_DECORATE 4

GLfloat cake_fire[4][2] = { { -0.5, 14.0 },{ -0.5, 13.0 },{ 0.5, 13.0 },{ 0.5, 14.0 } };
GLfloat cake_candle[4][2] = { { -1.0, 8.0 } ,{ -1.0, 13.0 },{ 1.0, 13.0 },{ 1.0, 8.0 } };
GLfloat cake_body[4][2] = { { 8.0, 5.0 },{ -8.0, 5.0 } ,{ -8.0, 8.0 },{ 8.0, 8.0 } };
GLfloat cake_bottom[4][2] = { { -10.0, 1.0 },{ -10.0, 5.0 },{ 10.0, 5.0 },{ 10.0, 1.0 } };
GLfloat cake_decorate[4][2] = { { -10.0, 0.0 },{ -10.0, 1.0 },{ 10.0, 1.0 },{ 10.0, 0.0 } };

GLfloat cake_color[5][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f }
};

GLuint VBO_cake, VAO_cake;

void prepare_cake() {
	int size = sizeof(cake_fire);
	GLsizeiptr buffer_size = sizeof(cake_fire) * 5;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, cake_fire);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, cake_candle);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, cake_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, cake_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, cake_decorate);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cake);
	glBindVertexArray(VAO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cake() {
	glBindVertexArray(VAO_cake);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_FIRE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_CANDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_DECORATE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindVertexArray(0);
}
float health = 80.0;
GLfloat health_bar[4][2] = { { 0.0,0.0 },{ 10.0,0.0 },{10.0, 0.5},{0.0, 0.5}};
GLuint VBO_hh, VAO_hh;
GLfloat health_color[3] = { 255 / 255.0f, 0 / 255.0f, 0 / 255.0f };
void prepare_h() {
	GLsizeiptr buffer_size = sizeof(health_bar);
	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_hh);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hh);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(health_bar), health_bar);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_hh);
	glBindVertexArray(VAO_hh);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_hh);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void draw_h() {
	glBindVertexArray(VAO_hh);

	glUniform3fv(loc_primitive_color, 1, health_color);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


	glBindVertexArray(0);
}

int shot_time=2;
int check_time;
int no_first_time = 1;
float left_sw_x;
float left_sw_y;
float l_sw_normx;
float l_sw_normy;
float right_sw_x;
float right_sw_y;
float r_sw_normx;
float r_sw_normy;
float car_normx;
float car_normy;

float leftu_sw_x;
float leftu_sw_y;
float rightu_sw_x;
float rightu_sw_y;
float lu_sw_normx;
float lu_sw_normy;
float ru_sw_normx;
float ru_sw_normy;
int upper_sw_clock;
int end_time;
int can = 0;
int no_fff_time = -5000;
int random;
float airx = 0.0f;
float airy = 0.0f;
int random_show;
float star_size_arr[10] = { 1.0f,2.1f,1.2f,2.3f,1.4f,2.5f,1.6f,2.7f,1.8f,2.9f };
float turn[10] = { 0.01f,0.01f,0.01f,0.01f,0.01f,0.01f,0.01f,0.01f,0.01f,0.01f };
float star_location[30][2];
float sw_size[4] = {1.0f,1.0f,1.0f,1.0f};
int dontouch = 0;
int donttouch = 0;
int just_one_event = 1;
float face_location[30] = {};
void display(void) {
	int i;
	float x, r, s, delx, delr, dels;
	glm::mat4 ModelMatrix;
	glClear(GL_COLOR_BUFFER_BIT);
	if (just_one_event == 1 && health <= 10.0f) {
		glClearColor(255 / 255.0f, 80 / 255.0f, 80 / 255.0f, 1.0f);
		just_one_event = 0;
	}
	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();
 	draw_line();


	
	//클릭시 위치 표시하는 집(회전집)
	ModelMatrix = glm::rotate(glm::mat4(1.0f), timestamp / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(27.0f, 27.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_hios();
	int house_clock = timestamp % 1080;

	if (leftbuttonpressed == 0) {

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, timestamp/3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();
	}
	else if (leftbuttonpressed == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, timestamp / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 70.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, timestamp / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(70.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, timestamp / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-70.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, timestamp / 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -70.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();
	}
	//움직이는 자동차 오차범위 추가해줌
	if (!((centerx >= click_mousex - 1.1f && centerx <= click_mousex + 1.1f) && (centery >= click_mousey - 1.1f && centery <= click_mousey + 1.1f))) {

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(centerx, centery, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, rotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car2();
		centerx += car_normx*2;
		centery += car_normy*2;
	}
	else {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(centerx, centery, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, rotate_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car2();
	}

	

	//700마다 아래칼발사
	int sword_clock = timestamp % 700;
	//현재 자동차 위치에 따라 칼 발사 650동안 발사 그려짐
	if (sword_clock==0 && sword_clock<=650 && no_first_time==1) {
		no_first_time = 0;
		shot_time *=-1 ;
		donttouch = 0;
		sw_size[0] = 1.0f;
		sw_size[1] = 1.0f;
		float nn;
		//0일시 좌측 상단 1일시 우측상단
		if (shot_time == -2) {
			left_sw_x = centerx + win_width / 2.1f;
			left_sw_y = centery + win_height / 2.1f;
			l_sw_normx = 1;
			l_sw_normy = (left_sw_y / left_sw_x);
			nn = sqrt(1 + l_sw_normy * l_sw_normy);
			l_sw_normx = l_sw_normx / nn;
			l_sw_normy = l_sw_normy / nn;
			//여기서 norm값 정의가 맞을듯. 발사즉시 속도(방향)도 결정되는 것이니깐.
			//printf("left_time");
		}
		else if (shot_time == 2) {
			right_sw_x = win_width / 2.1f - centerx;
			right_sw_y = centery + win_height / 2.1f;
			r_sw_normx = 1;
			r_sw_normy = (right_sw_y / right_sw_x);
			nn = sqrt(1 + r_sw_normy * r_sw_normy);
			r_sw_normx = r_sw_normx / nn;
			r_sw_normy = r_sw_normy / nn;
		}
		
	}
	//위의 칼
	upper_sw_clock = (timestamp % 700)-350;
	if (upper_sw_clock == -10) {
		can = 0;
		dontouch = 0;
	}
	if (upper_sw_clock==0 && can==0) {
		sw_size[2] = 1.0f;
		sw_size[3] = 1.0f;
		can = 1;
		random = rand() % 2;
		if (random == 0) {
			leftu_sw_x = centerx + win_width / 2.1f;
			leftu_sw_y = centery - win_height / 2.1f;
			float nn = sqrt(leftu_sw_x * leftu_sw_x + leftu_sw_y * leftu_sw_y);
			lu_sw_normx = leftu_sw_x / nn;
			lu_sw_normy = leftu_sw_y / nn;
		}
		if (random == 1) {
			rightu_sw_x = centerx - win_width / 2.1f;
			rightu_sw_y = centery- win_height / 2.1f;
			float nn = sqrt(rightu_sw_x * rightu_sw_x + rightu_sw_y * rightu_sw_y);
			ru_sw_normx = rightu_sw_x / nn;
			ru_sw_normy = rightu_sw_y / nn;
		}
	}
	if (can == 1 &&upper_sw_clock!=-10 && random==0) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-win_width / 2.1f + lu_sw_normx* 4*upper_sw_clock, win_height / 2.1f + lu_sw_normy * 4* upper_sw_clock, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, -atan2(leftu_sw_x, leftu_sw_y), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f*sw_size[2], 7.0f*sw_size[2], 1.0f));
		if (-win_width / 2.1f + lu_sw_normx * 4 * upper_sw_clock - 70.0f <= centerx && -win_width / 2.1f + lu_sw_normx * 4 * upper_sw_clock + 70.0f >= centerx &&
			win_height / 2.1f + lu_sw_normy * 4 * upper_sw_clock - 70.0f <= centery && win_height / 2.1f + lu_sw_normy * 4 * upper_sw_clock + 70.0f >= centery && dontouch == 0)
		{
			sw_size[2] = 0.0f;
			health -= 10.0f;
			//일시무적
			dontouch = 1;
		}
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		
		draw_sword();
	}
	else if (can == 1 && upper_sw_clock != -10 && random == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.1f + ru_sw_normx * 4 * upper_sw_clock, win_height / 2.1f + ru_sw_normy * 4 * upper_sw_clock, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, -atan2(rightu_sw_x, rightu_sw_y), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f*sw_size[3], 7.0f*sw_size[3], 1.0f));
		if (win_width / 2.1f + ru_sw_normx * 4 * upper_sw_clock - 70.0f <= centerx && win_width / 2.1f + ru_sw_normx * 4 * upper_sw_clock + 70.0f >= centerx &&
			win_height / 2.1f + ru_sw_normy * 4 * upper_sw_clock - 70.0f <= centery && win_height / 2.1f + ru_sw_normy * 4 * upper_sw_clock + 70.0f >= centery &&dontouch==0)
		{
			sw_size[3] = 0.0f;
			//일시무적
			health -= 10.0f;
			dontouch = 1;
		}
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_sword();
	}

	if (sword_clock <= 650 && shot_time==-2 && no_first_time==0) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-win_width / 2.1f+l_sw_normx*sword_clock*3, -win_height / 2.1f+ l_sw_normy*sword_clock*3, 0.0f));		
		ModelMatrix = glm::rotate(ModelMatrix, -atan2(left_sw_x, left_sw_y), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f*sw_size[0], 7.0f*sw_size[0], 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_sword();
		if (-win_width / 2.1f + l_sw_normx * sword_clock * 3 - 70.0f <= centerx && -win_width / 2.1f + l_sw_normx * sword_clock * 3 +70.0f >= centerx &&
			-win_height / 2.1f + l_sw_normy * sword_clock * 3 - 70.0f <= centery && -win_height / 2.1f + l_sw_normy * sword_clock * 3 + 70.0f >= centery && donttouch == 0)
		{
			sw_size[0] = 0.0f;
			//일시무적
			health -= 10.0f;
			donttouch = 1;
		}
	}
	else if (sword_clock <= 650 && shot_time == 2 && no_first_time == 0) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.1f - r_sw_normx * sword_clock * 3, -win_height / 2.1f + r_sw_normy * sword_clock * 3, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, atan2(right_sw_x, right_sw_y), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f*sw_size[1], 7.0f*sw_size[1], 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_sword();
		if (win_width / 2.1f - r_sw_normx * sword_clock * 3 - 70.0f <= centerx && win_width / 2.1f - r_sw_normx * sword_clock * 3 + 70.0f >= centerx &&
			-win_height / 2.1f + r_sw_normy * sword_clock * 3 - 70.0f <= centery && -win_height / 2.1f + r_sw_normy * sword_clock * 3 + 70.0f >= centery && donttouch == 0)
		{
			sw_size[1] = 0.0f;
			//일시무적
			health -= 10.0f;
			donttouch = 1;
		}
	}
	else if(sword_clock >= 650) {
		no_first_time = 1;
	}
	//30개의 별
	for (int q = 0; q < 30; q++) {
		int s_q = q % 3;
		star_size_arr[s_q] += turn[s_q];
		if (star_size_arr[s_q] >= 3.0f) {
			turn[s_q] = -0.01f;
		}
		else if (star_size_arr[s_q] <= 1.0f) {
			turn[s_q] = 0.01f;
		}
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(star_location[q][0], star_location[q][1], 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(star_size_arr[s_q], star_size_arr[s_q], 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_star();
	}


	//플레이어 위치에 따라 회전하는 케이크(초에서 칼 발사대.) 좌측하단 발사  
	float ritx = centerx + win_width / 2.1f;
	float rity = centery + win_height / 2.1f;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-win_width/2.1f, -win_height/2.1f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -atan2(ritx,rity), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake();
	//우측하단 발사
	ritx =  win_width / 2.1f- centerx;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.1f, -win_height / 2.1f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, atan2(ritx, rity), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake();
	//좌측상단
	ritx= centerx + win_width / 2.1f;
	rity = centery - win_height / 2.1f;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-win_width / 2.1f, win_height / 2.1f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -atan2(ritx, rity), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake();
	//우측상당
	ritx= win_width / 2.1f - centerx;
	ritx = win_width / 2.1f - centerx;
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.1f, win_height / 2.1f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, atan2(ritx, rity), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(7.0f, 7.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_cake();
	
	//비행기
	int air_clock = (timestamp % 1442) / 2 - 360;
	if (no_fff_time == -5000) {
		no_fff_time = air_clock;
		//printf("first");
	}
	if (no_fff_time == air_clock) {
		//printf("second");
		random_show = rand() % 3;
		//random_show = 2;
	}
	//sin1
	if (random_show == 0) {
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(100.0f * TO_RADIAN * 3*cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy+150, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(100.0f * TO_RADIAN * 3*cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy+120, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(100.0f * TO_RADIAN * 3*cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy +90, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
	}
	//sin2
	else if (random_show == 1) {
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(100.0f * TO_RADIAN *3* cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy - 150, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(100.0f * TO_RADIAN * 3*cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy - 120, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(150.0f * TO_RADIAN * 3*cosf((air_clock - k * 5) * 3 * TO_RADIAN));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy - 90, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 5) * 3, 100.0f * sinf((air_clock - k * 5) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
	}
	//tan
	else if (random_show == 2) {
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(150.0f * TO_RADIAN  / (cosf((air_clock - k *3) *3 * TO_RADIAN)* (cosf((air_clock - k *3) *3 * TO_RADIAN))));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx, airy, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k *3)*3 , 50*tanf((air_clock - k*3)*3  * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(150.0f * TO_RADIAN  / (cosf((air_clock - k * 3) *3* TO_RADIAN) * (cosf((air_clock - k * 3)*3 * TO_RADIAN))));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx-30, airy, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 3)*3, 50 * tanf((air_clock - k * 3)*3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
		for (int k = 0; k < 20; k++) {
			float rotation_angle_house = 90.0f + atanf(150.0f * TO_RADIAN / (cosf((air_clock - k * 3) * 3 * TO_RADIAN) * (cosf((air_clock - k * 3) * 3 * TO_RADIAN))));
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airx +30, airy, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3((float)(air_clock - k * 3) * 3, 50 * tanf((air_clock - k * 3) * 3 * TO_RADIAN), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_house, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}
	}
	/*ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-win_width / 4.0f, -win_height / 4.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, 90.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();  // 1
	
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.5f, -win_height / 8.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, 270.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();  // 2
 
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_height / 4.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, 45.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, -1.0f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -45.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-win_height / 4.0f, 0.0f, 0.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(win_width / 2.5f, -win_height / 8.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-win_width / 2.5f, win_height / 8.0f, 0.0f));
 
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(win_width / 2.5f, -win_height / 8.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, 270.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();  // 3
	 
	delx = win_width/14.0f; delr = 15.0f; dels = 1.1f;
	x = -delx; r = delr; s = dels;
	for (i = 0; i < 5; i++, x -= delx, r += delr, s *= dels) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, 15.0f*sqrtf(-x), 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, r*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			glTranslatef(x, 15.0f*sqrtf(-x), 0.0f);
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(s, s, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();  // 4
	}*/
	//테스트용 스티브 회오리
	//ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(mousex, mousey, 0.0f));

	for (int i = 0; i < 30; i++) {
		ModelMatrix = glm::rotate(glm::mat4(1.0f), timestamp / 100.0f+2.0f*(float)i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(face_location[i], 0.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		if (i == 2) {
			draw_steve();
		}
		else if (i == 7 || i == 13) {
			draw_cre();
		}
		else if (i % 2 == 1) {
			draw_steve();
		}
		else {
			draw_cre();
		}
		face_location[i] -= 0.2f;
		if (face_location[i] < 0.2f) {
			face_location[i] = 500.0f - (10.0f * i);
		}
	}
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, win_height/2-40, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(health, 80.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_h();
	if (health <= 0.0) {
		glutLeaveMainLoop();
	}
	glFlush();	
}   

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void special(int key, int x, int y) {
#define SENSITIVITY 2.0
	switch (key) {
	case GLUT_KEY_LEFT:
		centerx -= SENSITIVITY;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		centerx += SENSITIVITY;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		centery -= SENSITIVITY;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		centery += SENSITIVITY;
		glutPostRedisplay();
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		car_normx = mousex- centerx;
		car_normy = mousey-centery;
		float nn = sqrt(car_normx * car_normx + car_normy * car_normy);
		car_normx = car_normx / nn;
		car_normy = car_normy / nn;
		leftbuttonpressed = 1;
		//printf("~");
		static float tmpx = 0.0, tmpy = 0.0;
		float dx, dy;
		dx = centerx - mousex;
		dy = centery - mousey;
		click_mousex = mousex;
		click_mousey = mousey;
		rotate_angle = atan(dy / dx) + 0.0f * TO_RADIAN;
		/*if (dx > 0.0) {
			rotate_angle = atan(dy / dx) + 0.0f * TO_RADIAN;
		}
		else if (dx < 0.0) {
			rotate_angle = atan(dy / dx) - 0.0f * TO_RADIAN;
		}
		else if (dx == 0.0) {
			if (dy > 0.0) rotate_angle = 0.0f * TO_RADIAN;
			else  rotate_angle = 0.0f;
		}*/
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}
//마우스 누르면 변하는 곳
void motion(int x, int y) {
	/*static int delay = 0;
	static float tmpx = 0.0, tmpy = 0.0;
	float dx, dy;
	mousex = x - win_width / 2.0f;
	mousey = (win_height - y) - win_height / 2.0f;
	printf("@");
	if (leftbuttonpressed) {
		//작업중 . 비행기의 좌표를 여기가 아닌 display에서 바꾸는 것이 훨씬 좋지 않나 생각중. 구현 하기
		centerx =  x - win_width/2.0f, centery = (win_height - y) - win_height/2.0f;
		
		dx = centerx - tmpx;
		dy = centery - tmpy;
	  
		if (dx > 0.0) {
			rotate_angle = atan(dy / dx) + 90.0f*TO_RADIAN;
		}
		else if (dx < 0.0) {
			rotate_angle = atan(dy / dx) - 90.0f*TO_RADIAN;
		}
		else if (dx == 0.0) {
			if (dy > 0.0) rotate_angle = 180.0f*TO_RADIAN;
			else  rotate_angle = 0.0f;
		}
		printf("!");
		tmpx = centerx, tmpy = centery; 
		glutPostRedisplay();
	}
	*/
} 
void mousemove(int x, int y) {
	mousex = x - win_width / 2.0f;
	mousey = (win_height - y) - win_height / 2.0f;
}
void reshape(int width, int height) {
	win_width = width, win_height = height;
	for (int i = 0; i < 30; i++) {
		star_location[i][0] = rand() % win_width - (win_width / 2);
		star_location[i][1] = rand() % win_height - (win_height / 2);
	}
  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0, 
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();
	update_line();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_line);
	glDeleteBuffers(1, &VBO_line);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
	glutTimerFunc(10, timer, 0);
	//마우스 움직임 추적함수
	glutPassiveMotionFunc(mousemove);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(214 / 255.0f, 224 / 255.0f, 237 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_line();
	prepare_airplane();
	prepare_sword();
	prepare_cake();
	prepare_car2();
	prepare_house();
	prepare_star();
	prepare_cre();
	prepare_steve();
	prepare_hios();
	prepare_h();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

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

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 Simple2DTransformation_GLSL_3.0";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC', four arrows",
		"    - Mouse used: L-click and move"
	};
	srand(time(NULL));
	face_location[0] = 500.0f;
	for (int q = 1; q < 30; q++) {
		face_location[q] = face_location[0] - (10.0f * q);
	}
	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (1200, 800);


	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop ();
}



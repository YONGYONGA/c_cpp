//
//  DrawScene.cpp
//
//  Written for CSE4170
//  Department of Computer Science and Engineering
//  Copyright © 2022 Sogang University. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "LoadScene.h"

// Begin of shader setup
#include "Shaders/LoadShaders.h"
#include "ShadingInfo.h"
#include <glm/gtc/matrix_inverse.hpp> 
extern SCENE scene;
// for simple shaders
GLint loc_diffuse_texture, loc_flag_texture_mapping;

//go추가
GLuint h_ShaderProgram_simple, h_ShaderProgram_TXPS, h_ShaderProgram_GO, h_ShaderProgram_TX; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// for Phong Shading (Textured) shaders
#define NUMBER_OF_LIGHT_SUPPORTED 5
GLint loc_global_ambient_color;

loc_light_Parameters loc_light[NUMBER_OF_LIGHT_SUPPORTED];
GLint loc_flag_diffuse_texture_mapping, loc_flag_normal_texture_mapping, loc_flag_emissive_texture_mapping, loc_flag_normal_based_directX;
loc_Material_Parameters loc_material;
GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
//go추가
GLint loc_ModelViewProjectionMatrix_GO, loc_ModelViewMatrix_GO, loc_ModelViewMatrixInvTrans_GO;
loc_light_Parameters loc_light_GO[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material_GO;
GLint loc_global_ambient_color_GO;
//TX
GLint loc_ModelViewProjectionMatrix_TX, loc_ModelViewMatrix_TX, loc_ModelViewMatrixInvTrans_TX;
loc_light_Parameters loc_light_TX[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material_TX;
GLint loc_global_ambient_color_TX;

GLint loc_flag_fog; int flag_fog;

// for skybox shaders
GLuint h_ShaderProgram_skybox;
GLint loc_cubemap_skybox;
GLint loc_ModelViewProjectionMatrix_SKY;

#define TEXTURE_INDEX_DIFFUSE	(0)
#define TEXTURE_INDEX_NORMAL	(1)
#define TEXTURE_INDEX_SPECULAR	(2)
#define TEXTURE_INDEX_EMISSIVE	(3)
#define TEXTURE_INDEX_SKYMAP	(4)

// include glm/*.hpp only if necessary
// #include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.
glm::mat4 ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f


#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

/******************************** START: objects ********************************/
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];
// texture stuffs
#define TEXTURE_FLOOR				(0)
#define TEXTURE_TIGER				(1)
#define TEXTURE_WOLF				(2)
#define TEXTURE_SPIDER				(3)
#define TEXTURE_DRAGON				(4)
#define TEXTURE_OPTIMUS				(5)
#define TEXTURE_COW					(6)
#define TEXTURE_BUS					(7)
#define TEXTURE_BIKE				(8)
#define TEXTURE_GODZILLA			(9)
#define TEXTURE_IRONMAN				(10)
#define TEXTURE_TANK				(11)
#define TEXTURE_NATHAN				(12)
#define TEXTURE_OGRE				(13)
#define TEXTURE_CAT					(14)
#define TEXTURE_ANT					(15)
#define TEXTURE_TOWER				(16)
#define N_TEXTURES_USED				(17)

GLuint texture_names[N_TEXTURES_USED];

// texture id
#define TEXTURE_ID_DIFFUSE	(0)
#define TEXTURE_ID_NORMAL	(1)
//전역변수
GLint loc_u_flag_blending, loc_u_fragment_alpha;
GLint loc_u_flag_blending_TX, loc_u_fragment_alpha_TX;
GLint mo;

GLint loc_screen_effect, loc_screen_frequency, loc_screen_width, loc_screen_effect2,loc_rainbow;
GLint loc_angle;
GLfloat start_angle, end_angle;
int flag_screen_rainbow=0, flag_screen_effect=0;
int rainbow_on = 0;
float screen_frequency=1.0f, screen_width=0.125f;


int blend_mode=1;
int mosic=0;

float rotation_angle_cube;
unsigned int timestamp_scene = 0; // the global clock in the scene
int flag_animation, flag_polygon_fill;
int cur_frame_tiger = 0, cur_frame_ben = 0, cur_frame_wolf, cur_frame_spider = 0, cur_frame_nathan = 0;
int prevx, prevy;

using glm::mat4;
float dragon_x = -2533.3f, dragon_y = 1738.15f, dragon_z = 800.15f;
float c_dru[3] = { -1.0f,0.0f,0.0f }, c_drn[3] = { 0.0f,1.0f,0.0f };
float rotation_angle_tiger = 0.0f;
int change_tiger_movement = 0;
unsigned int tiger_timer = 0;
bool tiger_movement = false;
char camera_move_mode;
int camera_rotate = 0;
int prex, prey;
float zoom = 1.0f;
float ch_ratio = 0.05f;
float ben_angle;
float ben_move1;
float ben_move2;
float ben_move3;
glm::mat4  Model_Matrix_tiger_eye, ModelMatrix_tiger_behind, Inverse_for_camera;
//0~200 직선,  200~300 점프 300~500 직선
unsigned int ben_timer = 0;
//0 ,1일때 번갈아 가며 움직임
int change_ben_movement = 0;
float ben_x, ben_y, ben_z;
int tiger_head = 0;
float tiger_head_angle = 0.0f;
int spider_timer = 0;
int spider_ch = 0;
float spider_x, spider_y, spider_z;
int spider_timer2 = 0;
float spider_rotation;
float dragon_rotation = 0.0f;
typedef struct cows {
	float x, y, z;
	unsigned times;
}CC;
//최대 10
int cow_num = 0;
int cow_itr = -1;
int cow_start = 0;
CC cow_array[10];
glm::mat4 ModelMatrix_Tiger, ModelMatrix_Tiger_Eye;
bool phong_go = true;
int option3=1,option4=1,option5=1;
float cut_start_a=0.0f, cut_end_a=2.0f;
int angle_move = 0;


void calculate_ben_trans(int timer) {
	if (timer >= 500) {
		ben_timer = 0;
		timer = 0;
		if (change_ben_movement == 0) {
			change_ben_movement = 1;
		}
		else {
			change_ben_movement = 0;
		}
	}
	if (change_ben_movement == 0) {
		//직선이동
		if (timer <= 200) {
			ben_angle = atan2(600.0f, -800.0f);
			ben_x = 1400.0f + (600.0f / 200) * timer;
			ben_y = -1500.0f + (800.0f / 200) * timer;
			ben_z = 2000.0f;
		}
		//점프 높이는 1800으로
		else if (timer <= 300) {
			ben_angle = atan2(500, 400);
			ben_x = 2000.0f + (500.0f / 100) * (timer - 200);
			ben_y = -700.0f - (400.0f / 100) * (timer - 200);
			if (timer - 200 <= 30) {
				ben_z = 2000.0f + (timer - 200) * 10;
			}
			else {
				ben_z = 2300.0f - (timer - 230) * 7.2857f;
			}
		}
		else if (timer <= 500) {
			ben_angle = atan2(900, 300);
			ben_x = 2500.0f + (900.0f / 200) * (timer - 300);
			ben_y = -1100.0f - (300.0f / 200) * (timer - 300);
			ben_z = 1800.0f;
		}
	}
	else {
		if (timer <= 200) {
			ben_angle = atan2(-900, -300);
			ben_x = 3400.0f - (900.0f / 200) * (timer);
			ben_y = -1400.0f + (300.0f / 200) * (timer);
			ben_z = 1800.0f;
		}
		else if (timer <= 300) {
			if (timer <= 260) {
				ben_z = 1800.0f + (timer - 200) * 4;
			}
			else {
				ben_z = 2040.0f - (timer - 260);
			}
			ben_angle = atan2(-500, -400);
			ben_x = 2500.0f - (500.0f / 100) * (timer - 200);
			ben_y = -1100.0f + (400.0f / 100) * (timer - 200);
		}
		else if (timer <= 500) {
			ben_angle = atan2(-600.0f, 800.0f);
			ben_x = 2000.0f - (600.0f / 200) * (timer - 300);
			ben_y = -700.0f - (800.0f / 200) * (timer - 300);
			ben_z = 2000.0f;
		}
	}
}
void calculate_spider_trans(int time) {
	spider_timer++;
	if (time >= 580) {
		if (spider_ch == 0) {
			spider_ch = 1;
		}
		else {
			spider_ch = 0;
		}
		time = 0;
		spider_timer2 = 0;
		spider_timer = 0;
	}
	if (spider_ch == 0) {
		if (time < 200) {
			spider_x = 1900.0f;
			spider_y = -1300.0f;
			spider_z = 100.0f + 2 * time;
			spider_rotation = 0;
		}
		else if (time < 380) {
			if (time == 290 && spider_timer2 <= 200) {
				//printf("time: %d , time2 :%d\n", time, spider_timer2);
				spider_timer2++;
				spider_timer--;
				//내려왔다
				if (spider_timer2 <= 100) {
					spider_z = 700.0f - 1.5f * spider_timer2;
				}
				//올라왔다
				else {
					spider_z = 550.0f + 1.5f * (spider_timer2 - 100);
				}
			}
			else
			{
				spider_rotation += 1;
				spider_x = 1900.0f - (400.0f / 180) * (time - 200);
				spider_y = -1300.0f - (400.0f / 180) * (time - 200);
				float dd = sin(TO_RADIAN * spider_rotation);
				spider_z = 500.0f + 200.0f * dd;
			}
		}
		else {
			spider_x = 1500.0f; spider_y = -1700.0f;
			spider_z = 500 - 2 * (time - 380);
			spider_rotation = 180.0;
		}
	}
	else {
		if (time < 200) {
			spider_x = 1500.0f; spider_y = -1700.0f;
			spider_z = 100 + 2 * (time);
			spider_rotation = 0.0;
		}
		else if (time < 380) {
			if (time == 290 && spider_timer2 <= 200) {
				//printf("time: %d , time2 :%d\n", time, spider_timer2);
				spider_timer2++;
				spider_timer--;
				//내려왔다
				if (spider_timer2 <= 100) {
					spider_z = 700.0f - 1.5f * spider_timer2;
				}
				//올라왔다
				else {
					spider_z = 550.0f + 1.5f * (spider_timer2 - 100);
				}
			}
			else
			{
				spider_rotation -= 1;
				spider_x = 1500.0f + (400.0f / 180) * (time - 200);
				spider_y = -1700.0f + (400.0f / 180) * (time - 200);
				float dd = sin(TO_RADIAN * spider_rotation);
				spider_z = 500.0f - 200.0f * dd;
			}
			/*if (time - 200 < 40) {
				spider_z += 4;
			}
			else if(time-200>=40 && time-200<140)
			else {
				spider_z-=2;
			}*/
		}
		else {
			spider_x = 1900.0f;
			spider_y = -1300.0f;
			spider_z = 500 - 2 * (time - 380);
			spider_rotation = 180.0f;
		}
	}
}
void make_cow(int index) {
	cow_array[index].x = dragon_x;
	cow_array[index].y = dragon_y;
	cow_array[index].z = dragon_z;
	//printf("now: %lf", cow_array[index].z);
	cow_array[index].times = 0;
}
void My_glTexImage2D_from_file(const char* filename) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP* tx_pixmap, * tx_pixmap_32;

	int width, height;
	GLvoid* data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	FreeImage_FlipVertical(tx_pixmap);

	fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
}
int read_geometry_vnt(GLfloat** object, int bytes_per_primitive, char* filename) {
	int n_triangles;
	FILE* fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);

	*object = (float*)malloc(n_triangles * bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

// for multiple materials
int read_geometry_vntm(GLfloat** object, int bytes_per_primitive,
	int* n_matrial_indicies, int** material_indicies,
	int* n_materials, char*** diffuse_texture_names,
	Material_Parameters** material_parameters,
	bool* bOnce,
	char* filename) {
	FILE* fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}

	int n_faces;
	fread(&n_faces, sizeof(int), 1, fp);

	*object = (float*)malloc(n_faces * bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...\n", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_faces, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);

	fread(n_matrial_indicies, sizeof(int), 1, fp);

	int bytes_per_indices = sizeof(int) * 2;
	*material_indicies = (int*)malloc(bytes_per_indices * (*n_matrial_indicies)); // material id, offset
	if (*material_indicies == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...\n", filename);
		return -1;
	}

	fread(*material_indicies, bytes_per_indices, (*n_matrial_indicies), fp);

	if (*bOnce == false) {
		fread(n_materials, sizeof(int), 1, fp);

		*material_parameters = (Material_Parameters*)malloc(sizeof(Material_Parameters) * (*n_materials));
		*diffuse_texture_names = (char**)malloc(sizeof(char*) * (*n_materials));
		for (int i = 0; i < (*n_materials); i++) {
			fread((*material_parameters)[i].ambient_color, sizeof(float), 3, fp); //Ka
			fread((*material_parameters)[i].diffuse_color, sizeof(float), 3, fp); //Kd
			fread((*material_parameters)[i].specular_color, sizeof(float), 3, fp); //Ks
			fread(&(*material_parameters)[i].specular_exponent, sizeof(float), 1, fp); //Ns
			fread((*material_parameters)[i].emissive_color, sizeof(float), 3, fp); //Ke

			(*material_parameters)[i].ambient_color[3] = 1.0f;
			(*material_parameters)[i].diffuse_color[3] = 1.0f;
			(*material_parameters)[i].specular_color[3] = 1.0f;
			(*material_parameters)[i].emissive_color[3] = 1.0f;

			(*diffuse_texture_names)[i] = (char*)malloc(sizeof(char) * 256);
			fread((*diffuse_texture_names)[i], sizeof(char), 256, fp);
		}
		*bOnce = true;
	}

	fclose(fp);

	return n_faces;
}
/*********************************  START: camera *********************************/
typedef enum {
	CAMERA_u,
	CAMERA_i,
	CAMERA_o,
	CAMERA_p,
	CAMERA_a,
	CAMERA_t,
	CAMERA_g,
	CAMERA_new,
	NUM_CAMERAS
} CAMERA_INDEX;


typedef struct _Camera {
	float pos[3];
	float uaxis[3], vaxis[3], naxis[3];
	float fovy, aspect_ratio, near_c, far_c,zoom_factor;
	int move, rotation_axis;
} Camera;

Camera camera_info[NUM_CAMERAS];
Camera current_camera;

using glm::mat4;
void set_ViewMatrix_from_camera_frame(void) {
	ViewMatrix = glm::mat4(current_camera.uaxis[0], current_camera.vaxis[0], current_camera.naxis[0], 0.0f,
		current_camera.uaxis[1], current_camera.vaxis[1], current_camera.naxis[1], 0.0f,
		current_camera.uaxis[2], current_camera.vaxis[2], current_camera.naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-current_camera.pos[0], -current_camera.pos[1], -current_camera.pos[2]));
}

void set_current_camera(int camera_num) {
	if (camera_num == 13) {
		current_camera.pos[0] = dragon_x; current_camera.pos[1] = dragon_y; current_camera.pos[2] = dragon_z;
		current_camera.vaxis[0] = 0.0f; current_camera.vaxis[1] = 0.0f; current_camera.vaxis[2] = 1.0f;
		for (int i = 0; i < 3; i++) {
			current_camera.uaxis[i] = c_dru[i];
			current_camera.naxis[i] = c_drn[i];
		}
		current_camera.move = 0;
		current_camera.fovy = TO_RADIAN * scene.camera.fovy, current_camera.aspect_ratio = scene.camera.aspect, current_camera.near_c = 0.1f; current_camera.far_c = 50000.0f;
	}
	else
	{
		Camera* pCamera = &camera_info[camera_num];

		memcpy(&current_camera, pCamera, sizeof(Camera));

	}
	set_ViewMatrix_from_camera_frame();
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void initialize_camera(void) {
	//CAMERA_1 : original view
	//CAMERA_1 : original view
	
	Camera* pCamera = &camera_info[CAMERA_a];/*
	for (int k = 0; k < 3; k++)
	{
		pCamera->pos[k] = scene.camera.e[k];
		pCamera->uaxis[k] = scene.camera.u[k];
		pCamera->vaxis[k] = scene.camera.v[k];
		pCamera->naxis[k] = scene.camera.n[k];
	}

	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_2 : bistro view
	pCamera = &camera_info[CAMERA_2];
	pCamera->pos[0] = -241.351807f; pCamera->pos[1] = 1444.346313f; pCamera->pos[2] = 283.968231f;
	pCamera->uaxis[0] = -0.998411f; pCamera->uaxis[1] = 0.049119f; pCamera->uaxis[2] = -0.027553f;
	pCamera->vaxis[0] = -0.028485f; pCamera->vaxis[1] = -0.018375f; pCamera->vaxis[2] = 0.999417f;
	pCamera->naxis[0] = 0.048585f; pCamera->naxis[1] = 0.998617f; pCamera->naxis[2] = 0.019746f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_3 : tree view
	pCamera = &camera_info[CAMERA_3];
	pCamera->pos[0] = 1974.856567f; pCamera->pos[1] = -1588.545776f; pCamera->pos[2] = 98.843971f;
	pCamera->uaxis[0] = -0.357811f; pCamera->uaxis[1] = -0.933725f; pCamera->uaxis[2] = 0.010082f;
	pCamera->vaxis[0] = -0.180880f; pCamera->vaxis[1] = 0.079899f; pCamera->vaxis[2] = 0.980231f;
	pCamera->naxis[0] = -0.916095f; pCamera->naxis[1] = 0.348920f; pCamera->naxis[2] = -0.197483f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_4 : top view
	pCamera = &camera_info[CAMERA_4];
	pCamera->pos[0] = 0.0f; pCamera->pos[1] = 0.0f; pCamera->pos[2] = 18300.0f;
	pCamera->uaxis[0] = 1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 1.0f; pCamera->vaxis[2] = 0.0f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 0.0f; pCamera->naxis[2] = 1.0f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_5 : front view
	pCamera = &camera_info[CAMERA_5];
	pCamera->pos[0] = 0.0f; pCamera->pos[1] = 11700.0f; pCamera->pos[2] = 0.0f;
	pCamera->uaxis[0] = 1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 0.0f; pCamera->vaxis[2] = 1.0f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 1.0f; pCamera->naxis[2] = 0.0f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_6 : side view
	pCamera = &camera_info[CAMERA_6];
	pCamera->pos[0] = 14600.0f; pCamera->pos[1] = 0.0f; pCamera->pos[2] = 0.0f;
	pCamera->uaxis[0] = 0.0f; pCamera->uaxis[1] = 1.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 0.0f; pCamera->vaxis[2] = 1.0f;
	pCamera->naxis[0] = 1.0f; pCamera->naxis[1] = 0.0f; pCamera->naxis[2] = 0.0f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;
	*/
	//카메라 시작 u
	pCamera = &camera_info[CAMERA_u];
	pCamera->pos[0] = 4047.74414f; pCamera->pos[1] = -2365.170898f; pCamera->pos[2] = 803.752441f;
	pCamera->uaxis[0] = 0.445224f; pCamera->uaxis[1] = 0.893150f; pCamera->uaxis[2] = 0.063660f;
	pCamera->vaxis[0] = -0.303151f; pCamera->vaxis[1] = 0.083461f; pCamera->vaxis[2] = 0.949276f;
	pCamera->naxis[0] = 0.842529f; pCamera->naxis[1] = -0.441941f; pCamera->naxis[2] = 0.307917f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_i
	pCamera = &camera_info[CAMERA_i];
	pCamera->pos[0] = -2549.720215f; pCamera->pos[1] = 1696.579956f; pCamera->pos[2] = 1476.453857f;
	pCamera->uaxis[0] = -0.754710f; pCamera->uaxis[1] = -0.654198f; pCamera->uaxis[2] = -0.049175f;
	pCamera->vaxis[0] = 0.262310f; pCamera->vaxis[1] = -0.369620f; pCamera->vaxis[2] = 0.891372f;
	pCamera->naxis[0] = -0.601310f; pCamera->naxis[1] = 0.659831f; pCamera->naxis[2] = 0.450560f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_o
	pCamera = &camera_info[CAMERA_o];
	pCamera->pos[0] = 1216.968872f; pCamera->pos[1] = -2916.345703f; pCamera->pos[2] = 2713.749756f;
	pCamera->uaxis[0] = 0.886621f; pCamera->uaxis[1] = -0.462428f; pCamera->uaxis[2] = 0.005255f;
	pCamera->vaxis[0] = 0.239767f; pCamera->vaxis[1] = 0.469359f; pCamera->vaxis[2] = 0.849804f;
	pCamera->naxis[0] = -0.395442f; pCamera->naxis[1] = -0.752199f; pCamera->naxis[2] = 0.527021f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_p
	pCamera = &camera_info[CAMERA_p];
	pCamera->pos[0] = 404.55f; pCamera->pos[1] = 1911.39f; pCamera->pos[2] = 5605.56;
	pCamera->uaxis[0] = 0.6689f; pCamera->uaxis[1] = 0.74121f; pCamera->uaxis[2] = -0.055f;
	pCamera->vaxis[0] = -0.71f; pCamera->vaxis[1] = 0.66f; pCamera->vaxis[2] = 0.2277f;
	pCamera->naxis[0] = 0.2053f; pCamera->naxis[1] = -0.1128f; pCamera->naxis[2] = 0.9721f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	//CAMERA_a
	pCamera = &camera_info[CAMERA_a];
	pCamera->pos[0] = 5063.4414f; pCamera->pos[1] = -5016.1328f; pCamera->pos[2] = 4132.614746f;
	pCamera->uaxis[0] = 0.805736f; pCamera->uaxis[1] = 0.591593f; pCamera->uaxis[2] = 0.028354f;
	pCamera->vaxis[0] = -0.384997f; pCamera->vaxis[1] = 0.486781f; pCamera->vaxis[2] = 0.784102f;
	pCamera->naxis[0] = 0.450066f; pCamera->naxis[1] = -0.642695f; pCamera->naxis[2] = 0.619978f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;


	//드래곤카메라

	pCamera = &camera_info[CAMERA_new];
	pCamera->pos[0] = dragon_x; pCamera->pos[1] = dragon_y; pCamera->pos[2] = dragon_z;
	pCamera->uaxis[0] = -1.0f; pCamera->uaxis[1] = 0.0f; pCamera->uaxis[2] = 0.0f;
	pCamera->vaxis[0] = 0.0f; pCamera->vaxis[1] = 0.0f; pCamera->vaxis[2] = 1.0f;
	pCamera->naxis[0] = 0.0f; pCamera->naxis[1] = 1.0f; pCamera->naxis[2] = 0.0f;
	pCamera->move = 0;
	pCamera->fovy = TO_RADIAN * scene.camera.fovy, pCamera->aspect_ratio = scene.camera.aspect, pCamera->near_c = 0.1f; pCamera->far_c = 50000.0f;

	set_current_camera(CAMERA_a);
}
/*********************************  END: camera *********************************/

void bind_texture(GLuint tex, int glTextureId, GLuint texture_name) {
	glActiveTexture(GL_TEXTURE0 + glTextureId);
	glBindTexture(GL_TEXTURE_2D, texture_name);
	glUniform1i(tex, glTextureId);
}

/******************************  START: shader setup ****************************/
// Begin of Callback function definitions
void prepare_shader_program(void) {
	char string[256];

	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram_simple = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram_simple);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	//TXPS는 퐁
	ShaderInfo shader_info_TXPS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
		{ GL_NONE, NULL }
	};
	h_ShaderProgram_TXPS = LoadShaders(shader_info_TXPS);
	loc_ModelViewProjectionMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrixInvTrans");
	loc_global_ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_global_ambient_color");
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[i].light_on = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].shadow_on", i);
		loc_light[i].shadow_on = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[i].position = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[i].ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[i].specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[i].spot_direction = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_TXPS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_TXPS, string);
	}
	//phong 것
	loc_material.ambient_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.ambient_color");
	loc_material.diffuse_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.diffuse_color");
	loc_material.specular_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_color");
	loc_material.emissive_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.emissive_color");
	loc_material.specular_exponent = glGetUniformLocation(h_ShaderProgram_TXPS, "u_material.specular_exponent");

	loc_material.diffuseTex = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	loc_material.normalTex = glGetUniformLocation(h_ShaderProgram_TXPS, "u_normal_texture");
	loc_material.emissiveTex = glGetUniformLocation(h_ShaderProgram_TXPS, "u_emissive_texture");
	loc_diffuse_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_mapping");
	loc_flag_diffuse_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_diffuse_texture_mapping");
	loc_flag_normal_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_normal_texture_mapping");
	loc_flag_emissive_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_emissive_texture_mapping");
	loc_flag_normal_based_directX = glGetUniformLocation(h_ShaderProgram_TXPS, "u_normal_based_directX");
	loc_flag_fog = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_fog");
	//블렌딩용
	loc_u_flag_blending = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_blending");
	loc_u_fragment_alpha = glGetUniformLocation(h_ShaderProgram_TXPS, "u_fragment_alpha");

	start_angle = glGetUniformLocation(h_ShaderProgram_TXPS, "start_a");
	end_angle = glGetUniformLocation(h_ShaderProgram_TXPS, "end_a");
	loc_angle= glGetUniformLocation(h_ShaderProgram_TXPS, "moving_effect");

	//텍스쳐 없는 쉐이더 2개.에서 어짜피 텍스처 안입히니깐, 텍스처관련은 무시. 그외 phong처럼 쉐이더 연결
	ShaderInfo shader_info_GO[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Gouraud.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Gouraud.frag" },
		{ GL_NONE, NULL }
	};
	h_ShaderProgram_GO = LoadShaders(shader_info_GO);
	loc_ModelViewProjectionMatrix_GO = glGetUniformLocation(h_ShaderProgram_GO, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_GO = glGetUniformLocation(h_ShaderProgram_GO, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_GO = glGetUniformLocation(h_ShaderProgram_GO, "u_ModelViewMatrixInvTrans");
	loc_global_ambient_color_GO = glGetUniformLocation(h_ShaderProgram_GO, "u_global_ambient_color");
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light_GO[i].light_on = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].shadow_on", i);
		loc_light_GO[i].shadow_on = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light_GO[i].position = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light_GO[i].ambient_color = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light_GO[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light_GO[i].specular_color = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light_GO[i].spot_direction = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light_GO[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light_GO[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_GO, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light_GO[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_GO, string);
	}
	loc_material_GO.ambient_color = glGetUniformLocation(h_ShaderProgram_GO, "u_material.ambient_color");
	loc_material_GO.diffuse_color = glGetUniformLocation(h_ShaderProgram_GO, "u_material.diffuse_color");
	loc_material_GO.specular_color = glGetUniformLocation(h_ShaderProgram_GO, "u_material.specular_color");
	loc_material_GO.emissive_color = glGetUniformLocation(h_ShaderProgram_GO, "u_material.emissive_color");
	loc_material_GO.specular_exponent = glGetUniformLocation(h_ShaderProgram_GO, "u_material.specular_exponent");
	//2
	ShaderInfo shader_info_TX[3] = {
	{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
	{ GL_NONE, NULL }
	};
	h_ShaderProgram_TX = LoadShaders(shader_info_TX);
	loc_ModelViewProjectionMatrix_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_ModelViewMatrixInvTrans");
	loc_global_ambient_color_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_global_ambient_color");
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light_TX[i].light_on = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].shadow_on", i);
		loc_light_TX[i].shadow_on = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light_TX[i].position = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light_TX[i].ambient_color = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light_TX[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light_TX[i].specular_color = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light_TX[i].spot_direction = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light_TX[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light_TX[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_TX, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light_TX[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_TX, string);
	}

	loc_material_TX.ambient_color = glGetUniformLocation(h_ShaderProgram_TX, "u_material.ambient_color");
	loc_material_TX.diffuse_color = glGetUniformLocation(h_ShaderProgram_TX, "u_material.diffuse_color");
	loc_material_TX.specular_color = glGetUniformLocation(h_ShaderProgram_TX, "u_material.specular_color");
	loc_material_TX.emissive_color = glGetUniformLocation(h_ShaderProgram_TX, "u_material.emissive_color");
	loc_material_TX.specular_exponent = glGetUniformLocation(h_ShaderProgram_TX, "u_material.specular_exponent");

	//블렌딩용
	loc_u_flag_blending_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_flag_blending");
	loc_u_fragment_alpha_TX = glGetUniformLocation(h_ShaderProgram_TX, "u_fragment_alpha"); 
	//addi
	loc_screen_effect = glGetUniformLocation(h_ShaderProgram_TX, "screen_effect");
	loc_screen_effect2 = glGetUniformLocation(h_ShaderProgram_TX, "screen_effect2");
	loc_rainbow= glGetUniformLocation(h_ShaderProgram_TX, "rainbow");

	loc_screen_frequency = glGetUniformLocation(h_ShaderProgram_TX, "screen_frequency");
	loc_screen_width = glGetUniformLocation(h_ShaderProgram_TX, "screen_width");

	start_angle = glGetUniformLocation(h_ShaderProgram_TX, "start_a");
	end_angle = glGetUniformLocation(h_ShaderProgram_TX, "end_a");
	loc_angle = glGetUniformLocation(h_ShaderProgram_TXPS, "moving_effect");

	ShaderInfo shader_info_skybox[3] = {
	{ GL_VERTEX_SHADER, "Shaders/skybox.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/skybox.frag" },
	{ GL_NONE, NULL }
	};

	h_ShaderProgram_skybox = LoadShaders(shader_info_skybox);
	loc_cubemap_skybox = glGetUniformLocation(h_ShaderProgram_skybox, "u_skymap");
	loc_ModelViewProjectionMatrix_SKY = glGetUniformLocation(h_ShaderProgram_skybox, "u_ModelViewProjectionMatrix");
}
/*******************************  END: shder setup ******************************/

/****************************  START: geometry setup ****************************/
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define INDEX_VERTEX_POSITION	0
#define INDEX_NORMAL			1
#define INDEX_TEX_COORD			2

bool b_draw_grid = false;

//axes
GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) {
	// Initialize vertex buffer object.
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	fprintf(stdout, " * Loaded axes into graphics memory.\n");
}

void draw_axes(void) {
	if (!b_draw_grid)
		return;

	glUseProgram(h_ShaderProgram_simple);
	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(8000.0f, 8000.0f, 8000.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
	glLineWidth(1.0f);
	glUseProgram(0);
}

//grid
#define GRID_LENGTH			(100)
#define NUM_GRID_VETICES	((2 * GRID_LENGTH + 1) * 4)
GLuint grid_VBO, grid_VAO;
GLfloat grid_vertices[NUM_GRID_VETICES][3];
GLfloat grid_color[3] = { 0.5f, 0.5f, 0.5f };

void prepare_grid(void) {

	//set grid vertices
	int vertex_idx = 0;
	for (int x_idx = -GRID_LENGTH; x_idx <= GRID_LENGTH; x_idx++)
	{
		grid_vertices[vertex_idx][0] = x_idx;
		grid_vertices[vertex_idx][1] = -GRID_LENGTH;
		grid_vertices[vertex_idx][2] = 0.0f;
		vertex_idx++;

		grid_vertices[vertex_idx][0] = x_idx;
		grid_vertices[vertex_idx][1] = GRID_LENGTH;
		grid_vertices[vertex_idx][2] = 0.0f;
		vertex_idx++;
	}

	for (int y_idx = -GRID_LENGTH; y_idx <= GRID_LENGTH; y_idx++)
	{
		grid_vertices[vertex_idx][0] = -GRID_LENGTH;
		grid_vertices[vertex_idx][1] = y_idx;
		grid_vertices[vertex_idx][2] = 0.0f;
		vertex_idx++;

		grid_vertices[vertex_idx][0] = GRID_LENGTH;
		grid_vertices[vertex_idx][1] = y_idx;
		grid_vertices[vertex_idx][2] = 0.0f;
		vertex_idx++;
	}

	// Initialize vertex buffer object.
	glGenBuffers(1, &grid_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), &grid_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &grid_VAO);
	glBindVertexArray(grid_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, grid_VAO);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	fprintf(stdout, " * Loaded grid into graphics memory.\n");
}

void draw_grid(void) {
	if (!b_draw_grid)
		return;

	glUseProgram(h_ShaderProgram_simple);
	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(1.0f);
	glBindVertexArray(grid_VAO);
	glUniform3fv(loc_primitive_color, 1, grid_color);
	glDrawArrays(GL_LINES, 0, NUM_GRID_VETICES);
	glBindVertexArray(0);
	glLineWidth(1.0f);
	glUseProgram(0);
}
//object
// 
// 
void set_material(Material_Parameters* material_parameters) {
	glUseProgram(h_ShaderProgram_TXPS);
	glUniform4fv(loc_material.ambient_color, 1, material_parameters->ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_parameters->diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_parameters->specular_color);
	glUniform1f(loc_material.specular_exponent, material_parameters->specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_parameters->emissive_color);
}
//go
void set_material2(Material_Parameters* material_parameters) {
	glUseProgram(h_ShaderProgram_GO);
	glUniform4fv(loc_material_GO.ambient_color, 1, material_parameters->ambient_color);
	glUniform4fv(loc_material_GO.diffuse_color, 1, material_parameters->diffuse_color);
	glUniform4fv(loc_material_GO.specular_color, 1, material_parameters->specular_color);
	glUniform1f(loc_material_GO.specular_exponent, material_parameters->specular_exponent);
	glUniform4fv(loc_material_GO.emissive_color, 1, material_parameters->emissive_color);
}
//tx
void set_material3(Material_Parameters* material_parameters) {
	glUseProgram(h_ShaderProgram_TX);
	glUniform4fv(loc_material_TX.ambient_color, 1, material_parameters->ambient_color);
	glUniform4fv(loc_material_TX.diffuse_color, 1, material_parameters->diffuse_color);
	glUniform4fv(loc_material_TX.specular_color, 1, material_parameters->specular_color);
	glUniform1f(loc_material_TX.specular_exponent, material_parameters->specular_exponent);
	glUniform4fv(loc_material_TX.emissive_color, 1, material_parameters->emissive_color);
}

//rectangle
GLuint rectangle_VBO, rectangle_VAO;
GLfloat rectangle_vertices[12][3] = {  // vertices enumerated counterclockwise
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
};

Material_Parameters material_screen;

void prepare_rectangle(void) { // Draw coordinate axes.
	// Initialize vertex buffer object.
	glGenBuffers(1, &rectangle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &rectangle_VAO);
	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_screen.ambient_color[0] = 0.1745f;
	material_screen.ambient_color[1] = 0.0117f;
	material_screen.ambient_color[2] = 0.0117f;
	material_screen.ambient_color[3] = 1.0f;

	material_screen.diffuse_color[0] = 0.6142f;
	material_screen.diffuse_color[1] = 0.0413f;
	material_screen.diffuse_color[2] = 0.0413;
	material_screen.diffuse_color[3] = 1.0f;

	material_screen.specular_color[0] = 0.7278f;
	material_screen.specular_color[1] = 0.6269f;
	material_screen.specular_color[2] = 0.6269f;
	material_screen.specular_color[3] = 1.0f;

	material_screen.specular_exponent = 20.5f;

	material_screen.emissive_color[0] = 0.0f;
	material_screen.emissive_color[1] = 0.0f;
	material_screen.emissive_color[2] = 0.0f;
	material_screen.emissive_color[3] = 1.0f;
}
void draw_screen(void) {
	set_material3(&material_screen);
	glUniform1f(loc_u_fragment_alpha_TX, 1.0f);
	glUniform1i(loc_screen_effect, flag_screen_effect);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(3500.21f, -3500.37f, 100.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f* TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f* TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TX, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TX, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TX, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);



	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUniform1i(loc_screen_effect, 0);
	glUseProgram(0);
}
void draw_screen2(void) {
	set_material3(&material_screen);
	glUniform1i(loc_screen_effect2, flag_screen_rainbow);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-100.0f, -5000.0f, 2500.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 10000.0f, 1000.0f));
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f* TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TX, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TX, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TX, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);



	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUniform1i(loc_screen_effect2, 0);
	glUseProgram(0);
}
//큐브
GLuint cube_VBO, cube_VAO;
GLfloat cube_verticess[72][3] = { // vertices enumerated counterclockwise
	{ -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
	{ -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f },
	{ 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f },
	{ 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f },
	{ -1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f },
	{ 1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f },
	{ -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f },
	{ -1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }
};
Material_Parameters material_cube;
float cube_alpha;

void prepare_cube(void) {
	// Initialize vertex buffer object.
	glGenBuffers(1, &cube_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verticess), &cube_verticess[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &cube_VAO);
	glBindVertexArray(cube_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_cube.ambient_color[0] = 0.1745f;
	material_cube.ambient_color[1] = 0.01175f;
	material_cube.ambient_color[2] = 0.01175f;
	material_cube.ambient_color[3] = 1.0f;

	material_cube.diffuse_color[0] = 0.61424f;
	material_cube.diffuse_color[1] = 0.04136f;
	material_cube.diffuse_color[2] = 0.04136f;
	material_cube.diffuse_color[3] = 1.0f;

	material_cube.specular_color[0] = 0.727811f;
	material_cube.specular_color[1] = 0.626959f;
	material_cube.specular_color[2] = 0.626959f;
	material_cube.specular_color[3] = 1.0f;

	material_cube.specular_exponent = 76.8f;

	material_cube.emissive_color[0] = 0.0f;
	material_cube.emissive_color[1] = 0.0f;
	material_cube.emissive_color[2] = 0.0f;
	material_cube.emissive_color[3] = 1.0f;

	cube_alpha = 0.5f;
}
void draw_cube(void) {
	set_material3(&material_cube);
	glUniform1f(loc_u_fragment_alpha_TX, cube_alpha);


	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(4458.21f,-4339.37f, 3784.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));

	ModelViewMatrix = glm::rotate(ModelViewMatrix, rotation_angle_cube, glm::vec3(0.0f, 1.0f, 1.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TX, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TX, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TX, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glCullFace(GL_FRONT);

	glFrontFace(GL_CCW);

	glBindVertexArray(cube_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	glBindVertexArray(0);
	glCullFace(GL_BACK);

	glFrontFace(GL_CCW);

	glBindVertexArray(cube_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	glBindVertexArray(0);

	glUseProgram(0);

}

// ironman object
GLuint ironman_VBO, ironman_VAO;
int ironman_n_triangles;
GLfloat* ironman_vertices;

Material_Parameters material_ironman;

void prepare_ironman(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle, ironman_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/ironman_vnt.geom");
	ironman_n_triangles = read_geometry_vnt(&ironman_vertices, n_bytes_per_triangle, filename);
	ironman_n_triangles = read_geometry_vnt(&ironman_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	ironman_n_total_triangles += ironman_n_triangles;

	// initialize vertex buffer object
	glGenBuffers(1, &ironman_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glBufferData(GL_ARRAY_BUFFER, ironman_n_total_triangles * 3 * n_bytes_per_vertex, ironman_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(ironman_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &ironman_VAO);
	glBindVertexArray(ironman_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ironman_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_ironman.ambient_color[0] = 0.4f;
	material_ironman.ambient_color[1] = 0.4f;
	material_ironman.ambient_color[2] = 0.4f;
	material_ironman.ambient_color[3] = 1.0f;

	material_ironman.diffuse_color[0] = 0.900000f;
	material_ironman.diffuse_color[1] = 0.200000f;
	material_ironman.diffuse_color[2] = 0.200000f;
	material_ironman.diffuse_color[3] = 1.0f;

	material_ironman.specular_color[0] = 0.7f;
	material_ironman.specular_color[1] = 0.6f;
	material_ironman.specular_color[2] = 0.4f;
	material_ironman.specular_color[3] = 1.0f;

	material_ironman.specular_exponent = 52.334717f;

	material_ironman.emissive_color[0] = 0.1f;
	material_ironman.emissive_color[1] = 0.1f;
	material_ironman.emissive_color[2] = 0.0f;
	material_ironman.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_IRONMAN]);

	My_glTexImage2D_from_file("Data/static_objects/Ironman_Body.png");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_ironman(void) {
	glUseProgram(h_ShaderProgram_TXPS);
	set_material(&material_ironman);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_IRONMAN]);

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(367.21f, 1728.37f, 10.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (-40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);


	glFrontFace(GL_CW);

	glBindVertexArray(ironman_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * ironman_n_triangles);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
void draw_ironman2(void) {
	set_material(&material_ironman);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_IRONMAN]);

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(109.0f, -475.29f, 250.6527f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);




	glBindVertexArray(ironman_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * ironman_n_triangles);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
//godzilla
GLuint godzilla_VBO, godzilla_VAO;
int godzilla_n_triangles;
GLfloat* godzilla_vertices;

Material_Parameters material_godzilla;

void prepare_godzilla(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle, godzilla_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/godzilla_vnt.geom");
	godzilla_n_triangles = read_geometry_vnt(&godzilla_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	godzilla_n_total_triangles += godzilla_n_triangles;

	// initialize vertex buffer object
	glGenBuffers(1, &godzilla_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, godzilla_VBO);
	glBufferData(GL_ARRAY_BUFFER, godzilla_n_total_triangles * 3 * n_bytes_per_vertex, godzilla_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(godzilla_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &godzilla_VAO);
	glBindVertexArray(godzilla_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, godzilla_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_godzilla.ambient_color[0] = 0.5f;
	material_godzilla.ambient_color[1] = 0.5f;
	material_godzilla.ambient_color[2] = 0.5f;
	material_godzilla.ambient_color[3] = 1.0f;

	material_godzilla.diffuse_color[0] = 0.4f;
	material_godzilla.diffuse_color[1] = 0.4f;
	material_godzilla.diffuse_color[2] = 0.2f;
	material_godzilla.diffuse_color[3] = 1.0f;

	material_godzilla.specular_color[0] = 0.4f;
	material_godzilla.specular_color[1] = 0.4f;
	material_godzilla.specular_color[2] = 0.2f;
	material_godzilla.specular_color[3] = 1.0f;

	material_godzilla.specular_exponent = 5.334717f;

	material_godzilla.emissive_color[0] = 0.3f;
	material_godzilla.emissive_color[1] = 0.5f;
	material_godzilla.emissive_color[2] = 0.3f;
	material_godzilla.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_GODZILLA]);

	My_glTexImage2D_from_file("Data/static_objects/lele.png");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void draw_godzilla(void) {

	if (phong_go)
	{
		set_material3(&material_godzilla);
		//고릴라는 블렌딩 영향 없게 그냥 계속 초기화. 
		glUniform1f(loc_u_flag_blending_TX, 0);
		glUniform1f(loc_u_fragment_alpha_TX, 1.0f);
	}
	else {
		set_material2(&material_godzilla);

	}
	//bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_GODZILLA]);
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(3200.6f, -1950.79f, 10.0f));
	//ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (-100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	if(phong_go)
	{
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TX, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TX, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TX, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}
	else {
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GO, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GO, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GO, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	}

	glFrontFace(GL_CW);

	glBindVertexArray(godzilla_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * godzilla_n_triangles);
	glBindVertexArray(0);
	glUseProgram(0);

	//glBindTexture(GL_TEXTURE_2D, 0);
}
void draw_godzilla2(void) {
	set_material(&material_godzilla);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_GODZILLA]);


	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(1422.6f, 3554.85f, 10.87f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glFrontFace(GL_CW);

	glBindVertexArray(godzilla_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * godzilla_n_triangles);
	glBindVertexArray(0);
	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// ben object
#define N_BEN_FRAMES 30
GLuint ben_VBO, ben_VAO;
int ben_n_triangles[N_BEN_FRAMES];
int ben_vertex_offset[N_BEN_FRAMES];
GLfloat* ben_vertices[N_BEN_FRAMES];

int ben_n_material_indices;
int* ben_material_indices[N_BEN_FRAMES];

int ben_n_materials;
char** ben_diffuse_texture;
GLuint* ben_texture_names;

Material_Parameters* material_ben;

void prepare_ben(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, ben_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	bool bOnce = false;
	for (i = 0; i < N_BEN_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/ben/ben_vntm_%d%d.geom", i / 10, i % 10);
		ben_n_triangles[i] = read_geometry_vntm(&ben_vertices[i], n_bytes_per_triangle,
			&ben_n_material_indices,
			&ben_material_indices[i],
			&ben_n_materials,
			&ben_diffuse_texture,
			&material_ben,
			&bOnce,
			filename);

		// assume all geometry files are effective
		ben_n_total_triangles += ben_n_triangles[i];

		if (i == 0)
			ben_vertex_offset[i] = 0;
		else
			ben_vertex_offset[i] = ben_vertex_offset[i - 1] + 3 * ben_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &ben_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glBufferData(GL_ARRAY_BUFFER, ben_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_BEN_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, ben_vertex_offset[i] * n_bytes_per_vertex,
			ben_n_triangles[i] * n_bytes_per_triangle, ben_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_BEN_FRAMES; i++)
		free(ben_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &ben_VAO);
	glBindVertexArray(ben_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, ben_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	ben_texture_names = (GLuint*)malloc(sizeof(GLuint) * ben_n_materials);
	glGenTextures(ben_n_materials, ben_texture_names);

	for (int i = 0; i < ben_n_materials; i++) {
		if (strcmp("", ben_diffuse_texture[i])) {
			glBindTexture(GL_TEXTURE_2D, ben_texture_names[i]);

			sprintf(filename, "Data/dynamic_objects/ben/%s", ben_diffuse_texture[i]);
			My_glTexImage2D_from_file(filename);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void draw_ben(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(ben_VAO);

	for (int i = 0; i < ben_n_material_indices - 1; i++) {
		int cur_material_id = ben_material_indices[cur_frame_ben][2 * i];
		set_material(&material_ben[cur_material_id]);
		bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, ben_texture_names[cur_material_id]);

		int cur_vertex_offset = ben_vertex_offset[cur_frame_ben] + ben_material_indices[cur_frame_ben][2 * i + 1];
		int n_vertices = ben_material_indices[cur_frame_ben][2 * (i + 1) + 1] - ben_material_indices[cur_frame_ben][2 * i + 1];
		glDrawArrays(GL_TRIANGLES, cur_vertex_offset, n_vertices);

		glBindTexture(GL_TEXTURE_2D, 0);
		calculate_ben_trans(ben_timer);
		//ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(spider_x, spider_y, spider_z));
		ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(ben_x, ben_y, ben_z));
		//ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(3400.0f, -1400.0f, 1800.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(200.0f, 200.0f, 200.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, ben_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
		
		//눈이 0,.-0.2,0은 아니겠지만 효과상 그냥 작게함.(음수일수록 빛의 좌표는 위로(높이) 올라감) 
		glm::vec4 modeling_light(0.0f, -0.2f,0.0f,1.0f);
		glm::vec4 po_EC = ModelViewMatrix * modeling_light;
		//printf("%lf = %lf\n", modeling_light.x, po_EC.x);
		
		//이게 ben의 눈방향인듯 싶다
		glm::vec3 modeling_light_di(0.0f, 0.0f, -1.0f);
		glm::vec3 direct_EC= ModelViewMatrixInvTrans * modeling_light_di;

		for (int i = 0; i < 3; i++) {
			light[3].position[i] = po_EC[i];
			light[3].spot_direction[i] = direct_EC[i];
		}
		//printf("%lf = %lf , %lf = %lf \n", light[3].position[0],po_EC.x,light[3].spot_direction[0],direct_EC.x);
		//printf("%lf = %lf , %lf = %lf \n", light[3].position[1], po_EC.y, light[3].spot_direction[1], direct_EC.y);
		//printf("%lf = %lf , %lf = %lf \n", light[3].position[2], po_EC.z, light[3].spot_direction[2], direct_EC.z);



		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);


	}

	glBindVertexArray(0);
	glUseProgram(0);

}

//tiger
#define N_TIGER_FRAMES 12
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat* tiger_vertices[N_TIGER_FRAMES];

Material_Parameters material_tiger;

void prepare_tiger(void) { // vertices enumerated clockwise
	int i, n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/tiger/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		tiger_n_triangles[i] = read_geometry_vnt(&tiger_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		tiger_n_total_triangles += tiger_n_triangles[i];

		if (i == 0)
			tiger_vertex_offset[i] = 0;
		else
			tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &tiger_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_TIGER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, tiger_vertex_offset[i] * n_bytes_per_vertex,
			tiger_n_triangles[i] * n_bytes_per_triangle, tiger_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_TIGER_FRAMES; i++)
		free(tiger_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &tiger_VAO);
	glBindVertexArray(tiger_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_tiger.ambient_color[0] = 0.24725f;
	material_tiger.ambient_color[1] = 0.1995f;
	material_tiger.ambient_color[2] = 0.0745f;
	material_tiger.ambient_color[3] = 1.0f;

	material_tiger.diffuse_color[0] = 0.75164f;
	material_tiger.diffuse_color[1] = 0.60648f;
	material_tiger.diffuse_color[2] = 0.22648f;
	material_tiger.diffuse_color[3] = 1.0f;

	material_tiger.specular_color[0] = 0.728281f;
	material_tiger.specular_color[1] = 0.655802f;
	material_tiger.specular_color[2] = 0.466065f;
	material_tiger.specular_color[3] = 1.0f;

	material_tiger.specular_exponent = 51.2f;

	material_tiger.emissive_color[0] = 0.1f;
	material_tiger.emissive_color[1] = 0.1f;
	material_tiger.emissive_color[2] = 0.0f;
	material_tiger.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_TIGER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/tiger/tiger_tex.jpg");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_tiger(void) {
	glUseProgram(h_ShaderProgram_TXPS);

	set_material(&material_tiger);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_TIGER]);

	if (change_tiger_movement % 2 == 0) {
		ModelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4500.0f, -2700.0f, 10.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, rotation_angle_tiger, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-750.0f, 300.0f, 0.0f));
	}
	else {
		long double n = 3750.0f * 3750.0f + 1800.0f * 1800.0f;
		n = sqrt(n);
		if (tiger_timer >= 180) {
			ModelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3750.0f * (tiger_timer - 180) * 10 / n, -1900.0f * (tiger_timer - 180) * 10 / n, 10.0f));
			ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-3750.0f * 180 * 10 / n, 1900.0f * 180 * 10 / n, 10.0f));
			ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(3750.0f, -2400.0f, 10.0f));
			ModelViewMatrix = glm::rotate(ModelViewMatrix, atan2(3750.0f, 1800.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else {
			ModelViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3750.0f * tiger_timer * 10 / n, 1900.0f * tiger_timer * 10 / n, 10.0f));
			ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(3750.0f, -2400.0f, 10.0f));
			ModelViewMatrix = glm::rotate(ModelViewMatrix, atan2(-3750.0f, -1800.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	//ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1.0f, 5.0f, 5.0f));
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * (-80.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));

	//ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	if (camera_move_mode == 't') {
		//0,1,2,3  12,13,14,15에 각도 감소
		if (tiger_head < 4 || tiger_head >= 12) {
			if (tiger_head == 0) {
				//보험용
				tiger_head_angle = 0.0f;
			}
			else if (tiger_head >= 15) {
				tiger_head = 0;
			}
			tiger_head_angle -= 0.1f;
		}
		//4,5,6,7,8,9,10,11 에 각도 증가
		else {
			tiger_head_angle += 0.1f;
		}
		tiger_head++;
		Model_Matrix_tiger_eye = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -88.0f, 62.0f));
		Model_Matrix_tiger_eye = glm::rotate(Model_Matrix_tiger_eye, TO_RADIAN * tiger_head_angle, glm::vec3(1.0f, 0.0f, 0.0f));
		Model_Matrix_tiger_eye = glm::rotate(Model_Matrix_tiger_eye, TO_RADIAN * (180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Model_Matrix_tiger_eye = glm::rotate(Model_Matrix_tiger_eye, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Inverse_for_camera = ModelViewMatrix * Model_Matrix_tiger_eye;

		ViewMatrix = glm::affineInverse(Inverse_for_camera);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}
	else if (camera_move_mode == 'g') {
		Model_Matrix_tiger_eye = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 100.0f));
		Model_Matrix_tiger_eye = glm::rotate(Model_Matrix_tiger_eye, TO_RADIAN * (180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Model_Matrix_tiger_eye = glm::rotate(Model_Matrix_tiger_eye, TO_RADIAN * (90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Inverse_for_camera = ModelViewMatrix * Model_Matrix_tiger_eye;

		ViewMatrix = glm::affineInverse(Inverse_for_camera);
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	}

	glm::mat4 temp = ModelViewMatrix;
	ModelViewMatrix = ViewMatrix * ModelViewMatrix;
	ModelViewProjectionMatrix = ViewProjectionMatrix * temp;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);



	glFrontFace(GL_CW);

	glBindVertexArray(tiger_VAO);
	glDrawArrays(GL_TRIANGLES, tiger_vertex_offset[cur_frame_tiger], 3 * tiger_n_triangles[cur_frame_tiger]);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

// spider object
#define N_SPIDER_FRAMES 16
GLuint spider_VBO, spider_VAO;
int spider_n_triangles[N_SPIDER_FRAMES];
int spider_vertex_offset[N_SPIDER_FRAMES];
GLfloat* spider_vertices[N_SPIDER_FRAMES];

Material_Parameters material_spider;

void prepare_spider(void) {
	int i, n_bytes_per_vertex, n_bytes_per_triangle, spider_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_SPIDER_FRAMES; i++) {
		sprintf(filename, "Data/dynamic_objects/spider/spider_vnt_%d%d.geom", i / 10, i % 10);
		spider_n_triangles[i] = read_geometry_vnt(&spider_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		spider_n_total_triangles += spider_n_triangles[i];

		if (i == 0)
			spider_vertex_offset[i] = 0;
		else
			spider_vertex_offset[i] = spider_vertex_offset[i - 1] + 3 * spider_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &spider_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glBufferData(GL_ARRAY_BUFFER, spider_n_total_triangles * n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_SPIDER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, spider_vertex_offset[i] * n_bytes_per_vertex,
			spider_n_triangles[i] * n_bytes_per_triangle, spider_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_SPIDER_FRAMES; i++)
		free(spider_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &spider_VAO);
	glBindVertexArray(spider_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, spider_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_spider.ambient_color[0] = 0.5f;
	material_spider.ambient_color[1] = 0.5f;
	material_spider.ambient_color[2] = 0.5f;
	material_spider.ambient_color[3] = 1.0f;

	material_spider.diffuse_color[0] = 0.9f;
	material_spider.diffuse_color[1] = 0.5f;
	material_spider.diffuse_color[2] = 0.1f;
	material_spider.diffuse_color[3] = 1.0f;

	material_spider.specular_color[0] = 0.5f;
	material_spider.specular_color[1] = 0.5f;
	material_spider.specular_color[2] = 0.5f;
	material_spider.specular_color[3] = 1.0f;

	material_spider.specular_exponent = 11.334717f;

	material_spider.emissive_color[0] = 0.000000f;
	material_spider.emissive_color[1] = 0.000000f;
	material_spider.emissive_color[2] = 0.000000f;
	material_spider.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_SPIDER]);

	My_glTexImage2D_from_file("Data/dynamic_objects/spider/lala.png");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_spider(void) {
	set_material(&material_spider);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_SPIDER]);

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(spider_x, spider_y, spider_z));
	//printf("%lf %lf %lf\n", spider_x, spider_y, spider_z);
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * spider_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	if (spider_ch == 1) {
		ModelViewMatrix = glm::rotate(ModelViewMatrix, TO_RADIAN * 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);


	glFrontFace(GL_CW);

	glBindVertexArray(spider_VAO);
	glDrawArrays(GL_TRIANGLES, spider_vertex_offset[cur_frame_spider], 3 * spider_n_triangles[cur_frame_spider]);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

// dragon object
GLuint dragon_VBO, dragon_VAO;
int dragon_n_triangles;
GLfloat* dragon_vertices;

Material_Parameters material_dragon;

void prepare_dragon(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle, dragon_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/dragon_vnt.geom");
	dragon_n_triangles = read_geometry_vnt(&dragon_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	dragon_n_total_triangles += dragon_n_triangles;

	// initialize vertex buffer object
	glGenBuffers(1, &dragon_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, dragon_VBO);
	glBufferData(GL_ARRAY_BUFFER, dragon_n_total_triangles * 3 * n_bytes_per_vertex, dragon_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(dragon_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &dragon_VAO);
	glBindVertexArray(dragon_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, dragon_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_dragon.ambient_color[0] = 0.5f;
	material_dragon.ambient_color[1] = 0.5f;
	material_dragon.ambient_color[2] = 0.5f;
	material_dragon.ambient_color[3] = 1.0f;

	material_dragon.diffuse_color[0] = 0.3f;
	material_dragon.diffuse_color[1] = 0.3f;
	material_dragon.diffuse_color[2] = 0.4f;
	material_dragon.diffuse_color[3] = 1.0f;

	material_dragon.specular_color[0] = 0.3f;
	material_dragon.specular_color[1] = 0.3f;
	material_dragon.specular_color[2] = 0.4f;
	material_dragon.specular_color[3] = 1.0f;

	material_dragon.specular_exponent = 11.334717f;

	material_dragon.emissive_color[0] = 0.000000f;
	material_dragon.emissive_color[1] = 0.000000f;
	material_dragon.emissive_color[2] = 0.000000f;
	material_dragon.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_DRAGON]);

	My_glTexImage2D_from_file("Data/static_objects/dragon.bmp");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_dragon(void) {
	set_material(&material_dragon);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_DRAGON]);

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(dragon_x, dragon_y, dragon_z - 200.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, dragon_rotation * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));

	ModelViewMatrix = glm::rotate(ModelViewMatrix, -80.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glFrontFace(GL_CW);

	glBindVertexArray(dragon_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * dragon_n_triangles);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

// cow object
GLuint cow_VBO, cow_VAO;
int cow_n_triangles;
GLfloat* cow_vertices;

Material_Parameters material_cow;

void prepare_cow(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle, cow_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	sprintf(filename, "Data/static_objects/cow_vnt.geom");
	cow_n_triangles = read_geometry_vnt(&cow_vertices, n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	cow_n_total_triangles += cow_n_triangles;

	// initialize vertex buffer object
	glGenBuffers(1, &cow_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, cow_VBO);
	glBufferData(GL_ARRAY_BUFFER, cow_n_total_triangles * 3 * n_bytes_per_vertex, cow_vertices, GL_STATIC_DRAW);

	// as the geometry data exists now in graphics memory, ...
	free(cow_vertices);

	// initialize vertex array object
	glGenVertexArrays(1, &cow_VAO);
	glBindVertexArray(cow_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, cow_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_cow.ambient_color[0] = 0.5f;
	material_cow.ambient_color[1] = 0.5f;
	material_cow.ambient_color[2] = 0.5f;
	material_cow.ambient_color[3] = 1.0f;

	material_cow.diffuse_color[0] = 0.7f;
	material_cow.diffuse_color[1] = 0.5f;
	material_cow.diffuse_color[2] = 0.2f;
	material_cow.diffuse_color[3] = 1.0f;

	material_cow.specular_color[0] = 0.4f;
	material_cow.specular_color[1] = 0.4f;
	material_cow.specular_color[2] = 0.2f;
	material_cow.specular_color[3] = 1.0f;

	material_cow.specular_exponent = 5.334717f;

	material_cow.emissive_color[0] = 0.000000f;
	material_cow.emissive_color[1] = 0.000000f;
	material_cow.emissive_color[2] = 0.000000f;
	material_cow.emissive_color[3] = 1.0f;

	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_COW]);

	My_glTexImage2D_from_file("Data/static_objects/Cow.png");

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void draw_cow(int j) {
	set_material(&material_cow);
	bind_texture(loc_diffuse_texture, TEXTURE_ID_DIFFUSE, texture_names[TEXTURE_COW]);

	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(cow_array[j].x, cow_array[j].y, cow_array[j].z));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, cow_array[j].times * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	glFrontFace(GL_CW);

	glBindVertexArray(cow_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * cow_n_triangles);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

//bistro_exterior
GLuint* bistro_exterior_VBO;
GLuint* bistro_exterior_VAO;
int* bistro_exterior_n_triangles;
int* bistro_exterior_vertex_offset;
GLfloat** bistro_exterior_vertices;
GLuint* bistro_exterior_texture_names;

bool* flag_texture_mapping;

bool readTexImage2D_from_file(char* filename) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP* tx_pixmap, * tx_pixmap_32;

	int width, height;
	GLvoid* data;
	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	if (tx_pixmap == NULL)
		return false;
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	GLenum format, internalFormat;
	if (tx_bits_per_pixel == 32) {
		format = GL_BGRA;
		internalFormat = GL_RGBA;
	}
	else if (tx_bits_per_pixel == 24) {
		format = GL_BGR;
		internalFormat = GL_RGB;
	}
	else {
		fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap = FreeImage_ConvertTo32Bits(tx_pixmap);
		format = GL_BGRA;
		internalFormat = GL_RGBA;
	}

	width = FreeImage_GetWidth(tx_pixmap);
	height = FreeImage_GetHeight(tx_pixmap);
	data = FreeImage_GetBits(tx_pixmap);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap);

	return true;
}

void prepare_bistro_exterior(void) {
	int n_bytes_per_vertex, n_bytes_per_triangle;

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	// VBO, VAO malloc
	bistro_exterior_VBO = (GLuint*)malloc(sizeof(GLuint) * scene.n_materials);
	bistro_exterior_VAO = (GLuint*)malloc(sizeof(GLuint) * scene.n_materials);

	bistro_exterior_n_triangles = (int*)malloc(sizeof(int) * scene.n_materials);
	bistro_exterior_vertex_offset = (int*)malloc(sizeof(int) * scene.n_materials);

	flag_texture_mapping = (bool*)malloc(sizeof(bool) * scene.n_textures);

	// vertices
	bistro_exterior_vertices = (GLfloat**)malloc(sizeof(GLfloat*) * scene.n_materials);

	for (int materialIdx = 0; materialIdx < scene.n_materials; materialIdx++) {
		MATERIAL* pMaterial = &(scene.material_list[materialIdx]);
		GEOMETRY_TRIANGULAR_MESH* tm = &(pMaterial->geometry.tm);

		// vertex
		bistro_exterior_vertices[materialIdx] = (GLfloat*)malloc(sizeof(GLfloat) * 8 * tm->n_triangle * 3);

		int vertexIdx = 0;
		for (int triIdx = 0; triIdx < tm->n_triangle; triIdx++) {
			TRIANGLE tri = tm->triangle_list[triIdx];
			for (int triVertex = 0; triVertex < 3; triVertex++) {
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.position[triVertex].x;
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.position[triVertex].y;
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.position[triVertex].z;

				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.normal_vetcor[triVertex].x;
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.normal_vetcor[triVertex].y;
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.normal_vetcor[triVertex].z;

				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.texture_list[triVertex][0].u;
				bistro_exterior_vertices[materialIdx][vertexIdx++] = tri.texture_list[triVertex][0].v;
			}
		}

		// # of triangles
		bistro_exterior_n_triangles[materialIdx] = tm->n_triangle;

		if (materialIdx == 0)
			bistro_exterior_vertex_offset[materialIdx] = 0;
		else
			bistro_exterior_vertex_offset[materialIdx] = bistro_exterior_vertex_offset[materialIdx - 1] + 3 * bistro_exterior_n_triangles[materialIdx - 1];

		glGenBuffers(1, &bistro_exterior_VBO[materialIdx]);

		glBindBuffer(GL_ARRAY_BUFFER, bistro_exterior_VBO[materialIdx]);
		glBufferData(GL_ARRAY_BUFFER, bistro_exterior_n_triangles[materialIdx] * 3 * n_bytes_per_vertex,
			bistro_exterior_vertices[materialIdx], GL_STATIC_DRAW);

		// As the geometry data exists now in graphics memory, ...
		free(bistro_exterior_vertices[materialIdx]);

		// Initialize vertex array object.
		glGenVertexArrays(1, &bistro_exterior_VAO[materialIdx]);
		glBindVertexArray(bistro_exterior_VAO[materialIdx]);

		glBindBuffer(GL_ARRAY_BUFFER, bistro_exterior_VBO[materialIdx]);
		glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
		glEnableVertexAttribArray(INDEX_VERTEX_POSITION);
		glVertexAttribPointer(INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
		glEnableVertexAttribArray(INDEX_NORMAL);
		glVertexAttribPointer(INDEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
		glEnableVertexAttribArray(INDEX_TEX_COORD);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if ((materialIdx > 0) && (materialIdx % 100 == 0))
			fprintf(stdout, " * Loaded %d bistro exterior materials into graphics memory.\n", materialIdx / 100 * 100);
	}
	fprintf(stdout, " * Loaded %d bistro exterior materials into graphics memory.\n", scene.n_materials);

	// textures
	bistro_exterior_texture_names = (GLuint*)malloc(sizeof(GLuint) * scene.n_textures);
	glGenTextures(scene.n_textures, bistro_exterior_texture_names);

	for (int texId = 0; texId < scene.n_textures; texId++) {
		glBindTexture(GL_TEXTURE_2D, bistro_exterior_texture_names[texId]);

		bool bReturn = readTexImage2D_from_file(scene.texture_file_name[texId]);

		if (bReturn) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			flag_texture_mapping[texId] = true;
		}
		else {
			flag_texture_mapping[texId] = false;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	fprintf(stdout, " * Loaded bistro exterior textures into graphics memory.\n");

	free(bistro_exterior_vertices);
}

void bindTexture(GLuint tex, int glTextureId, int texId) {
	if (INVALID_TEX_ID != texId) {
		glActiveTexture(GL_TEXTURE0 + glTextureId);
		glBindTexture(GL_TEXTURE_2D, bistro_exterior_texture_names[texId]);
		glUniform1i(tex, glTextureId);
	}
}

void draw_bistro_exterior(void) {
	glUseProgram(h_ShaderProgram_TXPS);

	ModelViewMatrix = ViewMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::transpose(glm::inverse(glm::mat3(ModelViewMatrix)));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);

	for (int materialIdx = 0; materialIdx < scene.n_materials; materialIdx++) {
		// set material
		glUniform4fv(loc_material.ambient_color, 1, scene.material_list[materialIdx].shading.ph.ka);
		glUniform4fv(loc_material.diffuse_color, 1, scene.material_list[materialIdx].shading.ph.kd);
		glUniform4fv(loc_material.specular_color, 1, scene.material_list[materialIdx].shading.ph.ks);
		glUniform1f(loc_material.specular_exponent, scene.material_list[materialIdx].shading.ph.spec_exp);
		glUniform4fv(loc_material.emissive_color, 1, scene.material_list[materialIdx].shading.ph.kr);

		int diffuseTexId = scene.material_list[materialIdx].diffuseTexId;
		int normalTexId = scene.material_list[materialIdx].normalMapTexId;
		int emissiveTexId = scene.material_list[materialIdx].emissiveTexId;

		bindTexture(loc_material.diffuseTex, TEXTURE_INDEX_DIFFUSE, diffuseTexId);
		glUniform1i(loc_flag_diffuse_texture_mapping, flag_texture_mapping[diffuseTexId]);
		bindTexture(loc_material.normalTex, TEXTURE_INDEX_NORMAL, normalTexId);
		glUniform1i(loc_flag_normal_texture_mapping, flag_texture_mapping[normalTexId]);
		glUniform1i(loc_flag_normal_based_directX, 1); // only for bistro exterior
		bindTexture(loc_material.emissiveTex, TEXTURE_INDEX_EMISSIVE, emissiveTexId);
		glUniform1i(loc_flag_emissive_texture_mapping, flag_texture_mapping[emissiveTexId]);

		glEnable(GL_TEXTURE_2D);

		glBindVertexArray(bistro_exterior_VAO[materialIdx]);
		glDrawArrays(GL_TRIANGLES, 0, 3 * bistro_exterior_n_triangles[materialIdx]);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glUseProgram(0);
}

// skybox
GLuint skybox_VBO, skybox_VAO;
GLuint skybox_texture_name;

GLfloat cube_vertices[72][3] = {
	// vertices enumerated clockwise
	  // 6*2*3 * 2 (POS & NORM)

	// position
	-1.0f,  1.0f, -1.0f,    1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  1.0f, //right
	 1.0f,  1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f, -1.0f, //left
	 1.0f, -1.0f, -1.0f,   -1.0f, -1.0f,  1.0f,    1.0f, -1.0f,  1.0f,

	-1.0f, -1.0f,  1.0f,   -1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f, //top
	 1.0f,  1.0f,  1.0f,    1.0f, -1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,   -1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f, //bottom
	 1.0f, -1.0f, -1.0f,    1.0f,  1.0f, -1.0f,   -1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,   -1.0f, -1.0f, -1.0f,   -1.0f,  1.0f, -1.0f, //back
	-1.0f,  1.0f, -1.0f,   -1.0f,  1.0f,  1.0f,   -1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,    1.0f, -1.0f,  1.0f,    1.0f,  1.0f,  1.0f, //front
	 1.0f,  1.0f,  1.0f,    1.0f,  1.0f, -1.0f,    1.0f, -1.0f, -1.0f,

	 // normal
	 0.0f, 0.0f, -1.0f,      0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,
	 0.0f, 0.0f, -1.0f,      0.0f, 0.0f, -1.0f,     0.0f, 0.0f, -1.0f,

	-1.0f, 0.0f,  0.0f,     -1.0f, 0.0f,  0.0f,    -1.0f, 0.0f,  0.0f,
	-1.0f, 0.0f,  0.0f,     -1.0f, 0.0f,  0.0f,    -1.0f, 0.0f,  0.0f,

	 1.0f, 0.0f,  0.0f,      1.0f, 0.0f,  0.0f,     1.0f, 0.0f,  0.0f,
	 1.0f, 0.0f,  0.0f,      1.0f, 0.0f,  0.0f,     1.0f, 0.0f,  0.0f,

	 0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
	 0.0f, 0.0f, 1.0f,      0.0f, 0.0f, 1.0f,     0.0f, 0.0f, 1.0f,

	 0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
	 0.0f, 1.0f, 0.0f,      0.0f, 1.0f, 0.0f,     0.0f, 1.0f, 0.0f,

	 0.0f, -1.0f, 0.0f,      0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
	 0.0f, -1.0f, 0.0f,      0.0f, -1.0f, 0.0f,     0.0f, -1.0f, 0.0f
};

void readTexImage2DForCubeMap(const char* filename, GLenum texture_target) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP* tx_pixmap;

	int width, height;
	GLvoid* data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);

	width = FreeImage_GetWidth(tx_pixmap);
	height = FreeImage_GetHeight(tx_pixmap);
	FreeImage_FlipVertical(tx_pixmap);
	data = FreeImage_GetBits(tx_pixmap);

	glTexImage2D(texture_target, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap);
}

void prepare_skybox(void) { // Draw skybox.
	glGenVertexArrays(1, &skybox_VAO);
	glGenBuffers(1, &skybox_VBO);

	glBindVertexArray(skybox_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &cube_vertices[0][0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &skybox_texture_name);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture_name);

	readTexImage2DForCubeMap("Scene/Cubemap/px.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	readTexImage2DForCubeMap("Scene/Cubemap/nx.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	readTexImage2DForCubeMap("Scene/Cubemap/py.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	readTexImage2DForCubeMap("Scene/Cubemap/ny.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	readTexImage2DForCubeMap("Scene/Cubemap/pz.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	readTexImage2DForCubeMap("Scene/Cubemap/nz.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	fprintf(stdout, " * Loaded cube map textures into graphics memory.\n\n");

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void draw_skybox(void) {
	glUseProgram(h_ShaderProgram_skybox);

	glUniform1i(loc_cubemap_skybox, TEXTURE_INDEX_SKYMAP);

	ModelViewMatrix = ViewMatrix * glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(20000.0f, 20000.0f, 20000.0f));
	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(20000.0f, 20000.0f, 20000.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::transpose(glm::inverse(glm::mat3(ModelViewMatrix)));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_SKY, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

	glBindVertexArray(skybox_VAO);
	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_SKYMAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture_name);

	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}
/*****************************  END: geometry setup *****************************/

/********************************  START: light *********************************/

//쉐이더 도 light supprot증가 필수
void initialize_lights(void) { // follow OpenGL conventions for initialization
	//파일에서 읽은 빛의 개수는 하나인듯. scene관련 변수는 다른 loadscene에서 파일로 읽히는듯 하다.
	for (int i = 0; i < scene.n_lights; i++) {
		light[i].light_on = 1;
		if (LIGHT_DIRECTIONAL == scene.light_list[i].type) {
			light[i].position[0] = scene.light_list[i].pos[0];
			light[i].position[1] = scene.light_list[i].pos[1];
			light[i].position[2] = scene.light_list[i].pos[2];
			light[i].position[3] = 0.0f;
			
			light[i].ambient_color[0] = 0.0f;
			light[i].ambient_color[1] = 0.0f;
			light[i].ambient_color[2] = 0.0f;
			light[i].ambient_color[3] = 1.0f;

			light[i].diffuse_color[0] = scene.light_list[i].color[0];
			light[i].diffuse_color[1] = scene.light_list[i].color[1];
			light[i].diffuse_color[2] = scene.light_list[i].color[2];
			light[i].diffuse_color[3] = 1.0f;

			light[i].specular_color[0] = 0.5f;
			light[i].specular_color[1] = 0.5f;
			light[i].specular_color[2] = 0.5f;
			light[i].specular_color[3] = 1.0f;
		}
		else if (LIGHT_POINT == scene.light_list[i].type) {
			light[i].position[0] = scene.light_list[i].pos[0];
			light[i].position[1] = scene.light_list[i].pos[1];
			light[i].position[2] = scene.light_list[i].pos[2];
			light[i].position[3] = scene.light_list[i].pos[3];
			
			light[i].ambient_color[0] = 0.0f;
			light[i].ambient_color[1] = 0.0f;
			light[i].ambient_color[2] = 0.0f;
			light[i].ambient_color[3] = 1.0f;

			light[i].diffuse_color[0] = scene.light_list[i].color[0];
			light[i].diffuse_color[1] = scene.light_list[i].color[1];
			light[i].diffuse_color[2] = scene.light_list[i].color[2];
			light[i].diffuse_color[3] = 1.0f;

			light[i].specular_color[0] = 0.8f;
			light[i].specular_color[1] = 0.8f;
			light[i].specular_color[2] = 0.8f;
			light[i].specular_color[3] = 1.0f;

			light[i].light_attenuation_factors[0] = 1.0f;
			light[i].light_attenuation_factors[1] = 0.01;
			light[i].light_attenuation_factors[2] = 0.0f;
			light[i].light_attenuation_factors[3] = 1.0f;

			light[i].spot_cutoff_angle = 180.0f;
		}
		else {
			// for spot light, volume light, ...
		}
	}
	//사용자 정의 빛 추가 (set_light에 눈좌표로 변환해주는것 존재)
	light[1].light_on = 0;
	light[1].position[0] = 4457.0f; light[1].position[1] = -2509.0f; 
	light[1].position[2] = 2847.0f; light[1].position[3] = 0.0f;

	light[1].ambient_color[0] = 0.0f; light[1].ambient_color[1] = 0.0f;
	light[1].ambient_color[2] = 0.5f; light[1].ambient_color[3] = 1.0f;
	light[1].diffuse_color[0] = 1.0f; light[1].diffuse_color[1] = 1.0f;
	light[1].diffuse_color[2] = 1.0f; light[1].diffuse_color[3] = 1.0f;
	light[1].specular_color[0] = 0.0f; light[1].specular_color[1] = 0.0f;
	light[1].specular_color[2] = 0.0f; light[1].specular_color[3] = 1.0f;

	//눈좌표설정 카메라
	light[2].light_on = 1;
	//카메라 초기화전에 light 초기화 시키므로 의미가 없음. 그냥 display에서 눈좌표 기준 연산예정
	light[2].position[0] = 4457.0f; light[2].position[1] = -2509.0f;
	light[2].position[2] = 2847.0f; light[2].position[3] = 1.0f;
	light[2].spot_direction[0] = 0.0f; light[1].spot_direction[2] = 0.0f; 
	light[2].spot_direction[2] = -1.0f;


	light[2].ambient_color[0] = 0.0f; light[2].ambient_color[1] = 0.0f;
	light[2].ambient_color[2] = 0.5f; light[2].ambient_color[3] = 1.0f;

	light[2].diffuse_color[0] = 0.5f; light[2].diffuse_color[1] = 0.5f;
	light[2].diffuse_color[2] = 0.5f; light[2].diffuse_color[3] = 1.0f;


	light[2].specular_color[0] = 0.0f; light[2].specular_color[1] = 0.0f;
	light[2].specular_color[2] = 0.0f; light[2].specular_color[3] = 1.0f;

	light[2].spot_cutoff_angle = 10.0f;
	light[2].spot_exponent = 15.0f;
	//모델링기준 카메라
	light[3].light_on = 1;
	//어짜피 draw쪽에서 새롭게 계산함. 의미 x
	light[3].position[0] =0.0f; light[3].position[1] = 0.0f;
	light[3].position[2] = 0.0f; light[3].position[3] = 1.0f;

	light[3].spot_direction[0] = 1.0f; light[3].spot_direction[2] = 0.0f;
	light[3].spot_direction[2] = 1.0f;


	light[3].ambient_color[0] = 0.0f; light[3].ambient_color[1] = 0.0f;
	light[3].ambient_color[2] = 0.5f; light[3].ambient_color[3] = 1.0f;

	light[3].diffuse_color[0] = 0.5f; light[3].diffuse_color[1] = 0.5f;
	light[3].diffuse_color[2] = 0.5f; light[3].diffuse_color[3] = 1.0f;


	light[3].specular_color[0] = 0.0f; light[3].specular_color[1] = 0.0f;
	light[3].specular_color[2] = 0.0f; light[3].specular_color[3] = 1.0f;

	light[3].spot_cutoff_angle = 20.0f;
	light[3].spot_exponent = 20.0f;

	//추가
	light[4].light_on = 1;
	light[4].position[0] = -1122.0f; light[4].position[1] = -1089.0f;
	light[4].position[2] = 4000.0f; light[4].position[3] = 1.0f;
	light[4].ambient_color[0] = 0.1745f; light[4].ambient_color[1] = 0.01175f;
	light[4].ambient_color[2] = 0.01175f; light[4].ambient_color[3] = 1.0f;

	light[4].diffuse_color[0] = 0.61424f; light[4].diffuse_color[1] = 0.04136f;
	light[4].diffuse_color[2] = 0.04136f; light[4].diffuse_color[3] = 1.0f;

	light[4].specular_color[0] = 0.728211f; light[4].specular_color[1] = 0.626959f;
	light[4].specular_color[2] = 0.626959f; light[4].specular_color[3] = 1.0f;

	light[4].spot_direction[0] = 0.0f; light[4].spot_direction[1] = 0.0f; // spot light direction in WC
	light[4].spot_direction[2] = -1.0f;
	light[4].spot_cutoff_angle = 20.0f;
	light[4].spot_exponent = 20.0f;

}

void set_lights(void) {
	glUseProgram(h_ShaderProgram_TXPS);

	glUniform4f(loc_global_ambient_color, 0.7f, 0.7f, 0.7f, 1.0f);

	glm::vec4 light_position_EC;
	glm::vec3 light_direction_EC;
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {

		glUniform1i(loc_light[i].light_on, light[i].light_on);
		glUniform1i(loc_light[i].shadow_on, light[i].shadow_on);
		if (i == 2) {
			//눈좌표 중심0,0,0
			light_position_EC = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else if (i == 3) {
			//이미 EC 연산 display에서 종료
			light_position_EC = glm::vec4(light[3].position[0], light[3].position[1], light[3].position[2], 1.0f);
		}
		else {
			light_position_EC = ViewMatrix * glm::vec4(light[i].position[0], light[i].position[1], light[i].position[2], light[i].position[3]);
		}
		glUniform4fv(loc_light[i].position, 1, &light_position_EC[0]);
		glUniform4fv(loc_light[i].ambient_color, 1, light[i].ambient_color);
		glUniform4fv(loc_light[i].diffuse_color, 1, light[i].diffuse_color);
		glUniform4fv(loc_light[i].specular_color, 1, light[i].specular_color);

		if (0.0f != light[i].position[3]) {
			if (i == 2) {
				//어짜피 눈좌표 중심인거 그냥 이것도 연산 x. 시점방향으로 빛쏘기.
				light_direction_EC = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else if (i == 3) {
				light_direction_EC = glm::vec3(light[3].spot_direction[0], light[3].spot_direction[1], light[3].spot_direction[2]);
			}
			else{
				light_direction_EC = glm::transpose(glm::inverse(glm::mat3(ViewMatrix))) * glm::vec3(light[i].spot_direction[0], light[i].spot_direction[1], light[i].spot_direction[2]);
			}
			glUniform3fv(loc_light[i].spot_direction, 1, &light_direction_EC[0]);
			glUniform1f(loc_light[i].spot_exponent, light[i].spot_exponent);
			glUniform1f(loc_light[i].spot_cutoff_angle, light[i].spot_cutoff_angle);
			glUniform4fv(loc_light[i].light_attenuation_factors, 1, light[i].light_attenuation_factors);
		}
	}

	glUseProgram(0);
}
//이건 그냥 보이는 용은 아님. 어짜피 배경에선 .TXPS쉐이더 쓰기때문에. 단지 여기서 shader 빛인자를 초기화 하길래 그냥 덮어써버린거임(이거에 영향받는건 고릴라와 큐브뿐)
void set_lights2(void) {
	//고릴라는 ben이랑 어짜피 멀어서 영향 전혀 안받을것 같으므로 i=3일땐 예외처리 없음
	glUseProgram(h_ShaderProgram_GO);
	glUniform4f(loc_global_ambient_color_GO, 0.7f, 0.7f, 0.7f, 1.0f);

	glm::vec4 light_position_EC;
	glm::vec3 light_direction_EC;
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light_GO[i].light_on, light[i].light_on);
		glUniform1i(loc_light_GO[i].shadow_on, light[i].shadow_on);
		
		if (i == 2) {
			//눈좌표 중심0,0,0
			light_position_EC = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else {

			light_position_EC = ViewMatrix * glm::vec4(light[i].position[0], light[i].position[1], light[i].position[2], light[i].position[3]);
		}
		glUniform4fv(loc_light_GO[i].position, 1, &light_position_EC[0]);

		glUniform4fv(loc_light_GO[i].ambient_color, 1, light[i].ambient_color);
		glUniform4fv(loc_light_GO[i].diffuse_color, 1, light[i].diffuse_color);
		glUniform4fv(loc_light_GO[i].specular_color, 1, light[i].specular_color);

		if (0.0f != light[i].position[3]) {
			if (i == 2) {
				//어짜피 눈좌표 중심인거 그냥 이것도 연산 x. 시점방향으로 빛쏘기.
				light_direction_EC = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else {
				light_direction_EC = glm::transpose(glm::inverse(glm::mat3(ViewMatrix))) * glm::vec3(light[i].spot_direction[0], light[i].spot_direction[1], light[i].spot_direction[2]);
			}
			glUniform3fv(loc_light_GO[i].spot_direction, 1, &light_direction_EC[0]);
			glUniform1f(loc_light_GO[i].spot_exponent, light[i].spot_exponent);
			glUniform1f(loc_light_GO[i].spot_cutoff_angle, light[i].spot_cutoff_angle);
			glUniform4fv(loc_light_GO[i].light_attenuation_factors, 1, light[i].light_attenuation_factors);
		}
	}

	glUseProgram(0);

	glUseProgram(h_ShaderProgram_TX);
	glUniform4f(loc_global_ambient_color_TX, 0.7f, 0.7f, 0.7f, 1.0f);

	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light_TX[i].light_on, light[i].light_on);
		glUniform1i(loc_light_TX[i].shadow_on, light[i].shadow_on);

		if (i == 2) {
			//눈좌표 중심0,0,0
			light_position_EC = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else {

			light_position_EC = ViewMatrix * glm::vec4(light[i].position[0], light[i].position[1], light[i].position[2], light[i].position[3]);
		}
		glUniform4fv(loc_light_TX[i].position, 1, &light_position_EC[0]);

		glUniform4fv(loc_light_TX[i].ambient_color, 1, light[i].ambient_color);
		glUniform4fv(loc_light_TX[i].diffuse_color, 1, light[i].diffuse_color);
		glUniform4fv(loc_light_TX[i].specular_color, 1, light[i].specular_color);

		if (0.0f != light[i].position[3]) {
			if (i == 2) {
				//어짜피 눈좌표 중심인거 그냥 이것도 연산 x. 시점방향으로 빛쏘기.
				light_direction_EC = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else {
				light_direction_EC = glm::transpose(glm::inverse(glm::mat3(ViewMatrix))) * glm::vec3(light[i].spot_direction[0], light[i].spot_direction[1], light[i].spot_direction[2]);
			}
			glUniform3fv(loc_light_TX[i].spot_direction, 1, &light_direction_EC[0]);
			glUniform1f(loc_light_TX[i].spot_exponent, light[i].spot_exponent);
			glUniform1f(loc_light_TX[i].spot_cutoff_angle, light[i].spot_cutoff_angle);
			glUniform4fv(loc_light_TX[i].light_attenuation_factors, 1, light[i].light_attenuation_factors);
		}
	}

	glUseProgram(0);

}
/*********************************  END: light **********************************/

/********************  START: callback function definitions *********************/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//a아니면 그냥 불끄기. 키보드에서 바뀌어도 여기서 그냥 0됨.
	if (camera_move_mode != 'a') {
		light[2].light_on = 0;
	}


	set_lights2();
	set_lights();

	draw_grid();
	draw_axes();
	draw_bistro_exterior();
	draw_skybox();

	draw_tiger();
	draw_spider();
	draw_ironman();
	draw_ironman2();
	draw_godzilla();
	draw_godzilla2();
	draw_dragon();
	for (int j = 0; j < cow_num; j++) {
		draw_cow(j);
	}
	draw_ben();
	if (blend_mode == 1) {
		glUseProgram(h_ShaderProgram_TX);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(loc_u_flag_blending_TX, 1);
	}
	draw_cube();
	glUseProgram(h_ShaderProgram_TX);
	glUniform1i(loc_u_flag_blending_TX, 0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);

	if (flag_screen_effect) {
		draw_screen();
	}
	if (flag_screen_rainbow) {
		draw_screen2();
	}
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		//호랑이 움직임 제어하는 s
	case 's':
		if (tiger_movement)
			tiger_movement = false;
		else
			tiger_movement = true;
		glutPostRedisplay();
		break;
	case 'f':
		b_draw_grid = b_draw_grid ? false : true;
		glutPostRedisplay();
		break;
	case '1':
		phong_go = false;
		break;
	case '2':
		phong_go = true;
		break;
	case '3':
		light[1].light_on=1- light[1].light_on;
		break;
	case '4':
		light[2].light_on = 1 - light[2].light_on;
		break;
	case '5':
		light[3].light_on = 1 - light[3].light_on;
		break;
	case '6':
		blend_mode = 1 - blend_mode;
		glutPostRedisplay();
		break;
	case 'r':
		if (blend_mode) {
			cube_alpha += 0.05f;
			if (cube_alpha > 1.0f)
				cube_alpha = 1.0f;
			glutPostRedisplay();
		}
		break;
	case 'R':
		if (blend_mode) {
			cube_alpha -= 0.05f;
			if (cube_alpha < 0.0f)
				cube_alpha = 0.0f;
			glutPostRedisplay();
		}
		break;
	case '7':
		flag_screen_effect = 1 - flag_screen_effect;
		glutPostRedisplay();
		break;
	case '8':
		flag_screen_rainbow = 1 - flag_screen_rainbow;
		glutPostRedisplay();
		break;
	case '9':
		angle_move = 1 - angle_move;
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1f(loc_angle, angle_move);
		glUseProgram(0);
		break;
	case 'e':
		if (flag_screen_rainbow) {
			rainbow_on = 1 - rainbow_on;
			glUseProgram(h_ShaderProgram_TX);
			glUniform1f(loc_rainbow, rainbow_on);
			glUseProgram(0);
			glutPostRedisplay();
		}
		break;
	case 'w':
		if (flag_screen_effect) {
			screen_frequency += 5.0f;
			if (screen_frequency > 50.0f)
				screen_frequency = 50.0f;
			glUseProgram(h_ShaderProgram_TX);
			glUniform1f(loc_screen_frequency, screen_frequency);
			glUseProgram(0);
			glutPostRedisplay();
		}
		break;
	case 'W':
		if (flag_screen_effect) {
			screen_frequency -= 5.0f;
			if (screen_frequency <1.0f)
				screen_frequency = 1.0f;
			glUseProgram(h_ShaderProgram_TX);
			glUniform1f(loc_screen_frequency, screen_frequency);
			glUseProgram(0);
			glutPostRedisplay();
		}
		break;
	case 'u':
		camera_move_mode = 'u';
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		set_current_camera(CAMERA_u);
		glutPostRedisplay();
		break;
	case 'i':
		camera_move_mode = 'i';
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		set_current_camera(CAMERA_i);
		glutPostRedisplay();
		break;
	case 'o':
		camera_move_mode = 'o';
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		set_current_camera(CAMERA_o);
		glutPostRedisplay();
		break;
	case 'p':
		camera_move_mode = 'p';
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		set_current_camera(CAMERA_p);
		glutPostRedisplay();
		break;
	case 'a':
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		camera_move_mode = 'a';
		set_current_camera(CAMERA_a);
		glutPostRedisplay();
		break;
	case 't':
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		camera_move_mode = 't';
		glutPostRedisplay();
		break;
	case 'g':
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		camera_move_mode = 'g';
		glutPostRedisplay();
		break;
	case 'c':
		camera_move_mode = 'c';
		zoom = 1.0f;
		ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		set_current_camera(CAMERA_new);
		glutPostRedisplay();
		break;
	case 'x':
		if (camera_move_mode == 'c') {
			cow_num++;
			if (cow_num > 10)
				cow_num = 10;
			//소 배열 수정인덱스 확인 숫자
			cow_itr++;
			cow_itr %= 10;
			make_cow(cow_itr);
		}
		break;
	case'z'://이동카메라 구현 먼저 하고, 이걸로 좌표찍어서 그쪽에 4개 설치하자.
		printf("camera position : %lf %lf %lf", current_camera.pos[0], current_camera.pos[1], current_camera.pos[2]);
		printf("\ncamera uaxis : %lf %lf %lf", current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
		printf("\ncamera vaxis : %lf %lf %lf", current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]);
		printf("\ncamera naxis : %lf %lf %lf\n", current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}
void timer_scene(int value) {
	if (angle_move) {
		cut_start_a += 0.2f;
		cut_end_a += 0.2f;
		if (cut_start_a >= 20.0f) {
			cut_start_a = 0.0f;
		}
		if (cut_end_a >= 20.0f) {
			cut_end_a = 0.0f;
		}
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1f(start_angle, cut_start_a);
		glUniform1f(end_angle, cut_end_a);
		glUseProgram(0);
	}
	rotation_angle_cube = (timestamp_scene % 360) * TO_RADIAN;
	for (int i = 0; i < cow_num; i++) {
		//printf("%lf\n", cow_array[i].z);

		if (cow_array[i].z >= 50.0f) {
			cow_array[i].z -= 10.0f;
			cow_array[i].times = (cow_array[i].times + 1) % UINT_MAX;
		}
	}
	timestamp_scene = (timestamp_scene + 1) % UINT_MAX;
	cur_frame_ben = timestamp_scene % N_BEN_FRAMES;
	cur_frame_spider = timestamp_scene % 16;
	calculate_spider_trans(spider_timer);
	ben_timer++;
	if (tiger_movement) {
		tiger_timer++;
		cur_frame_tiger = tiger_timer % 12;
		if (tiger_timer % 360 >= 358) {
			tiger_timer = 0;
			change_tiger_movement++;
		}
		if (change_tiger_movement % 2 == 0)
		{
			rotation_angle_tiger = (tiger_timer % 360) * TO_RADIAN;
		}
	}
	glutPostRedisplay();
	if (flag_animation)
		glutTimerFunc(10, timer_scene, 0);
}
void motion(int x, int y) {
	glm::mat3 RotationMatrix;
	glm::vec3 direction;
	if (camera_rotate == 1) {
		//u축회전
		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), 0.1f * TO_RADIAN * (y - prey), glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2])));
		direction = RotationMatrix * glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]);

		current_camera.vaxis[0] = direction.x;
		current_camera.vaxis[1] = direction.y;
		current_camera.vaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
		current_camera.naxis[0] = direction.x;
		current_camera.naxis[1] = direction.y;
		current_camera.naxis[2] = direction.z;
		//v축회전
		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), 0.1f * TO_RADIAN * (x - prex), glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2])));
		direction = RotationMatrix * glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
		current_camera.uaxis[0] = direction.x;
		current_camera.uaxis[1] = direction.y;
		current_camera.uaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
		current_camera.naxis[0] = direction.x;
		current_camera.naxis[1] = direction.y;
		current_camera.naxis[2] = direction.z;
	}
	else if (camera_rotate == 2) {
		//n축회전
		RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), 0.5f * TO_RADIAN * (y - prey), glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2])));
		direction = RotationMatrix * glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
		current_camera.uaxis[0] = direction.x;
		current_camera.uaxis[1] = direction.y;
		current_camera.uaxis[2] = direction.z;
		direction = RotationMatrix * glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2]);
		current_camera.vaxis[0] = direction.x;
		current_camera.vaxis[1] = direction.y;
		current_camera.vaxis[2] = direction.z;
	}
	prex = x;
	prey = y;
	set_ViewMatrix_from_camera_frame();
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glutPostRedisplay();
}

void special_key(int key, int x, int y) {
	if (camera_move_mode == 'a') {
		if (key == GLUT_KEY_LEFT) {
			current_camera.pos[0] -= 30.0f * current_camera.uaxis[0];
			current_camera.pos[1] -= 30.0f * current_camera.uaxis[1];
			current_camera.pos[2] -= 30.0f * current_camera.uaxis[2];
		}
		else if (key == GLUT_KEY_RIGHT) {
			current_camera.pos[0] += 30.0f * current_camera.uaxis[0];
			current_camera.pos[1] += 30.0f * current_camera.uaxis[1];
			current_camera.pos[2] += 30.0f * current_camera.uaxis[2];
		}
		else if (key == GLUT_KEY_UP) {
			current_camera.pos[0] += 30.0f * current_camera.vaxis[0];
			current_camera.pos[1] += 30.0f * current_camera.vaxis[1];
			current_camera.pos[2] += 30.0f * current_camera.vaxis[2];
		}
		else if (key == GLUT_KEY_DOWN) {
			current_camera.pos[0] -= 30.0f * current_camera.vaxis[0];
			current_camera.pos[1] -= 30.0f * current_camera.vaxis[1];
			current_camera.pos[2] -= 30.0f * current_camera.vaxis[2];
		}
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
	else if (camera_move_mode == 'c') {
		glm::mat3 RotationMatrix;
		glm::vec3 direction;
		if (key == GLUT_KEY_LEFT) {
			RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), 1.0f * TO_RADIAN, glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2])));
			direction = RotationMatrix * glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
			current_camera.uaxis[0] = direction.x;
			current_camera.uaxis[1] = direction.y;
			current_camera.uaxis[2] = direction.z;
			direction = RotationMatrix * glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
			current_camera.naxis[0] = direction.x;
			current_camera.naxis[1] = direction.y;
			current_camera.naxis[2] = direction.z;
			dragon_rotation++;
		}
		else if (key == GLUT_KEY_RIGHT) {
			RotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), -1.0f * TO_RADIAN, glm::vec3(current_camera.vaxis[0], current_camera.vaxis[1], current_camera.vaxis[2])));
			direction = RotationMatrix * glm::vec3(current_camera.uaxis[0], current_camera.uaxis[1], current_camera.uaxis[2]);
			current_camera.uaxis[0] = direction.x;
			current_camera.uaxis[1] = direction.y;
			current_camera.uaxis[2] = direction.z;
			direction = RotationMatrix * glm::vec3(current_camera.naxis[0], current_camera.naxis[1], current_camera.naxis[2]);
			current_camera.naxis[0] = direction.x;
			current_camera.naxis[1] = direction.y;
			current_camera.naxis[2] = direction.z;
			dragon_rotation--;
		}
		else if (key == GLUT_KEY_UP) {
			current_camera.pos[0] -= 40.0f * current_camera.naxis[0];
			current_camera.pos[1] -= 40.0f * current_camera.naxis[1];
			current_camera.pos[2] -= 40.0f * current_camera.naxis[2];
		}
		else if (key == GLUT_KEY_DOWN) {
			current_camera.pos[0] += 40.0f * current_camera.naxis[0];
			current_camera.pos[1] += 40.0f * current_camera.naxis[1];
			current_camera.pos[2] += 40.0f * current_camera.naxis[2];
		}
		dragon_x = current_camera.pos[0];
		dragon_y = current_camera.pos[1];
		dragon_z = current_camera.pos[2];
		for (int i = 0; i < 3; i++) {
			c_dru[i] = current_camera.uaxis[i];
			c_drn[i] = current_camera.naxis[i];
		}
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
}
void wheel(int wheel, int d, int x, int y) {
	if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		if (d > 0) {
			zoom -= ch_ratio;
			ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		}
		else {
			zoom += ch_ratio;
			ProjectionMatrix = glm::perspective(zoom * current_camera.fovy, current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
		}
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
	else if (camera_move_mode == 'a') {

		if (d > 0) {
			current_camera.pos[0] -= 20.0f * current_camera.naxis[0];
			current_camera.pos[1] -= 20.0f * current_camera.naxis[1];
			current_camera.pos[2] -= 20.0f * current_camera.naxis[2];
		}
		else {
			current_camera.pos[0] += 20.0f * current_camera.naxis[0];
			current_camera.pos[1] += 20.0f * current_camera.naxis[1];
			current_camera.pos[2] += 20.0f * current_camera.naxis[2];
		}
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}
	else if (camera_move_mode == 'c') {
		if (d > 0) {
			current_camera.pos[0] += 40.0f * current_camera.vaxis[0];
			current_camera.pos[1] += 40.0f * current_camera.vaxis[1];
			current_camera.pos[2] += 40.0f * current_camera.vaxis[2];
		}
		else {
			current_camera.pos[0] -= 40.0f * current_camera.vaxis[0];
			current_camera.pos[1] -= 40.0f * current_camera.vaxis[1];
			current_camera.pos[2] -= 40.0f * current_camera.vaxis[2];
		}
		dragon_x = current_camera.pos[0];
		dragon_y = current_camera.pos[1];
		dragon_z = current_camera.pos[2];
		set_ViewMatrix_from_camera_frame();
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
	}

}

void mouse(int button, int state, int x, int y) {
	if ((camera_move_mode == 'a' || camera_move_mode == 'c') && glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				camera_rotate = 1;
				prex = x;
				prey = y;
			}
			else {
				camera_rotate = 0;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			if (state == GLUT_DOWN) {
				camera_rotate = 2;
				prex = x;
				prey = y;
			}
			else {
				camera_rotate = 0;
			}
		}
	}
}

void reshape(int width, int height) {
	float aspect_ratio;
	glViewport(0, 0, width, height);
	current_camera.aspect_ratio = (float)width / height;
	ProjectionMatrix = glm::perspective(current_camera.zoom_factor * current_camera.fovy,
		current_camera.aspect_ratio, current_camera.near_c, current_camera.far_c);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &grid_VAO);
	glDeleteBuffers(1, &grid_VBO);

	glDeleteVertexArrays(scene.n_materials, bistro_exterior_VAO);
	glDeleteBuffers(scene.n_materials, bistro_exterior_VBO);

	glDeleteVertexArrays(1, &skybox_VAO);
	glDeleteBuffers(1, &skybox_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);


	glDeleteVertexArrays(1, &spider_VAO);
	glDeleteBuffers(1, &spider_VBO);

	glDeleteVertexArrays(1, &dragon_VAO);
	glDeleteBuffers(1, &dragon_VBO);

	glDeleteVertexArrays(1, &cow_VAO);
	glDeleteBuffers(1, &cow_VBO);


	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);

	for (int i = 0; i < ben_n_materials; i++)
		free(ben_diffuse_texture[i]);

	free(ben_diffuse_texture);
	free(material_ben);
	glDeleteTextures(ben_n_materials, ben_texture_names);
	
	
	glDeleteVertexArrays(1, &ironman_VAO);
	glDeleteBuffers(1, &ironman_VBO);

	glDeleteVertexArrays(1, &godzilla_VAO);
	glDeleteBuffers(1, &godzilla_VBO);

	glDeleteVertexArrays(1, &cube_VAO);
	glDeleteBuffers(1, &cube_VBO);

	glDeleteTextures(scene.n_textures, bistro_exterior_texture_names);
	glDeleteTextures(N_TEXTURES_USED, texture_names);

	free(bistro_exterior_n_triangles);
	free(bistro_exterior_vertex_offset);

	free(bistro_exterior_VAO);
	free(bistro_exterior_VBO);

	free(bistro_exterior_texture_names);
	free(flag_texture_mapping);
}
/*********************  END: callback function definitions **********************/

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(wheel);
	glutSpecialFunc(special_key);

	glutCloseFunc(cleanup);
}

void initialize_flags(void) {
	flag_animation = 1;
	flag_polygon_fill = 1;
	flag_fog = 0;

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_flag_fog, flag_fog);
	glUniform1i(loc_flag_texture_mapping, true);
	glUseProgram(0);
}

void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::mat4(1.0f);
	ProjectionMatrix = glm::mat4(1.0f);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	initialize_lights();
	initialize_flags();
	glGenTextures(N_TEXTURES_USED, texture_names);
}
void set_up_scene_lights(void) {
	// point_light_EC: use light 0
	light[0].light_on = 1;
	light[0].position[0] = 0.0f; light[0].position[1] = 100.0f; 	// point light position in EC
	light[0].position[2] = 0.0f; light[0].position[3] = 1.0f;

	light[0].ambient_color[0] = 0.13f; light[0].ambient_color[1] = 0.13f;
	light[0].ambient_color[2] = 0.13f; light[0].ambient_color[3] = 1.0f;

	light[0].diffuse_color[0] = 0.5f; light[0].diffuse_color[1] = 0.5f;
	light[0].diffuse_color[2] = 0.5f; light[0].diffuse_color[3] = 1.5f;

	light[0].specular_color[0] = 0.8f; light[0].specular_color[1] = 0.8f;
	light[0].specular_color[2] = 0.8f; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: use light 1
	/*light[1].light_on = 1;
	light[1].position[0] = -200.0f; light[1].position[1] = 500.0f; // spot light position in WC
	light[1].position[2] = -200.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.152f; light[1].ambient_color[1] = 0.152f;
	light[1].ambient_color[2] = 0.152f; light[1].ambient_color[3] = 1.0f;

	light[1].diffuse_color[0] = 0.572f; light[1].diffuse_color[1] = 0.572f;
	light[1].diffuse_color[2] = 0.572f; light[1].diffuse_color[3] = 1.0f;

	light[1].specular_color[0] = 0.772f; light[1].specular_color[1] = 0.772f;
	light[1].specular_color[2] = 0.772f; light[1].specular_color[3] = 1.0f;

	light[1].spot_direction[0] = 0.0f; light[1].spot_direction[1] = -1.0f; // spot light direction in WC
	light[1].spot_direction[2] = 0.0f;
	light[1].spot_cutoff_angle = 20.0f;
	light[1].spot_exponent = 8.0f;*/

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_light[0].light_on, light[0].light_on);
	glUniform4fv(loc_light[0].position, 1, light[0].position);
	glUniform4fv(loc_light[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[1].light_on, light[1].light_on);
	// need to supply position in EC for shading
	glm::vec4 position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
		light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1],
		light[1].spot_direction[2]);
	glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1].spot_exponent, light[1].spot_exponent);


	glUseProgram(h_ShaderProgram_GO);

	glUniform1i(loc_light_GO[0].light_on, 1);
	glUniform4fv(loc_light_GO[0].position, 1, light[0].position);
	glUniform4fv(loc_light_GO[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light_GO[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light_GO[0].specular_color, 1, light[0].specular_color);

	glUseProgram(0);
}
void prepare_scene(void) {
	prepare_axes();
	prepare_grid();
	prepare_bistro_exterior();
	prepare_ironman();
	prepare_godzilla();
	prepare_ben();
	prepare_spider();
	prepare_tiger();
	prepare_dragon();
	prepare_cow();
	prepare_skybox();
	prepare_cube();
	prepare_rectangle();
	//set_up_scene_lights();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
	initialize_camera();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "********************************************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "********************************************************************************\n\n");
}

void print_message(const char* m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char* program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "********************************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n********************************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 9
void drawScene(int argc, char* argv[]) {
	char program_name[64] = "Sogang CSE4170 Bistro Exterior Scene";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used:",
		"		'f' : draw x, y, z axes and grid",
		"		'1' : set the camera for original view",
		"		'2' : set the camera for bistro view",
		"		'3' : set the camera for tree view",
		"		'4' : set the camera for top view",
		"		'5' : set the camera for front view",
		"		'6' : set the camera for side view",
		"		'ESC' : program close",
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 600);
	glutInitWindowPosition(20, 20);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}

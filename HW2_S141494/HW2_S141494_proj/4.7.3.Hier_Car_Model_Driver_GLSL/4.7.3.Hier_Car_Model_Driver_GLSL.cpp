#define _CRT_SECURE_NO_WARNINGS

#define USE_set_ViewMatrix_for_driver2

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define FRONT_WHEEL_ROLL_ANGLE_DELTA 5.0f
#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> //inverse, affineInverse, etc.



glm::mat4 ModelMatrix_CAR_BODY, ModelMatrix_CAR_WHEEL, ModelMatrix_CAR_NUT, ModelMatrix_CAR_DRIVER;
// The following two variables are computed only once in initialize_camera().
glm::mat4 ModelMatrix_CAR_BODY_to_DRIVER; 
glm::mat4 ModelMatrix_CAR_BODY_to_DRIVER_INVERSE; // used for set_ViewMatrix_for_driver2()
glm::mat4 ModelMatrix_CAR_BODY_INVERSE; // used for set_ViewMatrix_for_driver2()

#include "Camera.h"
#include "Geometry.h"


float wheel_angle = 0, box_scale=0, wheel_angle2 = 0;
glm::vec3 main_loc,hier_loc,loot_loc,king_loc,queen_loc,jack_loc;
void draw_0();
void draw_1();
void draw_2();


void draw_0() {

	main_loc = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::mat4 ModelMatrix_OBJECT;
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, main_loc+glm::vec3(2,1.5,0));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.3f, 0.3f, 0.3f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.85f, 0.80f, 0.5f);
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT);
	for (float i = 0; i < 4; i++) {
		hier_loc =main_loc+glm::vec3(2*i+0.0f, 0.0f, 0.0f);
		draw_1();
	}

}

void draw_1() {
	glm::mat4 ModelMatrix_OBJECT;
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT,hier_loc);
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.15f, 0.30f, 0.5f);
	draw_geom_obj(GEOM_OBJ_ID_BOX);
	for (int i = 0; i < 5; i++) {
		loot_loc = hier_loc - glm::vec3(0.0, 1.0, -1.0 + i*0.5);
		draw_2();
	}
}
void draw_2() {
	glm::mat4 ModelMatrix_OBJECT;
		ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, loot_loc);
		ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.1f, 0.1f, 0.1f));
		ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniform3f(loc_primitive_color, 1.0f, 1.0f, 1.0f);
		draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);


}

void draw_j() {
	glm::mat4 ModelMatrix_OBJECT;
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, jack_loc);
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.5f, 0.5f, 0.5f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 1.0f, 1.0f, 1.0f);
	draw_geom_obj(GEOM_OBJ_ID_BOX);
}

void draw_q() {
	glm::mat4 ModelMatrix_OBJECT;
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, queen_loc);

	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.3f, 0.3f, 0.3f);
	draw_geom_obj(GEOM_OBJ_ID_BOX);
	for (float i = 0; i < 3; i++) {
		for (float j = 0; j < 3; j++) {
			jack_loc = queen_loc + glm::vec3(-1 + 1 * i, -1+ 1 * j, 1.0f);
			draw_j();
		}
	}
}
void draw_k() {
	glm::mat4 ModelMatrix_OBJECT;
	king_loc = glm::vec3(0.0f, 0.0f, 20.0f);
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, king_loc);
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(2.0f, 2.0f, 2.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.0f, 0.0f, 0.0f);
	draw_geom_obj(GEOM_OBJ_ID_BOX);
	for (float i = 0; i < 3; i++) {
		for (float j = 0; j < 3; j++) {
			queen_loc = king_loc + glm::vec3(-2.0+2.0 * i, -2.0 + 2.0 * j, 3.0f);
			draw_q();
		}
	}
}


void draw_objects_in_world_2(void) {
	glm::mat4 ModelMatrix_OBJECT;
	for (int i = 0; i < 4; i++)
	{
		ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(60.0f-i*10.0f, 0.0f, 0.0f));
		ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f*i, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniform3f(loc_primitive_color, 0.15f*i, 0.30f*i, 0.5f*i);
		draw_geom_obj(GEOM_OBJ_ID_TEAPOT);
	}// 4개의 주전자 회전*이동*회전*변환
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 12, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(50.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(4.0f, 4.0f, 4.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.5, 0.5, 0.5);
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT);// 박스 회전*이동*회전*크기변환

	for (int i = 0; i < 4; i++)
	{
		ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 24, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(60.0f - i*10.0f, 0.0f, 0.0f));
		ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f*i, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniform3f(loc_primitive_color, 0.3f*i, 0.15f*i, 0.1f*i);
		draw_geom_obj(GEOM_OBJ_ID_BOX);
	}//4개의 박스 회전*이동*회전 변환
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 36, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(50.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(3.0f, 3.0f, 3.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.5, 0.5, 0.5);
	draw_geom_obj(GEOM_OBJ_ID_BOX);// 큰 박스 회전*이동*크기 변환

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 48, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(50.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.8f, 0.4f, 0.1f);
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);// 바퀴 회전*이동 변환
	
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 48, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(70.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.8f, 0.0f, 0.1f);
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY);// 차체 회전*이동 변환

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 48, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(20.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(1.0f, 7.0f, 7.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.4f,0.2f, 0.7f);
	draw_geom_obj(GEOM_OBJ_ID_CAR_NUT);// 너트 회전*이동*z축확대 변환
	
	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 60, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(40.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(1.0f, 1.0f, 10.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.0, 0.0, 0.0);
	draw_geom_obj(GEOM_OBJ_ID_CAR_NUT);// 너트 회전*이동*확대*회전  변환

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 180, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(30.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*wheel_angle, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*wheel_angle2, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.8f, 0.4f, 0.1f);
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);// 모든방향으로 회전하는 바퀴 동적회전*동적회전(키보드 상하좌우)

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 0, glm::vec3(0.0f,1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(wheel_angle/(3.4f* 3.14), 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*wheel_angle, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.5f, 0.2f, 0.2f);
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);// 구르는 바퀴 동적회전*동적이동 (키보드 상하)

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * 200, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(15.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(0.1f+box_scale, 0.1f+ box_scale, 0.1f+ box_scale));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*wheel_angle, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.0f, 0.0f, 0.0f);
	draw_geom_obj(GEOM_OBJ_ID_BOX);// 자체회전하며 커졌다 작아지는 박스 동적회전*동적스케일(키보드 상하)

	ModelMatrix_OBJECT = glm::rotate(glm::mat4(1.0f), TO_RADIAN * (wheel_angle/5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_OBJECT = glm::translate(ModelMatrix_OBJECT, glm::vec3(15.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::rotate(ModelMatrix_OBJECT, -TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_OBJECT = glm::scale(ModelMatrix_OBJECT, glm::vec3(1.0f, 1.0f, 1.0 + 0.1*sin(wheel_angle / 15.0f)));
	ModelViewProjectionMatrix = ViewProjectionMatrix[0] * ModelMatrix_OBJECT;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.0f, 0.0f, 0.0f);
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT);//WD주변을 회전하는 찻주전자 동적회전* 동적변형(키보드 상하)

}

/*********************************  START: callbacks *********************************/
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[0], glm::vec3(8.0f, 8.0f, 8.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
 	draw_axes();
	glLineWidth(1.0f);

	draw_objects_in_world_2();
	draw_0();
	draw_k();

	glutSwapBuffers();
}
void save() {
//	temp[now] = camera[0];
	temp[now].prp = camera[0].prp;
	temp[now].vrp = camera[0].vrp;
	temp[now].vup = camera[0].vup;
	temp[now].fov_y = camera[0].fov_y;
}
void load() {
//	camera[0] = temp[now];
	camera[0].prp= temp[now].prp;
	camera[0].vrp = temp[now].vrp ;
	camera[0].vup=temp[now].vup ;
	camera[0].fov_y = temp[now].fov_y;
}
void keyboard(unsigned char key, int x, int y) {
	glm::mat4 rt;
//	if(now==VIEW_WORLD)
	switch (key) {
	case 'f'://폴리곤타입
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 'l'://폴리곤타입
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
		now = VIEW_WORLD;
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '1':
		save();
		now = VIEW_WORLD;
		load();
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		glutPostRedisplay();
		break;

	case '2'://카메라선택
		save();
		now = VIEW_WHEEL;
		load();
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		glutPostRedisplay();
		break;
	case '3'://카메라 선택
		save();
		now = VIEW_CAR;
		load();
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		glutPostRedisplay();
		break;

	case 'i':
		glutPostRedisplay();
		break;

	case 'q'://x축으로 전진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(0.2, 0.0, 0.0) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;
	case 'a'://후진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(-0.2, 0.0, 0.0) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;
	case 'w'://z축으로 전진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(0.0, 0.0, 0.2) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;
	case 's'://후진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(0.0, 0.0, -0.2) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;
	case 'e'://y축으로 전진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(0.0, 0.2, 0.0) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;
	case 'd'://후진
		if (now == VIEW_WORLD) {
			camera[0].vrp = glm::vec3(0.0, -0.2, 0.0) + camera[0].vrp;
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
			glutPostRedisplay();
		}
		break;

	case '4':	//x축회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '7'://역회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*-2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '5'://z축회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '8'://z축 역회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*-2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '6'://y축회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '9'://y축역회전
		rt = glm::rotate(glm::mat4(1.0f), TO_RADIAN*-2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		camera[0].prp = glm::vec3(rt*glm::vec4(camera[0].prp, 1.0f));
		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '+'://줌인
		if (now == VIEW_CAR) {
			float size = (camera[0].prp.x - camera[0].vrp.x)*(camera[0].prp.x - camera[0].vrp.x)
				+ (camera[0].prp.y - camera[0].vrp.y)*(camera[0].prp.y - camera[0].vrp.y)
				+ (camera[0].prp.z - camera[0].vrp.z)*(camera[0].prp.z - camera[0].vrp.z);
			size = (float)sqrt(size);//단위벡터를 구하기위해 크기를 구한다.
			glm::vec4 unit = glm::vec4(camera[0].vrp - camera[0].prp, 1.0f)*glm::vec4(1.0f / size);//단위벡터
			if (size>1) {
				camera[0].prp = glm::vec3(glm::vec4(camera[0].prp, 1.0f) + unit);
				ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);

			}
		}
		else {
			camera[0].fov_y *= 0.9f;
			if (camera[0].fov_y < 0.1f)camera[0].fov_y = 0.1;
	
		}
		ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	case '-'://아웃
		if (now == VIEW_CAR) {
			float size = (camera[0].prp.x - camera[0].vrp.x)*(camera[0].prp.x - camera[0].vrp.x)
				+ (camera[0].prp.y - camera[0].vrp.y)*(camera[0].prp.y - camera[0].vrp.y)
				+ (camera[0].prp.z - camera[0].vrp.z)*(camera[0].prp.z - camera[0].vrp.z);
			size = (float)sqrt(size);//단위벡터를 구하기위해 크기를 구한다.
			glm::vec4 unit = glm::vec4(camera[0].vrp - camera[0].prp, 1.0f)*glm::vec4(1.0f / size);//단위벡터

			camera[0].prp = glm::vec3(glm::vec4(camera[0].prp, 1.0f) - unit);
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);

		}
		else {
			camera[0].fov_y *= 1.11f;
			if (camera[0].fov_y > 150)camera[0].fov_y = 150;
		}
		ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
		break;
	

	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}

}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		wheel_angle += FRONT_WHEEL_ROLL_ANGLE_DELTA;

		if (now == VIEW_WHEEL) {
			camera[0].prp = glm::vec3(wheel_angle / (3.14*3.4) + 1.7, 0.0f, 0.0f);
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		}
		else temp[VIEW_WHEEL].prp = glm::vec3(wheel_angle / (3.14*3.4) + 1.7, 0.0f, 0.0f);
		if (box_scale < 3.0f)box_scale += 0.2f;
		else box_scale = 0.2f;
		break;
	case GLUT_KEY_DOWN:
		wheel_angle -= FRONT_WHEEL_ROLL_ANGLE_DELTA;

		if (now == VIEW_WHEEL) {
			camera[0].prp = glm::vec3(wheel_angle / (3.14*3.4f) + 3, 0.0f, 0.0f);
			ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
			ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		}
		else temp[VIEW_WHEEL].prp = glm::vec3(wheel_angle/(3.14*3.4f)+2, 0.0f, 0.0f);

		if (box_scale >= 0.2f)box_scale -= 0.2f;
		else box_scale = 3.0f;
		break;
	case GLUT_KEY_LEFT:
		wheel_angle2 += FRONT_WHEEL_ROLL_ANGLE_DELTA;
		break;
	case GLUT_KEY_RIGHT:
		wheel_angle2 -= FRONT_WHEEL_ROLL_ANGLE_DELTA;
		break;

	}
	glutPostRedisplay();
}

#define CAM_ROT_SENSITIVITY			0.25f

void motion(int x, int y) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_tmp;
	float delx, dely;

	if (leftbutton_pressed &&now !=1) {
		delx = (float)(x - prevx), dely = -(float)(y - prevy);
		prevx = x, prevy = y;

		mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
		mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY*delx*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
		mat4_tmp = glm::translate(mat4_tmp, -camera[0].vrp);

		camera[0].prp = glm::vec3(mat4_tmp*glm::vec4(camera[0].prp, 1.0f));
		camera[0].vup = glm::vec3(mat4_tmp*glm::vec4(camera[0].vup, 0.0f));

		vec3_tmp = glm::cross(camera[0].vup, camera[0].vrp - camera[0].prp);
		mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
		mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY*dely*TO_RADIAN, vec3_tmp);
		mat4_tmp = glm::translate(mat4_tmp, -camera[0].vrp);

		camera[0].prp = glm::vec3(mat4_tmp*glm::vec4(camera[0].prp, 1.0f));
		camera[0].vup = glm::vec3(mat4_tmp*glm::vec4(camera[0].vup, 0.0f));

		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON)) {
		if (state == GLUT_DOWN) {
			leftbutton_pressed = 1;
	//		glutPostRedisplay();
			prevx = x; prevy = y;
		}
		else if (state == GLUT_UP) {
			leftbutton_pressed = 0;
		}
	}
	else if ((button == GLUT_RIGHT_BUTTON)) {
		if (state == GLUT_DOWN) {
			camera[now].flag_move_mode = 1;
			prevx = x; prevy = y;
		}
		else if (state == GLUT_UP)
			camera[now].flag_move_mode = 0;
	}
}

void reshape(int width, int height) {
	camera[0].aspect_ratio = (float)width / height;

	ProjectionMatrix[0] = glm::perspective(camera[0].fov_y*TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

	camera[1].aspect_ratio = camera[0].aspect_ratio; // for the time being ...

	ProjectionMatrix[1] = glm::perspective(camera[1].fov_y*TO_RADIAN, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
	ViewProjectionMatrix[1] = ProjectionMatrix[1] * ViewMatrix[1];

	glutPostRedisplay();
}

void cleanup(void) {
	free_axes();
	free_path();

	free_geom_obj(GEOM_OBJ_ID_CAR_BODY);
	free_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);
	free_geom_obj(GEOM_OBJ_ID_CAR_NUT);
	free_geom_obj(GEOM_OBJ_ID_CAR_BODY);
//	free_geom_obj(GEOM_OBJ_ID_COW);
	free_geom_obj(GEOM_OBJ_ID_TEAPOT);
	free_geom_obj(GEOM_OBJ_ID_BOX);
}
/*********************************  END: callbacks *********************************/

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
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

void prepare_geometry(void) {
	prepare_axes(); 
	prepare_path();

	prepare_geom_obj(GEOM_OBJ_ID_CAR_BODY, "Data/car_body_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_WHEEL, "Data/car_wheel_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_NUT, "Data/car_nut_triangles_v.txt", GEOM_OBJ_TYPE_V);
//	prepare_geom_obj(GEOM_OBJ_ID_COW, "Data/cow_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_TEAPOT, "Data/teapot_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_BOX, "Data/box_triangles_v.txt", GEOM_OBJ_TYPE_V);
}

void initialize_scene(void) {
	prepare_geometry();
	initialize_camera();

}

void initialize_OpenGL(void) {
	// initialize the 0th camera.
	camera[0].prp = glm::vec3(0.0f, 0.0f, 100.0f);
	
	camera[0].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[0].vup = glm::vec3(0.0f, 1.0f, 0.0f);
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	camera[0].vup = glm::vec3(ViewMatrix[0][0].y, ViewMatrix[0][1].y, ViewMatrix[0][2].y); // in this example code, make vup always equal to the v direction.
	temp[0].prp = camera[0].prp;
	temp[0].vup = camera[0].vup;
	temp[0].vrp = camera[0].vrp;

	temp[1].prp = glm::vec3(3.0f, 0.0f, 0.0f);;
	temp[1].vrp = glm::vec3(10000.0f, 0.0f, 0.0f);;
	temp[1].vup = temp[0].vup;

	temp[2].vup = temp[0].vup;
	temp[2].vrp = glm::vec3(46.8,0,-52.0);
	temp[2].prp= glm::vec3(20.0f, 10.0f, 20.0f);;
#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 0", ViewMatrix[0]);
#endif

	camera[0].fov_y = 15.0f;
	camera[0].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[0].near_clip = 0.1f;
	camera[0].far_clip = 500.0f;
	camera[0].zoom_factor = 1.0f; // will be used for zoomming in and out.
								  //initialize the 1st camera.
	for (int i = 0; i < 3; i++)
	{

		temp[i].fov_y = 15.0f;
		temp[i].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
		temp[i].near_clip = 0.1f;
		temp[i].far_clip = 500.0f;
		temp[i].zoom_factor = 1.0f; // will be used for zoomming in and out.
	}
	camera[1].prp = glm::vec3(0.0f, 50.0f, 0.0f);
	camera[1].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[1].vup = glm::vec3(0.0f, 0.0f, 1.0f);
	ViewMatrix[1] = glm::lookAt(camera[1].prp, camera[1].vrp, camera[1].vup);
	camera[1].vup = glm::vec3(ViewMatrix[1][0].y, ViewMatrix[1][1].y, ViewMatrix[1][2].y); // in this example code, make vup always equal to the v direction.

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 1", ViewMatrix[1]);
#endif

	camera[1].fov_y = 16.0f;
	camera[1].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[1].near_clip = 0.1f;
	camera[1].far_clip = 100.0f;
	camera[1].zoom_factor = 1.0f; // will be used for zoomming in and out.

	now = VIEW_WORLD;

	glClearColor(35 / 255.0f, 155 / 255.0f, 86 / 255.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	initialize_scene();
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

void print_message(const char * m) {
	fprintf(stdout, "%s\n\n", m);
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
	char program_name[64] = "Sogang CSE4170 4.7.3.Hier_Car_Model_Driver_GLSL";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: 'q,a', 'w,s', 'e,d', '4,7','5,8','7,9','+','-','1,2,3', four arrows, 'ESC'",
											"    - Mouse used: LClick and move" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}

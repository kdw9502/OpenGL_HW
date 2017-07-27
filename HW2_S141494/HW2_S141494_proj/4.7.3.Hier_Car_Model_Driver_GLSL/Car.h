#define WHEEL_NUT_RADIUS				(1.7f - 0.5f)
#define WHEEL_NUT_Z_OFFSET				1.0f

#define FRONT_WHEEL_ROLL_ANGLE_DELTA	5.0f
#define FRONT_WHEEL_TURN_ANGLE_DELTA	2.5f
#define FRONT_WHEEL_TURN_ANGLE_MAX		45.0f
#define BODY_YAW_SENSITIVITY			0.1f

struct _CAR_STATUS {
	float body_yaw_angle;
	float front_wheel_roll_angle;
	float front_wheel_turn_angle;
	glm::mat4 Matrix_CAR_WHEEL_ROTATE;
	int flag_body_yaw_mode;
} car_status;

void update_car_body_transformation(void) {
	ModelMatrix_CAR_BODY = glm::rotate(glm::mat4(1.0f),
		TO_RADIAN*car_status.body_yaw_angle, glm::vec3(0.0f, 1.0f, 0.0f));

	ModelMatrix_CAR_BODY_INVERSE = glm::rotate(glm::mat4(1.0f),
		-TO_RADIAN*car_status.body_yaw_angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void update_car_front_wheel_rotate_matrix(void) {
	car_status.Matrix_CAR_WHEEL_ROTATE = glm::rotate(glm::mat4(1.0f),
		TO_RADIAN*car_status.front_wheel_turn_angle, glm::vec3(0.0f, 1.0f, 0.0f));

	car_status.Matrix_CAR_WHEEL_ROTATE = glm::rotate(car_status.Matrix_CAR_WHEEL_ROTATE,
		TO_RADIAN*car_status.front_wheel_roll_angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void initialize_car(void) {
	car_status.body_yaw_angle = 0.0f;
	update_car_body_transformation();
	if (camera_type == CAMERA_DRIVER) {

#ifdef USE_set_ViewMatrix_for_driver2
		set_ViewMatrix_for_driver2();
#else
		set_ViewMatrix_for_driver();
#endif
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	}

	car_status.front_wheel_roll_angle = 0.0f;
	car_status.front_wheel_turn_angle = 0.0f;
	update_car_front_wheel_rotate_matrix();
}

void draw_wheel_and_nut() {
	glUniform3f(loc_primitive_color, 0.000f, 0.808f, 0.820f); // color name: DarkTurquoise
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw wheel

	for (int i = 0; i < 5; i++) {
		ModelMatrix_CAR_NUT = glm::rotate(ModelMatrix_CAR_WHEEL, TO_RADIAN*72.0f*i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix_CAR_NUT = glm::translate(ModelMatrix_CAR_NUT, glm::vec3(WHEEL_NUT_RADIUS, 0.0f, WHEEL_NUT_Z_OFFSET));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_NUT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glUniform3f(loc_primitive_color, 0.690f, 0.769f, 0.871f); // color name: LightSteelBlue
		draw_geom_obj(GEOM_OBJ_ID_CAR_NUT); // draw i-th nut
	}
}

void draw_car(void) {
	glUniform3f(loc_primitive_color, 0.498f, 1.000f, 0.831f); // color name: Aquamarine
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY); // draw body

	glLineWidth(5.0f);
	draw_axes(); // draw MC axes of body
	glLineWidth(1.0f);

	ModelMatrix_CAR_DRIVER = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_DRIVER = glm::rotate(ModelMatrix_CAR_DRIVER, TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_DRIVER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw camera frame at driver seat
	glLineWidth(1.0f);

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, 4.5f));
	ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 0

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, 4.5f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 1

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, -4.5f));
	ModelMatrix_CAR_WHEEL *= car_status.Matrix_CAR_WHEEL_ROTATE;
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 2

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, -4.5f));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 3
}

int prevx, prevy;
unsigned int leftbutton_pressed = 0;
typedef struct {
	glm::vec3 prp, vrp, vup; // in this example code, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
	int flag_move_mode ;
} Camera;
typedef enum {
	VIEW_WORLD, VIEW_WHEEL, VIEW_CAR
} VIEW_MODE;
#define NUMBER_OF_CAMERAS 2

Camera camera[NUMBER_OF_CAMERAS];
Camera temp[NUMBER_OF_CAMERAS];


glm::mat4 ViewProjectionMatrix[NUMBER_OF_CAMERAS], ViewMatrix[NUMBER_OF_CAMERAS], ProjectionMatrix[NUMBER_OF_CAMERAS];
glm::mat4 ModelViewProjectionMatrix;
VIEW_MODE now;

void set_ViewMatrix_for_world_viewer(void) {
	now = VIEW_WORLD;
}

void set_ViewMatrix_for_driver(void) {
	now = VIEW_WHEEL;
}

 
#define CAMER_ROTATION_RADIUS		50.0f
void initialize_camera(void) {
	 now= VIEW_WORLD;

}


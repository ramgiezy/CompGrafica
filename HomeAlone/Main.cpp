#include <iostream>
#include <cmath>
#include <math.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0, rotRodDer = 0, rotCodIzq = 0, rotCodDer = 0;
float rotTorso = 0, rotNeck = 0;
float Ypaint = 0, Zpaint = 0, rotPaint = 0;

float Xdog = -120.0f, Zdog = -100.0f, rotDog = 0;
float Xspid = 30, Zspid = 0, rotSpid = 180;

float Xboy = 0, Yboy = 0, Zboy = 0, rotBoy = 0;

float rotDoor = 0;


#define MAX_FRAMES 9
int i_max_steps = 50; //190
int i_curr_steps = 0;
int i_curr_steps2 = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotRodDer;
	float rotCodIzq;
	float rotCodDer;
	float rotTorso;
	float rotNeck;
	float rotInc;
	float rotInc2;
	float rotInc3;
	float rotInc4;
	float rotInc5;
	float rotInc6;

}FRAME;

typedef struct _frame2
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotBoy;
	float rotInc;

}FRAME2;

FRAME KeyFrame[MAX_FRAMES];
FRAME2 KeyFrame2[MAX_FRAMES];
int FrameIndex = 7;			//introducir datos
bool play = false;
bool pPlay = false;
bool play2 = false;
bool play3 = false;
int playIndex = 0;
int playIndex2 = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;





void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;

	rotCodIzq = KeyFrame[0].rotCodIzq;
	rotCodDer = KeyFrame[0].rotCodDer;

	rotTorso = KeyFrame[0].rotTorso;

	rotNeck = KeyFrame[0].rotNeck;

}

void resetElements2(void)
{
	Xboy = KeyFrame2[0].posX;
	Yboy = KeyFrame2[0].posY;
	Zboy = KeyFrame2[0].posZ;

	rotBoy = KeyFrame2[0].rotBoy;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotCodIzq - KeyFrame[playIndex].rotCodIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotCodDer - KeyFrame[playIndex].rotCodDer) / i_max_steps;

	KeyFrame[playIndex].rotInc5 = (KeyFrame[playIndex + 1].rotTorso - KeyFrame[playIndex].rotTorso) / i_max_steps;
	KeyFrame[playIndex].rotInc6 = (KeyFrame[playIndex + 1].rotNeck - KeyFrame[playIndex].rotNeck) / i_max_steps;
}

void interpolation2(void)
{

	KeyFrame2[playIndex2].incX = (KeyFrame2[playIndex2 + 1].posX - KeyFrame2[playIndex2].posX) / i_max_steps;
	KeyFrame2[playIndex2].incY = (KeyFrame2[playIndex2 + 1].posY - KeyFrame2[playIndex2].posY) / i_max_steps;
	KeyFrame2[playIndex2].incZ = (KeyFrame2[playIndex2 + 1].posZ - KeyFrame2[playIndex2].posZ) / i_max_steps;

	KeyFrame2[playIndex2].rotInc = (KeyFrame2[playIndex2 + 1].rotBoy - KeyFrame2[playIndex2].rotBoy) / i_max_steps;

}

int daytime = 0;

bool duckS0 = true, duckS1 = false, duckS2 = false, duckS3 = false;
float duckX = 0.0f, duckZ = 0.0f, duckRot = 0.0f, bd = 0.0f;
float incDX = 0.5f, incDZ = 0.5f;

bool ballS0 = false, ballS1 = false, ballS2 = false, ballS3 = false;
float ballX = 0.0f, ballY = 0.0f, ballZ = 0.0f, ballIncX = 0.0f, ballIncZ = 0.0f;
bool playBall = false;
float topBall = 0.0f;

bool birdS0 = true, birdS1 = true;
float birdX = 0.0f, birdZ = 0.0f, birdRot = 0.0f;

//camera controls
bool recorrido = false;
bool recorridoStart = false;
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f;
int cameraState = 0;
GLfloat cameraRX = 0.0f, cameraRY = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Home Alone", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	//Model House((char*)"Models/fullHouse/fullHouse.obj");
	Model House((char*)"Models/fullHouseV2/house.obj");
	Model Duck((char*)"Models/animables/duck.obj");
	Model Bird((char*)"Models/animables/bird.obj");
	Model Ball((char*)"Models/animables/ball.obj");
	Model Bucket((char*)"Models/cubetaObj/bucket.obj");
	//Model Floor((char*)"Models/piso/floor.obj");
	/*
	Model Rhead((char*)"Models/robber/head.obj");
	Model RlArm((char*)"Models/robber/leftArm.obj");
	Model RlLeg((char*)"Models/robber/leftLeg.obj");
	Model RrArm((char*)"Models/robber/rightArm.obj");
	Model RrForeA((char*)"Models/robber/rightForeArm.obj");
	Model RrLeg((char*)"Models/robber/rightLeg.obj");
	Model Rtorso((char*)"Models/robber/torso.obj");
	*/
	Model Boy((char*)"Models/boy/boy.obj");
	Model Dog((char*)"Models/dog1/shiba.obj");
	Model Spider((char*)"Models/spider/spider.obj");
	Model Door((char*)"Models/front door/frontDoor.obj");
	// Build and compile our shader program

	//Inicialización de KeyFrames

	posX = 0.0f;
	posY = 0.0f;
	posZ = 0.0f;

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotCodIzq = 0;
		KeyFrame[i].rotCodDer = 0;
		KeyFrame[i].rotTorso = 0;
		KeyFrame[i].rotNeck = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotInc2 = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotInc4 = 0;
		KeyFrame[i].rotInc5 = 0;
		KeyFrame[i].rotInc6 = 0;
	} 
	//animación del ladron
	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;

	KeyFrame[1].posX = 0;
	KeyFrame[1].posY = 2;
	KeyFrame[1].posZ = -10;
	KeyFrame[1].rotTorso = -10;
	KeyFrame[1].rotNeck = 20;

	KeyFrame[2].posX = 0;
	KeyFrame[2].posY = -1;
	KeyFrame[2].posZ = -11;
	KeyFrame[2].rotTorso = -30;
	KeyFrame[2].rotNeck = 20;
	KeyFrame[2].rotCodIzq = 180;
	KeyFrame[2].rotCodDer = 90;
	KeyFrame[2].rotRodIzq = 135;
	KeyFrame[2].rotRodDer = 0;

	KeyFrame[3].posX = -0;
	KeyFrame[3].posY = -2;
	KeyFrame[3].posZ = -18;
	KeyFrame[3].rotTorso = -45;
	KeyFrame[3].rotNeck = 20;
	KeyFrame[3].rotCodIzq = 90;
	KeyFrame[3].rotCodDer = 45;
	KeyFrame[3].rotRodIzq = 90;
	KeyFrame[3].rotRodDer = 30;

	KeyFrame[4].posX = 0;
	KeyFrame[4].posY = -5;
	KeyFrame[4].posZ = -22;
	KeyFrame[4].rotTorso = -60;
	KeyFrame[4].rotNeck = 20;
	KeyFrame[4].rotCodIzq = 90;
	KeyFrame[4].rotCodDer = 45;
	KeyFrame[4].rotRodIzq = 45;
	KeyFrame[4].rotRodDer = 30;

	KeyFrame[5].posX = 0;
	KeyFrame[5].posY = -6;
	KeyFrame[5].posZ = -23;
	KeyFrame[5].rotTorso = -85;
	KeyFrame[5].rotNeck = 35;
	KeyFrame[5].rotCodIzq = 95;
	KeyFrame[5].rotCodDer = 50;
	KeyFrame[5].rotRodIzq = 50;
	KeyFrame[5].rotRodDer = 40;

	KeyFrame[6].posX = 0;
	KeyFrame[6].posY = -6;
	KeyFrame[6].posZ = -23;
	KeyFrame[6].rotTorso = -90;
	KeyFrame[6].rotNeck = 0;
	KeyFrame[6].rotCodIzq = 0;
	KeyFrame[6].rotCodDer = -90;
	KeyFrame[6].rotRodIzq = 0;
	KeyFrame[6].rotRodDer = 0;

	KeyFrame[7].posX = 0;
	KeyFrame[7].posY = -6;
	KeyFrame[7].posZ = -23;
	KeyFrame[7].rotTorso = -90;
	KeyFrame[7].rotNeck = 0;
	KeyFrame[7].rotCodIzq = 0;
	KeyFrame[7].rotCodDer = -90;
	KeyFrame[7].rotRodIzq = 0;
	KeyFrame[7].rotRodDer = 0;
	
	//animacion del niño
	KeyFrame2[0].posX = 0;
	KeyFrame2[0].posY = 0;
	KeyFrame2[0].posZ = 0;
	KeyFrame2[0].rotBoy = 0;

	KeyFrame2[1].posX = 0;
	KeyFrame2[1].posY = 0;
	KeyFrame2[1].posZ = 0;
	KeyFrame2[1].rotBoy = 90;

	KeyFrame2[2].posX = 25;
	KeyFrame2[2].posY = 0;
	KeyFrame2[2].posZ = 0;
	KeyFrame2[2].rotBoy = 90;

	KeyFrame2[3].posX = 25;
	KeyFrame2[3].posY = 0;
	KeyFrame2[3].posZ = 0;
	KeyFrame2[3].rotBoy = 0; //giro

	KeyFrame2[4].posX = 25;
	KeyFrame2[4].posY = 5;
	KeyFrame2[4].posZ = -20;
	KeyFrame2[4].rotBoy = 0; //escaleras

	KeyFrame2[5].posX = 22;
	KeyFrame2[5].posY = 5;
	KeyFrame2[5].posZ = -20;
	KeyFrame2[5].rotBoy = -90; //giro y mira

	KeyFrame2[6].posX = 25;
	KeyFrame2[6].posY = 18;
	KeyFrame2[6].posZ = -70;
	KeyFrame2[6].rotBoy = 0; //sigue
	/*
	KeyFrame2[7].posX = 25;
	KeyFrame2[7].posY = 15;
	KeyFrame2[7].posZ = -30;
	KeyFrame2[7].rotBoy = 0; */

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/day/right.tga");
	faces.push_back("SkyBox/day/left.tga");
	faces.push_back("SkyBox/day/top.tga");
	faces.push_back("SkyBox/day/bottom.tga");
	faces.push_back("SkyBox/day/back.tga");
	faces.push_back("SkyBox/day/front.tga");

	
	// Load textures
	vector<const GLchar*> faces2;
	faces2.push_back("SkyBox/night/right.tga");
	faces2.push_back("SkyBox/night/left.tga");
	faces2.push_back("SkyBox/night/top.tga");
	faces2.push_back("SkyBox/night/bottom.tga");
	faces2.push_back("SkyBox/night/back.tga");
	faces2.push_back("SkyBox/night/front.tga");

	GLuint cubemapTexture;

	//Arreglo de los skyboxes

	vector<const GLchar*> mysky[] = { faces,faces2 };

	cubemapTexture = TextureLoading::LoadCubemap(mysky[0]);
	
	// Load textures
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("images/cabinet.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);
	
	// Specular map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/cabinet.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "tex1"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "tex2"), 1);


	GLuint texture3, texture4;
	glGenTextures(1, &texture3);
	glGenTextures(1, &texture4);

	int textureWidth2, textureHeight2, nrChannels2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image2;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image2 = stbi_load("images/cabinetFront.jpg", &textureWidth2, &textureHeight2, &nrChannels2, 0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image2);

	// Specular map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image2 = stbi_load("images/cabinetFront.jpg", &textureWidth2, &textureHeight2, &nrChannels2, 0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth2, textureHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image2);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "tex3"), 2);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "tex4"), 3);
	

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f); //32
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		if (daytime == 0) {
			// Directional light
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		}
		else {
			// Directional light
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		}


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Bind specular map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp
		glm::mat4 model(1);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.0f, -7.5f, 130.0f));
		model = glm::scale(model, glm::vec3(3.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.0f, -7.5f, 130.0f));
		model = glm::scale(model, glm::vec3(3.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(15.0f, 10.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 6, 36);

		// Bind diffuse map
		glBindTexture(GL_TEXTURE_2D, texture3);

		// Bind specular map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);


		glBindVertexArray(VAO);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		 
		//House
		view = camera.GetViewMatrix();
		model = glm::mat4(1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		House.Draw(lightingShader);
		/*
		//Bucket
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(20.0f, 20.0f, 35.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(Zpaint, Ypaint, 0.0f));
		model = glm::rotate(model, glm::radians(rotPaint), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bucket.Draw(lightingShader); */
		/*
		//glm::mat4 tmp = glm::mat4(1.0f); //Temp
		//ladron
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		tmp = model = glm::translate(model, glm::vec3(10.0f, 2.0f, 25.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		tmp = model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(rotTorso), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Rtorso.Draw(lightingShader);
		//Pierna Izq
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.6f, 7.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RlLeg.Draw(lightingShader);
		
		//Pierna Der         //no sirve
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(-1.2f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer+20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RrLeg.Draw(lightingShader);
		
		//Brazo derecho
		view = camera.GetViewMatrix();
		//model = glm::translate(tmp, glm::vec3(-0.6f, 12.4f, 1.7f));
		model = glm::translate(tmp, glm::vec3(-1.7f, 12.4f, -0.6f));
		model = glm::rotate(model, glm::radians(-rotCodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RrArm.Draw(lightingShader);
		//antebrazo derecho
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, 2.7f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RrForeA.Draw(lightingShader);
		//Brazo Izquierdo
		view = camera.GetViewMatrix();
		//model = glm::translate(tmp, glm::vec3(0.2f, 11.8f, -1.7f));
		model = glm::translate(tmp, glm::vec3(1.7f, 11.8f, 0.2f));
		model = glm::rotate(model, glm::radians(-rotCodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RlArm.Draw(lightingShader);
		//Cabeza
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(0.0f, 13.1f, 0.5f));
		model = glm::rotate(model, glm::radians(-rotNeck), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Rhead.Draw(lightingShader);
		*/
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(Zdog, -6.0f, Xdog));
		model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Dog.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-39.0f, 4.0f, 43.0f));
		model = glm::rotate(model, glm::radians(rotDoor), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Door.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(Zspid, -5.0f, Xspid));
		model = glm::rotate(model, glm::radians(rotSpid), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Spider.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(47.0f, 19.0f, 25.0f));
		model = glm::translate(model, glm::vec3(Zboy, Yboy, Xboy));
		model = glm::rotate(model, glm::radians(rotBoy), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Boy.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(230.0f, -3.0f, 30.0f));
		model = glm::translate(model, glm::vec3(duckX, 0.0f, duckZ));
		//model = glm::translate(model, glm::vec3(Zboy, Yboy, Xboy));
		model = glm::rotate(model, glm::radians(duckRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Duck.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-50.0f, -9.0f, -80.0f));
		model = glm::translate(model, glm::vec3(ballX, ballY, ballZ));
		//model = glm::translate(model, glm::vec3(Zboy, Yboy, Xboy));
		//model = glm::rotate(model, glm::radians(rotBoy), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(birdX, 150.0f, birdZ));
		//model = glm::translate(model, glm::vec3(Zboy, Yboy, Xboy));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-birdRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Bird.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		/*
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		} */
		glBindVertexArray(0);

			// Draw skybox as last
			glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
			SkyBoxshader.Use();
			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
			glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			

			// skybox cube
			glBindVertexArray(skyboxVAO);
			cubemapTexture = TextureLoading::LoadCubemap(mysky[daytime]); //Esto es necesario para que el skybox cambie dinamicamente
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // Set depth function back to default
		
		



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{

	//Movimiento del personaje
	if (pPlay)
	{
		if (Zpaint >= -4.0f) {
			Zpaint -= 0.2f;
			Ypaint = (Zpaint + 1.73f)*(Zpaint + 1.73f) - 3.0f;
			rotPaint -= 4.5f;
		}
		else {
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;

			//play = true;
			//Zpaint = 0;
			//Ypaint = 0;
			pPlay = false;
		}
	}

	if (play)
	{
		//printf("reproduciendo");
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;
			rotRodDer += KeyFrame[playIndex].rotInc2;

			rotCodIzq += KeyFrame[playIndex].rotInc3;
			rotCodDer += KeyFrame[playIndex].rotInc4;

			rotTorso += KeyFrame[playIndex].rotInc5;

			rotNeck += KeyFrame[playIndex].rotInc6;

			i_curr_steps++;
		}

	}

	if (play2) {
		//printf("play2 ya es true");
		if (rotDoor > -90.0f) {
			rotDoor -= 1.0f;
		}
		else {
			play2 = false;
		}
	}

	if (play3)
	{
		//printf("reproduciendo 3");
		if (i_curr_steps2 >= i_max_steps) //end of animation between frames?
		{
			playIndex2++;
			if (playIndex2 > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex2 = 0;
				play3 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps2 = 0; //Reset counter
								  //Interpolation
				interpolation2();
			}
		}
		else
		{
			//Draw animation
			
			Xboy += KeyFrame2[playIndex2].incX;
			Yboy += KeyFrame2[playIndex2].incY;
			Zboy += KeyFrame2[playIndex2].incZ;

			

			rotBoy += KeyFrame2[playIndex2].rotInc;

			//printf("%2.1f   **   ", rotBoy);

			i_curr_steps2++;
		}

	}

	if (playBall) {
		if (ballS0 == false) {
			ballIncX = 2 * (double)rand() / (double)RAND_MAX - 1;
			ballIncZ = 2 * (double)rand() / (double)RAND_MAX - 1;
			topBall = 8;
			ballS0 = true;
		}
		if (topBall > 0) {
			ballX += ballIncX;
			ballZ += ballIncZ;
			if (ballY < topBall && ballS1 == false) {
				ballY += 1.0f;
				//printf("Y: %d", ballY);
			}
			else if (ballS1 == false) ballS1 = true;
			else if (ballY > 0 && ballS1 == true) {
				ballY -= 0.5f;
			}
			else {
				topBall -= 2;
				ballS1 = false;
			}
		}
		else {
			ballS0 = false;
			playBall = false;
		}
		/*
		if (ballS0 == false && ballS1 == false && ballS2 == false && ballS3 == false) {
			ballIncX = (double)rand() / (double)RAND_MAX - 1;
			ballIncZ = (double)rand() / (double)RAND_MAX - 1;
			ballS0 = true;
		}
		if (ballS0 == true) {
			ballX += ballIncX;
			ballZ += ballIncZ;
			ballY = (ballZ * tan(glm::radians(70.0f))) - ((9.8f*ballZ*ballZ) / (100 * cos(glm::radians(70.0f))*cos(glm::radians(70.0f)))); //1.22173
			//printf("Y: %d ", ballY);
			if (ballY <= 0) {
				ballS0 = false;
				ballS1 = true;
			}
		} else if (ballS1 == true) {
			ballX += ballIncX;
			ballZ += ballIncZ;
			ballY = (ballZ * tan(glm::radians(70.0f))) - ((9.8f*ballZ*ballZ) / (50 * cos(glm::radians(70.0f))*cos(glm::radians(70.0f))));
			if (ballY <= 0) {
				ballS1 = false;
				ballS2 = true;
			}
		}
		else if (ballS2 == true) {
			ballX += ballIncX;
			ballZ += ballIncZ;
			ballY = (ballZ * tan(glm::radians(70.0f))) - ((9.8f*ballZ*ballZ) / (25 * cos(glm::radians(70.0f))*cos(glm::radians(70.0f))));
			if (ballY <= 0) {
				ballS2 = false;
				ballS3 = true;
			}
		}
		else if (ballS3 == true) {
			ballX += ballIncX;
			ballZ += ballIncZ;
			ballY = (ballZ * tan(glm::radians(70.0f))) - ((9.8f*ballZ*ballZ) / (5 * cos(glm::radians(70.0f))*cos(glm::radians(70.0f))));
			if (ballY <= 0) {
				ballS3 = false;
				playBall = false;
			}
		} */
	}

	//movimiento del perro
	if (Zdog == -100.0f && Xdog < 140.0f) {
		Xdog += 1.0f;
		rotDog = 0.0f;
	}
	else if (Xdog == 140.0f && Zdog < 140.0f) {
		Zdog += 1;
		rotDog = 90.0f;
	}
	else if (Zdog == 140.0f && Xdog > -120.0f) {
		Xdog -= 1;
		rotDog = 180.0f;
	}
	else if (Xdog == -120.0f && Zdog > -100.0f) {
		Zdog -= 1;
		rotDog = -90.0f;
	}

	//movimiento de araña
	if (Zspid == 0.0f && Xspid > -30.0f) {
		Xspid -= 1.0f;
		rotSpid = 180.0f;
	}
	else if (Xspid == -30.0f && Zspid < 70.0f) {
		Zspid += 1;
		rotSpid = 90.0f;
	}
	else if (Zspid == 70.0f && Xspid < 50.0f) {
		Xspid += 1;
		rotSpid = 0.0f;
	}
	else if (Xspid == 50.0f && Zspid > 0.0f) {
		Zspid -= 1;
		rotSpid = -90.0f;
	}
	/*
	//movimiento pato
	if (duckS0 == true) {
		if (duckZ <= 0.0f) {
			duckS0 = false;
			duckS1 = true;
			bd = duckX;
		}
			duckZ -= 1;
			duckX = -duckZ + bd;	
	} else if (duckS1 == true) {
		if (duckX >= 5.0f) {
			duckS1 = false;
			duckS2 = true;
			bd = 5 + duckZ;
		}
		duckZ += 1;
		duckX = duckZ + bd;
	}
	else if (duckS2 == true) {
		if (duckZ >= 10.0f) {
			duckS2 = false;
			duckS3 = true;
			bd = duckX - 10.0f;
		}
		duckZ += 1;
		duckX = -duckZ + bd;
	}
	else if (duckS3 == true) {
		if (duckX <= 0.0f) {
			duckS3 = false;
			duckS0 = true;
			bd = duckZ;
		}
		duckZ -= 1;
		duckX = duckZ + bd;
	} */

	//movimiento de pato
	if (duckX >= 12.0f || duckX <=-20.0f) {
		incDX = -incDX;
	}
	if (duckZ >= 70.0f || duckZ <= -30.0f) {
		incDZ = -incDZ;
	}
	duckX += incDX;
	duckZ += incDZ;
	if (duckRot == 360.0f) duckRot = 0.0f;
	duckRot += 1.0f;

	//movimiento de pajaro
	if (birdS0 == true) {
		birdZ += 1;
		birdX = sqrt(40000 - (birdZ*birdZ));
		if (birdZ == 200) {
			birdS0 = false;
			birdS1 = true;
		}
	} else if (birdS1 == true) {
		birdZ -= 1;
		birdX = -sqrt(40000 - (birdZ*birdZ));
		if (birdZ == -200) {
			birdS1 = false;
			birdS0 = true;
		}
	}
	if (birdRot == 360.0f) birdRot = 0;
	birdRot += 0.45f;
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_1])
	{
		if (pPlay == false && (FrameIndex > 1))
		{
			Ypaint = 0;
			Zpaint = 0;
			rotPaint = 0;
			pPlay = true;

		}
		else
		{
			play = false;
			//pPlay = false;
		}

		if (playBall == false) playBall = true;

	}

	if (keys[GLFW_KEY_2])
	{
		//printf("animacion 2");
		if (play2 == false) {
			//printf("play 2 es false");
			rotDoor = 0.0f;
			play2 = true;
		}
		
	}

	if (keys[GLFW_KEY_3])
	{
		if (play3 == false && (FrameIndex > 1))
		{
			Xboy = 0;
			Yboy = 0;
			Zboy = 0;
			rotBoy = 0;

			
			resetElements2();
			//First Interpolation
			interpolation2();

			play3 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0; 
		}
		else
		{
			play3 = false;
			//pPlay = false;
		}

	}

	if (keys[GLFW_KEY_R]) {
		if (recorrido == false) recorrido = true;
		else {
			recorrido = false;
			recorridoStart = false;
		}
	}
	
	if (keys[GLFW_KEY_T])
	{
		if (daytime == 0)
		{
			//printf("daytime es true");
			daytime = 1;
		}
		else
		{
			//printf("daytime es false");
			daytime = 0;
		}
		
	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (recorrido == false) {
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		GLfloat xOffset = xPos - lastX;
		GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

		lastX = xPos;
		lastY = yPos;

		cameraRY = yOffset;


		camera.ProcessMouseMovement(xOffset, yOffset);
	}
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	

	if (recorrido == false) {
		// Camera controls
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		{
			camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);

		}

		if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime + 0.4f);


		}

		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		{
			camera.ProcessKeyboard(LEFT, deltaTime + 0.4f);


		}

		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		{
			camera.ProcessKeyboard(RIGHT, deltaTime + 0.4f);
		}

	}
	else {
		if (recorridoStart == false) {
			camera=(glm::vec3(-150.0f, 3.0f, -45.0f));
			camera.ProcessMouseMovement(0, -cameraRY);
			//printf(" %d %d ", cameraRX, cameraRY);
			//PosIni(-95.0f, 1.0f, -45.0f);
			cameraX = 0.0f;
			cameraY = 0.0f;
			cameraZ = 0.0f;
			cameraState = 0;
			recorridoStart = true;
		}
		else {
			if (cameraState == 0) {
				camera.ProcessMouseMovement(320.0f, 0.0f);
				cameraState = 1;
			}
			if (cameraZ < 785 && cameraState==1) {
				//printf(" %d ", cameraZ);
				camera.ProcessKeyboard(RIGHT, deltaTime + 0.7f);
				cameraZ += 0.8;
				camera.ProcessMouseMovement(-1.5f, 0.0f);
			}
			else if (cameraState == 1) cameraState = 2;
			else if (cameraX < 25 && cameraState == 2) {
				camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);
				camera.ProcessMouseMovement(0.0f, 0.05f);
				cameraX += 0.8f;
				cameraZ = 0;
			}
			else if (cameraZ < 25 && cameraState == 2) {
				camera.ProcessKeyboard(RIGHT, deltaTime + 0.4f);
				cameraZ += 1;
				//cameraState = 4;
			}
			else if (cameraState == 2) {
				cameraState = 3;
				cameraZ = 0;
				cameraX = 0;
			}
			else if (cameraX < 15 && cameraState == 3) {
				camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);
				cameraX += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraState == 3) {
				cameraState = 4;
				cameraZ = 0;
				cameraX = 0;
			}
			else if (cameraZ < 30 && cameraState == 4) {
				camera.ProcessKeyboard(LEFT, deltaTime + 0.4f);
				cameraZ += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraX < 20 && cameraState == 4) {
				camera.ProcessKeyboard(BACKWARD, deltaTime + 0.4f);
				cameraX += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraState == 4) {
				camera.ProcessMouseMovement(0.0f, 250.0f);
				cameraState = 5;
				cameraZ = 0;
				cameraX = 0;

				cameraY = 0;
			}
			else if (cameraY < 12 && cameraState == 5) {
				camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);
				cameraY += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraState == 5) {
				camera.ProcessMouseMovement(0.0f, -250.0f);
				cameraState = 6;
				cameraZ = 0;
				cameraX = 0;
				cameraY = 0;
			}
			else if (cameraX < 25 && cameraState == 6) {
				camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);
				cameraX += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraZ < 30 && cameraState == 6) {
				camera.ProcessKeyboard(RIGHT, deltaTime + 0.4f);
				cameraZ += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraY < 55 && cameraState == 6) {
				camera.ProcessKeyboard(BACKWARD, deltaTime + 0.4f);
				cameraY += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraState == 6) {
				camera.ProcessMouseMovement(0.0f, -250.0f);
				cameraState = 7;
				cameraZ = 0;
				cameraX = 0;

				cameraY = 0;
			}
			else if (cameraY < 12 && cameraState == 7) {
				camera.ProcessKeyboard(FORWARD, deltaTime + 0.4f);
				cameraY += 0.8f;
				//cameraZ = 0;
			}
			else if (cameraState == 7) {
				camera.ProcessMouseMovement(0.0f, 250.0f);
				cameraState = 8;
				cameraZ = 0;
				cameraX = 0;

				cameraY = 0;
			}
			else if (cameraZ < 10 && cameraState == 8) {
				camera.ProcessKeyboard(LEFT, deltaTime + 0.4f);
				cameraZ += 0.8f;
				//cameraZ = 0;
			}
			else {
				cameraState = 0;
				recorridoStart = false;
			}
		}
	}



}
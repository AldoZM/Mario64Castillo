/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* 09 - Animación
*
* 
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>
#include "GameObject.h"

#include <irrKlang.h>
using namespace irrklang;

// Max number of bones
#define MAX_RIGGING_BONES 100
#define MAX_PILLARS 10

#define MAX_CONSOLES 11

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera1st(glm::vec3(0.0f, 0.0f, 0.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));
Camera* activeCamera;


// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
////////////// Animacion 1
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;
////////////// Animacion 2
float deltaTime_a2 = 0.0f;
float lastFrame_a2 = 0.0f;
float elapsedTime_a2 = 0.0f;

////////////// Animacion 3
float deltaTime_a3 = 0.0f;
float lastFrame_a3 = 0.0f;
float elapsedTime_a3 = 0.0f;

////////////// Animacion 4
float deltaTime_a4 = 0.0f;
float lastFrame_a4 = 0.0f;
float elapsedTime_a4 = 0.0f;
////////////// Animacion de los Toads //////////////////////////////////////////////

////////////// Animacion bob
float deltaTime_R1 = 0.0f;
float lastFrame_R1 = 0.0f;
float elapsedTime_R1 = 0.0f;

glm::vec3 playerPosition(0.0f, 4.5f, 0.0f); // Posicion del personaje
glm::vec3 forwardView(0.0f, 0.0f, 1.0f); // Movimiento hacia adelante
glm::vec3 camera1stPersonOffset(0.0f, 2.0f, -1.0f); // Cambiar posision en Y
glm::vec3 camera3rdPersonOffset(0.0f, 4.0f, -5.0f);
glm::vec3 textInfoOffset(0.0f, 0.0f, 0.81f);

float     scaleV = 0.1f;
float     scaleH = 0.005f;
float     rotateCharacter = 0.0f;

// Shaders
Shader* ourShader;
Shader* cubemapShader;
Shader* mLightsShader;
Shader* staticShader;
Shader* wavesShader;
Shader* waterfallShader;

// Carga la información del modelo
Model* castle;
Model* castleTransparent;
Model* pillar;
Model* toad;
Model* yoshi;
Model* yoshiRojo;
Model* bob;
Model* player;
Model* mario;
Model* cascada;
Model* lago1;
Model* valla;
// Nuevos modelos
Model* mesa;
Model* sensor;
Model* sillon;
Model* ventana;
Model* OXXO;
Model* LittleCessar;
Model* Cangre;



Model* librero;
Model* computadora;
Model* silla;
Model* root;
Model* xbox;
Model* xbox360;

Model* n64;
Model* ps1;
Model* wii;
Model* psp;
Model* ps2;

Model* gameBoy;
Model* snes;
Model* atari;
Model* gameCube;
Model* nes;


Model* textInfo[11];
GameObject* gameObjectsPillars[MAX_PILLARS];
GameObject* moveObject;
GameObject* atariGameObject;
GameObject* consoles[MAX_CONSOLES];


enum INDEX_CONSOLE {ATARI, NES, SNES, PS1, N64, PS2, GAMECUBE, PSP, XBOX360, WII, XBOXSX};
// Cubemap
CubeMap* mainCubeMap;

// Materiales
Material material;
Material oro;
Material silver;
Material plastic;

// Light gLight;
std::vector<Light> gLights;

// Pose inicial del modelo


glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];

//Animacion 2
glm::mat4 gBones_a2[MAX_RIGGING_BONES]; 

//Animacion 3
glm::mat4 gBones_a3[MAX_RIGGING_BONES]; 

//Animacion 4
glm::mat4 gBones_a4[MAX_RIGGING_BONES]; 

///////Animacion bob////////////////////////////////////////////////

//Animacion bob
glm::mat4 gBones_R1[MAX_RIGGING_BONES]; 

glm::mat4 projection; // Variable usada para guardar la matriz de proyecciòn
glm::mat4 view;  // Variable usada para guardar la matriz de vista(camara)
glm::mat4 model; // Variable usada para guardar la matriz de las transformaciones del modelo

//////// Animacion 1
float	fps = 0.0f;
int		keys = 0;
int		animationCount = 0;
//////// Animacion 2
float	fps_a2 = 0.0f;
int		keys_a2 = 0;
int		animationCount_a2 = 0;

//////// Animacion 3
float	fps_a3 = 0.0f;
int		keys_a3 = 0;
int		animationCount_a3 = 0;

//////// Animacion 4
float	fps_a4 = 0.0f;
int		keys_a4 = 0;
int		animationCount_a4 = 0;

///////Animacion de los bob////////////////////////////////////////////////

//////// Animacion bob
float	fps_R1 = 0.0f;
int		keys_R1 = 0;
int		animationCount_R1 = 0;
/*
*/
float elapsedTime2 = 0.0f;

float proceduralTime = 0.0f;
float wavesTime = 0.0f; // Variable de tiempo para el agua
float waterfallTime = 0.0f; // Variable de tiempo para la cascada

float distance; // Variable usada para guardar la distancia entre la camara y un modelo
float minimalDistanceSounds;

const char* soundPath;
Model* textPathc;

float lightDirectionX;
float lightDirectionY;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

int minimalDistanceAudio = 3;
int minimalDistanceTransforms = 8;

bool disableSounds = false;
bool disableText = false;
bool pressE = false;

std::vector <glm::vec3> consoleFinalPositions; // Arreglo para la posición final de las consolas
std::vector <bool> boolFinalPositions;

int selectedConsole;

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	oro.ambient = glm::vec4(0.24725f, 0.1995f, 0.0745, 1.0f);
	oro.diffuse = glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f);
	oro.specular = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f);
	oro.transparency = 1.0f;

	silver.ambient = glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f);
	silver.diffuse = glm::vec4(0.50754f, 0.50754f, 0.50754f, 1.0f);
	silver.specular = glm::vec4(0.508273f, 0.508273f, 0.508273f, 1.0f);
	silver.transparency = 1.0f;

	plastic.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 0.25f);
	plastic.diffuse = glm::vec4(0.55f, 0.55f, 0.55f, 0.25f);
	plastic.specular = glm::vec4(0.70f, 0.70f, 0.70f, 0.25f);
	plastic.transparency = 1.0f;

	std::vector<glm::vec3> pillarsPositions; // Arreglo para guardar las posiciones de los pilares
	std::vector <std::string> soundEffectsPaths; // Arreglo para guardar las direcciones de los efectos de sonido
	std::vector <std::string > textPaths;
	std::vector <glm::vec3> consolePositions; // Arreglo para guardar las posiciones de las consolas
	

	// Inicialización de GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto museo de videojuegos", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	ourShader = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs"); // Shader para el personaje
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs"); // Shader para el cubemap
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs"); // Shader sin fuentes de iluminaciòn
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs"); // Shader para multiples fuentes de iluminaciòn
	wavesShader = new Shader("shaders/wavesAnimation.vs", "shaders/wavesAnimation.fs"); // Shader para la el agua
	waterfallShader = new Shader("shaders/waterfallAnimation.vs", "shaders/waterfallAnimation.fs"); // Shader para la cascada

	// Máximo número de huesos: 100
	ourShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	castle = new Model("models/CastilloMaquetado2.fbx");
	castleTransparent = new Model("models/CastleTransparentElements.fbx");
	pillar = new Model("models/pilarGriego.fbx");
	yoshi = new Model("models/Yoshi.fbx");
	yoshiRojo = new Model("models/YoshiRojo.fbx");
	toad = new Model("models/Toad.fbx");
	bob = new Model("models/Bob-ombs.fbx");
	player = new Model("models/Yoshi.fbx");
	mario = new Model("models/Mario.fbx");
	cascada = new Model("models/cascada.fbx");
	lago1 = new Model("models/AguaRefinado.fbx");
	valla = new Model("models/CordonMuseo.fbx");


	Cangre = new Model("models/Cangre.fbx");
	LittleCessar = new Model("models/Little_Ceassar.fbx");
	OXXO = new Model("models/OXXO.fbx");
	mesa = new Model("models/mesa.fbx");
	sensor = new Model("models/sensor.fbx");
	sillon = new Model("models/sillon.fbx");

	librero = new Model("models/librero.fbx");
	computadora = new Model("models/computadora.fbx");
	silla = new Model("models/silla.fbx");
	root = new Model("models/rooflamp.fbx");
	xbox = new Model("models/XboxSerieX.fbx");
	xbox360 = new Model("models/xbox360.fbx");

	n64 = new Model("models/N64.fbx");
	ps1 = new Model("models/PS1.fbx");
	wii = new Model("models/Wii.fbx");
	psp = new Model("models/PSP.fbx");
	ps2 = new Model("models/PS2.fbx");

	gameBoy = new Model("models/GameBoy.fbx");
	snes = new Model("models/SNES.fbx");
	atari = new Model("models/Atari.fbx");
	gameCube = new Model("models/Gamecube.fbx");
	nes = new Model("models/NES.fbx");


	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.png",
		"textures/cubemap/01/negx.png",
		"textures/cubemap/01/posy.png",
		"textures/cubemap/01/negy.png",
		"textures/cubemap/01/posz.png",
		"textures/cubemap/01/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);

	// time, arrays
	player->SetPose(0.0f, gBones);

	fps = (float)player->getFramerate();
	keys = (int)player->getNumFrames();
	////Animacion 2
	yoshi->SetPose(0.0f, gBones_a2);

	fps_a2 = (float)yoshi->getFramerate();
	keys_a2 = (int)yoshi->getNumFrames();
	////Animacion 3
	toad->SetPose(0.0f, gBones_a3);

	fps_a3 = (float)toad->getFramerate();
	keys_a3 = (int)toad->getNumFrames();
	////Animacion 4
	mario->SetPose(0.0f, gBones_a4);

	fps_a4 = (float)mario->getFramerate();
	keys_a4 = (int)mario->getNumFrames();

	////Animacion bob
	

	bob->SetPose(0.0f, gBones_a4);

	fps_R1 = (float)bob->getFramerate();
	keys_R1 = (int)bob->getNumFrames();
	/*
	*/
	camera1st.Position = playerPosition; // Posicion de la camara en 1ra persona
	camera1st.Position += camera1stPersonOffset; // Alto respecto al personaje
	camera1st.Front = forwardView;

	camera3rd.Position = playerPosition; // Posiciòn de la camara en 3era persona
	camera3rd.Position += camera3rdPersonOffset; // Alto respecto al personaje
	camera3rd.Front = forwardView;

	// Lights configuration

	Light light;
	light.Position = glm::vec3(4.0f, 20.0f, 45.0f);
	light.Direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	light.Color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);

	gLights.push_back(light);

	Light light02;
	light02.Position = glm::vec3(-5.0f, 20.0f, 53.0f);
	light02.Direction = glm::vec3(1.0f, 0.0f, 0.0f);
	light02.Color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);

	gLights.push_back(light02);
	

	// Configuración de la camara
	activeCamera = &camera3rd; // Se activa la camara en tercera persona

	// Configuración de los archivos a cargar
	pillarsPositions.push_back(glm::vec3(4.0f, 4.25f, 41.0f));//z y x //centro-atras izquierda
	pillarsPositions.push_back(glm::vec3(-5.0f, 4.25f, 40.0f));//centro-atras derecha
	pillarsPositions.push_back(glm::vec3(2.5f, 4.25f, 53.0f));//centro-frente izquierda
	pillarsPositions.push_back(glm::vec3(-3.5f, 4.25f, 53.8f));//centro-frente, derecha
	pillarsPositions.push_back(glm::vec3(10.1f, 4.25f, 61.5f));//izwuierda enfrente
	pillarsPositions.push_back(glm::vec3(-11.0f, 4.25f, 61.2f));//enfrente,x negativa
	pillarsPositions.push_back(glm::vec3(12.5f, 4.25f, 53.5f));//izquierda atras
	pillarsPositions.push_back(glm::vec3(-14.5f, 4.25f, 54.0f));//atras, xnegativo
	pillarsPositions.push_back(glm::vec3(8.0f, 9.25f, 64.0f));
	pillarsPositions.push_back(glm::vec3(-5.0f, 9.25f, 61.0f));

	soundEffectsPaths.push_back("audio/atariSound.mp3");
	soundEffectsPaths.push_back("audio/NESsound.mp3");
	soundEffectsPaths.push_back("audio/Xbox360Sound.mp3");
	soundEffectsPaths.push_back("audio/SNESSsound.mp3");
	soundEffectsPaths.push_back("audio/ps2.mp3");
	soundEffectsPaths.push_back("audio/N64Sound.mp3");
	soundEffectsPaths.push_back("audio/ps1.mp3");
	soundEffectsPaths.push_back("audio/GameCubeSound.mp3");
	soundEffectsPaths.push_back("audio/gb.mp3");
	soundEffectsPaths.push_back("audio/orb.mp3");


	textPaths.push_back("models/textatari.fbx");
	textPaths.push_back("models/textnes.fbx");
	textPaths.push_back("models/textXbox360.fbx");
	textPaths.push_back("models/textsnes.fbx");
	textPaths.push_back("models/textps2.fbx");
	textPaths.push_back("models/textN64.fbx");
	textPaths.push_back("models/textpsp.fbx");
	textPaths.push_back("models/textgamecube.fbx");
	textPaths.push_back("models/textplay1.fbx");
	textPaths.push_back("models/textwii.fbx");
	textPaths.push_back("models/textxbox.fbx");

	consolePositions.push_back(glm::vec3(5.5f, 4.25f, 50.0f)); // posición atari
	consolePositions.push_back(glm::vec3(-5.0f, 4.25f, 40.0f));//centro-atras derecha
	consolePositions.push_back(glm::vec3(2.5f, 4.25f, 53.0f));//centro-frente izquierda
	consolePositions.push_back(glm::vec3(-3.5f, 4.25f, 53.8f));//centro-frente, derecha
	consolePositions.push_back(glm::vec3(10.1f, 4.25f, 61.5f));//izwuierda enfrente
	consolePositions.push_back(glm::vec3(-11.0f, 4.25f, 61.2f));//enfrente,x negativa
	consolePositions.push_back(glm::vec3(12.5f, 4.25f, 53.5f));//izquierda atras
	consolePositions.push_back(glm::vec3(-14.5f, 4.25f, 54.0f));//atras, xnegativo
	consolePositions.push_back(glm::vec3(8.0f, 9.25f, 64.0f));
	consolePositions.push_back(glm::vec3(-5.0f, 9.25f, 61.0f));
	consolePositions.push_back(glm::vec3(-5.0f, 9.25f, 61.0f));

	consoleFinalPositions.push_back(glm::vec3(4.0f, 4.9f, 41.0f)); // Posicion final de Atari
	consoleFinalPositions.push_back(glm::vec3(-5.0f, 5.3f, 40.0f)); // Posicion final de NES
	consoleFinalPositions.push_back(glm::vec3(-3.5f, 5.15f, 53.8f)); // Posicion final de SNES
	consoleFinalPositions.push_back(glm::vec3(-14.7f, 4.80f, 58.0f)); // Posicion final de PS1
	consoleFinalPositions.push_back(glm::vec3(-8.0f, 5.0f, 59.0f)); // Posicion final de n64
	consoleFinalPositions.push_back(glm::vec3(10.0, 5.0f, 61.3f)); // Posicion final de PS2
	consoleFinalPositions.push_back(glm::vec3(-14.5f, 4.85f, 54.0f)); // Posicion final de GAMECUBE
	consoleFinalPositions.push_back(glm::vec3(12.5f, 5.2f, 53.5f)); // Posicion final de PSP
	consoleFinalPositions.push_back(glm::vec3(2.5f, 5.5f, 53.0f)); // Posicion final de XBOX360
	consoleFinalPositions.push_back(glm::vec3(-11.0f, 5.0f, 60.8f)); // Posicion final de WII
	consoleFinalPositions.push_back(glm::vec3(13.8f, 4.9f, 56.0f));  // Posicion final de XBOXSX
	//consoleFinalPositions.push_back();

	for (size_t i = 0; i < MAX_CONSOLES; i++)
	{
		boolFinalPositions.push_back(false);
	}
	
	glm::vec3 position;
	glm::vec3 consolePosition;
	std::string soundEffectPath;
	
	for (size_t i = 0; i < MAX_PILLARS; i++)
	{
		position = pillarsPositions.at(i);
		soundEffectPath = soundEffectsPaths.at(i);
		textInfo[i] = new Model(textPaths.at(i));
		gameObjectsPillars[i] = new GameObject(position, soundEffectPath); // Iniciamos los game objects
	}

	for (size_t i = 0; i < MAX_CONSOLES; i++) // Ciclo para iniciar las posiciones de las consolas
	{
		consolePosition = consolePositions.at(i);
		consoles[i] = new GameObject(consolePosition);
	}
	// RGBa (Red, Green, Blue and Alpha)
	SoundEngine->play2D("audio/gta.mp3");
	// SoundEngine->play2D("audio/KonosubaAudio.mp3");

	minimalDistanceAudio = 5;

	// atariGameObject = new GameObject(position);
	
	return true;
}



void SetLightUniformInt(Shader* shader, const char* propertyName, size_t lightIndex, int value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setInt(uniformName.c_str(), value);
}
void SetLightUniformFloat(Shader* shader, const char* propertyName, size_t lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setFloat(uniformName.c_str(), value);
}
void SetLightUniformVec4(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec4(uniformName.c_str(), value);
}
void SetLightUniformVec3(Shader* shader, const char* propertyName, size_t lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << propertyName;
	std::string uniformName = ss.str();

	shader->setVec3(uniformName.c_str(), value);
}


bool Update() {

///// Animacion 1
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	elapsedTime += deltaTime;
	elapsedTime2 += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		// Configuración de la pose en el instante t
		player->SetPose((float)animationCount, gBones);
		elapsedTime = 0.0f;

	}
///// Animacion 2
	float currentFrame_a2 = (float)glfwGetTime();
	deltaTime_a2 = currentFrame_a2 - lastFrame_a2;
	lastFrame_a2 = currentFrame_a2;

	elapsedTime_a2 += deltaTime_a2;
	if (elapsedTime_a2 > 1.0f / fps_a2) {
		animationCount_a2++;
		if (animationCount_a2 > keys_a2 - 1) {
			animationCount_a2 = 0;
		}
		// Configuración de la pose en el instante t
		yoshi->SetPose((float)animationCount_a2, gBones_a2);
		elapsedTime_a2 = 0.0f;

	}
///// Animacion 3
	float currentFrame_a3 = (float)glfwGetTime();
	deltaTime_a3 = currentFrame_a3 - lastFrame_a3;
	lastFrame_a3 = currentFrame_a3;

	elapsedTime_a3 += deltaTime_a3;
	if (elapsedTime_a3 > 1.0f / fps_a3) {
		animationCount_a3++;
		if (animationCount_a3 > keys_a3 - 1) {
			animationCount_a3 = 0;
		}
		// Configuración de la pose en el instante t
		toad->SetPose((float)animationCount_a3, gBones_a3);
		elapsedTime_a3 = 0.0f;

	}
/*
	*/
///// Animacion 4
	float currentFrame_a4 = (float)glfwGetTime();
	deltaTime_a4 = currentFrame_a4 - lastFrame_a4;
	lastFrame_a4 = currentFrame_a4;

	elapsedTime2 += deltaTime; // Tiempo para la tecla de consolas

	elapsedTime_a4 += deltaTime_a4;
	if (elapsedTime_a4 > 1.0f / fps_a4) {
		animationCount_a4++;
		if (animationCount_a4 > keys_a4 - 1) {
			animationCount_a4 = 0;
		}
		// Configuración de la pose en el instante t
		mario->SetPose((float)animationCount_a4, gBones_a4);
		elapsedTime_a4 = 0.0f;

	}
///// Animacion bob
	float currentFrame_R1 = (float)glfwGetTime();
	deltaTime_R1 = currentFrame_R1 - lastFrame_R1;
	lastFrame_R1 = currentFrame_R1;


	elapsedTime_R1 += deltaTime_R1;
	if (elapsedTime_R1 > 1.0f / fps_R1) {
		animationCount_R1++;
		if (animationCount_R1 > keys_R1 - 1) {
			animationCount_R1 = 0;
		}
		// Configuración de la pose en el instante t
		bob->SetPose((float)animationCount_R1, gBones_R1);
		elapsedTime_R1 = 0.0f;

	}
	/*
*/
	

	// Procesa la entrada del teclado o mouse
	processInput(window);

	projection = glm::perspective(glm::radians(activeCamera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f); // Sè define la matriz de perspectiva
	view = activeCamera->GetViewMatrix();

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cubemap (fondo)
	{
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}


	{
		staticShader->use();
		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Activamos el shader del plano
		staticShader->setMat4("view", view);
		staticShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		castle->Draw(*staticShader);
	}

	{
		// Activamos el shader de Phong
		waterfallShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		waterfallShader->setMat4("projection", projection);
		waterfallShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		waterfallShader->setMat4("model", model);

		waterfallShader->setFloat("time", waterfallTime);

		cascada->Draw(*waterfallShader);
		waterfallTime += 0.001;
	}

	{
		// Activamos el shader de Phong
		wavesShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		wavesShader->setMat4("projection", projection);
		wavesShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		wavesShader->setMat4("model", model);

		wavesShader->setFloat("time", wavesTime);

		lago1->Draw(*wavesShader);
		
		wavesTime += 0.001;
	}

	{
		// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, playerPosition); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0035f, 0.0035f, 0.0035f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Dibujamos el modelo
		player->Draw(*ourShader);
	}
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(59.0f, -1.0f, -9.5f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00005f, 0.00005f, 0.00005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_a2);

		// Dibujamos el modelo
		yoshi->Draw(*ourShader);
	}

		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 3.7f, 29.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00005f, 0.00005f, 0.00005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_a3);

		// Dibujamos el modelo
		toad->Draw(*ourShader);
	}
	
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-9.0f, 3.7f, 45.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00005f, 0.00005f, 0.00005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_a3);

		// Dibujamos el modelo
		toad->Draw(*ourShader);
	}
	
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(13.0f, 3.7f, 47.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00005f, 0.00005f, 0.00005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_a3);

		// Dibujamos el modelo
		toad->Draw(*ourShader);
	}
	
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	
	

	
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.5f, 3.5f, 56.5f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(220.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00012f, 0.00012f, 0.00012f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_a4);

		// Dibujamos el modelo
		mario->Draw(*ourShader);
	}
	
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	
	{
			// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		ourShader->setMat4("view", view);
		ourShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 4.0f, 56.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones_R1);

		// Dibujamos el modelo
		bob->Draw(*ourShader);
	}
	
		glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 
	

	mLightsShader->use();

	{
		// Activamos el shader del plano
		mLightsShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (size_t i = 0; i < MAX_PILLARS; i++)
		{
			// Aplicamos transformaciones de proyección y cámara (si las hubiera)
			mLightsShader->setMat4("projection", projection);
			mLightsShader->setMat4("view", view);

			glm::vec3 objectPosition = gameObjectsPillars[i]->getObjectPosition();
			float objectRotationY = gameObjectsPillars[i]->getAngleZ();
			auto distance = glm::length(playerPosition - objectPosition);



			soundPath = gameObjectsPillars[i]->getSoundPathC();
			if (distance < minimalDistanceAudio && !SoundEngine->isCurrentlyPlaying(soundPath) && !disableSounds) {
				SoundEngine->stopAllSounds();
				SoundEngine->play2D(soundPath);

			}
			if (distance < minimalDistanceTransforms) {
				moveObject = gameObjectsPillars[i];
				objectPosition = gameObjectsPillars[i]->getObjectPosition();
			}
			
			if (distance < minimalDistanceAudio && !disableText) {
				model = glm::mat4(1.0f);
				


				//model = glm::translate(model, camera1st.Position); // translate it down so it's at the center of the scene
				//model = glm::rotate(model, glm::radians(rotateCharacter), camera1st.Front);
				model = glm::translate(model, camera1st.Position + glm::vec3(0.0f, 0.0f, 0.81f)); // translate it down so it's at the center of the scene
				//model = glm::translate(model, camera1st.Position + camera1stPersonOffset+ glm::vec3(0.0f, 0.0f, 1.0f)); // translate it down so it's at the center of the scene
				model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				
				model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
				staticShader->setMat4("model", model);
				textInfo[i]->Draw(*staticShader);
			}

			

			model = glm::mat4(1.0f);
			model = glm::translate(model, objectPosition); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(objectRotationY), glm::vec3(0.0, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.4f));	// it's a bit too big for our scene, so scale it down
			mLightsShader->setMat4("model", model);

			mLightsShader->setInt("numLights", (int)gLights.size());
			for (size_t i = 0; i < gLights.size(); ++i) {
				SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
				SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
				SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
				SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
				SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
				SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
			}

			mLightsShader->setVec3("eye", activeCamera->Position);

			mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
			mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
			mLightsShader->setVec4("MaterialSpecularColor", material.specular);
			mLightsShader->setFloat("transparency", material.transparency);

			pillar->Draw(*mLightsShader);
		}
	}

	staticShader->use();
	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa2
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, 4.1f, 56.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		mesa->Draw(*staticShader);
		//mESA 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(7.0f, 4.1f, 59.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		mesa->Draw(*staticShader);

		//Mesa 4
		model = glm::mat4(1.0f);				//-x en sentido de -z    // +z en sentido de -x
		model = glm::translate(model, glm::vec3(-14.7f, 4.1f, 58.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		mesa->Draw(*staticShader);

		//Mesa 3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 4.1f, 59.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(130.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		mesa->Draw(*staticShader);
		// OXXO
		model = glm::mat4(1.0f);
		//										  X  	Z	 Y
		model = glm::translate(model, glm::vec3(70.0f, 6.5f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(20.35f, 20.35f, 20.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		OXXO->Draw(*staticShader);

		// LittleCessar
		model = glm::mat4(1.0f);
		//										  X  	Z	 Y
		model = glm::translate(model, glm::vec3(-70.0f, 10.3f, 41.5f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		LittleCessar->Draw(*staticShader);
		
		// Krustaseo Kascarudo
		model = glm::mat4(1.0f);
		//										  X  	Z	 Y
		model = glm::translate(model, glm::vec3(-70.0f, 5.0f, 20.5f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(110.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		Cangre->Draw(*staticShader);

	}

	staticShader->use();
	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa2
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 6.3f, 43.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00045f, 0.00047f, 0.00047f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		librero->Draw(*staticShader);
	}


	mLightsShader->use(); // Cambiamos a que OpenGL use el shader de multiples iluminaciones
	{
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		if (pressE && !boolFinalPositions.at(PS1) && selectedConsole == PS1) {
			consoles[PS1]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[PS1]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(PS1) - objectPosition);
		if (distance <= 5.0f) {
			model = glm::translate(model, consoleFinalPositions.at(PS1)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(PS1) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}


		
		
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down //y x z
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i){
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);
		ps1->Draw(*mLightsShader);
	}



	mLightsShader->use(); // Cambiamos a que OpenGL use el shader de multiples iluminaciones
	{
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.4f, 5.5f, 34.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 2.0f));	// it's a bit too big for our scene, so scale it down	//y 
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);
		sensor->Draw(*mLightsShader);
	}



	mLightsShader->use(); // Cambiamos a que OpenGL use el shader de multiples iluminaciones
	{
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 4.0f, 42.5f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);
		sillon->Draw(*mLightsShader);
	}

	mLightsShader->use();

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		if (pressE && !boolFinalPositions.at(XBOXSX) && selectedConsole == XBOXSX) {
			consoles[XBOXSX]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[XBOXSX]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(XBOXSX) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(XBOXSX)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(XBOXSX) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}
		
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		xbox->Draw(*mLightsShader);
	}

	mLightsShader->use(); // Activamos el shader de iluminación

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(N64) && selectedConsole == N64) {
			consoles[N64]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[N64]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(N64) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(N64)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(N64) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		n64->Draw(*mLightsShader);
	}

	mLightsShader->use(); // Activamos el shader de iluminación

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//Computadora
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.6f, 5.0f, 58.6f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0002f, 0.0002f, 0.0002f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", silver.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", silver.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", silver.specular);
		mLightsShader->setFloat("transparency", silver.transparency);

		computadora->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//Silla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.0f, 4.5f, 58.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", plastic.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", plastic.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", plastic.specular);
		mLightsShader->setFloat("transparency", plastic.transparency);

		silla->Draw(*mLightsShader);
	}

	mLightsShader->use();

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(XBOX360) && selectedConsole == XBOX360) {
			consoles[XBOX360]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[XBOX360]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(XBOX360) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(XBOX360)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(XBOX360) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(-190.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.2f, 0.1f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		xbox360->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(SNES) && selectedConsole == SNES) {
			consoles[SNES]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[SNES]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(SNES) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(SNES)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(SNES) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		snes->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(GAMECUBE) && selectedConsole == GAMECUBE) {
			consoles[GAMECUBE]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[GAMECUBE]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(GAMECUBE) - objectPosition);
		if (distance <= 2.0f) {
			model = glm::translate(model, consoleFinalPositions.at(GAMECUBE)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(GAMECUBE) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		gameCube->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		if (pressE && !boolFinalPositions.at(ATARI) && selectedConsole == ATARI) {
			consoles[ATARI]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[ATARI]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(ATARI) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(ATARI)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(ATARI) = true;
		}	
		else {
			model = glm::translate(model, objectPosition);
		}
			
		
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.04f, 0.04f));	// it's a bit too big for our scene, so scale it down//x y z
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		atari->Draw(*mLightsShader);
	}


	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(NES) && selectedConsole == NES) {
			consoles[NES]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[NES]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(NES) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(NES)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(NES) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}


		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		nes->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(WII) && selectedConsole == WII) {
			consoles[WII]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[WII]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(WII) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(WII)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(WII) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		wii->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(PSP) && selectedConsole == PSP) {
			consoles[PSP]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[PSP]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(PSP) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(PSP)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(PSP) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}

		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		psp->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		if (pressE && !boolFinalPositions.at(PS2) && selectedConsole == PS2) {
			consoles[PS2]->setObjectPosition(playerPosition + textInfoOffset); // añadir offset
		}
		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		glm::vec3 objectPosition = consoles[PS2]->getObjectPosition();
		auto distance = glm::length(consoleFinalPositions.at(PS2) - objectPosition);
		if (distance <= 1.5f) {
			model = glm::translate(model, consoleFinalPositions.at(PS2)); // translate it down so it's at the center of the scene
			boolFinalPositions.at(PS2) = true;
		}
		else {
			model = glm::translate(model, objectPosition);
		}


		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", material.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", material.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", material.specular);
		mLightsShader->setFloat("transparency", material.transparency);

		ps2->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f); //-3.5f, 4.25f, 53.8f));//centro-frente, derecha
		model = glm::translate(model, glm::vec3(-0.4f, 3.6f, 50.7f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", oro.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", oro.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", oro.specular);
		mLightsShader->setFloat("transparency", oro.transparency);

		valla->Draw(*mLightsShader);
	}


	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f); //-3.5f, 4.25f, 53.8f));//centro-frente, derecha
		model = glm::translate(model, glm::vec3(-0.4f, 20.1, 50.7f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", silver.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", silver.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", silver.specular);
		mLightsShader->setFloat("transparency", silver.transparency);

		root->Draw(*mLightsShader);
	}


	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f); //-3.5f, 4.25f, 53.8f));//centro-frente, derecha
		model = glm::translate(model, glm::vec3(-0.4f, 20.1, 60.7f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));	// it's a bit too big for our scene, so scale it down
		mLightsShader->setMat4("model", model);

		mLightsShader->setInt("numLights", (int)gLights.size());
		for (size_t i = 0; i < gLights.size(); ++i) {
			SetLightUniformVec3(mLightsShader, "Position", i, gLights[i].Position);
			SetLightUniformVec3(mLightsShader, "Direction", i, gLights[i].Direction);
			SetLightUniformVec4(mLightsShader, "Color", i, gLights[i].Color);
			SetLightUniformVec4(mLightsShader, "Power", i, gLights[i].Power);
			SetLightUniformInt(mLightsShader, "alphaIndex", i, gLights[i].alphaIndex);
			SetLightUniformFloat(mLightsShader, "distance", i, gLights[i].distance);
		}

		mLightsShader->setVec3("eye", activeCamera->Position);

		mLightsShader->setVec4("MaterialAmbientColor", silver.ambient);
		mLightsShader->setVec4("MaterialDiffuseColor", silver.diffuse);
		mLightsShader->setVec4("MaterialSpecularColor", silver.specular);
		mLightsShader->setFloat("transparency", silver.transparency);

		root->Draw(*mLightsShader);
	}


	{
		staticShader->use();
		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Activamos el shader del plano
		staticShader->setMat4("view", view);
		staticShader->setMat4("projection", projection);

		// Aplicamos transformaciones del modelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		
		castleTransparent->Draw(*staticShader);
	}


	glUseProgram(0); /// Instruccion para detener el ciclo de renderizado 

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		selectedConsole = -1;
		if (elapsedTime2 > 0.2f) {
			pressE = !pressE;
			elapsedTime2 = 0.0f;
		}

		if (pressE) {
			for (size_t i = 0; i < MAX_CONSOLES; i++)
			{
				glm::vec3 objectPosition = consoles[i]->getObjectPosition();
				auto distance = glm::length(playerPosition - objectPosition);
				if (distance <= 1.5f) {
					selectedConsole = i;
					break;
				}
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
	}


	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera3rd.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera3rd.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Funciòn para mover un objeto en z hacìa el lado negativo
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(0.0f, 0.0f, -0.01f);
		}

		moveObject = NULL;
	}

	// Funciòn para mover un objeto en z hacìa el lado positivo
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(0.0f, 0.0f, 0.01f);
		}

		moveObject = NULL;
	}

	// Funciòn para mover un objeto en x hacìa el lado positivo
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(0.01f, 0.0f, 0.0f);
		}

		moveObject = NULL;
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(-0.01f, 0.0f, 0.0f);
		}

		moveObject = NULL;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->rotateObjectZ(0.1f);
		}

		moveObject = NULL;
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->rotateObjectZ(-0.1f);
		}

		moveObject = NULL;
	}
	

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		
		playerPosition = playerPosition + scaleV * forwardView;
		camera1st.Front = forwardView;
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		playerPosition = playerPosition - scaleV * forwardView;
		camera1st.Front = forwardView;
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
		
		camera1st.Front = forwardView;
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
		
		camera1st.Front = forwardView;
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
	}

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		/*Sirve para refrescar la posición de la camara debido a que la posición del jugador
		se mueve, entonces si estamos en la otra camara los cambios no se ven reflejados en la activación de esta camara*/
		camera1st.Front = forwardView;
		camera1st.ProcessKeyboard(FORWARD, deltaTime);
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
		activeCamera = &camera1st;
	}

	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		
		camera3rd.Position = playerPosition;
		camera3rd.Position += camera3rdPersonOffset;
		activeCamera = &camera3rd;
	}

	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		if (elapsedTime2 > 0.5f) {
			SoundEngine->stopAllSounds();
			disableSounds = !disableSounds;
			elapsedTime2 = 0.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		if (elapsedTime2 > 0.5f) {
			disableText = !disableText;
			elapsedTime2 = 0.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		gLights[0].Direction.x += 0.01f;
		std::cout << glm::to_string(gLights[0].Direction) << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		gLights[0].Direction.x -= 0.01f;
		std::cout << glm::to_string(gLights[0].Direction) << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		gLights[0].Direction.y += 0.01f;
		std::cout << glm::to_string(gLights[0].Direction) << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		gLights[0].Direction.y -= 0.01f;
		std::cout << glm::to_string(gLights[0].Direction) << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		scaleV -= 0.001f;
		if (scaleV <= 0) {
			scaleV = 0.1f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		scaleV += 0.001f;
		if (scaleV >= 0.2f) {
			scaleV = 0.1f;
		}
	}
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera3rd.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera3rd.ProcessMouseScroll((float)yoffset);
}

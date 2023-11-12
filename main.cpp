/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* 09 - Animación
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
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 playerPosition(0.0f, 4.5f, 0.0f); // Posicion del personaje
glm::vec3 forwardView(0.0f, 0.0f, 1.0f); // Movimiento hacia adelante
glm::vec3 camera1stPersonOffset(0.0f, 2.0f, -1.0f);
glm::vec3 camera3rdPersonOffset(0.0f, 4.0f, -5.0f);

float     scaleV = 0.5f;
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
Model* player;
Model* cascada;
Model* lago1;
Model* lago2;
// Nuevos modelos
Model* mesa;
Model* sensor;
Model* sillon;
Model* ventana;

//Modelos carlos
Model* librero;
Model* computadora;
Model* silla;
Model* root;
Model* xbox;

//Modelos aldo
Model* n64;
Model* ps1;
Model* wii;

//Modelos Ernesto
Model* gameBoy;
Model* snes;
Model* atari;
Model* gameCube;
Model* nes;

GameObject* gameObjectsPillars[10];
GameObject* moveObject;

// Cubemap
CubeMap* mainCubeMap;

// Materiales
Material material;

// Light gLight;
std::vector<Light> gLights;

// Pose inicial del modelo
glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];

glm::mat4 projection; // Variable usada para guardar la matriz de proyecciòn
glm::mat4 view;  // Variable usada para guardar la matriz de vista(camara)
glm::mat4 model; // Variable usada para guardar la matriz de las transformaciones del modelo

float	fps = 0.0f;
int		keys = 0;
int		animationCount = 0;

float elapsedTime2 = 0.0f;

float proceduralTime = 0.0f;
float wavesTime = 0.0f; // Variable de tiempo para el agua
float waterfallTime = 0.0f; // Variable de tiempo para la cascada

float distance; // Variable usada para guardar la distancia entre la camara y un modelo
float minimalDistanceSounds;

const char* soundPath;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();

int minimalDistanceAudio = 3;
int minimalDistanceTransforms = 8;

bool disableSounds = false;

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
	std::vector<glm::vec3> pillarsPositions; // Arreglo para guardar las posiciones de los pilares
	std::vector <std::string> soundEffectsPaths; // Arreglo para guardar las direcciones de los efectos de sonido

	// Inicialización de GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FBX Animation with OpenGL", NULL, NULL);
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
	player = new Model("models/player.fbx");
	cascada = new Model("models/cascada.fbx");
	lago1 = new Model("models/AguaRefinado.fbx");

	//Nuevos modelos
	mesa = new Model("models/mesa.fbx");
	sensor = new Model("models/sensor.fbx");
	sillon = new Model("models/sillon.fbx");
	//Modelos Carlos
	librero = new Model("models/librerofinal.fbx");
	//computadora = new Model("models/computadora.fbx");
	silla = new Model("models/silla.fbx");
	root = new Model("models/rooflamp.fbx");
	xbox = new Model("models/XboxSerieX.fbx");
	//Modelos aldo
	n64 = new Model("models/N64.fbx");
	ps1 = new Model("models/PS1.fbx");
	wii = new Model("models/Wii.fbx");

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

	camera1st.Position = playerPosition; // Posicion de la camara en 1ra persona
	camera1st.Position += camera1stPersonOffset; // Alto respecto al personaje
	camera1st.Front = forwardView;

	camera3rd.Position = playerPosition; // Posiciòn de la camara en 3era persona
	camera3rd.Position += camera3rdPersonOffset; // Alto respecto al personaje
	camera3rd.Front = forwardView;

	// Lights configuration

	Light light;
	light.Position = glm::vec3(4.0f, 20.0f, 53.0f);
	light.Color = glm::vec4(0.2f, 0.0f, 0.0f, 1.0f);
	// mainLight.alphaIndex = 13; // àngulo de rotaciòn
	//gLights.push_back(light);

	Light light02;
	light02.Position = glm::vec3(-5.0f, 20.0f, 53.0f);
	light02.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

	soundEffectsPaths.push_back("audio/ps1.mp3");
	soundEffectsPaths.push_back("audio/gba.mp3");
	soundEffectsPaths.push_back("audio/ps2.mp3");
	soundEffectsPaths.push_back("audio/pc.mp3");
	soundEffectsPaths.push_back("audio/gb.mp3");
	soundEffectsPaths.push_back("audio/orb.mp3");
	soundEffectsPaths.push_back("audio/xbox.mp3");
	soundEffectsPaths.push_back("audio/pda.mp3");
	soundEffectsPaths.push_back("audio/ok.mp3");
	soundEffectsPaths.push_back("audio/oof.mp3");

	glm::vec3 position;
	std::string soundEffectPath;
	for (size_t i = 0; i < MAX_PILLARS; i++)
	{
		position = pillarsPositions.at(i);
		soundEffectPath = soundEffectsPaths.at(i);
		gameObjectsPillars[i] = new GameObject(position, soundEffectPath); // Iniciamos los game objects
	}
	// RGBa (Red, Green, Blue and Alpha)
	SoundEngine->play2D("audio/gta.mp3");

	minimalDistanceAudio = 5;
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
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		// Configuración de la pose en el instante t
		player->SetPose((float)animationCount, gBones);
		elapsedTime = 0.0f;

	}
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
		castleTransparent->Draw(*staticShader);
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
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Dibujamos el modelo
		player->Draw(*ourShader);
	}

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

			model = glm::mat4(1.0f);
			model = glm::translate(model, objectPosition); // translate it down so it's at the center of the scene
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//								//ancho	//volumen //altura
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

	}

	staticShader->use();
	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa2
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 6.0f, 42.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		librero->Draw(*staticShader);
	}


	mLightsShader->use(); // Cambiamos a que OpenGL use el shader de multiples iluminaciones
	{
		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);			//z(positivo en +z)   y   x(positivo en -x)
		model = glm::translate(model, glm::vec3(-14.7f, 4.80f, 58.0f)); // translate it down so it's at the center of the scene
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
		glm::mat4 model = glm::mat4(1.0f);
		//										z(positivo sentido -z)		y    x(positivo sentido -x)
		model = glm::translate(model, glm::vec3(13.8f, 4.9f, 56.0f)); // translate it down so it's at the center of the scene
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
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-8.0f, 5.0f, 59.0f)); // translate it down so it's at the center of the scene
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

	mLightsShader->use();

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);//(2.5f, 4.25f, 53.0f));//centro-frente izquierda //6.0f, 5.0f, 58.0f
		model = glm::translate(model, glm::vec3(2.5f, 5.1f, 53.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-190.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

		gameBoy->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f); //-3.5f, 4.25f, 53.8f));//centro-frente, derecha
		model = glm::translate(model, glm::vec3(-3.5f, 5.15f, 53.8f)); // translate it down so it's at the center of the scene
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
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 50.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

		gameCube->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 10.0f, 50.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

		atari->Draw(*mLightsShader);
	}


	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(9.0f, 10.0f, 50.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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

		nes->Draw(*mLightsShader);
	}

	{
		// Aplicamos transformaciones de proyección y cámara (si las hubiera)

		mLightsShader->setMat4("projection", projection);
		mLightsShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		//mesa1
		model = glm::mat4(1.0f); //-3.5f, 4.25f, 53.8f));//centro-frente, derecha
		model = glm::translate(model, glm::vec3(-3.5f, 8.15f, 53.8f)); // translate it down so it's at the center of the scene
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

		wii->Draw(*mLightsShader);
	}
	glUseProgram(0);

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


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

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {}

	// Funciòn para mover un objeto en z hacìa el lado negativo
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(0.0f, 0.0f, -0.01f);
		}

		moveObject = NULL;
	}

	// Funciòn para mover un objeto en z hacìa el lado positivo
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if (moveObject != NULL) {
			moveObject->moveObjectPosition(0.0f, 0.0f, 0.01f);
		}

		moveObject = NULL;
	}


	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		playerPosition = playerPosition + scaleV * forwardView;
		camera1st.Front = forwardView;
		camera1st.ProcessKeyboard(FORWARD, deltaTime);
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		playerPosition = playerPosition - scaleV * forwardView;
		camera1st.Front = forwardView;
		camera1st.ProcessKeyboard(BACKWARD, deltaTime);
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
		
		elapsedTime2 += deltaTime;
		if (elapsedTime2 > 0.5f) {
			SoundEngine->stopAllSounds();
			disableSounds = !disableSounds;
			elapsedTime2 = 0.0f;
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

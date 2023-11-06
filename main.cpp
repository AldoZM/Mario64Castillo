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

// #include <irrKlang.h>
// using namespace irrklang;

// Max number of bones
#define MAX_RIGGING_BONES 100

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void chooseCamera(glm::mat4* projection, glm::mat4* view);
// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera1st(glm::vec3(0.0f, 0.0f, 0.0f));
Camera camera3rd(glm::vec3(0.0f, 0.0f, 0.0f));


// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;


glm::vec3 playerPosition(0.0f, 3.0f, 0.0f); // Posicion del personaje
glm::vec3 forwardView(0.0f, 0.0f, 1.0f); // Movimiento hacia adelante
glm::vec3 camera1stPersonOffset(0.0f, 4.0f,-1.0f);
glm::vec3 camera3rdPersonOffset(0.0f, 4.0f, -5.0f);

glm::vec3 rightView(1.0f, 0.0f, 0.0f);

float     scaleV = 0.005f;
float     scaleH = 0.005f;
float     rotateCharacter = 0.0f;

// Shaders
Shader *ourShader;
Shader *cubemapShader;
Shader *mLightsShader;
Shader *proceduralShader;
Shader *wavesShader;
Shader* staticShader;
Shader* phongShader;

// Carga la información del modelo
Model* castle;
Model* pillar;
Model* player;

// Cubemap
CubeMap *mainCubeMap;

// Materiales
Material material01;

// Light gLight;
std::vector<Light> gLights;
Light    lightMoon;

// Pose inicial del modelo
glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];

float	fps = 0.0f;
int		keys = 0;
int		animationCount = 0;

float proceduralTime = 0.0f;
float wavesTime = 0.0f;

// Audio
// ISoundEngine *SoundEngine = createIrrKlangDevice();

// selección de cámara
bool    thirdPerson = true; // activamos la camara en tercera persona

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
	// Inicialización de GLFW
	//camera.setFront(glm::vec3(0.0f, 0.0f, 0.0f));
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
	ourShader     = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");
	staticShader = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	mLightsShader = new Shader("shaders/11_PhongShaderMultLights.vs", "shaders/11_PhongShaderMultLights.fs");
	proceduralShader = new Shader("shaders/12_ProceduralAnimation.vs", "shaders/12_ProceduralAnimation.fs");
	wavesShader = new Shader("shaders/13_wavesAnimation.vs", "shaders/13_wavesAnimation.fs");

	// Máximo número de huesos: 100
	ourShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	castle = new Model("models/CastleOpaqueElements.fbx");
	pillar = new Model("models/pillar.fbx");
	player = new Model("models/player.fbx");


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
	keys = (int) player->getNumFrames();
	


	camera1st.Position = playerPosition; // Posicion de la camara en 1ra persona
	camera1st.Position += camera1stPersonOffset; // Alto respecto al personaje
	camera1st.Position -= forwardView;
	camera1st.Front = forwardView;

	camera3rd.Position = playerPosition; // Posiciòn de la camara en 3era persona
	camera3rd.Position += camera3rdPersonOffset; // Alto respecto al personaje
	camera3rd.Position -= forwardView;
	camera3rd.Front = forwardView;

	// Lights configuration
	
	Light light01;
	light01.Position = glm::vec3(5.0f, 2.0f, 5.0f);
	light01.Color = glm::vec4(0.2f, 0.0f, 0.0f, 1.0f);
	gLights.push_back(light01);

	Light light02;
	light02.Position = glm::vec3(-5.0f, 2.0f, 5.0f);
	light02.Color = glm::vec4(0.0f, 0.2f, 0.0f, 1.0f);
	gLights.push_back(light02);

	Light light03;
	light03.Position = glm::vec3(5.0f, 2.0f, -5.0f);
	light03.Color = glm::vec4(0.0f, 0.0f, 0.2f, 1.0f);
	gLights.push_back(light03);

	Light light04;
	light04.Position = glm::vec3(-5.0f, 2.0f, -5.0f);
	light04.Color = glm::vec4(0.2f, 0.2f, 0.0f, 1.0f);
	gLights.push_back(light04);
	

	// SoundEngine->play2D("sound/EternalGarden.mp3", true);

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

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cubemap (fondo)
	{
		glm::mat4 projection;
		glm::mat4 view;

		chooseCamera(&projection, &view);

		
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}
	
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection;
		glm::mat4 view;

		chooseCamera(&projection, &view);

		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		castle->Draw(*staticShader);
	}

	glUseProgram(0);



	{
		// Activación del shader del personaje
		ourShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection;
		glm::mat4 view;

		chooseCamera(&projection, &view);


		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, playerPosition); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Dibujamos el modelo
		player->Draw(*ourShader);
	}

	glUseProgram(0);


	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection;
		glm::mat4 view;

		chooseCamera(&projection, &view);
		
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 5.5f, 40.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		pillar->Draw(*staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 5.5f, 40.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		pillar->Draw(*staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.5f, 5.5f, 53.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		pillar->Draw(*staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.5f, 5.5f, 53.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);
		pillar->Draw(*staticShader);
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
		playerPosition = playerPosition + scaleV * forwardView;
		if (thirdPerson) {
			camera3rd.Front = forwardView;
			camera3rd.ProcessKeyboard(FORWARD, deltaTime);
			camera3rd.Position = playerPosition;
			camera3rd.Position += camera3rdPersonOffset;

		}
		else {
			playerPosition = playerPosition + scaleV * forwardView;
			 // camera1st.Front = forwardView;
			camera1st.ProcessKeyboard(FORWARD, deltaTime);
			camera1st.Position = playerPosition;
			camera1st.Position += camera1stPersonOffset;

		}

		std::cout << glm::to_string(playerPosition) << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		playerPosition = playerPosition - scaleV * forwardView;
		if (thirdPerson) {
			// camera3rd.Front = forwardView;
			camera3rd.ProcessKeyboard(BACKWARD, deltaTime);
			camera3rd.Position = playerPosition;
			camera3rd.Position += camera3rdPersonOffset;
		}
		else {
			// camera1st.Front = forwardView;
			camera1st.ProcessKeyboard(BACKWARD, deltaTime);
			camera1st.Position = playerPosition;
			camera1st.Position += camera1stPersonOffset;

		}
		
		std::cout << glm::to_string(playerPosition) << std::endl;
	}
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotateCharacter += 0.5f;
		if (thirdPerson) {
			// camera3rd.Front = forwardView;
			camera3rd.Position = playerPosition;
			camera3rd.Position += camera3rdPersonOffset;
		}
		else {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);
			camera1st.Front = forwardView;
			camera1st.Position = playerPosition;
			camera1st.Position += camera1stPersonOffset;
		}


	}
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		if (thirdPerson) {
			camera3rd.Front = forwardView;
			camera3rd.Position = playerPosition;
			camera3rd.Position += camera3rdPersonOffset;
		}
		else {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			forwardView = glm::vec3(viewVector);
			forwardView = glm::normalize(forwardView);

			camera1st.Front = forwardView;
			camera1st.Position = playerPosition;
			camera1st.Position += camera1stPersonOffset;
		}
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
		

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		
		
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	}

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		/*Sirve para refrescar la posición de la camara debido a que la posición del jugador
		se mueve, entonces si estamos en la otra camara los cambios no se ven reflejados en la activación de esta camara*/
		camera1st.Front = forwardView; 
		camera1st.ProcessKeyboard(FORWARD, deltaTime);
		camera1st.Position = playerPosition;
		camera1st.Position += camera1stPersonOffset;
		thirdPerson = false;
	}
		
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		camera3rd.Front = forwardView;
		camera3rd.ProcessKeyboard(FORWARD, deltaTime);
		camera3rd.Position = playerPosition;
		camera3rd.Position += camera3rdPersonOffset;
		thirdPerson = true;
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

	camera1st.ProcessMouseMovement(xoffset, 0);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera3rd.ProcessMouseScroll((float)yoffset);
}

void chooseCamera(glm::mat4 *projection, glm::mat4 *view) {
	if (thirdPerson) {
		*projection = glm::perspective(glm::radians(camera3rd.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		*view = camera3rd.GetViewMatrix();
	}
	else {
		*projection = glm::perspective(glm::radians(camera1st.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		*view = camera1st.GetViewMatrix();
	}
}
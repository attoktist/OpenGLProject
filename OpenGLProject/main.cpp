//#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
// GLEW нужно подключать до GLFW.
#define GLEW_STATIC
#include <windows.h>
#include <glad\glad.h>
#include <glad\glad.c>





#include <gl\gl.h>   

#include <gl\glu.h>  
//#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <soil.h>
#include <iostream>
#include <time.h>
//#include <ctime>
#include "Shader.h"
#include "Camera.h"
#include "Wall.h"
#include "Light.h"
#include "Model.h"
#include "Particle.h"


#define D2R(x) ((x)*3.141592741f/180)
#define SCR_WIDTH 1366
#define SCR_HEIGHT 768

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouseKey(GLFWwindow* window, int button, int action, int mode);

//камера
Camera camera(glm::vec3(-3.0f, 2.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool leftS = false;

//----------------------------------------------------------------

//void PickingSelect(GLFWwindow *window, glm::mat4 MVP, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, Quad* quads,int n,Shader pickingShader, GLuint transformLocSelectShader, GLuint PickingColorSelectShader)
//{
//	if (leftS)
//	{
//		glDisable(GL_DEPTH_TEST);
//		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		//glUseProgram(pickingShader.Program);
//		pickingShader.Use();
//		
//		
//		//glUniformMatrix4fv(PickingColorSelectShader, 1, GL_FALSE, glm::value_ptr(MVP));
//		glUniformMatrix4fv(transformLocSelectShader, 1, GL_FALSE, glm::value_ptr(MVP));
//		for (int i = 0; i < n; i++)
//		{
//			
//			int r = (quads[i].id & 0x000000FF) >> 0;
//			int g = (quads[i].id & 0x0000FF00) >> 8;
//			int b = (quads[i].id & 0x00FF0000) >> 16;
//			glm::vec4 a = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
//			glUniform4f(PickingColorSelectShader, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
//
//			quads[i].Draw(pickingShader);
//		}
//		
//		// Wait until all the pending drawing commands are really done.
//		// Ultra-mega-over slow ! 
//		// There are usually a long time between glDrawElements() and
//		// all the fragments completely rasterized.
//		glFlush();
//		glFinish();
//		//glReadBuffer(GL_FRONT);
//		//glfwSwapBuffers(window);
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//
//		// Read the pixel at the center of the screen.
//		// You can also use glfwGetMousePos().
//		// Ultra-mega-over slow too, even for 1 pixel, 
//		// because the framebuffer is on the GPU.
//		unsigned char data[4];
//		/*double x, y;
//		glfwGetCursorPos(window, &x, &y);*/
//		
//		glReadPixels(SCR_WIDTH/2, SCR_HEIGHT/2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
//
//		int pickedID =
//			data[0] +
//			data[1] * 256 +
//			data[2] * 256 * 256;
//		std::cout << "r: " << (int)data[0] << " g: " << (int)data[1] << " b: " << (int)data[2] << "\r\n";
//		for (int i = 0; i < n; i++)
//		{
//			if (quads[i].id == pickedID)
//			{
//				if (quads[i].select) quads[i].select = false;
//				else quads[i].select = true;
//			}
//		}
//		leftS = false;
//		glEnable(GL_DEPTH_TEST);
//		glDepthFunc(GL_LESS);
//		//glfwSwapBuffers(window);
//	//	Sleep(5000);
//	}
//}
Wall walls[100];

bool CollisionWall(Wall *walls, int n)
{
	GLfloat xmax, xmin;
	GLfloat ymax, ymin;
	GLfloat zmax, zmin;

	for (int i = 0; i < n; i++)
	{
		xmax = walls[i].x + walls[i].lx + 0.1f;
		xmin = walls[i].x - walls[i].lx - 0.1f;
		ymax = walls[i].y + walls[i].ly+0.1f;
		ymin = walls[i].y - walls[i].ly-0.1f;
		zmax = walls[i].z + walls[i].lz+0.1f;
		zmin = walls[i].z - walls[i].lz-0.1f;

		if ((xmin < camera.Position.x) && (xmax > camera.Position.x) &&
			(ymin < camera.Position.y) && (ymax > camera.Position.y) &&
			(zmin < camera.Position.z) && (zmax > camera.Position.z))
		{
			return true;
		}
	}
	return false;
}

Light light;

GLuint LoadTexture(std::string path_texture)
{
	int widthT, heightT;
	unsigned char* image;
	GLuint texture;
	image = SOIL_load_image(path_texture.c_str(), &widthT, &heightT, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	////Для повторений если текстура закончилась
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//по x
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);//по y

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthT, heightT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "rus");
	glfwInit();
	//Задается минимальная требуемая версия OpenGL.
	//Мажорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	camera.Pitch = -50.0f;
	camera.Yaw = 0.0f;

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLFirstApp", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	std::cout << "Failed to initialize GLEW" << std::endl;
	//	return -1;
	//}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//GLFW_CUR
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	glfwSetMouseButtonCallback(window, mouseKey);

	glEnable(GL_DEPTH_TEST);	
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	
	Shader lightShader("LightVertShader.vert", "LightFragShader.frag");
	Shader AssimpLightShader("3.model_loading.vs", "3.model_loading.fs");
	Shader posLightShader("PositionLightVertShader.vert", "PositionLightFragShader.frag");
	Shader particleShader("ParticleVertShader.vert", "ParticleFragShader.frag");
	//Shader SelectShader("VertSelectShader.vert", "FragSelectShader.frag");
	
	
	//GLint objectColorLoc = glGetUniformLocation(lightShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightShader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(lightShader.Program, "lightPos");
	GLuint transformLocModelLightShader = glGetUniformLocation(lightShader.Program, "model");
	GLuint transformLocMVPLightShader = glGetUniformLocation(lightShader.Program, "MVP");
	GLuint transformLocLightPosLightShader = glGetUniformLocation(lightShader.Program, "lightPos");
	GLuint transformLocLightColorLightShader = glGetUniformLocation(lightShader.Program, "lightColor");
	GLuint transformLocViewPosLightShader = glGetUniformLocation(lightShader.Program, "viewPos");
	//Shader SKShader("SkyBoxVertShader.vert", "SkyBoxFragShader.frag");

	glm::vec3 mod = glm::vec3(3.0f, 2.0f, 2.0f);
	

	float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
	glm::mat4 trans;
	glm::mat4 iden = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	trans = iden;
	 trans = glm::translate(iden, glm::vec3(1.0f, 0.0f, 0.0f));

	  light.SetLight(-10.0f, 3.8f, 10.5f, 0.5f, 0.2f, 0.5f,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(20.0f,20.0f,20.0f),glm::vec3(1.0f,1.0f,1.0f), glm::vec3(1.0f, 1.0f, 1.0f),1.0f,0.07f,0.017f);
	// GLuint transformLocLightShader = glGetUniformLocation(lightShader.Program, "MVP");
	 GLuint postransLightShader = glGetUniformLocation(posLightShader.Program, "MVP");
	
	//GLuint transformLocOurShader = glGetUniformLocation(ourShader.Program, "MVP");
	

	GLuint wall = LoadTexture("Textures/mk.jpg");
	GLuint normal_wall = LoadTexture("Textures/mk_normal1.jpg");
	//GLuint wall = LoadTexture("Textures/wall.jpg");
	//GLuint normal_wall = LoadTexture("Textures/wall_normal.jpg");
	GLuint pol = LoadTexture("Textures/pol.jpg");
	GLuint normal_pol = LoadTexture("Textures/pol_normal.jpg");
	GLuint potolok = LoadTexture("Textures/potolok.jpg");
	GLuint normal_potolok = LoadTexture("Textures/potolok_normal.jpg");
	GLuint fire = LoadTexture("Textures/fire1.jpg");
	
	//walls[0].SetWall(1.0f, 3.0f, 1.0f, 0.1f, 3.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//walls[1].SetWall(-19.0f, 3.0f, 1.0f, 0.1f, 3.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//walls[2].SetWall(-11.0f, 3.0f, -9.0f, 0.1f, 3.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//walls[3].SetWall(9.0f, 3.0f, -9.0f, 0.1f, 3.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//walls[4].SetWall(-0.1f, 9.0f, 1.0f, 0.1f, 10.0f, 10.0f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f), 0.3f, pol, normal_pol);
	//walls[5].SetWall(5.1f, 9.0f, 1.0f, 0.1f, 10.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f), 0.4f, potolok, normal_potolok);


	//стена1
	walls[0].SetWall(1.0f, 3.0f, 1.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[1].SetWall(1.0f, 1.0f, 1.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[2].SetWall(1.0f, 3.0f, 3.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[3].SetWall(1.0f, 1.0f, 3.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[4].SetWall(1.0f, 3.0f, 5.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[5].SetWall(1.0f, 1.0f, 5.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[6].SetWall(1.0f, 3.0f, 7.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[7].SetWall(1.0f, 1.0f, 7.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[8].SetWall(1.0f, 3.0f, 9.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[9].SetWall(1.0f, 1.0f, 9.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[10].SetWall(1.0f, 3.0f, 11.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[11].SetWall(1.0f, 1.0f, 11.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[12].SetWall(1.0f, 3.0f, 13.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[13].SetWall(1.0f, 1.0f, 13.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[14].SetWall(1.0f, 3.0f, 15.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[15].SetWall(1.0f, 1.0f, 15.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[16].SetWall(1.0f, 3.0f, 17.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[17].SetWall(1.0f, 1.0f, 17.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[18].SetWall(1.0f, 3.0f, 19.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[19].SetWall(1.0f, 1.0f, 19.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//стена2
	walls[20].SetWall(-19.0f, 3.0f, 1.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[21].SetWall(-19.0f, 1.0f, 1.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[22].SetWall(-19.0f, 3.0f, 3.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[23].SetWall(-19.0f, 1.0f, 3.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[24].SetWall(-19.0f, 3.0f, 5.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[25].SetWall(-19.0f, 1.0f, 5.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[26].SetWall(-19.0f, 3.0f, 7.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[27].SetWall(-19.0f, 1.0f, 7.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[28].SetWall(-19.0f, 3.0f, 9.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[29].SetWall(-19.0f, 1.0f, 9.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[30].SetWall(-19.0f, 3.0f, 11.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[31].SetWall(-19.0f, 1.0f, 11.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[32].SetWall(-19.0f, 3.0f, 13.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[33].SetWall(-19.0f, 1.0f, 13.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[34].SetWall(-19.0f, 3.0f, 15.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[35].SetWall(-19.0f, 1.0f, 15.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[36].SetWall(-19.0f, 3.0f, 17.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[37].SetWall(-19.0f, 1.0f, 17.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[38].SetWall(-19.0f, 3.0f, 19.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[39].SetWall(-19.0f, 1.0f, 19.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//стена3
	walls[40].SetWall(-20.0f, 3.0f, 0.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[41].SetWall(-20.0f, 1.0f, 0.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[42].SetWall(-20.0f, 3.0f, -2.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[43].SetWall(-20.0f, 1.0f, -2.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[44].SetWall(-20.0f, 3.0f, -4.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[45].SetWall(-20.0f, 1.0f, -4.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[46].SetWall(-20.0f, 3.0f, -6.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[47].SetWall(-20.0f, 1.0f, -6.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[48].SetWall(-20.0f, 3.0f, -8.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[49].SetWall(-20.0f, 1.0f, -8.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[50].SetWall(-20.0f, 3.0f, -10.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[51].SetWall(-20.0f, 1.0f, -10.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[52].SetWall(-20.0f, 3.0f, -12.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[53].SetWall(-20.0f, 1.0f, -12.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[54].SetWall(-20.0f, 3.0f, -14.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[55].SetWall(-20.0f, 1.0f, -14.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[56].SetWall(-20.0f, 3.0f, -16.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[57].SetWall(-20.0f, 1.0f, -16.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[58].SetWall(-20.0f, 3.0f, -18.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[59].SetWall(-20.0f, 1.0f, -18.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//стена4
	walls[60].SetWall(0.0f, 3.0f, 0.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[61].SetWall(0.0f, 1.0f, 0.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[62].SetWall(0.0f, 3.0f, -2.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[63].SetWall(0.0f, 1.0f, -2.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[64].SetWall(0.0f, 3.0f, -4.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[65].SetWall(0.0f, 1.0f, -4.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[66].SetWall(0.0f, 3.0f, -6.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[67].SetWall(0.0f, 1.0f, -6.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[68].SetWall(0.0f, 3.0f, -8.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[69].SetWall(0.0f, 1.0f, -8.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[70].SetWall(0.0f, 3.0f, -10.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[71].SetWall(0.0f, 1.0f, -10.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[72].SetWall(0.0f, 3.0f, -12.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[73].SetWall(0.0f, 1.0f, -12.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[74].SetWall(0.0f, 3.0f, -14.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[75].SetWall(0.0f, 1.0f, -14.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[76].SetWall(0.0f, 3.0f, -16.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[77].SetWall(0.0f, 1.0f, -16.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[78].SetWall(0.0f, 3.0f, -18.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	walls[79].SetWall(0.0f, 1.0f, -18.0f, 0.1f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	
	walls[80].SetWall(-0.1f, 9.0f, 10.0f, 0.1f, 10.0f, 10.0f, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f), 0.3f, pol, normal_pol);
	walls[81].SetWall(4.1f, 9.0f, 10.0f, 0.1f, 10.0f, 10.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.3f, 0.3f, 0.3f), 0.4f, potolok, normal_potolok);




	//Wall wall2(-10.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	//Wall wall1(-10.0f, 0.0f, 10.0f, 1.0f, 1.0f, 1.0f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.1f), 0.1f, wall, normal_wall);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//SkyBox sk(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);

	clock_t newClock, oldClock;
	float deltaTime = 0.001f;
	oldClock = GetTickCount();
	float dt = 0.0f;
	// Игровой цикл
	//bool s = false;
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
	
	//Model thinker("Model/12228_Dog_v1_L2.obj");
	Model thinker("Model/12335_The_Thinker_v3_l2.obj");	
	Model skull("Model/12140_Skull_v3_L2.obj");
	Model kamin("Model/13111_fireplacebricks_v2_l2.obj");
	Model dog("Model/12228_Dog_v1_L2.obj");
	Model sword("Model/moonlight.obj");


	ParticleGenerator   *Particles;

	Particles = new ParticleGenerator(fire,5000);

	while (!glfwWindowShouldClose(window))
	{
		// Проверяем события и вызываем функции обратного вызова.
		newClock = GetTickCount();
		dt = (newClock - oldClock)/10000.0;
		oldClock = newClock;
		
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 MVP = projection*view*trans;
		//PickingSelect(window, MVP,view,projection, quads, 16, SelectShader, transformLocSelectShader, pickingColorID);
		
		// Команды отрисовки здесь
		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					
		angleY += D2R(60.0f)*deltaTime;
		angleZ += D2R(90.0f)*deltaTime;
		
		trans = glm::rotate(iden,  angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		//trans = iden;
		
		//trans = glm::translate(iden, glm::vec3(1.0f, 0.0f, 0.0f));
		//trans = glm::rotate(trans, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		//trans = glm::rotate(trans, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
		
		

		posLightShader.Use();
		glm::mat4 transLight;
		transLight = glm::rotate(iden, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		MVP = projection * view * transLight;
		glUniformMatrix4fv(postransLightShader, 1, GL_FALSE, glm::value_ptr(MVP));
		
		light.Draw(posLightShader);
				
		AssimpLightShader.Use();
		set_light(AssimpLightShader,camera,light);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		AssimpLightShader.setMat4("projection", projection);
		AssimpLightShader.setMat4("view", view);

		glm::mat4 model5 = glm::mat4(1.0f);
		model5 = glm::translate(model5, glm::vec3(0.0f, 1.1f, 19.0f));
		model5 = glm::rotate(model5, D2R(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model5 = glm::rotate(model5, D2R(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// translate it down so it's at the center of the scene
		model5 = glm::scale(model5, glm::vec3(0.009f, 0.009f, 0.009f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model5);
		sword.Draw(AssimpLightShader);

		glm::mat4 model4 = glm::mat4(1.0f);
		model4 = glm::translate(model4, glm::vec3(0.0f, 0.0f, 1.0f));
		model4 = glm::rotate(model4, D2R(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model4 = glm::rotate(model4, D2R(315.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// translate it down so it's at the center of the scene
		model4 = glm::scale(model4, glm::vec3(0.03f, 0.03f, 0.03f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model4);
		dog.Draw(AssimpLightShader);

		glm::mat4 model3 = glm::mat4(1.0f);
		model3 = glm::translate(model3, glm::vec3(-10.0f, 0.0f, 0.0f));
		model3 = glm::rotate(model3, D2R(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		// translate it down so it's at the center of the scene
		model3 = glm::scale(model3, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model3);
		kamin.Draw(AssimpLightShader);

		glm::mat4 model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
		//model2 = glm::rotate(model2, D2R(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		// translate it down so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model2);
		//walls[0].Draw(AssimpLightShader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-18.0f, 0.0f, 1.0f));
		model = glm::rotate(model, D2R(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, D2R(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		 // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model);
		///////////////////////////////

		thinker.Draw(AssimpLightShader);

		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(-18.0f, 0.0f, 19.0f));
		model1 = glm::rotate(model1, D2R(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, D2R(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// translate it down so it's at the center of the scene
		model1 = glm::scale(model1, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		AssimpLightShader.setMat4("model", model1);
		skull.Draw(AssimpLightShader);
		//thinker.Draw(lightShader);
		//thinker.SpecialDrawBar2(AssimpLightShader);

		lightShader.Use();
		MVP = projection * view*trans;
		glUniformMatrix4fv(transformLocMVPLightShader, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(transformLocModelLightShader, 1, GL_FALSE, glm::value_ptr(trans));
		//glUniform3f(transformLocLightPosLightShader, light.x, light.y, light.z);
		glUniform3f(transformLocLightColorLightShader, light.lightColor.r, light.lightColor.g, light.lightColor.b);
		glUniform3f(transformLocViewPosLightShader, camera.Position.x, camera.Position.y, camera.Position.z);
		lightShader.setVec3("light.position", light.x, light.y, light.z);
		lightShader.setVec3("light.direction", light.direction.x, light.direction.y, light.direction.z);
		lightShader.setVec3("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
		lightShader.setVec3("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z); // darken the light a bit to fit the scene
		lightShader.setVec3("light.specular", light.specular.x, light.specular.y, light.specular.z);
		lightShader.setFloat("light.constant", light.constant);
		lightShader.setFloat("light.linear", light.linear);
		lightShader.setFloat("light.quadratic", light.quadratic);
		//walls[0].Draw(lightShader);
		//walls[1].Draw(lightShader);
		for (int i = 0; i < 40; i++)
		{
			walls[i].Draw(lightShader);
		}
		//wall2.Draw(lightShader);
		//wall1.Draw(lightShader);
		glm::mat4 trans1;
		trans1 = glm::rotate(iden, D2R(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MVP = projection * view*trans1;
		glUniformMatrix4fv(transformLocMVPLightShader, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(transformLocModelLightShader, 1, GL_FALSE, glm::value_ptr(trans1));
		glUniform3f(transformLocLightPosLightShader, light.x, light.y, light.z);
		glUniform3f(transformLocLightColorLightShader, light.lightColor.r, light.lightColor.g, light.lightColor.b);
		glUniform3f(transformLocViewPosLightShader, camera.Position.x, camera.Position.y, camera.Position.z);
		for (int i = 40; i < 80; i++)
		{
			walls[i].Draw(lightShader);
		}
		//walls[2].Draw(lightShader);
		//walls[3].Draw(lightShader);

		glm::mat4 trans2;
		trans2 = glm::rotate(iden, D2R(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MVP = projection * view*trans2;
		glUniformMatrix4fv(transformLocMVPLightShader, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(transformLocModelLightShader, 1, GL_FALSE, glm::value_ptr(trans2));
		glUniform3f(transformLocLightPosLightShader, light.x, light.y, light.z);
		glUniform3f(transformLocLightColorLightShader, light.lightColor.r, light.lightColor.g, light.lightColor.b);
		glUniform3f(transformLocViewPosLightShader, camera.Position.x, camera.Position.y, camera.Position.z);
		walls[80].Draw(lightShader);
		walls[81].Draw(lightShader);

		Particles->Update(dt, 10, glm::vec3(-10.0f,0.2f,0.25f));
		particleShader.Use();
		particleShader.setMat4("projection", projection);
		
		particleShader.setMat4("view", view);
		Particles->Draw(particleShader);
		
		// Меняем буферы местами
		//sk.Draw(SKShader, view, projection);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	static bool wireframe = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		wireframe = !wireframe;
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
	//	if (!CollisionWall(walls, 6))
	//	{
			camera.ProcessKeyboard(FORWARD, 0.1f);
		//}
		//else
		//{
		//	camera.ProcessKeyboard(BACKWARD, 0.1f);
		//}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
	//	if (!CollisionWall(walls, 6))
	//	{
			camera.ProcessKeyboard(BACKWARD, 0.1f);
	//	}
//		else
	//	{
	//		camera.ProcessKeyboard(FORWARD, 0.1f);
		//}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
	//	if (!CollisionWall(walls, 6))
	//	{
			camera.ProcessKeyboard(LEFT, 0.1f);
	//	}
	//	else
	//	{
	//		camera.ProcessKeyboard(RIGHT, 0.1f);
		//}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//if (!CollisionWall(walls, 6))
		//{
			camera.ProcessKeyboard(RIGHT, 0.1f);
		//}
		//else
		//{
		//	camera.ProcessKeyboard(LEFT, 0.1f);
		//}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		light.x += 0.1f;
		light.UpdatePosition();
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		light.x -= 0.1f;
		light.UpdatePosition();
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		light.z += 0.1f;
		light.UpdatePosition();
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		light.z -= 0.1f;
		light.UpdatePosition();
	}

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		light.y += 0.1f;
		light.UpdatePosition();
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		light.y -= 0.1f;
		light.UpdatePosition();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(window);//тут меняете на xoffset и yoffset, если хотите подвигать камеру
	//glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
}

void mouseKey(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		leftS = true;
		
	}
		// handle right mouse-click
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

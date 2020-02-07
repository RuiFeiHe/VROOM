#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <Project1/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "model.h"
#include "camera.h"
#include "room.h"
//#include "filesystem.h"

//注册一个回调函数(Callback Function)，在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//注册一个输入控制的集成函数
int processInput(GLFWwindow* window);
//注册一个鼠标移动控制的集成函数
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//注册一个滚轮控制的集成函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//注册一个纹理输入控制的集成函数
float textureInput(GLFWwindow* window);
//注册一个绘制的集成函数
//void renderScene(const Shader& shader, Model& ourModel, Model& lampModel, Shader& lampShader, Sphere& ball, Camera& camera);
void renderScene(const Shader& shader, Model& ourModel, Model& lampModel, Shader& lampShader, Sphere& ball, Camera& camera, Model& uplightModel, Model& sofaModel, Model& plantModel, Model& tvModel, Model& winModel);

const unsigned int SCR_WIDTH = 1600;//宽度
const unsigned int SCR_HEIGHT = 600;//高度

// camera
Two_Camera camera(glm::vec3(-0.3f, 0.0f, 0.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// lighting
glm::vec3 lightPos(-0.4f, 1.0f, 2.0f);
glm::vec3 lampPos(0.2f, -3.25f, 0.1f);
glm::vec3 uplightPos(0.0f, 4.0f, -2.0f);
glm::vec3 sofaPos(0.0f, -4.0f, -2.0f);
glm::vec3 plantPos(-2.0f, -4.0f, 1.0f);
glm::vec3 tvPos(0.0f, -4.0f, 1.5f);
glm::vec3 winPos(4.2f, -4.0f, -1.0f);
float winsize = 0.01f;
unsigned int planeVAO;
unsigned int lightVAO;
int flag;

//立方体顶点
float vertices[] = {
	//---- 位置 ----     - 法线坐标 -   - 纹理坐标 -
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
//立方体位置
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  -2.0f,  0.0f),
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
//平面顶点
float planeVertices[] = {
	// positions              - 法线坐标 -       - 纹理坐标 -    // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

	 4.0f, -2.0f,  4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 4.0f, -2.0f, -4.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f
};
glm::vec3 planePositions[] = {
	// positions        

	//glm::vec3(-6.0f, 0.0f,  0.0f),
	//glm::vec3(6.0f,  0.0f,  0.0f),
	//glm::vec3(0.0f,  0.0f, -6.0f),
	glm::vec3(2.0f, 0.0f,  0.0f),
	glm::vec3(-2.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f, -2.0f),
	glm::vec3(0.0f,  2.0f, 0.0f),
	glm::vec3(0.0f,  0.0f, 2.0f) //windowwall
};
glm::vec3 planeRotate[] = {
	// Rotate xis        

	glm::vec3(0.0f,  0.0f,  1.0f),
	glm::vec3(0.0f,  0.0f, -1.0f),
	glm::vec3(1.0f,  0.0f,  0.0f),
	glm::vec3(1.0f,  0.0f,  0.0f),
	glm::vec3(-1.0f,  0.0f,  0.0f)//windowwall
};
// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.9f,  0.2f,  -2.0f),
	glm::vec3(-1.9f, 0.2f, -2.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};


int main()
{
	/******************************************************************************************************
	窗口初始化
	******************************************************************************************************/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
//创建一个窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Old-fashioned Room", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_CURSOR_NORMAL);// 设置捕获鼠标
	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//设置窗口的维度(Dimension)
	glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT);
	//启用深度测试depth test
	glEnable(GL_DEPTH_TEST);
	/****************************************************************************************************
	着色器与顶点对象属性初始化
	****************************************************************************************************/

	Shader ourShader("glsl\\shader.vs", "glsl\\shader.fs");	//读取着色器文件并生成着色器程序
	Shader lampShader("glsl\\lamp.vs", "glsl\\lamp.fs");//光源着色器
	Shader simpleDepthShader("glsl\\shadow.vs", "glsl\\shadow.fs");

	Model ourModel("obj\\table-\\HSM0015.obj");
	Model lampModel("obj\\lamp-\\eb_lamp_01.obj");//good
	Model uplightModel("obj\\eb_ceiling_light_01\\eb_ceiling_light_01.obj");
	Model sofaModel("obj\\sofa-\\ROUND SOFA.obj");//good
	Model plantModel("obj\\plant2\\eb_house_plant_01.obj");
	Model winModel("obj\\Door\\door.obj");
	Model tvModel("obj\\Oldtv\\20349_Old_Television_set_v1 Textured.obj");
	Sphere ball(9, 18);

	//声明顶点缓冲对象：Vertex Buffer Object，VBO和顶点数组对象：Vertex Array Object，VAO
	//立方体顶点
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//把定义的顶点数据复制到缓冲的内存中

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //以顶点属性位置值'0'作为参数，启用顶点属性——位置；
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); //以顶点属性位置值'1'作为参数，启用顶点属性——法线坐标；
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); //以顶点属性位置值'2'作为参数，启用顶点属性——纹理坐标；
	//墙面顶点
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//光源顶点
	//unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/******************************************************************************************************
	纹理初始化
	******************************************************************************************************/
	//unsigned int texture1, texture2;
	unsigned int diffuseMap, specularMap;
	//纹理1
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load("img\\container2.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load container2.png" << std::endl;
	}
	stbi_image_free(data);
	//纹理2
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	data = stbi_load("img\\container2_specular.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load container2_specular.png" << std::endl;
	}
	stbi_image_free(data);
	//地板 纹理3
	unsigned int floorTexture;
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	data = stbi_load("img\\floor.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load floor.jpg" << std::endl;
	}
	stbi_image_free(data);
	//墙面 纹理4
	unsigned int wallTexture;
	glGenTextures(1, &wallTexture);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	data = stbi_load("img\\stonewall.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load wall.jpg" << std::endl;
	}
	stbi_image_free(data);
	//镜面反射常量 纹理5
	unsigned int grayTexture;
	glGenTextures(1, &grayTexture);
	glBindTexture(GL_TEXTURE_2D, grayTexture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	data = stbi_load("img\\gray.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load gray.png" << std::endl;
	}
	stbi_image_free(data);
	//windowwall 纹理6
	unsigned int windowTexture;
	glGenTextures(1, &windowTexture);
	glBindTexture(GL_TEXTURE_2D, windowTexture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	data = stbi_load("img\\wallwindow.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load windowwall.png" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use(); // 别忘记在激活着色器前先设置uniform！
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);//使用着色器类设置设置每个采样器告诉OpenGL其属于哪个纹理单元
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, windowTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grayTexture);
	/******************************************************************************************************
	光源初始化
	******************************************************************************************************/
	ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
	ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
	ourShader.setFloat("material.shininess", 32.0f);//镜面高光的散射/半径

	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// 平行光
	ourShader.setVec3("dirLight.direction", -2.0f, -1.0f, 2.0f);//光源方向
	ourShader.setVec3("dirLight.ambient", 0.02f, 0.02f, 0.02f);//光源环境光强度
	ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);//光源漫反射强度
	ourShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);//光源镜面光强度
	// point light 1
	ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);//光源位置
	ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("pointLights[0].constant", 1.0f);//光源距离系数
	ourShader.setFloat("pointLights[0].linear", 0.09);//光源距离系数
	ourShader.setFloat("pointLights[0].quadratic", 0.032);//光源距离系数
	// point light 2
	ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	ourShader.setFloat("pointLights[1].constant", 1.0f);
	ourShader.setFloat("pointLights[1].linear", 0.09);
	ourShader.setFloat("pointLights[1].quadratic", 0.032);
	/******************************************************************************************************
	3D变换矩阵
	******************************************************************************************************/
	glm::mat4 trans;
	glm::mat4 model;//模型矩阵，包含位移、缩放与旋转。变换到全局世界空间
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 projection;//投影矩阵
	projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH) / 2 / float(SCR_HEIGHT), 0.1f, 100.0f);
	ourShader.setMat4("model", model);
	ourShader.setMat4("projection", projection);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/******************************************************************************************************
	configure depth map FBO // added
	******************************************************************************************************/

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//  shader configuration
	ourShader.setInt("shadowMap", 2);

	//创建渲染循环(Render Loop)
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		flag = processInput(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		//glCullFace(GL_FRONT);// peter
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);


		renderScene(simpleDepthShader, ourModel, lampModel, lampShader, ball, camera.camera1, uplightModel, sofaModel, plantModel, tvModel, winModel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH / 2, SCR_HEIGHT);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.setVec3("viewPos", camera.camera1.Position);
		//cout << camera.camera1.Position.x << endl;
		//cout << camera.camera2.Position.x << endl;
		ourShader.use();//使用着色器		
		//摄像机投影方式
		glm::mat4 projection = glm::perspective(glm::radians(camera.camera1.Zoom), (float)SCR_WIDTH / 2 / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		//截取摄像机内view
		glm::mat4 view = camera.camera1.GetViewMatrix();
		ourShader.setMat4("view", view);

		ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 32.0f);//镜面高光的散射/半径
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		ourShader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// wall
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 64.0f);//镜面高光的散射/半径
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model; float angle;
			model = glm::translate(model, planePositions[i]);
			if (i == 3) {
				angle = 180.0f; ourShader.setFloat("material.shininess", 32.0f);//镜面高光的散射/半径
			}// 房顶较暗 
			else { angle = 90.0f; }
			model = glm::rotate(model, glm::radians(angle), planeRotate[i]);
			ourShader.setMat4("model", model);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//windowwall
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 64.0f);//镜面高光的散射/半径
		{
			glm::mat4 model; float angle;
			model = glm::translate(model, planePositions[4]);
			angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), planeRotate[4]);
			ourShader.setMat4("model", model);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		/**********************************************导入的模型*******************************************/
		{	//table
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
			//model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
			float angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);
		}
		{	//lamp
			glm::mat4 model;
			model = glm::translate(model, lampPos);
			float angle = 135.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			lampModel.Draw(ourShader);
		}
		{ // uplight
			glm::mat4 model;
			model = glm::translate(model, uplightPos);
			//float angle = 135.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.04f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			uplightModel.Draw(ourShader);
		}
		{ // sofa
			glm::mat4 model;
			model = glm::translate(model, sofaPos);
			//float angle = 135.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.002f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			sofaModel.Draw(ourShader);
		}
		{ // plant
			glm::mat4 model;
			model = glm::translate(model, plantPos);
			//model = glm::translate(model, glm::vec3(0.9f, -3.25f, 0.1f));
			//float angle = 135.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			plantModel.Draw(ourShader);
		}
		{ // tv
			glm::mat4 model;
			model = glm::translate(model, tvPos);
			//float angle = 90.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			float angle = -90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			tvModel.Draw(ourShader);
		}
		{ // door
			glm::mat4 model;
			model = glm::translate(model, winPos);
			//float angle = 90.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			float angle = -90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(winsize)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			winModel.Draw(ourShader);
		}

		/**********************************************光源*******************************************/
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 0.1f));
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);

			model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller obj
			lampShader.setMat4("model", model);
			ball.draw(lampShader);
		}


		glBindVertexArray(0);

		// right eye view
		//-----------------------------------------------------------------------------
		//-----------------------------------------------------------------------------
		glViewport(SCR_WIDTH / 2, 0, SCR_WIDTH / 2, SCR_HEIGHT);

		// 1. render depth of scene to texture (from light's perspective)
		simpleDepthShader.use();
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderScene(simpleDepthShader, ourModel, lampModel, lampShader, ball, camera.camera2, uplightModel, sofaModel, plantModel, tvModel, winModel);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(SCR_WIDTH / 2, 0, SCR_WIDTH / 2, SCR_HEIGHT);
		//glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.setVec3("viewPos", camera.camera2.Position);
		//cout << camera.camera2.Position.x << endl;
		ourShader.use();//使用着色器
		projection = glm::perspective(glm::radians(camera.camera2.Zoom), (float)SCR_WIDTH / 2 / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		//截取摄像机内view
		view = camera.camera2.GetViewMatrix();
		ourShader.setMat4("view", view);
		/**********************************************地板*******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 32.0f);//镜面高光的散射/半径
		glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(0.0f, -2.0f, 0.0f));
		ourShader.setMat4("model", model2);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/********************************************** 墙壁 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wallTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 64.0f);//镜面高光的散射/半径
		for (unsigned int i = 0; i < 4; i++)
		{
			glm::mat4 model; float angle;
			model = glm::translate(model, planePositions[i]);
			if (i == 3) {
				angle = 180.0f; ourShader.setFloat("material.shininess", 32.0f);//镜面高光的散射/半径
			}// 房顶较暗 
			else { angle = 90.0f; }
			model = glm::rotate(model, glm::radians(angle), planeRotate[i]);
			ourShader.setMat4("model", model);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		//windowwall
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, grayTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		ourShader.setInt("material.diffuse", 0);//物体材质——漫反射贴图——漫反射光照下物体的颜色
		ourShader.setInt("material.specular", 1);//物体材质——镜面反射贴图——镜面光照对物体的颜色影响
		ourShader.setFloat("material.shininess", 64.0f);//镜面高光的散射/半径
		{
			glm::mat4 model; float angle;
			model = glm::translate(model, planePositions[4]);
			angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), planeRotate[4]);
			ourShader.setMat4("model", model);
			glBindVertexArray(planeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		/**********************************************导入的模型*******************************************/
		{	//table
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
			//model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
			float angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);
		}
		{	//lamp
			glm::mat4 model;
			model = glm::translate(model, lampPos);
			float angle = 135.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			lampModel.Draw(ourShader);
		}
		{ // uplight
			glm::mat4 model;
			model = glm::translate(model, uplightPos);
			float angle = 135.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.04f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			uplightModel.Draw(ourShader);
		}
		{ // sofa
			glm::mat4 model;
			model = glm::translate(model, sofaPos);
			//float angle = 135.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.002f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			sofaModel.Draw(ourShader);
		}
		{ // plant
			glm::mat4 model;
			model = glm::translate(model, plantPos);
			//model = glm::translate(model, glm::vec3(0.9f, -3.25f, 0.1f));
			/*float angle = 90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));*/
			model = glm::scale(model, glm::vec3(0.05f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			plantModel.Draw(ourShader);
		}
		{ // tv
			glm::mat4 model;
			model = glm::translate(model, tvPos);
			//model = glm::translate(model, glm::vec3(0.9f, -3.25f, 0.1f));
			float angle = -90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			tvModel.Draw(ourShader);
		}
		{ // window
			glm::mat4 model;
			model = glm::translate(model, winPos);
			//float angle = 90.0f;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
			float angle = -90.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(winsize)); // Make it a smaller obj
			ourShader.setMat4("model", model);
			winModel.Draw(ourShader);
		}

		/**********************************************光源*******************************************/
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 2; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (unsigned int i = 0; i < 2; i++)
		{
			lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 0.1f));
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);

			model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller obj
			lampShader.setMat4("model", model);
			ball.draw(lampShader);
		}

		glBindVertexArray(0);


		// 交换缓冲并查询IO事件
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// 删掉缓冲数组对象和缓冲对象
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}
// renders the 3D scene
void renderScene(const Shader& shader, Model& ourModel, Model& lampModel, Shader& lampShader, Sphere& ball, Camera& camera, Model& uplightModel, Model& sofaModel, Model& plantModel, Model& tvModel, Model& winModel)
{
	// floor
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	shader.setMat4("model", model);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	for (unsigned int i = 0; i < 4; i++)
	{
		glm::mat4 model; float angle;
		model = glm::translate(model, planePositions[i]);
		if (i == 3) {
			angle = 180.0f;
		}
		else { angle = 90.0f; }
		model = glm::rotate(model, glm::radians(angle), planeRotate[i]);
		shader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	//windowwall
	{
		glm::mat4 model; float angle;
		model = glm::translate(model, planePositions[4]);
		angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), planeRotate[4]);
		shader.setMat4("model", model);
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	// model 
	{	//table
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f));
		float angle = 90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f)); // Make it a smaller obj
		shader.setMat4("model", model);
		ourModel.Draw(shader);
	}
	{	//lamp
		glm::mat4 model;
		model = glm::translate(model, lampPos);
		float angle = 135.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f)); // Make it a smaller obj
		shader.setMat4("model", model);
		lampModel.Draw(shader);
	}
	{ // uplight
		glm::mat4 model;
		model = glm::translate(model, uplightPos);
		float angle = 135.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.04f)); // Make it a smaller obj
		shader.setMat4("model", model);
		uplightModel.Draw(shader);
	}
	{ // sofa
		glm::mat4 model;
		model = glm::translate(model, sofaPos);
		//float angle = 135.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.002f)); // Make it a smaller obj
		shader.setMat4("model", model);
		sofaModel.Draw(shader);
	}
	{ // plant
		glm::mat4 model;
		model = glm::translate(model, plantPos);
		//model = glm::translate(model, glm::vec3(0.9f, -3.25f, 0.1f));
		//float angle = 135.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f)); // Make it a smaller obj
		shader.setMat4("model", model);
		plantModel.Draw(shader);
	}
	{ // tv
		glm::mat4 model;
		model = glm::translate(model, tvPos);
		//model = glm::translate(model, glm::vec3(0.9f, -3.25f, 0.1f));
		//float angle = 90.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller obj
		shader.setMat4("model", model);
		tvModel.Draw(shader);
	}
	{ // window
		glm::mat4 model;
		model = glm::translate(model, winPos);
		//float angle = 90.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(winsize)); // Make it a smaller obj
		shader.setMat4("model", model);
		winModel.Draw(shader);
	}
	// light
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / 2 / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	glBindVertexArray(lightVAO);
	for (unsigned int i = 0; i < 2; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		lampShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	for (unsigned int i = 0; i < 2; i++)
	{
		lampShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 0.1f));
		glm::mat4 model;
		model = glm::translate(model, pointLightPositions[i]);

		model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller obj
		lampShader.setMat4("model", model);
		ball.draw(lampShader);
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//退出程序
		glfwSetWindowShouldClose(window, true);
	//键盘控制摄像机移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		return 1;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		return 2;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		return 3;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		return 4;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		return 5;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		return 6;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		return 7;
	return 0;
}

float textureInput(GLFWwindow* window)
{
	//上下键改变两个纹理透明度
	static float tempMix = 0.1f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		tempMix += 0.0001f;
		if (tempMix >= 1.0)
			tempMix = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		tempMix -= 0.0001f;
		if (tempMix <= 0.0)
			tempMix = 0.0f;
	}
	return tempMix;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS) {
		//std::cout << "按下" << std::endl;
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;
		if (flag == 0) { camera.ProcessMouseMovement(xoffset, yoffset); }
	//std:cout << flag << std::endl;
		glm::vec3 move = glm::vec3(xoffset / 100,  0.0f,yoffset / 100);
		glm::vec3 dis_ = glm::vec3(xoffset / 100,  0.0f,0.0f);
		switch (flag) {
		case 1: lightPos += move; break;
		case 2: lampPos += move;   break;
		case 3: uplightPos += move; break;
		case 4: sofaPos += move; break;
		case 5: plantPos += move; break;
		case 6: tvPos += move; break;
		case 7: camera.dis += dis_; camera.ProcessKeyboard(LEFT, 0); break;
		}
	}
	else {
		if (state == GLFW_RELEASE) {
			//std::cout << "起来" << std::endl;
			lastX = xpos;
			lastY = ypos;
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
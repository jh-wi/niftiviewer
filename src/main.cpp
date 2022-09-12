#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <shader.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "core/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/camera.h"
//#include "core/mesh.h"
//#include "core/model.h"
#include "isosurface/MarchingCubes.h"
#include "testlib.h"

void ProcessInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double posX, double posY);
//int StartWindow() { return 17; }
//int main();

glm::vec3 camForward = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX;
float lastY;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;
bool escLastFrame = false;
bool freeMouse = false;

float camSpeed = 10.0f;
float sensitivity = 0.1f;

int windowX = 1920;
int windowY = 1080;

float iso = 0;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

const char* const* const foo = nullptr;

/*extern "C" {
	int TestWindow() {
		return StartWindow();
	}
}*/

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	
	lastX = windowX/2;
	lastY = windowY/2;
	GLFWwindow* window = glfwCreateWindow(windowX, windowY, "MRIviewer", NULL/*glfwGetPrimaryMonitor()*/, NULL);
	if (window == NULL) {
		std::cout << "GLFW window creation failed\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD initialization failed\n";
		return -1;
	}
	
	const char* glsl_version = "#version 460";
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);
	
	//std::cout << addfive(22) << std::endl;
	
	/*float vertices[] = {
		-0.5f, -0.5f, 0, 1, 0.5f, 0, 0, 0,
		-0.5f, 0.5f, 0, 0.5f, 1, 0, 0, 1,
		0.5f, 0.5f, 0, 1, 1, 0, 1, 1,
		0.5f, -0.5f, 0, 0.5f, 0.5f, 1, 1, 0
	};*/
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	for (int i = 0; i < 180; i++) {
		vertices[i] *= 0.1f;
	}
	
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	/*float densities[] = {
		1, 1, 1, 1, 1, 0.5f, 1, 1, 1,
		1, 1, 1, 0.25f, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1
	};*/
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1, 1, 0, 1};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("./assets/test texture.png", &width, &height, &nChannels, 0);
	
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Error: Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Shader testShader("./src/shaders/vertex_test.glsl", "./src/shaders/fragment_test.glsl");
	
	testShader.Use();
	testShader.SetInt("tex", 0);
	
	//glm::vec3 camUp = glm::cross(camDir, camRight);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	
	const float rad = 10;
	
	//char modelPath[] = "./assets/backpack/backpack.obj";
	//Model backpack(modelPath);
	
	MarchingCubes mc;
	
	float* densities = new float[256*256*256];
	int xmax, ymax, zmax;
	int maxval = 0;
	
	std::ifstream file("./data/densities.txt");
	if (file.is_open()) {
		file >> xmax >> ymax >> zmax;
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				for (int z = 0; z < 64; z++) {
					int val;
					file >> val;
					if (val > maxval) maxval = val;
					densities[x] = val;
				}
			}
		}
	}
	file.close();
	
	for (int i = 0; i < 64*64*64; i++) {
		densities[i] = (float)densities[i]/maxval;
	}
	
	//for (int i = 134*256*256; i < 256*256*256; i++) {
	//	densities[i] = 0;
	//}
	
	
	while (!glfwWindowShouldClose(window)) {
		float currentFrame =  glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		ProcessInput(window);
		
		glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//triangle rendering
		testShader.Use();
		
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		/*glm::mat4 view = glm::mat4(1.0f);
		float camX = sin(glfwGetTime()) * rad;
		float camY = cos(glfwGetTime()) * rad;
		view = glm::lookAt(camPos, camPos + camForward, camUp);*/
		glm::mat4 view = cam.GetViewMatrix();
		
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
		//testShader.SetMat4("model", model);
		testShader.SetMat4("view", view);
		testShader.SetMat4("projection", projection);
		
		
		//glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				for (int z = 0; z < 10; z++) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(x * 2 - 5, y * 2 - 5, z * 2 - 5));
					testShader.SetMat4("model", model);
					float col[4] = {0.1f * x, 0.1f * y, 0.1f * z, 1};
					testShader.SetVec4("albedo", col);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
		
		//backpack.Draw(testShader);
		
		//GUI rendering
		/*ImGui::Begin("Rectangle Properties");
		static float translation[2] = {0, 0};
		ImGui::SliderFloat2("translation", translation, -1, 1);
		testShader.SetVec2("translation", translation);
		static float col[4] = {1, 1, 1, 1};
		//ImGui::ColorEdit3("albedo", col);
		//testShader.SetVec4("albedo", col);
		ImGui::End();*/
		
		ImGui::Begin("Misc");
		ImGui::InputFloat("camera speed", &camSpeed, 0.5f, 5.0f);
		cam.speed = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ? camSpeed * 2 : camSpeed;
		ImGui::InputFloat("sensitivity", &sensitivity, 0.05f, 0.5f);
		cam.sensitivity = sensitivity;
		ImGui::SliderFloat("isolevel", &iso, -1, 1);
		if (ImGui::Button("Quit")) {
			glfwSetWindowShouldClose(window, true);
		}
		ImGui::End();
		
		glm::mat4 model = glm::mat4(1.0f);
		testShader.SetMat4("model", model);
		float col[4] = {0.1f, 0.5f, 0.5f, 1};
		testShader.SetVec4("albedo", col);
		int d = 64;
		SimpleMesh mesh = mc.BuildSurfaceMesh(densities, (int)(sizeof(densities)/sizeof(densities[0])), (d-1)*(d-1)*(d-1), d, iso);
		mesh.Draw(testShader);
		
		glBindVertexArray(VAO);
		
		//visualize densities
		/*int chunkSize = d;
		for (int x = 0; x < chunkSize; x++) {
			for (int y = 0; y < chunkSize; y++) {
				for (int z = 0; z < chunkSize; z++) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(x, y, z));
					testShader.SetMat4("model", model);
					float density = densities[MarchingCubes::GetVoxelIndex(glm::ivec3(x, y, z), chunkSize)];
					float col2[4] = {density, density, density, 1};
					testShader.SetVec4("albedo", col2);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}*/
		
		
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);	
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !escLastFrame) {
		//glfwSetWindowShouldClose(window, true);
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			freeMouse = true;
			firstMouse = true;
		} else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			freeMouse = false;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.ProcessKeyboard(BACK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cam.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cam.ProcessKeyboard(DOWN, deltaTime);
	
	escLastFrame = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

void MouseCallback(GLFWwindow* window, double posX, double posY) {
	if (freeMouse) return;
	
	if (firstMouse) {
		lastX = posX;
		lastY = posY;
		firstMouse = false;
	}
	
	float offsetX = posX - lastX;
	float offsetY = posY - lastY;
	lastX = posX;
	lastY = posY;
	/*yaw += offsetX;
	pitch -= offsetY;
	
	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	
	glm::vec3 dir = glm::vec3(0.0f);
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camForward = glm::normalize(dir);*/
	
	cam.ProcessMouse(offsetX, offsetY);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
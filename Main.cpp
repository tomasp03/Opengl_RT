#include <iostream>
#include "WindowClass.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VBO.h"
#include "Camera.h"
#include "TimerClass.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

float vertices[] =
{
	-1.0f,  1.0f,
	 1.0f,  1.0f,
	-1.0f, -1.0f,
	-1.0f, -1.0f,
	 1.0f,  1.0f,
	 1.0f, -1.0f,
};

int main()
{
	WindowClass window(WINDOW_WIDTH, WINDOW_HEIGHT, "pepa");
	Shader shader("default.vert", "default.frag");
	VertexArray VAO;
	VBO VBO1(vertices, sizeof(vertices));

	VAO.Bind();
	VAO.LinkBuffer(VBO1, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);

	shader.Activate();
	glUniform2fv(glGetUniformLocation(shader.ID, "u_resolution"), 1, glm::value_ptr(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT)));
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glUniform3fv(glGetUniformLocation(shader.ID, "u_camPos"), 1, glm::value_ptr(camPos));
	glm::vec3 sphereCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(shader.ID, "u_sphereCenter"), 1, glm::value_ptr(sphereCenter));
	glm::vec3 sphereCenter2 = glm::vec3(0.0f, -101.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(shader.ID, "u_sphereCenter2"), 1, glm::value_ptr(sphereCenter2));
	glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 2.0f);
	glUniform3fv(glGetUniformLocation(shader.ID, "u_lightPos"), 1, glm::value_ptr(lightPos));

	Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, camPos, 90.0f);


	window.Vsync(OFF);
	float camx = 0.0f;
	float camz = -1.0f;
	float speed = 1.0f;
	double mouseX = 0.0f;
	double mouseY = 0.0f;
	double dt;
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	while (!window.ShouldClose())
	{
		TimerClass timer(&dt);
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		glfwGetCursorPos(window.window, &mouseX, &mouseY);
		glm::vec2 mousePos = glm::vec2(mouseX, mouseY);
		glUniform2fv(glGetUniformLocation(shader.ID, "u_mousePos"), 1, glm::value_ptr(mousePos));

		glm::mat4 rotateBlue = glm::mat4(1.0f);
		//rotateBlue = glm::translate(rotateBlue, -sphereCenter2);
		rotateBlue = glm::rotate(rotateBlue, (float)glfwGetTime() * glm::radians(50.0f), glm::normalize(glm::vec3(1.0, 1.0, 1.0)));
		//rotateBlue = glm::translate(rotateBlue, sphereCenter2);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_rotate"), 1, GL_FALSE, glm::value_ptr(rotateBlue));


		float dtF = (float)dt;
		// Handles key inputs
		if (glfwGetKey(window.window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Position += dtF * speed * camera.Orientation;
		}
		if (glfwGetKey(window.window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Position += dtF * speed * -glm::normalize(glm::cross(glm::vec3(0, 0, -1), Up));
		}
		if (glfwGetKey(window.window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Position += dtF * speed * -camera.Orientation;
		}
		if (glfwGetKey(window.window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Position += dtF * speed * glm::normalize(glm::cross(glm::vec3(0, 0, -1), Up));
		}
		if (glfwGetKey(window.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Position += dtF * speed * Up;
		}
		if (glfwGetKey(window.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			Position += dtF * speed * -Up;
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Position);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "u_model"), 1, GL_FALSE, glm::value_ptr(model));
		//camera.Inputs(window.window);
		camera.Matrix(0.01f, 100.0f, shader, "u_projection");

		shader.Activate();
		glUniform1f(glGetUniformLocation(shader.ID, "u_time"), (float)glfwGetTime());
		glDrawArrays(GL_TRIANGLES, 0, 6);


		window.SwapBuffers();
		window.PollEvents();
		window.ShowFPS();
	}
}
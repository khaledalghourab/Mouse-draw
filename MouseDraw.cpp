#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse(GLFWwindow* w, int button, int action, int mode);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource =
"#version 330 core\n"
"in vec3 aPos;\n"
"uniform vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

float vertices[] = {
	// positions         // colors
	0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // p1
	0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // p2
};

float r = 1, g = 1, b = 1;
GLint aColor, aPos;
float NorX1, NorY1, NorX2, NorY2;
int drawFlag = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glBindAttribLocation(shaderProgram, 0, "aPos");

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	aPos = glGetAttribLocation(shaderProgram, "aPos");
	aColor = glGetUniformLocation(shaderProgram, "aColor");
	printf("aPos= %i   aColor= %i\n", aPos, aColor);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	GLuint VBO;
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::
		endl;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shaderProgram);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		if (drawFlag == 1) {
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glUniform3f(aColor, r, g, b);

			drawFlag = 0;
		}

		glDrawArrays(GL_LINES, 0, 2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		r = 0.0;
		g = 1.0;
		b = 0.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		r = 0.0;
		g = 0.0;
		b = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[6] = 0;
		vertices[7] = 0;
		drawFlag = 1;
	}
}

void mouse(GLFWwindow* w, int button, int action, int mode) {
	double xpos = 0, ypos = 0;
	int width, height;
	//getting cursor position
	glfwGetCursorPos(w, &xpos, &ypos);
	glfwGetWindowSize(w, &width, &height);

	if (action == 1) {
		NorX1 = (xpos - (width / 2)) / (width / 2);
		NorY1 = -(ypos - (height / 2)) / (height / 2);
		printf("Button = %i \t isClicked? = %i \t X = %.3f \t Y = %.3f \t NorX = %.3f \t NorY = %.3f \n", button, action, xpos, ypos, NorX1, NorY1);
	}
	else if (action == 0)
	{
		drawFlag = 1;
		NorX2 = (xpos - (width / 2)) / (width / 2);
		NorY2 = -(ypos - (height / 2)) / (height / 2);
		vertices[0] = NorX1;
		vertices[1] = NorY1;
		vertices[6] = NorX2;
		vertices[7] = NorY2;
		printf("Button = %i \t isClicked? = %i \t X = %.3f \t Y = %.3f \t NorX = %.3f \t NorY = %.3f \n", button, action, xpos, ypos, NorX2, NorY2);
	}

}




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);

}

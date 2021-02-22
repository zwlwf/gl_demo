#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float alpha = 0.0f;
GLuint prog, vao;

void keyFun(GLFWwindow* window , int key , int scancode ,int action , int mode ) {
	if( action = GLFW_PRESS ) {
		if(key==GLFW_KEY_Q) {
			glfwSetWindowShouldClose(window, true);
		} else {
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(prog);
			alpha+=2.4f;
			glUniform1f( glGetUniformLocation(prog,"alpha"), alpha);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
			glFinish();
			glfwSwapBuffers(window);
		}
	}
}

char *loadText(const char* fname ) {
	FILE *fp = fopen(fname,"rb");
	if(!fp) {
		printf("File %s does not exist",fname);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int n = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char * ans = malloc(n+1);
	fread(ans, n, sizeof(char), fp);
	ans[n] = 0;
	fclose(fp);
	return ans;
}

GLuint initShader() {
	char* vertexShaderSource = loadText("a.vs");
	char* fragmentShaderSource = loadText("a.fs");
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource( vertexShader, 1, (char const* const*) &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int flag;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &flag);
	if(!flag) {
		glGetShaderInfoLog( vertexShader, 512, NULL, log);
		printf("Error : %s ", log);
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource( fragmentShader, 1, (char const* const*) &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &flag);
	if(!flag) {
		glGetShaderInfoLog( fragmentShader, 512, NULL, log);
		printf("Error : %s ", log);
	}
	GLuint prog = glCreateProgram();
	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fragmentShader);
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &flag);
	if(!flag) {
		glGetProgramInfoLog( prog, 512, NULL, log);
		printf("Error : %s ", log);
	}
	return prog;
}

GLuint initVAO() {
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, & VAO);
	glBindVertexArray( VAO );
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float arr[] = {
		-0.5f, -0.5f, 0.0, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0, 1.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0, 1.0f, 0.0f, 1.0f };

	glBufferData(GL_ARRAY_BUFFER, sizeof(arr), arr, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	return VAO;
}

int main() {
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(600,600,"zwl", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyFun);
	if( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) )  {
		printf("OPENGL load failed\n");
		return -1;
	}
	prog = initShader();
	vao = initVAO();
	while( !glfwWindowShouldClose(window) ) {
		glfwWaitEvents(); //低频扫描
		//glfwPollEvents(); // 高效执行，立马返回
	}
	glfwTerminate();

	return 0;
}

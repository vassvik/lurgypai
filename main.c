#include <stdio.h>

#include <glad/glad.c>
#include <GLFW/glfw3.h>


void error_callback(int error, const char* description);
char *read_entire_file(const char *filename);
int compile_shader(const char * file_path, GLuint shader_ID);
GLuint load_shaders(const char * vertex_file_path,const char * fragment_file_path);



int main() {
	//
	glfwSetErrorCallback(error_callback);

	//
	if (!glfwInit()) {
		return -1;
	}

	//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //
    int resx = 1280;
    int resy = 720;
    GLFWwindow *window = glfwCreateWindow(resx, resy, "Title", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -2;
    }

    //
    glfwMakeContextCurrent(window); 
    glfwSwapInterval(1);

    //
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Something went wrong while loading GLAD!\n");
        glfwTerminate();
        return -3;
    }

    // 
    GLuint program = load_shaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    //
    GLuint vao;
    glGenVertexArrays(1, &vao);


    //
    glClearColor(1.0, 0.7, 0.4, 1.0);
    while (!glfwWindowShouldClose(window)) {
    	//
    	glfwPollEvents();

    	//
    	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);

    	//
    	glClear(GL_COLOR_BUFFER_BIT);

    	//
    	glUseProgram(program);
    	
    	//
    	glBindVertexArray(vao);
    	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    	//
    	glfwSwapBuffers(window);
    }

    //
    glfwTerminate();

	return 0;
}






void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s (%d)\n", description, error);
}


// Shader utility functions. 
char *read_entire_file(const char *filename) {
    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = (char*)malloc(fsize + 1);
    fread(string, fsize, 1, f);
    string[fsize] = '\0';
    fclose(f);

    return string;
}

int compile_shader(const char * file_path, GLuint shader_ID) {
    char *shader_code = read_entire_file(file_path);
    if (shader_code == NULL) {
        fprintf(stderr, "Error: Could not read shader file: \"%s\"\n", file_path);
        return -1;
    }
    printf("Compiling shader : %s\n", file_path);
    glShaderSource(shader_ID, 1, (const char**)&shader_code , NULL);
    glCompileShader(shader_ID);

    GLint result;
    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &result);

    if ( result == GL_FALSE ){
        GLint info_log_length;
        glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &info_log_length);

        char shader_error_message[info_log_length+1];
        glGetShaderInfoLog(shader_ID, info_log_length, NULL, shader_error_message);
        fprintf(stderr, "Error while compiling shader \"%s\":\n%s", file_path, shader_error_message);

        free(shader_code);
        return -2;
    }

    free(shader_code);

    return 0;
}

GLuint load_shaders(const char * vertex_file_path,const char * fragment_file_path){
    GLuint vertex_shader_ID   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

    int err1 = compile_shader(vertex_file_path, vertex_shader_ID);
    int err2 = compile_shader(fragment_file_path, fragment_shader_ID);

    if (err1 || err2) {
        glDeleteShader(vertex_shader_ID);
        glDeleteShader(fragment_shader_ID);
        return 0;
    }

    GLuint program_ID = glCreateProgram();
    glAttachShader(program_ID, vertex_shader_ID);
    glAttachShader(program_ID, fragment_shader_ID);
    glLinkProgram(program_ID);

    GLint result;
    glGetProgramiv(program_ID, GL_LINK_STATUS, &result);

    if ( result == GL_FALSE ){
        GLint info_log_length;
        glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar program_error_message[info_log_length+1];
        glGetProgramInfoLog(program_ID, info_log_length, NULL, program_error_message);
        printf("Error while linking program:\n%s\n", program_error_message);
        
        glDeleteShader(vertex_shader_ID);
        glDeleteShader(fragment_shader_ID);
        return 0;
    }

    glDeleteShader(vertex_shader_ID);
    glDeleteShader(fragment_shader_ID);

    return program_ID;
}
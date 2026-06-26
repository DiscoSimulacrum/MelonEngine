#pragma once

#include <glad/glad.h>

struct VBO {
	GLuint id;
};

struct VAO {
	GLuint id;
};

VBO createVBO(const GLfloat* data, GLsizeiptr size);
VAO createVAO();

void bindVBO(const VBO& vbo);
void bindVAO(const VAO& vao);

void unbindVBO();
void unbindVAO();

void deleteVBO(const VBO& vbo);
void deleteVAO(const VAO& vao);

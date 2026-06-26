#include "VertexBuffer.h"

//Create a VBO and store in VBO struct
VBO createVBO(const GLfloat* data, GLsizeiptr size) {
	VBO vbo;
	glGenBuffers(1, &vbo.id);  // Generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo.id);  // Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);  // Set the buffer data
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the VBO
	return vbo;
}

//Create a VAO and store in VAO struct
VAO createVAO() {
	VAO vao;
	glGenVertexArrays(1, &vao.id);
	return vao;
}

//Bind a VBO
void bindVBO(const VBO& vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
}

//Bind a VAO
void bindVAO(const VAO& vao) {
	glBindVertexArray(vao.id);
}

//Unbind a VBO
void unbindVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Unbind a VAO
void unbindVAO() {
	glBindVertexArray(0);
}

//Delete a VBO
void deleteVBO(const VBO& vbo) {
	glDeleteBuffers(1, &vbo.id);
	//vbo.id = 0;  // Reset the ID
}

//Delete a VAO
void deleteVAO(const VAO& vao) {
	glDeleteVertexArrays(1, &vao.id);
	//vao.id = 0;  // Reset the ID
}
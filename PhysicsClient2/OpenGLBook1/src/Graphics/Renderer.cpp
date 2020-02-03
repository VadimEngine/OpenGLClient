#include <SOIL.h>
#include <fstream>

#include "Renderer.h"


Renderer::Renderer(Shader* shader)
:shader(shader){
	initRenderData();
}

Renderer::~Renderer() {
	vertList.clear();
	delete shader;
}

void Renderer::initRenderData() {
	//GLuint VBO; //,VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void Renderer::Draw() {//, glm::vec3 color	
	shader->Use();
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertList.size() * sizeof(GLfloat), vertList.data(), GL_DYNAMIC_DRAW);//GL_STATIC_DRAW

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);// 3 * sizeof(GLfloat) instead of 0?
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe
	glDrawArrays(GL_TRIANGLES, 0, vertList.size()/3);//breaks here after some time: Exception thrown at 0x5DD1FA90 (ig75icd32.dll) in OpenGLBook1.exe: 0xC0000005: Access violation reading location 0x00000010.
	glBindVertexArray(0);//Unbind
	
	vertList.clear();	
}

//Temp metthod for rendering a circle
void Renderer::addVertices(GLfloat x, GLfloat y) {
	vertList.push_back(x);
	vertList.push_back(y);
	vertList.push_back(0);
	//use a queue instead?
}

void Renderer::clear() {
	vertList.clear();
}

//render methods
//Render text by rendering from a spritesheet of characters
void Renderer::renderText(std::string theString, float x, float y) {

}

//x y is the top left corner
//add counter clock wise
void Renderer::renderRectangle(float x, float y, float width, float height) {
	//add vertex to list
	addVertices(x, y);
	addVertices(x, y - height);
	addVertices(x + width, y - height);
	
	addVertices(x + width, y);
	addVertices(x, y);
	addVertices(x + width, y - height);
}

void Renderer::renderCircle(float x, float y, float radius, int sides) {
	double pi = 3.14159265358979323846;

	for (int i = 0; i < sides; i++) {
		GLfloat x1 = x;
		GLfloat y1 = y;

		GLfloat x2 = x + cos((2 * pi / (float)sides) * (float)i) * radius;
		GLfloat y2 = y + sin((2 * pi / (float)sides) * (float)i) * radius;

		GLfloat x3 = x + cos((2 * pi / (float)sides) * (float)(i + 1)) * radius;
		GLfloat y3 = y + sin((2 * pi / (float)sides) * (float)(i + 1)) * radius;

		addVertices(x1, y1);
		addVertices(x2, y2);
		addVertices(x3, y3);
	};
}

void renderImage() {
	//GLuint FramebufferName
}

void renderPolygon(float* vertices) {

}
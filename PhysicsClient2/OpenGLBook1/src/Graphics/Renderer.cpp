#include <SOIL.h>
#include <fstream>
#include "Renderer.h"
#include <GL/glew.h>//include GLEW before glfw
#include <GLFW/glfw3.h>


//idea, hold verts in a map<glm::vec3, index> avoid redundnet vertices 
// and quickly use a EBO for efficiency

Renderer::Renderer(Shader* polygonShader, Shader* imageShader)
:polygonShader(polygonShader), imageShader(imageShader) {
	initRenderData();
	//init the charKeyMap

	charKeyMap.insert({ 'A', glm::vec2(0,0) });
	charKeyMap.insert({ 'B', glm::vec2(1,0) });
	charKeyMap.insert({ 'C', glm::vec2(2,0) });
	charKeyMap.insert({ 'D', glm::vec2(3,0) });
	charKeyMap.insert({ 'E', glm::vec2(4,0) });
	charKeyMap.insert({ 'F', glm::vec2(5,0) });
	charKeyMap.insert({ 'G', glm::vec2(6,0) });
	charKeyMap.insert({ 'H', glm::vec2(7,0) });
	charKeyMap.insert({ 'I', glm::vec2(8,0) });
	charKeyMap.insert({ 'J', glm::vec2(9,0) });
	charKeyMap.insert({ 'K', glm::vec2(10,0) });
	charKeyMap.insert({ 'L', glm::vec2(11,0) });
	charKeyMap.insert({ 'M', glm::vec2(12,0) });
	charKeyMap.insert({ 'N', glm::vec2(13,0) });
	charKeyMap.insert({ 'O', glm::vec2(14,0) });
	charKeyMap.insert({ 'P', glm::vec2(15,0) });
	charKeyMap.insert({ 'Q', glm::vec2(16,0) });
	charKeyMap.insert({ 'R', glm::vec2(17,0) });
	charKeyMap.insert({ 'S', glm::vec2(18,0) });
	charKeyMap.insert({ 'T', glm::vec2(19,0) });
	charKeyMap.insert({ 'U', glm::vec2(20,0) });
	charKeyMap.insert({ 'V', glm::vec2(21,0) });
	charKeyMap.insert({ 'W', glm::vec2(22,0) });
	charKeyMap.insert({ 'X', glm::vec2(23,0) });
	charKeyMap.insert({ 'Y', glm::vec2(24,0) });
	charKeyMap.insert({ 'Z', glm::vec2(25,0) });

	charKeyMap.insert({ '.', glm::vec2(26,0) });
	charKeyMap.insert({ ',', glm::vec2(27,0) });

	charKeyMap.insert({ 'a', glm::vec2(0,1) });
	charKeyMap.insert({ 'b', glm::vec2(1,1) });
	charKeyMap.insert({ 'c', glm::vec2(2,1) });
	charKeyMap.insert({ 'd', glm::vec2(3,1) });
	charKeyMap.insert({ 'e', glm::vec2(4,1) });
	charKeyMap.insert({ 'f', glm::vec2(5,1) });
	charKeyMap.insert({ 'g', glm::vec2(6,1) });
	charKeyMap.insert({ 'h', glm::vec2(7,1) });
	charKeyMap.insert({ 'i', glm::vec2(8,1) });
	charKeyMap.insert({ 'j', glm::vec2(9,1) });
	charKeyMap.insert({ 'k', glm::vec2(10,1) });
	charKeyMap.insert({ 'l', glm::vec2(11,1) });
	charKeyMap.insert({ 'm', glm::vec2(12,1) });
	charKeyMap.insert({ 'n', glm::vec2(13,1) });
	charKeyMap.insert({ 'o', glm::vec2(14,1) });
	charKeyMap.insert({ 'p', glm::vec2(15,1) });
	charKeyMap.insert({ 'q', glm::vec2(16,1) });
	charKeyMap.insert({ 'r', glm::vec2(17,1) });
	charKeyMap.insert({ 's', glm::vec2(18,1) });
	charKeyMap.insert({ 't', glm::vec2(19,1) });
	charKeyMap.insert({ 'u', glm::vec2(20,1) });
	charKeyMap.insert({ 'v', glm::vec2(21,1) });
	charKeyMap.insert({ 'w', glm::vec2(22,1) });
	charKeyMap.insert({ 'x', glm::vec2(23,1) });
	charKeyMap.insert({ 'y', glm::vec2(24,1) });
	charKeyMap.insert({ 'z', glm::vec2(25,1) });

	charKeyMap.insert({ '0', glm::vec2(0,2) });
	charKeyMap.insert({ '1', glm::vec2(1,2) });
	charKeyMap.insert({ '2', glm::vec2(2,2) });
	charKeyMap.insert({ '3', glm::vec2(3,2) });
	charKeyMap.insert({ '4', glm::vec2(4,2) });
	charKeyMap.insert({ '5', glm::vec2(5,2) });
	charKeyMap.insert({ '6', glm::vec2(6,2) });
	charKeyMap.insert({ '7', glm::vec2(7,2) });
	charKeyMap.insert({ '8', glm::vec2(8,2) });
	charKeyMap.insert({ '9', glm::vec2(9,2) });
}

Renderer::~Renderer() {
	vertList.clear();
	delete polygonShader;
	delete imageShader;
}

void Renderer::initRenderData() {
	


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	int width, height;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* image = SOIL_load_image("res/font.png", &width, &height, 0 , SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &polyVAO);
	glGenBuffers(1, &polyVBO);
}

void Renderer::Draw() {

	//draw polygon

	glBindVertexArray(polyVAO);

	glBindBuffer(GL_ARRAY_BUFFER, polyVBO);
	glBufferData(GL_ARRAY_BUFFER, otherVertList.size() * sizeof(GLfloat), otherVertList.data(), GL_DYNAMIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Texture cord attribute

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	polygonShader->Use();//should use after bind?

	glBindVertexArray(polyVAO);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe
	//glDrawArrays(GL_TRIANGLES, 0, vertList.size()/3);
	//elements might use a EBO
	//glDrawElements(GL_TRIANGLES, vertList.size() / 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, otherVertList.size() / 6);

	glBindVertexArray(0);//Unbind

	otherVertList.clear();



	//Draw image
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertList.size() * sizeof(GLfloat), vertList.data(), GL_DYNAMIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Texture cord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	imageShader->Use();//should use after bind?

	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(imageShader->Program, "ourTexture"), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Wireframe
	//glDrawArrays(GL_TRIANGLES, 0, vertList.size()/3);
	//elements might use a EBO
	//glDrawElements(GL_TRIANGLES, vertList.size() / 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, vertList.size()/8);

	glBindVertexArray(0);//Unbind
	
	vertList.clear();	
}

void Renderer::clear() {
	vertList.clear();
	otherVertList.clear();
}

//kind of works but is buggy
void Renderer::renderLineColor(glm::vec3 p1, glm::vec3 p2, glm::vec3 color, float width) {
	//draw an angled rectangle
	//right
	renderTriangleColor(glm::vec3(p1.x + (width/2), p1.y, p1.z),
						glm::vec3(p2.x + (width / 2), p2.y, p2.z),
						glm::vec3(p2.x - (width / 2), p2.y, p1.z),
						color);
	//left
	renderTriangleColor(glm::vec3(p1.x - (width / 2), p1.y, p1.z),
						glm::vec3(p1.x + (width / 2), p1.y, p1.z),
						glm::vec3(p2.x - (width / 2), p2.y, p2.z),
						color);
}

void Renderer::renderTriangleColor(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color) {
	addVertexAndColor(glm::vec3(p1.x, p1.y, p1.z), color);
	addVertexAndColor(glm::vec3(p2.x, p2.y, p2.z), color);
	addVertexAndColor(glm::vec3(p3.x, p3.y, p3.z), color);
}

void Renderer::renderRectangleColor(glm::vec3 topLeft, float width, float height, glm::vec3 color) {
	//bottom triangle
	renderTriangleColor(glm::vec3(topLeft.x, topLeft.y, topLeft.z),
						glm::vec3(topLeft.x, topLeft.y - height, topLeft.z),
						glm::vec3(topLeft.x + width, topLeft.y - height, topLeft.z),
						color);
	//top triangle
	renderTriangleColor(glm::vec3(topLeft.x, topLeft.y, topLeft.z),
						glm::vec3(topLeft.x + width, topLeft.y, topLeft.z),
						glm::vec3(topLeft.x + width, topLeft.y - height, topLeft.z),
						color);
}

void Renderer::addVertexAndColor(glm::vec3 point, glm::vec3 color) {
	//addpoint
	otherVertList.push_back(point.x);
	otherVertList.push_back(point.y);
	otherVertList.push_back(point.z);
	//add color
	otherVertList.push_back(color.x);
	otherVertList.push_back(color.y);
	otherVertList.push_back(color.z);
}

void Renderer::renderCircleColor(glm::vec3 position, glm::vec3 color, float radius, float sides) {
	double pi = 3.14159265358979323846;
	for (int i = 0; i < sides; i++) {
		GLfloat x1 = position.x;
		GLfloat y1 = position.y;

		GLfloat x2 = position.x + cos((2 * pi / (float)sides) * (float)i) * radius;
		GLfloat y2 = position.y + sin((2 * pi / (float)sides) * (float)i) * radius;

		GLfloat x3 = position.x + cos((2 * pi / (float)sides) * (float)(i + 1)) * radius;
		GLfloat y3 = position.y + sin((2 * pi / (float)sides) * (float)(i + 1)) * radius;

		renderTriangleColor(glm::vec3(x1, y1, 0),
							glm::vec3(x2, y2, 0),
							glm::vec3(x3, y3, 0), 
							color);
	};
}

void Renderer::renderImage(glm::vec3 position, glm::vec3 color) {
	float width = .1;
	//addpoint 1 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0);
	vertList.push_back(1);

	//addpoint 2 bottom left
	vertList.push_back(position.x);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0);
	vertList.push_back(0);

	//addpoint 3 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(1);
	vertList.push_back(0);

	//addpoint 4 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(1);
	vertList.push_back(0);

	//addpoint 5 top right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y );
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(1);
	vertList.push_back(1);

	//addpoint 6 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0);
	vertList.push_back(1);
}

//
void Renderer::renderSubImage(glm::vec3 position, glm::vec3 color) {
	//render the letter a for now  5x8 top left corner
	// texture is 256 x 32 
	float width = .1;
	//addpoint 1 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0);//good
	vertList.push_back(1);

	//addpoint 2 bottom left
	vertList.push_back(position.x);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0); 
	vertList.push_back(0 + 0.75f); //(32 - 8)  / 32

	//addpoint 3 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + (5.0f / 256.0f)); //(5/256)
	vertList.push_back(0 + 0.75f);

	//addpoint 4 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - width);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + (5.0f / 256.0f));
	vertList.push_back(0 + 0.75f);

	//addpoint 5 top right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + (5.0f / 256.0f));
	vertList.push_back(1);

	//addpoint 6 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0);
	vertList.push_back(1);
}


void Renderer::renderSubImageAdvanced(glm::vec3 position,
									  glm::vec3 color,
									  float texWidth,
									  float texHeight,
									  float subWidth,
									  float subHeight,
									  float subX,
									  float subY) {

	float width = .06 / 2.0f;
	float height = .08 / 2.0f;
	//addpoint 1 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + (subX * (subWidth / texWidth)));//good
	vertList.push_back(1 - (subY * (subHeight / texHeight)));

	//addpoint 2 bottom left
	vertList.push_back(position.x);
	vertList.push_back(position.y - height);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture

	vertList.push_back(0 + (subX * (subWidth / texWidth)));
	vertList.push_back(1 - ((subY + 1) * (subHeight / texHeight)));

	//addpoint 3 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - height);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + ((subX + 1) * (subWidth / texWidth)));
	vertList.push_back(1 - ((subY + 1) * (subHeight / texHeight)));

	//addpoint 4 bottom right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y - height);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + ((subX + 1) * (subWidth / texWidth)));
	vertList.push_back(1 - ((subY + 1) * (subHeight / texHeight)));

	//addpoint 5 top right
	vertList.push_back(position.x + width);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + ((subX + 1) * (subWidth / texWidth)));
	vertList.push_back(1 - (subY * (subHeight / texHeight)));

	//addpoint 6 top left
	vertList.push_back(position.x);
	vertList.push_back(position.y);
	vertList.push_back(position.z);
	//add color
	vertList.push_back(color.x);
	vertList.push_back(color.y);
	vertList.push_back(color.z);
	//add texture
	vertList.push_back(0 + (subX * (subWidth / texWidth)));
	vertList.push_back(1 - (subY * (subHeight / texHeight)));

}


void Renderer::renderChar(char theChar, glm::vec3 position, glm::vec3 color, float size) {
	renderSubImageAdvanced(position, color, 256, 32, 6, 8,
						   charKeyMap[theChar].x,
						   charKeyMap[theChar].y);
}


void Renderer::renderString(std::string theString, float x, float y) {
	//limit the ascii range
	// take in font/font-size
	//assume character width

	for (int i = 0; i < theString.size(); i++) {
		//if (theString[i] >= '.' && theString[i] <= 'z') {
		if ( charKeyMap.find(theString[i]) != charKeyMap.end()) {

			renderChar(theString[i], glm::vec3(x + (i*1.0f*.03), y, 0) ,glm::vec3(1,1,1), 1.0f);
		}
	}

}
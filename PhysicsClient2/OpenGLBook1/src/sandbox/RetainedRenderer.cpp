#include "RetainedRenderer.h"




RetainedRenderer::RetainedRenderer(SandboxShader* shader)
:shader(shader){
	initRenderData();
}

RetainedRenderer::~RetainedRenderer() {
	std::cout << "Delete VertList " << vertList.size() << std::endl;
	vertList.clear();
	delete shader;
}

void RetainedRenderer::initRenderData() {
	//GLuint VBO; //,VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vertList.size() * sizeof(GLfloat), vertList.data(), GL_DYNAMIC_DRAW);//GL_STATIC_DRAW

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);//3 * sizeof(GLfloat)
	//glEnableVertexAttribArray(0);


	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	//glBindVertexArray(0);

}


void RetainedRenderer::Draw() {//, glm::vec3 color
	//std::reverse(vertList.begin(), vertList.end());
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

void RetainedRenderer::addVertices(GLfloat x, GLfloat y) {
	vertList.push_back(x);
	vertList.push_back(y);
	vertList.push_back(0);
	//use a queue instead?
}

void RetainedRenderer::clear() {
	vertList.clear();
}
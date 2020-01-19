#include "RetainedObj.h"

const char* vertPath = "src/shaders/sandbox.vert";
const char* fragPath = "src/shaders/sandbox.frag";

static GLuint VAO;

RetainedObj::RetainedObj(GLfloat x, GLfloat y, glm::vec3 color,
	SandboxShader* myShader, GLboolean isPlayer)
	:x(x), y(y), color(color), isPlayer(isPlayer){

	position = glm::vec2(x, y);
	float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


	velocity = glm::vec2(c1, c2);//player vel = 0, but calculate it when it moves (x_0 - x_1) / dt on tick
	lastPosition = glm::vec2(x,y);
}

void RetainedObj::tick(GLfloat dt, GLboolean keys[1024]) {
	GLfloat speed = 1;
	if (isPlayer) {
		if (keys[GLFW_KEY_UP]) {
			position.y += speed * dt;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			position.x += speed * dt;
		}
		if (keys[GLFW_KEY_DOWN]) {
			position.y -= speed * dt;
		}
		if (keys[GLFW_KEY_LEFT]) {
			position.x -= speed * dt;
		}

		velocity = glm::vec2((position.x - lastPosition.x)/dt, (position.y - lastPosition.y) / dt);

		lastPosition.x = position.x;
		lastPosition.y = position.y;


	} else {
		position += velocity * dt;
	}
}

void RetainedObj::render(RetainedRenderer* renderer) {
	renderCircle(renderer);
#ifdef false
	if (isPlayer) {
		renderCircle(renderer);
	} else {
		GLfloat radius = .25;

		renderer->addVertices(x + -radius, y + -radius);
		renderer->addVertices(x + 0, y + radius);
		renderer->addVertices(x + radius, y + -radius);
	}
#endif // false
}

void RetainedObj::renderCircle(RetainedRenderer* renderer) {//save relative coords(without the +x+y)
	GLfloat radius = 0.04f;//0.25f;
	GLuint sides = 10;

	double pi = 3.14159265358979323846;

	for (int i = 0; i < sides; i++) {
		GLfloat x1 = position.x;
		GLfloat y1 = position.y;
		
		GLfloat x2 = position.x + cos((2 * pi / (float)sides) * (float)i) * radius;
		GLfloat y2 = position.y + sin((2 * pi / (float)sides) * (float)i) * radius;

		GLfloat x3 = position.x + cos((2 * pi / (float)sides) * (float)(i+1)) * radius;
		GLfloat y3 = position.y + sin((2 * pi / (float)sides) * (float)(i+1)) * radius;

		//std::cout << x1 << ", " << y1 << ": " << x2 << ", " << y2 << ": " << x3 << ", " << y3 << std::endl;
		renderer->addVertices(x1, y1);
		renderer->addVertices(x2, y2);
		renderer->addVertices(x3, y3);
	}



}



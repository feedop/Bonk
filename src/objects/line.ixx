export module line;

import std;
import <glad/glad.h>;
import <glm/mat4x4.hpp>;

import colors;
import object;
import glutils;

export class Line : public Object
{
public:
	Line() : Object(colors::white)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		ScopedBindArray ba(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	}

	virtual void draw(const Shader* shader) const
	{
		ScopedBindArray ba(VAO);
		shader->setMatrix("model", model);
		shader->setVector("color", color);
		glDrawArrays(GL_LINES, 0, vertices.size());
	}

private:
	inline static const std::vector<glm::vec3> vertices
	{
		{ 0, 0, 0 },
		{ 0, glm::sqrt(3), 0}
	};
};
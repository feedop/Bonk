export module cube;

import std;
import <glad/glad.h>;
import <glm/vec3.hpp>;
import <glm/gtc/quaternion.hpp>;
import <glm/gtx/quaternion.hpp>;

import colors;
import lightedobject;
import math;
import glutils;

std::vector<VertexWithNormal> createVertices()
{
	return {
        // Front face
        {{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}},
        {{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}},

        // Back face
        {{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}},
        {{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}},
        {{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}},

        // Left face
        {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f}},
        {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}},

        // Right face
        {{ 1.0f, -1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 1.0f,  0.0f,  0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 1.0f,  0.0f,  0.0f}},

        // Top face
        {{-1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  1.0f,  0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}},
        {{-1.0f,  1.0f, -1.0f}, { 0.0f,  1.0f,  0.0f}},

        // Bottom face
        {{-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}},
        {{ 1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}},
        {{-1.0f, -1.0f,  1.0f}, { 0.0f, -1.0f,  0.0f}},
    };
}

std::vector<unsigned int> createIndices()
{
	return {
		// Front face
		0, 1, 2,  2, 3, 0,
		// Back face
		4, 5, 6,  6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};
}

export class Cube : public LightedObject
{
public:
	Cube() : LightedObject(createVertices(), createIndices(), colors::yellow)
	{
        model = math::cubeRotationMatrix * math::scale(0.5f) * math::translate({ 1.0f, 1.0f, 1.0f });
	}

	virtual void draw(const Shader* shader) const
	{
		ScopedBindArray ba(VAO);
		// Model set externally
		shader->setVector("color", color);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
	}
};
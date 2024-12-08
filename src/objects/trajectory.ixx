export module trajectory;

import std;
import <glad/glad.h>;
import <glm/vec3.hpp>;

import colors;
import drawable;
import math;
import glutils;

export class Trajectory : public Drawable
{
public:
    Trajectory()
    {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
    }

    void setData(const std::vector<glm::vec3>& data, size_t length)
    {
        dataSize = std::min(data.size(), length);
        if (dataSize == 0)
            return;

        size_t offset = data.size() > length ? data.size() - length : 0;
        ScopedBindArray ba(VAO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3), &data[offset], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    }

    virtual void draw(const Shader* shader) const
    {
        ScopedBindArray ba(VAO);
        shader->setMatrix("model", glm::mat4{1.0f});
        shader->setVector("color", colors::red);
        glDrawArrays(GL_LINE_STRIP, 0, dataSize);
    }

private:
    unsigned int dataSize = 0;
};
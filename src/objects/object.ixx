export module object;

import std;
import <glm/mat4x4.hpp>;

import drawable;

export class Object : public Drawable
{
public:
	inline void setModel(const glm::mat4& newModel)
	{
		model = newModel;
	}

	inline auto& getModel() const
	{
		return model;
	}

protected:
	glm::mat4 model{ 1.0f };
	glm::vec4 color;

	Object(const glm::vec4& color) : color(color)
	{}
};
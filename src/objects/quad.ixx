export module quad;

import std;
import <glm/vec2.hpp>;
import <glm/vec3.hpp>;

import colors;
import lightedobject;
import math;

export struct VertexWithTex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

export class Quad : public LightedObject
{
public:
	static std::vector<VertexWithNormal> createVerticesWithNormal()
	{
		return {
			{
				{-1.0f, 1.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f }
			},
			{
				{1.0f, 1.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f }
			},
			{
				{-1.0f, -1.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f }
			},
			{
				{1.0f, -1.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f }
			}
		};
	}

	static std::vector<VertexWithTex> createVerticesWithTex()
	{
		return {
			{
				{-1.0f, 1.0f, 0.0f},
				{ 0.0f, 0.0f }
			},
			{
				{1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f}
			},
			{
				{-1.0f, -1.0f, 0.0f},
				{1.0f, 0.0f}
			},
			{
				{1.0f, -1.0f, 0.0f},
				{1.0f, 1.0f}
			}
		};
	}

	static std::vector<unsigned int> createIndices()
	{
		return {
			0, 1, 2,
			1, 3, 2
		};
	}

	Quad() : LightedObject(createVerticesWithNormal(), createIndices(), colors::transparentCyan)
	{
		model = math::translate({ 0.0f, 0.001f, 0.0f }) * math::scale(5.0f) * math::rotate(-math::pi / 2, { 1, 0, 0 });
	}
};
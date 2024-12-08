export module simulation:config;

import std;
import <glm/mat3x3.hpp>;

export struct SimulationConfig
{
	float dt;
	glm::mat3 inertiaTensor;
	float deviation;
	float spinAngularVelocity;
	float size;
	float density;
	bool gravity;
};
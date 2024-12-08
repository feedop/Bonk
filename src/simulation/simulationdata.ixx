export module simulation:data;

import <glm/vec3.hpp>;
import <glm/gtc/quaternion.hpp>;

export struct SimulationData
{
	glm::vec3 angularVelocity;
	glm::quat rotation;
	double t;
};
export module math;

import <glm/mat4x4.hpp>;
import <glm/gtc/matrix_transform.hpp>;
import <glm/gtx/matrix_operation.hpp>;
import <glm/gtc/quaternion.hpp>;
import <glm/gtx/quaternion.hpp>;

export namespace math
{
	inline constexpr double eps = 1e-6;
	inline constexpr float pi = glm::pi<float>();

	glm::mat4 rotate(float angle, const glm::vec3& vector)
	{
		const float cos = glm::cos(angle);
		const float sin = glm::sin(angle);

		glm::vec3 axis = glm::normalize(vector);
		glm::vec3 temp = (1.0f - cos) * axis;

		return glm::mat4{
			cos + temp[0] * axis[0], temp[0] * axis[1] + sin * axis[2], temp[0] * axis[2] - sin * axis[1], 0,
			temp[1] * axis[0] - sin * axis[2], cos + temp[1] * axis[1], temp[1] * axis[2] + sin * axis[0], 0,
			temp[2] * axis[0] + sin * axis[1], temp[2] * axis[1] - sin * axis[0], cos + temp[2] * axis[2], 0,
			0, 0, 0, 1
		};
	}

	glm::mat4 translate(const glm::vec3& vector)
	{
		glm::mat4 result{ 1.0f };
		result[3] = { vector[0],vector[1],vector[2], 1.0f };
		return result;
	}

	glm::mat4 scale(const glm::vec3& vector)
	{
		return glm::diagonal4x4(glm::vec4(vector, 1.0f));
	}

	inline glm::mat4 scale(float scaleValue)
	{
		return scale({ scaleValue, scaleValue, scaleValue });
	}

	glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		const glm::vec3 front(normalize(center - eye));
		const glm::vec3 right(normalize(cross(front, up)));
		const glm::vec3 upnorm(cross(right, front));

		glm::mat4 result{ 1.0f };
		result[0][0] = right.x;
		result[1][1] = right.y;
		result[2][0] = right.z;
		result[0][1] = upnorm.x;
		result[1][1] = upnorm.y;
		result[2][1] = upnorm.z;
		result[0][2] = -front.x;
		result[1][2] = -front.y;
		result[2][2] = -front.z;
		result[3][0] = -dot(right, eye);
		result[3][1] = -dot(upnorm, eye);
		result[3][2] = dot(front, eye);
		return result;
		return glm::lookAt(eye, center, up);
	}

	glm::mat4 perspective(float fov, float aspect, float zNear, float zFar)
	{
		const float tanHalfFov = glm::tan(fov / 2.0f);

		glm::mat4 result{ 0.0f };
		result[0][0] = 1.0f / (aspect * tanHalfFov);
		result[1][1] = 1.0f / (tanHalfFov);
		result[2][2] = -(zFar + zNear) / (zFar - zNear);
		result[2][3] = -1.0f;
		result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return result;
	}

	glm::mat4 frustum(float left, float right, float bottom, float top, float near, float far)
	{
		glm::mat4 result{ 0.0f };
		result[0][0] = (2.0f * near) / (right - left);
		result[1][1] = (2.0f * near) / (top - bottom);
		result[2][0] = (right + left) / (right - left);
		result[2][1] = (top + bottom) / (top - bottom);
		result[2][2] = -(far + near) / (far - near);
		result[2][3] = -1.0f;
		result[3][2] = -(2.0f * far * near) / (far - near);
		return result;
	}

	inline const glm::quat rotZ = glm::angleAxis(glm::pi<float>() / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	inline const glm::quat rotX = glm::angleAxis(-glm::atan(1.0f / glm::sqrt(2.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	inline const glm::quat finalRotation = rotZ * rotX;
	
	inline const glm::quat cubeRotationQuat = /*glm::quat_cast(cubeRotationMatrix);*/ glm::quat({ 0, math::pi / 4, glm::atan(glm::sqrt(2.0f)) });
	inline const glm::mat4 cubeRotationMatrix = /*math::rotate(glm::atan(1.0f/glm::sqrt(0.5f)), { 0, 0, 1 })* math::rotate(math::pi / 4, { 0, 1, 0 });*/ glm::toMat4(cubeRotationQuat);
	inline const glm::mat4 cubeRotationMatrixTransposed = glm::transpose(cubeRotationMatrix);
	

	glm::mat3 calculateInertiaTensor(float size, float density)
	{
		float m = density * size * size * size;
		glm::mat3 I{
			{8, -3, -3},
			{-3, 8, -3},
			{-3, -3, 8}
		};
		I *= (m * size * size / 12);
		glm::mat3 IRotated = cubeRotationMatrix * glm::mat4(I) * cubeRotationMatrixTransposed;
		return IRotated;
	}		
}
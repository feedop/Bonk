export module renderer;

import std;

import <glad/glad.h>;
import <glm/vec3.hpp>;
import <glm/vec4.hpp>;
import <glm/gtc/matrix_transform.hpp>;
import <glm/gtx/quaternion.hpp>;

import camera;
import colors;
import config;
import cube;
import drawable;
import scrollingbuffer;
import shader;
import simulation;
import quad;
import grid;
import object;
import lightedobject;
import line;
import math;
import trajectory;


/// <summary>
/// Responsible for rendering the scene using OpenGL
/// </summary>
export class Renderer
{
public:
	Renderer(int windowWidth, int windowHeight, Camera& camera, SimulationEngine& simulation) :
		windowWidth(windowWidth), windowHeight(windowHeight), camera(camera), simulation(simulation)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_PROGRAM_POINT_SIZE);

		// Create objects
		trajectory = std::make_unique<Trajectory>();
		diagonal = std::make_unique<Line>();
		cube = std::make_unique<Cube>();
		quad = std::make_unique<Quad>();
		grid = std::make_unique<Grid>();
	}

	/// <summary>
	/// Render the scene
	/// </summary>
	void draw(const SimulationData& data)
	{
		// Clear
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.15f, 0.0f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Fill trajectory
		auto rotationMatrix = glm::toMat4(data.rotation);
		auto pos = math::scale(cubeSize) * rotationMatrix * glm::vec4{ 0, glm::sqrt(3.0f), 0, 0 };
		trajectoryData.addPoint(glm::vec3(pos));

		drawScene(data, rotationMatrix);

		// Grid	
		if (drawGrid)
		{
			gridShader->use();
			gridShader->setMatrix("view", camera.getView());
			gridShader->setMatrix("projection", camera.getProjection());
			grid->draw(gridShader.get());
		}
	}

	void setWindowSize(int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
		glViewport(0, 0, windowWidth, windowHeight);
	}

	inline void reset()
	{
		trajectoryData.erase();
	}

private:
	friend class GuiController;

	Camera& camera;
	SimulationEngine& simulation;

	std::unique_ptr<Shader> uniformColorShader = std::make_unique<UniformColorShader>();
	std::unique_ptr<Shader> phongShader = std::make_unique<PhongShader>();
	std::unique_ptr<Shader> gridShader = std::make_unique<InfiniteGridShader>();

	std::unique_ptr<Trajectory> trajectory;
	std::unique_ptr<Object> cube;
	std::unique_ptr<Object> diagonal;
	std::unique_ptr<Drawable> quad;
	std::unique_ptr<Drawable> grid;

	int windowWidth;
	int windowHeight;
	bool drawGrid = true;
	bool drawCube = true;
	bool drawDiagonal = false;
	bool drawTrajectory = false;
	bool drawBase = true;
	int trajectoryLength = 5000;
	float cubeSize = cfg::initialCubeSize;

	ScrollingBuffer<glm::vec3, cfg::maxTrajectoryLength> trajectoryData;

	void drawScene(const SimulationData& data, const glm::mat4& rotationMatrix) const
	{
		phongShader->use();
		phongShader->setMatrix("view", camera.getView());
		phongShader->setMatrix("projection", camera.getProjection());

		if (drawCube)
		{
			// Use the data from the simulation
			auto&& model = cube->getModel();
			auto fullModel = math::scale(cubeSize) * rotationMatrix * model;
			phongShader->setMatrix("model", fullModel);
			cube->draw(phongShader.get());
		}
		
		uniformColorShader->use();
		uniformColorShader->setMatrix("view", camera.getView());
		uniformColorShader->setMatrix("projection", camera.getProjection());

		if (drawDiagonal)
		{
			glDisable(GL_DEPTH_TEST);
			diagonal->setModel(rotationMatrix * math::scale(cubeSize));
			diagonal->draw(uniformColorShader.get());
			glEnable(GL_DEPTH_TEST);
		}
		
		if (drawTrajectory)
		{
			trajectory->setData(trajectoryData.data, trajectoryLength);
			trajectory->draw(uniformColorShader.get());
		}

		if (drawBase)
			quad->draw(phongShader.get());
	}
};
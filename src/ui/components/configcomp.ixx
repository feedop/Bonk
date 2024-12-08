export module gui:configcomp;

import std;
import <imgui.h>;

import :controller;
import config;
import imguiext;
import simulation;
import math;

export void GuiController::renderConfig(const SimulationData& data)
{
	static bool configChanged = false;
	static bool inertiaChanged = false;

	ImGui::Text("Display settings:");
	ImGui::Checkbox("Show grid", &renderer.drawGrid);
	ImGui::Checkbox("Show cube", &renderer.drawCube);
	ImGui::Checkbox("Show diagonal", &renderer.drawDiagonal);
	ImGui::Checkbox("Show trajectory", &renderer.drawTrajectory);
	ImGui::Checkbox("Show base plane", &renderer.drawBase);
	ext::InputClampedInt("Trajectory length (points)", &renderer.trajectoryLength, 1, cfg::maxTrajectoryLength);
	ImGui::NewLine();

	ImGui::Text("Simulation settings:");
	configChanged |= ImGui::SliderFloat("dt", &config.dt, 0.001f, 1.0f);
	inertiaChanged |= ImGui::SliderFloat("Cube size", &renderer.cubeSize, 0.0001f, 10.0f);
	inertiaChanged |= ImGui::SliderFloat("Cube density", &config.density, 0.0001f, 10.0f);
	configChanged |= ImGui::SliderFloat("Angular velocity [rad/s]", &config.spinAngularVelocity, -400.0f, 400.0f);
	configChanged |= ImGui::SliderFloat("Deviation", &config.deviation, 0.0f, math::pi/2);
	configChanged |= inertiaChanged;
	
	ImGui::Checkbox("Gravity", &config.gravity);

	ImGui::SameLine();
	if (ImGui::Button("Restart"))
	{
		renderer.reset();
		simulation.setData(data);
	}	

	if (simulation.isPaused())
	{
		if (ImGui::Button("Resume"))
			simulation.resume();
	}
	else
	{
		if (ImGui::Button("Pause"))
			simulation.pause();
	}

	ImGui::NewLine();
	ImGui::Text("Simulation data: ");
	ImGui::Text(std::format("Angular velocity: [{}, {}, {}]", data.angularVelocity.x, data.angularVelocity.y, data.angularVelocity.z).c_str());

	if (inertiaChanged)
	{
		config.size = renderer.cubeSize;
		config.inertiaTensor = math::calculateInertiaTensor(config.size, config.density);
	}
	
	if (configChanged)
		simulation.setConfig(config);
}
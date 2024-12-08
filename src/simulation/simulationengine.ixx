export module simulation:engine;

import std;
import <glm/vec3.hpp>;
import <glm/gtc/matrix_transform.hpp>;
import <glm/gtx/quaternion.hpp>;

import math;
import :config;
import :data;

// Function to calculate dW/dt
glm::vec3 dW_dt(const glm::vec3& W, const glm::vec3& N, const SimulationConfig& config)
{
	return glm::inverse(config.inertiaTensor) * (N + glm::cross(config.inertiaTensor * W, W));
}

// Function to calculate dQ/dt
glm::quat dQ_dt(const glm::quat& Q, const glm::vec3& W)
{
	return Q * glm::quat(0, W) * 0.5f;
}

export class SimulationEngine
{
public:
	SimulationEngine(const SimulationConfig& config, const SimulationData& data) : config(config), data(data)
	{}

	void start()
	{
		running = true;
		simulationThread = std::thread(&SimulationEngine::threadFunction, this);
	}

	void stop()
	{
		running = false;
		simulationThread.join();
	}

	void pause()
	{
		std::lock_guard<decltype(cvMutex)> lk(cvMutex);
		paused = true;
	}

	void resume()
	{
		std::lock_guard<decltype(cvMutex)> lk(cvMutex);
		paused = false;
		cv.notify_one();
	}

	void setConfig(const SimulationConfig& config)
	{
		const std::lock_guard<std::mutex> lock(configMutex);
		this->config = config;
	}

	void setData(const SimulationData& data)
	{
		stop();
		this->data = data;
		start();
	}

	SimulationData getData()
	{
		const std::lock_guard<std::mutex> lock(dataMutex);
		return data;
	}

	inline bool isPaused()
	{
		return paused;
	}

private:
	friend class GuiController;

	inline static constexpr float g = 9.80665f;

	SimulationConfig config;
	SimulationData data;

	std::mutex configMutex, dataMutex, cvMutex;
	std::condition_variable cv;
	std::atomic_bool running = false;
	bool paused = false;
	std::thread simulationThread;

	void threadFunction()
	{
		SimulationData localData;
		SimulationConfig localConfig;
		{
			const std::lock_guard<std::mutex> lock(configMutex);
			localConfig = config;
		}
		{
			const std::lock_guard<std::mutex> lock(dataMutex);
			localData = data;
		}

		localData.angularVelocity = { 0, localConfig.spinAngularVelocity, 0 };
		localData.rotation = glm::quat({ 0, 0, localConfig.deviation });
		while (running)
		{
			nap();

			auto clockBegin = std::chrono::steady_clock::now();

			// Copy config to local
			{
				const std::lock_guard<std::mutex> lock(configMutex);
				localConfig = config;
			}

			glm::vec3 N{ 0,0,0 };
			if (localConfig.gravity)
			{
				auto mass = localConfig.size * localConfig.size * localConfig.size * localConfig.density;
				auto r = glm::vec3(0, glm::sqrt(3.0f) / 2.0f, 0) * localConfig.size;
				auto gravityForce = glm::vec3(0, -g, 0) * mass;
				glm::vec3 rotatedGravity = glm::conjugate(localData.rotation) * gravityForce;
				N = glm::cross(r, rotatedGravity);
			}

			// Calculate k for W
			glm::vec3 k1_W = dW_dt(localData.angularVelocity, N, localConfig);
			glm::vec3 k2_W = dW_dt(localData.angularVelocity + 0.5f * localConfig.dt * k1_W, N, localConfig);
			glm::vec3 k3_W = dW_dt(localData.angularVelocity + 0.5f * localConfig.dt * k2_W, N, localConfig);
			glm::vec3 k4_W = dW_dt(localData.angularVelocity + localConfig.dt * k3_W, N, localConfig);

			localData.angularVelocity += (localConfig.dt / 6.0f) * (k1_W + 2.0f * k2_W + 2.0f * k3_W + k4_W);

			// Calculate k for Q
			glm::quat k1_Q = dQ_dt(localData.rotation, localData.angularVelocity);
			glm::quat k2_Q = dQ_dt(localData.rotation + 0.5f * localConfig.dt * k1_Q, localData.angularVelocity);
			glm::quat k3_Q = dQ_dt(localData.rotation + 0.5f * localConfig.dt * k2_Q, localData.angularVelocity);
			glm::quat k4_Q = dQ_dt(localData.rotation + localConfig.dt * k3_Q, localData.angularVelocity);

			// Update W and Q using RK4 formula
			
			localData.rotation += (localConfig.dt / 6.0f) * (k1_Q + 2.0f * k2_Q + 2.0f * k3_Q + k4_Q);
			localData.rotation = glm::normalize(localData.rotation);  // Normalize quaternion to avoid drift

			localData.t += localConfig.dt;

			// Copy data
			{
				const std::lock_guard<std::mutex> lock(dataMutex);
				data = localData;
			}
			
			// Sleep
			int ns = localConfig.dt * 1e9;
			auto clockEnd = std::chrono::steady_clock::now();
			int elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(clockEnd - clockBegin).count();

			if (elapsed < ns)
				std::this_thread::sleep_for(std::chrono::nanoseconds(ns - elapsed));
			else
				std::cerr << "Can't sleep - the simulation step took too long\n";
		}
	}

	void nap()
	{
		std::unique_lock<decltype(cvMutex)> lk(cvMutex);
		cv.wait(lk, [this] { return !paused; });
	}
};
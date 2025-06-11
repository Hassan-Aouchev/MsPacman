#pragma once
#include <string>
#include <functional>

namespace dae
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath,int screenWidth,int screenHeight, int borderSize);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;


	private:

		float FIXED_TIME_STEP = 1.0f/60.0f;
		
		const int MS_PER_FRAME = 1000 / 60;  // 60 FPS, 1000 ms = 1 second
	};
}
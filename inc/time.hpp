#pragma once

#include <chrono>
#include <iostream>

class Time
{
public:
	Time()
	: m_start(std::chrono::high_resolution_clock::now())
	{}

	~Time()
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> time = end - m_start;
		std::cout << time.count() << std::endl;
	}

private:
	std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::_V2::system_clock::duration>  m_start;
};

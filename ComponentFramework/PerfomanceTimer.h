#ifndef PERFOMANCETIMER_H
#define PERFOMANCETIMER_H

#include <chrono>
#include <iostream>


class PerfomanceTimer
{
public:
	PerfomanceTimer(const char* name)
		:m_Name(name), m_Stopped(false) {
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~PerfomanceTimer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast
			<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast
			<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;

		std::cout << "Duration: " << duration << "ms" << std::endl;
	}


private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#endif // !define PERFOMANCETIMER_H


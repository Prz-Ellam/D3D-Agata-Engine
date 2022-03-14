#include "pch.h"
#include "Timer.h"

namespace Agata {

	Timer::Timer() {

		//QueryPerformanceCounter(&m_Begin);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_Frequency = static_cast<double>(frequency.QuadPart);
		//pause = false;

	}

	void Timer::Start() {

		QueryPerformanceCounter(&m_Begin);
		pause = false;

	}

	void Timer::Restart() {

		m_Begin = m_End;

	}

	void Timer::Stop() {

		QueryPerformanceCounter(&m_End);
		pause = true;

	}

	double Timer::GetMiliseconds() {

		if (pause) {
			return static_cast<double>((m_End.QuadPart - m_Begin.QuadPart)) / m_Frequency;
		}
		else {
			return -1.0;
		}

	}

}
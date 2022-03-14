#ifndef AGATA_D11_TIMER_H
#define AGATA_D11_TIMER_H
#pragma once

#include <Windows.h>

namespace Agata {

	class Timer {
	public:
		Timer();
		Timer(const Timer& rhs) = default;
		Timer(Timer&& rhs) noexcept = default;
		virtual ~Timer() = default;

		void Start();
		void Restart();
		void Stop();

		double GetMiliseconds();
	private:
		LARGE_INTEGER m_Begin, m_End;
		double m_Frequency;
		bool pause;
	};

}

#endif

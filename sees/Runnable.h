#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include <atomic>
#include <thread>

class Runnable
{
	Runnable(Runnable const&) = delete;
	Runnable& operator =(Runnable const&) = delete;

public:
	Runnable();
	virtual ~Runnable();

	void stop();
	void start();
	void stop_point();
	bool is_running();

protected:
	virtual void run() = 0;
	std::atomic<bool> m_stop;

private:
	std::thread m_thread;
};

#endif // RUNNABLE_H

#include "Runnable.h"

Runnable::Runnable() : m_stop(true), m_thread() 
{ 
	;	
}

Runnable::~Runnable()
{ 
	stop(); 
}

void Runnable::start() 
{
	// Create new thread
	m_thread = std::thread(&Runnable::run, this); 
	m_thread.detach();
}

void Runnable::stop() 
{ 	
	if(m_stop == true)
		return;

	m_stop = true; 
	//m_thread.join(); 
}

void Runnable::stop_point() 
{ 
	if(m_stop) 
	{
		throw -1; 
	}
}

bool Runnable::is_running()
{
	return !m_stop;
}


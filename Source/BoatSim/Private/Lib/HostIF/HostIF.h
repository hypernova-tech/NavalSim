#pragma once

class IHostIF
{
public:
	virtual void OnRecievedMessage(void* p_commands) = 0;
	virtual void* GetOwningActor() = 0;
};
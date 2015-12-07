#pragma once

class Drone;

class State {
public:
	virtual void Update(Drone&, float ) = 0;
};

class Patrol : public State {
	
	static Patrol* instance;

	Patrol();
public:
	static Patrol* Instance();

	virtual void Update(Drone&, float);
};

class Trace : public State {

	static Trace* instance;

	Trace();
public:
	static Trace* Instance();

	virtual void Update(Drone&, float);
};
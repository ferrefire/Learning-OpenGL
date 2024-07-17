#include "cinematic.hpp"
#include "manager.hpp"
#include "time.hpp"
#include <iostream>

Cinematic::Cinematic()
{

}

Cinematic::~Cinematic()
{

}

void Cinematic::AddKeyPosition(glm::vec3 value, float duration)
{
	CinematicKey key;
	key.value = value;
	key.duration = duration;

	keyPositions.push_back(key);
}

void Cinematic::AddKeyRotation(glm::vec3 value, float duration)
{
	CinematicKey key;
	key.value = value;
	key.duration = duration;

	keyRotations.push_back(key);
}

void Cinematic::Start()
{
	bool ended = true;

	if (positionIndex + 1 < keyPositions.size())
	{
		Manager::camera.SetPosition(glm::mix(keyPositions[positionIndex].value, keyPositions[positionIndex + 1].value,
			positionDuration / keyPositions[positionIndex + 1].duration));

		positionDuration += Time::deltaTime;

		if (positionDuration >= keyPositions[positionIndex + 1].duration)
		{
			positionIndex++;
			positionDuration = 0;
		}

		ended = false;
	}

	if (rotationIndex + 1 < keyRotations.size())
	{
		Manager::camera.SetRotation(glm::mix(keyRotations[rotationIndex].value, keyRotations[rotationIndex + 1].value,
		rotationDuration / keyRotations[rotationIndex + 1].duration));

		rotationDuration += Time::deltaTime;

		if (rotationDuration >= keyRotations[rotationIndex + 1].duration)
		{
			rotationIndex++;
			rotationDuration = 0;
		}

		ended = false;
	}

	if (running && ended)
	{
		running = false;
		float fps = float(totalFrames) / totalFrameTime;
		std::cout << fps << std::endl; 
	}
	else
	{
		totalFrames++;
		totalFrameTime += Time::deltaTime;
	}
}

void Cinematic::Stop()
{

}

void Cinematic::Pause()
{

}

void Cinematic::Resume()
{

}

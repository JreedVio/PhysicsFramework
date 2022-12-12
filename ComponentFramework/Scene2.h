#ifndef SCENE2_H
#define SCENE2_H

#include "Scene.h"
#include <unordered_map>
#include "Actor.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Ray.h"
#include <stdio.h>
#include <irrKlang.h>

using namespace irrklang;

struct Scene2 : public Scene
{
	const int JOYSTICK_DEAD_ZONE = 8000;

	bool renderMeshes = true;
	bool renderCollisionShapes = true;
	explicit Scene2() {};
	virtual ~Scene2() {};

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render(ImVec4 bgColour) const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	// Keep track of Actors from XML asset manager
	std::unordered_map< std::string, Ref<Component>> actors;
	// We only have one camera and light, so they don't need to be in the map
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	// We'll use a ray to click on our geometry objects 
	GEOMETRY::RayIntersectionInfo rayInfo;
	Ref<Actor> pickedActor;
	//PHYSICS::PhysicsObject pickedPhysicsObject;
	MATH::Vec3 intersectionPoint;
	bool haveClickedOnSomething = false;

	SDL_Joystick* gGameController = NULL;

	ISoundEngine* engine;
};

#endif


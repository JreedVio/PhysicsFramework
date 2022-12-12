#ifndef SCENE_H
#define SCENE_H
union SDL_Event;
#include "Component.h"
#include "imgui.h"

class Scene{
public:	
	virtual ~Scene() {}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render(ImVec4 bgColour) const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	
};
#endif
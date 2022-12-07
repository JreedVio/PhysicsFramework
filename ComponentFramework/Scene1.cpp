#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"
#include "XMLAssetManager.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"

using namespace GEOMETRY;

bool Scene1::OnCreate()
{
	XMLAssetManager assetManager;
	std::vector<std::string> names{
		"ActorGameBoard" , "ActorChecker1", "ActorChecker2",
		"ActorDefaultChecker", "ActorTinyChecker", "ActorSkull",
		"ActorCube", "ActorCube2",
		"ActorMario"
	};
	for (const auto& name : names) {
		auto asset = assetManager.xmlAssets.find(name);
		actors[name] = asset->second;
	}
	camera = std::dynamic_pointer_cast<CameraActor>(assetManager.xmlAssets.find("Camera1")->second);
	light = std::dynamic_pointer_cast<LightActor>(assetManager.xmlAssets.find("Light1")->second);
	return true;
}

void Scene1::OnDestroy()
{
	actors.clear();
}


void Scene1::HandleEvents(const SDL_Event& sdlEvent)
{
	Ref<TransformComponent> cameraTransform = camera->GetComponent <TransformComponent>();
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			cameraTransform->SetTransform(cameraTransform->pos, cameraTransform->GetOrientation() * QMath::angleAxisRotation(-2.0f, Vec3(1.0f, 0.0f, 0.0f)));
			camera->UpdateViewMatrix();

		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_M) {
			renderMeshes = !renderMeshes;
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_C) {
			renderCollisionShapes = !renderCollisionShapes;
		}

		break;

	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
			// TODO: get a ray pointing into the world
			// We have the x, y pixel coordinates
			// Need to convert this into world space to build our ray
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			Matrix4 ndc = MMath::viewportNDC(viewport[2], viewport[3]);
			Matrix4 view = camera->GetViewMatrix();
			Matrix4 projection = camera->GetProjectionMatrix();
			Matrix4 rayTransform = MMath::inverse(ndc * projection * view);
			Vec3 ray_worldDirection = VMath::normalize(rayTransform * Vec3(mouseCoords.x,
				mouseCoords.y, 1.0f));
			Vec3 ray_worldStart(0.0f, 0.0f, 0.0f);


			const GEOMETRY::Ray ray{ ray_worldStart, ray_worldDirection };
			haveClickedOnSomething = false;


			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value
			for (auto it = actors.begin(); it != actors.end(); ++it) {
				Ref<Actor> actor = std::dynamic_pointer_cast<Actor>(it->second);
				Ref<TransformComponent> transformComponent = actor->GetComponent <TransformComponent>();
				Ref<ShapeComponent> shapeComponent = actor->GetComponent <ShapeComponent>();
				if (shapeComponent->shapeType == ShapeType::sphere
					|| shapeComponent->shapeType == ShapeType::cylinder
					|| shapeComponent->shapeType == ShapeType::capsule
					|| shapeComponent->shapeType == ShapeType::box
					) {

					// TODO: Transform the ray into the local space of the object and check if a collision occured
					Vec3 rayStartInObjectSpace = MMath::inverse(actor->GetModelMatrix()) * ray.start;
					Vec3 rayDirInObjectSpace = MMath::inverse(actor->GetModelMatrix()).multiplyWithoutDividingOutW(Vec4(ray.dir, 0.0f));
					GEOMETRY::Ray rayInObjectSpace{ rayStartInObjectSpace, rayDirInObjectSpace };
					rayInfo = shapeComponent->shape->rayIntersectionInfo(rayInObjectSpace);

					if (rayInfo.isIntersected) {
						// TO DO: Check this is the first object hit by the ray. 
						//Ignore anything hit behind that object
						std::cout << "You picked: " << it->first << std::endl;
						intersectionPoint = actor->GetModelMatrix() * rayInfo.intersectionPoint;
						pickedActor = actor;
						haveClickedOnSomething = true;
						//std::cout << "Hello";

					}
				}

			}
			
		}
		break;

	default:
		break;
	}

}

void Scene1::Update(const float deltaTime)
{
}

void Scene1::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatricesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	// Let it go
	glBindTexture(GL_TEXTURE_2D, 0);

	for (auto it = actors.begin(); it != actors.end(); ++it) {
		Ref<Actor> actor = std::dynamic_pointer_cast<Actor>(it->second);
		glUseProgram(actor->GetComponent<ShaderComponent>()->GetProgram());
		glUniformMatrix4fv(actor->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, actor->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, actor->GetComponent<MaterialComponent>()->getTextureID());
		if (renderMeshes) {
			actor->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
		if (renderCollisionShapes) {
			// Drawing the primitive geometry associated with the mesh to help debug ray intersects, culling, and collision detection
			actor->GetComponent<ShapeComponent>()->Render();
		}
	}
}


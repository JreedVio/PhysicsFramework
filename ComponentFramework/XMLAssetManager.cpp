#include "XMLAssetManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"
#include "MaterialComponent.h"
#include "PhysicsComponent.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "QMath.h"

XMLAssetManager::XMLAssetManager()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Scene0.xml");
	bool status = doc.Error();
	if (status) {
		std::cout << doc.ErrorIDToName(doc.ErrorID()) << std::endl;
		return;
	}
	// These are referred to as nodes. Get the top of the system
	tinyxml2::XMLElement* rootData = doc.RootElement();
	tinyxml2::XMLElement* sceneData = rootData->FirstChildElement("Scene0");

	for (const tinyxml2::XMLElement* child = rootData->FirstChildElement("Scene0")->FirstChildElement();
		child;
		child = child->NextSiblingElement())
	{
		AddMaterial(child);
		AddShader(child);
		AddCamera(child);
		AddLight(child);
		
		if (std::string(child->Name()) == "Mesh") {
			AddComponent<MeshComponent>(child->Attribute("name"), nullptr, child->Attribute("filename"));
			
			AddCylinderShape(child);
			AddSphereShape(child);
			AddCapsuleShape(child);
			AddBoxShape(child);
		}
	}

	// Now we have all the shared assets ready, time to build the actors
	for (const tinyxml2::XMLElement* child = rootData->FirstChildElement("Scene0")->FirstChildElement("Actor");
		child;
		child = child->NextSiblingElement())
	{
		if (std::string(child->Name()) == "Actor") {
			Component* parent = nullptr;
			if (std::string(child->Attribute("parent")) == "none") {
				parent = nullptr;
			}
			if (std::string(child->Attribute("parent")) == "ActorGameBoard") {
				parent = GetComponent<Actor>("ActorGameBoard").get();
			}

			Ref<Actor> actor = std::make_shared<Actor>(parent);
			// Add shared assets to the actor
			AddMeshToActor(child, actor);
			AddShaderToActor(child, actor);
			AddMaterialToActor(child, actor);
			AddShapeToActor(child, actor);
			// The transform is unique for the actor. Needs the parent too
			AddTransformToActor(child, actor, parent);
			AddPhysicsToActor(child, actor, parent);

			actor->OnCreate();
			AddComponent(child->Attribute("actorname"), actor);
		}
	}
}
XMLAssetManager::~XMLAssetManager()
{
	xmlAssets.clear();
}

void XMLAssetManager::AddSphereShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Sphere") {
		float sphereRadius = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("radius");
		
		Vec3 sphereCentre;
		sphereCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreX");
		sphereCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreY");
		sphereCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Sphere")->FloatAttribute("centreZ");

		GEOMETRY::Sphere sphere;
		sphere.set(sphereCentre.x, sphereCentre.y, sphereCentre.z, sphereRadius);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, sphere);
	}
}

void XMLAssetManager::AddCylinderShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Cylinder") {
		float cylinderRadius = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("radius");

		Vec3 topCapCentre;
		topCapCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("topCapCentreX");
		topCapCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("topCapCentreY");
		topCapCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("topCapCentreZ");

		Vec3 bottomCapCentre;
		bottomCapCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("bottomCapCentreX");
		bottomCapCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("bottomCapCentreY");
		bottomCapCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Cylinder")->FloatAttribute("bottomCapCentreZ");

		GEOMETRY::Cylinder cylinder;
		cylinder.set(cylinderRadius, bottomCapCentre, topCapCentre);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, cylinder);
	}
}

void XMLAssetManager::AddCapsuleShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Capsule") {
		float cylinderRadius = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("radius");

		Vec3 topSphereCentre;
		topSphereCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("topSphereCentreX");
		topSphereCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("topSphereCentreY");
		topSphereCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("topCapCentreZ");

		Vec3 bottomSphereCentre;
		bottomSphereCentre.x = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("bottomSphereCentreX");
		bottomSphereCentre.y = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("bottomSphereCentreY");
		bottomSphereCentre.z = child->FirstChildElement("Shape")->FirstChildElement("Capsule")->FloatAttribute("bottomSphereCentreZ");

		GEOMETRY::Capsule capsule;
		capsule.set(cylinderRadius, bottomSphereCentre, topSphereCentre);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, capsule);
	}
}

void XMLAssetManager::AddBoxShape(const tinyxml2::XMLElement* child)
{
	if (std::string(child->FirstChildElement("Shape")->FirstChildElement()->Name()) == "Box") {
		Vec3 centre;
		centre.x = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("centreX");
		centre.y = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("centreY");
		centre.z = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("centreZ");

		Vec3 halfExtent;
		halfExtent.x = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("halfExtentX");
		halfExtent.y = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("halfExtentY");
		halfExtent.z = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("halfExtentZ");

		float angleDeg;
		angleDeg = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("angleDeg");

		Vec3 axis;
		axis.x = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("axisX");
		axis.y = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("axisY");
		axis.z = child->FirstChildElement("Shape")->FirstChildElement("Box")->FloatAttribute("axisZ");

		MATH::Quaternion orientation = MATH::QMath::angleAxisRotation(angleDeg, axis);

		GEOMETRY::Box box;
		box.set(centre, halfExtent, orientation);

		AddComponent<ShapeComponent>(child->FirstChildElement("Shape")->Attribute("name"), nullptr, box);
	}
}

void XMLAssetManager::AddMaterial(const tinyxml2::XMLElement* child)
{
	if (std::string(child->Name()) == "Material") {
		AddComponent<MaterialComponent>(child->Attribute("name"), nullptr, child->Attribute("filename"));
	}
}

void XMLAssetManager::AddShader(const tinyxml2::XMLElement* child)
{
	if (std::string(child->Name()) == "Shader") {
		AddComponent<ShaderComponent>(child->Attribute("name"), nullptr, child->Attribute("vertFilename"), child->Attribute("fragFilename"));
	}
}

void XMLAssetManager::AddCamera(const tinyxml2::XMLElement* child)
{
	// Build camera
	Vec3 cameraPos;
	Vec3 axis;
	Vec3 scale;
	float angleDeg = 0.0f;
	Component* cameraParent = nullptr;

	if (child->Attribute("cameraname")) {
		if (child->FirstChildElement("Transform")) {
			cameraPos.x = child->FirstChildElement("Transform")->FloatAttribute("posx");
			cameraPos.y = child->FirstChildElement("Transform")->FloatAttribute("posy");
			cameraPos.z = child->FirstChildElement("Transform")->FloatAttribute("posz");
			angleDeg = child->FirstChildElement("Transform")->FloatAttribute("angleDeg");;
			axis.x = child->FirstChildElement("Transform")->FloatAttribute("axisx");
			axis.y = child->FirstChildElement("Transform")->FloatAttribute("axisy");
			axis.z = child->FirstChildElement("Transform")->FloatAttribute("axisz");
			scale.x = child->FirstChildElement("Transform")->FloatAttribute("scalex");
			scale.y = child->FirstChildElement("Transform")->FloatAttribute("scaley");
			scale.z = child->FirstChildElement("Transform")->FloatAttribute("scalez");

			if (std::string(child->Attribute("parent")) == "none") {
				cameraParent = nullptr;
			}
		}
	}

	if (std::string(child->Name()) == "Camera") {
		Ref<CameraActor> camera = std::make_shared<CameraActor>(cameraParent);
		camera->AddComponent<TransformComponent>(nullptr, cameraPos, QMath::angleAxisRotation(angleDeg, axis));
		camera->OnCreate();
		AddComponent(child->Attribute("cameraname"), camera);
	}
}

void XMLAssetManager::AddLight(const tinyxml2::XMLElement* child)
{
	// Build Light
	Vec3 lightPos;
	Vec4 colour;
	Vec3 falloff;
	Component* lightParent = nullptr;
	LightStyle lightstyle;
	float intensity = 1.0f;
	if (child->Attribute("lightname")) {
		lightPos.x = child->FloatAttribute("posx");
		lightPos.y = child->FloatAttribute("posy");
		lightPos.z = child->FloatAttribute("posz");

		colour.x = child->FloatAttribute("red");
		colour.y = child->FloatAttribute("green");
		colour.z = child->FloatAttribute("blue");
		colour.w = child->FloatAttribute("alpha");

		intensity = child->FloatAttribute("intensity");;

		falloff.x = child->FloatAttribute("falloffx");
		falloff.y = child->FloatAttribute("falloffy");
		falloff.z = child->FloatAttribute("falloffz");

		if (std::string(child->Attribute("parent")) == "none") {
			lightParent = nullptr;
		}

		if (std::string(child->Attribute("lightstyle")) == "DirectionLight") {
			lightstyle = LightStyle::DirectionLight;
		}

	}

	if (std::string(child->Name()) == "Light") {
		Ref<LightActor> light = std::make_shared<LightActor>(lightParent, lightstyle, lightPos, colour, intensity, falloff);
		light->OnCreate();
		AddComponent(child->Attribute("lightname"), light);
	}
}

void XMLAssetManager::AddMeshToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Mesh")) {
		const char* name = child->FirstChildElement("Mesh")->Attribute("name");
		Ref<MeshComponent> mesh = GetComponent<MeshComponent>(name);
		actor->AddComponent<MeshComponent>(mesh);
	}

}

void XMLAssetManager::AddShaderToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Shader")) {
		const char* name = child->FirstChildElement("Shader")->Attribute("name");
		Ref<ShaderComponent> shader = GetComponent<ShaderComponent>(name);
		actor->AddComponent<ShaderComponent>(shader);
	}
}

void XMLAssetManager::AddMaterialToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (child->FirstChildElement("Material")) {
		const char* name = child->FirstChildElement("Material")->Attribute("name");
		Ref<MaterialComponent> material = GetComponent<MaterialComponent>(name);
		actor->AddComponent<MaterialComponent>(material);
	}
}

void XMLAssetManager::AddShapeToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor)
{
	if (std::string(child->FirstChildElement("Shape")->Name()) == "Shape") {
		const char* name = child->FirstChildElement("Shape")->Attribute("name");
		Ref<ShapeComponent> shape = GetComponent<ShapeComponent>(name);
		actor->AddComponent<ShapeComponent>(shape);
	}
}

void XMLAssetManager::AddTransformToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent)
{
	if (child->FirstChildElement("Transform")) {
		Vec3 pos;
		Vec3 axis;
		Vec3 scale;
		pos.x = child->FirstChildElement("Transform")->FloatAttribute("posx");
		pos.y = child->FirstChildElement("Transform")->FloatAttribute("posy");
		pos.z = child->FirstChildElement("Transform")->FloatAttribute("posz");
		float angleDeg = child->FirstChildElement("Transform")->FloatAttribute("angleDeg");;
		axis.x = child->FirstChildElement("Transform")->FloatAttribute("axisx");
		axis.y = child->FirstChildElement("Transform")->FloatAttribute("axisy");
		axis.z = child->FirstChildElement("Transform")->FloatAttribute("axisz");
		scale.x = child->FirstChildElement("Transform")->FloatAttribute("scalex");
		scale.y = child->FirstChildElement("Transform")->FloatAttribute("scaley");
		scale.z = child->FirstChildElement("Transform")->FloatAttribute("scalez");
		
		actor->AddComponent<TransformComponent>(parent, pos, QMath::angleAxisRotation(angleDeg, axis), scale);
	}
}

void XMLAssetManager::AddPhysicsToActor(const tinyxml2::XMLElement* child, Ref<Actor> actor, Component* parent)
{
	if (child->FirstChildElement("Physics")) {
		float mass = child->FirstChildElement("Physics")->FloatAttribute("mass");

		Matrix3 rotI;
		rotI[0] = child->FirstChildElement("Physics")->FloatAttribute("rotIe1");
		rotI[4] = child->FirstChildElement("Physics")->FloatAttribute("rotIe2");
		rotI[8] = child->FirstChildElement("Physics")->FloatAttribute("rotIe3");

		Vec3 vel;
		vel.x = child->FirstChildElement("Physics")->FloatAttribute("velx");
		vel.y = child->FirstChildElement("Physics")->FloatAttribute("vely");
		vel.z = child->FirstChildElement("Physics")->FloatAttribute("velz");

		Vec3 acc;
		acc.x = child->FirstChildElement("Physics")->FloatAttribute("accx");
		acc.y = child->FirstChildElement("Physics")->FloatAttribute("accy");
		acc.z = child->FirstChildElement("Physics")->FloatAttribute("accz");

		Vec3 angularVel;
		angularVel.x = child->FirstChildElement("Physics")->FloatAttribute("angularVelx");
		angularVel.y = child->FirstChildElement("Physics")->FloatAttribute("angularVely");
		angularVel.z = child->FirstChildElement("Physics")->FloatAttribute("angularVelz");

		Vec3 angularAcc;
		angularAcc.x = child->FirstChildElement("Physics")->FloatAttribute("angularAccx");
		angularAcc.y = child->FirstChildElement("Physics")->FloatAttribute("angularAccy");
		angularAcc.z = child->FirstChildElement("Physics")->FloatAttribute("angularAccz");

		Vec3 pos = actor->GetComponent<TransformComponent>()->pos;
		Quaternion orientation = actor->GetComponent<TransformComponent>()->orientation;

		actor->AddComponent<PhysicsComponent>(parent, mass, rotI, pos, vel, acc, orientation, angularVel, angularAcc);
	}
}




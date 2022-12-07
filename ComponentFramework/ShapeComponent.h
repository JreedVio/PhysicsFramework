// Umer Noor 2022
// Original code from/taught by Dr. Scott Fielder

#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include "Component.h"
#include "Shape.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Capsule.h"
#include "Box.h"

enum class ShapeType {
	sphere,
	cylinder,
	capsule,
	box
};

class ShapeComponent: public Component{
	// Scott hates these implicit copy/moves
	ShapeComponent(const ShapeComponent&) = delete;
	ShapeComponent(ShapeComponent&&) = delete;
	ShapeComponent& operator = (const ShapeComponent&) = delete;
	ShapeComponent& operator = (ShapeComponent&&) = delete;

public:
	ShapeComponent(Component*, GEOMETRY::Sphere);
	ShapeComponent(Component*, GEOMETRY::Cylinder);
	ShapeComponent(Component*, GEOMETRY::Capsule);
	ShapeComponent(Component*, GEOMETRY::Box);

	Ref<GEOMETRY::Shape> shape;
	ShapeType shapeType;
	virtual ~ShapeComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

#endif

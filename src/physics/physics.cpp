#include <physics/physics.hpp>
using namespace engine;
PhysicsWorld::PhysicsWorld() : space(nullptr) {}
PhysicsWorld::~PhysicsWorld()
{
    if (space)
        cpSpaceFree(space);
}
void PhysicsWorld::init(float gravity)
{
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, gravity));
}
void PhysicsWorld::step(float dt)
{
    cpSpaceStep(space, dt);
}
void PhysicsWorld::addRigidBody(RigidBody *body)
{
    cpSpaceAddBody(space, body->body);
    cpSpaceAddShape(space, body->shape);
}
void PhysicsWorld::addStaticBody(StaticBody *body)
{
    cpSpaceAddShape(space, body->shape);
}
RigidBody::RigidBody(cpSpace *space, const glm::vec2 &position, float mass, float width, float height)
{
    cpFloat moment = cpMomentForBox(mass, width / PPM, height / PPM);
    body = cpBodyNew(mass, moment);
    cpBodySetPosition(body, glmToCp(position));
    float x = glmToCp(glm::vec2(position.x + width / 2.0f, position.y + height / 2.0f)).x; // Debug log
    float y = glmToCp(glm::vec2(position.x + width / 2.0f, position.y + height / 2.0f)).y; // Debug log
    shape = cpBoxShapeNew(body, width / PPM, height / PPM, 0.0f);
    cpShapeSetFriction(shape, 0.7f);
}
RigidBody::~RigidBody()
{
    cpShapeFree(shape);
    cpBodyFree(body);
}
void RigidBody::applyForce(const glm::vec2 &force)
{
    cpBodyApplyForceAtLocalPoint(body, glmToCp(force), cpvzero);
}
glm::vec2 RigidBody::getPosition() const
{
    return cpToGlm(cpBodyGetPosition(body));
}
float RigidBody::getAngle() const
{
    return cpBodyGetAngle(body);
}
StaticBody::StaticBody(cpSpace *space, float width, float height, glm::vec2 center) : space(space)
{
    cpBody *staticBody = cpSpaceGetStaticBody(space);
    shape = cpBoxShapeNew(staticBody, width / PPM, height / PPM, 0.0f);
    cpShapeSetFriction(shape, 0.7f);
    cpBodySetPosition(staticBody, glmToCp(center));
}
StaticBody::~StaticBody()
{
    if (shape && space)
    {
        cpSpaceRemoveShape(space, shape);
        cpShapeFree(shape);
    }
}
void PhysicsWorld::removeRigidBody(RigidBody *body)
{
    cpSpaceRemoveShape(space, body->shape);
    cpSpaceRemoveBody(space, body->body);
}
void PhysicsWorld::removeStaticBody(StaticBody *body)
{
    cpSpaceRemoveShape(space, body->shape);
}
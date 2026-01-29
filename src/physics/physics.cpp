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
RigidBody::RigidBody(cpSpace *space, const glm::vec2 &position, float mass, float width, float height, const std::string &name, float bounciness) : Body(name)
{
    cpFloat moment = cpMomentForBox(mass, width / PPM, height / PPM);
    body = cpBodyNew(mass, moment);
    cpBodySetPosition(body, glmToCp(position));
    shape = cpBoxShapeNew(body, width / PPM, height / PPM, 0.0f);
    cpShapeSetElasticity(shape, static_cast<cpFloat>(bounciness) / 100.0f);
    cpShapeSetFriction(shape, 0.7f);
    cpShapeSetUserData(shape, this);
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
StaticBody::StaticBody(cpSpace *space, float width, float height, glm::vec2 center, const std::string &name, float bounciness) : space(space), Body(name)
{
    cpBody *staticBody = cpSpaceGetStaticBody(space);
    shape = cpBoxShapeNew(staticBody, width / PPM, height / PPM, 0.0f);
    cpShapeSetFriction(shape, 0.7f);
    cpShapeSetElasticity(shape, static_cast<cpFloat>(bounciness) / 100.0f);
    cpBodySetPosition(staticBody, glmToCp(center));
    cpShapeSetUserData(shape, this);
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
void PhysicsWorld::setCollisionCallback(std::function<void(void *, void *)> cb)
{
    collisionCallback = cb;
    cpCollisionHandler *handler = cpSpaceAddDefaultCollisionHandler(space);
    handler->beginFunc = collisionHandler;
    handler->userData = this;
}
cpBool PhysicsWorld::collisionHandler(cpArbiter *arb, cpSpace *space, void *userData)
{
    PhysicsWorld *world = static_cast<PhysicsWorld *>(userData);
    if (world->collisionCallback)
    {
        cpShape *shapeA;
        cpShape *shapeB;
        cpArbiterGetShapes(arb, &shapeA, &shapeB);
        world->collisionCallback(cpShapeGetUserData(shapeA), cpShapeGetUserData(shapeB));
    }
    return cpTrue;
}
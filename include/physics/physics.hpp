#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>
namespace engine
{
    constexpr float PPM = 50.0f; // Pixels Per Meter
    inline cpVect glmToCp(const glm::vec2 &p)
    {
        return cpv(p.x / PPM, p.y / PPM);
    }
    inline glm::vec2 cpToGlm(const cpVect &v)
    {
        return glm::vec2(v.x * PPM, v.y * PPM);
    }
    class RigidBody
    {
    public:
        RigidBody(cpSpace *space, const glm::vec2 &position, float mass, float width, float height);
        ~RigidBody();
        void applyForce(const glm::vec2 &force);
        glm::vec2 getPosition() const;
        float getRotation() const { return cpBodyGetAngle(body); }
        float getAngle() const;
        void disableRotation()
        {
            cpBodySetMoment(body, INFINITY);
        }
        void enableRotation(float moment)
        {
            cpBodySetMoment(body, moment);
        }
        void resetVelocity()
        {
            cpBodySetVelocity(body, cpvzero);
            cpBodySetAngularVelocity(body, 0.0f);
        }
        cpBody *body;
        cpShape *shape;
    };
    class StaticBody
    {
    public:
        StaticBody(cpSpace *space, float width, float height, glm::vec2 center);
        ~StaticBody();
        cpShape *shape;
        cpSpace *space;  // Store reference to space for cleanup
    };
    class PhysicsWorld
    {
    public:
        PhysicsWorld();
        ~PhysicsWorld();
        void init(float gravity);
        void step(float dt);
        cpSpace *getSpace() { return space; }
        void addRigidBody(RigidBody *body);
        void addStaticBody(StaticBody *body);
        void removeRigidBody(RigidBody *body);
        void removeStaticBody(StaticBody *body);

    private:
        cpSpace *space;
    };

};
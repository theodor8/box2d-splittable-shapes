#include "ContactListener.h"

#include "box2d/box2d.h"
#include "GameObject.h"



void ContactListener::BeginContact(b2Contact *contact) {}

void ContactListener::EndContact(b2Contact *contact) {}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();

    uintptr_t bodyAUserData = bodyA->GetUserData().pointer;
    uintptr_t bodyBUserData = bodyB->GetUserData().pointer;

    if (bodyAUserData) ((GameObject*)bodyAUserData)->PostSolve(bodyB, contact, impulse);
    if (bodyBUserData) ((GameObject*)bodyBUserData)->PostSolve(bodyA, contact, impulse);
}
#include "BreakableObject.h"

#include "common.h"

extern b2World* world;

void BreakableObject::Step()
{
    // Split

    if (splitQueue.size() > 0)
    {
        b2Vec2 splitPt1 = splitQueue[0][0];
        b2Vec2 splitPt2 = splitQueue[0][1];
        splitQueue.erase(splitQueue.begin());

        b2BodyDef newBodyDef;
        newBodyDef.type = b2_dynamicBody;
        newBodyDef.angle = body->GetAngle();
        newBodyDef.angularVelocity = body->GetAngularVelocity();
        newBodyDef.linearVelocity = body->GetLinearVelocity();
        newBodyDef.position = body->GetPosition();
        
        b2Fixture* node = body->GetFixtureList();
        while (node)
        {
            b2Fixture* f = node;
            node = node->GetNext();
            
            auto newShapes = SplitFixture(f, splitPt1, splitPt2);
            if (!newShapes.size()) continue;

            b2FixtureDef newFixtureDef;
            newFixtureDef.density = f->GetDensity();
            newFixtureDef.friction = f->GetFriction();
            newFixtureDef.restitution = f->GetRestitution();
            body->DestroyFixture(f);

            for (auto& newShape : newShapes)
            {
                newFixtureDef.shape = &newShape;
                auto newBody = CreateGameObject<BreakableObject>(world->CreateBody(&newBodyDef), strength);
                newBody->body->CreateFixture(&newFixtureDef);
                newBody->splitQueue = splitQueue;
            }
        }
    }

    splitQueue.clear();

    GameObject::Step();

}

void BreakableObject::PostSolve(b2Body* other, b2Contact* contact, const b2ContactImpulse* impulse)
{
    const b2Manifold* manifold = contact->GetManifold();
    if (manifold->pointCount == 0) return;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    for (int32 i = 0; i < manifold->pointCount; ++i)
    {
        float totalImpulse = impulse->normalImpulses[i] + impulse->tangentImpulses[i];

        if (totalImpulse <= strength) continue;
        b2Vec2 contactPoint = worldManifold.points[i];
        if (!contactPoint.x && !contactPoint.y) continue;

        b2Vec2 contactToBody = body->GetWorldCenter() - contactPoint;
        float splitAngle = (float)(rand() % 20 - 10) * M_PI / 180.0f;
        b2Vec2 rotatedContactToBody = rotateVec2(contactToBody, splitAngle);
        rotatedContactToBody *= 1000.0f; // Make sure the other point is outside shape

        splitQueue.push_back({contactPoint, contactPoint + rotatedContactToBody});

    }
    
}
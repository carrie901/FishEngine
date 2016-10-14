#ifndef Collider_hpp
#define Collider_hpp

#include "FishEngine.hpp"
#include "Component.hpp"
#include "Bounds.hpp"
#include "Ray.hpp"

namespace physx {
    class PxShape;
    class PxRigidDynamic;
}

namespace FishEngine
{
    class Collider : public Component
    {
    public:
        Vector3 ClosestPointOnBounds(const Vector3& position);
        //bool Raycast(const Ray& ray, RaycastHit* out_hitInfo, const float maxDistance);
        
        physx::PxShape* physicsShape();
        
    protected:
        std::shared_ptr<Rigidbody> m_attachedRigidbody;
        Bounds  m_bounds;
        float   m_contactOffset;
        bool    m_enabled;
        bool    m_isTrigger;
        
        virtual void CreatePhysicsShape() = 0;
        
        physx::PxShape* m_physxShape = nullptr;
        //physx::PxRigidDynamic* m_physxRigidDynamic;
    };
}

#endif // Collider_hpp

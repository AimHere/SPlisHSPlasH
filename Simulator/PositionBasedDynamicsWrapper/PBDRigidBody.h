#ifndef __PBDRigidBody_h__
#define __PBDRigidBody_h__

#include "SPlisHSPlasH/Common.h"
#include "SPlisHSPlasH/RigidBodyObject.h"
#include "Simulation/RigidBody.h"
#include "SPlisHSPlasH/TimeManager.h"

namespace SPH 
{	
	class PBDRigidBody : public RigidBodyObject 
	{
	protected: 
		PBD::RigidBody *m_rigidBody;	

	public:
		PBDRigidBody(PBD::RigidBody* rigidBody) : RigidBodyObject(), m_rigidBody(rigidBody) { m_isAnimated = false; }

		virtual bool isDynamic() const { return m_rigidBody->getMass() != 0.0; }
		virtual void setIsAnimated(const bool b) { }

		virtual Real const getMass() const { return m_rigidBody->getMass(); }
		virtual Vector3r const& getPosition() const { return m_rigidBody->getPosition(); }
		virtual Vector3r const& getVelocity() const { return m_rigidBody->getVelocity(); }
		virtual void setVelocity(const Vector3r &v) { m_rigidBody->setVelocity(v); }
		virtual Quaternionr const& getRotation() const { return m_rigidBody->getRotation(); }
		virtual Vector3r const& getAngularVelocity() const { return m_rigidBody->getAngularVelocity(); }
		virtual void setAngularVelocity(const Vector3r &v) { m_rigidBody->setAngularVelocity(v); }


		virtual Vector3r const& getOldPosition() const { return m_rigidBody->getOldPosition(); }
		virtual Vector3r const& getLastPosition() const { return m_rigidBody->getLastPosition(); }
		virtual Vector3r const& getPosition0() const { return m_rigidBody->getPosition0(); }

		
		virtual void setPosition(const Vector3r &x) 
		{ 
			std::cout << __func__ << ":PBR SetPosition: " << x[0] << "," << x[1] << "," << x[2] << "\n";
			m_rigidBody->setPosition(x); 
			m_rigidBody->getGeometry().updateMeshTransformation(m_rigidBody->getPosition(), m_rigidBody->getRotationMatrix());
		}

		virtual void setOldPosition(const Vector3r &x) 
		{ 
			std::cout << __func__ << ":PBR SetOldPosition: " << x[0] << "," << x[1] << "," << x[2] << "\n";
			m_rigidBody->setOldPosition(x);
			m_rigidBody->setPosition0(x);
			m_rigidBody->setLastPosition(x);
			
			//m_rigidBody->getGeometry().updateMeshTransformation(m_rigidBody->getPosition(), m_rigidBody->getRotationMatrix());
		}
		
		virtual void setRotation(const Quaternionr &q) 
		{ 
			m_rigidBody->setRotation(q); m_rigidBody->rotationUpdated(); 
			m_rigidBody->getGeometry().updateMeshTransformation(m_rigidBody->getPosition(), m_rigidBody->getRotationMatrix());
		}

		virtual void addForce(const Vector3r &f) 
		{ 
			const Real dt = SPH::TimeManager::getCurrent()->getTimeStepSize();
			m_rigidBody->getVelocity() += (1.0/ m_rigidBody->getMass()) * f * dt; 
		}

		virtual void addTorque(const Vector3r &t) 
		{ 
			const Real dt = SPH::TimeManager::getCurrent()->getTimeStepSize();
			m_rigidBody->getAngularVelocity() += m_rigidBody->getInertiaTensorInverseW() * t * dt; 
		}

		// transformation local to:
		// p_world = R R_MAT^T (R_initial p_local + x_initial - x_MAT) + x
		virtual Vector3r getWorldSpacePosition() const 
		{ 
			const Matrix3r R2 = (m_rigidBody->getRotation() * m_rigidBody->getRotationMAT().inverse()).matrix();
			const Vector3r x = R2 * m_rigidBody->getPositionInitial_MAT() + m_rigidBody->getPosition();
			return x; 
		}

		virtual Matrix3r getWorldSpaceRotation() const 
		{ 
			return (m_rigidBody->getRotation() * m_rigidBody->getRotationMAT().inverse() * m_rigidBody->getRotationInitial()).matrix();
		}

		virtual void updateMeshTransformation()
		{
			m_rigidBody->getGeometry().updateMeshTransformation(m_rigidBody->getPosition(), m_rigidBody->getRotationMatrix());
		}

		virtual const std::vector<Vector3r> &getVertices() const { return *m_rigidBody->getGeometry().getVertexData().getVertices(); };
		virtual const std::vector<Vector3r> &getVertexNormals() const { return m_rigidBody->getGeometry().getMesh().getVertexNormals(); };
		virtual const std::vector<unsigned int> &getFaces() const { return m_rigidBody->getGeometry().getMesh().getFaces(); };
	};
}

#endif 

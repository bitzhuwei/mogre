#include "stdafx.h"
#include "MogreNode.h"
#include "Marshalling.h"

using namespace Mogre;

CPP_DECLARE_STLVECTOR(Node::, NodeVec, Mogre::Node^, Ogre::Node*);
CPP_DECLARE_ITERATOR(Node::, ChildNodeIterator, Ogre::Node::NodeVecIterator, Mogre::Node::NodeVec, Mogre::Node^, Ogre::Node*, );

Node::~Node()
{
	this->!Node();
}

Node::!Node()
{
	OnDisposing(this, nullptr);

	if (IsDisposed)
		return;

	if (!_preventDelete)
	{
		void* userObj = _native->getUserPointer();
		if (userObj)
			VoidPtrToGCHandle(userObj).Free();
	}

	if (_createdByCLR && _native)
	{
		delete _native;
		_native = 0;
	}

	_isDisposed = true;

	OnDisposed(this, nullptr);
}

bool Node::InheritOrientation::get()
{
	return _native->getInheritOrientation();
}
void Node::InheritOrientation::set(bool inherit)
{
	_native->setInheritOrientation(inherit);
}

bool Node::InheritScale::get()
{
	return _native->getInheritScale();
}

void Node::InheritScale::set(bool inherit)
{
	_native->setInheritScale(inherit);
}

/*Mogre::Matrix3 Node::LocalAxes::get()
{
	return static_cast<const Ogre::Node*>(_native)->getLocalAxes();
}*/

String^ Node::Name::get()
{
	return TO_CLR_STRING(_native->getName());
}

void Node::Name::set(String^ value)
{
	DECLARE_NATIVE_STRING(o_value, value);

	_native->setName(o_value);
}

bool Node::IsStatic::get()
{
	return _native->isStatic();
}

void Node::IsStatic::set(bool value)
{
	_native->setStatic(value);
}

Mogre::Quaternion Node::Orientation::get()
{
	return ToQuaternion(_native->getOrientation());
}

void Node::Orientation::set(Mogre::Quaternion q)
{
	_native->setOrientation(FromQuaternion(q));
}

Mogre::Node^ Node::Parent::get()
{
	return Node::GetManaged(_native->getParent());
}

Ogre::uint16 Node::DepthLevel::get()
{
	return _native->getDepthLevel();
}

Mogre::Vector3 Node::Position::get()
{
	return ToVector3(_native->getPosition());
}

void Node::Position::set(Mogre::Vector3 pos)
{
	_native->setPosition(FromVector3(pos));
}

Mogre::Vector3 Node::Scale::get()
{
	return ToVector3(_native->getScale());
}

void Node::Scale::set(Mogre::Vector3 value)
{
	_native->setScale(FromVector3(value));
}

Mogre::Quaternion Node::DerivedOrientation::get()
{
	return ToQuaternion(_native->_getDerivedOrientationUpdated());
}

void Node::DerivedOrientation::set(Mogre::Quaternion value)
{
	_native->_setDerivedOrientation(FromQuaternion(value));
}

Mogre::Vector3 Node::DerivedPosition::get()
{
	return ToVector3(_native->_getDerivedPositionUpdated());
}

void Node::DerivedPosition::set(Mogre::Vector3 value)
{
	_native->_setDerivedPosition(FromVector3(value));
}

Ogre::IdType Node::Id::get()
{
	return _native->getId();
}

void Node::SetOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	_native->setOrientation(w, x, y, z);
}

void Node::ResetOrientation()
{
	_native->resetOrientation();
}

void Node::SetPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	static_cast<Ogre::Node*>(_native)->setPosition(x, y, z);
}

void Node::SetScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	_native->setScale(x, y, z);
}

void Node::SetScale(Mogre::Vector3 scale)
{
	_native->setScale(FromVector3(scale));
}

Mogre::Vector3 Node::GetScale()
{
	return ToVector3(_native->getScale());
}

void Node::ScaleVector(Mogre::Vector3 scale)
{
	_native->scale(FromVector3(scale));
}

void Node::ScaleXYZ(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	_native->scale(x, y, z);
}

void Node::Translate(Mogre::Vector3 d, Mogre::Node::TransformSpace relativeTo)
{
	_native->translate(FromVector3(d), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Translate(Mogre::Vector3 d)
{
	_native->translate(FromVector3(d));
}

void Node::Translate(Ogre::Real x, Ogre::Real y, Ogre::Real z, Mogre::Node::TransformSpace relativeTo)
{
	_native->translate(x, y, z, (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Translate(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	_native->translate(x, y, z);
}

void Node::Translate(Mogre::Matrix3 axes, Mogre::Vector3 move, Mogre::Node::TransformSpace relativeTo)
{
	pin_ptr<Ogre::Matrix3> p_axes = interior_ptr<Ogre::Matrix3>(&axes.m00);

	_native->translate(*p_axes, FromVector3(move), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Translate(Mogre::Matrix3 axes, Mogre::Vector3 move)
{
	pin_ptr<Ogre::Matrix3> p_axes = interior_ptr<Ogre::Matrix3>(&axes.m00);

	_native->translate(*p_axes, FromVector3(move));
}

void Node::Translate(Mogre::Matrix3 axes, Mogre::Real x, Mogre::Real y, Mogre::Real z, Mogre::Node::TransformSpace relativeTo)
{
	pin_ptr<Ogre::Matrix3> p_axes = interior_ptr<Ogre::Matrix3>(&axes.m00);

	_native->translate(*p_axes, x, y, z, (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Translate(Mogre::Matrix3 axes, Mogre::Real x, Mogre::Real y, Mogre::Real z)
{
	pin_ptr<Ogre::Matrix3> p_axes = interior_ptr<Ogre::Matrix3>(&axes.m00);

	_native->translate(*p_axes, x, y, z);
}

void Node::Roll(Mogre::Radian angle, Mogre::Node::TransformSpace relativeTo)
{
	_native->roll(Ogre::Radian(angle.ValueRadians), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Roll(Mogre::Radian angle)
{
	_native->roll(Ogre::Radian(angle.ValueRadians));
}

void Node::Pitch(Mogre::Radian angle, Mogre::Node::TransformSpace relativeTo)
{
	_native->pitch(Ogre::Radian(angle.ValueRadians), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Pitch(Mogre::Radian angle)
{
	_native->pitch(Ogre::Radian(angle.ValueRadians));
}

void Node::Yaw(Mogre::Radian angle, Mogre::Node::TransformSpace relativeTo)
{
	_native->yaw(Ogre::Radian(angle.ValueRadians), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Yaw(Mogre::Radian angle)
{
	_native->yaw(Ogre::Radian(angle.ValueRadians));
}

void Node::Rotate(Mogre::Vector3 axis, Mogre::Radian angle, Mogre::Node::TransformSpace relativeTo)
{
	_native->rotate(FromVector3(axis), Ogre::Radian(angle.ValueRadians), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Rotate(Mogre::Vector3 axis, Mogre::Radian angle)
{
	_native->rotate(FromVector3(axis), Ogre::Radian(angle.ValueRadians));
}

void Node::Rotate(Mogre::Quaternion q, Mogre::Node::TransformSpace relativeTo)
{
	_native->rotate(FromQuaternion(q), (Ogre::Node::TransformSpace)relativeTo);
}

void Node::Rotate(Mogre::Quaternion q)
{
	_native->rotate(FromQuaternion(q));
}

Mogre::Node^ Node::CreateChild(SceneMemoryMgrTypes sceneType, Mogre::Vector3 translate, Mogre::Quaternion rotate)
{
	auto native = _native->createChild((Ogre::SceneMemoryMgrTypes)sceneType, FromVector3(translate), FromQuaternion(rotate));
	return gcnew Mogre::Node(native);
}

Mogre::Node^ Node::CreateChild(SceneMemoryMgrTypes sceneType, Mogre::Vector3 translate)
{
	auto native = _native->createChild((Ogre::SceneMemoryMgrTypes)sceneType, FromVector3(translate));
	return gcnew Mogre::Node(native);
}

Mogre::Node^ Node::CreateChild(SceneMemoryMgrTypes sceneType)
{
	auto native = _native->createChild((Ogre::SceneMemoryMgrTypes)sceneType);
	return gcnew Mogre::Node(native);
}

Mogre::Node^ Node::CreateChild()
{
	auto native = _native->createChild();
	return gcnew Mogre::Node(native);
}

void Node::AddChild(Mogre::Node^ child)
{
	_native->addChild(child);
}

unsigned short Node::NumChildren()
{
	return _native->numChildren();
}

Mogre::Node^ Node::GetChild(unsigned short index)
{
	return static_cast<const Ogre::Node*>(_native)->getChild(index);
}

Mogre::Node::ChildNodeIterator^ Node::GetChildIterator()
{
	return _native->getChildIterator();
}

void Node::RemoveChild(Mogre::Node^ child)
{
	_native->removeChild(child);
}

void Node::RemoveAllChildren()
{
	_native->removeAllChildren();
}

Mogre::Quaternion Node::_getDerivedOrientation()
{
	return ToQuaternion(_native->_getDerivedOrientation());
}

Mogre::Quaternion Node::_getDerivedOrientationUpdated()
{
	return ToQuaternion(_native->_getDerivedOrientationUpdated());
}

Mogre::Vector3 Node::_getDerivedPosition()
{
	return ToVector3(_native->_getDerivedPosition());
}

Mogre::Vector3 Node::_getDerivedPositionUpdated()
{
	return ToVector3(_native->_getDerivedPositionUpdated());
}

Mogre::Vector3 Node::_getDerivedScale()
{
	return ToVector3(_native->_getDerivedScale());
}

Mogre::Vector3 Node::_getDerivedScaleUpdated()
{
	return ToVector3(_native->_getDerivedScaleUpdated());
}

Mogre::Matrix4 Node::_getFullTransform()
{
	return ToMatrix4(_native->_getFullTransform());
}

Mogre::Matrix4 Node::_getFullTransformUpdated()
{
	return ToMatrix4(_native->_getFullTransformUpdated());
}

Node^ Node::GetManaged(Ogre::Node* native)
{
	if (native == 0)
		return nullptr;

	void* userObj = native->getUserPointer();
	if (userObj)
		return static_cast<Node^>(VoidPtrToGCHandle(userObj).Target);

	throw gcnew InvalidOperationException("Unknown Node object!");
}

Ogre::Node* Node::UnmanagedPointer::get()
{
	return _native;
}

void Node::UnmanagedPointer::set(Ogre::Node* value)
{
	if (value == 0) {
		return;
	}

	_native = value;
	if (_native->getUserPointer() == 0)
	{
		void* handle = GCHandleToVoidPtr(GCHandle::Alloc(this, GCHandleType::Normal));
		_native->setUserPointer(handle);
	}
	else
	{
		VoidPtrToGCHandle(_native->getUserPointer()).Target = this;
	}
}
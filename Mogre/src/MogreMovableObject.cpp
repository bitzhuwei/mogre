#include "stdafx.h"
#include "MogreMovableObject.h"
#include "MogreNode.h"
#include "MogreSceneNode.h"
#include "MogreSceneManager.h"
#include "Marshalling.h"

using namespace Mogre;

class MovableObject_Listener_Proxy : public Ogre::MovableObject::Listener
{
public:
	friend ref class Mogre::MovableObject::Listener;

	gcroot<Mogre::MovableObject::Listener^> _managed;

	MovableObject_Listener_Proxy(Mogre::MovableObject::Listener^ managedObj) : Ogre::MovableObject::Listener()
		, _managed(managedObj)
	{
	}

	virtual void objectDestroyed(Ogre::MovableObject* param1) override;

	virtual void objectAttached(Ogre::MovableObject* param1) override;

	virtual void objectDetached(Ogre::MovableObject* param1) override;
};

void MovableObject_Listener_Proxy::objectDestroyed(Ogre::MovableObject* param1)
{
	_managed->ObjectDestroyed(param1);
}

void MovableObject_Listener_Proxy::objectAttached(Ogre::MovableObject* param1)
{
	_managed->ObjectAttached(param1);
}

void MovableObject_Listener_Proxy::objectDetached(Ogre::MovableObject* param1)
{
	_managed->ObjectDetached(param1);
}

Ogre::MovableObject::Listener* MovableObject::Listener::_IListener_GetNativePtr()
{
	return static_cast<Ogre::MovableObject::Listener*>(static_cast<MovableObject_Listener_Proxy*>(_native));
}

MovableObject::Listener::Listener()
{
	_createdByCLR = true;
	Type^ thisType = this->GetType();
	MovableObject_Listener_Proxy* proxy = new MovableObject_Listener_Proxy(this);
	_native = proxy;
}

MovableObject::Listener::~Listener()
{
	this->!Listener();
}

MovableObject::Listener::!Listener()
{
	OnDisposing(this, nullptr);

	if (IsDisposed)
		return;

	if (_createdByCLR && _native != 0)
	{
		delete _native; _native = 0;
	}

	OnDisposed(this, nullptr);
}

void MovableObject::Listener::ObjectDestroyed(Mogre::MovableObject^ param1)
{
	static_cast<MovableObject_Listener_Proxy*>(_native)->objectDestroyed(param1);
}

void MovableObject::Listener::ObjectAttached(Mogre::MovableObject^ param1)
{
	static_cast<MovableObject_Listener_Proxy*>(_native)->objectAttached(param1);
}

void MovableObject::Listener::ObjectDetached(Mogre::MovableObject^ param1)
{
	static_cast<MovableObject_Listener_Proxy*>(_native)->objectDetached(param1);
}

// ------------- MovableObject -------------

MovableObject::~MovableObject()
{
	this->!MovableObject();
}

MovableObject::!MovableObject()
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

Mogre::Aabb MovableObject::LocalAabb::get()
{
	return ToAabb(_native->getLocalAabb());
}

void MovableObject::LocalAabb::set(Mogre::Aabb value)
{
	_native->setLocalAabb(FromAabb(value));
}

Mogre::Aabb MovableObject::WorldAabb::get()
{
	return ToAabb(_native->getWorldAabb());
}

Mogre::Aabb MovableObject::WorldAabbUpdated::get()
{
	return ToAabb(_native->getWorldAabbUpdated());
}

Ogre::Real MovableObject::WorldRadius::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getWorldRadius();
}

Ogre::Real MovableObject::WorldRadiusUpdated::get()
{
	return static_cast<Ogre::MovableObject*>(_native)->getWorldRadiusUpdated();
}

bool MovableObject::CastShadows::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getCastShadows();
}

void MovableObject::CastShadows::set(bool enabled)
{
	static_cast<Ogre::MovableObject*>(_native)->setCastShadows(enabled);
}

Ogre::uint32 MovableObject::DefaultQueryFlags::get()
{
	return Ogre::MovableObject::getDefaultQueryFlags();
}

void MovableObject::DefaultQueryFlags::set(Ogre::uint32 flags)
{
	Ogre::MovableObject::setDefaultQueryFlags(flags);
}

Ogre::uint32 MovableObject::DefaultVisibilityFlags::get()
{
	return Ogre::MovableObject::getDefaultVisibilityFlags();
}

void MovableObject::DefaultVisibilityFlags::set(Ogre::uint32 flags)
{
	Ogre::MovableObject::setDefaultVisibilityFlags(flags);
}

bool MovableObject::IsAttached::get()
{
	return _native->isAttached();
}

bool MovableObject::IsStatic::get()
{
	return _native->isStatic();
}

void MovableObject::IsStatic::set(bool visible)
{
	_native->setStatic(visible);
}

String^ MovableObject::MovableType::get()
{
	return TO_CLR_STRING(static_cast<const Ogre::MovableObject*>(_native)->getMovableType());
}

Ogre::IdType MovableObject::Id::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getId();
}

String^ MovableObject::Name::get()
{
	return TO_CLR_STRING(static_cast<const Ogre::MovableObject*>(_native)->getName());
}

void MovableObject::Name::set(String^ name)
{
	DECLARE_NATIVE_STRING(o_name, name);

	static_cast<Ogre::MovableObject*>(_native)->setName(o_name);
}

Mogre::Node^ MovableObject::ParentNode::get()
{
	return Node::GetManaged(_native->getParentNode());
}

Mogre::SceneNode^ MovableObject::ParentSceneNode::get()
{
	return (Mogre::SceneNode^)SceneNode::GetManaged(_native->getParentSceneNode());
}

Ogre::uint32 MovableObject::QueryFlags::get()
{
	return _native->getQueryFlags();
}

void MovableObject::QueryFlags::set(Ogre::uint32 flags)
{
	_native->setQueryFlags(flags);
}

Ogre::Real MovableObject::RenderingDistance::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getRenderingDistance();
}

void MovableObject::RenderingDistance::set(Ogre::Real dist)
{
	static_cast<Ogre::MovableObject*>(_native)->setRenderingDistance(dist);
}

Ogre::Real MovableObject::RenderingMinPixelSize::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getRenderingMinPixelSize();
}

void MovableObject::RenderingMinPixelSize::set(Ogre::Real dist)
{
	static_cast<Ogre::MovableObject*>(_native)->setRenderingMinPixelSize(dist);
}

Ogre::uint8 MovableObject::RenderQueueGroup::get()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getRenderQueueGroup();
}

void MovableObject::RenderQueueGroup::set(Ogre::uint8 queueID)
{
	static_cast<Ogre::MovableObject*>(_native)->setRenderQueueGroup(queueID);
}

Ogre::uint32 MovableObject::VisibilityFlags::get()
{
	return _native->getVisibilityFlags();
}

void MovableObject::VisibilityFlags::set(Ogre::uint32 flags)
{
	_native->setVisibilityFlags(flags);
}

bool MovableObject::Visible::get()
{
	return _native->getVisible();
}

void MovableObject::Visible::set(bool visible)
{
	_native->setVisible(visible);
}

void MovableObject::AddQueryFlags(Ogre::uint32 flags)
{
	_native->addQueryFlags(flags);
}

void MovableObject::RemoveQueryFlags(unsigned long flags)
{
	_native->removeQueryFlags(flags);
}

void MovableObject::AddVisibilityFlags(Ogre::uint32 flags)
{
	_native->addVisibilityFlags(flags);
}

void MovableObject::RemoveVisibilityFlags(Ogre::uint32 flags)
{
	_native->removeVisibilityFlags(flags);
}

void MovableObject::SetListener(Mogre::MovableObject::IListener^ listener)
{
	_native->setListener(listener->_GetNativePtr());
}

/*Mogre::MovableObject::IListener^ MovableObject::GetListener()
{
	return static_cast<const Ogre::MovableObject*>(_native)->getListener();
}*/

void MovableObject::DetachFromParent()
{
	_native->detachFromParent();
}

bool MovableObject::IsVisible()
{
	return _native->isVisible();
}

Mogre::SceneManager^ MovableObject::GetCreator()
{
	return Mogre::SceneManager::GetManaged(_native->_getManager());
}

MovableObject^ MovableObject::GetManaged(Ogre::MovableObject* native)
{
	if (native == 0)
		return nullptr;

	void* userObj = native->getUserPointer();
	if (userObj)
	{
		return static_cast<MovableObject^>(VoidPtrToGCHandle(userObj).Target);
	}

	throw gcnew InvalidOperationException("Unknown MovableObject object!");
}

Ogre::MovableObject* MovableObject::UnmanagedPointer::get()
{
	return _native;
}

void MovableObject::UnmanagedPointer::set(Ogre::MovableObject* value)
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
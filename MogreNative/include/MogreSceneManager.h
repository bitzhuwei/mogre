#pragma once

#include "IDisposable.h"
#include "OgreSceneManager.h"
#include "MogreCommon.h"

namespace Mogre
{
	ref class RenderSystem;

	[Flags]
	public enum class SceneType
	{
		ST_GENERIC = Ogre::ST_GENERIC,
		ST_EXTERIOR_CLOSE = Ogre::ST_EXTERIOR_CLOSE,
		ST_EXTERIOR_FAR = Ogre::ST_EXTERIOR_FAR,
		ST_EXTERIOR_REAL_FAR = Ogre::ST_EXTERIOR_REAL_FAR,
		ST_INTERIOR = Ogre::ST_INTERIOR,
		Generic = Ogre::ST_GENERIC,
		ExteriorClose = Ogre::ST_EXTERIOR_CLOSE,
		ExteriorFar = Ogre::ST_EXTERIOR_FAR,
		ExteriorRealFar = Ogre::ST_EXTERIOR_REAL_FAR,
		Interior = Ogre::ST_INTERIOR
	};

	public ref class SceneManager : IDisposable
	{
	public:
		/// <summary>Raised before any disposing is performed.</summary>
		virtual event EventHandler^ OnDisposing;
		/// <summary>Raised once all disposing is performed.</summary>
		virtual event EventHandler^ OnDisposed;

	private:
		Ogre::SceneManager* _native;
		bool _createdByCLR;

	private:
		SceneManager(Ogre::SceneManager* obj) : _native(obj)
		{
		}

	public:
		~SceneManager();
	protected:
		!SceneManager();
	public:
		property bool IsDisposed
		{
			virtual bool get();
		}

	internal:
		property Ogre::SceneManager* UnmanagedPointer
		{
			Ogre::SceneManager* get();
		}
	};
}
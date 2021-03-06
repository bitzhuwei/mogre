#pragma once

#include "OgreRoot.h"
#include "OgreFrameStats.h"
#include "OgreTimer.h"
#include "MogreFrameListener.h"
#include "MogreSceneManager.h"
#include "MogreCommon.h"
#include "STLContainerWrappers.h"
#include "IteratorWrapper.h"
#include "Marshalling.h"

namespace Mogre
{
	ref class RenderSystem;
	ref class RenderSystemList;
	ref class Const_RenderSystemList;

	ref class RenderTarget;
	ref class RenderWindow;
	ref class CompositorManager2;

	public enum class InstancingThreadedCullingMethod
	{
		SingleThread = Ogre::INSTANCING_CULLING_SINGLETHREAD,
		Threaded = Ogre::INSTANCING_CULLING_THREADED
	};

	public ref class FrameStats
	{
	public protected:
		FrameStats()
		{
		}

		float _fps;
		float _avgFPS;
		float _bestTime;
		float _worstTime;
		float _lastTime;
		float _avgTime;
		size_t _triangleCount;
		size_t _batchCount;

		static operator FrameStats ^ (const Ogre::FrameStats& obj)
		{
			FrameStats^ clr = gcnew FrameStats;
			clr->_fps = obj.getFps();
			clr->_avgFPS = obj.getAvgFps();
			clr->_bestTime = obj.getBestTime();
			clr->_worstTime = obj.getWorstTime();
			clr->_lastTime = obj.getLastTime();
			clr->_avgTime = obj.getAvgTime();

			return clr;
		}

		static operator FrameStats ^ (const Ogre::FrameStats* pObj)
		{
			return *pObj;
		}

		//Public Declarations
	public:
		property float FPS
		{
			float get()
			{
				return _fps;
			}
		}

		property float AvgFPS
		{
			float get()
			{
				return _avgFPS;
			}
		}

		property float BestTime
		{
			float get()
			{
				return _bestTime;
			}
		}

		property float WorstTime
		{
			float get()
			{
				return _worstTime;
			}
		}

		property float LastTime
		{
			float get()
			{
				return _lastTime;
			}
		}

		property float AvgTime
		{
			float get()
			{
				return _avgTime;
			}
		}
	};

	public ref class Timer
	{
	internal:
		Ogre::Timer* _native;

		Timer(Ogre::Timer* obj) : _native(obj)
		{
		}

	public: 
		void Reset()
		{
			_native->reset();
		}

		property unsigned long Milliseconds
		{
			unsigned long get()
			{
				return _native->getMilliseconds();
			}
		}

		property unsigned long Microseconds
		{
			unsigned long get()
			{
				return _native->getMicroseconds();
			}
		}

		property unsigned long MillisecondsCPU
		{
			unsigned long get()
			{
				return _native->getMillisecondsCPU();
			}
		}

		property unsigned long MicrosecondsCPU
		{
			unsigned long get()
			{
				return _native->getMicrosecondsCPU();
			}
		}
	};

	public ref class Root : IMogreDisposable, public IFrameListener_Receiver
	{
		Mogre::CompositorManager2^ _compositorManager2;
		Mogre::RenderSystem^ _activeRenderSystem;
		Mogre::Timer^ _timer;

	public:
		/// <summary>Raised before any disposing is performed.</summary>
		virtual event EventHandler^ OnDisposing;
		/// <summary>Raised once all disposing is performed.</summary>
		virtual event EventHandler^ OnDisposed;

	private:
		//Event and Listener fields
		FrameListener_Director* _frameListener;
		Mogre::FrameListener::FrameStartedHandler^ _frameStarted;
		array<Delegate^>^ _frameStartedDelegates;
		Mogre::FrameListener::FrameRenderingQueuedHandler^ _frameRenderingQueued;
		array<Delegate^>^ _frameRenderingQueuedDelegates;
		Mogre::FrameListener::FrameEndedHandler^ _frameEnded;
		array<Delegate^>^ _frameEndedDelegates;

		static Root^ _singleton;
		Ogre::Root* _native;
		bool _createdByCLR;

	private:
		Root(Ogre::Root* obj) : _native(obj)
		{
		}

	public:
		/// <summary>Creates a new instance of the Root class.</summary>
		Root(String^ pluginFileName, String^ configFileName, String^ logFileName);

		/// <summary>Creates a new instance of the Root class.</summary>
		Root(String^ pluginFileName, String^ configFileName);

		/// <summary>Creates a new instance of the Root class.</summary>
		Root(String^ pluginFileName);

		/// <summary>Creates a new instance of the Root class.</summary>
		Root();

		static property Root^ Singleton
		{
			Root^ get()
			{
				if (_singleton == CLR_NULL)
				{
					Ogre::Root* ptr = Ogre::Root::getSingletonPtr();
					if (ptr) _singleton = gcnew Root(ptr);
				}
				return _singleton;
			}
		}

	public:
		~Root();
	protected:
		!Root();
	public:
		property bool IsDisposed
		{
			virtual bool get();
		}
		
		property Mogre::Timer^ Timer
		{
			Mogre::Timer^ get();
		}

		event Mogre::FrameListener::FrameStartedHandler^ FrameStarted
		{
			void add(Mogre::FrameListener::FrameStartedHandler^ hnd)
			{
				if (_frameStarted == CLR_NULL)
				{
					if (_frameListener == 0)
					{
						_frameListener = new FrameListener_Director(this);
						_native->addFrameListener(_frameListener);
					}
					_frameListener->doCallForFrameStarted = true;
				}
				_frameStarted += hnd;
				_frameStartedDelegates = _frameStarted->GetInvocationList();
			}
			void remove(Mogre::FrameListener::FrameStartedHandler^ hnd)
			{
				_frameStarted -= hnd;
				if (_frameStarted == CLR_NULL) _frameListener->doCallForFrameStarted = false;
				if (_frameStarted == CLR_NULL) _frameStartedDelegates = nullptr; else _frameStartedDelegates = _frameStarted->GetInvocationList();
			}
		private:
			bool raise(Mogre::FrameEvent evt)
			{
				if (_frameStarted)
				{
					bool mp_return;
					for (int i = 0; i < _frameStartedDelegates->Length; i++)
					{
						mp_return = static_cast<Mogre::FrameListener::FrameStartedHandler^>(_frameStartedDelegates[i])(evt);
						if (mp_return == false) break;
					}
					return mp_return;
				}
			}
		}

		event Mogre::FrameListener::FrameRenderingQueuedHandler^ FrameRenderingQueued
		{
			void add(Mogre::FrameListener::FrameRenderingQueuedHandler^ hnd)
			{
				if (_frameRenderingQueued == CLR_NULL)
				{
					if (_frameListener == 0)
					{
						_frameListener = new FrameListener_Director(this);
						_native->addFrameListener(_frameListener);
					}
					_frameListener->doCallForFrameRenderingQueued = true;
				}
				_frameRenderingQueued += hnd;
				_frameRenderingQueuedDelegates = _frameRenderingQueued->GetInvocationList();
			}
			void remove(Mogre::FrameListener::FrameRenderingQueuedHandler^ hnd)
			{
				_frameRenderingQueued -= hnd;
				if (_frameRenderingQueued == CLR_NULL) _frameListener->doCallForFrameRenderingQueued = false;
				if (_frameRenderingQueued == CLR_NULL) _frameRenderingQueuedDelegates = nullptr; else _frameRenderingQueuedDelegates = _frameRenderingQueued->GetInvocationList();
			}
		private:
			bool raise(Mogre::FrameEvent evt)
			{
				if (_frameRenderingQueued)
				{
					bool mp_return;
					for (int i = 0; i < _frameRenderingQueuedDelegates->Length; i++)
					{
						mp_return = static_cast<Mogre::FrameListener::FrameRenderingQueuedHandler^>(_frameRenderingQueuedDelegates[i])(evt);
						if (mp_return == false) break;
					}
					return mp_return;
				}
			}
		}

		event Mogre::FrameListener::FrameEndedHandler^ FrameEnded
		{
			void add(Mogre::FrameListener::FrameEndedHandler^ hnd)
			{
				if (_frameEnded == CLR_NULL)
				{
					if (_frameListener == 0)
					{
						_frameListener = new FrameListener_Director(this);
						_native->addFrameListener(_frameListener);
					}
					_frameListener->doCallForFrameEnded = true;
				}
				_frameEnded += hnd;
				_frameEndedDelegates = _frameEnded->GetInvocationList();
			}
			void remove(Mogre::FrameListener::FrameEndedHandler^ hnd)
			{
				_frameEnded -= hnd;
				if (_frameEnded == CLR_NULL) _frameListener->doCallForFrameEnded = false;
				if (_frameEnded == CLR_NULL) _frameEndedDelegates = nullptr; else _frameEndedDelegates = _frameEnded->GetInvocationList();
			}
		private:
			bool raise(Mogre::FrameEvent evt)
			{
				if (_frameEnded)
				{
					bool mp_return;
					for (int i = 0; i < _frameEndedDelegates->Length; i++)
					{
						mp_return = static_cast<Mogre::FrameListener::FrameEndedHandler^>(_frameEndedDelegates[i])(evt);
						if (mp_return == false) break;
					}
					return mp_return;
				}
			}
		}

		Mogre::FrameStats^ GetFrameStats();

		property Mogre::CompositorManager2^ CompositorManager2
		{
		public:
			Mogre::CompositorManager2^ get();
		}

		property Mogre::RenderWindow^ AutoCreatedWindow
		{
		public:
			Mogre::RenderWindow^ get();
		}

		property unsigned int DisplayMonitorCount
		{
		public:
			unsigned int get();
		}

		property Ogre::Real FrameSmoothingPeriod
		{
		public:
			Ogre::Real get();
		public:
			void set(Ogre::Real period);
		}

		property bool IsInitialised
		{
		public:
			bool get();
		}

		property bool BlendIndicesGpuRedundant
		{
		public:
			bool get();
		public:
			void set(bool value);
		}

		property bool BlendWeightsGpuRedundant
		{
		public:
			bool get();
		public:
			void set(bool value);
		}

		property Ogre::Real DefaultMinPixelSize
		{
		public:
			Ogre::Real get();
		public:
			void set(Ogre::Real value);
		}

		property Mogre::RenderSystem^ RenderSystem
		{
		public:
			Mogre::RenderSystem^ get();
		public:
			void set(Mogre::RenderSystem^ system);
		}

		void SaveConfig();
		bool RestoreConfig();
		bool ShowConfigDialog();

		Mogre::Const_RenderSystemList^ GetAvailableRenderers();
		Mogre::RenderSystem^ GetRenderSystemByName(String^ name);

		Mogre::RenderWindow^ Initialise(bool autoCreateWindow, String^ windowTitle);
		Mogre::RenderWindow^ Initialise(bool autoCreateWindow);

		Mogre::SceneManager^ CreateSceneManager(String^ typeName, size_t numWorkerThreads, InstancingThreadedCullingMethod threadedCullingMethod, String^ instanceName);
		Mogre::SceneManager^ CreateSceneManager(String^ typeName, size_t numWorkerThreads, InstancingThreadedCullingMethod threadedCullingMethod);

		Mogre::SceneManager^ CreateSceneManager(SceneType typeMask, size_t numWorkerThreads, InstancingThreadedCullingMethod threadedCullingMethod, String^ instanceName);
		Mogre::SceneManager^ CreateSceneManager(SceneType typeMask, size_t numWorkerThreads, InstancingThreadedCullingMethod threadedCullingMethod);

		void DestroySceneManager(Mogre::SceneManager^ sceneManager);
		Mogre::SceneManager^ GetSceneManager(String^ instanceName);
		Mogre::SceneManagerEnumerator::SceneManagerIterator^ GetSceneManagerIterator();

		String^ GetErrorDescription(long errorNumber);

		void QueueEndRendering();
		void StartRendering();
		bool RenderOneFrame();
		bool RenderOneFrame(float timeSinceLastFrame);
		void Shutdown();

		void AddResourceLocation(String^ name, String^ locType, String^ groupName, bool recursive);
		void AddResourceLocation(String^ name, String^ locType, String^ groupName);
		void AddResourceLocation(String^ name, String^ locType);

		void RemoveResourceLocation(String^ name, String^ groupName);
		void RemoveResourceLocation(String^ name);

		Mogre::RenderWindow^ CreateRenderWindow(String^ name, unsigned int width, unsigned int height, bool fullScreen, Mogre::Const_NameValuePairList^ miscParams);
		Mogre::RenderWindow^ CreateRenderWindow(String^ name, unsigned int width, unsigned int height, bool fullScreen);
		Mogre::RenderWindow^ CreateRenderWindow(String^ name, unsigned int width, unsigned int height);

		Mogre::RenderWindow^ CreateRenderWindow(String^ name, IntPtr handle, unsigned int width, unsigned int height, bool fullScreen);
		Mogre::RenderWindow^ CreateRenderWindow(String^ name, IntPtr handle, unsigned int width, unsigned int height);

		void DetachRenderTarget(Mogre::RenderTarget^ target);
		void DetachRenderTarget(String^ name);

		Mogre::RenderTarget^ GetRenderTarget(String^ name);

		void LoadPlugin(String^ pluginName);
		void UnloadPlugin(String^ pluginName);

		void _updateAllRenderTargets();

	protected public:
		virtual bool OnFrameStarted(Mogre::FrameEvent evt) = IFrameListener_Receiver::FrameStarted
		{
			return FrameStarted(evt);
		}

			virtual bool OnFrameRenderingQueued(Mogre::FrameEvent evt) = IFrameListener_Receiver::FrameRenderingQueued
		{
			return FrameRenderingQueued(evt);
		}

			virtual bool OnFrameEnded(Mogre::FrameEvent evt) = IFrameListener_Receiver::FrameEnded
		{
			return FrameEnded(evt);
		}
	};

	INC_DECLARE_STLVECTOR(RenderSystemList, Mogre::RenderSystem^, Ogre::RenderSystem*, public, private);
}
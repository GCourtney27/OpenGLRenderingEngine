#pragma once

#include "engine.h"

class Object;

class Scene
{
public:
	Scene(class Engine* engine) : m_engine(engine) {}
	virtual ~Scene() {}

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual const char* Name() = 0;
	virtual const char* Desc() = 0;

	void AddObject(Object *object) { m_objects.push_back(object); }

	template<typename T>
	T* CreateObject(const std::string& name = "")
	{
		T* object = new T(this, name);
		AddObject(object);
		return object;
	}

	template<typename T>
	T* GetObject(const std::string& name = "")
	{
		for (Object * object : m_objects)
		{
			if (object->name == name || name.empty())
			{
				T* result = dynamic_cast<T*>(object);
				if (result != nullptr)
				{
					return result;
				}
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetObjects()
	{
		std::vector<T*> objects;
		for (Object* object : m_objects)
		{
			if (dynamic_cast<T*>(object) != nullptr)
			{
				objects.push_back(dynamic_cast<T*>(object));
			}
		}
		return objects;
	}

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

public:
	class Engine* m_engine;

protected:
	std::vector<Object*> m_objects;
};
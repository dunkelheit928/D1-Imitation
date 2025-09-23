#pragma once

using Entity = uint32;
static const Entity INVALID_ENTITY = 0;

/*----------------------
	EntityManager
-----------------------*/
class EntityManager
{
public:
	Entity Create()
	{
		return m_id++;
	}

private:
	Entity m_id = 1;
};


/*----------------------
	ComponentArray
-----------------------*/
template<typename T>
class ComponentArray
{
public:
	void Add(Entity e, const T& comp)
	{
		if (m_indexOf.find(e) != m_indexOf.end())
		{
			m_dense[m_indexOf[e]] = comp;
			return;
		}

		m_indexOf[e] = (uint32)m_dense.size();
		m_dense.push_back(comp);
		m_entities.push_back(e);
	}

	void Remove(Entity e)
	{
		auto iter = m_indexOf.find(e);
		if (iter == m_indexOf.end())
		{
			return;
		}

		uint32 index = iter->second;
		uint32 last = m_dense.size() - 1;
		if (index != last)
		{
			m_dense[index] = m_dense[last];
			m_entities[index] = m_entities[last];
			m_indexOf[m_entities[index]] = index;
		}
		m_dense.pop_back();
		m_entities.pop_back();
		m_indexOf.erase(iter);
	}

	bool Has(Entity e) const
	{
		return m_indexOf.find(e) != m_indexOf.end();
	}

	T* Get(Entity e)
	{
		auto iter = m_indexOf.find(e);
		if (iter == m_indexOf.end())
		{
			return nullptr;
		}
		return &m_dense[iter->second];
	}

	const std::vector<T>& Data() const
	{
		return m_dense;
	}

	const std::vector<Entity>& Entities() const
	{
		return m_entities;
	}

	uint32 Size() const
	{
		return (uint32)m_dense.size();
	}

private:
	std::vector<T> m_dense;
	std::vector<Entity> m_entities;
	std::unordered_map<Entity, uint32> m_indexOf;
};


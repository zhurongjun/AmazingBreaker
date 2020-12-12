#pragma once
template<typename T>
class CSingleton
{
public:
	virtual~CSingleton() {
	}
	static T*GetInstance() {
		if (!m_pSingle)
			m_pSingle = new T;
		return m_pSingle;
	}
private:
	static T*m_pSingle;
};
template<typename T>
T*CSingleton<T>::m_pSingle = nullptr;
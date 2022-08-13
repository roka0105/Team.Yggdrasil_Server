#pragma once


namespace Yeram
{
	template<class C, typename T, typename get_func = const T& (C::*)(void), typename set_func = void(C::*)(const T&)>
	class property
	
	{
	public:
		property();

		property(const property<C, T, get_func, set_func>& _ref);
		
		property(C* _req);
		
		property(get_func _get, set_func _set);
		
		void __Initalize(C* _req, get_func _get, set_func _set);
		
		/*const operator T& (const T& _data) const
		{
			(requester->*set)(_data);
		}
		*/
		void operator=(const T& _data)const;
		void operator=(const property<Test, int,get_func,set_func>& _property);
		const T& GetData();
		void Set_requester(C* _req);
	private:
		C* requester;
		get_func get;
		set_func set;
	};
	template<class C, typename T, typename get_func, typename set_func>
	inline property<C, T, get_func, set_func>::property()
	{
	}
	template<class C, typename T, typename get_func, typename set_func>
	inline property<C, T, get_func, set_func>::property(const property<C, T, get_func, set_func>& _ref)
	{
		this->requester = _ref.requester;
		this->get = _ref.get;
		this->set = _ref.set;
	}
	template<class C, typename T, typename get_func, typename set_func>
	inline property<C, T, get_func, set_func>::property(C* _req)
	{
		requester = _req;
	}
	template<class C, typename T, typename get_func, typename set_func>
	inline property<C, T, get_func, set_func>::property(get_func _get, set_func _set)
	{
		get = _get;
		set = _set;
	}

	template<class C, typename T, typename get_func, typename set_func>
	void inline property<C, T, get_func, set_func>::__Initalize(C* _req, get_func _get, set_func _set)
	{
		requester = _req;
		get = _get;
		set = _set;
	}
	
	template<class C, typename T, typename get_func, typename set_func>
	inline void property<C, T, get_func, set_func>::operator=(const T& _data) const
	{
		(requester->*set)(_data);
	}
	/*template<class C, typename T, typename get_func, typename set_func>
	inline void property<C, T, get_func, set_func>::operator=(const property<Test, int, get_func, set_func>& _property)
	{
		(requester->*get)((_property.requester->*get)());
	}*/
	template<class C, typename T, typename get_func, typename set_func>
	inline const T& property<C, T, get_func, set_func>::GetData()
	{
		return (requester->*get)(); 
	}
	
	template<class C, typename T, typename get_func, typename set_func>
	inline void property<C, T, get_func, set_func>::Set_requester(C* _req)
	{
		requester = _req;
	}
}


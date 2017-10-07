
template <typename T>
class MySingleton{
public:
	static T* GetSingleton()
	{
		if (NULL == _Singleton) 
		{
			_Singleton = new T;
		}
		return (_Singleton);
	}
	static void Release()
	{
		delete _Singleton;
		_Singleton = NULL;
	}
	static T* _Singleton;
};


#pragma once



namespace Memory {

	template<class T, class... Args>
	T* construct(void* object, Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...))) {
		return ::new(object) T(std::forward<Args>(args)...);
	}


	template<class T>
	void destroy(T* object) noexcept(noexcept(std::declval<T>().~T())) {
		object->~T();
	}

}

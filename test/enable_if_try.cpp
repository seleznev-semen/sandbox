#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>

template<bool B, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T>
{
	using type = T;
};

template <bool B, typename T = void>
using enable_if_t = typename enable_if< B, T>::type;

template <typename T, typename = enable_if_t<std::is_class_v<	std::remove_reference_t< T > > > >
void g(const T&)
{
	std::cout << "is class" << std::endl;
}

template <typename T, typename = enable_if_t<!std::is_class_v<	std::remove_reference_t< T > > > >
void g(T&&)
{
	std::cout << "is not a class" << std::endl;
}


template<typename T>
struct has_val
{
private:
	template<typename U>
	static auto detect(const U&) -> decltype(std::declval<U>().val);

	static char detect(...);

public:
	static const bool value = std::is_same_v<decltype(detect(std::declval<T>())), int>;
};

struct S1
{
	int val;
};

struct S2
{
	bool val;
};

int main()
{
	std::cout << std::boolalpha << has_val<S2>::value << std::endl;
	return 0;
}

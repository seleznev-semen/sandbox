#include <iostream>
#include <type_traits>
#include <boost/variant.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
using nested1 = boost::variant<int, double>;
using nested2 = boost::variant<char, float>;
using nested3 = boost::variant<int, double, char, float, std::string>;
using var = boost::variant<nested1, nested2>;

template <typename... Ts>
constexpr bool is_conv_to_var = ( true && ... && std::is_constructible<Ts, var>::value );

template <typename... Ts>
using IsFitTs = typename std::enable_if< is_conv_to_var< Ts... >, bool >::type;

struct tripleHandler
{
	template <typename... Ts, IsFitTs< Ts... > = true>
	void operator()( Ts... var )
	{
		std::cout << "sucess!!!\n";	
	}

	template <typename... T>
	void operator()(T...)
	{
		std::cout << "fail\n" ;
	}

};

struct fn
{
	int operator()( int x )
	{
		return x;
	}
};




struct value_printer
{
    template< typename U > void operator()(U x)
    {
        std::cout << x << '\n';
    }
};

int main()
{
    boost::mpl::for_each< boost::mpl::range_c<int,0,10> >( value_printer() );
}

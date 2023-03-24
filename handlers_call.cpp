#include <iostream>
#include <functional>
#include <type_traits>
struct ChainExecutor
{
    template< typename F, typename ... Args >
    static void NoParamCall( F&& func, Args&&... args )
    {
        constexpr bool has_ret_val = !std::is_same_v< void, decltype( func() ) >;
        if constexpr( has_ret_val )
        {
            auto ret_val = func();
            if constexpr( sizeof...(args) )
                Dispatch( std::move( ret_val ), std::forward< Args >( args )... );
        }
        else
        {
            func();
            if constexpr( sizeof...(args) )
                Dispatch( std::forward< Args >( args )... );
        }
    }

    template< typename P, typename F, typename ... Args >
    static void ParamCall( P&& param, F&& func, Args&&... args )
    {
        constexpr bool has_ret_val = !std::is_same_v< void, decltype( func(param) ) >;
        if constexpr( has_ret_val )
        {
            auto ret_val = func(param);
            if constexpr( sizeof...(args) )
                Dispatch( std::move( ret_val ),  std::forward< Args >( args )... );
        }
        else
        {
            func(param);
            if constexpr( sizeof...(args) )
                Dispatch( std::forward< Args >( args )... );
        }
    }

    template< typename P, typename F, typename ... Args >
    static void Dispatch( P&& param, F&& func, Args&&... args )
    {
        if constexpr( std::is_invocable_v< P > )
            NoParamCall( std::forward< P >( param ), std::forward< F >( func ), std::forward< Args >( args )... );
        else
            ParamCall( std::forward< P >( param ), std::forward< F >( func ), std::forward< Args >( args )... );
    }
};


int main()
{
	ChainExecutor b;
    auto l1 = [](){ std::cout << "l1\n"; };
    auto l2 = [](){ std::cout << "l2\n"; };
    auto l3 = [](){ std::cout << "l3\n"; return 1; }; 
    auto l4 = []( int i ){  std::cout << "l4: " << i << std::endl; return std::string( "string from l4" ); }; 
    auto l5 = []( std::string i ){ std::cout << "l5: " << i << std::endl; };

    b.Dispatch( l1, l2, l3, l4, l5);    
    
   // std::cout << std::is_same_v< void, decltype( l3() ) > << std::endl;
   //std::cout << std::is_invocable_v< decltype( l4 ), int > << std::endl;
    
}

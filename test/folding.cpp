#include <iostream>
#include <memory>
#include <type_traits>
class INode {};
class Node : public INode {};

template< typename T >
using rem_cvr = std::remove_cv_t< std::remove_reference_t< T > >;

template< typename T >
constexpr bool is_inode_ptr = std::is_convertible< rem_cvr< T >, std::shared_ptr< INode > >::value;

template< typename... T >
using NodePtr = typename std::enable_if< ( true && ... && is_inode_ptr< T > ), bool >::type;

template< typename ... NodePtrT, NodePtr< NodePtrT... > = true >
void Foo( NodePtrT&&... val)
{
    std::cout << sizeof...(val) << std::endl;
}

template< typename ... NodePtrT, NodePtr< NodePtrT... > = true >
void Bar( int i, NodePtrT... val)
{
    std::cout << sizeof...(val) << std::endl;
}

template< typename T >
struct DEMO;

int main()
{
    using INodePtr = std::shared_ptr< INode >;
    using NodePtr = std::shared_ptr< Node >;
    
    INodePtr a, b, c;
    NodePtr e, f, g;
  	 Foo( a, b, c );
	 Foo(a,b);
	 Foo(b,c);
	 Foo(a,b,c,e,f,g);
	 Bar(1);
	 Foo( std::make_shared< Node >(), std::make_shared< Node >());
}

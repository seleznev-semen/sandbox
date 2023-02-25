#include<iostream>
#include <initializer_list>
#include <vector>

class Interface
{
public:
	virtual ~Interface() {}
};

template< typename T >
class INode : public Interface
{
public:
	T value;
	INode( T s )
	{
		value = s;
	}
	virtual ~INode(){}

	virtual void print() 
	{
		std::cout << value << std::endl;
	}
};


class StrNode : public INode< std::string >
{
public:
	StrNode( std::string i ) : INode< std::string >( i ) {}
	virtual void print() override
	{
		INode::print();
	}
};

class IntNode : public INode< int >
{
public:
	IntNode( int i ) : INode< int >( i ) {}
	virtual void print() override
	{
		INode::print();
	}

};


int main()
{
	StrNode s( std::string("asd") );
	IntNode i( 5 );

	std::vector<Interface*> v;
	v.push_back(&s);
	v.push_back(&i);

	for(auto j : v )
		static_cast< INode* >( j )->Print();
	return 0;
}

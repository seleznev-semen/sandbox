#include <iostream>

std::string parse( const std::string& input, int& i )
{
	auto is_digit = []( char c ){ return c >= '0' && c <= '9'; };
	std::string result;
	const char op_br = '[';
	const char cl_br = ']';
	
	char current;
	int count = 0;
	std::string seq;
	bool seq_start = false;
	
	// 3[a2[b]]
	for( ; i < input.size(); ++i )
	{	
		current = input[ i ];

		if( !seq_start && is_digit( current ) )
		{
			count = count * 10 + ( current - '0' );
			continue;
		}
		
		if( current == op_br )
		{
			seq_start = true;
			continue;
		}
		
		if( current == cl_br )
		{
			seq_start = false;
			for( int j = 0; j < count; ++j )
				result += seq;
			
			if( input[ i + 1 ] == cl_br )
			    return result;

			seq.clear();
			count = 0;
			continue;
		}
		
		if( seq_start )
		{
			if( is_digit(current) )
			{
				std::string nested = parse( input, i) ;
				seq += nested;
			}
			else
				seq += current;
			
			continue;
		}
	}
	
	return result;
}

int main()
{
	int i = 0;
	std::string input;
	
	while(true)
	{
		std::cin >> input;
		if( input == "q" )
			break;

		i = 0;
		std::cout << parse( input, i ) << std::endl;
	}
	return 0;
}


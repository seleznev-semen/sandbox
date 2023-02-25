#include <iostream>
#include <memory>
#include <cstring>
#include <type_traits>

namespace detail
{
template< typename T >
constexpr bool is_char_t = std::is_same_v< char, T > || std::is_same_v< wchar_t, T >;

template< typename T >
using CharT = typename std::enable_if_t< is_char_t< T >, bool >;

template< typename T >
T eof();

template<>
char eof< char >()
{
	return '\0';
}

template<>
wchar_t eof< wchar_t >()
{
	return L'\0';
}
}


template< typename T, size_t N, detail::CharT< T > = true >
class Buffer
{
	using byte = char;
	using sym_c = std::size_t;

public:
	static Buffer& Instance()
	{
		static Buffer instance;
		return instance;
	}

	~Buffer()
	{
		if( mIsHeapUsed )
			delete[] static_cast< T* >( mStorage );
	}

	Buffer( const Buffer& ) = delete;
	Buffer( Buffer&& ) = delete;
	Buffer& operator=( const Buffer& ) = delete;
	Buffer& operator=( Buffer&& ) = delete;
	
	void operator<<( const std::basic_string< T >& input )
	{	
        sym_c shift = input.size();
		if( !IsFitForStorage( shift ) )
	    	Instance().ExtendStorage();
			
		std::memcpy( Instance().End(), &input[ 0 ], shift * sizeof( T ) );
		Instance().ShiftCursor( shift );   
	}
	
	void operator<<( const T* input )
	{
	    sym_c shift = std::char_traits< T >::length( input );
	    if( !IsFitForStorage( shift ) )
	    	Instance().ExtendStorage();

		std::memcpy( Instance().End(), &input[ 0 ], shift * sizeof( T ) );
		Instance().ShiftCursor( shift );  
	}
	
	const T* operator*()
	{
	    *( Instance().End() ) = detail::eof< T >();
		return Instance().Begin();
	}

	std::basic_string_view< T > operator&()
	{
		return std::basic_string_view< T >( Instance().Begin(), mCursor );
	}

	sym_c Size()
	{
		return Instance().mCursor;
	}

    void Reset()
    {
        mCursor = 0;
        mStorage = mStackStorage;
        mCapacity = N;
        mIsHeapUsed = false;
    }

private:
	Buffer()
		: mStackStorage()
        , mStorage( mStackStorage )
		, mCapacity( N )
		, mCursor( 0 )
        , mIsHeapUsed( false )
	{	
        std::cout << "constructed\n";
	}
	
	T* End()
	{
		return static_cast< T* >( mStorage ) + mCursor;
	}
	
	T* Begin()
	{
		return static_cast< T* >( mStorage );
	}
	
	void ShiftCursor( sym_c shift )
	{
		mCursor += shift;
	}

	bool IsFitForStorage( sym_c income )
	{
		return mCursor + income < mCapacity;
	}
	
	void ExtendStorage()
	{ 
        std::wcout << "Realloc!\n";
        mIsHeapUsed = true;
        mCapacity *= 2;
        byte* tmp = static_cast< byte* >( mStorage );
        mStorage = new byte[ sizeof( T ) * mCapacity ];
        std::memcpy( mStorage, tmp, sizeof( T ) * mCursor );
        delete[] tmp;
	}

    byte mStackStorage[ sizeof( T ) * N ];
	void* mStorage;
    sym_c mCapacity;
	sym_c mCursor;
    bool mIsHeapUsed;
};

void Bufferize( const std::basic_string< wchar_t>& seq, size_t repeat_cnt, bool reset )
{
	Buffer< wchar_t, 64 >& buf = Buffer< wchar_t, 64 >::Instance();
	if( reset )
		buf.Reset();

	for( int i = 0; i < repeat_cnt; ++i )
		buf << seq;

	std::wcout << *buf << std::endl;
	std::wcout << buf.Size() << "\n-------------------" << std::endl;
}

int main()
{
	auto& buf = Buffer< wchar_t, 64 >::Instance();
	
	for( int i = 0; i < 3; ++i )
	    buf << L"asdefgh\n";

	std::wcout << &buf;

	std::cout << "\n-------------------" << std::endl;

	Bufferize( L"zxc", 3, false );
	Bufferize( L"123", 30, true );
	Bufferize( L"asd", 5, true );
	
	return 0;
}

#include <iostream>
#include <memory>
#include <cstring>
#include <type_traits>

constexpr std::size_t BUFFER_CAPACITY = 64;

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

template< typename T, detail::CharT< T > = true >
class Buffer
{
public:
	static Buffer& Instance()
	{
		if( mInstance == nullptr )
			mInstance.reset( new Buffer< T >( BUFFER_CAPACITY ) );
		
		return *mInstance;
	}

	static void DestroyInstance()
	{
		if( mInstance == nullptr )
			return;
		
		mInstance.reset( nullptr );
	}

	~Buffer()
	{
	}

	Buffer() = delete;
	Buffer( const Buffer& ) = delete;
	Buffer( Buffer&& ) = delete;
	Buffer& operator=( const Buffer& ) = delete;
	Buffer& operator=( Buffer&& ) = delete;
	
	void operator<<( std::basic_string< T >&& input )
	{	
        std::size_t shift = input.size();
		if( !IsFitForStorage( shift ) )
	    	mInstance->ExtendStorage();
			
		std::memcpy( mInstance->End(), &input[ 0 ], shift * sizeof( T ) );
		mInstance->ShiftCursor( shift );   
	}
	
	void operator<<( const T* input )
	{
	    std::size_t shift = std::char_traits< T >::length( input );
	    if( !mInstance->IsFitForStorage( shift ) )
	    	mInstance->ExtendStorage();

		std::memcpy( mInstance->End(), &input[ 0 ], shift * sizeof( T ) );
		mInstance->ShiftCursor( shift );  
	}
	

	const T* /*std::basic_string_view< T >*/ Flush()
	{
	    *( mInstance->End() ) = detail::eof< T >();
		Reset();
		return mInstance->Begin();
		//return std::basic_string_view< T >( mInstance->Begin(), mCursor );
	}

	std::size_t Size()
	{
		return mInstance->mCursor;
	}

protected:
	Buffer( std::size_t capacity )
		: mStorage( new T[ capacity ] )
		, mCapacity( capacity )
		, mCursor( 0 )
	{	
	}

private:
	T* End()
	{
		return static_cast< T* >( mInstance->mStorage ) + mInstance->mCursor;
	}
	
	T* Begin()
	{
		return static_cast< T* >( mInstance->mStorage );
	}
	
	void ShiftCursor( std::size_t shift )
	{
		mInstance->mCursor += shift;
	}

	bool IsFitForStorage( std::size_t income )
	{
		return ( mInstance->mCursor + income ) < mInstance->mCapacity;
	}
	
	void ExtendStorage()
	{ 
        std::wcout << "Realloc!\n";
        mInstance->mCapacity *= 2;
        void* tmp = mInstance->mStorage;
        mInstance->mStorage = new T[ mInstance->mCapacity ];
        std::memcpy( mInstance->mStorage, tmp, sizeof( T ) * mInstance->mCursor );
        delete[] static_cast< T* >( tmp );
	}

	void Reset()
    {
        mInstance->mCursor = 0;
    }

	static void Deleter( Buffer< T >* instance )
	{
		if( instance == nullptr )
			return;

		if( instance->mStorage != nullptr )
			delete[] static_cast< T* >( instance->mStorage );
		
		delete instance;
		instance = nullptr;
	}

	static std::unique_ptr< Buffer< T >, decltype( &Buffer< T >::Deleter ) > mInstance;
	void* mStorage;
    std::size_t mCapacity;
	std::size_t mCursor;
};

template<>
std::unique_ptr< Buffer< wchar_t >, decltype( &Buffer< wchar_t >::Deleter ) > Buffer< wchar_t >::mInstance = { nullptr, &Buffer< wchar_t >::Deleter };

void Bufferize( const std::basic_string< wchar_t>& seq, size_t repeat_cnt, bool reset )
{
	Buffer< wchar_t >& buf = Buffer< wchar_t >::Instance();

	for( int i = 0; i < repeat_cnt; ++i )
		buf << seq;

	std::wcout << buf.Flush() << std::endl;
	std::wcout << buf.Size() << "\n-------------------" << std::endl;
}

int main()
{
	using WBuffer = Buffer< wchar_t >;

	auto& buf = Buffer< wchar_t >::Instance();
	
	for( int i = 0; i < 3; ++i )
	    buf << L"asdefgh\n";

	std::wcout << buf.Flush();

	std::wcout << "\n-------------------" << std::endl;

	Bufferize( L"zxc", 3, false );
	Bufferize( L"123", 30, true );
	Bufferize( L"asd", 5, true );
	
	WBuffer::DestroyInstance();

	return 0;
}

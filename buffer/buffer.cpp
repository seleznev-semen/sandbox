#include <iostream>
#include <memory>
#include <cstring>

class Buffer
{
	using byte = char;

public:
	static Buffer& Instance()
	{
		static std::unique_ptr< Buffer > instance( new Buffer( 64 ) );
		return *instance;
	}

	~Buffer()
	{
		if( mStorage )
			delete[] mStorage;
	}

	Buffer() = delete;
	Buffer( const Buffer& ) = delete;
	Buffer( Buffer&& ) = delete;
	Buffer& operator=( const Buffer& ) = delete;
	Buffer& operator=( Buffer&& ) = delete;

	void operator<<( const std::string& input )
	{	
	    Instance().CheckRealloc( input.size() );
		std::memmove( Instance().EndPos(), &input[ 0 ], input.size() );
		Instance().ShiftCursor( input.size() );     // TODO ptr_diff end - pos
	}
	
	void operator<<( std::string&& input )
	{	
	    Instance().CheckRealloc( input.size() );
		std::memcpy( Instance().EndPos(), &input[ 0 ], input.size() );
		Instance().ShiftCursor( input.size() );     // TODO ptr_diff end - pos
	}
	
	void operator<<( const char* input )
	{
	    std::size_t length = strlen( input );
	    Instance().CheckRealloc( length );
	    std::memmove( Instance().EndPos(), input, length );
	    Instance().ShiftCursor( length );
	}
	
	const char* operator*()
	{
	    *( Instance().EndPos() ) = '\0';
		return Instance().StartPos();
	}

	std::size_t Size()
	{
		return Instance().mCursor;
	}

private:
	Buffer( std::size_t volume )
		: mStorage( new byte[ volume ] )
		, mVol( volume )
		, mCursor( 0 )
	{	
	}
	
	char* EndPos()
	{
		return mStorage + mCursor;
	}
	
	char* StartPos()
	{
		return mStorage;
	}
	
	void ShiftCursor( std::size_t n )
	{
		mCursor += n;
	}
	
	void CheckRealloc( std::size_t in_bytes )
	{
        if( mCursor + in_bytes <= mVol )
            return;
            
        std::cout << "Realloc!\n";
        mVol *= 2;
        char* tmp = mStorage;
        mStorage = new char[ mVol ];
        std::memmove( mStorage, tmp, mCursor );
        delete[] tmp;
	}

	char* mStorage;
    std::size_t mVol;
	std::size_t mCursor;
};

int main()
{
	auto& buf = Buffer::Instance();
	
	for( int i = 0; i < 10; ++i )
	{
	    buf << "asdefgh\n";
	    std::cout << buf.Size() << std::endl;
	}
	
	return 0;
}




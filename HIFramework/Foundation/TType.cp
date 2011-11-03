// =============================================================================
//	TType.cp
// =============================================================================
//

#include <AssertMacros.h>

#include "TType.h"

// -----------------------------------------------------------------------------
//	TType constructor
// -----------------------------------------------------------------------------
//
TType::TType()
	:	TBase()
{
	fRetainCount = 0;
}

// -----------------------------------------------------------------------------
//	TCache destructor
// -----------------------------------------------------------------------------
//
TType::~TType()
{
	check( fRetainCount < 2 );
}

// -----------------------------------------------------------------------------
//	CopyDescription
// -----------------------------------------------------------------------------
//
CFStringRef
TType::CopyDescription() const
{
	return ::CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR( "TType, retain count %lu" ), fRetainCount );
}

// -----------------------------------------------------------------------------
//	GetRetainCount
// -----------------------------------------------------------------------------
//
CFIndex
TType::GetRetainCount() const
{
	return fRetainCount;
}

// -----------------------------------------------------------------------------
//	Release
// -----------------------------------------------------------------------------
//
void
TType::Release()
{
	check( fRetainCount > 0 );
	if ( fRetainCount == 1 )
		delete this;
	else
		fRetainCount--;
}

// -----------------------------------------------------------------------------
//	Retain
// -----------------------------------------------------------------------------
//
void
TType::Retain()
{
	fRetainCount++;
}

//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: linux dependant ASM code for CPU capability detection
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//


// NOTE: This has to be the last file included! (turned off below, since this is included like a header)
#include "tier0/memdbgon.h"




// Turn off memdbg macros (turned on up top) since this is included like a header
#include "tier0/memdbgoff.h"

#ifndef __e2k__ // e2k CPU don't have CPUID
static void cpuid(uint32 function, uint32& out_eax, uint32& out_ebx, uint32& out_ecx, uint32& out_edx)
{
#if defined(PLATFORM_64BITS)
	asm("mov %%rbx, %%rsi\n\t"
		"cpuid\n\t"
		"xchg %%rsi, %%rbx"
		: "=a" (out_eax),
		  "=S" (out_ebx),
		  "=c" (out_ecx),
		  "=d" (out_edx)
		: "a" (function) 
	);
#else
	asm("mov %%ebx, %%esi\n\t"
		"cpuid\n\t"
		"xchg %%esi, %%ebx"
		: "=a" (out_eax),
		  "=S" (out_ebx),
		  "=c" (out_ecx),
		  "=d" (out_edx)
		: "a" (function) 
	);
#endif
}
#endif // ifndef __e2k__

bool CheckMMXTechnology(void)
{
#if defined(__e2k__)
	#if defined(__MMX__)
		return true;
	#else
		return false;
	#endif
#else
    uint32 eax,ebx,edx,unused;
    cpuid(1,eax,ebx,unused,edx);

    return edx & 0x800000;
#endif
}

bool CheckSSETechnology(void)
{
#if defined(__e2k__)
	#if defined(__SSE__)
		return true;
	#else
		return false;
	#endif
#else
    uint32 eax,ebx,edx,unused;
    cpuid(1,eax,ebx,unused,edx);

    return edx & 0x2000000L;
#endif
}

bool CheckSSE2Technology(void)
{
#if defined(__e2k__)
	#if defined(__SSE2__)
		return true;
	#else
		return false;
	#endif
#else
    uint32 eax,ebx,edx,unused;
    cpuid(1,eax,ebx,unused,edx);

    return edx & 0x04000000;
#endif
}

bool Check3DNowTechnology(void)
{
#if defined(__e2k__)
	#if defined(__3dNOW__)
		return true;
	#else
		return false;
	#endif
#else
    uint32 eax, unused;
    cpuid(0x80000000,eax,unused,unused,unused);

    if ( eax > 0x80000000L )
    {
     	cpuid(0x80000001,unused,unused,unused,eax);
		return ( eax & 1<<31 );
    }
    return false;
#endif
}

#pragma once

#define HR_CHECK(hr)																					\
	if(FAILED(hr))																						\
	{																									\
		char buf[512];																					\
        sprintf_s(buf, "HRESULT failed: 0x%08X (%s:%d)\n", (unsigned int)hr, __FILE__, __LINE__);  \
        OutputDebugStringA(buf);																		\
	}
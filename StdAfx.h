/********************************************************************************************************************

                                                       StdAfx.h

						                    Copyright 2004, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Chess/StdAfx.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

// Standard C++ library

#include <bitset>
#include <exception>
#include <limits>
#include <queue>
#include <sstream>
#include <vector>
#include <xutility>

// MFC Library

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//}}AFX_INSERT_LOCATION

// Win32

#include <mmsystem.h>

// My stuff

#include "Misc/AfxAssert.h"
#include "Misc/Etc.h"
#include "Misc/Random.h"

// Chess includes

#include "GameState/ChessTypes.h"
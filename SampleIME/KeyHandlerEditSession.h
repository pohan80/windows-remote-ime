// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "EditSession.h"
#include "Globals.h"
#include "windows.h"

class CKeyHandlerEditSession : public CEditSessionBase
{
public:
    CKeyHandlerEditSession(CSampleIME *pTextService, ITfContext *pContext, UINT uCode, WCHAR wch, _KEYSTROKE_STATE keyState) : CEditSessionBase(pTextService, pContext)
    {
        _uCode = uCode;
        _wch = wch;
        _KeyState = keyState;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
    UINT _uCode;    // virtual key code
    WCHAR _wch;      // character code
    _KEYSTROKE_STATE _KeyState;     // key function regarding virtual key
};

class CCharHandlerEditSession : public CEditSessionBase
{
public:
    CCharHandlerEditSession(_In_ CSampleIME *pTextService, ITfContext *pContext, WCHAR *wch) : CEditSessionBase(pTextService, pContext)
    {
        _wch = wch;
		_pTextService = pTextService;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
    WCHAR *_wch;      // character code
	CSampleIME *_pTextService;
};
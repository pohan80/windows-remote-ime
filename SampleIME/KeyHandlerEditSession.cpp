// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "KeyHandlerEditSession.h"
#include "EditSession.h"
#include "SampleIME.h"
#include "CompositionProcessorEngine.h"
#include "KeyStateCategory.h"

//////////////////////////////////////////////////////////////////////
//
//    ITfEditSession
//        CEditSessionBase
// CKeyHandlerEditSession class
//
//////////////////////////////////////////////////////////////////////

//+---------------------------------------------------------------------------
//
// CKeyHandlerEditSession::DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{
    HRESULT hResult = S_OK;

    CKeyStateCategoryFactory* pKeyStateCategoryFactory = CKeyStateCategoryFactory::Instance();
    CKeyStateCategory* pKeyStateCategory = pKeyStateCategoryFactory->MakeKeyStateCategory(_KeyState.Category, _pTextService);

    if (pKeyStateCategory)
    {
        KeyHandlerEditSessionDTO keyHandlerEditSessioDTO(ec, _pContext, _uCode,_wch, _KeyState.Function);
        hResult = pKeyStateCategory->KeyStateHandler(_KeyState.Function, keyHandlerEditSessioDTO);

        pKeyStateCategory->Release();
        pKeyStateCategoryFactory->Release();
    }

    return hResult;
}

STDAPI CCharHandlerEditSession::DoEditSession(TfEditCookie ec)
{
	CStringRange candidateString;
	candidateString.Set(_wch, 1);
	
	HRESULT setTexthr = E_FAIL;

    ULONG fetched = 0;
    TF_SELECTION tfSelection;

    if ((setTexthr = _pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched)) != S_OK || fetched != 1)
        return setTexthr;

    setTexthr = tfSelection.range->SetText(ec, 0, (&candidateString)->Get(), (LONG)(&candidateString)->GetLength());
    if (setTexthr == S_OK)
    {
        tfSelection.range->Collapse(ec, TF_ANCHOR_END);
        _pContext->SetSelection(ec, 1, &tfSelection);
    }

    tfSelection.range->Release();


    HRESULT hResult = S_OK;

    return hResult;
}

/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  A PUNCTUATION element
*
****************************************************************************/

#ifndef PUNCT_INCLUDED
#define PUNCT_INCLUDED

#include "text.hpp"

class Punctuation : public Text {
public:
    Punctuation( Document* d, Element* p, const std::wstring* f, unsigned int r, unsigned int c ) :
        Text( d, p, f, r, c ) { };
    Punctuation( Document* d, Element* p, const std::wstring* f, unsigned int r,
        unsigned int c, const std::wstring& txt, bool ts = true ) :
        Text( d, p, f, r, c, txt, ts ) { };
    ~Punctuation() { };
    Lexer::Token parse( Lexer* lexer );
private:
    Punctuation( const Punctuation& rhs );              //no copy
    Punctuation& operator=( const Punctuation& rhs );   //no assignment
};

#endif //PUNCT_INCLUDED
/****************************************************************************
*
*                            Open Watcom Project
*
*  Copyright (c) 2004-2009 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WGML implement multi letter function &'upper( )
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*  script string function &'upper(                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* &'upper(string<,n<,length>>):   The  Uppercase  function  returns  the  */
/*    uppercase equivalent of the 'string' operand.   The first character  */
/*    to be uppercased may be specified  by 'n' and the 'length' defaults  */
/*    to the end of the string.    The conversion to uppercase is defined  */
/*    by the Translate Uppercase (.TU) control word.                       */
/*      &'upper(abc) ==> ABC                                               */
/*      &'upper('Now is the time') ==> NOW IS THE TIME                     */
/*      abc&'upper(time flies)xyz ==> abcTIME FLIESxyz                     */
/*      &'upper(abc)...&'upper(xyz) ==> ABC...XYZ                          */
/*      &'upper(abcdefg,3) ==> abCDEFG                                     */
/*      &'upper(abcdefg,3,2) ==> abCDefg                                   */
/*      &'upper(one,two,three) ==> invalid operands                        */
/*                                                                         */
/***************************************************************************/

condcode    scr_upper( parm parms[ MAX_FUN_PARMS ], size_t parmcount, char * * result )
{
    char            *   pval;
    char            *   pend;
    condcode            cc;
    int                 k;
    int                 n;
    int                 len;
    getnum_block        gn;

    if( (parmcount < 1) || (parmcount > 3) ) {
        cc = neg;
        return( cc );
    }

    pval = parms[ 0 ].a;
    pend = parms[ 0 ].e;

    unquote_if_quoted( &pval, &pend );

    len = pend - pval + 1;              // default length

    if( len <= 0 ) {                    // null string nothing to do
        **result = '\0';
        return( pos );
    }

    n   = 0;                            // default start pos
    gn.ignore_blanks = false;

    if( parmcount > 1 ) {               // evalute start pos
        if( parms[ 1 ].e >= parms[ 1 ].a ) {// start pos specified
            gn.argstart = parms[ 1 ].a;
            gn.argstop  = parms[ 1 ].e;
            cc = getnum( &gn );
            if( (cc != pos) || (gn.result > len) ) {
                if( !ProcFlags.suppress_msg ) {
                    if( input_cbs->fmflags & II_macro ) {
                        out_msg( "ERR_FUNCTION parm 2 (startpos) invalid\n"
                                 "\t\t\tLine %d of macro '%s'\n",
                                 input_cbs->s.m->lineno,
                                 input_cbs->s.m->mac->name );
                    } else {
                        out_msg( "ERR_FUNCTION parm 2 (startpos) invalid\n"
                                 "\t\t\tLine %d of file '%s'\n",
                                 input_cbs->s.f->lineno,
                                 input_cbs->s.f->filename );
                    }
                    err_count++;
                    show_include_stack();
                }
                return( cc );
            }
            n = gn.result - 1;
        }
    }

    if( parmcount > 2 ) {               // evalute length for upper
        if( parms[ 2 ].e >= parms[ 2 ].a ) {// length specified
            gn.argstart = parms[ 2 ].a;
            gn.argstop  = parms[ 2 ].e;
            cc = getnum( &gn );
            if( (cc != pos) || (gn.result == 0) ) {
                if( !ProcFlags.suppress_msg ) {
                    if( input_cbs->fmflags & II_macro ) {
                        out_msg( "ERR_FUNCTION parm 3 (length) invalid\n"
                                 "\t\t\tLine %d of macro '%s'\n",
                                 input_cbs->s.m->lineno,
                                 input_cbs->s.m->mac->name );
                    } else {
                        out_msg( "ERR_FUNCTION parm 3 (length) invalid\n"
                                 "\t\t\tLine %d of file '%s'\n",
                                 input_cbs->s.f->lineno,
                                 input_cbs->s.f->filename );
                    }
                    err_count++;
                    show_include_stack();
                }
                return( cc );
            }
            len = gn.result;
        }
    }

    for( k = 0; k < n; k++ ) {          // copy unchanged before startpos
        if( pval > pend ) {
            break;
        }
        **result = *pval++;
        *result += 1;
    }

    for( k = 0; k < len; k++ ) {        // translate
        if( pval > pend ) {
            break;
        }
        **result = toupper( *pval++ );
        *result += 1;
    }

    for( ; pval <= pend; pval++ ) {     // copy unchanged
        **result = *pval;
        *result += 1;
    }

    **result = '\0';

    return( pos );
}

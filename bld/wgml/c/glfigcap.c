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
* Description: WGML implement :FIGCAP tag for LAYOUT processing
*
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__  1      /* use safer C library              */

#include <errno.h>
#include <stdarg.h>

#include "wgml.h"
#include "gvars.h"

/***************************************************************************/
/*   :FIGCAP attributes                                                    */
/***************************************************************************/
const   lay_att     figcap_att[6] =
    { e_pre_lines, e_font, e_figcap_string, e_string_font, e_delim,
      e_dummy_zero };


/*********************************************************************************/
/*Define the characteristics of the figure caption entity.                       */
/*                                                                               */
/*:FIGCAP                                                                        */
/*        pre_lines = 1                                                          */
/*        font = 0                                                               */
/*        figcap_string = "Figure "                                              */
/*        string_font = 0                                                        */
/*        delim = '.'                                                            */
/*                                                                               */
/*pre_lines This attribute accepts vertical space units. A zero value means that */
/*no lines are skipped. If the skip value is a line unit, it is multiplied       */
/*by the current line spacing (see "Vertical Space Unit" on page 77 for          */
/*more information). The resulting number of lines are skipped                   */
/*before the figure caption. If the document entity starts a new page,           */
/*the specified number of lines are still skipped. The pre-lines value           */
/*is not merged with the previous document entity's post-skip value.             */
/*                                                                               */
/*font This attribute accepts a non-negative integer number. If a font           */
/*number is used for which no font has been defined, WATCOM                      */
/*Script/GML will use font zero. The font numbers from zero to three             */
/*correspond directly to the highlighting levels specified by the                */
/*highlighting phrase GML tags. The font attribute defines the font of           */
/*the figure caption text. The font value is linked to the pre_lines             */
/*attribute (see "Font Linkage" on page 77).                                     */
/*                                                                               */
/*figcap_string This attribute accepts a character string. The specified string  */
/*is the first part of the figure caption generated by WATCOM Script/GML.        */
/*                                                                               */
/*string_font This attribute accepts a non-negative integer number. If a font    */
/*number is used for which no font has been defined, WATCOM                      */
/*Script/GML will use font zero. The font numbers from zero to three             */
/*correspond directly to the highlighting levels specified by the                */
/*highlighting phrase GML tags. The string_font attribute defines the            */
/*font of the the figure caption string defined from the text specified          */
/*by the figcap_string attribute to the figure caption delimiter                 */
/*inclusive.                                                                     */
/*                                                                               */
/*delim This attribute accepts a quoted character value. The delimiter value     */
/*specifies the character which is inserted after the number of the              */
/*figure. If a character other than a blank space is specified, that             */
/*character followed by a blank space will be inserted. If a blank               */
/*space is specified, only that blank space will be inserted.                    */
/*********************************************************************************/


/***************************************************************************/
/*  lay_figcap                                                             */
/***************************************************************************/

void    lay_figcap( const gmltag * entry )
{
    char        *   p;
    condcode        cc;
    int             k;
    lay_att         curr;
    att_args        l_args;
    bool            cvterr;

    p = scan_start;
    cvterr = false;

    if( !GlobalFlags.firstpass ) {
        scan_start = scan_stop + 1;
        eat_lay_sub_tag();
        return;                         // process during first pass only
    }
    if( ProcFlags.lay_xxx != el_figcap ) {
        ProcFlags.lay_xxx = el_figcap;
        out_msg( ":FIGCAP nearly dummy\n" );
    }
    cc = get_lay_sub_and_value( &l_args );  // get att with value
    while( cc == pos ) {
        cvterr = true;
        for( k = 0, curr = figcap_att[k]; curr > 0; k++, curr = figcap_att[k] ) {

            if( !strnicmp( att_names[curr], l_args.start[0], l_args.len[0] ) ) {
                p = l_args.start[1];

                switch( curr ) {
                case   e_pre_lines:
                    cvterr = i_space_unit( p, curr,
                                           &layout_work.figcap.pre_lines );
                    break;
                case   e_font:
                    cvterr = i_int8( p, curr, &layout_work.figcap.font );
                    break;
                case   e_figcap_string:
                    cvterr = i_xx_string( p, curr,
                                            &layout_work.figcap.string );
                    break;
                case   e_string_font:
                    cvterr = i_int8( p, curr, &layout_work.figcap.string_font );
                    break;
                case   e_delim:
                    cvterr = i_char( p, curr, &layout_work.figcap.delim );
                    break;
                default:
                    out_msg( "WGML logic error.\n");
                    cvterr = true;
                    break;
                }
                if( cvterr ) {          // there was an error
                    err_count++;
                    g_err( err_att_val_inv );
                    file_mac_info();
                }
                break;                  // break out of for loop
            }
        }
        cc = get_lay_sub_and_value( &l_args );  // get att with value
    }
    scan_start = scan_stop + 1;
    return;
}

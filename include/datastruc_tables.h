/*==============================================================================
 HEADER: datastruc_tables.h		[wlcov]
 Written by: Mario A. Rodriguez-Meza
 Starting date: april 2023
 Purpose: Definitions of global variables and parameters
 Language: C
 Use: '#include "datastruc_tables.h"
 Major revisions:
 ==============================================================================*/
//        1          2          3          4        ^ 5          6          7

//
// lines where there is a "//B socket:" string are places to include module files
//  that can be found in addons/addons_include folder
//

#ifndef _datastruc_tables_h
#define _datastruc_tables_h

//B Structure definitions for histograms and tables
//

//B socket:
#ifdef ADDONS
#include "datastruc_tables_include.h"
#endif
//E

//
//E Structure definitions for histograms

#endif // ! _datastruc_tables_h


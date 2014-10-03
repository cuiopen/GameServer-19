
#ifndef GRIDINDEX_H_
#define GRIDINDEX_H_

#if defined(USING_GRIDINDEX_CELL) && defined(USING_GRIDINDEX_NINE)
#error "USING_GRIDINDEX_CELL and USING_GRIDINDEX_NINE defined the same time"
#endif

#ifdef USING_GRIDINDEX_CELL
#include "GridIndex_Cell.h"
#endif
#ifdef USING_GRIDINDEX_NINE
#include "GridIndex_Nine.h"
#endif

#endif // GRIDINDEX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


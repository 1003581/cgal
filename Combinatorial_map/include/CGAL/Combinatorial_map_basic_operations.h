// Copyright (c) 2010-2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef CGAL_COMBINATORIAL_MAP_BASIC_OPERATIONS_H
#define CGAL_COMBINATORIAL_MAP_BASIC_OPERATIONS_H 1

namespace CGAL {

/** @file Combinatorial_map_basic_operations.h
 * Basic operations on a combinatorial map.
 */

/** Test if the two given darts belong to the same given orbit.
 * @param amap a combinatorial map.
 * @param adart1 the first dart.
 * @param adart2 the second dart.
 * @return true iff the two darts belong to the same orbit.
 */
template < class Map, class Iterator >
bool belong_to_same_orbit(const Map & amap, 
			  typename Map::Dart_const_handle adart1,
			  typename Map::Dart_const_handle adart2)
{
  CGAL_assertion( (!Iterator::is_basic_iterator()) );
  bool found = false;

  for (Iterator it(amap, adart1); !found && it.cont(); ++it)
  {
      if (it == adart2) found = true;
  }

  return found;
}

/** Test if all the darts of a given orbit are marked.
 * @param amap a combinatorial map.
 * @param adart a dart of the orbit.
 * @param amark the mark.
 * @return true iff all the darts are marked.
 */
template < class Map, class Iterator >
bool is_whole_orbit_marked(const Map & amap, 
			   typename Map::Dart_const_handle adart,
			   int amark)
{
  CGAL_assertion( !Iterator::is_basic_iterator() );
  bool res = true;

  for ( Iterator it(amap, adart); res && it.cont(); ++it )
    {
      if (!amap.is_marked(it, amark)) res = false;
    }

  return res;
}

/** Test if all the darts of a given orbit are unmarked.
 * @param amap a combinatorial map.
 * @param adart a dart of the orbit.
 * @param amark the mark.
 * @return true iff all the darts are unmarked.
 */
template < class Map, class Iterator >
bool is_whole_orbit_unmarked(const Map & amap, 
			     typename Map::Dart_const_handle adart,
			     int amark)
{
  amap.negate_mark(amark);
  bool res = is_whole_orbit_marked<Map,Iterator>(amap, adart, amark);
  amap.negate_mark(amark);
  return res;
}

/** Mark a given orbit with a given mark.
 * @param amap a combinatorial map.
 * @param adart a dart of the orbit.
 * @param amark the mark.
 * @return the number of darts of the marked orbit.
 * @pre The whole orbit must be unmarked.
 */
template < class Map, class Iterator >
typename Map::size_type mark_orbit(const Map & amap,
				   typename Map::Dart_const_handle adart,
				   unsigned int amark)
{
  CGAL_assertion( Iterator::is_basic_iterator() );
  CGAL_assertion( (is_whole_orbit_unmarked<Map,
		   CMap_non_basic_iterator<Map,Iterator> >
		   (amap, adart, amark)) );
  typename Map::size_type res = 0;

  for (Iterator it(amap, adart, amark); it.cont(); ++it)
    {
      amap.mark(it, amark);
      ++res;
    }

  return res;
}

/** Unmark a given orbit with a given mark.
 * @param amap a combinatorial map.
 * @param adart a dart of the orbit.
 * @param amark the mark.
 * @return the number of darts of the unmarked orbit.
 * @pre The whole orbit must be marked.
 */
template < class Map, class Iterator >
typename Map::size_type unmark_orbit(const Map & amap, 
				     typename Map::Dart_const_handle adart,
				     int amark)
{
  amap.negate_mark(amark);
  typename Map::size_type res = mark_orbit<Map, Iterator>(amap, adart, amark);
  amap.negate_mark(amark);
  return res;
}

/** Test if the two given darts belong to the same cell.
 * @param amap a combinatorial map.
 * @param adart1 the first dart.
 * @param adart2 the second dart.
 * @return true iff the two darts belong to the same cell.
 */
 template < class Map, unsigned int i, unsigned int d>
 bool belong_to_same_cell(const Map & amap, 
			  typename Map::Dart_const_handle adart1,
			  typename Map::Dart_const_handle adart2)
{
  return belong_to_same_orbit<Map,CMap_dart_const_iterator_of_cell<Map,i,d> >
    (amap, adart1, adart2);
}

 template < class Map, unsigned int i>
 bool belong_to_same_cell(const Map & amap, 
			  typename Map::Dart_const_handle adart1,
			  typename Map::Dart_const_handle adart2)
{
  return belong_to_same_cell<Map,i,Map::dimension>(amap,adart1,adart2);
}


/** Test if all the darts of a given cell are marked.
 * @param amap a combinatorial map.
 * @param adart a dart of the cell.
 * @param amark the mark.
 * @return true iff all the darts are marked.
 */
template < class Map, unsigned int i, unsigned int d>
bool is_whole_cell_marked(const Map & amap, 
			  typename Map::Dart_const_handle adart,
			  unsigned int amark)
{  
  return is_whole_orbit_marked<Map,CMap_dart_const_iterator_of_cell<Map,i,d> >
        (amap, adart, amark);
}

template < class Map, unsigned int i>
  bool is_whole_cell_marked(const Map & amap, 
			    typename Map::Dart_const_handle adart,
			    unsigned int amark)
{  
  return is_whole_cell_marked<Map,i,Map::dimension>(amap,adart,amark);
}

/** Test if all the darts of a given cell are unmarked.
 * @param amap a combinatorial map.
 * @param adart a dart of the cell.
 * @param amark the mark.
 * @return true iff all the darts are marked.
 */
template < class Map, unsigned int i, unsigned int d >
bool is_whole_cell_unmarked(const Map & amap, 
			    typename Map::Dart_const_handle adart,
			    unsigned int amark)
{  
  return is_whole_orbit_unmarked<Map,CMap_dart_const_iterator_of_cell<Map,i,d> >
        (amap, adart, amark);
}

template < class Map, unsigned int i>
bool is_whole_cell_unmarked(const Map & amap, 
			    typename Map::Dart_const_handle adart,
			    unsigned int amark)
{  
  return is_whole_cell_unmarked<Map,i,Map::dimension>(amap,adart,amark);
}

/** Mark a given cell with a given mark.
 * @param amap a combinatorial map.
 * @param adart a dart of the cell.
 * @param amark the mark.
 * @return the number of darts of the marked cell.
 * @pre The whole cell must be unmarked.
 */
template < class Map, unsigned int i, unsigned int d >
typename Map::size_type mark_cell(const Map & amap, 
				  typename Map::Dart_const_handle adart,
				  int amark)
{ return mark_orbit<Map,CMap_dart_const_iterator_basic_of_cell<Map,i,d> >
    (amap, adart, amark); }
     
template < class Map, unsigned int i>
typename Map::size_type mark_cell(const Map & amap, 
				  typename Map::Dart_const_handle adart,
				  int amark)
{ return mark_cell<Map,i,Map::dimension>(amap, adart, amark);}

/** Unmark a given orbit with a given mark.
 * @param amap a combinatorial map.
 * @param adart a dart of the cell.
 * @param amark the mark.
 * @return the number of darts of the unmarked cell.
 * @pre The whole cell must be marked.
 */
template < class Map, unsigned int i, unsigned int d >
typename Map::size_type unmark_cell(const Map & amap, 
				    typename Map::Dart_handle adart,
				    int amark)
{ return unmark_orbit<Map,CMap_dart_const_iterator_basic_of_cell<Map,i,d> >
     (amap, adart, amark);}

template < class Map, unsigned int i >
typename Map::size_type unmark_cell(const Map & amap, 
				    typename Map::Dart_handle adart,
				    int amark)
{ return unmark_cell<Map,i,Map::dimension>(amap, adart, amark);}
     
/** Compute the degree of a given i-cell c.
 * The degree is the number of distinct i+1 cells incident to c.
 * @param amap a combinatorial map. 
 * @param adart a dart of the cell.
 * @return the degree of the cell.
 */
template < class Map, unsigned int i >
typename Map::size_type degree( const Map & amap, 
				typename Map::Dart_handle adart )
{
  CGAL_assertion(adart != NULL);
  
  typename Map::size_type nbIncident = 0;
  int mark = amap.get_new_mark();
  int treated = amap.get_new_mark();

  CMap_dart_const_iterator_basic_of_cell<Map,i> it(amap, adart, mark);
  for ( ;it.cont(); ++it )
    {
      if (!amap.is_marked(*it, treated))
	{
	  ++nbIncident;
	  mark_cell<Map,i+1>(amap, *it, treated);
	}
      amap.mark(*it,mark);
    }
  
  amap.negate_mark(mark);
  for (it.rewind(); it.cont(); ++it)
    {
      if (amap.is_marked(*it, treated))
	unmark_cell<Map,i+1>(amap, *it, treated);
      amap.mark(*it,mark);
    }
  
  amap.negate_mark(mark);
  CGAL_assertion( amap.is_whole_map_unmarked(mark) );
  CGAL_assertion( amap.is_whole_map_unmarked(treated) );

  amap.free_mark(mark);
  amap.free_mark(treated);
  
  CGAL_assertion(nbIncident != 0);
  return nbIncident;
}

/** Compute the co-degree of a given i-cell c.
 * The co-degree is the number of distinct i-1 cells incident to c.
 * @param amap a combinatorial map. 
 * @param adart a dart of the cell.
 * @return the co-degree of the cell.
 */
template < class Map, unsigned int i >
typename Map::size_type codegree(const Map & amap, 
				 typename Map::Dart_handle adart)
{
  CGAL_assertion(adart != NULL);
  
  typename Map::size_type nbIncident = 0;
  int mark = amap.get_new_mark();
  int treated = amap.get_new_mark();

  CMap_dart_const_iterator_basic_of_cell<Map,i> it(amap, adart, mark);
  for ( ; it.cont(); ++it)
    {
      if (!amap.is_marked(*it, treated))
	{
	  ++nbIncident;
	  mark_cell<Map,i-1>(amap, *it, treated);
	}
      amap.mark(*it,mark);
    }
  
  amap.negate_mark(mark);
  for (it.rewind(); it.cont(); ++it)
    {
      if (amap.is_marked(*it, treated))      
	unmark_cell<Map,i-1>(amap, *it, treated);
      amap.mark(*it,mark);
    }

  amap.negate_mark(mark);
  CGAL_assertion( amap.is_whole_map_unmarked(mark) );
  CGAL_assertion( amap.is_whole_map_unmarked(treated) );

  amap.free_mark(mark);
  amap.free_mark(treated);
  
  CGAL_assertion(nbIncident != 0);
  return nbIncident;
}

} // namespace CGAL

#endif // CGAL_COMBINATORIAL_MAP_BASIC_OPERATIONS_H //
// EOF //
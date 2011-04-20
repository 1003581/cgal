// Copyright (c) 2010 CNRS, LIRIS, http://liris.cnrs.fr/, All rights reserved.
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
#ifndef CGAL_CELL_ITERATORS_H
#define CGAL_CELL_ITERATORS_H 1

#include <CGAL/Dart_iterators.h>
#include <CGAL/Combinatorial_map_basic_operations.h>

// TODO do all the orbit iterator of any orbit ?

namespace CGAL {

  /** @file Cell_iterators.h
   * All the cell iterators. Not commented in doxygen to avoid too much
   * classes in the documentation.
   * There are 3 classes:
   *  - CMap_cell_iterator<Map,Ite,i,dim>
   *  - CMap_one_dart_per_incident_cell_iterator<Map,Ite,i,dim>
   *  - CMap_one_dart_per_cell_iterator<Map,Ite,i,dim>
   */

  //****************************************************************************
  template <typename Map_,typename Ite, unsigned int i,
	    unsigned int dim=Map_::dimension,bool Const=false,
	    typename Use_mark=typename Ite::Use_mark>
  class CMap_cell_iterator;
  //****************************************************************************
  /* Class CMap_cell_iterator<Map,Ite,i,dim,Tag_true>: to iterate onto the
   * cells incident to the given iterator which uses mark.
   */
  template <typename Map_,typename Ite,unsigned int i, 
	    unsigned int dim,bool Const>
  class CMap_cell_iterator<Map_,Ite,i,dim,Const,Tag_true>: public Ite
  {
  public:
    typedef CMap_cell_iterator<Map_,Ite,i,dim,Const,Tag_true> Self;
    typedef Ite Base;

    typedef typename Base::Dart_handle Dart_handle;
    typedef typename Base::Map Map;

  protected:
    /// Unmark all the marked darts during the iterator.
    void unmark_treated_darts()
    {
      if (this->mmap->is_whole_map_unmarked(mcell_mark_number)) return;
	    
      this->mmap->negate_mark(this->mmark_number);
      this->mmap->negate_mark(mcell_mark_number);

      Ite::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), 
			   mcell_mark_number);

      while (this->mmap->number_of_unmarked_darts(mcell_mark_number) > 0 ||
	     this->mmap->number_of_unmarked_darts(this->mmark_number) > 0)
	{
	  this->operator++();
	}
      
      this->mmap->negate_mark(mcell_mark_number);
      this->mmap->negate_mark(this->mmark_number);

      CGAL_assertion(this->mmap->is_whole_map_unmarked(this->mmark_number));
      CGAL_assertion(this->mmap->is_whole_map_unmarked(mcell_mark_number));
    }
      
  public:
    /// Main constructor.
    CMap_cell_iterator(Map& amap, Dart_handle adart):
      Ite(amap, adart, amap.get_new_mark()),
      mcell_mark_number(amap.get_new_mark())
    {
      CGAL_assertion( Ite::is_basic_iterator() );

      assert(amap.is_whole_map_unmarked(mcell_mark_number));
      mark_cell<Map,i,dim>(amap, adart, mcell_mark_number);
    }
  
    /// Destructor.
    ~CMap_cell_iterator()
    {
      if ( mcell_mark_number!=-1 )
	{
	  unmark_treated_darts();
	  this->mmap->free_mark(this->mmark_number);
	  this->mmap->free_mark(mcell_mark_number);
	}
    }

    /// Copy constructor.
    CMap_cell_iterator(const Self& aiterator):
      Ite(aiterator),
      mcell_mark_number(-1)
    { this->mmark_number = -1; }

    /// Assignment operator.
    Self& operator=(const Self& aiterator)
    {
      if (this != &aiterator)
	{
	  Ite::operator=(aiterator);
	  mcell_mark_number = -1;
	  this->mmark_number = -1;
	}
      return *this;
    }

    /// Rewind of the iterator to its beginning.
    void rewind()
    {
      unmark_treated_darts();
      Ite::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), mcell_mark_number);
    }

    /// Prefix ++ operator.
    Self& operator++()
    {
      CGAL_assertion(this->cont());
      while (this->cont() &&
             this->mmap->is_marked((*this), mcell_mark_number))
	{
	  Ite::operator++();
	}
      if (this->cont())
	{
	  mark_cell<Map,i,dim>(*this->mmap, (*this), 
			       mcell_mark_number);
	}
      return *this;
    }

    /// Postfix ++ operator.
    void operator++(int)
    { operator++(); }

  private:
    /// A mark used to mark treated cells.
    int mcell_mark_number;
  };
  //****************************************************************************
  /* Class CMap_cell_iterator<Map,Ite,i,dim,Tag_false>: to iterate onto the
   * cells incident to the given iterator which does not use mark.
   */
  template <typename Map_,typename Ite, unsigned int i, 
	    unsigned int dim,bool Const>
  class CMap_cell_iterator<Map_,Ite,i,dim,Const,Tag_false>: public Ite
  {
  public:
    typedef CMap_cell_iterator<Map_,Ite,i,dim,Const,Tag_false> Self;
    typedef Ite Base;

    typedef typename Base::Dart_handle Dart_handle;
    typedef typename Base::Map Map;

  protected:
    /// Unmark all the marked darts during the iterator.
    void unmark_treated_darts()
    {
      if (this->mmap->is_whole_map_unmarked(mmark_number)) return;

      this->mmap->negate_mark(mmark_number);

      if (this->mmap->is_whole_map_unmarked(mmark_number)) return;

      Ite::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), 
			   mmark_number);
      while (this->mmap->number_of_unmarked_darts(mmark_number) > 0)
	this->operator++();
      this->mmap->negate_mark(mmark_number);
      CGAL_assertion(this->mmap->is_whole_map_unmarked(mmark_number));
    }

  public:
    /// Main constructor.
    CMap_cell_iterator(Map& amap, Dart_handle adart):
      Ite(amap, adart),
      mmark_number(amap.get_new_mark())
    {      
      CGAL_assertion( Ite::is_basic_iterator() );
      assert(amap.is_whole_map_unmarked(mmark_number));
      mark_cell<Map,i,dim>(amap, adart, mmark_number);
    }

    /// Destructor.
    ~CMap_cell_iterator()
    {
      if (mmark_number!=-1)
	{
	  unmark_treated_darts();
	  this->mmap->free_mark(mmark_number);
	}
    }

    /// Copy constructor.
    CMap_cell_iterator(const Self& aiterator):
      Ite(aiterator),
      mmark_number(-1)
    {}

    /// Assignment operator.
    Self& operator=(const Self & aiterator)
    {
      if (this != &aiterator)
	{
	  Ite::operator=(aiterator);
	  mmark_number = -1;
	}
      return *this;
    }

    /// Rewind of the iterator to its beginning.
    void rewind()
    {
      unmark_treated_darts();
      Ite::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), mmark_number);
    }

    /// Postfix ++ operator.
    void operator++(int)
    { operator++(); }

    /// Prefix ++ operator.
    Self& operator++()
    {
      CGAL_assertion(this->cont());
      while (this->cont() &&
             this->mmap->is_marked((*this), mmark_number))
	Ite::operator++();
      if (this->cont())
	mark_cell<Map,i,dim>(*this->mmap, (*this), mmark_number);
      return *this;
    }

  private:
    /// A mark used to mark treated cells.
    int mmark_number;
  };
  //****************************************************************************
  /* Class CMap_cell_iterator<Map,CMap_dart_iterator_basic_of_all<Map>,
     i,dim,Tag_false>: specialization to iterate onto 
     * all the cells of the map.
     */
  template <typename Map_,unsigned int i,unsigned int dim,bool Const>
  class CMap_cell_iterator<Map_,CMap_dart_iterator_basic_of_all<Map_,Const>,
			   i,dim,Const,Tag_false>: 
    public CMap_dart_iterator_basic_of_all<Map_,Const>
  {
  public:
    typedef CMap_dart_iterator_basic_of_all<Map_,Const> Base;
    typedef CMap_cell_iterator<Map_,Base,i,dim,Const,Tag_false> Self;

    typedef typename Base::Dart_handle Dart_handle;
    typedef typename Base::Map Map;
  
  protected:
    /// Unmark all the marked darts during the iterator.
    void unmark_treated_darts()
    {
      if (this->mmap->is_whole_map_unmarked(mmark_number)) return;

      this->mmap->negate_mark(mmark_number);

      if (this->mmap->is_whole_map_unmarked(mmark_number)) return;

      Base::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), 
			   mmark_number);
      while (this->mmap->number_of_unmarked_darts(mmark_number) > 0)
	this->operator++();
      this->mmap->negate_mark(mmark_number);
      CGAL_assertion(this->mmap->is_whole_map_unmarked(mmark_number));
    }

  public:
    /// Main constructor.
    CMap_cell_iterator(Map& amap):
      Base(amap),
      mmark_number(amap.get_new_mark())
    {
      CGAL_assertion( Base::is_basic_iterator() );
      assert(amap.is_whole_map_unmarked(mmark_number));
      mark_cell<Map,i,dim>(amap, (*this), mmark_number);
    }

    /// Destructor.
    ~CMap_cell_iterator()
    {
      if (mmark_number!=-1)
	{
	  unmark_treated_darts();
	  this->mmap->free_mark(mmark_number);
	}
    }

    /// Copy constructor.
    CMap_cell_iterator(const Self& aiterator):
      Base(aiterator),
      mmark_number(-1)
    {}

    /// Assignment operator.
    Self& operator=(Self& aiterator)
    {
      if (this != &aiterator)
	{
	  Base::operator=(aiterator);
	  mmark_number = -1;
	}
      return *this;
    }

    /// Rewind of the iterator to its beginning.
    void rewind()
    {
      unmark_treated_darts();
      Base::rewind();
      mark_cell<Map,i,dim>(*this->mmap, (*this), mmark_number);
    }

    /// Postfix ++ operator.
    void operator++(int)
    { operator++(); }

    /// Prefix ++ operator.
    Self& operator++()
    {
      CGAL_assertion(this->cont());
      while (this->cont() &&
             this->mmap->is_marked((*this), mmark_number))
	Base::operator++();
      if (this->cont())
	mark_cell<Map,i,dim>(*this->mmap, (*this), mmark_number);
      return *this;
    }

  private:
    /// A mark used to mark treated cells.
    int mmark_number;
  };
  //****************************************************************************
  /* Class CMap_one_dart_per_incident_cell_iterator<Map,i,j,dim>: to iterate onto the i-cells
   * incident to the given j-cell (one dart by each i-cell of the j-cell).
   */
  template <typename Map_,unsigned int i,unsigned int j, 
	    unsigned int dim=Map_::dimension,bool Const=false>
  class CMap_one_dart_per_incident_cell_iterator:
    public CMap_cell_iterator<Map_, 
			      CMap_dart_iterator_basic_of_cell<Map_,j,dim,Const>,
			      i,dim,Const>
  {
  public:
    typedef CMap_one_dart_per_incident_cell_iterator<Map_,i,j,dim,Const> Self;
    typedef CMap_cell_iterator<Map_,
			       CMap_dart_iterator_basic_of_cell<Map_,j,
								dim,Const>,
			       i,dim,Const> Base;

    typedef typename Base::Dart_handle Dart_handle;
    typedef typename Base::Map Map;

    /// Main constructor.
    CMap_one_dart_per_incident_cell_iterator(Map& amap, Dart_handle adart): 
      Base(amap, adart)
    {}
};
  //****************************************************************************
  /* Class CMap_one_dart_per_cell_iterator<Map,i,dim>: to iterate onto the
   * i-cells incident of the map (one dart by each i-cell).
   */
  template <typename Map_,unsigned int i,unsigned int dim=Map_::dimension,
	    bool Const=false>
  class CMap_one_dart_per_cell_iterator:
    public CMap_cell_iterator<Map_,CMap_dart_iterator_basic_of_all<Map_,Const>,
			      i,dim,Const>
  {
  public:
    typedef CMap_one_dart_per_cell_iterator<Map_,i,dim,Const> Self;
    typedef CMap_cell_iterator<Map_,
			       CMap_dart_iterator_basic_of_all<Map_,Const>, 
			       i,dim,Const> Base;

    typedef typename Base::Dart_handle Dart_handle;
    typedef typename Base::Map Map;
    
    /// Main constructor.
    CMap_one_dart_per_cell_iterator(Map& amap): Base(amap)
    {}
    /// Constructor with a dart in parameter (for end iterator).
    CMap_one_dart_per_cell_iterator(Map& amap, Dart_handle adart): Base(amap)
    { this->set_current_dart(adart); }
  };
  //****************************************************************************
  //****************************************************************************
} // namespace CGAL
//******************************************************************************
#endif // CGAL_CELL_ITERATORS_H
//******************************************************************************

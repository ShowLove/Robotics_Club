/****************************************************************************
 *
 *****************************************************************************/


#ifndef FW_LIST_H
#define FW_LIST_H


/*!
  \file fwList.h
  \brief Definition of the list managment class
*/

#include <stdio.h>

#include <fw/fwConfig.h>
#include <fw/fwDebug.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS

/*!
  \class fwListElement
  \brief Each element of a list
*/
template <class type>
class fwListElement
{
 public:
  fwListElement<type> *prev; //<! pointer to the previous element in the list
  fwListElement<type> *next; //<! pointer to the next element in the list
  type val;             //<! value of the element
} ;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


/*!
  \class fwList

  \brief Provide simple list managment

  \author Eric Marchand   (Eric.Marchand@irisa.fr) Irisa / Inria Rennes

  \verbatim
  Data structure:
    each object fwListElement contains its own value and
    two pointers to the next element of the list and to the previous one

    A list is mainly a pointer to three of these elements
      - one (cur) points the current element
      - one (first) points to a virtual element located at the
        beginning of the list
      - one (last) points  to a virtual element located at the
        end of the list

      first, last and cur are used internally and are not to be considered
      by "basic" user.

  Notation:
    In this documentation we note such a list as
          [*, a, b, c, *]
           f     ^     l
    where
     - the * are the two virtual elements (first (f) and last (l))
     - the ^ denotes the position of the current element

  \endverbatim
*/



template <class type>
class fwList
{
 private:
  void init() ;
 public:

  int nb;                    //<! number of items in the List
  /*!
    \brief the first virtual item in the list
    \verbatim
          [*, a, b, c, *]
           f
    \endverbatim
  */
  fwListElement<type> *first;
  /*!
    \brief the last virtualitem in the list
    \verbatim
          [*, a, b, c, *]
                       l
    \endverbatim
  */
  fwListElement<type> *last;
  /*!
    \brief the current item in the list
    \verbatim
          [*, a, b, c, *]
                 ^
    \endverbatim
  */
  fwListElement<type> *cur;       // the current element
 public:
  fwList() ;                  // constr.
  fwList(fwList& l);       // cloning
  virtual ~fwList();                  // destr.

  inline void next(void) ;           // current element's successor ( cur = cur->next )
  inline void previous(void) ;       // current element's predecessor ( cur = cur->pred )
  inline void front(void) ;          // go to the front of the List (cur = first)
  inline void end(void) ;            // go back to the end of the List ( cur = last )
  inline bool outside(void) const;         // test whether we are outside the List

  bool empty(void) const;       // tests whether the List is empty

  inline type& value(void);         // returns the current element value
  inline const type& value(void) const;         // returns the current element value

  void suppress(void);       // deletes the current item
  void kill();              // empties the List

  void display() ;          // displays the content of the list
  void print() {display() ;}           // displays the content of the list


  inline void addRight(const type& el);   // inserts an element on the right
  inline void addLeft(const type& el);    // inserts an element on the left
  inline void modify(const type& el);     // modifies thevalue field of the curr. el.
  inline void addRight(type& el);   // inserts an element on the right
  inline void addLeft(type& el);    // inserts an element on the left
  inline int nbElement(void);       // returns the number of items currently in the list
  inline int nbElements(void);       // returns the number of items currently in the list

  void operator=(fwList<type>& l);
  inline void operator+=(fwList<type>& l);
  inline void operator+=(const type& l);

  // Other non fundamental member (may be somehow usefull)
  bool nextOutside(void) const;     // test whether we are outside the List
  bool previousOutside(void) const;// test whether we are outside the List


  type& previousValue(void); // returns the previous element value
  type& nextValue(void);     // returns the next element value
  type& firstValue(void) ;
  type& lastValue(void) ;


};


/*!
  \brief initialization, Create an empty list
  \verbatim
  init() --> [*, *]
  \endverbatim */
template<class type>
void fwList<type>::init()
{
  fwListElement<type> *x=new  fwListElement<type>;
  fwListElement<type> *y=new  fwListElement<type> ;

  first = x ;
  last = y ;

  x->prev = NULL ;
  x->next = y ;
  y->prev = x ;
  y->next =NULL ;

  cur = x ;
  nb = 0 ;
}

/*!
  \brief Basic constructor, initialization, Create an empty list
  \verbatim
  init() --> [*, *]
  \endverbatim
  \sa init()
 */
template<class type>
fwList<type>::fwList()
{
  init() ;
}



/*!
  \brief fwList destructor
  \sa kill()
 */
template<class type>
fwList<type>::~fwList()
{

  kill() ;

  if (first != NULL) delete first ;
  if (last != NULL) delete last ;


}

/*!
  \brief return the number of element in the list
 */
template<class type>
int fwList<type>::nbElement(void)
{
  return(nb) ;
}

/*!
  \brief return the number of element in the list
 */
template<class type>
int fwList<type>::nbElements(void)
{
  return(nb) ;
}


/*!
  \brief position the current element on the next one
  \verbatim
  [*, a, b, c, d, *] --> next() -->   [*, a, b, c, d, *]
         ^                                      ^
  \endverbatim
*/
template<class type>
void fwList<type>::next(void)
{
  cur = cur->next ;
}


/*!
  \brief position the current element on the previous one
  \verbatim
  [*, a, b, c, d, *] --> previous() -->   [*, a, b, c, d, *]
         ^                                ^
  \endverbatim
*/
template<class type>
void fwList<type>::previous(void)
{
  cur = cur->prev ;
}

/*!
  \brief return the value of the current element

  \verbatim
  [*, a, b, c, *]  --> value() return b
         ^
  \endverbatim
 */
template<class type>
type& fwList<type>::value(void)
{
  return(cur->val) ;
}

/*!
  \brief return the value of the current element using a const ref.

  \verbatim
  [*, a, b, c, *]  --> value() return b
         ^
  \endverbatim
 */
template<class type>
const type& fwList<type>::value(void) const
{
  return(cur->val) ;
}

/*!
  \brief return the value of the previous element

  \verbatim
  [*, a, b, c, *]  --> previousValue() return a
         ^
  \endverbatim
*/
template<class type>
type& fwList<type>::previousValue(void)
{
  return(cur->prev->val) ;
}

/*!
  \brief return the value of the next element
  \verbatim
  [*, a, b, c, d, *]  --> nextValue() return c
         ^
  \endverbatim
*/
template<class type>
type& fwList<type>::nextValue(void)
{
  return(cur->next->val) ;
}



/*!
  \brief return the first element of the list
   \verbatim
   [*, a, b, c, d, *]  --> firstValue() return a
  \endverbatim
 */
template<class type>
type& fwList<type>::firstValue(void)
{
  return(first->next->val) ;
}



/*!\brief return the last element of the list
   \verbatim
   [*, a, b, c, d, *]  --> lastValue() return d
  \endverbatim
 */
template<class type>
type& fwList<type>::lastValue(void)
{
  return(last->prev->val) ;
}


/*!
  \brief Position the current element on the first element of the list

  \verbatim
  [*, a, b, c, d, *]  --> front() --> [*, a, b, c, d, *]
         ^                                ^
  \endverbatim
 */
template<class type>
void fwList<type>::front(void)
{
    cur = first->next ;
}

/*!
  \brief Position the current element on the last element of the list

  \verbatim
  [*, a, b, c, d, *]  --> end() --> [*, a, b, c, d, *]
         ^                                         ^
  \endverbatim
 */
template<class type>
void fwList<type>::end(void)
{
    cur = last->prev ;
}

/*!
  \brief Test if the list is empty

  \verbatim
  [*, a, b, c, d, *]  --> empty return false
  [*, *]              --> empty return true
  \endverbatim
 */
template<class type>
bool fwList<type>::empty(void) const
{
  return((first->next == last) &&( first == last->prev)) ;
}

/*!
  \brief Test if the current element is outside the list
  (on the virtual element)

  \verbatim
  [*, a, b, c, d, *]  --> outside return false
         ^
  [*, a, b, c, d, *]  --> outside return true
   ^      or      ^
  \endverbatim
 */
template<class type>
bool fwList<type>::outside(void) const
{

  return((cur==first)||(cur==last)) ;
}

/*!
  \brief Test if the next element is outside the list
  (ie if the current element is the last one)

  \verbatim
  [*, a, b, c, d, *]  --> nextOutside return true
               ^
  \endverbatim
 */
template<class type>
bool fwList<type>::nextOutside(void) const
{
  return((cur->next==first)||(cur->next==last)) ;
}


/*!
  \brief Test if the previous element is outside the list
  (ie if the current element is the firts one)

  \verbatim
  [*, a, b, c, d, *]  --> nextOutside return true
      ^
  \endverbatim
 */
template<class type>
bool fwList<type>::previousOutside(void) const
{
  return((cur->prev==first)||(cur->prev==last)) ;
}


/*!
  \brief add a new element in the list, at the right of the current one

  \warning the new element becomes the current one

  \verbatim
  [*, a, b, c, *]  --> addRight(i) -->   [*, a, b, i, c, *]
         ^                                         ^
  \endverbatim
 */
template<class type>
void fwList<type>::addRight(const type& v)
{
  fwListElement<type> *x=new  fwListElement<type>;

  x->val = v ;
  if (empty())
  {
    cur = first ;
  }
  else
  {
    if (outside()) std::cout << "fwList: outside with addRight " << std::endl ;
  }
  cur->next->prev = x ;
  x->next = cur->next ;
  x->prev = cur ;
  cur->next = x ;
  cur = x ;
  nb++ ;
}


/*!
  \brief add a new element in the list, at the left of the current one

  \warning the new element becomes the current one

  \verbatim
  [*, a, b, c, *]  --> addLeft(i) -->   [*, a, i, b, c, *]
         ^                                     ^
  \endverbatim
 */
template<class type>
void fwList<type>::addLeft(const type& v)
{
  fwListElement<type> *x=new  fwListElement<type>;

  x->val = v ;

  if (empty())
  {
    cur = last ;
  }
  else
  {
    if (outside()) std::cout << "fwList: outside with addLeft " << std::endl ;
  }
  x->next = cur ;
  x->prev = cur->prev ;
  cur->prev->next = x ;
  cur->prev = x ;
  cur = x ;
  nb++ ;

}

/*!
  \brief add a new element in the list, at the right of the current one

  \warning the new element becomes the current one

  \verbatim
  [*, a, b, c, *]  --> addRight(i) -->   [*, a, b, i, c, *]
         ^                                         ^
  \endverbatim
 */
template<class type>
void fwList<type>::addRight(type& v)
{
  fwListElement<type> *x=new  fwListElement<type>;

  x->val = v ;
  if (empty())
  {
    cur = first ;
  }
  else
  {
    if (outside()) std::cout << "fwList: outside with addRight " << std::endl ;
  }
  cur->next->prev = x ;
  x->next = cur->next ;
  x->prev = cur ;
  cur->next = x ;
  cur = x ;
  nb++ ;
}


/*!
  \brief add a new element in the list, at the left of the current one

  \warning the new element becomes the current one

  \verbatim
  [*, a, b, c, *]  --> addLeft(i) -->   [*, a, i, b, c, *]
         ^                                     ^
  \endverbatim
 */
template<class type>
void fwList<type>::addLeft(type& v)
{
  fwListElement<type> *x=new  fwListElement<type>;

  x->val = v ;

  if (empty())
  {
    cur = last ;
  }
  else
  {
    if (outside()) std::cout << "fwList: outside with addLeft " << std::endl ;
  }
  x->next = cur ;
  x->prev = cur->prev ;
  cur->prev->next = x ;
  cur->prev = x ;
  cur = x ;
  nb++ ;

}

/*!
  \brief Modify the value of the current element

  \verbatim
  [*, a, b, c, *]  --> modify(i) -->   [*, a, i, c, *]
         ^                                    ^
  \endverbatim
 */
template<class type>
void fwList<type>::modify(const type& v)
{
  cur->val = v ;
}

/*!
  \brief Destroy the list

  \verbatim
  [*, a, b, c, *]  --> kill -->   [*, *]
         ^                            ^
  \endverbatim
 */
template<class type>
void fwList<type>::kill()
{

  front() ;
  while (!empty())
  {
    suppress() ;
  }

}



/*!
  \brief suppress the current element

  \warning new current element is on the next one

  \verbatim
  [*, a, b, c, d, *] --> suppress -->  [*, a, c, d, *]
         ^                                    ^
  \endverbatim
 */
template<class type>
void fwList<type>::suppress(void)
{
  fwListElement<type> *x ;

  cur->prev->next = cur->next ;
  cur->next->prev = cur->prev ;
  x = cur ;
  cur = cur->next ;

  if (x!=NULL) delete x ;

  nb-- ;


}



/*!
  \brief Copy constructor

  \remarks Cannot define this function as usual, ie, :
   <tt>template<class type>
   fwList<type>::fwList(const fwList<type>& l)</tt>
  since the list is indeed modified (not the element but the position
  of the current element.
 */

template<class type>
void fwList<type>::operator=(fwList<type>& l)
{
  type x ;
  fwListElement<type> *e ;

  kill() ;
  e = l.cur ;
  l.front() ;
  front() ;
  while (!l.outside())
  {
    x = l.value() ;
    addRight(x) ;
    l.next() ;
  }

  nb = l.nb ;
  cur = first->next ;
  l.cur = e ;
}

/*!
  \brief Append two lists

  \verbatim
  [*, a, b, *] += [*, c, d, *] --> [*, a, b, c, d, *]
                                                ^
  \endverbatim
 */
template<class type>
void fwList<type>::operator+=(fwList<type>& l)
{
  type x ;

  l.front() ;
  end() ;
  while (!l.outside())
  {
    x = l.value() ;
    addRight(x) ;
    l.next() ;
  }
}

/*!
  \brief  Append an element to a list

  \verbatim
  [*, a, b, *] += c --> [*, a, b, c, *]
                                  ^
  \endverbatim
 */
template<class type>
void fwList<type>::operator += (const type& l)
{
  end() ;
  addRight(l) ;
}


/*!
  \brief Copy constructor

  \remarks Cannot define this function as usual, ie, :
   <tt>template<class type>
   fwList<type>::fwList(const fwList<type>& l)</tt>
  since the list is indeed modified (not the element but the position
  of the current element.

  \sa operator=(fwList<type>& l)
 */
template<class type>
fwList<type>::fwList(fwList<type>& l)
{
  init() ;
  *this = l;
}

/*!
  \brief Print (std::cout) all the element of the list
 */
template<class type>
void fwList<type>::display()
{
  int k = 1 ;
  front() ;
  while(!outside()) {
    std::cout<<k<<" ---> "<<value()<<std::endl ;
    next() ;
    k++ ;
  }
  std::cout<< std::endl << std::endl ;
}

// For template instantiation with Visual Studio
#if defined(FW_BUILD_SHARED_LIBS) && defined(FW_USE_MSVC)
#include <fw/fwPoint.h>
#include <fw/fwMeSite.h>
#include <fw/fwBasicFeature.h>
#include <fw/fwForwardProjection.h>

template class FW_EXPORT fwList<int>;
template class FW_EXPORT fwList<double>;
template class FW_EXPORT fwList<float>;
template class FW_EXPORT fwList<unsigned int>;
template class FW_EXPORT fwList<fwBasicFeature *>;
template class FW_EXPORT fwList<fwForwardProjection *>;
template class FW_EXPORT fwList<fwPoint>;
template class FW_EXPORT fwList<fwMeSite>;
template class FW_EXPORT fwList<fwColVector>;
#endif


#endif  /* #ifndef VP_LIST_H */



/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */

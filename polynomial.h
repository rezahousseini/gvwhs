/////////////////////////////////// polynomial.h /////////////////////
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>
#include <list>
#include <iosfwd>
#include <cstdlib>

#include "aTerm.h"

using std::list;

extern int jPairs;
template<class aterm>

class polynomial
{
  public:

  typedef aterm term;
  typedef typename term::monomial monomial;
  typedef typename term::coefficient coefficient;

  inline const coefficient& lc () const
  {
    return terms.front().c();
  }

  inline const monomial& lm() const
  {
    return terms.front().m();
  }

  inline const term& lt () const
  {
    return terms.front ();
  }

  inline const list<term>& getList () const
  {
    return terms;
  }

  inline void pop_front ()
  {
    terms.pop_front ();
  }

  inline void push_back (const aterm& t)
  {
    terms.push_back (t);
  }

  inline bool isZero ()
  {
    return terms.empty ();
  }

  inline void addCoeff2LC (const coefficient& c)
  {
    terms.front().addCoeff (c);
  }

  inline int size () const
  {
    return terms.size ();
  }

  inline bool operator< (const polynomial& p) const
  {
    return lm() < p.lm ();
  }

  inline bool operator> (const polynomial& p) const
  {
    return lm() > p.lm ();
  }

  inline bool operator<= (const polynomial& p) const
  {
    return lm() <= p.lm ();
  }

  inline bool operator>= (const polynomial& p) const
  {
    return lm() >= p.lm ();
  }

  inline polynomial makeMonic()
  {
    operator/= (lc ());
    return *this ;
  }

  polynomial& operator/= (const coefficient& c)
  {
    if (!c)
      {
        terms.clear ();
        return *this;
      }
    typename list<term>::iterator itr;
    coefficient d = coefficient (1)/c;
    for (itr = terms.begin (); itr != terms.end (); ++itr)
      *itr *= d;
    return *this;
  }

  inline polynomial operator/ (const coefficient& c)
  {
    polynomial answer = *this;
    answer /= c;
    return answer;
  }

  polynomial& operator+= (const term& t)
  {
    if (terms.empty ())
      {
        terms.push_front (t);
        return *this;
      }
    typename list<term>::iterator itr = terms.begin ();
    while (itr != terms.end () && *itr > t) ++itr;
    if (itr == terms.end () || *itr < t)
      {
        terms.insert (itr, t);
      }
    else
      {
        itr->addCoeff (t.c ());
        if (!static_cast<bool>(*itr))
          terms.erase (itr);
      }
    combineTerms ();
    return *this ;
  }

  polynomial& operator+= (const polynomial& p)
  {
    typename list <term>::iterator myItr = terms.begin ();
    typename list <term>::const_iterator pItr = p.terms.begin ();
    for (pItr = p.terms.begin (); pItr != p.terms.end (); ++pItr)
      {
        while (myItr != terms.end () && *myItr > *pItr) ++myItr;
        myItr = ++terms.insert (myItr, *pItr);
      }
    combineTerms ();
    return *this ;
  }

  inline polynomial operator+ (const polynomial& p) const
  {
    polynomial answer = *this;
    return answer += p;
  }

  polynomial& operator-= (const polynomial& p)
  {
    typename list <term>::iterator myItr = terms.begin ();
    typename list <term>::const_iterator pItr = p.terms.begin ();
    for (pItr = p.terms.begin (); pItr != p.terms.end (); ++pItr)
      {
        while (myItr != terms.end () && *myItr > *pItr) ++myItr ;
        myItr = ++terms.insert (myItr, -*pItr);
      }
    combineTerms ();
    return *this;
  }

  void assertDecreasing (const char* desc) const
  {
    if (terms.size () < 2) return;
    typename list <term>::const_iterator lead, follow, end = terms.end ();
    follow = lead = terms.begin ();
    for (++lead; lead != end ; ++lead, ++follow)
      if (*lead >= *follow)
        {
          std::cerr << "Term list non decreasing:     " << std::endl;
          std::cerr << " called inside :     " << desc << std::endl;
          exit (0);
        }
  }

  polynomial operator- () const
  {
    polynomial answer = *this;
    typename list <term>::iterator itr;
    for (itr = answer.terms.begin (); itr != answer.terms.end (); ++itr)
      itr->neg ();
    return answer ;
  }

  inline polynomial operator- (const polynomial&& p) const
  {
    polynomial answer = *this;
    return answer -= p;
  }

  polynomial& operator*= (const term& t)
  {
    typename list <term>::iterator itr;
    for (itr = terms.begin (); itr != terms.end (); ++itr)
      *itr *= t;
    return *this;
  }

  polynomial& operator*= (const coefficient& c)
  {
    typename list <term>::iterator itr;
    for (itr = terms.begin (); itr != terms.end (); ++itr)
      itr->operator*=(c);
    return *this;
  }

  inline polynomial operator* (const term& t ) const
  {
    polynomial answer = *this;
    return answer *= t;
  }

  void print (std::ostream& o) const
  {
    if (terms.empty ())
      {
        o << 0;
        return ;
      }
    typename list <term>::const_iterator itr;
    int i = 1, size = terms.size ();
    for (itr = terms.begin (); itr != terms.end (); ++itr)
      {
        itr->print (o);
        if (i++ != size) o << "+";
      }
  }

  inline bool isDivisibleBy (const polynomial& p) const
  {
    return lm ().isDivisibleBy (p.lm ());
  }

  int divideThrough (const polynomial& p)
  {
    typename std::list <term>::iterator t;
    int changed = 0;
    for (t = terms.begin (); t != terms.end () && t->m() >= p.lm (); ++t)
      {
        if (t->m().isDivisibleBy (p.lm()))
          {
            changed++;
            term factor = *t/p.lt ();
            --t;
            operator-= (p * factor);
          }
      }
    return changed ;
  }

  private:

  void combineTerms ()
  {
    if (terms.size () > 1)
      {
        typename list <term>::iterator lead, follow, end = terms.end ();
        lead = follow = terms.begin ();
        ++lead;
        while (lead != end && follow != end)
          {
            if (*lead == *follow)
              {
                follow->addCoeff (lead->c ());
                lead = terms.erase (lead);
                if (follow->isZero ()) terms.erase (follow);
              }
            follow = lead;
            ++lead;
          }
      }
  }

  list<term> terms;
};

template<class term> 
std::ostream& operator<< (std::ostream& o, const polynomial<term>& poly)
{
  poly.print (o);
  return o;
}
#endif

/////////////////////////////////// j_pair.h /////////////////////////
#ifndef JPAIR_H
#define JPAIR_H

#include <iosfwd>
#include <vector>

template<class mod _elt, class poly>

class j_pair
{

  public:

  typedef typename poly::monomial monomial;
  typedef typename poly::term term;
  typedef mod_elt mod_mon;

  j_pair (const monomial& theirScale, int index, const mod_elt& usig,
          const monomial& vpoly, int tc) : 
      u(usig* theirScale),
      v(vpoly* theirScale), 
      origUVIdx(index),
      scale(theirScale), 
      termCount(tc) { }

  j_pair (const mod_elt& u1, const poly& v1) : u(u1), v(v1.lm()),
                                               origUVIdx(-1), p(v1)
  {
    termCount = p.size ();
  }

  inline bool operator<= (const j_pair& r ) const
  {
    return u <= r.u;
  }

  inline bool operator< (const j_pair& r ) const
  {
    return u < r.u;
  }

  inline bool operator>= (const j_pair& r ) const
  {
    return u >= r.u;
  }

  inline bool operator> (const j_pair& r ) const
  {
    return u > r.u;
  }

  inline bool operator== (const j_pair& r ) const
  {
    return u == r.u;
  }

// inline int idx () const { return origUVIdx ; }
  inline const mod_elt& uPart () const
  {
    return u;
  }

  inline const monomial& vMonomial () const
  {
    return v;
  }

  inline const poly& vPolynomial (const std::vector<poly>& Vlist)
  {
    if (origUVIdx >= 0)
      {
        p = Vlist[origUVIdx] * term(scale);
        origUVIdx = -1;
      }
    return p;
  }

  inline bool isDivisibleBy (const mod_elt& mon) const
  {
    return u.isDivisibleBy (mon);
  }

  inline const monomial& theScale () const
  {
    return scale;
  }

  inline int tc () const
  {
    return termCount;
  }

  void print (std::ostream& o) const
  {
    o << " (" << u << " , " << v << " ) " << std::endl;
  }

  private:

  poly p;
  mod_elt u;
  monomial v;
  int origUVIdx, termCount;
  monomial scale;
};
#endif

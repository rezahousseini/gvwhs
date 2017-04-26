/////////////////////////////////// monomial.h ///////////////////////
#ifndef MONOMIALH
#define MONOMIALH

#include <vector>
#include <iosfwd>

#include "term-order.h"

template<int n, class ORDERINIG, class T = short>
class monomial
{
  public:

  typedef T expType;
  static const int numVars = n;

  monomial ()
  {
    for (int i = 0; i < n; i++) exponents[i] = 0;
    degree = 0;
  }

  monomial (const T* expons)
  {
    degree = 0;
    for (int i = 0; i < n; i++) degree += exponents[i] = expons[i];
  }

  monomial (const std::vector<int>& expons)
  {
    degree = 0;
    for (int i = 0; i < n; i++) degree += exponents[i] = expons[i];
  }

  monomial operator* (const monomial& x) const
  {
    monomial y = x;
    for (int i = 0; i < n; i++) y.exponents[i] += exponents[i];
    y.updateDegree ();
    return y;
  }

  monomial& operator*= (const monomial& x)
  {
    for (int i = 0; i < n; i++) exponents[i] += x.exponents[i];
    updateDegree ();
    return *this;
  }

  inline bool isDivisibleBy (const monomial& x) const
  {
    if (x.degree > degree) return false;
    for (int i = 0; i < n; i++)
      if (x.exponents[i] > exponents[i]) return false;
    return true;
  }

  monomial operator/ (const monomial& x) const
  {
    monomial y = *this;
    for (int i = 0; i < n; i++)
      y.exponents[i] -= x.exponents[i];
    y.degree -= x.degree;
    return y;
  }

  monomial& operator/= (const monomial& x)
  {
    for (int i = 0; i < n; i++)
      exponents[i] -= x.exponents[i];
    degree -= x.degree;
    return *this;
  }

  inline bool operator== (const monomial& x) const
  {
    for (int i = 0; i < n; i++)
      if (exponents[i] != x.exponents[i]) return false ;
    return true;
  }

  inline bool operator!= (const monomial& x) const
  {
    return !operator== (x);
  }

  inline const T* getExponents () const
  {
    return exponents;
  }

  std::vector<int> getExpVector () const
  {
    std::vector<int> answer;
    for ( int i = 0; i < n; i++)
      answer.push_back (exponents[i]);
    return answer;
  }

  inline int getDegree () const
  {
    return degree;
  }

  inline int updateDegree ()
  {
    degree = 0;
    for (int i = 0; i < n; i++) degree += exponents[i];
    return degree;
  }

  monomial lcm (const monomial& rhs)
  {
    monomial answer = *this;
    for (int i = 0; i < n; i++)
      if (rhs.exponents[i] > exponents[i])
        answer.exponents[i] = rhs.exponents[i];
    answer.updateDegree ();
    return answer;
  }

  inline bool isConstant () const
  {
    return degree == 0;
  }

  void print (std::ostream& o) const
  {
    const T* e = getExponents ();
    const int oneBased = 1;
    for (int i = 0; i < n; i++)
      {
        if (e[i] > 0)
          {
            if (i < 10) o << "x " << (i + oneBased);
            else o << "x {" << (i + oneBased) << "} ";
            if (e [i] > 1 && e[i] < 10) o << "^ " << e[i];
            else if (e[i] > 9) o << "^ {" << e [i] << "} ";
          }
      }
  }

  private:

  T exponents[n];
  int degree;
};

template<int n, class ORDERING, class T>
std::ostream& operator<< (std::ostream& o, const monomial<n, ORDERING, T>& x)
{
  x.print (o);
  return o ;
}
#endif

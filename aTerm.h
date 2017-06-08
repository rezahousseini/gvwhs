/////////////////////////////////// aTerm.h //////////////////////////
#ifndef ATERMH
#define ATERMH

#include "term_order.h"

#include <iosfwd>

template<class monom, class field>
class aTerm
{
  public:

  typedef field coefficient;
  typedef monom monomial;

  aTerm (monom mon, field coef = field (1)) : myMonomial (mon),
                                              myCoefficient (coef) { }

  aTerm (field coef = field (1)) : myCoefficient (coef) {}

  inline const field& c () const
  {
    return myCoefficient;
  }

  inline const monomial& m() const
  {
    return myMonomial;
  }

  inline bool isZero ()
  {
    return myCoefficient.isZero ();
  }

  inline void neg ()
  {
    myCoefficient = -myCoefficient;
  }

  inline bool operator< (const aTerm& right) const
  {
    return myMonomial < right.myMonomial;
  }

  inline bool operator<= (const aTerm& right) const
  {
    return myMonomial <= right.myMonomial;
  }

  inline bool operator> (const aTerm& right) const
  {
    return myMonomial > right.myMonomial;
  }

  inline bool operator>= (const aTerm& right) const
  {
    return myMonomial >= right.myMonomial;
  }

  inline aTerm operator* (const aTerm& term) const
  {
    return aTerm(myMonomial*term.myMonomial,
                 myCoefficient*term.myCoefficent);
  }

  inline aTerm operator* (const coefficient& c)
  {
    return aTerm (myMonomial, myCoefficient*c);
  }

  aTerm& operator*= (const aTerm& right)
  {
    myCoefficient *= right.myCoefficient;
    myMonomial *= right.myMonomial;
    return *this;
  }

  inline aTerm& operator*= (const coefficient& c)
  {
    myCoefficient *= c;
    return *this;
  }

  inline aTerm& operator/= (const coefficient& c)
  {
    myCoefficient /= c;
    return *this ;
  }

  aTerm operator/ (const aTerm & term) const
  {
    aTerm temp = *this;
    temp.myMonomial /= term.myMonomial;
    temp.myCoefficient /= term.myCoefficient;
    return temp;
  }

  aTerm& operator/= (const aTerm& term)
  {
    myMonomial /= term.myMonomial;
    myCoefficient /= term.myCoefficient;
    return *this;
  }

  inline aTerm operator- () const
  {
    aTerm answer = *this;
    answer.myCoefficient = -myCoefficient;
    return answer;
  }

  inline bool operator== (const aTerm& right) const
  {
    return myMonomial == right.myMonomial;
  }

  inline bool operator!= (const aTerm& right) const
  {
    return myMonomial != right.myMonomial;
  }

  inline operator bool () const
  {
    return static_cast<bool>(myCoefficient);
  }

  inline bool isDivisibleBy (const aTerm& x) const
  {
    return myMonomial.isDivisibleBy (x.myMonomial);
  }

  inline field addCoeff (const field& c)
  {
    return myCoefficient += c;
  }

  inline bool printsNeg () const
  {
    return myCoefficient.printsNeg ();
  }

  void print (std::ostream& o) const
  {
    if (myMonomial.isConstant () || !myCoefficient.isOne ())
      {
        o << myCoefficient;
      }
    if (!myMonomial.isConstant ()) o << myMonomial;
  }

  private:

  monom myMonomial;
  field myCoefficient;
};

template<class monom, class field >
std::ostream& operator<< (std::ostream& o, const aTerm<monom, field>& term)
{
  term.print (o);
  return o;
}
#endif

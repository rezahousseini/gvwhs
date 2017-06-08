/////////////////////////////////// modp.h ///////////////////////////
#ifndef MODPH
#define MODPH

#include <iostream>
#include <iosfwd>
#include <cstdlib>

const bool negs = false; /// print smallest number in magnitude

template<int p, class T = short>
class modp
{
  public :
  modp (T enn = 0) : n (enn % p)
  {
    if (n < 0) n = n + p;
  }

  inline modp operator+ (modp enn) const
  {
    return modp((enn.n + n) % p);
  }

  inline modp& operator+= (modp enn)
  {
    n += enn.n;
    n %= p;
    return *this;
  }

  inline modp operator- () const
  {
    return modp(p - n);
  }
  inline modp operator- (modp enn) const
  {
    return modp((n - enn.n + p) % p);
  }

  inline modp& operator-= (modp enn)
  {
    n = (n - enn.n + p) % p;
    return *this;
  }

  inline modp operator* (modp enn) const
  {
    return modp((enn.n*n) % p);
  }

  inline modp& operator*= (modp enn)
  {
    n = (n * enn.n) % p;
    return *this;
  }

  inline modp inv () const
  {
    return modp (egcd (p, n, 0 , 1));
  }

  modp operator/ (modp enn) const
  {
    if (enn.n == 0)
      {
        std::cerr << "Division by zero (modp<p>::operator/)" << std::endl;
        exit (1);
        return *this ;
      }
    modp answer = *this;
    return answer * enn.inv ();
  }

  modp& operator/= (modp enn)
  {
    if (enn.n == 0)
      {
        std::cerr << "Division by zero (modp<p>::operator/=)" << std::endl;
        exit (1);
        return *this;
      }
    operator*= (enn.inv ());
    return *this;
  }

  inline bool operator== (modp enn) const
  {
    return n == enn.n;
  }

  inline bool operator!= (modp enn) const
  {
    return n != enn.n;
  }

  inline operator bool () const
  {
    return n != 0;
  }

// inline T getInt () const { return n; }
  inline bool isOne () const
  {
    return n == 1;
  }

  inline bool isZero () const
  {
    return n == 0;
  }

  inline bool printsNeg () const
  {
    const T p_over_two = p/2;
    if (!negs) return false;
    return n > p_over_two;
  }

  void print (std::ostream& o)
  {
    const int p_over_two = p/2;
    T enn = n;
    if (negs && enn > p_over_two ) enn -= p;
    o << enn ;
  }

  private:

  T n;
  T egcd(T x, T y, T i, T j) const
  {
    if (y == 0) return i;
    return egcd(y, x % y, j, i - (x/y)*j);
  }
};

template<int p>
std::ostream& operator<< (std::ostream& o, modp<p> enn)
{
  enn.print (o);
  return o;
}
#endif

/////////////////////////////////// module−order.h ///////////////////
#ifndef MODULEORDERH
#define MODULEORDERH

#include "module_monomial.h"

class POT
{
  public :
  static const int mode = 1;
};

class TOP
{
  public :
  static const int mode = 2;
};

class g1
{
  public :
  static const int mode = 3;
};

class g2
{
  public :
  static const int mode = 4;
};

// class POT
template<class monomial>
bool inline operator<= (const module_monomial<monomial, POT>& m1,
                        const module_monomial<monomial, POT>& m2)
{
  if (m1.idx () < m2.idx ()) return true;
  if (m1.idx () > m2.idx ()) return false;
  return m1.m() <= m2.m();
}

template<class monomial>
bool inline operator< (const module_monomial<monomial, POT>& m1,
                       const module_monomial<monomial, POT>& m2)
{
  if (m1.idx () < m2.idx ()) return true;
  if (m1.idx () > m2.idx ()) return false;
  return m1.m() < m2.m();
}

// greater thans
template<class monomial, class mod_ord>
bool inline operator>(const module_monomial<monomial, mod_ord>& m1,
                      const module_monomial<monomial, mod_ord>& m2)
{
  return !(m1 <= m2);
}

template<class monomial, class mod_ord>
bool inline operator>= (const module_monomial<monomial, mod_ord>& m1,
                        const module_monomial<monomial, mod_ord>& m2)
{
  return !(m1 < m2);
}

// class TOP
template<class monomial>
bool inline operator<= (const module_monomial<monomial, TOP>& m1,
                        const module_monomial<monomial, TOP>& m2)
{
  if (m1.m() < m2.m()) return true;
  if (m1.m() > m2.m()) return false;
  return m1.idx () <= m2.idx ();
}

template<class monomial>
bool inline operator< (const module_monomial<monomial, TOP>& m1,
                       const module_monomial<monomial, TOP>& m2)
{
  if (m1.m() < m2.m()) return true;
  if (m1.m() > m2.m()) return false;
  return m1.idx () < m2.idx ();
}

// class g1
template<class monomial>
bool inline operator<= (const module_monomial<monomial, g1>& m1,
                        const module_monomial<monomial, g1>& m2)
{
  if (m1.wd() < m2.wd()) return true;
  if (m1.wd() > m2.wd()) return false;
  if (m1.m() < m2.m()) return true;
  if (m1.m() > m2.m()) return false;
  return m1.idx () <= m2.idx ();
}

template<class monomial>
bool inline operator< (const module_monomial<monomial, g1>& m1,
                       const module_monomial<monomial, g1>& m2)
{
  if (m1.wd() < m2.wd()) return true;
  if (m1.wd() > m2.wd()) return false;
  if (m1.m() < m2.m()) return true;
  if (m1.m() > m2.m()) return false;
  return m1.idx () < m2.idx ();
}

// class g2
template<class monomial>
bool inline operator<= (const module_monomial<monomial, g2>& m1,
                        const module_monomial<monomial, g2>& m2)
{
  if (m1.wm() < m2.wm()) return true;
  if (m1.wm() > m2.wm()) return false;
  if (m1.idx () < m2.idx ()) return true;
  if (m1.idx () > m2.idx ()) return false;
  return m1.m() <= m2.m();
}

template<class monomial>
bool inline operator< (const module_monomial<monomial, g2>& m1,
                       const module_monomial<monomial, g2>& m2)
{
  if (m1.wm() < m2.wm()) return true;
  if (m1.wm() > m2.wm()) return false;
  if (m1.idx () < m2.idx ()) return true;
  if (m1.idx () > m2.idx ()) return false;
  return m1.m() < m2.m();
}
#endif

/////////////////////////////////// term−order.h ////////////////////
#ifndef TERMORDERH
#define TERMORDERH

template<int n, class ORDERING, class T>

class monomial;
class Lex;
class GrRevLex;
class BackwardLex;

// class Lex
template<int n, class T>
bool inline operator<= (const monomial<n, Lex ,T>& l,
                        const monomial<n, Lex , T>& r )
{
  const T* left = l.getExponents ();
  const T* right = r.getExponents ();
  for (int i = 0; i < n; i++)
    {
      if (left[i] > right[i]) return false;
      if (left[i] < right[i]) return true;
    }
  return true;
}

template<int n, class T>
bool inline operator< (const monomial<n, Lex ,T>& l,
                       const monomial<n, Lex , T>& r)
{
  const T *left = l.getExponents (), *right = r.getExponents ();
  for (int i = 0; i < n; i++)
    {
      if (left [i] > right[i]) return false;
      if (left [i] < right[i]) return true;
    }
  return false;
}

// BackwardLex
template<int n, class T>
bool inline operator<= (const monomial<n, BackwardLex ,T>& l,
                        const monomial<n, BackwardLex , T>& r)
{
  const T *left = l.getExponents ();
  const T *right = r.getExponents ();
  for (int i = n-1; i >= 0; i--)
    {
      if (left[i] > right[i]) return false;
      if (left[i] < right[i]) return true;
    }
  return true;
}

template<int n, class T>
bool inline operator< (const monomial<n, BackwardLex ,T>& l,
                       const monomial<n, BackwardLex , T>& r)
{
  const T *left = l.getExponents () , *right = r.getExponents ();
  for (int i = n-1; i >= 0; i--)
    {
      if (left[i] > right[i]) return false;
      if (left[i] < right[i]) return true;
    }
  return false;
}

// GrRevLex
template<int n, class T>
bool inline operator<= (const monomial<n, GrRevLex , T>& l,
                        const monomial<n, GrRevLex , T>& r )
{
  if (l.getDegree () < r.getDegree ()) return true;
  if (l.getDegree () > r.getDegree ()) return false;
  const T *left = l.getExponents ();
  const T *right = r.getExponents ();
  for (int i = n-1; i >= 0; i--)
    {
      if (left[i] > right[i]) return true;
      if (left[i] < right[i]) return false;
    }
  return true;
}

template<int n, class T>
bool inline operator< (const monomial<n, GrRevLex , T>& l,
                       const monomial<n, GrRevLex , T>& r)
{
  if (l.getDegree () < r.getDegree ()) return true;
  if (l.getDegree () > r.getDegree ()) return false;
  const T *left = l.getExponents ();
  const T *right = r.getExponents ();
  for (int i = n-1; i >= 0; i--)
    {
      if (left[i] > right[i]) return true;
      if (left[i] < right[i]) return false;
    }
  return false;
}

// greater thans
template<int n, class ORDERING, class T>
bool inline operator> (const monomial<n, ORDERING, T>& l,
                       const monomial<n, ORDERING, T>& r)
{
  return !(l <= r);
}

template<int n, class ORDERING, class T>
bool inline operator>= (const monomial<n, ORDERING, T>& l,
                        const monomial<n, ORDERING, T>& r)
{
  return !(l < r);
}
#endif

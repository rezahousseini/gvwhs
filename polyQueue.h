/////////////////////////////////// polyQueue.h //////////////////////
#ifndef POLYQUEUEH
#define POLYQUEUEH

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

template<class poly>

class polyQueue
{
  public:

  typedef typename poly::term term;
  typedef typename poly::monomial monom;
  typedef typename poly::coefficient coeff;

  polyQueue (const poly& p)
  {
    array.push_back (new poly (p));
    mults.push_back (coeff (1));
  }

  ~polyQueue ()
  {
    for (size_t i = 0; i < array.size (); i++) delete array[i];
  }

  inline const coeff lc () const
  {
    return array[0]->lc ();
  }

  inline const monom& lm() const
  {
    return array[0]->lm ();
  }

  inline const term& lt () const
  {
    return array[0]->lt ();
  }

  inline bool isZero () const
  {
    return array.empty ();
  }

  void operator/= (const coeff& c)
  {
    const coeff one (1);
    if (!array.empty ())
      {
        if (c != one)
          {
            coeff cinv = c.inv ();
            for (int i = 0; i < mults.size (); i++)
              mults[i] *= cinv;
          }
        scaleZeroth ();
      }
  }

  void sub2cancel (const poly& p)
  {
    poly* q = new poly (-p);
    array[0]->pop_front (); // leading terms just cancelled
    if (array[0]->isZero ())
      {
        delete array[0];
        array [0] = array.back ();
        mults [0] = mults.back ();
        array.pop_back ();
        mults.pop_back ();
      }
    bubble_down (1);
    q->pop_front ();
    if (!q->isZero ())
      {
        array.push_back (q);
        mults.push_back (coeff (1));
        bubble_up (array.size ());
      }
    else delete q;
    fix_lm ();
  }

  poly toPoly ()
  {
    poly answer;
    while (!isZero ())
      {
        answer.push_back (lt ());
        array[0]->pop_front ();
        if (array[0]->isZero ())
          {
            delete array[0];
            array[0] = array.back ();
            mults[0] = mults.back ();
            array.pop_back ();
            mults.pop_back ();
          }
        bubble_down (1);
        fix_lm ();
      }
    return answer;
  }

  private:

  void scaleZeroth ()
  {
    const coeff one (1);
    if (mults[0] != one)
      {
        array[0]->operator*= (mults[0]);
        mults[0] = one ;
      }
  }

  void fix_lm ()
  {
    bool repeat = true ;
    while (!array.empty() && repeat)
      {
        repeat = false ;
        for (size_t pos = 1; pos <= 2; pos++)
          {
            if (array.size () > pos && array[pos]->lm() == array[0]->lm())
              {
                repeat = true;
                array[0]->addCoeff2LC (array[pos]->lc ()*mults[pos]/mults[0]);
                array[pos]->pop_front ();
                if (array[pos]->isZero ())
                  {
                    delete array [pos] ;
                    array[pos] = array.back ();
                    mults[pos] = mults.back ();
                    array.pop_back ();
                    mults.pop_back ();
                  }
                bubble_down (pos+1);
              }
          }
        if (array[0]->lc ().isZero ())
          {
            array[0]->pop_front ();
            if (array[0]->isZero ())
              {
                delete array[0];
                array[0] = array.back ();
                mults[0] = mults.back ();
                array.pop_back ();
                mults.pop_back ();
              }
            bubble_down (1);
          }
      }
    const coeff one (1);
    if (mults[0] != one && !array.empty ()) scaleZeroth ();
  }

  void bubble_up (int pos)   // one based argument
  {
    while (pos > 1) 
      {
        if (array[pos - 1]->lm() <= array [(pos >> 1) - 1]->lm()) break;
        std::swap (array[pos - 1], array[(pos >> 1) - 1]);
        std::swap (mults[pos - 1], mults[(pos >> 1) - 1]);
        pos >>= 1;
      }
  }

  void bubble_down (size_t pos = 1)   // one based argument
  {
    if (pos << 1 <= array.size ())
      {
        size_t theLarger = pos << 1;
        if (theLarger + 1 <= array.size ()
             && array[theLarger - 1]->lm() < array[theLarger]->lm())
          theLarger++;
        if (array[theLarger - 1]->lm() > array[pos - 1]->lm())
          {
            std::swap (array[theLarger - 1], array[pos - 1]);
            std::swap (mults[theLarger - 1], mults[pos - 1]);
            bubble_down (theLarger);
          }
      }
  }

  std::vector<poly*> array;
  std::vector<coeff> mults;
};

#endif

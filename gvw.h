/////////////////////////////////// gvw .h ////////////////////////////
#ifndef GVWH
#define GVWH
#include <sstream>
#include <fstream>
#include <iosfwd>
#include <vector>
#include <list >
#include <queue>
#include "j−pair.h"
#include "polynomial.h"
#include "polyQueue.h"
#include <iterator>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include "poly_formatter.h"
#include "pqueue.h"

using namespace std ;

extern const char ∗ sing ;
int jPairs = 0;
template<class term , class mod_elt>

class gvw
{
public :

  typedef polynomial<term> poly ;
  typedef vector<poly> polyList ;
  typedef j pair <mod_elt , poly> jpair ;
  typedef typename mod_elt::monomial_mod_mon;
  typedef typename poly::coefficient_coefficient ;
  typedef typename poly::monomial_monom;
  typedef typename mod_elt::mod_order_module_ordering ;

  gvw(poly_formatter<poly> *format, const char *name = "gb") :
    reg0red(0), sup0red(0), polyFormatter(format),
    isReduced(false), isGroebner(false)
  {
    polyList vee = polyFormatter−>getBasis ();
    /////////////// RUN THE ALGORITHM ///////////////////
    ////// THIS STUFF COULD BE IN FUNCTION MAIN() ///////
    calculateGB ( vee );
#ifdef BRIEF
    printBriefStatistics ( cout );
#else
    printStatistics (cout , true );
#endif
    // uncomment for the basis output to a f i l e
    // ostringstream str ;
    // str << ”output/” << name << ”. txt ”;
    // ofstream f i l e ( str . str (). c str ());
    // // f i l e << sing << endl ;
    // f i l e << ” ideal j = ideal ();” << endl ;
    // // polyFormatter−>updateBasis ( getBasis ());
    // // polyFormatter−>printBasis ( file , ”j ”);
    // makeMinimal ();
    // vector<poly> ∗ vec = &V;
    // if (vec−>empty ()) vec = &minimalBasis ;
    // for ( int i = 0; i < vec−>size (); i++) {
    // f i l e << ”j [” << ( i+1) << ”] = ”;
    // polyFormatter−>printPoly ( file , vec−>operator []( i ));
    // f i l e << ”;” << endl ;
    // }
    // f i l e << ” validate2ideals ( sortIdeal ( j ) ,
    // makeMonic( groebner ( i )));” << endl ;
  }

  const polyList& getBasis()
  {
    if (! minimalBasis . empty ())
      return minimalBasis ;
    else return V;
  }

  const polyList& getMinimalBasis()
  {
    makeMinimal ();
    return minimalBasis ;
  }

  const polyList& getReducedBasis()
  {
    makeReduced ();
    return minimalBasis ;
  }

  void calculateGB (const polyList & vee )
  {
    if ( isGroebner ) return ;
    mod_elt::set generators ( vee );
    H. resize ( vee . size ());
    for ( int i = 0; i < vee . size (); i++)
      JPairs.enqueue( jpair ( mod elt ( i ) , vee [ i ] ) ) ;
    while (! JPairs . empty ())
      processJPair ( JPairs . dequeue ());
    isGroebner = true ;
    sizeOfBasis = V. size ();
  }

private :

  int findReductor (const mod elt & u, const term & v , monom & scale )
  {
    // −1 = No Reductor , −2 = Super Top−Reducible
    int answer = −1;
    for ( int i = 0; i < VLeadMonoms. size (); i++)
      {
        if (v.m(). isDivisibleBy (VLeadMonoms[ i ]))
          {
            scale = v.m() / VLeadMonoms[ i ] ;
            mod elt scaledSig = U[ i ] ∗ scale ;
            if ( scaledSig < u) return i ;
            if ( scaledSig == u) answer = −2;
          }
      }
    return answer ;
  }

  int regReduce( mod elt & u, poly & v)
  {
    int result = topReduce(u,v );
    if ( result < 0) return result ;
    poly vee ;
    while (! v. isZero ())
      {
        vee . push back (v. lt ());
        v. pop front ();
        topReduce(u,v );
      }
    v = vee ;
    return −1;
  }

  int topReduce(mod_elt& u, poly& v)
  {
    // POLYQUEUE
    const coefficient one (1);
    if (v.isZero ()) return −1;
    monom scale;
    polyQueue<poly> queue(v);
    while (true)
      {
        if (queue.isZero ())
          {
            // new syzygy
            v = poly ();
            return −1;
          }
        int reductor = findReductor (u, queue.lt(), scale);
        if (reductor == −2)
          {
            // irreducible but not primitive
            return −2;
          }
        if (reductor == −1)
          {
            // primitive
            v = queue.toPoly ();
            return reductor ;
          }
        coefficient c = queue.lc() / V[reductor].lc();
        queue.sub2cancel (V[reductor] * term(scale, c));
      }
  }

  void anticipateSyzygies (mod_elt u, term v)
  {
    int j, size = U.size ();
    for (j = 0; j < size ; j++)
      {
        // Calculate a new Syzygy
        mod_elt syzScaleSig [2] = { U[j]*v.m() ,u*VLeadMonoms[j] };
        if (syzScaleSig[0] != syzScaleSig[1] || V[j].lc() != v.c())
          {
            int largerSig = (syzScaleSig[0] > syzScaleSig[1]) ? 0 : 1;
            if (!reducibleByH (syzScaleSig[largerSig]))
              insertH (syzScaleSig[largerSig]);
          }
      }
  }

  void updatePairs (int i)
  {
    for (int j = 0; j < i ; j++)
      {
        monom lcm = VLeadMonoms[i].lcm(VLeadMonoms[j]);
        monom scale[2] = { lcm / VLeadMonoms[i], lcm / VLeadMonoms[j] };
        mod_elt scaleSig[2] = { U[i] * scale[0], U[j] * scale[1] };
        int indices[2] = { i, j };
        if (scaleSig[0] == scaleSig[1]) continue;
        int largerSig = (scaleSig[0] > scaleSig[1]) ? 0 : 1;
        if (!reducibleByH (scaleSig[largerSig]))
          {
            jpair jp ( scale[largerSig], indices[largerSig],
                       U[indices[largerSig]],
                       VLeadMonoms[indices[largerSig]],
                       V[indices[largerSig]].size ());
            JPairs.enqueue(jp);
          }
      }
  }

  void processJPair (jpair jp)
  {
    mod_elt u = jp.uPart ();
    if (!reducibleByH (u) && isMPair (u, jp.vMonomial()))
      {
        jPairs++;
        poly v = jp.vPolynomial(V);
        int result = regReduce(u, v);
        if (result == −2)   // super top−reducible
          {
            sup0red++;
            return ;
          }
        if (v.isZero())
          {
            reg0red++;
            insertH (u);
            return ;
          }
        anticipateSyzygies (u, v.lt());
        U.push_back (u);
        V.push_back (v);
        VLeadMonoms.push_back (v.lm());
        updatePairs (U.size() − 1);
      }
  }

  bool isMPair (const mod_elt& u, const monom& lm)
  {
    for (int i = 0; i < U.size(); i++)
      {
        if (u.isDivisibleBy(U[i]))
          {
            monom m = u.m() / U[i].m();
            if (lm > VLeadMonoms[i] * m) return false;
          }
      }
    return true;
  }

  void printH ()
  {
    for (int i = 0; i < H.size (); i++)
      {
        cout << "H[ " << (i+1) << " ].size() = " << H[i].size() << endl;
        for (int j = 0; j < H[i].size(); j++)
          {
            cout << " " << (i+1) << " , ";
            polyFormatter−>printMonom (cout, H[i][j], false);
            cout << endl;
          }
      }
  }

  bool reducibleByH (const mod_elt& m)
  {
    const mod_mon& mon = m.m();
    int pos = m.idx();
    const vector<mod_mon>& HList = H[pos];
    for (int i = 0; i < HList.size (); i++)
      if (mon.isDivisibleBy (HList[i])) return true;
    return false ;
  }

  void insertH (const mod_elt& m)
  {
    vector<mod_mon>& targetList = H[m.idx()];
    const mod_mon& mon = m.m();
    int size = targetList.size();
    vector<mod_mon> newList;
    for (int i = 0; i < size ; i++)
      {
        if (!targetList[i].isDivisibleBy(mon))
          {
            newList.push_back (targetList[i]);
          }
      }
    newList.push_back (mon);
    targetList.swap (newList);
  }

  void makeMinimal ()
  {
    if (!minimalBasis.empty()) return;
    if (!isGroebner)
      {
        cerr << "ERROR: non Groebner basis! " << endl;
        exit (1);
      }
    list <poly> minimal;
    while (!V.empty())   // save memory
      {
        minimal.push_front (V.back());
        V.pop_back ();
      }
    vector<poly>().swap(V);
    minimal.sort ();
    typename list <poly>::iterator divisor, dividend;
    for (divisor = minimal.begin(); divisor != minimal.end(); ++divisor)
      {
        dividend = divisor;
        for (++dividend; dividend != minimal.end();)
          if ( dividend−>isDivisibleBy (*divisor))
            dividend = minimal.erase (dividend);
          else ++dividend;
      }
    typename list <poly>::iterator itr;
    for (itr = minimal.begin ();
         itr != minimal.end ();
         itr = minimal.erase (itr))
      minimalBasis.push_back (itr−>makeMonic());
  }

  void makeReduced ()
  {
    if (isReduced) return ;
    if (!isGroebner)
      {
        cerr << "ERROR: non Groebner basis!" << endl;
        exit (1);
      }
    makeMinimal ();
    int i, j;
    int redo = 0;
    for (i = 1; i < minimalBasis.size (); i++)
      {
        // generator i is now fully reduced
        int changed = 0;
        for ( j = i − 1; j >= 0; j−−)
          changed += minimalBasis[i].divideThrough (minimalBasis[j]);
        if (changed) −−i; // redo it
      }
    isReduced = true ;
    for (int i = 0; i < minimalBasis.size (); ++i)
      minimalBasis[i].makeMonic ();
  }

  void printStatistics (ostream& o, bool min = true)
  {
    o << "\nAlgorithm Statistics :" << endl;
    o << " −JPairs processed :      " << jPairs << endl;
    if (min && minimalBasis.empty()) makeMinimal();
    if (min)
      o << " −Size of Groebner Basis (minimal): " << minimalBasis.size() << endl;
    o << " −Size of Groebner Basis (computed): " << sizeOfBasis << endl;
    o << " −Regular Zero Reductions :        " << reg0red << endl;
    o << " −Super Top−reductions :        " << sup0red << endl;
    ostringstream str;
    int sizeOfH = 0;
    str << " [ ";
    for (int i = 0; i < H.size(); i++)
      {
        if ( i ) str << " , " ;
        sizeOfH += H[i].size (); // changed Hnew to H
        str << H[i].size ();
      }
    str << " ] ";
    o << " −Size of H: " << sizeOfH << " " << str.str () << endl;
    o << endl;
  }

  void printBriefStatistics (ostream& o)
  {
    o << "MODE " << module ordering::mode << " : (" ;
    if (minimalBasis.empty ()) makeMinimal ();
    o << jPairs << " , " << minimalBasis.size () << "/ " << sizeOfBasis;
    int sizeOfH = 0;
    for (int i = 0; i < H.size (); i++)
      sizeOfH += H[i].size ();
    o << " , " << sizeOfH << " , " << reg0red << " , " << sup0red;
  }

  /*∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗∗*/
  vector<vector<mod_mon> > H;
  pqueue<jpair> JPairs ;
  vector<mod_elt> U;
  polyList V;
  vector<monom> VLeadMonoms;
  polyList minimalBasis ;
  bool isGroebner , isReduced ;
  poly formatter<poly> ∗ polyFormatter ;
  // statistics
  int reg0red , sup0red ;
  int sizeOfBasis ;
};
#endif

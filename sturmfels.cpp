#include <fstream>
#include <vector>
#include <iostream>
#include "modp.h"
#include "monomial.h"
#include "polynomial.h"
#include "module_monomial.h"
#include "module_order.h"
#include "j_pair.h"
#include "pqueue.h"
#include "singular.h"
#include "gvw.h"

using namespace std ;

// characteristic : 7583
// number of vars : 7
// block 1 : ordering dp
// : names x y z t u v h
// block 2 : ordering C
// i [1]=2x2+2y2+2z2+2t2+2u2+v2−vh
// i [2]=xy+yz+2zt+2tu+2uv+uh
// i [3]=2 xz+2yt+2zu+u2+2tv−th
// i [4]=2 xt+2yu+2tu+2zv−zh
// i [5]= t2+2xv+2yv+2zv−yh
// i [6]=2x+2y+2z+2t+2u+v−h
string vars[] = {"x", "y", "z", "t", "u", "v", "h"};
string j[] = {"2x2+2y2+2z2+2t2+2u2+v2−vh",
              "xy+yz+2zt+2tu+2uv+uh",
              "2xz+2yt+2zu+u2+2tv−th",
              "2xt+2yu+2tu+2zv−zh",
              "t2+2xv+2yv+2zv−yh",
              "2x+2y+2z+2t+2u+v−h" 
             };

typedef monomial<sizeof (vars) / sizeof (vars[0]), GrRevLex> mon;
typedef aTerm<mon, modp<7583> > term;
typedef polynomial<term> poly;
typedef module_monomial<mon, TOP> mod;
typedef j_pair <mod, poly> uv;
template<class MONOM, class ORD> vector<MONOM>
  module_monomial<MONOM, ORD>::generator_monomials;
template<class MONOM, class ORD> vector<int>
  module_monomial<MONOM, ORD>::generator_degrees;

int 
main() 
{
  singular<poly> singularFormatter;

  // ALL 2 OF THE NEXT LINES ARE EXAMPLE DEPENDENT
  singularFormatter.addVariables (vars, sizeof (vars) / sizeof (vars[0]));
  singularFormatter.addPolynomials (j, sizeof (j) / sizeof (j[0]));

  cout << "Original polynomials: " << endl;
  for (size_t i = 0; i < sizeof (j) / sizeof (j[0]); i++)
  cout << "j[" << (i+1) << "] = " << j[i] << endl;
  cout << endl;
  //cout << "Polynomials loaded they print as: " << endl;
  //singularFormatter.printBasis (cout, "j");
  //cout << endl;

  gvw<term, mod> gb (&singularFormatter, "katsura5");

  return 0;
}

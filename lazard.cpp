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

using namespace std;

string vars[] = {"a", "b", "c", "d"};
string j[] = {"a+b+c+d",
              "ab+bc+cd+da",
              "abc+bcd+cda+dab",
              "abcd-1" 
             };

typedef monomial<sizeof (vars) / sizeof (vars[0]), Lex> mon;
typedef aTerm<mon, modp<2> > term;
typedef polynomial<term> poly;
typedef module_monomial<mon, POT> mod;
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

  gvw<term, mod> gb (&singularFormatter, "arnborn");

  return 0;
}

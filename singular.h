/////////////////////////////////// singular.h ///////////////////////
#ifndef SINGULAR_H
#define SINGULAR_H

#include "poly-formatter.h"

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <iosfwd>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>

const bool debug = false;
typedef std::string::size_type size_t;

template<class poly>
class singular : public poly_formatter<poly>
{
  public:
  typedef typename poly::monomial monomial;
  typedef typename monomial::expType expType;
  typedef typename poly::coefficient coefficient;
  typedef typename poly::term term;

  void printMonom (std::ostream& o, const monomial& m,
                   bool printedCoeff )
  {
    std::vector<int> expons = m.getExpVector ();
    const int numVars = monomial::numVars;
    bool timesFlag = printedCoeff;
    int lastPrt = -1;
    for (int j = 0; j < numVars; j++)
      {
        if (expons[j] == 0) continue;
        if (timesFlag)
          {
            if ((lastPrt >= 0
                 && poly_formatter<poly>::revMapping[lastPrt].size () > 1)
                 || poly_formatter<poly>::revMapping[j].size () > 1)
              o << "∗";
          }
        lastPrt = j ;
        o << poly_formatter<poly>::revMapping[j];
        if (expons[j] > 1)
          {
            if (poly_formatter<poly>::revMapping[j].length () > 1) o << "^ ";
            o << expons[j];
          }
        timesFlag = true;
      }
  }

  void printPoly (std::ostream& o, const poly& p)
  {
    const std::list<term>& termList = p.getList ();
    typename std::list<term>::const_iterator itr;
    bool needPlus = false;
    for (itr = termList.begin (); itr != termList.end (); ++itr)
    {
      if (needPlus && !itr->printsNeg ()) o << "+";
      needPlus = true;
      if (itr->m().isConstant () || !itr->c ().isOne ()) o << itr->c ();
      if (!itr->m().isConstant ())
        {
          printMonom (o, itr->m(), !itr->c ().isOne ());
        }
    }
  }

  void printBasis (std::ostream& o, const std::string& idealName)
  {
    for (int i = 0; i < poly_formatter<poly>::theBasis.size (); i++)
      {
        o << idealName << " [ " << (i + 1) << " ] = ";
        printPoly (o, poly_formatter<poly>::theBasis[i]);
        o << " ;" << std::endl;
      }
  }

  int addPolynomial (const std::string& p)
  {
    if (debug) std::cerr << "−−−−−−Calling addPolynomial on " << p << std::endl;
    size_t start = 0, end = p.length ();
    int monomialCount = 0;
// term is negative , the ’−’ needs to be position 0
    poly thisPoly;
    while (start <= p.length () - 1)
      {
        monomialCount++;
        end = p.findfirstof ("+−", start + 1);
        if (end > p.length ()) end = p.length ();
        if (debug) std::cerr << "−−−−−−−Calling getNextMon on " 
                             << p.substr (start, end - start) 
                             << " " << std::endl;
        thisPoly += getNextMon(p, start, end);
        start = end;
      }
    poly_formatter<poly>::theBasis.push_back (thisPoly);
    return monomialCount;
  }

  private:

  term getNextMon (const std::string& p, size_t& start, size_t end)
  {
    if (debug) std::cerr << "Over [ " << start << " , " << end << " ): ";
    if (p[start] == "+") ++start;
    bool negative = (p[start] == "−") ? true : false;
    if (debug && negative) std::cerr << "NEG ";
    if (negative) ++start;
    int intCoeff = 1;
    if (isdigit (p[start]))
    {
      sscanf (p.substr (start, end - start).c_str (), "%d", &intCoeff);
      start = p.find_first_not_of ("0123456789", start);
      if (debug) std::cerr << "Coef(" << intCoeff << ") ";
    }
    if (debug) std::cerr << std::endl;
    if ( negative ) intCoeff = -intCoeff ;
    const static int numVars = monomial::numVars;
    std::vector<int> expons (numVars);
    while ( start < end)
      {
        start = p.find_first_not_of (" ∗", start);
        std::string varName(1, p[start++]);
        if (p[start] == " ( " )
          {
            int lastParen = p.find_first_not_of ("(0123456789)", start);
            varName += p.substr (start, lastParen - start);
            start = lastParen;
          }
        if (debug) 
          {
            std::cerr << "\tvarName(" << varName << ") " << "varIdx ("
                      << poly_formatter<poly>::varMapping[varName] << ") ";
          }
        if (p[start] == "^ ") ++start;
        if (isdigit (p[start]))
          {
            int lastOfExp = p.find_first_not_of ("1234567890", start);
            int exp;
            sscanf (p.substr (start, lastOfExp - start).c_str (), "%d", &exp);
            expons[poly_formatter<poly>::varMapping[varName]] += exp;
            start = lastOfExp;
          }
        else expons[poly_formatter<poly>::varMapping[varName]] += 1;
        if (debug) std::cerr << "exp("
                             << expons[poly_formatter<poly>::varMapping[varName]]
                             << ")" << std::endl;
      }
    return term(monomial (expons), coefficient (intCoeff));
  }
};
#endif

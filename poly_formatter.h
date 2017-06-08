/////////////////////////////////// poly-formatter.h /////////////////
#ifndef POLYFORMATTERH
#define POLYFORMATTERH

#include <string>
#include <iosfwd>
#include <map>
#include <vector>

template<class poly>
class poly_formatter
{
  public:

  typedef typename poly::monomial monomial;
  typedef typename poly::coefficient coefficient;
  typedef typename poly::term term;

  virtual void printMonom (std::ostream& o, const monomial& m,
                           bool printedCoeff ) = 0;
  virtual void printPoly (std::ostream& o, const poly& p) = 0;
  virtual void printBasis (std::ostream& o,
                           const std::string& name) = 0;
  virtual int addPolynomial (const std::string& p) = 0;

  poly_formatter () : varNumber(0) { }

  int addVariable (const std::string& s)
  {
    varMapping[s] = varNumber++;
    revMapping.push_back (s);
    if (revMapping.size () > monomial::numVars)
      {
        std::cerr << "ERROR: Inserted more than " << monomial:: numVars
                  << " variables .\n\n" << std::endl;
        exit (1);
      }
    return 1;
  }

  int addVariables (std::string c[], int n)
  {
    for (int i = 0; i < n; i++) addVariable (c[i]);
    return n;
  }

  int addVariables (const char *f, int start, int end)
  {
    char buffer [256];
    for (int i = start; i <= end; i++)
      {
        sprintf (buffer, f, i);
        addVariable (buffer);
      }
    return end - start + 1;
  }

  const std::vector<poly>& getBasis ()
  {
    return theBasis;
  }

  void printVariables ()
  {
    std::map<std::string, int>::iterator itr;
    std::cout << " varMapping revMapping" << std::endl;
    for (itr = varMapping.begin (); itr != varMapping.end (); ++itr)
      std::cout << itr->first << " => " << itr->second << " => "
                << revMapping[itr->second] << std::endl;
  }

  void updateBasis (const std::vector<poly>& basis)
  {
    theBasis = basis;
  }

  void addPolynomials (const std::string *p, int n)
  {
    for (int i = 0; i < n; i++)
      {
        addPolynomial(p[i]);
      }
  }
  
  protected:

  int varNumber;
  std::vector<poly> theBasis;
  std::map<std::string, int> varMapping;
  std::vector<std::string> revMapping;
};
#endif

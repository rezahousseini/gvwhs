/////////////////////////////////// module−monomial.h ////////////////
#ifndef MODULEMONOMIALH
#define MODULEMONOMIALH

#include <iosfwd>
#include <vector>
template<class monom, class module_ordering>
class module_monomial
{
  public:

  typedef monom monomial;
  typedef module_ordering mod_order;
  module_monomial (const monom& m, int idx)
  {
    myMon = m;
    myIdx = idx ;
    updateGWeighted ();
  }

  module_monomial (int idx)   // constructs a unit vector
  {
    myIdx = idx;
    myWeightedMon = generator_monomials[idx];
    myWeightedDegree = generator_degrees[idx];
  }

  inline const monom& m() const
  {
    return myMon;
  }

  inline int idx () const
  {
    return myIdx;
  }

  inline const monom& wm() const
  {
    return myWeightedMon;
  }

  inline int wd() const
  {
    return myWeightedDegree;
  }

  inline bool isDivisibleBy (const module monomial& m) const
  {
    if (m.myIdx != myIdx) return false;
    return myMon.isDivisibleBy (m.myMon);
  }

  inline bool isDivisibleBy (const monom& m) const
  {
    return myMon.isDivisibleBy (m);
  }

  template<class poly>
  static void set_generators (const std::vector<poly>& g)
  {
    for (int i = 0; i < g.size (); i++)
      {
        generator_monomials.push_back (monom(g[i].lm ().getExponents  ()));
        generator_degrees.push_back (generator_monomials[i].getDegree ());
      }
  }

  inline bool operator== (const module_monomial& m) const
  {
    return myIdx == m.myIdx && myMon == m.myMon;
  }

  inline bool operator!= (const module_monomial& m) const
  {
    return myIdx != m.myIdx || myMon != m.myMon;
  }

  template<class otherMonomial>
  module_monomial operator* (const otherMonomial& mon) const
  {
    module_monomial answer = *this ;
    monom castedMonom(mon.getExponents ());
    answer.myMon *= castedMonom;
    answer.updateGWeighted ();
    return answer;
  }

  void print (std::ostream& o) const
  {
    const int oneBased = 1;
    o << " ( " << (myIdx + oneBased) << " , " << myMon << " ) ";
  }

  private:

  void updateGWeighted ()
  {
    myWeightedDegree = myMon.getDegree () + generator_degrees[myIdx];
    myWeightedMon = myMon*generator_monomials[myIdx];
  }

  monom myMon;
  monom myWeightedMon;
  int myIdx; // index within Rˆm, 1 <= myIdx <= m
  int myWeightedDegree ;
  static std::vector<monom> generator_monomials;
  static std::vector<int> generator_degrees;
};

template<class monom, class module ordering>
std::ostream& operator<< (std::ostream& o,
                          const module_monomial<monom, module ordering>& m)
{
  m.print (o);
  return o;
}
#endif

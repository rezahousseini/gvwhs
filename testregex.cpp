#include <iostream>
#include <string>
#include <regex>

using namespace std;

int 
main() 
{
  std::string s ("this subject has a submarine as a subsequence");
  std::smatch m;
  std::regex e ("([-\+])");   // matches words beginning by "sub"

  std::cout << "Target sequence: " << s << std::endl;
  std::cout << "Regular expression: /\\b(sub)([^ ]*)/" << std::endl;
  std::cout << "The following matches and submatches were found:" << std::endl;

  while (std::regex_search (s,m,e)) {
    for (auto x:m) std::cout << x << " ";
    std::cout << std::endl;
    s = m.suffix().str();
  }

  return 0;
}

def options(opt):
  opt.load('compiler_cxx')

def configure(conf):
  conf.load('compiler_cxx')

def build(bld):
  bld.program(source=['testregex.cpp'], 
              target='testregex',
              cxxflags='-g -std=c++11 -fdiagnostics-color=always -Wall') 

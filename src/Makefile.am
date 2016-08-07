
bin_PROGRAMS = archtop
archtop_SOURCES=archtop.cpp
archtop_LDFLAGS=`wx-config --libs`
archtop_LDADD=$(DXFLIB_LIBS)
archtop_CFLAGS=$(DXFLIB_CFLAGS) \
               `wx-config --cxxflags` \
               -std=gnu++11
archtop_CXXFLAGS=$(archtop_CFLAGS)

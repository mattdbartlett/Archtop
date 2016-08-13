
noinst_LTLIBRARIES = libgtest.la libgmock.la
libgtest_la_SOURCES=googletest/googletest/src/gtest-all.cc
libgtest_la_LDFLAGS=-lpthread
libgtest_la_CXXFLAGS=-Igoogletest/googletest/include \
                     -Igoogletest/googletest/ \
                     -Werror \
                     -Wall \
                     -std=gnu++11


libgmock_la_SOURCES=googletest/googlemock/src/gmock-all.cc
libgmock_la_LDFLAGS=
libgmock_la_CFXXLAGS=-Igoogletest/googlemock/include \
                     -Igoogletest/googletest/include \
                     -Igoogletest/googletest/ \
                     -Igoogletest/googlemock/ \
                     -Werror \
                     -Wall \
                -std=gnu++11

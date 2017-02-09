noinst_LIBRARIES = libstringer.a libsubstrsorter.a libcartesiantree.a librmq.a
libstringer_a_SOURCES = stringer.cpp
libsubstrsorter_a_SOURCES = substrSorter.cpp
libsubstrsorter_a_LIBADD = libcartesiantree.a
libcartesiantree_a_SOURCES = CartesianTree.cpp
librmq_a_SOURCES = RMQ.cpp

noinst_PROGRAMS = trivialMain generateInput mainTest cartesianTreeTest skewMain hackerrank_version rmqTest
trivialMain_SOURCES = trivialMain.cpp
generateInput_SOURCES = generateInput.cpp
mainTest_SOURCES = mainTest.cpp libsubstrsorter.a
mainTest_LDADD = libsubstrsorter.a libcartesiantree.a
cartesianTreeTest_SOURCES = cartesianTreeTest.cpp
cartesianTreeTest_LDADD = libcartesiantree.a
skewMain_SOURCES = skewMain.cpp
skewMain_LDADD = libstringer.a libsubstrsorter.a libcartesiantree.a
hackerrank_version_SOURCES = hackerrank_version.cpp
rmqTest_SOURCES = RmqTester.cpp
rmqTest_LDADD = librmq.a

CXX = clang++
AM_CXXFLAGS = -std=c++14

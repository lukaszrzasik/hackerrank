noinst_PROGRAMS = trivialMain generateInput mainTest cartesianTreeTest
trivialMain_SOURCES = trivialMain.cpp
generateInput_SOURCES = generateInput.cpp
mainTest_SOURCES = mainTest.cpp libsubstrsorter.a
mainTest_LDADD = libsubstrsorter.a
cartesianTreeTest_SOURCES = cartesianTreeTest.cpp libcartesiantree.a
cartesianTreeTest_LDADD = libcartesiantree.a

noinst_LIBRARIES = libstringer.a libsubstrsorter.a libcartesiantree.a
libstringer_a_SOURCES = stringer.cpp
libsubstrsorter_a_SOURCES = substrSorter.cpp
libcartesiantree_a_SOURCES = CartesianTree.cpp

noinst_LIBRARIES = libstringer.a libsubstrsorter.a libcartesiantree.a
libstringer_a_SOURCES = stringer.cpp
libsubstrsorter_a_SOURCES = substrSorter.cpp
libsubstrsorter_a_LIBADD = libcartesiantree.a
libcartesiantree_a_SOURCES = CartesianTree.cpp

noinst_PROGRAMS = trivialMain generateInput mainTest cartesianTreeTest skewMain
trivialMain_SOURCES = trivialMain.cpp
generateInput_SOURCES = generateInput.cpp
mainTest_SOURCES = mainTest.cpp libsubstrsorter.a
mainTest_LDADD = libsubstrsorter.a libcartesiantree.a
cartesianTreeTest_SOURCES = cartesianTreeTest.cpp
cartesianTreeTest_LDADD = libcartesiantree.a
skewMain_SOURCES = skewMain.cpp
skewMain_LDADD = libstringer.a libsubstrsorter.a libcartesiantree.a

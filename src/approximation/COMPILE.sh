# Determine OS platform
python -mplatform | grep -qi Ubuntu && DISTRO=Ubuntu || DISTRO=Centos

IN=approximate.cpp
OUT=approximate.o
LANGUAGE=c++17
OPTIMIZATION=1 # 2-3 causes timing issues with certain optimizations leading to nan values.
OPENMP=openmp-simd
ARCH=core-avx2
INCLUDE=''

if [ "$DISTRO" = "Ubuntu" ]
then
    EXECUTABLE=approximate-ubuntu-x86_64
elif [ "$DISTRO" = "Centos" ]
then
    EXECUTABLE=approximate-centos7-x86_64
fi

# Compile
g++ -o $OUT -c $IN -pg -std=$LANGUAGE -O$OPTIMIZATION -march=$ARCH -funroll-loops \
-ftree-vectorize -fopt-info-vec-optimized -fstrict-aliasing -f$OPENMP \
-lpthread #

# Linking
g++ -o $EXECUTABLE $OUT

g++ test.cpp -o test -std=$LANGUAGE
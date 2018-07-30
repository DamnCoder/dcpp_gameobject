cd ..

mkdir build_vs
cd build_vs

cmake ../project/ -G "Visual Studio 14 2015" -T "LLVM-vs2014"

exit
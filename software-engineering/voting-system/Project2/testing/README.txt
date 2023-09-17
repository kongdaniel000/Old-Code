Use the following command to make a file, then run it.

1: Compile with:
clang++ -Wall -std=c++11 <.cc file name>.cc -o <output file name>.out -lgtest -pthread

2: Run with:
./<output file name>.out

As an example:
clang++ -Wall -std=c++11 <.cc file name>.cc -o <output file name>.out -lgtest -pthread
./example.out

cd src/
g++ -Wall -Werror -pedantic -std=c++11 -o mini-fps *.cpp -I/Library/Frameworks/SDL2.framework/Headers -I./glad/include -F/Library/Frameworks -framework SDL2
# clang++ main.cpp ./glad/src/glad.c -o prog -I/Library/Frameworks/SDL2.framework/Headers -I./glad/include -F/Library/Frameworks -framework SDL2
mv mini-fps ../app
cd ../app
./mini-fps
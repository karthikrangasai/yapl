if [ ! -d build ]; then
	mkdir build
	cd build/
	if [ ! -d src ]; then
		mkdir src
	fi
	cd ../
fi

echo "> Compiling the source of YAPL:"

g++ -Wall -g -c ./src/token.cpp -o ./build/src/token.o
echo "	Generated ./build/src/token.o"

g++ -Wall -g -c ./src/lexer.cpp -o ./build/src/lexer.o
echo "	Generated ./build/src/lexer.o"

g++ -Wall -g -c ./src/parser.cpp -o ./build/src/parser.o
echo "	Generated ./build/src/parser.o"

g++ -Wall -g -c main.cpp -o ./build/main.o
echo "	Generated ./build/src/main.o"

g++ -Wall -g -o yapl ./build/main.o ./build/src/parser.o ./build/src/lexer.o ./build/src/token.o
echo "	Generated yapl"
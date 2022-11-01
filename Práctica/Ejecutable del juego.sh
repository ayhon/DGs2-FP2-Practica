#!/bin/bash
EXECUTABLE="masterminer"
DIR="$(pwd)"

case $OSTYPE in
	darwin) COMPILER="clang++ -std=c++14";;
	linux-gnu) COMPILER="g++";;
	*) COMPILER="clang++ -std=c++14";;
esac

[ -e "$EXECUTABLE" ] || $COMPILER -o masterminer *.cpp

case $OSTYPE in
	darwin)
	open -a Terminal "$DIR/$EXECUTABLE";;

	linux-gnu)
	alacritty -e "$DIR/$EXECUTABLE" --working-directory "$DIR";;

	*)
	open -a Terminal "$DIR/$EXECUTABLE";;
esac

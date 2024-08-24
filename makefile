OBJECTS := utils.o quadratic.o quadratic_tests.o handle_flags.o colors.o custom_assert.o
SOURCE := ./src/utils.cpp ./src/quadratic.cpp ./src/quadratic_tests.cpp ./src/handle_flags.cpp ./src/colors.cpp ./src/custom_assert.cpp
FLAGS := -I include -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

#TODO create build folder
#TODO(optional) specify only sources (not object files)
#TODO(optional) specify src dir
a.exe: main.cpp ${OBJECTS}
	g++ main.cpp ${OBJECTS} ${FLAGS} -o quadratic.exe
	make clear
${OBJECTS}: ${SOURCE}
	g++ ${SOURCE} -c ${FLAGS}
${SOURCE}:

clear:
	del *.o
clean:
	del quadratic.exe



OBJECTS:=utils.o quadratic.o quadratic_tests.o handle_flags.o colors.o custom_assert.o
FLAGS:=-I include -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE
SRCDIR:=src\\
BINDIR:=bin\\
EXENAME:=quadratic.exe

all: ${EXENAME}

${EXENAME}:	${OBJECTS}
	g++ main.cpp $(addprefix ${BINDIR},${OBJECTS}) ${FLAGS} -o ${EXENAME}
${OBJECTS}:
	g++ -c $(addprefix ${SRCDIR},$(patsubst %.o,%.cpp,$@)) ${FLAGS} -o $(addprefix ${BINDIR},$@)
clean:
	del quadratic.exe
	$(foreach OBJ,${OBJECTS},$(shell del $(addprefix ${BINDIR},${OBJ})))


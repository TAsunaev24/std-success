CFLAGS := -O2

all: labtest pathtest
labtest:
	g++ ../backend/field.cpp ../backend/utils.cpp labtest.cpp ${CFLAGS} -o labtest
pathtest:
	g++ ../backend/field.cpp ../backend/utils.cpp pathtest.cpp ${CFLAGS} -o pathtest
clean:
	rm -f pathtest labtest

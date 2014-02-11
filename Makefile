#  Executables
EX=hw2#ex01
#  Libraries - Linux
#LIBS=-lglut -lGLU
#  Libraries - OSX
LIBS=-framework GLUT -framework OpenGL
#  Libraries - MinGW
#LIBS=-lglut32cu -lglu32 -lopengl32

#  Main target
all: $(EX)

#  Generic compile rules
.c.o:
	gcc -c -O3 -Wall $<
.cpp.o:
	g++ -c -O3 -Wall $<

#  Generic compile and link
%: %.c CSCIx239.a
	gcc -Wall -O3 -o $@ $^ $(LIBS)

#  Delete
clean:
	rm -f $(EX) *.o *.a

#  Create archive (include glWindowPos if you need it)
CSCIx239.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o fps.o elapsed.o shader.o noise.o
	ar -rcs CSCIx239.a $^

#  C++ Example
hw2:hw2.o Object.o Sphere.o Cube.o WaveOBJ.o CSCIx239.a
	g++ -Wall -O3 -o $@ $^ $(LIBS)
hw2.o:    hw2.cpp    Object.h  Cube.h Sphere.h WaveOBJ.h
ex01:ex01.o Object.o Sphere.o Cube.o WaveOBJ.o CSCIx239.a
	g++ -Wall -O3 -o $@ $^ $(LIBS)
ex01.o:    ex01.cpp    Object.h  Cube.h Sphere.h WaveOBJ.h
Object.o:  Object.cpp  Object.h
Cube.o:    Cube.cpp    Cube.h    Object.h
Sphere.o:  Sphere.cpp  Sphere.h  Object.h
WaveOBJ.o: WaveOBJ.cpp WaveOBJ.h Object.h Vec.h

#  OpenCV
ex12: ex12.o CSCIx239.a; gcc -Wall -O3 -o $@ $^ -lopencv_highgui $(LIBS)
#  OpenMP
ex21:ex21.c CSCIx239.a;  gcc -O -fopenmp -Wall -o $@ $^ $(LIBS)
#  CUDA
ex23:ex23.cu;            nvcc -o $@ $^
#  OpenCL
ex24:ex24.cpp;           g++ -Wall -o $@ -I/usr/local/cuda/include $^ -lOpenCL

#  Nbody OpenMP and OpenCL
InitCL.o:InitCL.cpp;g++ -O -Wall -c -I/usr/local/cuda/include $^
ex25:ex25.cpp InitCL.o CSCIx239.a;g++ -O -fopenmp -Wall -o $@ -I/usr/local/cuda/include $^ -lOpenCL $(LIBS)
#  Ray traced spheres
ex26: ex26.cpp RaySphere.cpp;g++ -fopenmp -Wall -O3 -o $@ $^ $(LIBS)
#  Mandelbulb CUDA
ex27:ex27.cu CSCIx239.a; nvcc -Xcompiler -fopenmp -o $@ $^ $(LIBS)

#  ZIP files
zip:
	zipc ex01.zip ex01.cpp Object.cpp Sphere.cpp Cube.cpp WaveOBJ.cpp
	zipc ex03
	zipc ex04
	zipc ex05
	zipc ex06
	zipc ex10
	zipc ex11
	zipc ex12
	zipc ex13
	zipc ex14
	zipc ex15
	zipc ex16
	zipc ex17
	zipc ex18
	zipc ex19 ex19.c vacuum-cleaner.rle
	zipc ex20
	zipc ex21
	zipc ex22
	zipc ex23.zip ex23.cu
	zipc ex24.zip ex24.cpp
	zipc ex25.zip ex25.cpp InitCL.cpp
	zipc ex26.zip ex26.cpp RaySphere.cpp
	zipc ex27.zip ex27.cu
	zipc CSCIx239.zip CSCIx239.h fatal.c loadtexbmp.c print.c project.c errcheck.c object.c fps.c elapsed.c shader.c noise.c Makefile

#  Obligatory UNIX inside joke
love:
	@echo "not war?"

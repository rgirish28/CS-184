CC = g++
CFLAGS = -Wall -O3 -I./
LDFLAGS = -L./ -lm -lfreeimage

Film.o : Film.cpp raytracer.h
	${CC} ${CFLAGS} -c Film.cpp
	
Sphere.o : Sphere.cpp raytracer.h
	${CC} ${CFLAGS} -c Sphere.cpp
	
Triangle.o : Triangle.cpp raytracer.h
	${CC} ${CFLAGS} -c Triangle.cpp

Sampler.o : Sampler.cpp raytracer.h
	${CC} ${CFLAGS} -c Sampler.cpp
		
Ray.o : Ray.cpp raytracer.h
	${CC} ${CFLAGS} -c Ray.cpp
			
Camera.o : Camera.cpp raytracer.h
	${CC} ${CFLAGS} -c Camera.cpp
		
Raytracer.o : Raytracer.cpp raytracer.h
	${CC} ${CFLAGS} -c Raytracer.cpp

Scene.o : Scene.cpp raytracer.h
	${CC} ${CFLAGS} -c Scene.cpp

main.o : main.cpp raytracer.h
	${CC} ${CFLAGS} -c main.cpp

raytracer : main.o Film.o Raytracer.o Scene.o Sphere.o Sampler.o Ray.o Camera.o Triangle.o
	${CC} ${CFLAGS} main.o Sampler.o Raytracer.o Sphere.o Film.o Ray.o Camera.o Scene.o Triangle.o ${LDFLAGS} -o raytracer
	
clean :
	rm -rf *o *~ raytracer
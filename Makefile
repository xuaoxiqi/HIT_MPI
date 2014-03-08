CC = mpic++
NVCC = nvcc
LD = mpic++
LIBS = -lcudart -lcufft -lcublas -lcuda  -lstdc++ -lm -lhdf5  -lhdf5 -lhdf5_hl
PATHS = -L/opt/cuda/lib64/ -L/usr/lib64 -L/usr/lib
INCLUDES = -I/opt/cuda/include
DEBUG = -g
GPU_SOURCES = $(wildcard src/*.cu)
CPU_SOURCES = $(wildcard src/*.c)
GPU_OBJECTS = $(GPU_SOURCES:.cu=.o)
CPU_OBJECTS = $(CPU_SOURCES:.c=.o)


all: $(GPU_OBJECTS) $(CPU_OBJECTS)
	mv src/*.o .
	$(LD) $(DEBUG) -o hitMPI *.o $(PATHS) $(LIBS)

$(CPU_OBJECTS): src/%.o: src/%.c
	$(CC) -c $(INCLUDES) $< -o $@

$(GPU_OBJECTS): src/%.o: src/%.cu
	$(NVCC) -c $(INCLUDES) $< -o $@

clean:
	rm *.o hitMPI

run:
	gcc main.c -o app.out -lraylib -lm -lpthread -ldl -lrt -lX11 && ./app.out
run-cuda:
	nvcc main_cuda.cu -o app.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 && ./app.out
test:
	gcc test.c -o test.out && ./test.out

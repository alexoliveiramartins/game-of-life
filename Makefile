run:
	gcc main.c -o app.out -lraylib -lm -lpthread -ldl -lrt -lX11 && ./app.out
test:
	gcc test.c -o test.out && ./test.out

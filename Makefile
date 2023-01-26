huffman: build_dir huffman_string.o huffman_node.o huffman_count.o
	cc src/main.c build/huffman_string.o build/huffman_node.o build/huffman_count.o -o huffman -I ./src

build_dir:
	mkdir -p build

huffman_string.o: build_dir
	cc -c src/huffman_string.c -o build/huffman_string.o -I ./src

huffman_node.o: build_dir
	cc -c src/huffman_node.c -o build/huffman_node.o -I ./src

huffman_count.o: build_dir
	cc -c src/huffman_count.c -o build/huffman_count.o -I ./src

clean:
	rm -rf build
	rm -f huffman
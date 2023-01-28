# huffman
 Simple experimental huffman coding compression tool

 ## Building
 The project is quite small and builds from the root directory with make.
 ```
$ make
```

 ## Usage
 The executable does two thing, compresses files and uncompresses them. Files 
 are compressed using the `-c` option and uncompressed using the `-x` option.

 Copression:
 ```
 $ ./huffman -c Shakspeare.txt Shakspeare.huff
 ```

 Extraction
 ```
 $ ./huffman -x Shakspear.huff Shakspear.txt
 ```

 ## Performance
 Since this is a simple huffman coding implimentation, it retains the typical
 huffman compression ratio of 1.5:1. For reference, The Complete Works of William 
 Shakespeare take up aproximatly 5.5 Mb unncompresssed. This algorithm compresses
 the file to 3.2 Mb, and zip will compress it to 2 Mb.

 ## File Format
 The compressed file is held in a custom file format that holds the huffman 
 tree followed by the encoded data. The tree is recusivly built, starting at 
 the root node with intermidiate notes holding the value `29` in decimal, and
 including the null nodes as `0` in decimal. The leaf nodes hold their 
 corresponding character.

 ## Pilospohy in Lit
 This project was made in conjunction with a Philosphy class on the nature of 
 perfection. While huffman coding technically is perfectly compressed at the 
 byte level, this algorithm does noticeably worse than zip since it does not 
 look for duplicate groupings.

 *Note to the teacher:* Under the assumption that you are unfamiliar with 
 programming, how to build this project, or how to run it, this won't mean 
 much to you, and quite frankly it isn't all that exciting to anyone but me. 
 For some reference, I took pains to make this program as efficent as possible,
 writting it in pure ANSI C without the help of any external libraries. It 
 took not insignificant effort with much complexity underneath despite the 
 seemingly simple result. With that being said, there remain sever known breaking
 flaws and obvious features that remain unaddressed due to time constraints. 

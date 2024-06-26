# 32-bit RISC-V Assembler 

A simple assembler

* Only supports assembly to binary
* Does not support labels
* Only supports uppercase instructions with lowercases register
* Does not support psuedo instructions
* Does not support register aliases

## To-do List

* Remove case sensitivity
* Add support for register aliases and psuedo instructions
* Add support for label decoding
* Add support for binary to assembly
* Add support for hexadecimal values, both ways
* Refactor, clean up and improve code
* Add an UI


## Program Data Flow

```mermaid

graph TD

    start(Start)
    file(Request file name)
    error(error)
    exit(Exit program)
    openFile(Open file)
    readLines(read and store
    the lines in the file)
    newFile(Create destination file)


        analyzeFile{Is there 
        an instruction to 
        assemble?}
        split(split instruction
        and add to
        temporary array)
        format(check format)
        function(call correct 
        assembler function)
        assembledFile(write assembled
        instruction to file)

    start --> file

    file -- File or filepath 
    does not exist --> error 

    file --> openFile
    error --> exit
    openFile --> newFile
    newFile --> readLines
    readLines --> analyzeFile
    analyzeFile -- Yes --> split
    analyzeFile -- No --> exit
    analyzeFile -- Incorrect code/syntax --> error
    split --> format
    format --> function
    function --> assembledFile
    assembledFile --> analyzeFile
```


## How to use

* Compile the program from the Source Code
* Create a .txt a file and fill it with instructions in the style as shown below

*remember that this program does not support labels yet, and you will have to calculate the offsets yourself at the time being*

* Run the program
* Assign file
* profit?

```MIPS
ADD x4, x6, x8
SW x4, 24, x28
ADDI x5, x5, 345
```

Get's turned into

```
00000000100000110000001000110011
00000000010011100010110000100011
00010101100100101000001010010011
```

After the program is done, you should have a file that is called *yourfilenamehere*_assembled.txt with your encoded instructions.
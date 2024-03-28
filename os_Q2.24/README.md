## Environment

- **Ubuntu 20.04.5 LTS**
- kernel version :**5.15.0-101-generic**

## Steps to run the program

- Open the terminal and navigate to the directory where the file **main.c**.
- To run

1.

```
gcc -o a main.c
./a
```

2. enter the name of input file(**sample.txt**)
3. enter the of output file(**output.txt**)

## command to create a traces system calls file

```
strace -o trace_output.txt ./a
```

## Screenshots

![img](2.24.PNG)

![img](2.24-1.png)

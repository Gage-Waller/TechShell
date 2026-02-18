# TechShell - Simple Unix Shell (CSC 222 SYSTEMS PROGRAMMING)

## Group Members
- Gage Waller

## Responsibilities

Gage Waller:
- Implemented command parsing logic
- Implemented I/O redirection
- Error handling using errno and strerror
- Implemented fork and execvp exec
- Implemented built in commands like cd and exit
- Implemented prompt format using getcwd

## Program Description

TechShell is a simple Unix shell written in C. The program repeatedly asks the user for input,
parses command line, and executes commands using fork and execvp.

The shell supports:
- Display the current working directory in the prompt
- Built in commands:
    - cd (change dir) Ex. cd Desktop
    - exit (exit the shell)
- Execution of external programs
- Input redirection using <
- Output redirection using >
- Error handling using errno

The shell updates the prompt when the working directory changes.

## How to clone repository

```bash
git clone https://github.com/Gage-Waller/TechShell.git
cd techshell
```

## How to compile

```bash
gcc techshell.c -o techshell
```

## How to run
```bash
./techshell
```

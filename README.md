# The Simple Shell
This simple shell is designed for the following:
- alternate inputs
    * read from stdin or a command file
- change directory
- exit shell
- basic input
    * similar to bash's read command that pushes input into env variables
- env variable substitution
- quoted characters
    * mechanism to unspecialize certain characters
- nested if statements
    * allows for flow control

## Compile
```console
make build && ./smsh
```
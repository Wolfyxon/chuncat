# Chuncat
File splitting utility

## Usage
```
chuncat <command> [<args>]...
```
### Splitting
#### Desired amount
```
chuncat split count <count> <file path>
```
##### Example:
```
chuncat split count 3 someFile.txt
```
```
someFile.txt.0
someFile.txt.1
someFile.txt.2
```
#### Per amount of bytes
```
chucat split bytes <count> <file path>
```
##### Example:
```
chuncat split bytes 2 someFile.txt
```
And the program will produce multiple files with 2 characters in them.

### Joining
TODO

## Compiling
Nothing complicated, just
```
gcc main.c
```

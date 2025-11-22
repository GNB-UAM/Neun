# How to parse from tex equations

## Prerequisites

You may need to install `flex` and `bison` in your system.
```
sudo apt install flex  bison
```


## Compilation and execution

1. Build parser by running
    ```
    make
    ```

2. Include the equations tex file equations.tex
3. Now run:
    ```
    ./parser < tests/equations.tex
    ```

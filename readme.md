
## Compilation

To compile the program, use the following command:
```sh
make
make clean
```

## Running the Program

To run the program, use the following command:

```sh
./motto datasets threads_flag threads delta omega
```
### Parameters:
datasets: The path to the datasets file.
threads_flag: Flag to enable or disable threading (1 to enable, 0 to disable).
threads: Number of threads to use.
delta: Delta value for the analysis.
omega: Omega value for the analysis.


### example :

```sh
./motto datasets.txt 1 32 3600 4
```
This command runs the program with the datasets.txt file, enables threading with 32 threads, and sets delta to 3600 and omega to 4.


### output :

The output results are represented using a 6$\times$6 matrix to display the counts of all temporal motif instances. In the provided 6$\times$6 matrix, the first four columns represent the counts of all Star temporal motif instances. In the last two columns, the first four rows indicate the counts of Triangle temporal motif instances, and the last two rows represent the counts of Pair temporal motif instances.







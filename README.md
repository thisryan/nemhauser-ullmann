# nemhauser-ullman

An implementation of the nemhauser-ullman algorithm for solving the knapsack problem

## datasets

- [instances_01_KP](http://artemisa.unicauca.edu.co/~johnyortega/instances_01_KP/)

## Usage

```c
make
./build/algorith_demo <path-to-file>
```

## Times

cpu: AMD® Ryzen 7 5700u

|datasets| time |
| --- | --- |
| knapPI_1_100_1000_1 | 1,9 ms|
| knapPI_1_200_1000_1 | 11,6 ms |
|knapPI_1_500_1000_1| 5,38 ms|
| knapPI_1_1000_1000_1 | 19,68 ms|
|knapPI_1_2000_1000_1 | 183,74 ms |
|knapPI_1_5000_1000_1| 2,93 secs |
|knapPI_1_10000_1000_1| 19,93 secs |
| knapPI_2_100_1000_1| 11,63 ms|
|knapPI_2_200_1000_1| 3,48 ms|
|knapPI_2_500_1000_1| 15,65 ms|
|knapPI_2_1000_1000_1| 37,04 ms|
|knapPI_2_2000_1000_1| 277,70 ms|
|knapPI_2_5000_1000_1| 10,22 secs |
|knapPI_2_10000_1000_1| 120,99 secs |
|knapPI_3_100_1000_1| 10,38 ms|
|knapPI_3_200_1000_1| 45,22 ms|
|knapPI_3_500_1000_1| 32,60 secs|
|knapPI_3_1000_1000_1| n.a |
|knapPI_3_2000_1000_1| n.a |
|knapPI_3_5000_1000_1| n.a |
|knapPI_3_10000_1000_1| n.a |
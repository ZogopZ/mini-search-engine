# mini-search-engine
A multi process application that simulates a mini search engine.

# Makefiles - How to compile
Open a terminal in the parent directory and type:
```sh
$ make
```
This will call all three makefiles in directories root, splitNmerge and leaves.

You can also use:
```sh
$ make clean
```
This removes all executable files, all object files and two aiding files 
(splitter, results) from directories root, splitNmerge and leaves.

# How to run
**The application must be executed from within root directory** and the flags 
can be given in any order. Record files Records100.bin, …, Records1000000.bin 
are zipped in file data.tar.gz in the parent directory.

To do some test runs of the application place a record file in root directory 
then open a terminal in the same directory and type:
```sh
$ ./myfind -h 5 -d Records1000.bin -p town -s
$ ./myfind -d Records10000.bin -h 3 -p more
$ ./myfind -p oy -s -h 2 -d Records100.bin
```

# Root Node
The root node is the anchor of the binary tree and is responsible for the 
following tasks:
1.	Checks given arguments from the tty and presence of required flags. 
If the tree’s `height` is an invalid value or the number of the arguments 
is wrong, it produces an error message.
2.	Creates splitter file in directory root. This file contains the records 
that each leaf node will be assigned. Furthermore, it is passed as an argument 
to all splitter/merger nodes and to leaf nodes. Depending on the presence of 
the `-s` flag all records are properly distributed. If there are any leftover 
records, each leaf is assigned one more until there aren’t any left.
3.	Creates the first splitter/merger node and a named pipe to communicate with. 
At this point, root node `blocks` waiting for something to read from the given 
named pipe. When the above mentioned splitter/merger nodes write the results in 
the named pipe, the root node will `unblock` and continue its execution.
4.	Writes the results read from the named pipe in the results file in 
directory root.
5.	Waits for statistics of the binary tree and for a `SIGUSR2` signal from each 
leaf node. When all `SIGUSR2` signals are handled, the root node has all the 
results it needs.
6.	Creates a new `sort` process and forwards all results to it.
7.	Outputs in a user-friendly way all binary tree’s statistics including 
itself (the root node).
8.	Gradually frees any allocated memory in a controlled way.

# Splitter/Merger Node
Processes 'living between' the root node and leaf nodes are the named 
splitter/merger nodes and each one of them is responsible for the following tasks:
1.	Check arguments passed to it by `execl()` and produce error messages in 
case of wrong number of arguments.
2.	Depending on the height argument create either two new splitter/merger 
nodes (`if height > 1`) or two leaf nodes (`if height == 1`) and also create 
the required named pipes for inter-process communication. Then `block` until 
their children write back the results into the named pipes.
3.	After they `unblock` (namely their children returned results using the 
named pipes), each splitter/merger node forwards back to its parent the results 
it received and repeats this task until their children terminate.
4.	The named pipes created to serve the communication between splitter/merger 
nodes and their children are closed by the parent splitter/merger nodes.
5.	Lastly, each splitter/merger node writes back to its parent its statistics 
and terminates, gradually freeing any allocated memory in a controlled way.

# Leaf Node
The leaf nodes are the leaves of the binary tree and each one of them is 
responsible for the following tasks:
1.	Check arguments passed to it by `execl()` and produce error messages in case 
of wrong number of arguments.
2.	Open splitter file in directory root and find out which range of records to 
work on.
3.	Open the binary file received from the arguments, read the assigned records, 
and place them in a buffer.
4.	Search the user-defined `substring` argument (passed all the way from root 
node to splitter/merger nodes to leaf node) in each assigned record. If the 
`substring` is found within a record, then return the record to its parent. The 
Naive algorithm is used to search a substring into a string and has a complexity 
of O(n*m).
5.	After the leaf node completes its search task it writes back to its parent 
the time it took to complete the given task and proceeds to terminate by 
gradually freeing any allocated memory in a controlled way. Then sends a 
`SIGUSR2` signal to the root node.

# Sort Node
The sort node is a child process of root nodes and is responsible for the 
following tasks:
1.	Receive from root node all the records that contain the user-defined 
`substring` (namely the results file) and forward them to the command-line 
utility `sort`.
2.	Using the command’s `-g` flag the results are sorted by the numeric value of 
the first column and then printed to the tty.

# Valgrind – Check
The application was checked for memory leaks using the tool **Valgrind**. Leaks
 found where caused by the `sort` command and **not** by user allocated memory.
To run the application using Valgrind place a record file in root directory 
then open a terminal in the same directory and type:
```sh
$ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes –trace 
children=yes ./myfind -h 5 -d Records1000000.bin -p more -s
```

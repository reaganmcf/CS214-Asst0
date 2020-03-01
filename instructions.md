## Abstract

Often computing involves IO directly at some point. This is in essence a given unless the processor is generating its own values to compute on. Algorithms for processing data are good to know, and their implementation is useful, however in nearly any case where you need to compute on some data you will also need to read it from or write it to somewhere.

	
	
## Introduction
You will write some code to read in data from a file, sort it and output the sorted values. In this case while the representation will be the same for all files, the data may be different. This is to say all files will be ASCII character files, however some will hold numeric data and some will hold strings. Your code will need to decide which type of information it is dealing with and select an appropriately-typed sorting function for the task. Keep in mind that the sorting function is not the same as the sorthing algorithm - one is an implementation, the other is a theoretical ordering of operations and partitioning of information. The user is allowed to select which sorting algorithm to use at runtime, so you will need to implement as the first parameter to your code a flag in order to select which sorting algorithm's implementation (function) your code should apply.
	
Your code must also take in as yoru second command line parameter a single file name, open it, read in all tokens from it and determine whether the data is representing integers or strings. You are guaranteed that the tokens will be ASCII character data and that they all will be comma-separated. You should however ignore all other separations (newlines, spaces, tabs, etc.).
	
Once all data has been sorted, your code should output the entire sorted contents to STDOUT with each value on its own line in increasing order (you might find it beneficial then to sort tokens in the same manner).
	

	
## Methodology
- Comparator
	- In order to implement this behavior you will first need to write two sorts; Insertion Sort and Quick Sort. They may not use any built-in sorting functions. They need to be type-agnositic, as they might need to sort either numeric or string data. The prototype for your sorts must be:
	
			int insertionSort( void* toSort, int (*comparator)(void*, void*)   )
			
			int quickSort( void* toSort, int (*comparator)(void*, void*) )
			
	- Both require a comparator function that is passed in as a function pointer that you will have to write as well. The comparator takes two things of any type and, based on the version of the comparator function, should typecast its arguments and compare them in a way that makes sense for the kind of information they represent. You will need to write a comparator for numeric data and a comparator for string data that can be used by either your insertion sort or your quick sort. Please have your quick sort select the first element of any list it is to sort as the pivot.
		
	- The user should select the sort they want with either '-i' for Insertion Sort or '-q' for Quick Sort as the first command line parameter. If there is no sort selected, that is a fatal error. The first command line parameter should be the sort selection flag, and the second the file name containing data to sort (see Example below).
		
	- (Warning!: You may not use built-in sorting functions! Be careful, strcmp() is off the table, however, things that allow you to classify characters as either alphabetic or numeric are okay (because they aren't sorting))
		
	- (Note: You may notice that the prototypes for both functions are the same, so it ought to be possible to make them one function with a parameter to select the sort type. Please do not do so, but use the prototypes as stated above.)
		
	- (Note: You are dealing with a user. Users are unfortuately humans, as humans are sadly quite buggy (re: fallible). Be sure to check for error in all cases. Do not expect that all inputs wil be correctly formatted, or present. )
	
	
- Error:
	- You have three general levels of error:
		1. Warnings:
			- These are things that seem like they may be incorrect or might result in unexpected results that you ought to print out a message about, however they aren't a reason to stop running. If the file given is empty, for instance, you should print out a warning.
			
		2. Errors:
		 	- these are things that go wrong that you can fix/react to. You should print out a message and let the user know they happened, but you don't have to stop running because you have a way to deal with it. If a malloc call returns NULL, for instance, you should print out an error.
		 	- (Note: errors are UNexpected - so do not declare an error if read() comes back with less than the requested bytes, that is expected (and normal) operation)
		 
		3. Fatal Errors:
		  	- these are things that go wrong that you can not fix/react to. You should definitely print out a message and stop running because you have no way to proceed. If the filename you are given does not exist, for instance, there is no way for you to perform your function, so you will need to let the user know and stop running.
		 	- (Note: this includes the user being a human, i.e. making mistakes. If they do not select a sort by specifying a command line parameter, that is a fatal error since you can not fix the issue or react appropriately)
		 	- (Note: stopping running doesn't mean you get to not play by the rules - make sure you still deallocate all dynamically-allocated memory and close() any open() file descriptors)
	
	
- C. File read

As discussed in lecture, IO is often fairly slow, and file IO is almost the slowest thing there is. You may well see non-blocking IO effects while reading from the file. You also will not know a-priori how long the file is, how long the individual strings or integers are, or how many there will be. Do not use any of the f- commands (fopen, fread, fwrite, etc.). Only the basic file desciptor-based commands are legal (open, close, read, write).
	
	Be sure that you read in your data carefully:
		- check for EOF (end of file) as you go
		- make sure you handle nonblocking-reads
		- make sure you find/drop spaces, tabs, newlines and other breaks in the token sequences
		- make sure you can read in your tokens with no presumption on token length
		- make sure you can read in your tokens with no presumption on total number of tokens

	(Note: Given there are a number of aspects that you do not know at compile time, you will need to do a lot of memory allocation at runtime. Be sure you free() any and all dynamically-allocated memory before your code ends.)

	
	
## Requirements
	- You must submit the following:
	  	- a compressed file, Asst0.tar.gz, containing:	
		- fileSort.c
			source code with your main() inplementing the above behavior
		- testplan.txt
			a document detailing how you tested your code - what tests did you do and, in a sentence (or two), what were you testing for
		- readme.pdf
			a document meant for a user detailing how your code operates and how to use it - like the man pages

		Make sure your code compiles in the following fashion:
			gcc fileSort.c
			
		(Note: You are not allowed to make use of any gcc flags: not fsanitize, not C99, etc.)
		(Note: Do not package your code with a Makefile - your Makefile will not be run)

		
		
## Conclusion
	- You can segment this work in to a series of modules, which in the end may ease the developement path considerably. You can break the code in to the following functional elements:
		- read from a file
			- open it (and error states)
			- read in one char, up to a comma, multiple comma-separated tokens, multiple tokens with interspersed noise (spaces, newlines..)
			- read on char at a time (optionally, read in n chars and extract tokens from the buffer)
		- sorts
			- code linked lists
			- code insertion sort first (it is simple)
			- write sorts on linked lists of numbers
			- duplicate code and change type/functions to work on linked lists of numbers
		- function pointers
			- write an integer and a string comparator
			- generalize the input types to be void*, but cast them inside the function
			- copy/paste sort code and modify it to accept your comparators
			
		Each of these modules can be written - and tested - in isolation 

		(Note: Keep in mind the way you output the tokens will depend on what they represent - don't forget to select the printf() format code appropriately at the end.)
		
		(Note: Keep in mind you should free() anything you malloc(), and close() anything you open())

	

Example: (Addendum 0)

	> ./fileSort -i ./somefile
	 If "./somefile" contains: hi,there, every,     one
	 output should be:
	 every
	 hi
	 one
     there


	> ./fileSort -q ./somefile
	 If "./somefile" contains: hi,there, every,     one
	 output should be:
	 every
	 hi
	 one
     there
	
	
	> ./fileSort ./somefile
	 If "./somefile" contains: hi,there, every,     one
	 output should be:
	 Fatal Error: expected two arguments, had one
	  
	 
	> ./fileSort ./somefile -q
	 If "./somefile" contains: hi,there, every,     one
	 output should be:
	 Fatal Error: "./somefile" is not a valid sort flag 
	       - or -
     Fatal Error: file "-q" does not exist
	 
	 
	 > ./fileSort -q ./someotherfile
	 If "./someotherfile" contains: 123, 14, 510     , 1
	 output should be:
	 1
	 14
	 123
	 510

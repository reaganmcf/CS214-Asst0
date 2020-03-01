# ASST0

### How does it work?

When running the program, we first check if the valid permutation of flags and filename is entered. If not, we throw an error to the user and exit. For more information and examples of valid and invalid argument and filename input order, please check `testplan.txt`.

Our program works by opening the file, and dynamically creating a Linked List that will store all of the contents of the file where each node is a valid token inbetween `,`. To do this, we do the following:

1. Create a temporary buffer
2. Read character by character, building up our temporary buffer
3. When we reach a comma, cast the temporary buffer to a `void*` in the end of our Linked List's `data` property.
4. Continue until `EOF`

Now that we have a built Linked List that contains all of our data, we need to sort it. Depending on the input flag the user has specified `-i = Insertion Sort` or `-q = Quick Sort`, we cast our `Node*` to a `void*` in order to make it type agnostic, and send it to either `quickSort()` or `insertionSort()`.

Each of these functions will perform the appropriate sorting algorithm, making sure to properly use the relevant `comparator()` function pointer when two Node's of the Linked List need to be compared. Inside each of these, we check if the `void*` contains a valid `char*` by looping over the length of the casted value if all contents pass the `isalpha()` test. If not, we will treat the contents as `int*`.

After the LinkedList has been properly sorted, we pass it over to `printLL()` which will print the contents of the Linked List checking for the proper `printf() syntax flag` as well as `' '` data edge cases.

After we are finished with `printLL()`, we go ahead and `free()` any allocated memory and exit the program
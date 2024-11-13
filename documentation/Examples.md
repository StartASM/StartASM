# Examples
The following are some basic examples that demonstrate core functionality in StartASM as well as some basic use cases. All of these examples can be found in the `examples` folder.


## Example 1 - Interactive Age Checker
The above program demonstrates numerous features of StartASM, including I/O, initializing values, arithmetic operations, and conditional jumps.
```
comment "Let's set a constant 21 and ask the user for their age"
create integer 21 to r1
print "How long till you're 21? Let's find out!"
print newline
print "Enter your age: "
input integer to r2
print newline
compare r2 with r1
jump if equal to 'is21'
jump if greater to 'past21'
jump if less to 'not21'
stop

comment "Case - if the user is exactly 21"
label 'is21'
print "You're just barely 21! Yay!"
stop

comment "Case - if the user is past 21, we'll print their age back to them"
label 'past21'
print "Wow, you're "
output r2
print " and you're using this calculator? Scram!"
stop
 
comment "Case - if the user is yet to be 21, we'll output the number of years left"
label 'not21'
sub r1 with r2 to r3
print " *sigh* you still have "
output r3
print " years to go till you're 21."
stop
 ```


## Example 2 - String Reader
Here is a separate example showing a way to input a string given individual character inputs. It terminates once the ~ is detected:
```
comment "First let's get the input string by looping until ~ is detected"
comment "First create three memory addresses - our start index, our current index and our increment amount"
create memory m<0> to r0
create memory m<0> to r1
create memory m<1> to r2
comment "Next create our terminating character"
create character ~ to r3

comment "Prompt the user for input"
print "Enter your string until you terminate with the ~ key"
print newline

comment "Loop the input until we detect the ~ key"
label 'inputLoop'
input character to r4
compare r4 with r3
jump if equal to 'outputLoop'
store r4 to r1
add r1 with r2 to r1
jump if unconditional to 'inputLoop'

comment "Now let's print the string back to them starting from our start index until it hit's our 'current' (or end) index"
label 'outputLoop'
load r0 to r4
output r4
add r0 with r2 to r0
compare r0 with r1
jump if greater to 'terminateProgram'
jump if unconditional to 'outputLoop'
 
label 'terminateProgram'
stop
```

The compiler will also return syntax and symbol/scope error messages that show the excepted line, token, and what the compiler expected, if applicable. For example, submitting this invalid code:
``` 
shift right r2 with r1
junk r1
move m<1> to r6
```

Will return the following errors:
```
Invalid syntax at line 1: 'shift right r2 with r1'
Unknown conjunction 'with'. Expected 'by'
 
Invalid syntax at line 2: 'junk r1'
Unknown instruction 'junk'

Invalid syntax at line 3: 'move m<1> to r6'
Unknown source 'm<1>'. Expected register r0-r9
```


## Example 3 - Pointer Demonstration
This program demonstrates some basic abilities with StartASM and handling pointers and memory addressing:
```
comment "Pointer code is to test some basic pointer operations in StartASM"

comment "First let's get the input string by looping until ~ is detected"
comment "First create three memory addresses - our start index, our current index and our increment amount"
create memory m<0> to r0
create memory m<0> to r1
create memory m<1> to r2
comment "Next create our terminating character"
create character ~ to r3

comment "Prompt the user for input"
print "Enter your string until you terminate with the ~ key"
print newline

comment "Loop the input until we detect the ~ key"
label 'inputLoop'
input character to r4
compare r4 with r3
jump if equal to 'outputLoop'
store r4 to r1
add r1 with r2 to r1
jump if unconditional to 'inputLoop'

comment "Now let's print the string back to them starting from our start index until it hit's our 'current' (or end) index"
label 'outputLoop'
load r0 to r4
output r4
add r0 with r2 to r0
compare r0 with r1
jump if greater to 'terminateProgram'
jump if unconditional to 'outputLoop'

label 'terminateProgram'
stop
```


## Example 4 - Math Calculator
The following program combines all the features showcased from examples 1-3 to create a basic math calculator, including string input parsing and arithmetic operations:
```
comment "Math calculator is an example to demonstrate practical functionality"

comment "Start by creating memory addresses for parsing the input string"
create memory m<0> to r0
create memory m<0> to r1
create memory m<1> to r2
create character = to r3

comment "Prompt the user for input"
print "Enter an expression (e.g., 5+3=): "
print newline

comment "Input loop for the string until the = character is detected"
label 'inputLoop'
input character to r4
compare r4 with r3
jump if equal to 'parseExpression'
store r4 to r1
add r1 with r2 to r1
jump if unconditional to 'inputLoop'

comment "Parse the expression - extract the numbers and the operation"
label 'parseExpression'
create integer 0 to r5
create integer 0 to r6
comment "create character nullvalue to r7"
create memory m<0> to r8
create boolean true to r9

label 'parseLoop'
load r8 to r4
compare r4 with r3
jump if equal to 'performCalculation'

compare r4 with +
jump if equal to 'storeOperation'
compare r4 with -
jump if equal to 'storeOperation'
compare r4 with *
jump if equal to 'storeOperation'
compare r4 with /
jump if equal to 'storeOperation'

sub r4 with 48 to r4
multiply r5 with 10 to r5
add r5 with r4 to r5
jump if unconditional to 'incrementParseIndex'

label 'storeOperation'
move r4 to r7
create integer 0 to r5
move false to r9

label 'incrementParseIndex'
add r8 with r2 to r8
jump if unconditional to 'parseLoop'

comment "Perform the calculation based on the operation"
label 'performCalculation'
compare r7 with +
jump if equal to 'addOperation'
compare r7 with -
jump if equal to 'subOperation'
compare r7 with *
jump if equal to 'mulOperation'
compare r7 with /
jump if equal to 'divOperation'

label 'addOperation'
add r5 with r6 to r5
jump if unconditional to 'outputResult'

label 'subOperation'
sub r5 with r6 to r5
jump if unconditional to 'outputResult'

label 'mulOperation'
multiply r5 with r6 to r5
jump if unconditional to 'outputResult'

label 'divOperation'
divide r5 with r6 to r5
jump if unconditional to 'outputResult'

comment "Output the result"
label 'outputResult'
print "Result: "
output r5
stop

```
 
# CPPLineCount
A test for standard file system library: Simple and reasonably accurate cpp source line counter.

Counts only physical source lines. No effort is made to determine logical lines of code. Empty lines, 
comment lines (lines starting with "//"), and block comment lines (lines between "/\*" and "*/") are 
counted separately and reported accordingly. 

## A known issue
Source lines that also contains a block comment in between the source (i.e. a block comment starting
and ending in the same line) is counted both source line and a block comment line. 

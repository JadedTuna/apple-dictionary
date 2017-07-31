# Apple Dictionary Tools
Tools for extracting data from Apple dictionary files (used by the Dictionary application).
**Note:** I have a bit more info in my recent blog post: [Progress on accessing data from Apple Dictionaries](https://jadedtuna.github.io/apple-dictionary/)

# Usage
General usage would be to first compile the code with `make` and then run

`./dedict path/to/Body.data | ./strip | ./checkxml.py > dictionary.xml`

or if you want to convert it to sqlite3 database directly

`./dedict path/to/Body.data | ./strip | ./checkxml.py | ./appledict2sqlite3.py`

which will create a file called `dictionary.db`. The sizes can vary, but on my
machine I got ~192MB for dictionary.xml and ~250MB dictionary.db. These tools have
been tested only with the New Oxford American Dictionary but they should work without
any problems with other dictionaries.

# Code
Some of the code has been taken from https://gist.github.com/josephg/5e134adf70760ee7e49d
and modified to fix errors and make it more useful.

#!/usr/bin/env python
#coding: utf-8
"""
Convert XML file created from an Apple Dictionary Base.body file
into an SQLite3 database.
"""
import os
import sys
import sqlite3
from xml.etree import ElementTree

# fetch name of the output file or use dictionary.db as default
output = None
if not sys.argv[1:]:
    output = "dictionary.db"
else:
    output = sys.argv[1]

# rename file if it exists already
if os.path.exists(output):
    import random
    new = output + str(int(10000 * random.random()))
    os.rename(output, new)
    sys.stderr.write("renamed {} to {}\n".format(output, new))

# create connection and the file
conn = sqlite3.connect(output)

# create the table
c = conn.cursor()
c.execute("""
    CREATE TABLE definitions (id text, title text, entry text)
""")

# read input line by line and insert the entires
linenum = 0
for line in sys.stdin:
    linenum += 1
    try:
        root = ElementTree.fromstring(line)
        # obtain the title
        id = root.get("id")
        for key in root.keys():
            if key.endswith("title"):
                title = root.get(key)
                break
        c.execute("INSERT INTO definitions VALUES (?, ?, ?)",
                  (id, title, memoryview(bytes(line,encoding="utf-8"))))
    except ElementTree.ParseError:
        # malformed XML
        sys.stderr.write("invalid XML on line {}\n".format(linenum))
        import traceback
        traceback.print_exc()
        break
    except KeyboardInterrupt:
        # Shutdown
        sys.stderr.write("shutdown requested\n")
        break

# save changes and close the connection
conn.commit()
conn.close()


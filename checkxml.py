#!/usr/bin/env python
#coding: utf-8
"""
The main idea of this file is to join together blocks of
XML data, as some entries are multiline and have to be
converted into a single-line entry before they can be
converted into a database format.
"""
import sys

buffer = None
# read input line by line and make sure it is valid XML
for line in sys.stdin:
    # remove whitespace on the right, if any
    line = line.rstrip()
    if buffer is not None:
        buffer += line
        if line.endswith("</d:entry>"):
            print(buffer)
            buffer = None
        continue

    if line.endswith("</d:entry>"):
        # the entry is on one line
        print(line)
    else:
        # only first part of a multiline entry
        buffer = line


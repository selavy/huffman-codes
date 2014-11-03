#!/usr/bin/env python

import sys
from optparse import OptionParser
import heapq

class Node():
    def __init__( self, data, left = None, right = None ):
        self._data = data
        self._left = left
        self._right = right
    def left( self ):
        return self._left
    def right( self ):
        return self._right
    def data( self ):
        return self._data

def traverse( root ):
    if root == None:
        return
    traverse( root.left() )
    print root.data()
    traverse( root.right() )

def createConv( root, d, s ):
    if root == None:
        print 'Returning None'
        return d
    d = createConv( root._left, d, s + '0' )
    if root._data:
        print 'Setting', root._data, 'to', s, ' before, dict =', d
        d[root._data] = s
    d = createConv( root._right, d, s + '1' )

# class Tree():
#     def __init__( self ):
#         self._root = None
#     def add( self, data ):
#         self._root = self.addImpl( self._root, data )
#     def display( self ):
#         self.printImpl( self._root )
#     def addImpl( self, node, data ):
#         if node == None:
#             node = Node( data )
#         elif data < node._data:
#             node._left = self.addImpl( node._left, data )
#         else:
#             node._right = self.addImpl( node._right, data )
#         return node
#     def printImpl( self, node ):
#         if node == None:
#             return
#         self.printImpl( node.left() )
#         print node.data()
#         self.printImpl( node.right() )
#     def addTree( self, tree ):
        
def checkOpts():
    parser = OptionParser()
    parser.add_option('-f', '--file', dest='file', default=None, help="File to compress")
    (options, args) = parser.parse_args()
    file = options.file
    if not file and args:
        file = args[0]
    return file

def main():
    convert = { 'a': 0, 'e':1, 'i':2, 's':3, 't':4, ' ':5, '\n':6 }
    reverser = { 0:'a', 1:'e', 2:'i', 3:'s', 4:'t', 5:' ', 6:'\n' }
    filename = checkOpts()
    print 'Compressing: ', filename
    f = open( filename, 'r' )
#    count = [0] * 256
    count = [0] * len( convert )
    for line in f:
        for c in line:
#            count[ord(c)] += 1
            count[convert[c]] += 1
    heap = []
    for i in range( len( count ) ):
#        heapq.heappush( heap, (count[i], Node(chr(i))) ) 
        heapq.heappush( heap, (count[i], Node( reverser[i] )) )

    while len( heap ) > 1:
        f = heapq.heappop( heap )
        s = heapq.heappop( heap )
        if s[0] < f[0]:
            node = Node( None, s[1], f[1] )
        else:
            node = Node( None, f[1], s[1] )
        heapq.heappush( heap, (s[0] + f[0], node) )
    root = heapq.heappop( heap )
#    traverse( root[1] )
    cc = {}
    cc = createConv( root[1], cc, '' )

if __name__ == '__main__':
    main()

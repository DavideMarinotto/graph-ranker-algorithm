# GraphRanker
Algorithm that finds the best k graphs in input

At the beginning, the program receives two parameters as input:
• d: the number of graph nodes (32-bit int)
• k: the length of the ranking (32-bit int)
After the program receives a sequence of commands.
The only two possible commands are:
AggiungiGrafo (AddGraph): which adds a graph to the list, the graph is represented in memory with an adjacency list.
TopK: witch return the ranking of the best k graphs based on the sum of the shortest paths from node (0,0) to all other reachable nodes of the graph.

The algorithm is optimized according to temporal and spatial complexity criteria.

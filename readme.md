A sokoban solver AI. It solves a simplified version of sokoban where each bix has to be pushed into one sink point, where it disappears. It is based on the sokoban levels of Nethack.

**Note:** Because the game is based on the Nethack Sokoban levels, the term "stone" is used for boxes (i.e. the things that are moved around the level).

# Compiling and running

Sokoban uses [tup](http://gittup.org/tup/) as the build system. It requires a C++ compiler with C++11 support (tested with GCC 4.8 and Clang 3.4) and (a reasonably recent version of) Boost.

To build the project:

1. Initialize submodule dependencies. `git submodule update --init --recursive`
2. If necessary, set the environment variables `EXTRA_CPP_FLAGS` (for additional compiler flags, e.g. `-I`) and `EXTRA_LD_FLAGS` (for additional linker flags, e.g. `-L`).
3. Initialize a tup variant from the build directory. `tup variant build/release.config`
4. Build the project. `tup upd`
5. To get a help of command line options, type: `./build-release/bin/sokoban --help`
6. To solve a game, give it as a parameter: `./build-release/bin/sokoban games/game1.txt`

# Input and output

The program reads its input from a file. Debug output is written to standard error. The solution is written to the standard output in the following format: `(x, y) --> direction`. For example:

```
( 7,  3) --> left
( 7,  2) --> up
( 5,  3) --> down
( 5,  6) --> right
```

If the `--old-style-output` option is given, the output looks like this instead: `(x1, y1) --> (x2, y2)`. For example:

```
( 7,  3) --> ( 8,  3)
( 7,  2) --> ( 7,  3)
( 5,  3) --> ( 5,  2)
( 5,  6) --> ( 4,  6)
```

The following additional files are written:
- `solution.dump`: Visualizes the solution.
- `partitions.dump`: Some information about the calculated heuristics table.
- If `--blocklist-heur-calculator` is enabled:
 - `blocklist.dump`: The result of the preprocessing. Shows which states are blocked or have additional cost.
 - `plusHeur.dump`: Tracks the usage of the preprocessed data.
- If `--dump-style=text` is enabled: `dump.dump`: shows the full progress of finding the solution.
- If `--dump-style=xml` is enabled: `dump.xml`: shows the full progress in a hierarchical form (in xml format).

The dump files can be opened with the `showdump` shell script. It opens the dump file in `less` with some additional coloring.

## Input file format

The input file consists of two parts. It is a file readable by `boost::program_options` (basically an ini file) that describes the extra parameters (typically the preferred preprocessing level) of that particular game (it overrides defaults but is overridden by command line parameters). The comments in this file are used for the actual input. It is of the following format:

```
#height
#width
#line1
#line2
#...
```

The game interprets the following characters:

- `.`: floor
- `o`: stone
- `x`: The destination (sink) tile
- `y`: The starting position of the player.
- Anything else is wall.

This input is case insensitive. So for example, stones can either be represented as `o` or `O`.

Some pre-built games are found in the `games` subdirectory.

## Output file format

The dumps are written in the following format:

```
info line(s)
stones
line1
line2
...
```

The *info lines* store some information that depends on the type of dump. The *stones* line stores the list of stone coordinates. The other lines are the visual representation of the current state (with spaces between columns).

- `*`: wall
- `o`: stone
- `O`: highlighted stone (typically represents stone being moved)
- `.`: floor
- `+`: highlighted floor (typically represents floor tiles reachable for the player)

# Algorithms

The solver uses A\* algorithm for solving the games. Cost is calculated as 1 for each node (moving of one stone one step in one direction). Heuristics is calculated by calculating a heuristic cost for one stone, then adding them. This can be refined with preprocessing.

When the algorithm starts, the processing queue consists of a single node: the initial position. The main loop of the algotithm is as follows:

1. Pop the first node from the processing queue (i.e. one with minimal cost + heuristic value).
2. Expand the popped node. Expansion returns a (possibly empty) list of nodes that are all descendants of the expanded node.
3. Add the new node to the processing queue.

The algorithm runs until either a solution (a node with no stones) is found or the queue becomes empty (i.e. no solution is found).

## Heuristics calculation

The following heuristics calculation methods are used:

### BasicHeurCalculator

This is the simplest method of heuristics calculation. It is typically only used for kickstarting more advanced calculators. The the manhattan distance of each tile from the target is stored. The heuristic values are added for each stone.

### AdvancedHeurCalculator

First, use a `BasicHeurCalculator` to calculate a preliminary heuristics table. Then for each tile:

1. Put a stone on the tile, and leave all other tiles empty.
2. Partition the remaining floor tiles so that each partition consists of the tiles reachable by the player from each other.
3. For each partition:
 1. position the player on a floor tile in the partition.
 4. Solve the problem.
 5. Store the solution

For each stone, get the heuristics value from the above calculated values. Use the partition that corresponds to the player's current position. Add the values for each stone.

This heuristics calculator is used by default.

### BlocklistHeurCalculator

This type of heuristics calculator requires preprocessing. During preprocessing, a set of states are found which have higher heuristics value than the value of the stones added. These states are stored, either in a vector or a decision tree. The difference between the two storage methods is that finding relevant states in the decision tree is algorithmically faster, although if there are a small amount of elements, vector can be faster because of the simpler data structure used.

The vector and the decision tree should theoretically yield the same result, but in practice they can be different in some cases.

## Expanding nodes

The following kind of expanders can be used.

### Normal expander

1. For each stone, try to push the stone one tile in each direction.
2. For each possible move, create a potential node.
3. Run a checker on the potential node.
4. If the check succeeds, check whether the node is new (it has never been found before).
5. If the node is new, add the node to the processing queue.

For a move to become a potential node, the following need to hold:
- The tile in the opposite direction of moving (i.e. before the stone) must be a floor tile that is reachable by the player.
- The tile where the stone is moved must be a floor tile.
- The heuristics calculator says that putting the stone to the destination tile is possible.






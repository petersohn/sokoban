[![Stories in Ready](https://badge.waffle.io/petersohn/sokoban.png?label=ready&title=Ready)](https://waffle.io/petersohn/sokoban)
A sokoban solver AI. It solves a simplified version of sokoban where each bix has to be pushed into one sink point, where it disappears. It is based on the sokoban levels of Nethack.

**Note:** Because the game is based on the Nethack Sokoban levels, the term "stone" is used for boxes (i.e. the things that are moved around the level).

# Table of contents

- [Getting started](#getting-started)
    - [Building](#building)
    - [Running](#running)
- [Input and output](#input-and-output)
    - [Input file format](#input-file-format)
    - [Output file format](#output-file-format)
- [Algorithms](#algorithms)
    - [Heuristics calculation](#heuristics-calculation)
        - [Basic Heuristics Calculator](#basic-heuristics-calculator)
        - [Advanced Heuristics Calculator](#advanced-heuristics-calculator)
        - [Blocklist Heuristics Calculator](#blocklist-heuristics-calculator)
    - [Expanding nodes](#expanding-nodes)
        - [Normal Expander](#normal-expander)
        - [Stone Pusher](#stone-pusher)
    - [Checkers](#checkers)
        - [Movable Checker](#movable-checker)
        - [Corridor Checker](#corridor-checker)
        - [Blocklist Checker](#blocklist-checker)
    - [Preprocessing](#preprocessing)
    - [Parallelization](#parallelization)

# Getting started

These are the first steps to start solving Sokoban problems. Read further for more detailed description of the program.

## Building

Sokoban uses [tup](http://gittup.org/tup/) as the build system. It requires a C++ compiler with C++11 support (tested with GCC 4.8 and Clang 3.4) and (a reasonably recent version of) Boost.

To build the project:

Initialize submodule dependencies.
```bash
git submodule update --init --recursive
```

If necessary, set the environment variables `EXTRA_CPP_FLAGS` (for additional compiler flags, e.g. `-I`) and `EXTRA_LD_FLAGS` (for additional linker flags, e.g. `-L`).
```bash
export EXTRA_CPP_FLAGS='-I/path/to/boost/include'
export EXTRA_LD_FLAGS='-L/path/to/boost/lib'
```

Initialize a tup variant from the build directory.
```bash
tup variant build/release.config
```

Build the project.
```bash
tup upd
```

## Running

Tup generates the binary files inside subdirectories corresponding to the variants. The actual binary has the path `build-<variant>/bin/sokoban`. In these examples, the *release* variant is used.

To get a help of command line options, use the `--help` option.
```bash
./build-release/bin/sokoban --help
```

To solve a game, give the file name as a parameter. Games are found under the `games` subdirectory.
```bash
./build-release/bin/sokoban games/game1.txt
```

The solution is printed to the standard output, but it is not very visual. A more visual representation of the solution is saved to a file, which can be visualized with a script.
```bash
./showdump solution.dump
```

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
- `plusHeur.dump`: Stores some information about experimental features. Currently it contains no useful information.
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

The heuristics calculators are used for each node (or one stone) to calculate the heuristic value for the A\* algorithm. At a time only one of them is used, but they can rely on others for their operation.

The following heuristics calculation methods are used:

### Basic Heuristics Calculator

This is the simplest method of heuristics calculation. It is typically only used for kickstarting more advanced calculators. The the manhattan distance of each tile from the target is stored. The heuristic values are added for each stone.

### Advanced Heuristics Calculator

First, use a Basic Heuristics Calculator to calculate a preliminary heuristics table. Then for each tile:

1. Put a stone on the tile, and leave all other tiles empty.
2. Partition the remaining floor tiles so that each partition consists of the tiles reachable by the player from each other.
3. For each partition:
 1. position the player on a floor tile in the partition.
 4. Solve the problem.
 5. Store the cost value of the solution (or that it is unsolvable).

For each stone, get the heuristics value from the above calculated values. Use the partition that corresponds to the player's current position. When calculating the heuristics for a node, add the values for each stone.

This heuristics calculator is used by default.

### Blocklist Heuristics Calculator

This type of heuristics calculator requires preprocessing. During preprocessing, a set of states are found which have higher heuristics value than the value of the stones added. These substates are stored, and checked against the state during heuristics calculation. If a set of stones match any substate, the stored heuristics value is added. For stones that do not match any of the substates or where all matched substates are already used up, the heuristics value of the fallback calculator is used. If heuristics is calculated for one stone only, the fallback calculator is used.

The states are stored either in a vector or a decision tree. The difference between the two storage methods is that finding relevant states in the decision tree is algorithmically faster, although if there are a small amount of elements, vector can be faster because of the simpler data structure used.

This heuristics calculator is not used by default.

## Expanding nodes

The task of the expanders is to find the child nodes for each node picked by the A\* algorithm. It is possible to use multiple expanders. The following expanders can be used.

### Normal Expander

1. For each stone, try to push the stone one tile in each direction.
2. For each possible move, create a potential node.
3. Run a checker on the potential node.
4. If the check succeeds, check whether the node is new (it has never been found before).
5. If the node is new, add the node to the processing queue.

For a move to become a potential node, the following need to hold:
- The tile in the opposite direction of moving (i.e. before the stone) must be a floor tile that is reachable by the player.
- The tile where the stone is moved must be a floor tile.
- The heuristics calculator says that putting the stone to the destination tile is possible.

This expander is always in use.

### Stone Pusher

If enabled, this expander runs before the normal expander.

1. Find a stone that can be trivially pushed to the destination tile.
2. Push the stone to the destination tile.
3. Repeat the previous steps until no more trivial pushes are possible.
4. If any stones are pushed:
 - Add a node with the new state to the processing queue (if new).
 - Stop processing the current node (skip normal expander)

A stone can be trivially pushed if both of the following is true:
- The stone can be moved to the destination tile without moving any other stones.
- At every move during the path, the heuristics value of the stone decreases by 1. This means that the *cost + heuristics* value does not increase, so the stone moves on an optimal path for the A\* algorithm.

This expander is used by default.

## Checkers

The checkers are run for each potential node by the normal expander. They check for certain conditions that make the game unwinnable from the current state. There is no need for checking for the stone pusher, as it only makes "safe" moves. If there are multiple checkers, any check failing means that the state is unsolvable.

### Movable Checker

This checker checks whether the stone moved is not stuck (i.e. movable). A stone is movable, if both tiles directly next to the stone either horizontally or vertically (i.e. either the tiles left and right of the stone, or the tiles above and below the stone) are either:
- Floor
- Another movable stone

For example, in the following situation the stones are not movable because all stones have another stone next to it which is not movable:

```
. . . .
. o o .
. o o .
. . . .
```

On the other hand, in the next situation all stones are movable, becuase the stones at either end have floor tiles in one direction, and the middle stone has a movable stone in both directions.

```
. . . *
. o o *
. o . *
. . . *
```

This checker is used by default.

### Corridor Checker

This checker checks for corridors near the stone moved. A corridor is a series of floor tiles that span either only one row or only one column. For example, the following situations have corridors (marked with +):

```
* * * *
* + o .
* + o .
* o . .
```

```
. . . . .
o o o o .
o + + + o
. o o o .
```

The following is not a corridor though:

```
. . . .
o o . .
o . o .
o . . o
* * * *
```

It is also not a corridor if the player is standing inside it.

If a corridor is found, the endpoints are checked. The check passes if the corridor can be opened from either end. A corridor can be opened if the tile just after the last floor tile in the row is a stone and both neighboring tiles perpendicular to the corridor are floor tiles. For example, the following corridor can be opened:

```
* o o o .
* + + + o
* o o o .
```

But this one can not:

```
o o o .
o . . o
. o o o
```

For 1x1 corridors (i.e. one isolated floor tile) the check passes if any of the four endpoints can be opened.

This checker is used by default.

### Blocklist Checker

Use preprocessing to find a set of states which are unsolvable. If any of these states are found in the current state, then the check fails.

This check is used if preprocessing is enabled.

## Preprocessing

Generate a set of states on the table with the following algorithm. The table does not contain the initial stones present in the game.

1. Put *n* stones on the table, with no two of them further away from each other than *k* in each dimension (for example, if *k*=4, then stones at (1,1) and (5,5) are allowed but at (1,1) and (1,6) are not). In other words, put *n* stones in every *k* x *k* portion of the table.
2. Run checkers to see whether there is a possible solution.
3. Find a solution.
4. If no solution is found or the total cost of the solution is more than the initial heuristic value, store the state.

The output is used by Blocklist Checker and Blocklist Heuristics Calculator.

Preprocessing can be parallelized. Since for each substate the program operates on distinct data, they can be run in parallel. In fact, the solutions are always generated on separate thread(s) from the main thread, so the generation of states always runs parallel to the solutions, even if the number of threads are set to 1.

Preprocessing is enabled if *n* > 1. If preprocessing is enabled, Blocklist Checker is automatically enabled. It is also a prerequisite for using Blocklist Heuristics Calculator.

The following parameters control preprocessing:

- `--blocklist-number`: Specifies *n*.
- `--blocklist-distance`: Specifies *k*.
- `--thread-num`: Specifies the number of working threads to use.

## Parallelization

Apart from parallel preprocessing, it is possible to use some parallelization for the main search. It can be done with the `--parallel-outer-expand` parameter. However, this usually does not yield good performance, so its usage is not recommended.

The basic concept of parallel expansion is to pick more than one nodes from the queue in the main loop and expand them in parallel. To keep the invariants of the A\* algorithm intact, only expand nodes with the same *cost + heuristics* value parallelly.







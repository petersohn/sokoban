[![Stories in Next](https://badge.waffle.io/petersohn/sokoban.png?label=next&title=Next)](https://waffle.io/petersohn/sokoban)

A sokoban solver AI. It solves a simplified version of sokoban where each bix has to be pushed into one sink point, where it disappears. It is based on the sokoban levels of Nethack.

**Note:** Because the game is based on the Nethack Sokoban levels, the term "stone" is used for boxes (i.e. the things that are moved around the level).

# Table of contents

- [Getting started](#getting-started)
    - [Building](#building)
    - [Running](#running)
- [Input and output](#input-and-output)
    - [Standard output formatting](#standard-output-formatting)
    - [Input file format](#input-file-format)
    - [Output file format](#output-file-format)
        - [Dump styles](#dump-styles)
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
        - [Choke points](#choke-points)

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

The program reads its input from a file. Debug output is written to standard error. Results are printed to standard output. The format is controlled by the `--output-format` parameter. By default nothing is printed.

The following additional files are written:
- `solution.dump`: Visualizes the solution. Can be configured from command line.
- `partitions.dump`: Some information about the calculated heuristics table. Can be configured from command line.
- If `--blocklist-heur-calculator` is enabled:
 - `blocklist.dump`: The result of the preprocessing. Shows which states are blocked or have additional cost.
- `plusHeur.dump`: Stores some information about experimental features. Currently it contains no useful information.
- If `--dump-style` is not set to `none`: `dump.*` shows information about the details of solving the problem. The default file name depends on the actual dump style, and can be configured from command line.

The dump files (the ones typically names `*.dump`) can be opened with the `showdump` shell script. It opens the dump file in `less` with some additional coloring.

## Standard output formatting

The output format parameter accepts parameters between `%` characters that are substituted by a value. For example: `Expanded nodes = %expanded-nodes%` is translated to something like `Expanded nodes = 12451`. These parameters may accept additional arguments, separated by `:` characters.. For example: %solution:dump%`. Standard escape sequences such as `\n` or `\t` are also accepted.

The following parameters are accepted:

| Name        | Description
| ----------- | -----------
| `solution`                   | The solution. Accepts solution type as a parameter. Default is `direction`.
| `length`                     | The length of the solution. If there is no solution, it is 0.
| `quality`                    | The quality of the solution. Possible values are: `none` (no solution), `good` (the solution is good) or `bad` (there is an error in the solution). If arguments are supplied, they are printed instead of the predefined strings.
| `status-moved`               | The number of times the `Status` data structure is moved.
| `status-copied`              | The number of times the `Status` data structure is copied.
| `calculate-reachable-called` | The number of times reachability is calculated for `Status`.
| `expanded-nodes`             | The number of nodes expanded by the A\* algorithm.
| `total-processor-time`       | Total processor time used for finding the solution.
| `total-real-time`            | Total real time used for finding the solution.
| `chokepoint-processor-time`  | Processor time used for finding choke points.
| `chokepoint-real-time`       | Real time used for finding choke points.
| `iteration-processor-time`   | Processor time used for all iterations during preprocessing.
| `iteration-real-time`        | Real time used for all iterations during preprocessing.

The solution can be displayed in the following formats:
- `direction`: Write each step of the solution in a new line in the following format: `(x, y) --> direction`. For example:

```
( 7,  3) --> left
( 7,  2) --> up
( 5,  3) --> down
( 5,  6) --> right
```

- `coordinate`: Similar to `direction`, but with the following format: `(x1, y1) --> (x2, y2)`. For example:

```
( 7,  3) --> ( 8,  3)
( 7,  2) --> ( 7,  3)
( 5,  3) --> ( 5,  2)
( 5,  6) --> ( 4,  6)
```
- `minimal`: Write a simple machine readable format without new lines. Each step is written as `x1,y1->x2,y2` and the steps are separated by spaces.
- `dump`: Use a visual representation that is the same as the one written into the solution file (see [Output file format](#output-file-format)).

The time formats optionally accept a parameter for displaying the time.

| Value            | Description
| ---------------- | ----------
| `full` (default) | Standard human-readable format (hh:mm:ss).
| `s`(`econds`)    | The total seconds rounded down.
| `ms`(`econds`)   | The total milliseconds.


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

### Dump styles

The `--dump-style` parameter controls what kind of dump is generated. More detailed dumps usually have a higher performance impact and are harder to see through, but they give more information about what is happening.

| Dump style    | Default file name | Performance impact | Description
| ------------- | ----------------- | ------------------ | -----------
| `none`        | -                 | -                  | No dumping.
| `text`        | `dump.dump`       | Low - moderate     | Dump everything in a standard dump format. Dumps when a node is added to the queue, expanded, pushed or rejected. Dumped elements can be filtered using the `--dump-filter` and `--dump-filter-type` command line parameters.
| `xml`         | `dump.xml`        | High               | Dumps the same information as `text`, but puts it into a tree in XML format. Note that the file is only written at the end, so there is no partial solution when the program is interrupted. It also greatly increases memory usage.
| `statistics`  | `dump.csv`        | Low                | Prints the number of times each event happens (node added to the queue, node expanded etc.) each second in a CSV format.
| `best`        | `dump.dump`       | Low                | Dumps nodes that are added to the queue, but only ones that has the lowest heuristic value so far.

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

This expander is decorated by another (normal) expander. If enabled, it tries to push stones to the destination, then calls the decorating expander.

1. Find a stone that can be trivially pushed to the destination tile.
2. Push the stone to the destination tile.
3. Repeat the previous steps until no more trivial pushes are possible.
4. If any stones are pushed, call the decorating expander with the resulting node. Otherwise, call it with the original node.

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

The following options are possible for this checker:
- `none`: This checker is not used.
- `simple`: The checker is used but reachability is not considered. This is the default.
- `extended`: Reachability is considered. A stone is only movable if the point where it is moved from is reachable.

Note that `extended` is much slower than `simple`.

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

The options for this checker is the same as for Movable Checker. In this case `extended` is somewhat slower than `simple`.

### Blocklist Checker

Use preprocessing to find a set of states which are unsolvable. If any of these states are found in the current state, then the check fails.

This check is used if preprocessing is enabled.

## Preprocessing

Generate a set of states on the table with the following algorithm. The table does not contain the initial stones present in the game.

1. Put *n* stones on the table, with no two of them further away from each other than *k* in each dimension (for example, if *k*=4, then stones at (1,1) and (5,5) are allowed but at (1,1) and (1,6) are not). In other words, put *n* stones in every *k* x *k* portion of the table. Exceptions may happen when choke points are enabled.
2. Run checkers to see whether there is a possible solution.
3. Find a solution.
4. If no solution is found or the total cost of the solution is more than the initial heuristic value, store the state.

The output is used by [Blocklist Checker](#blocklist-checker) and [Blocklist Heuristics Calculator](#blocklist-heuristics-calculator).

Preprocessing can be parallelized. Since for each substate the program operates on distinct data, they can be run in parallel. In fact, the solutions are always generated on separate thread(s) from the main thread, so the generation of states always runs parallel to the solutions, even if the number of threads are set to 1.

Preprocessing is enabled if *n* > 1. If preprocessing is enabled, Blocklist Checker is automatically enabled. It is also a prerequisite for using Blocklist Heuristics Calculator.

The following parameters control preprocessing:

- `--blocklist-number`: Specifies *n*.
- `--blocklist-distance`: Specifies *k*.
- `--thread-num`: Specifies the number of working threads to use.

### Choke points

It is possible to enhance preprocessing by finding so called choke points before running the preprocessing. After that, the conditions of point 1 above change with the following:

- No two stones can be further away from each other than *k* if none of them are on a choke point. At most *l* stones can be further away from each other than *k* if at least one of them is on a choke point.

The definition of a choke point is that putting a stone there can potentially split the table into more unreachable part. It is calculated by putting some stones on the table and checking whether there are more than one partitions that are not reachable from other parts of the table.

The rationale behind choke points is that it is more probable that something interesting is found if a stone is on a choke point than if they are just put down randomly. This way interesting states can be found with lower *k* values.

The following parameters control choke points:

- `--choke-point-number`: The number of stones to put on the table while finding choke points.
- `--choke-point-distance`: The minimum distance between stones while finding choke points. The purpose is to avoid large gaps to be considered as choke points.
- `--choke-point-distant-number`: Specifies *l*.





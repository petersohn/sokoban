common_source_files = \
		AdvancedHeurCalculator.cpp \
		BasicHeurCalculator.cpp \
		BlockListChecker.cpp \
		BlockListGenerator.cpp \
		BlockListHeurCalculator.cpp \
		Common.cpp \
		ComplexChecker.cpp \
		ComplexExpander.cpp \
		CorridorChecker.cpp \
		DumperFunctions.cpp \
		MovableChecker.cpp \
		Node.cpp \
		NormalExpander.cpp \
		Options.cpp \
		OptionsHelper.cpp \
		Point.cpp \
		PrioNodeQueue.cpp \
		SolutionChecker.cpp \
		Solver.cpp \
		SolverFactories.cpp \
		Status.cpp \
		StatusCreator.cpp \
		StonePusher.cpp \
		Table.cpp \
		TableIterator.cpp \
		TableHeurCalculator.cpp \
		TextDumper.cpp \
		ThreadPool.cpp \
		VisitedStateInfo.cpp \
		VisitedStates.cpp \
		XDumper.cpp \
		XML.cpp 


sokoban_SOURCES = main.cpp $(common_source_files)
		

unitTest_SOURCES = UnitTest/testMain.cpp \
		UnitTest/DecisionTreeTest.cpp \
		UnitTest/IsSubStateTest.cpp \
		UnitTest/IsSubStatusTest.cpp \
		 $(common_source_files)

bin_PROGRAMS = sokoban unitTest

AM_CXXFLAGS=-std=c++0x

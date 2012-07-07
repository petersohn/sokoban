status_source_files = \
		Status/Point.cpp \
		Status/Status.cpp \
		Status/StatusCreator.cpp \
		Status/Table.cpp
		
sokoban_source_files = \
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
		PrioNodeQueue.cpp \
		SolutionChecker.cpp \
		Solver.cpp \
		SolverFactories.cpp \
		StonePusher.cpp \
		TableIterator.cpp \
		TableHeurCalculator.cpp \
		TextDumper.cpp \
		ThreadPool.cpp \
		VisitedStateInfo.cpp \
		VisitedStates.cpp \
		XDumper.cpp \
		XML.cpp 


sokoban_SOURCES = main.cpp $(sokoban_source_files) $(status_source_files)
		

unitTest_SOURCES = UnitTest/testMain.cpp \
		UnitTest/DecisionTreeTest.cpp \
		UnitTest/IsSubStateTest.cpp \
		UnitTest/IsSubStatusTest.cpp \
		 $(status_source_files)

bin_PROGRAMS = sokoban unitTest

AM_CXXFLAGS=-std=c++0x

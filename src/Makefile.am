status_source_files = \
		Status/Point.cpp \
		Status/Status.cpp \
		Status/StatusCreator.cpp \
		Status/StatusUtils.cpp \
		Status/Table.cpp
		
dumper_source_files = \
		Dumper/DumperFunctions.cpp \
		Dumper/TextDumper.cpp \
		Dumper/XDumper.cpp \
		Dumper/XML.cpp 
		
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
		DecisionTreeHeurCalculator.cpp \
		MovableChecker.cpp \
		Node.cpp \
		NormalExpander.cpp \
		Options.cpp \
		OptionsHelper.cpp \
		PrioNodeQueue.cpp \
		ProgressBar.cpp \
		SolutionChecker.cpp \
		Solver.cpp \
		SolverFactories.cpp \
		StonePusher.cpp \
		TableIterator.cpp \
		TableHeurCalculator.cpp \
		ThreadPool.cpp \
		VisitedStateInfo.cpp \
		VisitedStates.cpp


sokoban_SOURCES = main.cpp \
		$(sokoban_source_files) \
		$(dumper_source_files) \
		$(status_source_files)
		

unitTest_SOURCES = UnitTest/testMain.cpp \
		UnitTest/IsSubStateTest.cpp \
		UnitTest/IsSubStatusTest.cpp \
		UnitTest/PointRangeTest.cpp \
		 $(status_source_files)

bin_PROGRAMS = sokoban unitTest

AM_CXXFLAGS=-std=c++0x

#include "config.h"
#include "Options.h"

#include <iostream>
#ifdef USE_PROGRAM_OPTIONS
# include <boost/program_options.hpp>
#endif


Options::Options(int argc, char **argv) {
#ifdef USE_PROGRAM_OPTIONS
	using namespace boost::program_options;
	options_description desc;
	desc.add_options()
		("help", "produce help message")
		("enable-dump,d", "enable dump file generation of the process")
		("enable-xml-dump,x", "enable XML dump file generation of the process")
		("old-style-output", "Produce output messages of (x1, y1) --> (x2, y2)"
				" instead of (x1, y1) --> direction")
		("filename", value<std::string>(), "input file name")
		;

	positional_options_description p;
	p.add("filename", 1);


	variables_map vm;
	store(command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
	notify(vm);

	if (vm.count("help") > 0) {
		std::cerr << desc << std::endl;
		exit(0);
	}
	if (vm.count("filename") == 0) {
		std::cerr << "No filename given." << std::endl <<
				desc << std::endl;
		exit(1);
	}
	enableDump_ = vm.count("enable-dump") > 0;
	enableXDump_ = vm.count("enable-xml-dump") > 0;
	oldStyleOutput_ = vm.count("old-style-output") > 0;
	filename_ = vm["filename"].as<std::string>();
#else
	enableDump_ = false;
	enableXDump_ = false;
	oldStyleOutput_ = false;
	if (argc > 1)
		filename_ = argv[1];
	else {
		std::cerr << "No filename given." << std::endl;
		exit(1);
	}
#endif
}


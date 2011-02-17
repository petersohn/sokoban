#include "Options.h"

#include <iostream>
#include <boost/program_options.hpp>


Options::Options(int argc, char **argv) {
	using namespace boost::program_options;
	options_description desc;
	desc.add_options()
		("help", "produce help message")
		("enable-dump,d", "enable dump file generation of the process")
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
	filename_ = vm["filename"].as<std::string>();
}


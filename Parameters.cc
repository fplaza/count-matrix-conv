#include "Parameters.hh"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <iostream>
#include <fstream>
#include <stdexcept>

Parameters Parameters::parse(int argc, char* argv[])
{
	Parameters pars;

	// Create options decription
	po::options_description opts_desc(
			"Converts a count matrix from ASCII to binary and vice-versa");

	opts_desc.add_options()
		("help,h", "print this help message")
		("input-file,i", po::value<std::string>(&pars.input_file)->required(), "count matrix file to convert")
		("output-file,o", po::value<std::string>(&pars.output_file)->required(), "count matrix file generated after the conversion")
		;

	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, opts_desc), vm);

	// Print help
	if (argc == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	po::notify(vm);

	check_file_is_readable(pars.input_file);
	check_file_is_writable(pars.output_file);

	return (pars);
}


void Parameters::check_file_is_readable(const std::string& filepath)
{
	std::ifstream ifs;
	ifs.open(filepath.c_str());
	if (ifs.good())
	{
		ifs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be opened. Check that the path is valid and that you have read permissions."));
	}
}
void Parameters::check_file_is_writable(const std::string& filepath)
{
	std::ofstream ofs;
	ofs.open(filepath.c_str());
	if (ofs.good())
	{
		ofs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be created. Check that the path is valid and that you have write permissions."));
	}
}


#include "Parameters.hh"
#include "CountMatrixReader.hh"
#include "CountMatrixWriter.hh"
#include <iostream>

int main(int argc, char *argv[])
{
	try
	{
		const Parameters& pars = Parameters::parse(argc, argv);
		std::auto_ptr<CountMatrixReader> reader = 
			CountMatrixReaderFactory::create_reader(pars.input_file);
		std::auto_ptr<CountMatrixWriter> writer = 
			CountMatrixWriterFactory::create_writer(pars.output_file);

		std::cout << "Reading input file " << pars.input_file << std::endl;
		const CountMatrix& cm = reader->read_file();
		std::cout << "Done. Input file has " << cm.num_genes() << " genes and " << cm.num_samples() << " samples.\n" << std::endl;


		std::cout << "Conversion in progress..." << std::endl;
		writer->write_to_file(cm);
		std::cout << "Done. Converted file written in " << pars.output_file << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

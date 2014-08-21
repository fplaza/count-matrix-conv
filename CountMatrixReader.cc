#include "CountMatrixReader.hh"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <stdexcept>
#include <fstream>

std::auto_ptr<CountMatrixReader> CountMatrixReaderFactory::create_reader(const std::string& src_path)
{
	std::string file_extension = 
		fs::extension(src_path);

	std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), ::tolower);

	if (file_extension == BINARY_EXT)
	{
		return std::auto_ptr<CountMatrixReader>(new BinaryReader(src_path));
	}
	else if (file_extension== ASCII_EXT)
	{
		return std::auto_ptr<CountMatrixReader>(new AsciiReader(src_path));
	}
	else
	{
		throw (std::invalid_argument("error: " +  file_extension+  " is a unknown file extension."));
	}
}

const std::string CountMatrixReaderFactory::BINARY_EXT = ".bin";
const std::string CountMatrixReaderFactory::ASCII_EXT = ".txt";

CountMatrix BinaryReader::read_file()
{
	std::ifstream ifs;
	ifs.open(src_path_.c_str(), std::ios::binary);


	// Read matrix dimensions
	size_t num_genes;
	ifs.read(reinterpret_cast<char*>(&num_genes), sizeof(size_t));

	size_t num_samples;
	ifs.read(reinterpret_cast<char*>(&num_samples), sizeof(size_t));

	// Allocate memory to store the count matrix
	CountMatrix cm(num_genes, num_samples);

	// Read how many bytes are used to store samples names
	size_t samples_names_size;
	ifs.read(reinterpret_cast<char*>(&samples_names_size), sizeof(size_t));

	// Read samples names (they are separated with a NULL char)
	std::vector<char> samples_names(samples_names_size);
	char* next_sample_name = samples_names.data();
	ifs.read(samples_names.data(), sizeof(char)*samples_names_size);

	for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
	{
		std::string sample_name(next_sample_name);
		cm.samples_names_[curr_sample] = sample_name;
		next_sample_name += sample_name.length()+1;
	}

	// Read genes ids
	ifs.read(reinterpret_cast<char*>(cm.genes_ids_.data()), sizeof(unsigned)*cm.num_genes());
	// Reads genes counts
	ifs.read(reinterpret_cast<char*>(cm.data_.data()), sizeof(float)*cm.num_genes()*cm.num_samples());

	ifs.close();
	

	return cm;
}

CountMatrix AsciiReader::read_file()
{
	char line[MAX_LINE_LENGTH];

	std::ifstream ifs;
	ifs.open(src_path_.c_str());

	// FIRST PASS: get the dimensions of the matrix
	// Get the number of samples
	std::string header;
	std::getline(ifs, header);
	size_t num_samples = std::count(header.begin(), header.end(), '\t');

	// Get the number of genes
	size_t num_genes = 0;
	while (ifs.getline(line, MAX_LINE_LENGTH))
		num_genes++;

	// Allocate memory to store the count matrix
	CountMatrix cm(num_genes, num_samples);

	//SECOND PASS: fill the matrix
	ifs.clear();
	ifs.seekg(0);

	// Get samples names
	// Skip "gene_id" or "fragment_id" string
	ifs >> header;
	for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
	{
		ifs >> cm.samples_names_[curr_sample];
	}
	ifs.get();

	// Get genes ids and genes counts
	for (size_t curr_gene = 0; curr_gene < cm.num_genes(); curr_gene++)
	{
		ifs.getline(line, MAX_LINE_LENGTH);
		char* pos = line;

		cm.genes_ids_[curr_gene] = strtoul(pos, &pos, 10);

		for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
		{
			cm.data(curr_gene, curr_sample) = strtof(pos, &pos);
		}
	}

	ifs.close();

	return cm;
}

// Should be sufficient for the biggest matrices currently processed
const size_t AsciiReader::MAX_LINE_LENGTH = 100000;

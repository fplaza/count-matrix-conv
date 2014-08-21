#include "CountMatrixWriter.hh"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <stdexcept>
#include <fstream>

std::auto_ptr<CountMatrixWriter> CountMatrixWriterFactory::create_writer(const std::string& dest_path)
{
	std::string file_extension = 
		fs::extension(dest_path);

	std::transform(file_extension.begin(), file_extension.end(), file_extension.begin(), ::tolower);

	if (file_extension == BINARY_EXT)
	{
		return std::auto_ptr<CountMatrixWriter>(new BinaryWriter(dest_path));
	}
	else if (file_extension== ASCII_EXT)
	{
		return std::auto_ptr<CountMatrixWriter>(new AsciiWriter(dest_path));
	}
	else
	{
		throw (std::invalid_argument("error: " +  file_extension+  " is a unknown file extension."));
	}
}

const std::string CountMatrixWriterFactory::BINARY_EXT = ".bin";
const std::string CountMatrixWriterFactory::ASCII_EXT = ".txt";

void BinaryWriter::write_to_file(const CountMatrix& cm)
{
	std::ofstream ofs;
	ofs.open(dest_path_.c_str(), std::ios::binary);

	// Write matrix dimensions
	size_t num_genes = cm.num_genes();
	ofs.write(reinterpret_cast<char*>(&num_genes), sizeof(size_t));
	size_t num_samples = cm.num_samples();
	ofs.write(reinterpret_cast<char*>(&num_samples), sizeof(size_t));


	// Compute how many bytes are necessary to store samples names
	size_t samples_names_size = 0;
	for (unsigned curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
	{
		const std::string& curr_sample_name = cm.sample_name(curr_sample);
		samples_names_size += curr_sample_name.length() + 1;
	}

	// Write samples names
	ofs.write(reinterpret_cast<char*>(&samples_names_size), sizeof(size_t));
	for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
	{
		const std::string& curr_sample_name = cm.sample_name(curr_sample);
		ofs.write(curr_sample_name.c_str(), sizeof(char)*curr_sample_name.length());
		ofs.write("\0", sizeof(char));
	}

	// Write genes ids
	ofs.write(reinterpret_cast<const char*>(cm.genes_ids_data()), sizeof(unsigned)*cm.num_genes());
	// Write genes counts
	ofs.write(reinterpret_cast<const char*>(cm.data()), sizeof(float)*cm.num_genes()*cm.num_samples());

	ofs.close();
}

void AsciiWriter::write_to_file(const CountMatrix& cm)
{
	std::ofstream ofs;
	ofs.open(dest_path_.c_str());

	// Write header with samples names
	ofs << "gene_id";

	for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
		ofs << '\t' << cm.sample_name(curr_sample);
	ofs << '\n';

	// Write genes counts
	for (size_t curr_gene = 0; curr_gene < cm.num_genes(); curr_gene++)
	{
		ofs << cm.gene_id(curr_gene);

		for (size_t curr_sample = 0; curr_sample < cm.num_samples(); curr_sample++)
			ofs << '\t' << cm.data(curr_gene, curr_sample);
		ofs << '\n';
	}

	ofs.close();
}

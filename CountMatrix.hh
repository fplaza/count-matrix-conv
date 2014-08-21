#ifndef COUNT_MATRIX_HH
#define COUNT_MATRIX_HH

#include <vector>
#include <string>

class CountMatrix
{
	friend class AsciiReader;
	friend class BinaryReader;
	private:
		std::vector<unsigned> genes_ids_;
		std::vector<std::string> samples_names_;
		std::vector<float> data_;
		CountMatrix(size_t num_genes, size_t num_samples);
		float& data(size_t g, size_t s);
	public:
		size_t num_samples() const;
		size_t num_genes() const;
		const std::string& sample_name(size_t s) const;
		unsigned gene_id(size_t g) const;
		float data(size_t g, size_t s) const;
		const unsigned* genes_ids_data() const;
		const float* data() const;

};

inline CountMatrix::CountMatrix(size_t num_genes, size_t num_samples)
	: genes_ids_(num_genes), samples_names_(num_samples), data_(num_genes*num_samples)
{};

inline float& CountMatrix::data(size_t g, size_t s)
{
	return data_[g*num_samples() + s];
}

inline size_t CountMatrix::num_samples() const
{
	return samples_names_.size();
}

inline size_t CountMatrix::num_genes() const
{
	return genes_ids_.size();
}

inline const std::string& CountMatrix::sample_name(size_t s) const
{
	return samples_names_[s];
}

inline unsigned CountMatrix::gene_id(size_t g) const
{
	return genes_ids_[g];
}


inline float CountMatrix::data(size_t g, size_t s) const
{
	return data_[g*num_samples() + s];
}

inline const unsigned* CountMatrix::genes_ids_data() const
{
	return genes_ids_.data();
}

inline const float* CountMatrix::data() const
{
	return data_.data();
}

#endif // COUNT_MATRIX_HH

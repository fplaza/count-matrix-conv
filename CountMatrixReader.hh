#ifndef COUNT_MATRIX_READER_HH
#define COUNT_MATRIX_READER_HH

#include "CountMatrix.hh"
#include <string>
#include <memory>

class CountMatrixReader
{
	public:
		virtual CountMatrix read_file() = 0;
		virtual ~CountMatrixReader();
	protected:
		const std::string src_path_;
		CountMatrixReader(const std::string& src_path);
};

inline CountMatrixReader::CountMatrixReader(const std::string& src_path)
	: src_path_(src_path)
{};

inline CountMatrixReader::~CountMatrixReader()
{};

class AsciiReader : public CountMatrixReader
{
	friend class CountMatrixReaderFactory;
	public:
		virtual CountMatrix read_file();
	protected:
		AsciiReader(const std::string& src_path);
	private:
		static const size_t MAX_LINE_LENGTH;
};

inline AsciiReader::AsciiReader(const std::string& src_path)
	: CountMatrixReader(src_path)
{};

class BinaryReader : public CountMatrixReader
{
	friend class CountMatrixReaderFactory;
	public:
		virtual CountMatrix read_file();
	protected:
		BinaryReader(const std::string& src_path);
};

inline BinaryReader::BinaryReader(const std::string& src_path)
	: CountMatrixReader(src_path)
{};

class CountMatrixReaderFactory 
{
	public:
		static std::auto_ptr<CountMatrixReader> create_reader(const std::string& src_path);
	private:
		static const std::string BINARY_EXT;
		static const std::string ASCII_EXT;
};


#endif // COUNT_MATRIX_READER_HH

#ifndef COUNT_MATRIX_WRITER_HH
#define COUNT_MATRIX_WRITER_HH

#include "CountMatrix.hh"
#include <string>
#include <memory>

class CountMatrixWriter
{
	public:
		virtual void write_to_file(const CountMatrix& cm) = 0;
		virtual ~CountMatrixWriter();
	protected:
		CountMatrixWriter(const std::string& dest_path);
		const std::string dest_path_;
};

inline CountMatrixWriter::~CountMatrixWriter()
{};

inline CountMatrixWriter::CountMatrixWriter(const std::string& dest_path)
	: dest_path_(dest_path)
{};

class AsciiWriter : public CountMatrixWriter
{
	friend class CountMatrixWriterFactory;
	public:
		virtual void write_to_file(const CountMatrix& cm);
	protected:
		AsciiWriter(const std::string& dest_path);
};

inline AsciiWriter::AsciiWriter(const std::string& dest_path)
	: CountMatrixWriter(dest_path)
{};

class BinaryWriter : public CountMatrixWriter
{
	friend class CountMatrixWriterFactory;
	public:
		virtual void write_to_file(const CountMatrix& cm);
	protected:
		BinaryWriter(const std::string& dest_path);
};

inline BinaryWriter::BinaryWriter(const std::string& dest_path)
	: CountMatrixWriter(dest_path)
{};

class CountMatrixWriterFactory 
{
	public:
		static std::auto_ptr<CountMatrixWriter> create_writer(const std::string& dest_path);
	private:
		static const std::string BINARY_EXT;
		static const std::string ASCII_EXT;
};


#endif // COUNT_MATRIX_WRITER_HH

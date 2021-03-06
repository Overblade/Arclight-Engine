#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

#include "uri.h"
#include "types.h"



class File {

public:

	typedef u8 Flags;

	static constexpr u8 In		= std::ios::in;
	static constexpr u8 Out		= std::ios::out;
	static constexpr u8 Binary	= std::ios::binary;
	static constexpr u8 AtEnd	= std::ios::ate;
	static constexpr u8 Append	= std::ios::app;
	static constexpr u8 Trunc	= std::ios::trunc;

	File();
	explicit File(const Uri& path, File::Flags flags = File::In);

	bool open(const Uri& path, File::Flags flags = File::In);
	void close();

	std::string read(u64 count);
	std::string readWord();
	std::string readLine();
	std::string readAll();
	void write(const std::string& text);
	void writeLine(const std::string& line);

	void read(u8* data, u64 count);
	void write(const u8* data, u64 count);

	void seek(u64 pos);
	void seekRelative(i64 pos);

	u64 tell() const;

	bool isOpen() const;
	u64 getFileSize() const;

	Uri getUri() const;
	Flags getStreamFlags() const;
	u64 getLastWriteTime() const;
	

private:
	mutable std::fstream stream;
	Uri filepath;
	Flags openFlags;

};
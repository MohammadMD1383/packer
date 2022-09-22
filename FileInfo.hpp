#ifndef PACKER_FILEINFO_HPP
#define PACKER_FILEINFO_HPP

#include <cstdint>
#include <ios>

struct FileInfo {
	uint64_t fileSize;
	uint16_t nameSize;
	
	friend std::ostream &operator<<(std::ostream &os, FileInfo &_this) {
		os.write(reinterpret_cast<char *>(&_this), sizeof _this);
		return os;
	}
	
	friend std::istream &operator>>(std::istream &is, FileInfo &_this) {
		is.read(reinterpret_cast<char *>(&_this), sizeof _this);
		return is;
	}
};

#endif //PACKER_FILEINFO_HPP

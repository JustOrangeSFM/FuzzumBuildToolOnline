#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "lz4.h"
namespace fs = std::filesystem;
#include <unordered_map>

class PakReader {
	std::ifstream file_;
	struct FileInfo {
		uint64_t offset;
		uint64_t compressedSize;
		uint64_t originalSize;
	};
	std::unordered_map<std::string, FileInfo> index_;

public:
	bool open(const std::string& pakPath) {
		file_.open(pakPath, std::ios::binary);
		if (!file_) return false;

		char magic[4];
		file_.read(magic, 4);
		if (std::string(magic, 4) != "PACK") {
			file_.close();
			return false;
		}

		uint32_t count;
		file_.read(reinterpret_cast<char*>(&count), sizeof(count));

		for (uint32_t i = 0; i < count; ++i) {
			uint32_t pathLength;
			file_.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));

			std::string path(pathLength, '\0');
			file_.read(&path[0], pathLength);

			// Пропускаем паддинг
			file_.seekg(256 - pathLength, std::ios::cur);

			uint64_t offset, compSize, origSize;
			file_.read(reinterpret_cast<char*>(&offset), sizeof(offset));
			file_.read(reinterpret_cast<char*>(&compSize), sizeof(compSize));
			file_.read(reinterpret_cast<char*>(&origSize), sizeof(origSize));

			index_[path] = { offset, compSize, origSize };
		}
		return true;
	}

	std::vector<char> readFile(const std::string& path) {
		auto it = index_.find(path);
		if (it == index_.end()) return {};

		const auto& info = it->second;
		file_.seekg(info.offset);
		std::vector<char> compressed(info.compressedSize);
		file_.read(compressed.data(), info.compressedSize);

		std::vector<char> decompressed(info.originalSize);
		int decompressedSize = LZ4_decompress_safe(
			compressed.data(),
			decompressed.data(),
			static_cast<int>(info.compressedSize),
			static_cast<int>(info.originalSize)
		);

		if (decompressedSize < 0) {
			std::cerr << "LZ4 decompression error for: " << path << "\n";
			return {};
		}
		return decompressed;
	}

	// Получить список всех файлов в паке
	std::vector<std::string> getFileList() const {
		std::vector<std::string> list;
		for (const auto& [path, _] : index_) {
			list.push_back(path);
			std::cout << "Path " << path << std::endl;
		}
		return list;
	}
};




/*
    std::vector<std::filesystem::path> files = {
        "Assets/image1.png",
        "Assets/sound1.wav"
    };

    std::string projectDir = "Assets";          
    std::string pakFilename = "Resources.pak"; 

    // Writing
    PakWriter writer(pakFilename);
    writer.writeFiles(files, projectDir);
    std::cout << "Pak file created: " << pakFilename << std::endl;

    // Reading
    PakReader reader;
    if (reader.open(pakFilename)) {
        auto fileList = reader.getFileList();
        std::cout << "Files in pak:" << std::endl;
        for (auto& path : fileList) {
            std::cout << " - " << path << std::endl;
            auto data = reader.readFile(path);

            std::cout << "   Size: " << data.size() << " bytes" << std::endl;
        }
    }*/
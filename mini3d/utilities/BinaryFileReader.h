
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


#ifndef MINI3D_BINARYFILEREADER_H
#define MINI3D_BINARYFILEREADER_H

#include <iostream>
#include <fstream>
#include <sstream>

namespace mini3d
{
namespace utilites
{
class BinaryFileReader
{
public:
	static char* ReadBytesFromFile(WCHAR* fileName, unsigned int& sizeInBytes)
	{
		char* data;

		std::fstream fileStream(fileName, std::ios_base::binary | std::ios_base::in);
		if (fileStream)
		{
			fileStream.seekg(0, std::ios::end);
			long fileSize = long(fileStream.tellg()); // pretty safe downcast since this is not supposed to be used with files that arae gazillions of bytes long
			fileStream.seekg(0, std::ios::beg);

			data = new char[fileSize];
			fileStream.read(&data[0], fileSize);
			fileStream.close();

			sizeInBytes = fileSize;
		}
		return data;
	}
};

}
}

#endif

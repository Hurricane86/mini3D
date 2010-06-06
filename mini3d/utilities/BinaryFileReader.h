/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace mini3d
{
namespace utilites
{
class BinaryFileReader
{
public:
	static char* ReadBytesFromFile(std::wstring file, unsigned int& sizeInBytes)
	{
		char* data;

		std::fstream fileStream(file.c_str(), std::ios_base::binary | std::ios_base::in);
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

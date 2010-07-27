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

#ifndef AURORA_ERROR_H
#define AURORA_ERROR_H

namespace mini3d
{
class Error
{
public:
	static const unsigned int MINI3D_ERROR_UNKNOWN = 0;
	static const unsigned int MINI3D_ERROR_NON_POWER_OF_TWO = 1;
	static const unsigned int MINI3D_ERROR_TEXTURE_INDEX_OUTSIDE_VALID_RANGE = 2;
	static const unsigned int MINI3D_ERROR_UNKNOWN_BACK_BUFFER_FORMAT = 3;
	static const unsigned int MINI3D_ERROR_NO_SUPPORTED_DEPTH_STENCIL_FORMAT = 4;
};
}

#endif
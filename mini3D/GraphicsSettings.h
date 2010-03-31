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

#ifndef AURORA_GRAPHICSSETTINGS_H
#define AURORA_GRAPHICSSETTINGS_H

namespace mini3d
{
struct GraphicsSettings
{
	enum MultisampleFormat
	{
		NO_MULTISAMPLE = 0,
		TWO_SAMPLES = 1,
		FOUR_SAMPLES = 2,
		EIGHT_SAMPLES = 3,
		SIXTEEN_SAMPLES = 4
	};

	enum CullMode
	{
		NO_CULLING,
		CLOCKWIZE,
		COUNTER_CLOCKWIZE
	};

	bool fullscreen;
	MultisampleFormat multisampleFormat;
	CullMode cullMode;

	inline GraphicsSettings operator= (const GraphicsSettings &rhs)
	{
		GraphicsSettings graphicsSettings;
		
		graphicsSettings.fullscreen = rhs.fullscreen;
		graphicsSettings.multisampleFormat = rhs.multisampleFormat;
		graphicsSettings.cullMode = rhs.cullMode;

		return graphicsSettings;
	};

	inline bool operator== (const GraphicsSettings &rhs)
	{
		if (this->fullscreen == rhs.fullscreen &&
			this->multisampleFormat == rhs.multisampleFormat &&
			this->cullMode == rhs.cullMode)
		{
			return true;
		}
		else
		{
			return false;
		}
	};
};
}



#endif
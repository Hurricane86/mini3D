
#ifndef AURORA_GRAPHICSSETTINGS_H
#define AURORA_GRAPHICSSETTINGS_H

struct GraphicsSettings
{
	enum ColorFormat
	{
		COLOR_FORMAT_16BIT,
		COLOR_FORMAT_32BIT,
		COLOR_FORMAT_64BIT
	};

	enum MultisampleFormat
	{
		NO_MULTISAMPLE,
		TWO_SAMPLES,
		FOUR_SAMPLES,
		EIGHT_SAMPLES,
		SIXTEEN_SAMPLES
	};

	enum CullMode
	{
		NO_CULLING,
		CLOCKWIZE,
		COUNTER_CLOCKWIZE
	};

	bool windowed;
	ColorFormat colorFormat;
	MultisampleFormat multisampleFormat;
	CullMode cullMode;

	inline GraphicsSettings operator= (const GraphicsSettings &rhs)
	{
		GraphicsSettings graphicsSettings;
		
		graphicsSettings.windowed = rhs.windowed;
		graphicsSettings.colorFormat = rhs.colorFormat;
		graphicsSettings.multisampleFormat = rhs.multisampleFormat;
		graphicsSettings.cullMode = rhs.cullMode;

		return graphicsSettings;
	};

	inline bool operator== (const GraphicsSettings &rhs)
	{
		if (this->windowed == rhs.windowed &&
			this->colorFormat == rhs.colorFormat &&
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



#endif
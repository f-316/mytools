#pragma once

class CMathEx
{
public:

	template <typename TNum>
	/**
	* ”’l‚ğ‘µ‚¦‚Ü‚·B
	* @param[in] val
	* @param[in] align
	* @note
	*  1, 7: 7
	*  8, 7: 14
	* 23, 7: 28
	*/
	static TNum Align(TNum val, int align)
	{
		if (val < (TNum)0 || align <= 0) return 0;
		if (val % align == 0) return val;
		val += align - val % align;
		return val;
	}

};

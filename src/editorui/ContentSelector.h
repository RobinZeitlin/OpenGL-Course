#pragma once

class ContentSelector {
public:
	ContentSelector();

	template<typename T>
	T getContent();
};

template<typename T>
inline T ContentSelector::getContent()
{
	return T();
}

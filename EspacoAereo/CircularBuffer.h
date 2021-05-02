#pragma once
#include <Windows.h>
#include <string>
#include "SharedStructContents.h"

class CircularBuffer {

	CircBuffer* data;

	HANDLE mutex_get;
	HANDLE mutex_set;
	HANDLE mutex_empty_spot;
	HANDLE mutex_available_item;

public:

	CircularBuffer(CircBuffer* data, const TCHAR* mutex_prefix);
	~CircularBuffer();

	PlaneControlMessage get_next_element();
	void set_next_element(PlaneControlMessage& element);

};

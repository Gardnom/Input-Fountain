#pragma once
#include <vector>
#include <queue>

template<class KEYCODETYPE>
class IInputInterface {
public:
	virtual int KeyState(KEYCODETYPE keycode) = 0;
	virtual std::queue<KEYCODETYPE>& CurrentKeys() = 0;

	virtual void Update() = 0;

	KEYCODETYPE ConsumeCurrentKey() {
		auto currKeys = CurrentKeys();
		if (currKeys.size() > 0) {
			KEYCODETYPE toReturn = currKeys.front();
			currKeys.pop();
			return toReturn;
		}
		return 0;
	}
	
	virtual ~IInputInterface() {

	}
};
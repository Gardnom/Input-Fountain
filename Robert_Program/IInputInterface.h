#pragma once
#include <vector>
#include <queue>

template<class KEYCODETYPE>
class IInputInterface {
public:
	virtual int KeyState(KEYCODETYPE keycode) = 0;
	virtual std::queue<KEYCODETYPE>& CurrentKeys() = 0;

	virtual bool Update() = 0;



	virtual bool IsConnected() = 0;

	virtual KEYCODETYPE ConsumeCurrentKey() {
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
	
	virtual int GetDPADState() {
		return 0;
	}
};

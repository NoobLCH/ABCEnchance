#pragma once

#include <future>
#include <list>

class ITaskItem {
public:
	virtual ITaskItem* Start() = 0;
	virtual bool IsReady() = 0;
	virtual std::function<void()> GetCaller() = 0;
protected:
	std::thread m_pThread;
	std::atomic_bool m_bReady = false;
};

template <typename Result>
class CTaskItem : public ITaskItem {
public:
	CTaskItem(std::future<Result>& futrue) {
		auto& refB = m_bReady;
		auto& refV = m_pValue;
		m_pThread = std::thread(
			[&refB, &refV, futrue = std::move(futrue)]() mutable {
				refV = futrue.get();
				refB = true;
			}
		);
	}
	~CTaskItem() {
		if (m_pThread.joinable())
			m_pThread.join();
	}
	//continue in main thread
	//if you wanna capture some ref with recursion, PLEASE NOT TRUST lambda capture
	//First of function arg must same as Result
	template <typename F, typename... Args>
	CTaskItem* ContinueWith(F&& func, Args&&... args) {
		m_pContinue = std::bind(std::forward<F>(func), std::placeholders::_1, std::forward<Args>(args)...);
		return this;
	}
	//start task
	virtual CTaskItem* Start() override {
		m_pThread.detach();
		return this;
	}
	//is ready
	virtual bool IsReady() override {
		return m_bReady;
	}
	//get caller
	virtual std::function<void()> GetCaller() override {
		return std::bind(m_pContinue, m_pValue);
	}
private:
	Result m_pValue;
	std::function<void(Result&)> m_pContinue = nullptr;
};

class CTaskManager {
public:
	//add a new async task
	template <typename Result>
	CTaskItem<Result>* Add(std::future<Result>& func) {
		CTaskItem<Result>* item = new CTaskItem<Result>(func);
		m_aryPending.push_back(item);
		return item;
	}
	//has this task?
	bool Has(ITaskItem* check);
	//check per frame
	void CheckAll();
private:
	std::list<ITaskItem*> m_aryList;
	std::list<ITaskItem*> m_aryPending;
};
CTaskManager* GetTaskManager();
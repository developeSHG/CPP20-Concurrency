#include <iostream>
using namespace std;
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <concepts>
#include <coroutine>
#include <array>
#include <set>
#include <numbers>
#include <numeric>
#include <bit>
#include <bitset>
#include <chrono>
#include <format>
#include <source_location>
#include <functional>
#include <atomic>
#include <thread>
#include <semaphore>

// 오늘의 주제 : semaphore

using namespace std;

counting_semaphore<1> sem(0);
vector<int> vec;

void PrepareWork()
{
	vec.insert(vec.end(), { 1, 2, 3, 4, 5 });

	this_thread::sleep_for(1s); // 1초 대기

	cout << "데이터 준비됨!" << endl;
	sem.release();
}

void CompleteWork()
{
	cout << "데이터 기다리는중..." << endl;
	sem.acquire(); // release로 양수가 될 때까지 대기중.

	cout << "데이터 출력중..." << endl;

	for (auto n : vec)
		cout << n << endl;
}

int main()
{
	// 실무에선 잘 사용안하지만,
	// 면접에선 크리티컬 섹션과 더불어 세마포어에 대해 종종 물어봄.
	

	// - mutex
	// 화장실 키 1개
	// [키] 
	// 한 번에 한 명이 들어가고, 한 명이 잠구는 역할
	// 끝나면 문을 열어 나오고, 그 후 다시 다음사람이 들어가고~
	// 이렇게 상호배타적인 특성을 보장하기 위해서 사용하는게 뮤텍스.

	// - semaphore
	// 화장실 키 n개
	// [키][키][키]
	// 식당 외부에 화장실이 있는데, 아무나 가서 사용하면 안되니
	// 내부에 식당 키를 배치해, 키를 하나씩 들고가 문을 열어 사용하게끔.
	// 특성은 최대 카운트가 1이 아니라 내가 지정할 수 있는 n개

	// C++20에 추가된 것
	counting_semaphore<3>; // 최대 개수를 지정해줄 수 있다.
	//binary_semaphore = counting_semaphore<1>; // 1은 거의 mutex랑 유사

	// semaphore 함수
	{
		// max(); // 최대 카운터값
		// acquire(); // counter를 1만큼 감소 (counter = 0이면 양수 될때까지 대기)
		// release(upd = 1) // counter를 upd만큼 증가 (대기하던 쓰레드가 있으면 풀어줌)
		// try_acquire(); // 카운터가 0보다 크면, 1 감소
		// try_acquire_for(relTime); // 최대 relTime만큼 counter를 1 감소하려 시도
		// try_acquire_until(absTime); // 최대 absTime까지 counter를 1 감소하려 시도
	}

	thread t1(PrepareWork);
	thread t2(CompleteWork);

	t1.join();
	t2.join();

	// 결과
	{
		// 데이터 기다리는중
		// 데이터 준비됨
		// 데이터 출력중
		// 1
		// 2
		// 3
		// 4
		// 5
	}

	return 0;
}
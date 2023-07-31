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
#include <thread>
#include <mutex>

// 오늘의 주제 : jthread

mutex mut;

void ThreadMain()
{
	mut.lock();

	while (true)
	{
		// 오래 걸리는 작업
	}

	mut.unlock();
}

int main()
{
	thread t1(ThreadMain); // 스레드 실행
	
	t1.join(); // 스레드가 종료될 때까지 대기. 
	// 만약 이 코드가 없으면 스레드 소멸자 함수에서 joinable 상태일 때(아직 끝나지 않았을 때), 소멸자를 꺼버린다. (Destroy)
	
	// 죽여버린다고 가정할 때, ThreadMain에 뮤텍스가 있는 상태에서 Lock이 잡혀있다고 가정해보자.
	// 원래는 unlock을 나중에 해주겠지만, 스레드에서 오래 걸리는 작업을 수행하고 있는데
	// 만약에 외부에서 함수를 이용해 스레드를 강제로 종료했다고 하면, lock은 영영 풀어주지않고, unlock을 못하니
	// 전체 시스템에 악영향을 줄 수 밖에 없다.
	// 그렇기 때문에, 강제로 다른 스레드를 꺼버리는 작업은 굉장히 위험한 작업이다.
	// 그런 걸 방지하기 위해 보통 join을 한다거나 작업이 끝나기를 기다린 다음에 다음 작업으로 넘어간다.
	// 근데 스레드가 한 개가 있으면 괜찮다하더라도, 많이 있다고 생각해보면

	vector<thread> threads;
	for (int i = 0; i < 5; ++i)
		threads.push_back(thread(ThreadMain));
	
	for (int i = 0; i < 5; ++i)
		threads[i].join();

	// 위와 같은 식으로 순회를 하면서 일일이 join을 해야하는 지저분한 코드가 만들어진다.
	// 가독성과 비효율적 때문인지 자동으로 join이 되는 thread를 C++20에서 추가되었다.

	// Joining Thgread
	jthread jt(ThreadMain);

	// 알아서 소멸이 될 때, join을 해준다.
	//~jthread() {
		//_Try_cancle_and_join();
	//}
	//void _Try_cancle_and_join() noexcept {
		//if (_Impl.joinable()) // join을 할 수 있는 상태면
		//{
			//_Ssource.request_stop(); // 강제로 끄지않고, 멈춰달라 요청
			//_Impl.join(); 
		//}
	//}
	
	// 이왕이면, 일반 thread를 사용하는 것보다
	// 일일이 join을 하나하나 만드는 경우가 지저분하니
	// jthread를 활용하는 경우가 좋다. (편의성, 안전성)
}
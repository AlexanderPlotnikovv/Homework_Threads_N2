#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

void sumvector(std::vector<int>& vec1, std::vector<int>& vec2, std::vector<int>& vec, int i1, int j1) // допустим мы разбиваем вектор на j1 кусков и вызываем функцию на i1 блоке (например, если разбить вектор размером 1000 на 4 блока то обработка 2 блока обработает элементы [250;500)
{
	for (int i = vec.size()*(i1-1)/j1; i != vec.size()*i1/j1; ++i)
	{
		vec[i] = (vec1[i] + vec2[i]);
	}
}

double do_with_threads(int count, int vectorsize)
{
	std::vector<int> vec1;
	std::vector<int> vec2;
	std::vector<int> vec(vectorsize);
	for (int i = 0; i < vectorsize; ++i)
	{
		vec1.push_back(i);
		vec2.push_back(i);
	}
	auto start = std::chrono::steady_clock::now();
	std::vector<std::thread> threads;
	for (int i = 0; i < count; ++i)
	{
		threads.push_back(std::thread(sumvector, std::ref(vec1), std::ref(vec2), std::ref(vec), i+1, count));
	}
	for (auto& it : threads)
	{
		it.join();
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	return elapsed_seconds.count();
}

int main()
{
	std::cout << "Quantity of cores: " << std::thread::hardware_concurrency() << std::endl << std::endl;
	std::cout << "\t";
	for (int i = 1000; i <= 1000000; i *= 10)
	{
		std::cout << i << "\t" << "\t";
	}
	std::cout << std::endl;
	for (int i = 1; i <= (1 << 4); i *= 2)
	{
		std::cout << i << " cores: ";
		for (int j = 1000; j <= 1000000; j *= 10)
		{
			std::cout << do_with_threads(i, j) << "s" << "\t";
		}
		std::cout << std::endl;
	}
	return 0;
}
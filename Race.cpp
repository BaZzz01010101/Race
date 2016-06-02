#include <sstream>
#include <iostream>
#include <ctime>

#define MAX_CITY_COUNT 100000

struct City
{
  long long w;
  long long g;
  long long g_;
};

struct Data
{
  int n;
  int k;
  City cities[MAX_CITY_COUNT];
};

int testFromPos(int start, Data & data)
{
  long long * w = &data.cities[start].w;
  long long * g = &data.cities[start].g;
  long long * g_ = &data.cities[start].g_;
  const int wStep = sizeof(data.cities[0]) / sizeof(*w);
  const int gStep = sizeof(data.cities[0]) / sizeof(*g);
  const int g_Step = sizeof(data.cities[0]) / sizeof(*g_);
  int finish = data.n - 1;
  long long gifts = data.k;

  long long gas = 0;
  long long backwardGas = 0;
  long long minBackwardGas = 0;
  int i;
  int maxL = 1;

  for (i = start; i < finish; i++)
  {
    gas += *g - *w;

    if (gas >= 0)
      *g_ = 0;
    else if (gifts >= -gas)
    {
      *g_ = -gas;
      gifts -= -gas;
      gas = 0;
    }
    else
      return maxL;

    if (i > start)
    {
      backwardGas += *(w - wStep) - *g - *g_;

      if (backwardGas < minBackwardGas)
        minBackwardGas = backwardGas;
    }

    if (gifts + *(g + gStep) >= backwardGas + *w - minBackwardGas)
      maxL = i + 1 - start + 1;

    w += wStep;
    g += gStep;
    g_ += g_Step;
  }
  
  return maxL;
}

void init(Data * data)
{
  std::cin >> data->n >> data->k;

  for (int i = 0; i < data->n - 1; i++)
    std::cin >> data->cities[i].w;

  for (int i = 0; i < data->n; i++)
    std::cin >> data->cities[i].g;
}

int test1(Data & data)
{
  int maxL = 1;

  for (int i = 0; i < data.n; i++)
  {
    int l = testFromPos(i, data);

    if (l > maxL)
      maxL = l;
  }

  return maxL;
}

int test()
{
  static Data data;
  init(&data);
  return test1(data);
}

int main()
{
  std::istringstream iss;

  std::cout << "Test 1: ";
  iss.str("2 0\r\n"
          "2\r\n"
          "1 1\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 2: ";
  iss.str("3 1\r\n"
          "2 2\r\n"
          "1 1 1\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 3: ";
  iss.str("3 0 "
          "1 2 "
          "1 1 1 ");  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 2 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 4: ";
  iss.str("4 4\r\n"
          "2 2 2\r\n"
          "1 1 1 1\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 4 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 5: ";
  iss.str("8 5\r\n"
          "2 2 2 3 7 3 1\r\n"
          "1 3 1 5 4 0 2 5\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 6: ";
  iss.str("15 10\r\n"
          "4 1 30 23 32 37 26 18 41 38 6 38 23 40\r\n"
          "5 7 6 7 8 15 1 11 16 7 3 20 20 9 1\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 3 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 7: ";
  iss.str("20 100\r\n"
          "2 28 35 16 40 25 50 36 35 47 25 5 34 26 22 49 26 7 10\r\n"
          "12 12 13 8 8 19 18 9 0 18 13 20 9 10 0 7 5 6 15 2\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 8: ";
  iss.str("50 100\r\n"
          "21 49 12 29 5 28 1 18 8 32 9 46 36 40 10 44 9 12 22 34 45 24 14 39 34 29 50 36 31 7 14 7 33 16 20 44 36 38 21 1 36 6 26 9 39 18 40 32 49\r\n"
          "16 8 1 19 3 1 16 13 11 15 18 16 16 7 15 12 9 14 20 4 6 13 2 13 2 5 20 16 12 4 3 14 20 20 3 19 15 13 10 6 4 7 0 8 4 5 9 12 1 16\r\n");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 10 ? "complete" : "failed") << "\r\n";

  srand(2389);

  int n = 100000;
  int k = 100000000;
  std::stringstream oss;
  oss << n << " " << k << "\r\n";

  for (int i = 0; i < n - 1; i++)
  {
    long long a = 1;
    long long b = rand() % 100000;
    oss << a * b << (i < n - 2 ? " " : "\r\n");
  }

  for (int i = 0; i < n; i++)
  {
    long long a = 1;
    long long b = rand() % 500;
    long long c = 1;// rand() % 4;
    oss << a * b * c << (i < n - 1 ? " " : "\r\n");
  }

  iss.str(oss.str());
  std::cin.rdbuf(iss.rdbuf());

  clock_t t = clock();
  std::cout << "Last test result: " << test() << "\r\n";
  t = clock() - t;
  std::cout << t << " ms";
  // 668
  getchar();
}

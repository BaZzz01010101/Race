#include <sstream>
#include <iostream>
#include <ctime>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define MAX_CITY_COUNT 100000

struct City
{
  long long w;
  long long g;
  long long gf;
  long long bg;
  long long minbg;
};

struct Data
{
  int n;
  int k;
  City cities[MAX_CITY_COUNT];
};

int testFromPos(int start, Data & data)
{
  City * begin = data.cities + start;
  City * end = data.cities + data.n;
  City * head = begin;
  City * tail = begin;
  City * finish = begin;
  City * lastMinBackwardGasCity = begin;

  int maxL = 1;
  long long gifts = data.k;
  long long gas = 0;
  long long backwardGas = 0;
  long long minBackwardGas = 0;
  begin->bg = 0;
  begin->minbg = 0;

  while (1)
  {
    while (head < end - 1)
    {
      long long g = head->g;
      long long w = head->w;
      long long gf = 0;

      gas += g - w;

      if (gas >= 0)
        head->gf = gf = 0;
      else if (gifts >= -gas)
      {
        head->gf = gf = -gas;
        gifts -= -gas;
        gas = 0;
      }
      else
        break;

      if (head > begin)
      {
        backwardGas += (head - 1)->w - g - gf;

        if (backwardGas <= minBackwardGas)
        {
          minBackwardGas = backwardGas;
          lastMinBackwardGasCity = head;
        }

        head->bg = backwardGas;
        head->minbg = minBackwardGas;
      }

      // minBackwardGas <= 0
      if (gifts + (head + 1)->g >= backwardGas - minBackwardGas + w)
      {
        maxL = max(maxL, head - tail + 2);
        finish = head + 1;
      }

      head++;
    }

    while (tail <= lastMinBackwardGasCity && tail < head)
    {
      if (tail->gf)
      {
        gifts += tail->gf;
        tail++;

        // проверка возможности возврата с городов после текущего финиша и до головы, 
        // с учетом увеличения кол-ва подарков после перемещения хвоста

        for (const City * city = head - 1; city > max(tail, finish); city--)
        {
          if (gifts + (city + 1)->g >= city->bg - city->minbg + city->w)
          {
            maxL = max(maxL, city - tail + 2);
            finish = head + 1;
            break;
          }
        }
      }
      else
        tail++;

      if (head == end - 1)
      {
        if (maxL >= head - tail + 1)
          break;
      }
      else if (gifts >= -gas)
      {
        head->gf = -gas;
        gifts -= -gas;
        gas = 0;
        head++;
        break;
      }

    }

    if (tail > lastMinBackwardGasCity)
    {
      // если хвост покинул город, имеющий минимальное количество топлива на обратном пути, то далее 
      // необходим пересчет для каждого города city->bg и city->minbg, а также backwardGas и minBackwardGas
      backwardGas = 0;
      minBackwardGas = 0;
      lastMinBackwardGasCity = tail;

      for (City * city = tail + 1; city <= min(head, end - 2); city++)
      {
        backwardGas += (city - 1)->w - city->g - city->gf;

        if (backwardGas <= minBackwardGas)
        {
          minBackwardGas = backwardGas;
          lastMinBackwardGasCity = city;
        }

        head->bg = backwardGas;
        head->minbg = minBackwardGas;
      }
    }

    if(tail == head || (data.k < -gas && head - tail + 2 <= maxL))
    {
      // если всего исходного кол-ва подарков недостаточно для преодоления текущей дороги
      // и подтягивать хвост более нет смысла, то сразу переводим старт на город, 
      // следующий за проблемной дорогой

      while (++head < end && head->w > head->g);

      begin = head;
      head = begin;
      tail = begin;
      finish = begin;
      lastMinBackwardGasCity = begin;
      gifts = data.k;
      gas = 0;
      backwardGas = 0;
      minBackwardGas = 0;
      begin->bg = 0;
      begin->minbg = 0;
    }

    if (maxL >= end - tail)
      break;
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

int test()
{
  static Data data;
  init(&data);
  int maxL = 1;

  //for (int i = 0; i < data.n; i++)
  {
    //int l = testFromPos(i, data);
    int l = testFromPos(0, data);

    if (l > maxL)
      maxL = l;

    //if (maxL >= data.n - i)
    //  break;
  }

  return maxL;
}

int main()
{
  std::istringstream iss;

  std::cout << "Test 1: ";
  iss.str("2 0 "
          "2 "
          "1 1 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 2: ";
  iss.str("3 1 "
          "2 2 "
          "1 1 1 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 3: ";
  iss.str("3 0 "
          "1 2 "
          "1 1 1 ");  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 2 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 4: ";
  iss.str("4 4 "
          "2 2 2 "
          "1 1 1 1 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 4 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 5: ";
  iss.str("8 5 "
          "2 2 2 3 7 3 1 "
          "1 3 1 5 4 0 2 5 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 6: ";
  iss.str("15 10 "
          "4 1 30 23 32 37 26 18 41 38 6 38 23 40 "
          "5 7 6 7 8 15 1 11 16 7 3 20 20 9 1 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 3 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 7: ";
  iss.str("20 100 "
          "2 28 35 16 40 25 50 36 35 47 25 5 34 26 22 49 26 7 10 "
          "12 12 13 8 8 19 18 9 0 18 13 20 9 10 0 7 5 6 15 2 ");
  std::cin.rdbuf(iss.rdbuf());
  std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  std::cout << "Test 8: ";
  iss.str("50 100 "
          "21 49 12 29 5 28 1 18 8 32 9 46 36 40 10 44 9 12 22 34 45 24 14 39 34 29 50 36 31 7 14 7 33 16 20 44 36 38 21 1 36 6 26 9 39 18 40 32 49 "
          "16 8 1 19 3 1 16 13 11 15 18 16 16 7 15 12 9 14 20 4 6 13 2 13 2 5 20 16 12 4 3 14 20 20 3 19 15 13 10 6 4 7 0 8 4 5 9 12 1 16 ");
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
  std::cout << "Time: " << t << " ms";

  //Last test result : 6337
  //Time : 5526 ms  

  //Last test result : 6337
  //Time : 5625 ms  

  //Last test result : 6337
  //Time : 4994 ms

  getchar();
}

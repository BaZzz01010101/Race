#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define MAX_CITY_COUNT 100000

struct City
{
  long long g;
  long long w;
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

struct Result
{
  City * start;
  City * finish;
  int length() { return finish - start + 1; }
};

bool testInterval(int start, int finish, Data & data)
{
  bool retVal = false;
  static long long g_[100000];
  long long gas = 0;
  long long gifts = data.k;
  int i;

  for (i = start; i < finish; i++)
  {
    long long & w = data.cities[i].w;
    long long & g = data.cities[i].g;
    long long & g_ = data.cities[i].gf;

    gas += g - w;

    if (gas >= 0)
      g_ = 0;
    else if (gifts >= -gas)
    {
      g_ = -gas;
      gifts -= -gas;
      gas = 0;
    }
    else
      return false;
  }

  if (i == finish)
  {
    data.cities[finish].gf = gifts;

    gas = 0;

    for (i = finish; i > start; i--)
    {
      long long & w = data.cities[i].w;
      long long & g = data.cities[i].g;
      long long & g_ = data.cities[i].gf;

      gas += g + g_ - data.cities[i - 1].w;

      if (gas < 0)
        return false;
    }

    return true;
  }

  return false;
}

int trueLen;
int trueStart;
int seed = -1;

void printData(Data & data, City * tail, City * head, City * start, City * finish, City * firstMinBackwardGasCity, City * lastMinBackwardGasCity, long long gas, long long nextGas, long long gifts, long long minBackwardGas, long long backwardGas)
{
  if(seed != -1)
    return;

  std::system("cls");
  std::cout << "True interval: " << trueStart << " - " << trueStart + trueLen - 1 << "\r\n";
  std::cout << "Gas: " << gas << "\r\n";
  std::cout << "NextGas: " << nextGas << "\r\n";
  std::cout << "Gifts: " << gifts << "\r\n";
  std::cout << "BackwardGas: " << backwardGas << "\r\n";
  std::cout << "MinBackwardGas: " << minBackwardGas << "\r\n";

  std::cout << "\r\n      ";
  for (int i = 0; i < data.n; i++)
  {
    std::cout << ((i == tail - data.cities) ? "[" : " ");
    std::cout << i;
    std::cout << ((i == head - data.cities) ? "]" : " ");
  }

  std::cout << "\r\n     ";
  for (int i = 0; i < data.n; i++)
  {
    std::cout << " ";
    std::cout << ((i == firstMinBackwardGasCity - data.cities) ? "<" : " ");
    std::cout << ((i == lastMinBackwardGasCity - data.cities) ? ">" : " ");
  }

  std::cout << "\r\nW:   ";
  for (int i = 0; i < data.n - 1; i++)
    std::cout << std::setw(3) << data.cities[i].w;

  std::cout << "\r\nG:   ";
  for (int i = 0; i < data.n; i++)
    std::cout << std::setw(3) << data.cities[i].g;

  std::cout << "\r\nGF:  ";
  for (int i = 0; i < data.n; i++)
    std::cout << std::setw(3) << data.cities[i].gf;

  std::cout << "\r\nBG:  ";
  for (int i = 0; i < data.n; i++)
    std::cout << std::setw(3) << data.cities[i].bg;

  std::cout << "\r\nMBG: ";
  for (int i = 0; i < data.n; i++)
    std::cout << std::setw(3) << data.cities[i].minbg;

  std::cout << "\r\n     ";
  for (int i = 0; i < data.n; i++)
  {
    std::cout << " ";
    std::cout << ((i == start - data.cities) ? "s" : " ");
    std::cout << ((i == finish - data.cities) ? "f" : " ");
  }

  std::cout << "\r\n";
  getchar();
}

Result testData(Data & data)
{
  City * begin = data.cities;
  City * end = data.cities + data.n;
  City * head = begin;
  City * tail = begin;
  City * start = begin;
  City * finish = begin;
  City * lastMinBackwardGasCity = begin;
  City * firstMinBackwardGasCity = begin;

  int maxL = 1;
  long long gifts = data.k;
  long long gas = 0;
  long long nextGas = 0;
  long long backwardGas = 0;
  long long minBackwardGas = 0;

  while (1)
  {
    printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

    // ==============
    // PUSH HEAD LOOP
    // ==============
    while (head < end - 1)
    {
      long long g = head->g;
      long long w = head->w;
      long long gf = 0;

      nextGas = gas + g - w;

      if (nextGas >= 0)
      {
        gas = nextGas;
        head->gf = gf = 0;
        nextGas = 0;
      }
      else if (gifts >= -nextGas)
      {
        head->gf = gf = -nextGas;
        gifts += nextGas;
        gas = 0;
        nextGas = 0;
      }
      else
        break;

      if (head > begin)
      {
        backwardGas += (head - 1)->w - g - gf;

        if (backwardGas < minBackwardGas)
        {
          minBackwardGas = backwardGas;
          firstMinBackwardGasCity = head;
          lastMinBackwardGasCity = head;
        }
        else if (backwardGas == minBackwardGas)
          lastMinBackwardGasCity = head;
      }

      head->bg = backwardGas;
      head->minbg = minBackwardGas;
      head++;

      // minBackwardGas <= 0
      if (gifts + head->g >= backwardGas - minBackwardGas + (head - 1)->w)
      {
        int l = head - tail + 1;

        if (l > maxL)
        {
          maxL = l;
          start = tail;
          finish = head;
        }
      }
    }

    printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

    // ==============
    // PULL TAIL LOOP
    // ==============
    while (tail < head)
    {
      // если всего исходного кол-ва подарков недостаточно для преодоления текущей дороги
      // и подтягивать хвост более нет смысла, или все гифты собраны, то выходим
      if (data.k < -nextGas && head - tail + 1 <= maxL)
        break;

      bool needRecheck = false;

      if (tail->gf)
      {
        needRecheck = true;
        gifts += tail->gf;
        tail->gf = 0;
        tail++;

        printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
      }
      else
      {
        tail++;
        long long gas_ = 0;

        for (City * city = tail; city < head; city++)
        {
          gas_ += city->g + city->gf - city->w;

          if (gas_ < 0)
          {
            gifts += gas_;

            if (city->gf)
            {
              city->gf -= gas_;
              gas_ = 0;
              break;
            }
            else
            {
              city->gf = -gas_;
              gas_ = 0;
            }
          }
        }

        if(gifts < 0)
          printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

        // возвращаем голову назад, компенсируя отрицательное кол-во подарков
        while (gifts < 0)
        {
          head--;
          gas -= head->g + head->gf - head->w;
          gifts += head->gf;
          backwardGas = (head > begin) ? (head - 1)->bg : 0;
          head->gf = 0;
          head->bg = 0;
          head->minbg = 0;
        }

        nextGas = gas + head->g - head->w;

        if (head <= firstMinBackwardGasCity)
        {
          printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
          // если голова вернулась раньше первого города, с минимальным количеством топлива на обратном пути, то далее 
          // необходим пересчет для каждого города city->bg и city->minbg, а также backwardGas и minBackwardGas
          needRecheck = true;
          backwardGas = 0;
          minBackwardGas = 0;
          firstMinBackwardGasCity = tail;
          lastMinBackwardGasCity = tail;
          tail->bg = 0;
          tail->minbg = 0;

          for (City * city = tail + 1; city < head; city++)
          {
            backwardGas += (city - 1)->w - city->g - city->gf;

            if (backwardGas < minBackwardGas)
            {
              minBackwardGas = backwardGas;
              firstMinBackwardGasCity = city;
              lastMinBackwardGasCity = city;
            }
            else if (backwardGas == minBackwardGas)
              lastMinBackwardGasCity = city;

            city->bg = backwardGas;
            city->minbg = minBackwardGas;
          }
        }
        else if (head <= lastMinBackwardGasCity)
        {
          printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
          while ((--lastMinBackwardGasCity)->bg != minBackwardGas);
        }

        printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
      }

      if (tail == head)
        break;

      if (tail > lastMinBackwardGasCity)
      {
        // если хвост покинул последний город, имеющий минимальное количество топлива на обратном пути, то далее 
        // необходим пересчет для каждого города city->bg и city->minbg, а также backwardGas и minBackwardGas
        needRecheck = true;
        backwardGas = 0;
        minBackwardGas = 0;
        firstMinBackwardGasCity = tail;
        lastMinBackwardGasCity = tail;
        tail->bg = backwardGas;
        tail->minbg = minBackwardGas;
        printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

        for (City * city = tail + 1; city < head; city++)
        {
          backwardGas += (city - 1)->w - city->g - city->gf;

          if (backwardGas < minBackwardGas)
          {
            minBackwardGas = backwardGas;
            firstMinBackwardGasCity = city;
            lastMinBackwardGasCity = city;
          }
          else if (backwardGas == minBackwardGas)
            lastMinBackwardGasCity = city;

          city->bg = backwardGas;
          city->minbg = minBackwardGas;
        }

        printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
      }
      else if (tail > firstMinBackwardGasCity)
      {
        while ((++firstMinBackwardGasCity)->bg != minBackwardGas);
      }



      if(needRecheck)
      {
        // проверка возможности возврата с городов после текущего финиша и до головы, 
        // с учетом увеличения кол-ва подарков после перемещения хвоста
        long long gifts_ = gifts;

        for (City * city = head - 1; city >= max(tail, head - maxL -1); city--)
        {
          if (gifts_ + (city + 1)->g >= city->bg - city->minbg + city->w)
          {
            if (maxL < city - tail + 2)
            {
              maxL = city - tail + 2;
              start = tail;
              finish = city + 1;
              printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
              break;
            }
          }

          gifts_ += city->gf;
        }
      }

      printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

      if (head == end - 1)
      {
        if (maxL >= head - tail + 1)
          break;
      }
      else if (gifts >= -nextGas)
      {
        head->gf = -nextGas;
        gifts -= -nextGas;
        gas = 0;
        nextGas = 0;

// ***
        backwardGas += (head - 1)->w - head->g - head->gf;

        if (backwardGas < minBackwardGas)
        {
          minBackwardGas = backwardGas;
          firstMinBackwardGasCity = head;
          lastMinBackwardGasCity = head;
        }
        else if (backwardGas == minBackwardGas)
          lastMinBackwardGasCity = head;

        head->bg = backwardGas;
        head->minbg = minBackwardGas;

        printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);

        // minBackwardGas <= 0
        if (gifts + (head + 1)->g >= backwardGas - minBackwardGas + head->w)
        {
          if (maxL < head - tail + 2)
          {
            maxL = head - tail + 2;
            start = tail;
            finish = head + 1;
            printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
          }
        }
// ***

        head++;
        break;
      }

    }

    if (data.k < -nextGas && head - tail + 1 <= maxL)
    {
      head++;
      tail = head;
    }

    if (tail == head)
    {
      begin = head;
      tail = head;
      firstMinBackwardGasCity = head;
      lastMinBackwardGasCity = head;
      gifts = data.k;
      gas = 0;
      nextGas = 0;
      backwardGas = 0;
      minBackwardGas = 0;

      printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
    }

    if (maxL >= end - tail)
    {
      printData(data, tail, head, start, finish, firstMinBackwardGasCity, lastMinBackwardGasCity, gas, nextGas, gifts, minBackwardGas, backwardGas);
      break;
    }
  }

  return { start, finish };
}

void init(Data * data)
{
  std::cin >> data->n >> data->k;

  for (int i = 0; i < data->n - 1; i++)
    std::cin >> data->cities[i].w;

  for (int i = 0; i < data->n; i++)
    std::cin >> data->cities[i].g;

}

void clean(Data * data)
{
  for (int i = 0; i < data->n; i++)
  {
    data->cities[i].gf = 0;
    data->cities[i].bg = 0;
    data->cities[i].minbg = 0;
  }
}

int test()
{
  static Data data;
  init(&data);
  clean(&data);

  for (trueLen = data.n; trueLen > 0; trueLen--)
  {
    for (trueStart = 0; trueStart <= data.n - trueLen; trueStart++)
    {
      if (testInterval(trueStart, trueStart + trueLen - 1, data))
        goto end;
    }
  }
end:

  clean(&data);

  Result result = testData(data);

  if (trueLen != result.finish - result.start + 1)
  {
    std::cout << "Seed: " << seed;
    std::cout << " Control result: " << trueLen << "(" << trueStart << "-" << trueStart + trueLen - 1 << ") ";
    std::cout << " ERROR: " << result.finish - result.start + 1 << "(" << result.start - data.cities << "-" << result.finish - data.cities << ") ";
    getchar();
  }


  //else
  //  std::cout << " OK: " << result.finish - result.start + 1 << "(" << result.start - data.cities << "-" << result.finish - data.cities << ") ";

  //if (!testInterval(result.start - data.cities, result.finish - data.cities, data))
  //  std::cout << " WRONG: (" << result.start - data.cities << "-" << result.finish - data.cities << ") ";

  return result.finish - result.start + 1;
}

int main()
{
  std::istringstream iss;

  //std::cout << "Test 1: ";
  //iss.str("2 0 "
  //        "2 "
  //        "1 1 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 2: ";
  //iss.str("3 1 "
  //        "2 2 "
  //        "1 1 1 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 1 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 3: ";
  //iss.str("3 0 "
  //        "1 2 "
  //        "1 1 1 ");  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 2 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 4: ";
  //iss.str("4 4 "
  //        "2 2 2 "
  //        "1 1 1 1 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 4 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 5: ";
  //iss.str("8 5 "
  //        "2 2 2 3 7 3 1 "
  //        "1 3 1 5 4 0 2 5 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 6: ";
  //iss.str("15 10 "
  //        "4 1 30 23 32 37 26 18 41 38 6 38 23 40 "
  //        "5 7 6 7 8 15 1 11 16 7 3 20 20 9 1 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 3 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 7: ";
  //iss.str("20 100 "
  //        "2 28 35 16 40 25 50 36 35 47 25 5 34 26 22 49 26 7 10 "
  //        "12 12 13 8 8 19 18 9 0 18 13 20 9 10 0 7 5 6 15 2 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 7 ? "complete" : "failed") << "\r\n";

  //std::cout << "Test 8: ";
  //iss.str("50 100 "
  //        "21 49 12 29 5 28 1 18 8 32 9 46 36 40 10 44 9 12 22 34 45 24 14 39 34 29 50 36 31 7 14 7 33 16 20 44 36 38 21 1 36 6 26 9 39 18 40 32 49 "
  //        "16 8 1 19 3 1 16 13 11 15 18 16 16 7 15 12 9 14 20 4 6 13 2 13 2 5 20 16 12 4 3 14 20 20 3 19 15 13 10 6 4 7 0 8 4 5 9 12 1 16 ");
  //std::cin.rdbuf(iss.rdbuf());
  //std::cout << (test() == 10 ? "complete" : "failed") << "\r\n";

  //208 2010
  //Last test result : 1803  
  srand(9873);

  for (seed = 0; seed < 1000; seed++)
  {
    if (!(seed % 10))
      std::cout << "\rTests: " << seed;

    srand(seed);
    rand();
    int n = 2 + rand() % 1000;
    int k = rand() % 1000;
    std::stringstream oss;
    oss << n << " " << k << " ";

    for (int i = 0; i < n - 1; i++)
    {
      long long a = rand() % 20;
      long long b = 1;// rand() % 3000;
      oss << a * b << " ";
    }

    for (int i = 0; i < n; i++)
    {
      long long a = rand() % 10;
      long long b = 1;// rand() % 2870;
      long long c = 1;// rand() % 4;
      oss << a * b * c << " ";
    }

    iss.str(oss.str());
    std::cin.rdbuf(iss.rdbuf());

    int len = test();
    //std::cout << " Result: " << len << "\r\n";
  }

/*
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
*/
  //clock_t t = clock();
  //std::cout << "Last test result: " << test() << "\r\n";
  //t = clock() - t;
  //std::cout << "Time: " << t << " ms";

  //Last test result : 6337
  //Time : 5526 ms  

  //Last test result : 6337
  //Time : 5625 ms  

  //Last test result : 6337
  //Time : 4994 ms

  std::cout << "\r\nPress 'Enter' to exit\r\n";
  getchar();
}

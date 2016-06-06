#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string.h>

#define LLONG_MIN (-9223372036854775807LL - 1)  // minimum signed long long int value
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

class Data
{
public:
  City cities[MAX_CITY_COUNT];
  int n;
  int k;
  long long gifts;
  long long gas;
  long long nextGas;
  int maxL;
  City * end;
  City * head;
  City * tail;
  City * start;
  City * finish;
  City * lastMinBackwardGasCity;
  City * firstMinBackwardGasCity;

  void init()
  {
    memset(cities, 0, sizeof(cities));

    std::cin >> n >> k;

    for (int i = 0; i < n - 1; i++)
      std::cin >> cities[i].w;

    for (int i = 0; i < n; i++)
      std::cin >> cities[i].g;

    end = cities + n;
    head = cities;
    tail = cities;
    start = cities;
    finish = cities;
    lastMinBackwardGasCity = cities;
    firstMinBackwardGasCity = cities;

    gifts = k;
    gas = 0;
    nextGas = 0;
    maxL = 1;
  }
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
int seed = 0;

void printData(Data & data)
{
  if(seed != -1)
    return;

  std::system("cls");
  std::cout << "True interval: " << trueStart << " - " << trueStart + trueLen - 1 << "\r\n";
  std::cout << "data.gas: " << data.gas << "\r\n";
  std::cout << "data.nextGas: " << data.nextGas << "\r\n";
  std::cout << "data.gifts: " << data.gifts << "\r\n";

  std::cout << "\r\n      ";
  for (int i = 0; i < data.n; i++)
  {
    std::cout << ((i == data.tail - data.cities) ? "[" : " ");
    std::cout << i;
    std::cout << ((i == data.head - data.cities) ? "]" : " ");
  }

  std::cout << "\r\n     ";
  for (int i = 0; i < data.n; i++)
  {
    std::cout << " ";
    std::cout << ((i == data.firstMinBackwardGasCity - data.cities) ? "<" : " ");
    std::cout << ((i == data.lastMinBackwardGasCity - data.cities) ? ">" : " ");
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
    std::cout << ((i == data.start - data.cities) ? "s" : " ");
    std::cout << ((i == data.finish - data.cities) ? "f" : " ");
  }

  std::cout << "\r\n";
  getchar();
}

// пересчет значений bg и minbg в конкретном городе, основываясь на значениях предыдущего города
void backCount(City * city, Data & data)
{
  if (city == data.tail)
  {
    city->bg = city->minbg = 0;
    data.firstMinBackwardGasCity = data.lastMinBackwardGasCity = data.tail;
  }
  else
  {
    city->bg = (city - 1)->bg + (city - 1)->w - city->g - city->gf;

    if (city->bg < (city - 1)->minbg)
    {
      city->minbg = city->bg;
      data.firstMinBackwardGasCity = city;
      data.lastMinBackwardGasCity = city;
    }
    else if (city->bg == (city - 1)->minbg)
    {
      city->minbg = city->bg;
      data.lastMinBackwardGasCity = city;
    }
    else
      city->minbg = (city - 1)->minbg;
  }
};

// пересчет значений bg и minbg во всех городах от хвоста до головы
void backRecountAll(Data & data)
{
  for (City * city = data.tail; city < data.head; city++)
    backCount(city, data);

  printData(data);
};

// проверка возможности возврата к хвосту из конкретного города
bool backCheck(City * city, long long gifts, Data & data)
{
  if (gifts + city->g >= (city - 1)->bg - (city - 1)->minbg + (city - 1)->w)
  {
    int l = city - data.tail + 1;

    if (l > data.maxL)
    {
      data.maxL = l;
      data.start = data.tail;
      data.finish = city;
      return true;
    }
  }

  return false;
};

// проверка возможности возврата к хвосту со всех городов между хвостом и головой 
void backCheckAll(Data & data)
{
  printData(data);

  long long gifts_ = data.gifts;

  for (City * city = data.head; city > max(data.tail, data.tail + data.maxL - 1); city--)
  {
    gifts_ += city->gf;

    if (backCheck(city, gifts_, data))
      break;
  }
};


Result testData(Data & data)
{
  while (1)
  {
    printData(data);

    // ==============
    // PUSH data.head LOOP
    // ==============
    while (data.head < data.end - 1)
    {
      long long g = data.head->g;
      long long w = data.head->w;
      long long gf = 0;

      data.nextGas = (data.head == data.end - 1) ? LLONG_MIN : data.gas + g - w;

      if (data.nextGas >= 0)
      {
        data.gas = data.nextGas;
        data.head->gf = gf = 0;
        data.nextGas = 0;
      }
      else if (data.gifts >= -data.nextGas)
      {
        data.head->gf = gf = -data.nextGas;
        data.gifts += data.nextGas;
        data.gas = 0;
        data.nextGas = 0;
      }
      else
        break;

      backCount(data.head, data);
      data.head++;

      backCheck(data.head, data.gifts, data);
    }

    printData(data);

    // ==============
    // PULL data.tail LOOP
    // ==============
    while (data.tail < data.head)
    {
      // если всего исходного кол-ва подарков недостаточно для преодоления текущей дороги
      // и подтягивать хвост более нет смысла, или все гифты собраны, то выходим
      if (data.k < -data.nextGas && data.head - data.tail + 1 <= data.maxL)
        break;

      if (data.head - data.tail + 1 <= data.maxL)
      {
        while (!data.tail->gf)
          data.tail++;
      }

      if (data.tail->gf)
      {
        data.gifts += data.tail->gf;
        data.tail->gf = 0;
        data.tail++;
        backCheckAll(data);

        printData(data);
      }
      else
      {
        data.tail++;

        long long gas_ = 0;
        bool gasChanged = true;

        // необходимо оптимизировать
        for (City * city = data.tail; city < data.head; city++)
        {
          gas_ += city->g + city->gf - city->w;

          if (gas_ <= 0 && city->gf)
          {
            data.gifts += gas_;
            city->gf -= gas_;
            gas_ = 0;
            gasChanged = false;
            break;
          }
          else if (gas_ < 0)
          {
            data.gifts += gas_;
            city->gf = -gas_;
            gas_ = 0;
          }
        }

        if (gasChanged)
          data.gas = gas_;

        if(data.gifts < 0)
          printData(data);

        // возвращаем голову назад, компенсируя отрицательное кол-во подарков
        while (data.gifts < 0)
        {
          data.head--;
          data.gas -= data.head->g + data.head->gf - data.head->w;
          data.gifts += data.head->gf;
          data.head->gf = 0;
          data.head->bg = 0;
          data.head->minbg = 0;
        }

        data.nextGas = data.gas + data.head->g - data.head->w;

        if (data.head <= data.firstMinBackwardGasCity)
        {
          printData(data);
          // если голова вернулась раньше первого города, с минимальным количеством топлива на обратном пути, то далее 
          // необходим пересчет для каждого города city->bg и city->minbg, а также backwardGas и minBackwardGas
          backRecountAll(data);
          backCheckAll(data);
        }
        else if (data.head <= data.lastMinBackwardGasCity)
          while (data.lastMinBackwardGasCity->bg != data.lastMinBackwardGasCity->minbg)
            data.lastMinBackwardGasCity--;

        printData(data);
      }

      if (data.tail == data.head)
        break;

      if (data.tail > data.lastMinBackwardGasCity)
      {
        // если хвост покинул последний город, имеющий минимальное количество топлива на обратном пути, то далее 
        // необходим пересчет для каждого города city->bg и city->minbg, а также backwardGas и minBackwardGas
        backRecountAll(data);
        backCheckAll(data);
        printData(data);
      }
      else if (data.tail > data.firstMinBackwardGasCity)
        while (data.firstMinBackwardGasCity->bg != data.firstMinBackwardGasCity->minbg)
          data.firstMinBackwardGasCity++;

      printData(data);

      if (data.head == data.end - 1)
      {
        if (data.maxL >= data.head - data.tail + 1)
          break;
      }
      else if (data.gifts >= -data.nextGas)
      {
        data.head->gf = -data.nextGas;
        data.gifts -= -data.nextGas;
        data.gas = 0;
        data.nextGas = 0;
        backCount(data.head, data);
        printData(data);
        data.head++;
        backCheck(data.head, data.gifts, data);
        break;
      }
    }

    if (data.k < -data.nextGas && data.head - data.tail + 1 <= data.maxL)
    {
      data.head++;
      data.tail = data.head;
    }

    if (data.tail == data.head)
    {
      data.tail = data.head;
      data.firstMinBackwardGasCity = data.head;
      data.lastMinBackwardGasCity = data.head;
      data.gifts = data.k;
      data.gas = 0;
      data.nextGas = 0;
      printData(data);
    }

    if (data.maxL >= data.end - data.tail)
    {
      printData(data);
      break;
    }
  }

  return { data.start, data.finish };
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
  data.init();

  if (data.n < 10000)
  {
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
  }

  Result result = testData(data);

  if (data.n < 10000)
  {
    if (trueLen != result.finish - result.start + 1)
    {
      std::cout << "Seed: " << seed;
      std::cout << " Control result: " << trueLen << "(" << trueStart << "-" << trueStart + trueLen - 1 << ") ";
      std::cout << " ERROR: " << result.finish - result.start + 1 << "(" << result.start - data.cities << "-" << result.finish - data.cities << ") ";
      getchar();
    }
  }

  return result.length();
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
  /*
  srand(9873);

  for (seed = 0; seed < 10000; seed++)
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
      long long a = rand() % 100;// % 1000000000;
      long long b = 1;// rand() % 3000;
      oss << a * b << " ";
    }

    for (int i = 0; i < n; i++)
    {
      long long a = rand() % 50;// % 1000000000;
      long long b = 1;// rand() % 2870;
      long long c = 1;// rand() % 4;
      oss << a * b * c << " ";
    }

    iss.str(oss.str());
    std::cin.rdbuf(iss.rdbuf());

    int len = test();
  }
  std::cout << "\r\n";
  */

  srand(2389);

  int n = 100000;
  int k = 1000000000;
  std::stringstream oss;
  oss << n << " " << k << " ";

  for (int i = 0; i < n - 1; i++)
  {
    long long a = i * 250;
    long long b = 1;// rand() % 3;
    oss << a * b << " ";
  }

  for (int i = 0; i < n; i++)
  {
    long long a = 10000000 - i * 100;
    long long b = 1;// rand() % 100;
    long long c = 1;// rand() % 4;
    oss << a * b * c << " ";
  }

  iss.str(oss.str());
  std::cin.rdbuf(iss.rdbuf());

  clock_t t = clock();
  std::cout << "Control result: 30963\r\n";
  std::cout << "Control time: 3365\r\n";
  std::cout << "Test result: " << test() << "\r\n";
  t = clock() - t;
  std::cout << "Time: " << t << " ms";


  std::cout << "\r\nPress 'Enter' to exit\r\n";
  getchar();
}

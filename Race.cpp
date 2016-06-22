#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string.h>

#undef LLONG_MIN 
#undef LLONG_MAX 
#define LLONG_MIN (-9223372036854775807LL - 1)
#define LLONG_MAX 9223372036854775807LL
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define MAX_CITY_COUNT 1000000

#define DEBUG_MODE

#ifdef DEBUG_MODE

struct Test
{
  const char * in;
  int out;
};

int trueLen;
int trueStart;
int seed = 0;

#endif

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

    std::streambuf * sbuf = std::cin.rdbuf();

    auto getCinValue = [=]() -> long long
    {
      long long retVal = 0;
      int ch = ' ';

      while (ch == ' ' || ch == '\r' || ch == '\n')
        ch = sbuf->sbumpc();

      while (ch >= '0' && ch <= '9')
      {
        retVal = retVal * 10 + ch - '0';
        ch = sbuf->sbumpc();
      }

      return retVal;
    };

    n = (int)getCinValue();
    k = (int)getCinValue();

    for (int i = 0; i < n - 1; i++)
      cities[i].w = getCinValue();

    for (int i = 0; i < n; i++)
      cities[i].g = getCinValue();

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

#ifdef DEBUG_MODE

bool testInterval(int start, int finish, Data & data)
{
  bool retVal = false;
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

#endif

// count city's bg and minbg from previous city values
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

// recount bg and minbg values for all cities from the tail to the head
void backRecountAll(Data & data)
{
  for (City * city = data.tail; city < data.head; city++)
    backCount(city, data);

#ifdef DEBUG_MODE
  printData(data);
#endif
};

// check return possibility from the city to the tail city
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

// check return possibility to the tail city from all cities between the tail and head
long long backCheckAll(Data & data)
{
#ifdef DEBUG_MODE
  printData(data);
#endif

  long long gifts_ = data.gifts;
  long long minGiftsDeficit = LLONG_MAX;

  for (City * city = data.head; city > max(data.tail, data.tail + data.maxL - 1); city--)
  {
    gifts_ += city->gf;

    long long giftsLeft = gifts_ + city->g - (city - 1)->w - ((city - 1)->bg - (city - 1)->minbg);

    if (giftsLeft >= 0)
    {
      data.maxL = city - data.tail + 1;
      data.start = data.tail;
      data.finish = city;
      break;
    }
    else
      minGiftsDeficit = min(minGiftsDeficit, -giftsLeft);
  }

  return minGiftsDeficit;
};


Result testData(Data & data)
{
  while (data.end - data.tail > data.maxL)
  {
    if (data.tail == data.head)
    {
      data.firstMinBackwardGasCity = data.head;
      data.lastMinBackwardGasCity = data.head;
      data.gifts = data.k;
      data.gas = 0;
      data.nextGas = 0;

#ifdef DEBUG_MODE
      printData(data);
#endif
    }

#ifdef DEBUG_MODE
    printData(data);
#endif

    // head pushing loop
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

#ifdef DEBUG_MODE
    printData(data);
#endif

    // tail pulling loop
    long long minGiftsDeficit = 0;

    while (data.tail < data.head)
    {
      // if all initial gifts number is insufficient to push head to the next city and 
      // no sense to pull the tail, because we already have longer run, then exit loop
      if (data.k < -data.nextGas && data.head - data.tail + 1 <= data.maxL)
        break;

      if (data.head - data.tail + 1 <= data.maxL)
      {
        while (!data.tail->gf && data.tail < data.head)
          data.tail++;
      }

      while (!data.tail->gf && data.tail->w <= (data.tail + 1)->g && data.tail < data.head)
        data.tail++;

      if (long long tailGifts = data.tail->gf)
      {
        data.gifts += tailGifts;
        data.tail->gf = 0;
        data.tail++;
        minGiftsDeficit -= tailGifts;

        if(minGiftsDeficit <= 0)
          minGiftsDeficit = backCheckAll(data);

#ifdef DEBUG_MODE
        printData(data);
#endif
      }
      else
      {
        data.tail++;
        long long gas_ = 0;
        bool gasChanged = true;

        // TODO : bottleneck, have to optimize
        // recount gifts and gas left
        // gifts number may becomes negative
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

#ifdef DEBUG_MODE
        if(data.gifts < 0)
          printData(data);
#endif

        // pull back head to compensate negative gifts number
        while (data.gifts < 0)
        {
          data.head--;
          data.gas -= data.head->g + data.head->gf - data.head->w;
          data.gifts += data.head->gf;
          data.head->gf = 0;
          data.head->bg = 0;
          data.head->minbg = 0;
          minGiftsDeficit = 0;
        }

        data.nextGas = data.gas + data.head->g - data.head->w;

        // if head returns before the first city with (city->bg == city->minbg) then have to recount 
        // all city->bg and city->minbg values for all cities between tail and head
        // else if head returns before the last city with (city->bg == city->minbg) then have to find
        // new lastMinBackwardGasCity
        if (data.head <= data.firstMinBackwardGasCity)
        {
#ifdef DEBUG_MODE
          printData(data);
#endif
          backRecountAll(data);
          minGiftsDeficit = backCheckAll(data);
        }
        else if (data.head <= data.lastMinBackwardGasCity)
          while (data.lastMinBackwardGasCity->bg != data.lastMinBackwardGasCity->minbg)
            data.lastMinBackwardGasCity--;

#ifdef DEBUG_MODE
        printData(data);
#endif
      }

      if (data.tail == data.head)
        break;

      // if tail passed lastMinBackwardGasCity, then have to recount city->bg è city->minbg for all cities 
      // between the tail and head
      if (data.tail > data.lastMinBackwardGasCity)
      {
        backRecountAll(data);
        minGiftsDeficit = backCheckAll(data);

#ifdef DEBUG_MODE
        printData(data);
#endif
      }
      else if (data.tail > data.firstMinBackwardGasCity)
        while (data.firstMinBackwardGasCity->bg != data.firstMinBackwardGasCity->minbg)
          data.firstMinBackwardGasCity++;

#ifdef DEBUG_MODE
      printData(data);
#endif

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

#ifdef DEBUG_MODE
        printData(data);
#endif
        data.head++;
        backCheck(data.head, data.gifts, data);
        break;
      }
    }

    if (data.k < -data.nextGas && data.head - data.tail + 1 <= data.maxL)
      data.tail = ++data.head;
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

#ifdef DEBUG_MODE

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

#endif

  Result result = testData(data);

#ifdef DEBUG_MODE

  if (data.n < 10000)
  {
    if (trueLen != result.finish - result.start + 1)
    {
      std::cout << "\rSeed: " << seed;
      std::cout << " Control result: " << trueLen << "(" << trueStart << "-" << trueStart + trueLen - 1 << ") ";
      std::cout << " ERROR: " << result.finish - result.start + 1 << "(" << result.start - data.cities << "-" << result.finish - data.cities << ") ";
      getchar();
    }
  }

#endif

  return result.length();
}

int main()
{
#ifdef DEBUG_MODE

  std::istringstream iss;

  static Test tests[] =
  {
    {
      "2 0 "
      "2 "
      "1 1 ", 1
    },
    {
      "3 1 "
      "2 2 "
      "1 1 1 ", 1
    },
    {
      "3 0 "
      "1 2 "
      "1 1 1 ", 2    },    {      "4 4 "
      "2 2 2 "
      "1 1 1 1 ", 4
    },
    {
      "8 5 "
      "2 2 2 3 7 3 1 "
      "1 3 1 5 4 0 2 5 ", 7
    },
    {
      "15 10 "
      "4 1 30 23 32 37 26 18 41 38 6 38 23 40 "
      "5 7 6 7 8 15 1 11 16 7 3 20 20 9 1 ", 3
    },
    {
      "20 100 "
      "2 28 35 16 40 25 50 36 35 47 25 5 34 26 22 49 26 7 10 "
      "12 12 13 8 8 19 18 9 0 18 13 20 9 10 0 7 5 6 15 2 ", 7
    },
    {
      "50 100 "
      "21 49 12 29 5 28 1 18 8 32 9 46 36 40 10 44 9 12 22 34 45 24 14 39 34 29 50 36 31 7 14 7 33 16 20 44 36 38 21 1 36 6 26 9 39 18 40 32 49 "
      "16 8 1 19 3 1 16 13 11 15 18 16 16 7 15 12 9 14 20 4 6 13 2 13 2 5 20 16 12 4 3 14 20 20 3 19 15 13 10 6 4 7 0 8 4 5 9 12 1 16 ", 10
    }
  };

  for (int t = 0; t < sizeof(tests) / sizeof(tests[0]); t++)
  {
    std::cout << "Test " << t << ": ";
    iss.str(tests[t].in);
    std::cin.rdbuf(iss.rdbuf());
    std::cout << (test() == tests[t].out ? "complete" : "failed") << "\r\n";
  }

  std::cout << "\r\n";
  std::cout << "Main test:\r\n";
  std::cout << "City count: 100 000\r\n";
  std::cout << "Gift count: 1 000 000 000\r\n";
  std::cout << "Control result: 44464\r\n";
  std::cout << "Control time(release): 1215ms\r\n";

  srand(2389);
  int n = 100000;
  int k = 1000000000;
  std::stringstream oss;
  oss << n << " " << k << " ";

  for (int i = 0; i < n - 1; i++)
  {
    unsigned int range = (unsigned int)(70000 * (0.5f + 0.5f * sin(i / 10000.0f)));
    oss << range << " ";
  }

  for (int i = 0; i < n; i++)
  {
    unsigned int fuel = 0;// rand() % 100;
    oss << fuel << " ";
  }

  iss.str(oss.str());
  std::cin.rdbuf(iss.rdbuf());

  clock_t t = clock();
  std::cout << "Test result: " << test() << "\r\n";
  t = clock() - t;
  std::cout << "Time: " << t << " ms\r\n";


  std::cout << "\r\nPress 'Enter' to exit\r\n";
  getchar();

#else

  std::cout << test();

#endif
}

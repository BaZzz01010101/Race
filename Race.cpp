#include <sstream>
#include <iostream>
#include <vector>

long long testInterval(int start, int finish, long long gifts, const long long * g, const long long * w)
{
  bool retVal = false;
  static long long g_[100000];
  long long gas = 0;
  int i;

  for (i = start; i < finish; i++)
  {
    gas += g[i] - w[i];

    if (gas >= 0)
      g_[i] = 0;
    else if (gifts >= -gas)
    {
      g_[i] = -gas;
      gifts -= -gas;
      gas = 0;
    }
    else
      return -1;
  }

  if (i == finish)
  {
    g_[finish] = gifts;

    gas = 0;

    for (i = finish; i > start; i--)
    {
      gas += g[i] + g_[i] - w[i - 1];

      if (gas < 0)
        return -1;
    }

    return gifts;
  }

  return -1;
}

long long lengthAtPos(int start, int maxFinish, long long gifts, const long long * g, const long long * w)
{
  bool retVal = false;
  static long long g_[100000];
  long long gas = 0;
  long long backwardGas = 0;
  long long maxGasDeficit = 0;
  int i;

  for (i = start; i < maxFinish; i++)
  {
    gas += g[i] - w[i];
    if (gas >= 0)
      g_[i] = 0;
    else if (gifts >= -gas)
    {
      g_[i] = -gas;
      gifts -= -gas;
      gas = 0;
    }
    else
      return -1;

    if (i > start)
    {
      backwardGas += g[i] + g_[i] - w[i - 1];

      if (backwardGas < -maxGasDeficit)
        maxGasDeficit = -backwardGas;
    }
  }

  if (i == maxFinish)
  {
    g_[i] = gifts;

    gas = 0;

    for (; i > start; i--)
    {
      gas += g[i] + g_[i] - w[i - 1];

      if (gas < 0)
        return -1;
    }

    return gifts;
  }

  return -1;
}

int testAll()
{
  int maxL = 1;
  int n, k;
  static long long w[100000];
  static long long g[100000];

  static long long g_[100000];
  static long long forward_gas_left[100000];
  static long long forward_gift_spent[100000];
  static long long backward_gas_left[100000];
  static long long backward_gift_spent[100000];

  std::cin >> n >> k;

  for (int i = 0; i < n - 1; i++)
    std::cin >> w[i];

  for (int i = 0; i < n; i++)
    std::cin >> g[i];


  //for (int l = n; l > 0; l--)
  //{
  //  for (int start = 0; start <= n - l; start++)
  //  {
  //    int finish = start + l - 1;

  //    if (testInterval(start, finish, k, g, w) >= 0)
  //    {
  //      std::cout << start << " " << finish << "\r\n";
  //      std::cout << finish - start + 1 << "\r\n";
  //      goto cont;
  //      return finish - start + 1;
  //    }
  //  }
  //}
  //cont:

  forward_gas_left[0] = 0;
  long long gas = 0;
  long long gift_spent = 0;

  for (int i = 0; i < n - 1; i++)
  {
    gas += g[i] - w[i];

    if (gas < 0)
    {
      g_[i] = -gas;
      gift_spent += -gas;
      gas = 0;
    }

    forward_gas_left[i + 1] = gas;
    forward_gift_spent[i + 1] = gift_spent;
  }

  backward_gas_left[n - 1] = 0;
  gas = 0;
  gift_spent = 0;

  for (int i = n - 1; i > 0; i--)
  {
    gas += g[i] + g_[i] - w[i - 1];

    if (gas < 0)
    {
      gift_spent -= gas;
      gas = 0;
    }

    backward_gas_left[i - 1] = gas;
    backward_gift_spent[i - 1] = gift_spent;
  }

  //#ifdef _DEBUG
  //  for (int i = 0; i < n - 1; i++)
  //    std::cout << g[i] << " (-" << w[i] << ") ";
  //
  //  std::cout << g[n - 1] << "\r\n";
  //
  //  for (int i = 0; i < n; i++)
  //    std::cout << forward_gas_left[i] << "(+" << forward_gift_spent[i] << ")  ";
  //
  //  std::cout << "\r\n";
  //  std::cout << "\r\n";
  //
  //  for (int i = 0; i < n - 1; i++)
  //    std::cout << g[i] + g_[i] << " (-" << w[i] << ") ";
  //
  //  std::cout << g[n - 1] << "\r\n";
  //
  //  for (int i = 0; i < n; i++)
  //    std::cout << backward_gas_left[i] << "(+" << backward_gift_spent[i] << ")  ";
  //
  //  std::cout << "\r\n";
  //#endif

  int addCounter = 10;
  int l = 2 * n;
  int step = n;
  long long maxGiftLeft = -1;

  while (1)
  {
    l += (maxGiftLeft >= 0) ? step : -step;

    if (l + step < maxL)
      break;

    maxGiftLeft = -1;

    for (int start = 0; start <= n - l; start++)
    {
      int finish = start + l - 1;
      long long giftLeft = -1;
      int savedStart = start;

      if (start == 2)
      {
        int kk = 0;
      }

      //while (start > 0 && g[start - 1] > w[start - 1])
      //  start--;

      //while (finish < n - 1 && g[finish + 1] > w[finish + 1])
      //  finish++;

      //if ((forward_gift_spent[finish] - forward_gift_spent[start]/* + forward_gas_left[start]*//*) +
      //  (backward_gift_spent[start] - backward_gift_spent[finish]/* + backward_gas_left[finish]*/) <= k)
      {
        giftLeft = testInterval(start, finish, k, g, w);

      }

      if (giftLeft >= 0 && finish - start + 1 > maxL)
      {
        maxL = finish - start + 1;
      }

      start = savedStart;

      if (giftLeft > maxGiftLeft)
        maxGiftLeft = giftLeft;

      if (maxGiftLeft >= 0)
        break;
    }

    if (step == 1)
    {
      if (!addCounter--)
        break;
    }

    step = step / 2 + step % 2;
  }

  return maxL;
/*
  for (; l > 0; l--)
  {
    for (int start = 0; start < n - l; start++)
    {
      int finish = start + l - 1;

      if ((forward_gift_spent[finish] - forward_gift_spent[start] + forward_gas_left[start]) +
        (backward_gift_spent[start] - backward_gift_spent[finish] + backward_gas_left[finish]) > k)
      {
        continue;
      }

      if (testInterval(start, finish, k, g, w) >= 0)
      {
        std::cout << finish - start + 1;
        getchar();
        return 0;
      }
    }
  }

  std::cout << 1;
  getchar();
  return 0;
  */
}

int main()
{
  int test = 50;
  std::istringstream iss;

  switch(test)
  {
    case 2:
      // 1
      iss.str("2 0\r\n"
            "2\r\n"
            "1 1\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 3:
      // 1
      iss.str("3 1\r\n"
              "2 2\r\n"
              "1 1 1\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 4:
      // 4
      iss.str("4 4\r\n"
              "2 2 2\r\n"
              "1 1 1 1\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 8:
      // 7
      iss.str("8 5\r\n"
              "2 2 2 3 7 3 1\r\n"
              "1 3 1 5 4 0 2 5\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 15:
      // 3
      iss.str("15 10\r\n"
              "4 1 30 23 32 37 26 18 41 38 6 38 23 40\r\n"
              "5 7 6 7 8 15 1 11 16 7 3 20 20 9 1\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 20:
      // 7
      iss.str("20 100\r\n"
              "2 28 35 16 40 25 50 36 35 47 25 5 34 26 22 49 26 7 10\r\n"
              "12 12 13 8 8 19 18 9 0 18 13 20 9 10 0 7 5 6 15 2\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 50:
      // 10
      iss.str("50 100\r\n"
              "21 49 12 29 5 28 1 18 8 32 9 46 36 40 10 44 9 12 22 34 45 24 14 39 34 29 50 36 31 7 14 7 33 16 20 44 36 38 21 1 36 6 26 9 39 18 40 32 49\r\n"
              "16 8 1 19 3 1 16 13 11 15 18 16 16 7 15 12 9 14 20 4 6 13 2 13 2 5 20 16 12 4 3 14 20 20 3 19 15 13 10 6 4 7 0 8 4 5 9 12 1 16\r\n");
      std::cin.rdbuf(iss.rdbuf());
      break;

    case 10000:
      
      srand(9873);

      int n = 10000;
      int k = 100000000;
      std::stringstream oss;
      oss << n << " " << k << "\r\n";

      for (int i = 0; i < n - 1; i++)
      {
        long long a = rand();
        long long b = rand() % 30000;
        oss << a * b << (i < n - 2 ? " " : "\r\n");
      }
      
      for (int i = 0; i < n; i++)
      {
        long long a = rand();
        long long b = rand() % 28700;
        long long c = 1;// rand() % 4;
        oss << a * b * c << (i < n - 1 ? " " : "\r\n");
      }

      iss.str(oss.str());
      std::cin.rdbuf(iss.rdbuf());
      break;
  }

  std::cout << testAll();
  getchar();
}

/*
int main()
{
  // 2
  //std::istringstream iss("2 0\r\n"
  //                       "2\r\n"
  //                       "1 1\r\n");

  // 4
  //std::istringstream iss("4 4\r\n"
  //                      "2 2 2\r\n"
  //                      "1 1 1 1\r\n");

  // 7
  //std::istringstream iss("8 5\r\n"
  //                       "2 2 2 3 7 3 1\r\n"
  //                       "1 3 1 5 4 0 2 5\r\n");

  //std::istringstream iss("10 5\r\n"
  //                       "1 2 3 4 5 6 7 8 9\r\n"
  //                       "0 1 2 3 4 5 6 7 8 9\r\n");

  std::istringstream iss("10 5\r\n"
                         "9 8 7 6 5 4 3 2 1\r\n"
                         "9 8 7 6 5 4 3 2 1 0\r\n");

  //std::istringstream iss("32 7\r\n"
  //                        "2 2 2 3 7 3 1 2 2 2 3 7 3 1 4 2 2 2 3 7 3 1 2 2 2 3 7 3 1 4 2\r\n"
  //                        "1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5\r\n");
  std::cin.rdbuf(iss.rdbuf());

  int maxL = 1;
  int n, k;
  std::cin >> n >> k;
  std::vector<int> w(n - 1);
  std::vector<int> g(n);
  std::vector<int> g_(n);

  for (int i = 0; i < n - 1; i++)
    std::cin >> w[i];

  for (int i = 0; i < n; i++)
    std::cin >> g[i];

  for (int l = n; l > 0; l--)
  {
    for (int start = 0; start < n - l; start++)
    {
      if (l + 1 > maxL)
      {
        int finish = start + l;
        int gifts = k;
        int gas = 0;
        int i;

        for (i = start; i < finish; i++)
        {
          gas += g[i] - w[i];

          if (gas >= 0)
            g_[i] = 0;
          else if (gifts >= -gas)
          {
            g_[i] = -gas;
            gifts -= -gas;
            gas = 0;
          }
          else
            break;
        }

        if (i == finish)
        {
          g_[finish] = gifts;

          gas = 0;

          for (i = finish; i > start; i--)
          {
            gas += g[i] + g_[i] - w[i - 1];

            if (gas < 0)
              break;
          }

          if (i == start && l + 1 > maxL)
            maxL = l + 1;
        }
      }
    }
  }


  std::cout << maxL;
  getchar();
}
*/

/*
int n, k;
std::vector<int> w;
std::vector<int> g;
int maxL = 1;

void step(int start, int finish, int i, int gas, int gifts, std::vector<int> & g)
{
  if (finish < 0)
  {
    if (i < n - 1 && gas + g[i] + gifts >= w[i])
    {
      gas += g[i];
      int addGas = w[i] - gas;

      if (addGas > 0)
      {
        std::vector<int> g_ = g;
        std::cout << ">" << i << " g[i]=" << g[i] << " gas=" << gas << " : add: " << addGas << "\r\n";
        g_[i] += addGas;
        step(start, finish, i + 1, 0, gifts - addGas, g_);
      }
      else
        step(start, finish, i + 1, gas - w[i], gifts, g);
    }

    if (i - start + 1 > maxL)
    {
      std::cout << "finish: " << i << "\r\n";
      return step(start, i, i, 0, gifts, g);
    }
  }
  else if (i == start)
  {
    std::cout << start << " " << finish << "\r\n";

    for (int j = 0; j < n; j++)
      std::cout << g[j] << " - ";

    std::cout << "\r\n";

    if (finish - start + 1 > maxL)
      maxL = finish - start + 1;
  }
  else
  {
    if (gas + g[i] + gifts >= w[i - 1])
    {
      gas += g[i];
      int addGas = w[i - 1] - gas;

      if (addGas > 0)
      {
        std::vector<int> g_ = g;
        std::cout << "<" << i << " g[i]=" << g[i] << " gas=" << gas << " : add: " << addGas << "\r\n";
        g_[i] += addGas;
        gifts -= addGas;
        return step(start, finish, i - 1, 0, gifts, g_);
      }
      else
        return step(start, finish, i - 1, gas - w[i - 1], gifts, g);
    }
    else
      std::cout << "<" << i << " no gas\r\n";
  }
}

int main()
{
  //std::istringstream iss("4 4\r\n"
  //                        "2 2 2\r\n"
  //                        "1 1 1 1\r\n");
  std::istringstream iss("8 5\r\n"
                         "2 2 2 3 7 3 1\r\n"
                         "1 3 1 5 4 0 2 5\r\n");
  //std::istringstream iss("32 7\r\n"
  //                        "2 2 2 3 7 3 1 2 2 2 3 7 3 1 4 2 2 2 3 7 3 1 2 2 2 3 7 3 1 4 2\r\n"
  //                        "1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5 1 3 1 5 4 0 2 5\r\n");
  std::cin.rdbuf(iss.rdbuf());
  
  std::cin >> n >> k;
  w.resize(n - 1);
  g.resize(n);

  for (int i = 0; i < n - 1; i++)
    std::cin >> w[i];

  for (int i = 0; i < n; i++)
    std::cin >> g[i];

  for (int i = 0; i < n - 1; i++)
  {
    if (n - i + 1 > maxL)
      step(i, -1, i, 0, k, g);
    else
      std::cout << "no test\r\n";
  }

  std::cout << "maxL = " << maxL;
  getchar();
}
*/